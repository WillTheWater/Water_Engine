// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/AudioTypes.h"
#include "Interface/IAssetDirector.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace we
{
    // =============================================================================
    // ASSET STATE TRACKING
    // =============================================================================

    /*
     * Lifecycle states for an asynchronous asset load.
     * Transitions: Loading -> (Ready | Failed)
     */
    enum class AssetState : uint8 { Loading, Ready, Failed };

    /*
     * Priority levels for the background loader thread queue.
     * Higher priority assets are loaded first when multiple requests are pending.
     */
    enum class AssetPriority : uint8 { Low, Normal, High };

    // =============================================================================
    // ASSET HANDLE - Thread-Safe Reference to Loading Asset
    // =============================================================================

    /*
     * Handle to an asset that may be loading asynchronously.
     *
     * THREAD SAFETY:
     * - Uses shared internal state so all copies of a handle see the same loading status
     * - Main thread queries state via IsReady()/IsValid()
     * - Background loader thread updates state atomically via shared pointer
     *
     * USAGE:
     * - Store handle in class (e.g., AssetHandle<texture> LoadedTexture)
     * - Check IsReady() in Update() before using the asset
     * - Access asset via *handle, handle->, or handle.Get() only when ready
     */
    template<typename T>
    class AssetHandle
    {
    public:
        /* Creates handle in Loading state (default constructor) */
        AssetHandle()
            : InternalHandleState(std::make_shared<InternalState>())
        {
        }

        /* Creates handle already in Ready state (used for cache hits) */
        explicit AssetHandle(shared<T> InAsset)
            : InternalHandleState(std::make_shared<InternalState>())
        {
            InternalHandleState->Asset = std::move(InAsset);
            InternalHandleState->State = AssetState::Ready;
        }

        /* Returns true if load succeeded (Ready state) */
        bool IsValid()   const { return InternalHandleState->State != AssetState::Failed; }

        /* Returns true when asset is loaded and ready for use */
        bool IsReady()   const { return InternalHandleState->State == AssetState::Ready; }

        /* Returns true while load is in progress */
        bool IsLoading() const { return InternalHandleState->State == AssetState::Loading; }

        /*
         * Raw pointer access - safe to call anytime, returns null if not ready.
         */
        T* Get() const { return InternalHandleState->Asset.get(); }

        /* Arrow operator - ONLY use if IsReady() returns true */
        T* operator->() const { return InternalHandleState->Asset.get(); }

        /* Dereference - ONLY use if IsReady() returns true */
        T& operator*() const { return *InternalHandleState->Asset; }

        /* Boolean conversion - returns IsReady() */
        explicit operator bool() const { return IsReady(); }

    private:
        friend class ResourceSubsystem;

        /*
         * Shared internal state between all copies of this handle.
         * Allows the background thread to update status while main thread polls.
         */
        struct InternalState
        {
            shared<T> Asset;              // The actual loaded asset (null until ready)
            AssetState State = AssetState::Loading;  // Current lifecycle state
        };

        shared<InternalState> InternalHandleState;  // Shared_ptr enables thread-safe copies
    };

    // Forward declaration for music streaming
    class MusicMemoryStream;

    // =============================================================================
    // RESOURCE SUBSYSTEM - Central Asset Management
    // =============================================================================

    /*
     * Manages all asset loading with async/sync support and automatic caching.
     *
     * ARCHITECTURE:
     * - Single background thread performs file I/O (disk reads from .pak files)
     * - Main thread submits requests and polls completions via PollCompletedRequests()
     * - File I/O happens on background thread to avoid blocking gameplay
     * - GPU resource creation happens on main thread
     * - Weak reference caches allow automatic cleanup when assets are no longer used
     *
     * THREADING MODEL:
     * - PendingRequests: Protected by PendingMutex, accessed by both threads
     * - CompletedRequests: Protected by CompletedMutex, main thread drains it
     * - Condition variable wakes loader thread when work is available
     *
     * USAGE:
     * - Engine calls PollCompletedRequests() once per frame in main loop
     * - Game code uses LoadAsync() for non-blocking loads, checks IsReady()
     * - Game code uses LoadSync() for critical assets needed immediately
     */
    class ResourceSubsystem
    {
    public:
        /* Singleton accessor */
        static ResourceSubsystem& Get();

        ResourceSubsystem();
        ~ResourceSubsystem();

        /*
         * Sets the file system interface (e.g., PakDirectory for .pak files).
         * Must be called before any loading operations.
         */
        void SetAssetDirectory(shared<IAssetDirector> Directory);

        /*
         * Shuts down the background loader thread.
         * Called automatically on destruction.
         */
        void Shutdown();

        // -------------------------------------------------------------------------
        // ASYNC LOADING API
        // -------------------------------------------------------------------------

        /*
         * Starts loading a texture in the background.
         * Returns immediately with a handle in Loading state.
         * Check IsReady() later, call PollCompletedRequests() in engine loop.
         */
        AssetHandle<texture> LoadTextureAsync(const string& Path, AssetPriority Priority = AssetPriority::Normal);

        /* Starts loading a font asynchronously. Font data persists automatically. */
        AssetHandle<font>    LoadFontAsync(const string& Path, AssetPriority Priority = AssetPriority::Normal);

        /* Starts loading a sound buffer asynchronously. */
        AssetHandle<soundBuffer> LoadSoundAsync(const string& Path, AssetPriority Priority = AssetPriority::Normal);

        // -------------------------------------------------------------------------
        // SYNC LOADING API
        // -------------------------------------------------------------------------

        /*
         * Loads texture immediately, blocking until complete.
         * Use for critical startup assets (window icons, initial UI).
         */
        shared<texture> LoadTextureSync(const string& Path);

        /* Loads font immediately, blocking until complete. */
        shared<font>    LoadFontSync(const string& Path);

        /* Loads sound immediately, blocking until complete. */
        shared<soundBuffer> LoadSoundSync(const string& Path);

        /*
         * Loads music with persistent memory stream.
         * Music requires random access to file data during playback.
         */
        shared<music>   LoadMusic(const string& Path);

        /*
         * Loads shader from source code.
         * Currently uncached - creates new instance each call.
         */
        shared<shader>  LoadShader(const string& Path, shader::Type Type);

        /*
         * Returns a 1x1 white placeholder texture for sprites that need
         * to be constructed before their actual texture is loaded.
         * The texture is created on first call and cached.
         */
        shared<texture> GetPlaceholderTexture();

        // -------------------------------------------------------------------------
        // ENGINE INTERFACE
        // -------------------------------------------------------------------------

        /*
         * Processes completed async loads and finalizes them.
         * CALL THIS: Once per frame from the main engine loop.
         * This moves assets from "raw data loaded" to "GPU object created".
         */
        void PollCompletedRequests();

        /*
         * Cleans up expired cache entries.
         * CALL THIS: Periodically from engine (actual cleanup runs every 3 seconds).
         */
        void GarbageCycle(float DeltaTime);

    private:
        // =============================================================================
        // INTERNAL REQUEST SYSTEM
        // =============================================================================

        /*
         * Type-erased base for the request queues.
         * Allows mixing different asset types in single queue.
         */
        struct RequestBase
        {
            virtual ~RequestBase() = default;

            /* Called on background thread - performs file I/O */
            virtual void ExecuteLoad(shared<IAssetDirector> Assets) = 0;

            /* Called on main thread - creates GPU resources */
            virtual void ExecuteFinalize() = 0;

            /* Returns priority for queue sorting */
            virtual AssetPriority GetPriority() const = 0;

            string Path;                    // File path to load
            AssetPriority Priority;         // Loading priority
            bool bIsDone = false;             // Set true after ExecuteLoad completes
        };

        /*
         * Typed request containing asset-specific data.
         * Template parameter T is the asset type (texture, font, etc.).
         */
        template<typename T>
        struct Request : RequestBase
        {
            AssetHandle<T> Handle;          // Handle returned to caller (shared state)
            shared<T> Result;               // Temporary storage during finalization
            vector<uint8> Data;             // Raw file bytes loaded by thread
            std::function<bool(T&, const vector<uint8>&)> Loader;  // Parse function

            AssetPriority GetPriority() const override { return Priority; }

            /*
             * STEP 1: Background thread execution.
             * Reads raw file data from disk. No GPU operations here.
             */
            void ExecuteLoad(shared<IAssetDirector> Assets) override
            {
                if (Assets)
                    Assets->ReadFile(Path, Data);
            }

            /*
             * STEP 2: Main thread execution (via PollCompletedRequests).
             * Parses raw data and creates GPU-ready asset.
             * Updates handle state so caller sees Ready/Failed status.
             */
            void ExecuteFinalize() override
            {
                if (Data.empty())
                {
                    Handle.InternalHandleState->State = AssetState::Failed;
                    return;
                }

                // Create asset and parse the raw data
                auto asset = make_shared<T>();
                if (Loader(*asset, Data))
                {
                    Handle.InternalHandleState->Asset = asset;   // Make available
                    Handle.InternalHandleState->State = AssetState::Ready;
                }
                else
                {
                    Handle.InternalHandleState->State = AssetState::Failed;
                }
            }
        };

        // =============================================================================
        // BACKGROUND LOADER THREAD
        // =============================================================================

        /*
         * Background thread entry point.
         * Loop: Wait for work -> Pop highest priority request -> Load file -> Move to completed queue.
         */
        void LoaderThread();

        // =============================================================================
        // INTERNAL HELPERS
        // =============================================================================

        template<typename T>
        AssetHandle<T> LoadAsyncInternal(const string& Path, AssetPriority Priority,
            std::function<bool(T&, const vector<uint8>&)> Loader);

        template<typename T>
        shared<T> LoadSyncInternal(const string& Path,
            std::function<bool(T&, const vector<uint8>&)> Loader);

        /*
         * Checks weak cache for existing asset.
         * Returns shared_ptr if found and still alive, null otherwise.
         */
        template<typename T>
        shared<T> FindInCache(const string& Path, dictionary<string, weak<T>>& Cache)
        {
            auto it = Cache.find(Path);
            if (it != Cache.end())
                return it->second.lock();  // Convert weak to shared (may fail if expired)
            return nullptr;
        }

        /*
         * Stores asset in weak cache.
         * Does not keep asset alive - only tracks for reuse.
         */
        template<typename T>
        void AddToCache(const string& Path, shared<T> Asset, dictionary<string, weak<T>>& Cache)
        {
            Cache[Path] = Asset;
        }

    private:
        // =============================================================================
        // MEMBER VARIABLES
        // =============================================================================

        static ResourceSubsystem* RSubInstance; 

        shared<IAssetDirector> AssetDirector;      // File system abstraction (.pak access)

        // Threading primitives
        std::thread AssetThread;                   // Background loader thread
        std::mutex PendingMutex;                   // Protects PendingRequests queue
        std::mutex CompletedMutex;                 // Protects CompletedRequests queue
        std::condition_variable CV;                // Wakes thread when work available
        bool bIsRunning = true;                    // Thread shutdown signal

        // Priority comparator for the pending queue (higher priority = popped first)
        struct PriorityCompare
        {
            bool operator()(const unique<RequestBase>& a, const unique<RequestBase>& b) const
            {
                return static_cast<uint8>(a->GetPriority()) < static_cast<uint8>(b->GetPriority());
            }
        };

        // Request queues
        std::priority_queue<unique<RequestBase>, vector<unique<RequestBase>>, PriorityCompare> PendingRequests;
        vector<unique<RequestBase>> CompletedRequests;    // Loaded, awaiting finalization

        // Weak caches (assets kept alive by external shared_ptrs, not by cache)
        dictionary<string, weak<texture>> TextureCache;
        dictionary<string, weak<font>>    FontCache;
        dictionary<string, weak<soundBuffer>> SoundCache;

        // Persistent data storage (fonts/music need raw bytes to stay alive)
        dictionary<string, shared<vector<uint8>>> FontDataCache;      // Font raw data
        dictionary<string, shared<vector<uint8>>> MusicDataCache;     // Music raw data
        dictionary<string, shared<MusicMemoryStream>> MusicStreamCache; // Active streams

        float GarbageCycleTimer = 0.0f;            // Accumulates delta time for cleanup

        shared<texture> PlaceholderTexture;        // 1x1 white texture for default sprite construction
    };

    /* Global shorthand accessor: LoadAsset().LoadTextureAsync(...) */
    inline ResourceSubsystem& LoadAsset() { return ResourceSubsystem::Get(); }

    // =============================================================================
    // MUSIC MEMORY STREAM
    // =============================================================================

    /*
     * SFML InputStream implementation for in-memory music playback.
     *
     * WHY NEEDED:
     * sf::Music streams data from disk during playback to save memory.
     * For packed assets (.pak files), must provide a stream interface
     * that reads from memory instead of disk.
     *
     * LIFETIME:
     * The shared_ptr to data keeps the music bytes alive as long as
     * the MusicMemoryStream exists. MusicStreamCache in ResourceSubsystem
     * holds these streams for active music playback.
     */
    class MusicMemoryStream : public sf::InputStream
    {
    public:
        /* Takes shared ownership of the music data buffer */
        explicit MusicMemoryStream(shared<vector<uint8>> Data)
            : MusicMemoryData(std::move(Data)), MusicMemoryPosition(0) {
        }

        /* Read bytes from current position, returns bytes actually read */
        std::optional<std::size_t> read(void* data, std::size_t size) override
        {
            std::size_t available = MusicMemoryData->size() - MusicMemoryPosition;
            std::size_t toRead = (size < available) ? size : available;
            if (toRead > 0)
            {
                std::memcpy(data, MusicMemoryData->data() + MusicMemoryPosition, toRead);
                MusicMemoryPosition += toRead;
            }
            return toRead;
        }

        /* Seek to absolute position, clamped to data bounds */
        std::optional<std::size_t> seek(std::size_t position) override
        {
            MusicMemoryPosition = (position > MusicMemoryData->size()) ? MusicMemoryData->size() : position;
            return MusicMemoryPosition;
        }

        /* Returns current read position */
        std::optional<std::size_t> tell() override { return MusicMemoryPosition; }

        /* Returns total size of the music data */
        std::optional<std::size_t> getSize() override { return MusicMemoryData->size(); }

    private:
        shared<vector<uint8>> MusicMemoryData;
        std::size_t MusicMemoryPosition;
    };
}
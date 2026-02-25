# Water Engine v3 - Complete Redesign Plan

## Executive Summary

This document outlines a comprehensive redesign of the Water Engine to address critical architectural flaws that are limiting scalability, developer experience, and maintainability. The current engine (v2) suffers from subsystem pollution, convoluted asset loading, build system nightmares, and lack of real-time editing capabilities.

---

## Current Issues Summary

### 1. **JSON Configuration Over-Engineering** ❌
- **Problem**: Using JSON to parse file paths and asset addresses adds unnecessary complexity
- **Impact**: Build-time JSON embedding creates fragile dependencies, difficult to modify at runtime
- **Current**: `EngineConfigData.h` generated from JSON, hardcoded at compile time

### 2. **Pak File System for Debug Builds** ❌
- **Problem**: `.pak` files required for all builds including debug
- **Impact**: Slower iteration times, debugging nightmares, asset hot-reloading impossible
- **Current**: `PakDirectory` mounted at startup, all assets must be packed

### 3. **Render/Window Subsystem Spaghetti** ❌
- **Problem**: Tight coupling between Window, Render, GUI, and Cursor subsystems
- **Impact**: Unreadable code flow, difficult to modify, impossible to unit test
- **Current**: RenderSubsystem directly depends on WindowSubsystem, complex render target management

### 4. **No Hot-Reload Capability** ❌
- **Problem**: Assets and code changes require full restart
- **Impact**: Extremely slow development iteration
- **Current**: No asset watching, no code reloading

### 5. **Subsystem Pollution** ❌
- **Problem**: Too many subsystems with overlapping responsibilities
- **Impact**: Global state everywhere, unclear ownership, initialization order bugs
- **Current**: 14+ subsystems with complex interdependencies

### 6. **No Editor/Visual Tools** ❌
- **Problem**: Everything requires code changes
- **Impact**: Non-programmers cannot contribute, slow iteration
- **Current**: No editor, no runtime inspection

---

## Current Feature Inventory

### Core Systems (WaterEngine)

| Feature | Status | Notes |
|---------|--------|-------|
| Window Management | ✅ | Fullscreen, VSync, resize handling |
| Render System | ✅ | Multi-layer rendering (World, ScreenUI, WorldUI, Cursor) |
| Post-Processing | ✅ | Bloom, custom PPE support |
| Physics | ✅ | Box2D integration |
| Audio | ✅ | SFX and music playback |
| Input | ✅ | Keyboard, mouse, gamepad support |
| Asset Loading | ⚠️ | Async loading via thread pool, but pak-only |
| Save/Load | ✅ | Basic implementation |
| GUI System | ⚠️ | Custom widgets, but no editor integration |
| World Management | ✅ | Actor spawn/destroy, world switching |
| Camera | ✅ | Follow, smooth, multiple cameras |
| Cursor | ✅ | Custom cursor with gamepad support |

### Widget/GUI Features

| Widget | Status |
|--------|--------|
| Button | ✅ |
| Panel | ✅ |
| TextBlock | ✅ |
| CheckBox | ✅ |
| Slider | ✅ |
| ProgressBar | ✅ |
| VerticalBox | ✅ |
| HorizontalBox | ✅ |
| GridBox | ✅ |
| AutoPanel | ✅ |

### Actor Framework

| Feature | Status |
|---------|--------|
| Actor base class | ✅ |
| Character (with physics) | ✅ |
| Camera | ✅ |
| Border | ✅ |
| Animation Component | ✅ |
| Physics Component | ✅ |
| Camera Component | ✅ |

---

## Redesign Recommendations

### 1. **Configuration System - Replace JSON**

**Current:**
```cpp
// JSON embedded at compile time - terrible!
json Config = json::parse(EMBEDDED_ENGINE_CONFIG);
AssetDirectory = Config["AssetDirectory"];
```

**Proposed:**
```cpp
// Simple INI-style config files, runtime loadable
// EngineConfig.ini (plain text, hot-reloadable)
[Window]
Width = 1920
Height = 1080
Fullscreen = false

[Paths]
AssetDirectory = "Content"
DefaultFont = "Fonts/Default.ttf"
```

**Implementation:**
- Use simple INI or TOML format instead of JSON
- Hot-reloadable in debug builds
- Separate configs for Engine, Game, and Editor
- No compile-time code generation

---

### 2. **Asset System - Pak Files for Release Only**

**Current:**
```cpp
// Always uses pak files
auto PD = make_shared<PakDirectory>(EC.AssetDirectory);
Subsystem.AssetLoader->SetAssetDirectory(PD);
```

**Proposed:**
```cpp
// Debug: Direct file system access
// Release: Pak file access
#ifdef WE_RELEASE
    AssetLoader->MountPak("Content.pak");
#else
    AssetLoader->MountDirectory("Content/");
#endif
```

**Implementation:**
- `DirectoryAssetDirector` for debug builds (direct FS access)
- `PakAssetDirector` for release builds (packed)
- Asset watching for hot-reload in debug
- Async loading with dependency tracking

---

### 3. **Render System - Decouple & Simplify**

**Current Issues:**
- RenderSubsystem knows about WindowSubsystem
- Complex multi-target rendering
- Hardcoded render layers
- GUI mixed with world rendering

**Proposed Architecture:**
```cpp
// Clear separation of concerns
class RenderContext { /* Low-level rendering */ };
class Viewport { /* View/camera management */ };
class Renderer2D { /* 2D batch rendering */ };
class UIRenderer { /* IMGUI rendering */ };
class PostProcessStack { /* Post-processing */ };

// Window is just a platform surface
class Window {
    void Present(RenderTexture& frame);
};
```

**Key Changes:**
- Remove render-to-texture for world (render directly unless post-processing needed)
- Separate UI rendering from world rendering
- Use IMGUI for all UI (debug, editor, game UI)
- Simplified render pipeline

---

### 4. **IMGUI for Editor & Runtime UI**

**Why IMGUI:**
- Industry standard for game editors
- Immediate mode = simpler code
- Built-in widgets (sliders, color pickers, etc.)
- Easy to create tools
- Works with SFML (imgui-sfml)

**Editor Features Needed:**
- Scene hierarchy view
- Inspector (component editing)
- Asset browser
- Console/log viewer
- Play/Pause/Step controls
- Entity transform gizmos

**Game UI:**
- Use IMGUI for menus, HUD
- Or use retained UI library (consider Dear ImGui's retained mode or other)

---

### 5. **Hot Reload Architecture**

**Asset Hot Reload:**
```cpp
class AssetManager {
    void WatchForChanges();  // File system watcher
    void ReloadAsset(const Path& path);
    
    template<typename T>
    shared<T> Load(const Path& path, bool hotReload = true);
};
```

**Code Hot Reload (Advanced):**
- DLL-based module system
- Game logic in separate DLL
- Reload DLL without restarting engine
- State preservation across reloads

---

### 6. **Subsystem Cleanup - Modular Design**

**Current (14 subsystems):**
- Resource, Window, Render, Time, Cursor, Input, SaveLoad, World, GameState, Audio, GUI, Physics, Camera

**Proposed (Core + Modules):**

**Core Engine (Always Present):**
```cpp
class Engine {
    ModuleManager Modules;
    EventSystem Events;
    ConfigManager Config;
    
    void Initialize();
    void Run();
    void Shutdown();
};
```

**Modules (Loadable/Optional):**
```cpp
class IModule {
    virtual void OnLoad() = 0;
    virtual void OnUpdate(float dt) = 0;
    virtual void OnUnload() = 0;
};

// Example modules
class RenderModule : public IModule { };
class PhysicsModule : public IModule { };
class AudioModule : public IModule { };
class EditorModule : public IModule { };  // Editor-only
```

**Benefits:**
- Clear dependencies
- Optional modules
- Easier testing
- Editor can be a module

---

### 7. **World/Actor System Improvements**

**Current Issues:**
- Inheritance-heavy (Actor -> Character)
- No composition pattern
- Spawning is complex

**Proposed (ECS or Composition):**
```cpp
// Option A: ECS (Entity Component System)
EntityManager EM;
Entity player = EM.CreateEntity();
EM.AddComponent<Transform>(player, {0, 0});
EM.AddComponent<SpriteRenderer>(player, "player.png");
EM.AddComponent<PhysicsBody>(player, bodyDef);

// Option B: Composition (keep inheritance but improve)
class Actor {
    vector<unique<IComponent>> Components;
    
    template<typename T>
    T* AddComponent(Args... args);
    
    template<typename T>
    T* GetComponent();
};
```

**Recommendation:** Start with improved composition, migrate to ECS if needed later.

---

### 8. **Input System - Context-Based**

**Current:**
```cpp
// Global input, hard to manage
Subsystem.Input->Bind(ACTION_INTERACT, Input::Keyboard{ sf::Keyboard::Scan::E });
```

**Proposed:**
```cpp
// Context-based input
InputContext GameplayContext;
InputContext DialogContext;
InputContext MenuContext;

// Only active context receives input
InputManager.SetActiveContext(GameplayContext);

// Or stacked contexts
InputManager.PushContext(DialogContext);  // Dialog input takes priority
InputManager.PopContext();  // Back to gameplay
```

**Benefits:**
- E key can mean "Interact" in game, "Advance" in dialog, "Select" in menu
- No manual state checking
- Rebindable per context

---

## Implementation Steps

### Phase 1: Foundation (Weeks 1-2)

**Step 1.1: New CMake Structure**
```
WaterEngine/
├── CMakeLists.txt
├── Engine/           # Core engine library
│   ├── Core/
│   ├── Math/
│   └── Platform/
├── Modules/          # Optional modules
│   ├── RenderModule/
│   ├── PhysicsModule/
│   └── AudioModule/
├── Editor/           # Editor executable
└── Runtime/          # Game runtime executable
```

**Step 1.2: Basic INI Config System**
- Replace JSON config with simple INI parser
- Hot-reloadable in debug
- No compile-time generation

**Step 1.3: New Asset System (Debug Mode)**
- Implement `DirectoryAssetDirector`
- Direct file system access
- Asset file watching (for hot reload)
- Pak system disabled in debug

### Phase 2: Render System Refactor (Weeks 3-4)

**Step 2.1: Decouple Render from Window**
- Render to texture optional (only for post-processing)
- Direct rendering path for efficiency
- Clear render pipeline stages

**Step 2.2: IMGUI Integration**
- Add imgui-sfml
- Basic debug UI
- Console window

**Step 2.3: Editor Framework**
- Editor as separate target
- Docking window system
- Basic panels (hierarchy, inspector)

### Phase 3: Subsystem Cleanup (Weeks 5-6)

**Step 3.1: Module System**
- Define IModule interface
- Convert subsystems to modules
- Module dependency resolution

**Step 3.2: Input Context System**
- Context-based input
- Stack-based priority
- Better gamepad support

**Step 3.3: Actor System Cleanup**
- Composition over inheritance
- Better spawn/destroy
- Component reflection for editor

### Phase 4: Hot Reload & Polish (Weeks 7-8)

**Step 4.1: Asset Hot Reload**
- File watching
- Texture reloading
- Shader reloading

**Step 4.2: Editor Features**
- Scene editing
- Property editing
- Asset browser

**Step 4.3: Release Build System**
- Pak file generation
- Stripped debug code
- Optimized rendering

---

## Suggested Technology Choices

| Category | Current | Proposed | Notes |
|----------|---------|----------|-------|
| UI Library | Custom | **IMGUI** | Industry standard for editors |
| Config Format | JSON (embedded) | **INI/TOML** | Simple, hot-reloadable |
| Asset Packing | Always | **Debug: FS, Release: Pak** | Better iteration |
| Render API | SFML | **SFML + Custom** | Keep SFML, simplify pipeline |
| Physics | Box2D | **Box2D** | Keep, it's working |
| Audio | SFML Audio | **SFML Audio** | Keep, but maybe add FMOD option |
| Logging | spdlog | **spdlog** | Keep, excellent library |
| Reflection | None | **Custom macro-based** | For editor integration |

### GUI Library Comparison

| Library | Pros | Cons | Verdict |
|---------|------|------|---------|
| **IMGUI** | Fast, easy, great for tools | Not great for complex game UI | ✅ Use for Editor + Debug |
| **RmlUi** | HTML/CSS-like, retained | Heavy, complex | Consider for game UI |
| **Custom** | Full control | Time consuming | ❌ Don't rebuild |

**Recommendation:**
- Use **IMGUI** for Editor, Debug UI, and simple game UIs
- Keep existing custom widgets for complex game HUD if needed
- Or use IMGUI's retained mode extensions

---

## Architecture Diagram (Proposed)

```
┌─────────────────────────────────────────────────────────────┐
│                        APPLICATION                           │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │    Editor    │  │    Game      │  │   Server     │      │
│  │  (Optional)  │  │  (Runtime)   │  │  (Optional)  │      │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘      │
└─────────┼────────────────┼────────────────┼────────────────┘
          │                │                │
          └────────────────┴────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────┐
│                      ENGINE CORE                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────┐ │
│  │   Module    │  │    Event    │  │     Config      │ │
│  │   Manager   │  │    System   │  │    (INI/TOML)   │ │
│  └──────┬──────┘  └─────────────┘  └─────────────────┘ │
└─────────┼───────────────────────────────────────────────┘
          │
    ┌─────┴─────┬─────────────┬─────────────┬─────────────┐
    │           │             │             │             │
┌───▼───┐  ┌───▼────┐  ┌────▼────┐  ┌────▼────┐  ┌────▼────┐
│Render │  │Physics │  │  Audio  │  │  Input  │  │  World  │
│Module │  │Module  │  │ Module  │  │ Context │  │ Module  │
└───┬───┘  └────────┘  └─────────┘  └─────────┘  └─────────┘
    │
┌───▼─────────────────────────────────────────────────────┐
│                   PLATFORM LAYER (SFML)                  │
│         Window, Graphics, Audio, Input, Network          │
└───────────────────────────────────────────────────────────┘
```

---

## Naming Conventions (Keep Unreal Style)

Continue using current naming conventions:

```cpp
// Classes: PascalCase
class ActorComponent { };
class RenderSubsystem { };

// Variables: camelCase
float deltaTime;
string assetPath;

// Member variables: Prefix with m or use trailing underscore (choose one)
// Option A (Unreal style):
int32 m_health;
float m_movementSpeed;

// Option B (Google style):
int32 health_;
float movementSpeed_;

// Functions: PascalCase
void BeginPlay();
void Tick(float DeltaTime);
void SetPosition(const vec2f& Position);

// Constants/Enums: UPPER_SNAKE or kPascalCase
constexpr float MAX_HEALTH = 100.0f;
enum class ERenderLayer { World, UI };

// Macros: UPPER_SNAKE with WE_ prefix
#define WE_ASSERT(condition, message) ...
#define WE_LOG(category, level, message) ...
```

---

## Migration Strategy

### Gradual vs Complete Rewrite

**Recommendation:** Gradual migration with feature flags

1. **Create new branch** `v3-redesign`
2. **Keep v2 working** while building v3 components
3. **Migrate one subsystem at a time**
4. **Use feature flags** to switch between old/new

Example:
```cpp
#ifdef WE_V3_RENDER
    // New render system
    RenderModule->Initialize();
#else
    // Old render system
    Subsystem.Render = make_unique<RenderSubsystem>(...);
#endif
```

### Priority Order

1. **Config System** (easy win, immediate benefit)
2. **Asset System** (debug file access, hot reload)
3. **IMGUI Integration** (editor can start immediately)
4. **Render System** (biggest impact, most complex)
5. **Subsystem Cleanup** (after render is stable)
6. **Hot Reload** (final polish)

---

## File Structure (Proposed)

```
WaterEngine/
├── CMakeLists.txt
├── README.md
├── AGENTS.md
├── Config/
│   ├── EngineConfig.ini
│   └── EditorLayout.ini
├── Source/
│   ├── Core/                    # Always built
│   │   ├── Types.h
│   │   ├── Math.h
│   │   ├── Memory.h
│   │   ├── Log.h
│   │   └── Config.h
│   ├── Platform/               # Platform abstraction
│   │   ├── Window.h
│   │   ├── Input.h
│   │   └── FileSystem.h
│   ├── Engine/                 # Core engine
│   │   ├── Engine.h
│   │   ├── Module.h
│   │   ├── Event.h
│   │   └── Plugin.h
│   ├── Modules/                # Optional modules
│   │   ├── Render/
│   │   │   ├── RenderModule.h
│   │   │   ├── Renderer2D.h
│   │   │   ├── Viewport.h
│   │   │   └── PostProcess.h
│   │   ├── Physics/
│   │   │   ├── PhysicsModule.h
│   │   │   └── PhysicsWorld.h
│   │   ├── Audio/
│   │   │   └── AudioModule.h
│   │   ├── World/
│   │   │   ├── WorldModule.h
│   │   │   ├── Entity.h
│   │   │   └── Component.h
│   │   └── Editor/
│   │       ├── EditorModule.h
│   │       ├── EditorUI.h
│   │       └── Panels/
│   └── Asset/
│       ├── AssetManager.h
│       ├── AssetWatcher.h
│       ├── DirectorySource.h
│       └── PakSource.h
├── ThirdParty/
│   ├── CMakeLists.txt
│   └── ... (sfml, box2d, imgui, etc)
├── Editor/
│   ├── CMakeLists.txt
│   └── Source/
│       └── main.cpp
└── Runtime/
    ├── CMakeLists.txt
    └── Source/
        └── main.cpp
```

---

## Conclusion

This redesign addresses all major architectural issues:

1. ✅ **Simple INI configs** instead of embedded JSON
2. ✅ **Direct file access** in debug, pak only in release
3. ✅ **Decoupled render system** with IMGUI
4. ✅ **Hot reload** for assets and potentially code
5. ✅ **Modular subsystem** design with clear ownership
6. ✅ **Built-in editor** using IMGUI

The plan is ambitious but achievable in 6-8 weeks of focused development. The modular approach allows incremental delivery and testing throughout the process.

**Next Steps:**
1. Review and approve this plan
2. Create v3-redesign branch
3. Start with Phase 1 (CMake + Config + Asset)
4. Build IMGUI editor framework early for immediate benefit

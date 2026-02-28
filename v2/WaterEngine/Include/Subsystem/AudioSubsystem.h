// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/AudioTypes.h"
#include "Core/EngineConfig.h"
#include "Core/MusicMemoryStream.h"

namespace we
{
    // Forward declarations
    class ResourceSubsystem;

    // =========================================================================
    // Audio Subsystem - Bus-based mixing with streaming music support
    // =========================================================================
    class AudioSubsystem
    {
    public:
        // Initialize with config and resource subsystem for loading
        AudioSubsystem(const EngineConfig::AudioConfig& Config, ResourceSubsystem& Resources);
        ~AudioSubsystem();

        // Called once per frame
        void Update(float DeltaTime);

        // =====================================================================
        // Playback
        // =====================================================================
        void PlayMusic(const string& Path, const AudioPlaybackConfig& Config = {});
        void PlayAmbient(const string& Path, const AudioPlaybackConfig& Config = {});
        void PlaySFX(const string& Path, const AudioPlaybackConfig& Config = {});
        void PlayVoice(const string& Path, const AudioPlaybackConfig& Config = {});
        void PlayUI(const string& Path, const AudioPlaybackConfig& Config = {});

        // Crossfade to new music (fade out current, fade in new)
        void CrossfadeMusic(const string& Path, float FadeDuration, const AudioPlaybackConfig& Config = {});

        // =====================================================================
        // Stop
        // =====================================================================
        void StopMusic(float FadeDuration = 0.0f);
        void StopAmbient(float FadeDuration = 0.0f);
        void StopAllSFX();
        void StopAllVoice();
        void StopAll();

        // =====================================================================
        // Volume Control (0.0 - 1.0)
        // =====================================================================
        void SetMasterVolume(Volume Vol);
        Volume GetMasterVolume() const;

        void SetChannelVolume(AudioChannel Channel, Volume Vol);
        Volume GetChannelVolume(AudioChannel Channel) const;

        // =====================================================================
        // Mute
        // =====================================================================
        void SetMasterMuted(bool bMuted);
        bool IsMasterMuted() const;
        void SetChannelMuted(AudioChannel Channel, bool bMuted);
        bool IsChannelMuted(AudioChannel Channel) const;

        // =====================================================================
        // Pause (affects all except UI)
        // =====================================================================
        void SetPaused(bool bPaused);
        bool IsPaused() const;

        // =====================================================================
        // 3D Audio
        // =====================================================================
        void SetListenerPosition(vec2f Position);
        vec2f GetListenerPosition() const;
        void SetListenerDirection(vec2f Direction);
        vec2f GetListenerDirection() const;

        // =====================================================================
        // Queries
        // =====================================================================
        bool IsMusicPlaying() const;
        bool IsAmbientPlaying() const;
        size_t GetActiveSFXCount() const;
        size_t GetActiveVoiceCount() const;

    private:
        // Internal track structures
        struct MusicTrack
        {
            // Note: Order matters! Music must be destroyed before Stream
            unique<MusicMemoryStream> Stream;
            shared<vector<uint8>> DataBuffer;
            shared<music> Music;
            AudioPlaybackConfig Config;
            Volume CurrentVolume = 1.0f;
            Volume TargetVolume = 1.0f;
            float FadeSpeed = 0.0f;
        };

        struct SFXInstance
        {
            shared<soundBuffer> Buffer;
            unique<sound> Sound;
            AudioPlaybackConfig Config;
            Volume CurrentVolume = 1.0f;
        };

        // Internal helpers
        void UpdateFades(float DeltaTime);
        void UpdateSpatialAudio();
        void CleanupStoppedSounds();
        void ApplyVolumeToMusic(MusicTrack& Track);
        void ApplyVolumeToSFX(SFXInstance& Instance);
        Volume GetEffectiveChannelVolume(AudioChannel Channel) const;

    private:
        ResourceSubsystem& Resources;
        const EngineConfig::AudioConfig Config;
        
        // Additional audio settings (not in config)
        static constexpr size_t MaxVoiceInstances = 4;
        static constexpr float DistanceAttenuationFactor = 1.0f;

        // Playback state
        unique<MusicTrack> CurrentMusic;
        unique<MusicTrack> CurrentAmbient;
        vector<SFXInstance> ActiveSFX;
        vector<SFXInstance> ActiveVoice;

        // Volume state
        Volume MasterVolume = 1.0f;
        std::array<Volume, static_cast<size_t>(AudioChannel::Count)> ChannelVolumes;
        std::array<bool, static_cast<size_t>(AudioChannel::Count)> ChannelMuted;
        bool bMasterMuted = false;

        // Pause state
        bool bPaused = false;

        // 3D audio state
        vec2f ListenerPosition = { 0.0f, 0.0f };
        vec2f ListenerDirection = { 0.0f, -1.0f };
    };
}

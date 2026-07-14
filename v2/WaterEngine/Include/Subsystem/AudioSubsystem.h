// =============================================================================
// Water Engine v2.2.4
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    using Volume = float;

    // Handle for a persistent positional loop started via PlayLoop (0 = invalid).
    using AudioLoopId = ulong;

    enum class AudioChannel : uint8
    {
        Master, Music, Ambient, SFX, Voice, UI, ChannelSize
    };

    class AudioSubsystem
    {
    public:
        AudioSubsystem();

        static AudioSubsystem& Get();

        void Update(float DeltaTime);

        void PlayMusic(const string& Path, float FadeInDuration = 0.0f);
        void PlayAmbient(const string& Path, float FadeInDuration = 0.0f);
        void CrossfadeMusic(const string& Path, float Duration);
        void CrossfadeAmbient(const string& Path, float Duration);

        // One-shots. Gain is the sound's 0..1 mix level, multiplied under its
        // bus volume (per-sound mixing lives in game data, not in the files).
        void PlaySFX(const string& Path, Volume Gain = 1.0f);
        void PlayVoice(const string& Path, Volume Gain = 1.0f);

        // UI feedback on the UI bus. Unlike every other channel it ignores
        // SetPaused - menu clicks happen while the game is paused.
        void PlayUI(const string& Path, Volume Gain = 1.0f);

        // Spatialized playback. The listener is a world position the game drives
        // each frame (player); everything played with a WorldPos attenuates with
        // distance from it. SFML only spatializes MONO buffers - stereo assets
        // play at full volume everywhere. MinDistance = full-volume radius and
        // Attenuation = falloff steepness, both in world units (callers pass
        // their GameConfig knobs).
        void SetListenerPosition(vec2f WorldPos);
        void PlaySFX(const string& Path, vec2f WorldPos, float MinDistance, float Attenuation,
                     Volume Gain = 1.0f);

        // Persistent loops (ambient emitters). Loops never end on their own:
        // the owner must StopLoop (or StopAll) them. The positional overload
        // attenuates by listener distance; the head-locked overload plays
        // everywhere and is shaped by the owner via SetLoopVolume (a 0..1 gain
        // multiplied under the Ambient bus volume).
        AudioLoopId PlayLoop(const string& Path, vec2f WorldPos, float MinDistance, float Attenuation);
        AudioLoopId PlayLoop(const string& Path);
        void SetLoopPosition(AudioLoopId Id, vec2f WorldPos);
        void SetLoopVolume(AudioLoopId Id, Volume Vol);
        void StopLoop(AudioLoopId Id);
        void StopAllLoops();

        void StopMusic(float FadeDuration = 0.0f);
        void StopAmbient(float FadeDuration = 0.0f);
        void StopAllSFX();
        void StopAllVoice();
        void StopAllUI();
        void StopAll();

        void SetMasterVolume(Volume Vol);
        Volume GetMasterVolume() const;

        void SetChannelVolume(AudioChannel Channel, Volume Vol);
        Volume GetChannelVolume(AudioChannel Channel) const;

        void SetMasterMuted(bool bMuted);
        bool IsMasterMuted() const;
        void SetChannelMuted(AudioChannel Channel, bool bMuted);
        bool IsChannelMuted(AudioChannel Channel) const;

        void SetPaused(bool bPaused);
        bool IsPaused() const;

        bool IsMusicPlaying() const;
        bool IsAmbientPlaying() const;
        ulong GetActiveSFXCount() const;
        ulong GetActiveVoiceCount() const;
        ulong GetActiveUICount() const;
        ulong GetActiveLoopCount() const;

    private:
        struct MusicTrack
        {
            shared<music> Music;
            Volume CurrentVolume = 1.0f;
            Volume TargetVolume = 1.0f;
            float FadeSpeed = 0.0f;
            bool bFadingIn = false;
        };

        struct SFXInstance
        {
            shared<soundBuffer> Buffer;
            unique<sound> Sound;
            Volume UserVolume = 1.0f;   // per-sound mix gain under the bus volume
        };

        struct LoopInstance
        {
            AudioLoopId Id = 0;
            shared<soundBuffer> Buffer;
            unique<sound> Sound;
            Volume UserVolume = 1.0f;   // per-loop gain under the Ambient bus
        };

        void UpdateFades(float DeltaTime);
        void CleanupStoppedSounds();
        void ApplyVolumeToMusic(MusicTrack& Track, AudioChannel Channel);
        Volume GetEffectiveVolume(AudioChannel Channel) const;
        LoopInstance* FindLoop(AudioLoopId Id);
        void ApplyLoopVolume(LoopInstance& Loop);
        void ApplyOneShotVolume(SFXInstance& Instance, AudioChannel Channel);
        void MakeRoomForSFX();

    private:
        // Voice-count ceiling for the SFX bus: at the cap the oldest playing
        // instance is evicted, so a packed fight can't pile up unbounded voices.
        static constexpr ulong MAX_ACTIVE_SFX = 32;

        static AudioSubsystem* Instance;
        unique<MusicTrack> CurrentMusic;
        unique<MusicTrack> CurrentAmbient;
        unique<MusicTrack> FadingMusic;
        unique<MusicTrack> FadingAmbient;
        vector<SFXInstance> ActiveSFX;
        vector<SFXInstance> ActiveVoice;
        vector<SFXInstance> ActiveUI;
        vector<LoopInstance> ActiveLoops;
        AudioLoopId NextLoopId = 1;

        Volume MasterVolume = 1.0f;
        array<Volume, static_cast<ulong>(AudioChannel::ChannelSize)> ChannelVolumes;
        array<bool, static_cast<ulong>(AudioChannel::ChannelSize)> ChannelMuted;
        bool bMasterMuted = false;
        bool bPaused = false;
    };

    inline AudioSubsystem& PlayAudio() { return AudioSubsystem::Get(); }
}
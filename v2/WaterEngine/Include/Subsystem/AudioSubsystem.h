// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    using Volume = float;

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
        void PlaySFX(const string& Path);
        void PlayVoice(const string& Path);

        void StopMusic(float FadeDuration = 0.0f);
        void StopAmbient(float FadeDuration = 0.0f);
        void StopAllSFX();
        void StopAllVoice();
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
        };

        void UpdateFades(float DeltaTime);
        void CleanupStoppedSounds();
        void ApplyVolumeToMusic(MusicTrack& Track, AudioChannel Channel);
        Volume GetEffectiveVolume(AudioChannel Channel) const;

    private:
        static AudioSubsystem* Instance;
        unique<MusicTrack> CurrentMusic;
        unique<MusicTrack> CurrentAmbient;
        unique<MusicTrack> FadingMusic;
        unique<MusicTrack> FadingAmbient;
        vector<SFXInstance> ActiveSFX;
        vector<SFXInstance> ActiveVoice;

        Volume MasterVolume = 1.0f;
        array<Volume, static_cast<ulong>(AudioChannel::ChannelSize)> ChannelVolumes;
        array<bool, static_cast<ulong>(AudioChannel::ChannelSize)> ChannelMuted;
        bool bMasterMuted = false;
        bool bPaused = false;
    };

    inline AudioSubsystem& PlayAudio() { return AudioSubsystem::Get(); }
}
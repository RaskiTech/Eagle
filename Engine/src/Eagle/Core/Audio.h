#pragma once
#include <AudioFile.h>
#include <atomic>
#include "AssetManager.h"
#include "Core.h"
#define MAX_PLAYING_CLIP_COUNT 16

// AudioSource is an ecs holder for a pointer to AudioSample
// Audio callback gets pointers to AudioSample

// AudioSample owns AudioClip and some settings
// AudioClip owns music data


// TODO: Recourse manager because entt is likely moving AudioSource and that deletes 
//		 AudioSample. Is it also needed so don't need to manage clip pointers
// Then this will work:
// player.AddComponent<AudioSource>(clip).Play(true);
// ground.AddComponent<AudioSource>(clip2).Play(true);

namespace Egl {
    typedef float AudioSamplePrecision;

    struct AudioClip {

        // Note: This is loaded and NEVER modified again
        AudioFile<AudioSamplePrecision> data;

        AudioClip(const std::string& audioFilePath);
    };
    struct AudioSample {
        std::atomic<bool> playing = true;
        std::atomic<float> volume = 0.5f;
        std::atomic<bool> loop = false;
        std::atomic<int> samplePosition = 0; // time
        std::atomic<AudioClip*> clip;

        float GetPlayTime() const { return (float)samplePosition / ((AudioClip*)clip)->data.getSampleRate(); }
        AudioSample(AudioClipID audioClip);
    private:
        AudioClipID id; // Assets should be stored in raw pointers, so keep the ID to get the reference count right
        uint8_t referencesHere = 1; // Reference counting in AudioSample
        friend struct AudioSource;
    };

    // Audio playing manager
    class Audio {
    public:
        static void AddSample(AudioSample* sample);
        static void RemoveSample(AudioSample* sample);
    protected:
        static void Init();
        static void Close();
        friend class Application;

    private:
        static void* stream;
        static std::array<std::atomic<AudioSample*>, MAX_PLAYING_CLIP_COUNT> playingSamples;
    };
}
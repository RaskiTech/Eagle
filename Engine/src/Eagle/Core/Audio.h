#pragma once
#include <AudioFile.h>
#include "Core.h"
#define MAX_PLAYING_CLIP_COUNT 16

// AudioSource is an ecs holder for a pointer to AudioSample
// Audio callback gets pointers to AudioSample

// AudioSample owns AudioClip and some settings
// AudioClip owns music data


// Needed Improvement for file loader:
// Constructor that takes filepath and error callback
// ability to check whether there is a proper file


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
        AudioSample(AudioClip* audioClip) : clip(audioClip) {}
    };

    // Audio playing manager
    class Audio {
    public:
        static void AddSample(AudioSample* clip);
    protected:
        static void Init();
        static void Close();
        friend class Application;

    private:
        static void* stream;
        static std::array<std::atomic<AudioSample*>, MAX_PLAYING_CLIP_COUNT> playingSamples;
    };
}
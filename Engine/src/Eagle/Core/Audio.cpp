#include <EaglePCH.h>
#include <AudioFile.h>
#include <portaudio.h>
#include "Audio.h"

#define NUM_SECONDS   4
#define SAMPLE_RATE   44100

namespace Egl {
    void* Audio::stream;
    std::array<std::atomic<AudioSample*>, MAX_PLAYING_CLIP_COUNT> Audio::playingSamples;

    AudioClip::AudioClip(const std::string& audioFilePath) {
        EAGLE_PROFILE_SCOPE("AudioFile: load audioClip");
        data.shouldLogErrorsToConsole(false);
        bool loaded = data.load(audioFilePath);
        if (!loaded)
            LOG_ENG_ERROR("Couldn't load the file at", audioFilePath);
    }
    AudioSample::AudioSample(AudioClipID id) : id(id), clip(Assets::GetClip(id)) {

    }

    void Audio::AddSample(AudioSample* clip) {
        EAGLE_PROFILE_FUNCTION();
        for (int i = 0; i < MAX_PLAYING_CLIP_COUNT; i++) {
            if (playingSamples[i] != nullptr)
                continue;

            playingSamples[i] = clip;

            /*
            LOG_ENG("Info about the clip:");
            LOG_ENG("Length:", clip->audio.getLengthInSeconds());
            LOG_ENG("Channels:", clip->audio.getNumChannels());
            LOG_ENG("Sample rate:", clip->audio.getSampleRate());
            LOG_ENG("Bit depth:", clip->audio.getBitDepth());
            //*/

            return;
        }

        LOG_ENG_WARN("Already playing the max amount of audio clips.");
    }
    void Audio::RemoveSample(AudioSample* sample) {
        EAGLE_PROFILE_FUNCTION();
        for (int i = 0; i < MAX_PLAYING_CLIP_COUNT; i++) {
            if (playingSamples[i] != sample)
                continue;

            playingSamples[i] = nullptr;
            return;
        }

        LOG_ENG_WARN("Didn't find a sample that was meant to be removed.");
    }

    static void ErrorCall(PaError error) {
        LOG_ENG_ERROR("An error occurred while using the portaudio stream");
        LOG_ENG_ERROR("Error number:", error);
        LOG_ENG_ERROR("Error message:", Pa_GetErrorText(error));
        Pa_Terminate();
    }

    static std::array<int, MAX_PLAYING_CLIP_COUNT> sampleRateRemainder;
    template<bool SetDontAdd>
    static inline void PlayClipToBuffer(const int clipIndex, AudioSamplePrecision* out, const unsigned long framesPerBuffer,
        std::atomic<AudioSample*>& clip) {
        //AudioClip* clipPtr = clip;
        
        if (!(*clip).playing) {
            clip = nullptr;
            return;
        }


        for (int i = 0; i < framesPerBuffer; i++) {
            auto& data = (*(*clip).clip).data;
            if constexpr (SetDontAdd) {
                *out++ = (*clip).volume * data.samples[0][(*clip).samplePosition];
                *out++ = (*clip).volume * data.samples[data.getNumChannels() > 1 ? 1 : 0][(*clip).samplePosition];
            }
            else {
                *out++ += (*clip).volume * data.samples[0][(*clip).samplePosition];
                *out++ += (*clip).volume * data.samples[data.getNumChannels() > 1 ? 1 : 0][(*clip).samplePosition];
            }

            // Sync different sample rates
            sampleRateRemainder[clipIndex] += data.getSampleRate();
            while (sampleRateRemainder[clipIndex] > SAMPLE_RATE - 1) {
                sampleRateRemainder[clipIndex] -= SAMPLE_RATE;
                (*clip).samplePosition++;
            }

            if ((*clip).samplePosition >= data.getNumSamplesPerChannel()) {
                (*clip).playing = false;
                clip = nullptr;
                return;
            }
        }
    }
    static int paCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
    {
        AudioSamplePrecision* outStart = (AudioSamplePrecision*)outputBuffer;
        auto& playingClips = *(std::array<std::atomic<AudioSample*>, MAX_PLAYING_CLIP_COUNT>*)userData;

        bool firstPlay = true;
        for (int clipIndex = 0; clipIndex < MAX_PLAYING_CLIP_COUNT; clipIndex++) {
            auto& clip = playingClips[clipIndex];
            if (clip == nullptr)
                continue;

            if (firstPlay) PlayClipToBuffer<true >(clipIndex, outStart, framesPerBuffer, clip);
            else           PlayClipToBuffer<false>(clipIndex, outStart, framesPerBuffer, clip);

            firstPlay = false;
        }

        if (firstPlay) {
            for (int i = 0; i < framesPerBuffer; i++)
                *outStart++ = 0;
        }

        return 0;
    }

    void Audio::Init() {
        EAGLE_PROFILE_FUNCTION();
        PaError err;

        {
            EAGLE_PROFILE_SCOPE("Initialize PortAudio");
            err = Pa_Initialize();
            if (err != paNoError) {
                ErrorCall(err);
                return;
            }
        }

        {
            EAGLE_PROFILE_SCOPE("Open PortAudio Stream");
            err = Pa_OpenDefaultStream(&stream,
                0,          // no input channels 
                2,          // stereo output 
                paFloat32,  // 32 bit floating point output 
                SAMPLE_RATE,
                256,        // frames per buffer 
                paCallback,
                &playingSamples);
            if (err != paNoError) {
                ErrorCall(err);
                return;
            }
        }
        {
            EAGLE_PROFILE_SCOPE("Start PortAudio Stream");
            err = Pa_StartStream(stream);
            if (err != paNoError) {
                ErrorCall(err);
                return;
            }
        }
    }

    void Audio::Close() {
        EAGLE_PROFILE_FUNCTION();
        PaError err = Pa_StopStream(stream);
        if (err != paNoError) {
            ErrorCall(err);
            return;
        }

        err = Pa_CloseStream(stream);
        if (err != paNoError) {
            ErrorCall(err);
            return;
        }

        Pa_Terminate();
    }

}
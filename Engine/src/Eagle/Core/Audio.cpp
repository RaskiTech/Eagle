#include <EaglePCH.h>
#include <AudioFile.h>
#include <portaudio.h>
#include "Audio.h"

#define NUM_SECONDS   4
#define SAMPLE_RATE   44100

namespace Egl {
    void* Audio::stream;
    std::array<std::atomic<AudioClip*>, MAX_PLAYING_CLIP_COUNT> Audio::playingClips;

    AudioClip::AudioClip(const std::string& audioFilePath) {
        audio.shouldLogErrorsToConsole(false);
        bool loaded = audio.load(audioFilePath);
        if (!loaded)
            LOG_ENG_ERROR("Couldn't load the file at", audioFilePath);
    }

    void Audio::AddClip(AudioClip* clip) {
        for (int i = 0; i < MAX_PLAYING_CLIP_COUNT; i++) {
            if (playingClips[i] != nullptr)
                continue;

            playingClips[i] = clip;

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

    static void ErrorCall(PaError error) {
        LOG_ENG_ERROR("An error occurred while using the portaudio stream");
        LOG_ENG_ERROR("Error number:", error);
        LOG_ENG_ERROR("Error message:", Pa_GetErrorText(error));
        Pa_Terminate();
    }

    static std::array<int, MAX_PLAYING_CLIP_COUNT> sampleRateRemainder;
    static int paCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
    {
        float* out = (float*)outputBuffer;
        auto& playingClips = *(std::array<std::atomic<AudioClip*>, MAX_PLAYING_CLIP_COUNT>*)userData;

        for (int clipIndex = 0; clipIndex < MAX_PLAYING_CLIP_COUNT; clipIndex++) {
            AudioClip* clip = playingClips[clipIndex];
            if (clip == nullptr)
                continue;
            if (!clip->playing) {
                playingClips[clipIndex] = nullptr;
                continue;
            }
            

            for (int i = 0; i < framesPerBuffer; i++) {
                *out++ = clip->volume * clip->audio.samples[0][clip->samplePosition];
                *out++ = clip->volume * clip->audio.samples[clip->audio.getNumChannels() > 1 ? 1 : 0][clip->samplePosition];

                // Sync different sample rates
                sampleRateRemainder[clipIndex] += clip->audio.getSampleRate();
                while (sampleRateRemainder[clipIndex] > SAMPLE_RATE - 1) {
                    sampleRateRemainder[clipIndex] -= SAMPLE_RATE;
                    clip->samplePosition++;
                }

                if (clip->samplePosition >= clip->audio.getNumSamplesPerChannel()) {
                    clip->playing = false;
                    playingClips[clipIndex] = nullptr;
                    break;
                }
            }
        }

        return 0;
    }

    static AudioClip* tempTestClip;
    void Audio::Init() {
        PaError err;

        err = Pa_Initialize();
        if (err != paNoError) {
            ErrorCall(err);
            return;
        }

        err = Pa_OpenDefaultStream(&stream,
            0,          // no input channels 
            2,          // stereo output 
            paFloat32,  // 32 bit floating point output 
            SAMPLE_RATE,
            256,        // frames per buffer 
            paCallback,
            &playingClips);
        if (err != paNoError) {
            ErrorCall(err);
            return;
        }

        err = Pa_StartStream(stream);
        if (err != paNoError) {
            ErrorCall(err);
            return;
        }
    }

    void Audio::Close() {
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
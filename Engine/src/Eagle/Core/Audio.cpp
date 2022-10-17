#include <EaglePCH.h>
#include <AudioFile.h>
#include <portaudio.h>
#include "Audio.h"


namespace Egl {
    void* AudioPlayer::stream;
    std::array<std::atomic<AudioSample*>, MAX_PLAYING_CLIP_COUNT> AudioPlayer::playingSamples;

    AudioClip::AudioClip(const std::string& audioFilePath) {
        EAGLE_PROFILE_SCOPE("AudioFile: load audioClip");
        data.shouldLogErrorsToConsole(false);
        bool loaded = data.load(audioFilePath);
        if (!loaded)
            LOG_ENG_ERROR("Couldn't load the file at", audioFilePath, "Make sure it is wav or aiff format and the file exists.");
    }
    AudioSample::AudioSample(AudioClipRef id) : id(id), clip(Assets::GetClip(id)) { }

    void AudioPlayer::AddSample(AudioSample* clip) {
        EAGLE_PROFILE_FUNCTION();
        for (int i = 0; i < MAX_PLAYING_CLIP_COUNT; i++) {
            if (playingSamples[i] != nullptr)
                continue;

            playingSamples[i] = clip;

            return;
        }

        LOG_ENG_WARN("Already playing the max amount of audio clips.");
    }
    void AudioPlayer::RemoveSample(AudioSample* sample) {
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


        for (uint32_t i = 0; i < (uint32_t)framesPerBuffer; i++) {
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
                if ((*clip).loop)
                    (*clip).samplePosition = 0;
                else {
                    (*clip).playing = false;
                    clip = nullptr;
                }
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
            // We have two output channels, so double framesPerBuffer
            for (uint32_t i = 0; i < (uint32_t)framesPerBuffer*2; i++)
                *outStart++ = 0.0f;
        }


        return 0;
    }

    void AudioPlayer::Init() {
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
                &playingSamples); // User data
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

    void AudioPlayer::Close() {
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
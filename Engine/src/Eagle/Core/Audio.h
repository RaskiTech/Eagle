#pragma once
#include <AudioFile.h>
#include "Core.h"
#define MAX_PLAYING_CLIP_COUNT 16

namespace Egl {
    struct AudioClip {
        std::atomic<bool> playing = true;
        std::atomic<float> volume = 0.5f;
        std::atomic<bool> loop = false;
        std::atomic<int> samplePosition = 0;

        // Note: This is loaded and NEVER modified again
        AudioFile<float> audio;

        AudioClip(const std::string& audioFilePath);
    private:
        int sampleRateStep; // Sync up different sample rates;
    };

    class Audio {
    public:
        static void AddClip(AudioClip* clip);
    protected:
        static void Init();
        static void Close();
        friend class Application;

    private:
        static void* stream;
        static std::array<std::atomic<AudioClip*>, MAX_PLAYING_CLIP_COUNT> playingClips;
    };


    namespace Notes {
        using Note = uint32_t;
        enum : Note {
            C_3 = 131,
            C_4 = 262,
            C_5 = 523,
            C_6 = 1046,
            C_7 = 2096,
            Cs_3 = 139,
            Cs_4 = 277,
            Cs_5 = 554,
            Cs_6 = 1108,
            Cs_7 = 2217,
            D_3 = 147,
            D_4 = 294,
            D_5 = 587,
            D_6 = 1175,
            D_7 = 2349,
            Ds_3 = 156,
            Ds_4 = 311,
            Ds_5 = 622,
            Ds_6 = 1244,
            Ds_7 = 2489,
            E_3 = 165,
            E_4 = 330,
            E_5 = 659,
            E_6 = 1318,
            E_7 = 2637,
            F_3 = 175,
            F_4 = 349,
            F_5 = 698,
            F_6 = 1397,
            F_7 = 2794,
            Fs_3 = 185,
            Fs_4 = 370,
            Fs_5 = 740,
            Fs_6 = 1480,
            Fs_7 = 2960,
            G_3 = 196,
            G_4 = 392,
            G_5 = 784,
            G_6 = 1568,
            G_7 = 3136,
            Gs_3 = 208,
            Gs_4 = 415,
            Gs_5 = 831,
            Gs_6 = 1664,
            Gs_7 = 3322,
            A_3 = 220,
            A_4 = 440,
            A_5 = 880,
            A_6 = 1760,
            A_7 = 3520,
            As_3 = 233,
            As_4 = 466,
            As_5 = 932,
            As_6 = 1866,
            As_7 = 3729,
            B_3 = 248,
            B_4 = 494,
            B_5 = 988,
            B_6 = 1973,
            B_7 = 3951
        };
    }
}
#pragma once
#include <string>
#include "Core.h"
#include "AudioSample.h"

namespace Egl {
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
    
    // Example of what the API could look like. Use portAudio
    //
    // AudioSample audio = Audio::LoadWav(path);
    // audio.Play();
    // if (audio.isLooping)
    //     audio.Stop();


    class Audio {
    public:
        static void PlayWav(wchar_t* path);
        static void PlayLoopingWav(wchar_t* path);
    //  static DWORD PlayMidi(HWND hWndNotify, LPCWSTR lpszMIDIFileName);
    //
    //  void    SetActiveMP3(Ref<AudioSample> sample);
    //  void   PlayActiveMP3();
    //  void   PlayActiveMP3ToEnd();
    //  void    SetActiveMP3ToStart();
    //  void   PlayActiveMPLoop3();
    //  void  PauseActiveMP3();
    //  void ResumeActiveMP3();
    //  void   StopActiveMP3();
    //  void  CloseActiveMP3();
    //private:
    //    static Ref<AudioSample> activeMP3;
    };
}
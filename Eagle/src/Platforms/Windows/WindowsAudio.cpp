#include <EaglePCH.h>
#include <thread>
#include <Eagle/Core/Application.h>
#include "Eagle/Core/AudioSample.h"
#include "Eagle/Core/Audio.h"

#include <Mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "winmm.lib")


namespace Egl {

    void Audio::PlayNote(uint32_t pitch, uint32_t milliseconds) {
        std::thread thread(Beep, pitch, milliseconds);
        Beep(5.3f, 10);
        thread.detach();
    }
    void Audio::PlayWav(wchar_t* path) {
        sndPlaySound(path, SND_FILENAME | SND_ASYNC);
    }
    void Audio::PlayLoopingWav(wchar_t* path) {
        sndPlaySound(path, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
}
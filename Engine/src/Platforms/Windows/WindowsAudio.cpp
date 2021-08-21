#include <EaglePCH.h>
#include <thread>
#include <Eagle/Core/Application.h>
#include "Eagle/Core/AudioSample.h"
#include "Eagle/Core/Audio.h"

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")


namespace Egl {
    void Audio::PlayWav(wchar_t* path) {
        sndPlaySound(path, SND_FILENAME | SND_ASYNC);
    }
    void Audio::PlayLoopingWav(wchar_t* path) {
        sndPlaySound(path, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
}
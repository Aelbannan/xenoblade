#pragma once

#include <types.h>

class homebutton {
public:
    void PlaySeq();
    void InitAxSound();
    void ShutdownAxSound();
    void StopAllSeq();
    void SetVolumeAllSeq();
    void SetSoundMode();

    // TODO: add fields
    void AxSoundMain();
};


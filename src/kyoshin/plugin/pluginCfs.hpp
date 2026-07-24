#pragma once

#include "monolib/vm/yvm2.h"

// Plugin script functions for the CFS (Common File System / script) subsystem.
// These are registered via pluginCfsRegist() and called from the VM.

extern "C" bool func_8004A2E8();
extern "C" void func_8004A400();

extern "C" void pluginCfsRegist();

#pragma once

#include "monolib/vm/yvm2.h"

extern "C" int select(VMThread* pThread);
extern "C" int restore(VMThread* pThread);
extern "C" int setPos(VMThread* pThread);
extern "C" int setLookat(VMThread* pThread);
extern "C" int setDir(VMThread* pThread);
extern "C" int setRotX(VMThread* pThread);
extern "C" int setRotY(VMThread* pThread);
extern "C" int setFov(VMThread* pThread);
extern "C" int setTarget(VMThread* pThread);
extern "C" int setPosOfs(VMThread* pThread);
extern "C" int setLookatOfs(VMThread* pThread);
extern "C" int getPos(VMThread* pThread);
extern "C" int getRot(VMThread* pThread);
extern "C" int keyBegin(VMThread* pThread);
extern "C" int keyEnd(VMThread* pThread);
extern "C" int keyAdd(VMThread* pThread);
extern "C" int isKeyMove(VMThread* pThread);
extern "C" int waitKeyMove(VMThread* pThread);
extern "C" int shake(VMThread* pThread);
extern "C" int stopShake(VMThread* pThread);

void pluginCamRegist();
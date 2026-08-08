#pragma once

#include <types.h>

class CMenuQuestLog {
public:
    CMenuQuestLog();
    virtual ~CMenuQuestLog();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

class UnkClass_8011C974 {
public:
    UnkClass_8011C974();

    // TODO: add fields
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" bool isIdle__11CTitleAHelpFv(void*);
extern "C" bool func_80227CCC(void*);
extern "C" unsigned int func_80228394(void*);
extern "C" void func_80229768(void*, unsigned short);
extern "C" void func_80229510(void*);


#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dt__12CTaskGameEvtFv();
extern "C" void cbRenderBefore__12CTaskGameEvtFv(void* self);
extern "C" int func_80295764(void* self);
extern "C" void removeRenderCB__4CScnFP10IScnRender(void*, void*);

class CTaskGameEvt {
public:
    CTaskGameEvt();
    virtual ~CTaskGameEvt();
    void Init();
    void Term();
    void Move();
    void create();

    // TODO: add fields
    void cbRenderBefore();
    void Draw();
    void OnFileEvent();
};


#pragma once

#include <types.h>

#include "monolib/work/CTTask.hpp"
#include "monolib/lib/CLibCri.hpp"

u32 func_80164910();

class CProcess;
class CScnNw4r;
class CView;

// Manager object behind lbl_eu_80664240 (recovered layout; only the fields
// this TU reads are declared).
struct CEventMgr {
    u8 field_0x00[0x6C];
    u32 field_0x6C;          // +0x6C flag word (bit0/bit6 read by this TU)
    u8 field_0x70[0x1E0 - 0x70];
    CLibCri* mCri;           // +0x1E0 CRI movie-play controller, or (CLibCri*)-1
};

// Data symbols (retail linker names - global scope, no extern "C" needed)
extern u32 lbl_eu_80662380;
extern u32 lbl_eu_80662384;
extern f32 lbl_eu_8066762C;
extern CEventMgr* lbl_eu_80664240;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
    int func_80164C28();
    void func_80165014();
    float func_80164478();
    int func_80164FB4();
    u32 func_80166150(CEventMgr* self, u32 arg);
    // Value-returning tail call through the CRI movie controller: CLibCri.hpp
    // declares func_80459AC4 as void, but retail func_80164FB4 performs a
    // value tail call through it, so import the mangled symbol with an
    // explicit value signature.
    u32 func_80459AC4__7CLibCriFv(CLibCri* self);
}

namespace cf{
    class CTaskREvent : public CTTask<CTaskREvent>{
    public:
        static CTaskREvent* getInstance();
        static CTaskREvent* create(CProcess* pParent, CScnNw4r* pScene, CView* pView);

        //0x000-0x054 CTTask
    char unk054[0x1F8 - 0x054]; //0x054

    static CTaskREvent* spInstance;
public:
    virtual ~CTaskREvent();
    void Init();
    void Term();
    void Move();
    void Draw();
    void cbRenderBefore();
    void OnFileEvent();
    }; //size = 0x1F8
} //namespace cf

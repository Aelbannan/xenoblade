#pragma once

#include <types.h>

#include "monolib/work/CTTask.hpp"
#include "monolib/lib/CLibCri.hpp"
#include "monolib/core/CViewFrame.hpp" // ml::CRect

u32 func_80164910();

class CProcess;
class CScnNw4r;
class CView;

namespace cf {
class CTaskREvent;
}

// Manager object behind lbl_eu_80664240 (recovered layout; only the fields
// this TU reads are declared).
struct CEventMgr {
    u8 field_0x00[0x6C];
    u32 field_0x6C;          // +0x6C flag word (bit0/bit6 read by this TU)
    u8 field_0x70[0xB0 - 0x70];
    u32 field_0xB0;          // +0xB0 flag word (nonzero gate for func_80164410 / func_80164DB8)
    u8 field_0xB4[0x1BC - 0xB4];
    u8 field_0x1BC;          // +0x1BC byte flag
    u8 field_0x1BD[0x1D0 - 0x1BD];
    u32 field_0x1D0;         // +0x1D0 word cleared to zero by func_80164DB8
    s32 field_0x1D4;         // +0x1D4 signed counter/index
    u8 field_0x1D8[0x1E0 - 0x1D8];
    CLibCri* mCri;           // +0x1E0 CRI movie-play controller, or (CLibCri*)-1
};

// Data symbols (retail linker names - global scope, no extern "C" needed)
extern u32 lbl_eu_80662380;
extern u32 lbl_eu_80662384;
extern f32 lbl_eu_8066762C;
extern CEventMgr* lbl_eu_80664240;
extern u32 lbl_eu_80663E28; // event flag word (bit7 tested by func_80164910)
extern s16 lbl_eu_80502F90[]; // event-id table walked by func_801644D8 (32 entries)

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
    void func_80164F6C();
    void func_80165DF4(cf::CTaskREvent* self, int arg);
    void func_80166050(cf::CTaskREvent* self, int arg);
    void func_8016462C(int index);
    // Retail ctor symbol is the pre-mangled name __ct__cf_CTaskREvent (a
    // global function, not a cf::CTaskREvent member); declared here so the
    // definition in the .cpp keeps C linkage and emits the plain name.
    cf::CTaskREvent* __ct__cf_CTaskREvent(cf::CTaskREvent* pMem, CScnNw4r* pScene, CView* pView);
    // Targets defined in this TU (declared here so the definitions keep C
    // linkage and emit the retail unmangled symbol names).
    int func_80164410();
    void func_80164DB8();
    void func_801644D8(cf::CTaskREvent* self, int type, int upper, int lower);
    cf::CTaskREvent* func_801665A4(CProcess* pParent, CScnNw4r* pScene, CView* pView);
    // Imports for func_80164DB8
    void func_80168484(int arg);
    void func_80043BC4();
    // Imports for cf::CTaskREvent::cbRenderBefore
    CView* getCurrentView__5CViewFv();
    void func_8043EA88__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect, CView* view);
    void func_80459ACC__7CLibCriFv(CLibCri* self, ml::CRect& rect);
}

namespace cf{
    class CTaskREvent : public CTTask<CTaskREvent>{
    public:
        static CTaskREvent* getInstance();
        static CTaskREvent* create(CProcess* pParent, CScnNw4r* pScene, CView* pView);

        //0x000-0x054 CTTask
    char unk054[0x1E0 - 0x054]; //0x054
    CLibCri* mCri;              //0x1E0 CRI movie-play controller, or (CLibCri*)-1
    char unk1E4[0x1F8 - 0x1E4]; //0x1E4

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

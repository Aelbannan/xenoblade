#pragma once

#include <types.h>

#include "monolib/work/CTTask.hpp"
#include "monolib/lib/CLibCri.hpp"
#include "monolib/core/CViewFrame.hpp" // ml::CRect

u32 func_80164910();

class CProcess;
class CScn;
class CScnNw4r;
class CView;

namespace cf {
class CTaskREvent;
}

// Object pointed at by CEventMgr::field_0xB0; only byte +0x39 is touched
// (func_80164CFC clears it to 1 while tearing an event down).
struct CEventMgrB0 {
    u8 field_0x00[0x39];
    u8 field_0x39;          // +0x39 byte flag
};

// Manager object behind lbl_eu_80664240 (recovered layout; only the fields
// this TU reads are declared).
struct CEventMgr {
    u8 field_0x00[0x6C];
    u32 field_0x6C;          // +0x6C flag word (bit0/bit6 read by this TU)
    u8 field_0x70[0x74 - 0x70];
    u32 field_0x74;          // +0x74 flag word (bit0 read by func_80164C48)
    u8 field_0x78[0xB0 - 0x78];
    CEventMgrB0* field_0xB0; // +0xB0 gate object (null-checked by func_80164410 / func_80164DB8 / func_80164C48)
    u8 field_0xB4[0x1BC - 0xB4];
    u8 field_0x1BC;          // +0x1BC byte flag
    u8 field_0x1BD[0x1D0 - 0x1BD];
    u32 field_0x1D0;         // +0x1D0 word cleared to zero by func_80164DB8
    s32 field_0x1D4;         // +0x1D4 signed counter/index
    u8 field_0x1D8[0x1E0 - 0x1D8];
    CLibCri* mCri;           // +0x1E0 CRI movie-play controller, or (CLibCri*)-1
};

// 32-slot event data table filled by func_8016455C: byte slots at +0x00
// (0x28 each, 20-byte sub-slots set to 1) and word slots at +0x500 (0x20
// each, 16-byte sub-slots zeroed), plus per-slot u8/u16 flags.
struct CEventDataTable {
    struct ByteSlot {
        u8 data[0x14];
    };
    struct WordSlot {
        u32 data[4];
    };
    ByteSlot mBytes[32][2];  // +0x000 (32 x 0x28)
    WordSlot mWords[32][2];  // +0x500 (32 x 0x20)
    u8 mFlags900[32];        // +0x900
    u16 mIds920[32];         // +0x920
};

// Data symbols (retail linker names - global scope, no extern "C" needed)
extern u32 lbl_eu_80662380;
extern u32 lbl_eu_80662384;
extern f32 lbl_eu_8066762C;
extern CEventMgr* lbl_eu_80664240;
extern u32 lbl_eu_80663E28; // event flag word (bit7 tested by func_80164910)
extern u32 lbl_eu_80663E24; // global event/presentation bitfield (bit6 cleared by func_80164CFC)
extern s16 lbl_eu_80502F90[]; // event-id table walked by func_801644D8 (32 entries)
extern CScn* lbl_eu_80663E14; // scene object (alloc-handle source for func_80495FF0)
extern const char lbl_eu_80503008[];  // Init: base string copied into each entry
// Init: 0xAE suffix strings appended to each entry (walked by pointer).
extern const char* lbl_eu_80530458[];

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
    // CRI movie-player setup: retail func_80164ED0 passes 4 extra words even
    // though the retail symbol is Fv; declared here with the caller's shape.
    CLibCri* func_80459AA8__7CLibCriFv(const void* self, u32 memHandle, u32 buffer, int flag, int zero);
    u32 func_8016847C();
    u32 func_80495FF0(CScn* scene);
    void func_80164ED0(const char* path, int flag, void* handle);
    int func_80164C48();
    void func_80164CFC();
    void func_8016455C(CEventDataTable* self);
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
    char unk054[0x1C8 - 0x054]; //0x054
    u8* mBuf;                   //0x1C8 Init: allocated string-table buffer (0x1400 bytes)
    u32 mIdx;                   //0x1CC Init: current table entry index
    char unk1D0[0x1E0 - 0x1D0]; //0x1D0
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

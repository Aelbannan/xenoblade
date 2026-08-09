// Auto-scaffolded catalog TU for kyoshin/menu/CMenuMapSelect
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuMapSelect.hpp"

// ---------------------------------------------------------------------------
// External retail symbols
// ---------------------------------------------------------------------------
extern "C" {
void __ct__8CProcessFv(CProcess* self);
u32 getWorkMem__17CWorkThreadSystemFv();
void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);
void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent, bool insertTop);
void __ct__CBgTex(CBgTex* self, u8 arg);
void __ct__CTitleAHelp(CTitleAHelp* self, char* arg1, u8 arg2);
void __ct__CMapSel(CMapSel* self);
void __ct__CFade(CFade* self);
void __ct__CFloorMap(CFloorMap* self);

extern u32 __ptmf_null[3];
extern char lbl_eu_8052BF70[];
extern char lbl_eu_80536D28[];
extern u32 lbl_eu_80664790;
extern const f32 lbl_eu_80668728;

// CfPadData access (defined in include/kyoshin/cf/CfGameManager.hpp)
// CfPadData is at offset 0x128. CPad is at offset 0x00 (0xF8 bytes).
// CPad::mPressedButtonFlags is at CPad+0x4.
// CfPadData::mTurboPressButtonFlags is at CfPadData+0x104.
struct CfPadDataLocal {
    u32 mPadHeldFlags;      // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;   // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0xF0];          // 0x08-0xF7 - rest of CPad
    u32 mHeldButtonFlags;   // 0xF8
    u32 mPrevHeldButtonFlags; // 0xFC
    u32 mPressedButtonFlags;  // 0x100
    u32 mTurboPressButtonFlags; // 0x104
    u8 mTimers[0x20];       // 0x108
};

// Callee functions
void func_80138078__FUl(u32 arg);
int func_800FEDF8();
void func_800FF914();
void func_80244518(CFade* self);
void func_8024371C(CMapSel* self);
void func_80243768(CMapSel* self);
void func_80243838(CMapSel* self);
void func_8024391C(CMapSel* self);
void func_802439CC(CMapSel* self);

// CfGameManager static methods
CfPadDataLocal* getCfPadData__Q22cf13CfGameManagerFv();
int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
}

// Byte-range shim over the CProcess header + PTMF zone, so the constructor
// can write the vtable (+0x10) and callback slots (+0x3C..0x53) without raw
// pointer arithmetic. Layout matches CProcess (0x00-0x3B) exactly.
struct CMapSelCtorShim {
    u8 _00[0x10];
    void* vtable;          // 0x10 -- CProcess vtable, overwritten by this ctor
    u8 _14[0x28];          // 0x14-0x3B -- rest of CProcess
    u32 callbacks[6];      // 0x3C-0x53 -- PTMF callback slots (__ptmf_null copies)
    u8 field54;            // 0x54
    u8 field55;            // 0x55
};

// ---------------------------------------------------------------------------
// Target: us-802443d4 -- CMenuMapSelect constructor
// ---------------------------------------------------------------------------
extern "C" CMenuMapSelect* __ct__CMenuMapSelect(CProcess* parent, CProcess* parent2) {
    if (lbl_eu_80664790 != 0) {
        return 0;
    }

    u32 handle = getWorkMem__17CWorkThreadSystemFv();
    CMapSelCtorShim* shim = (CMapSelCtorShim*)allocate__Q23mtl10MemManagerFUlUl(0x34E4, handle);

    if (shim != 0) {
        __ct__8CProcessFv((CProcess*)shim);
        shim->vtable = lbl_eu_8052BF70;

        u32* ptmf = __ptmf_null;
        char* vtFinal = lbl_eu_80536D28;

        u32 ptmf1 = ptmf[1];
        u32 ptmf0 = ptmf[0];
        void* iscnVtbl = vtFinal + 0x24;

        shim->callbacks[0] = ptmf0;
        shim->callbacks[1] = ptmf1;

        u32 ptmf2 = ptmf[2];
        shim->callbacks[2] = ptmf2;

        ptmf1 = ptmf[1];
        ptmf0 = ptmf[0];
        shim->callbacks[3] = ptmf0;
        shim->callbacks[4] = ptmf1;
        ptmf2 = ptmf[2];
        shim->callbacks[5] = ptmf2;

        shim->field54 = 0;
        shim->field55 = 0;

        CMenuMapSelect* self = (CMenuMapSelect*)shim;
        shim->vtable = vtFinal;
        *(u32*)((u8*)self + 0x58) = (u32)iscnVtbl;
        self->mParent = parent2;

        __ct__CBgTex(&self->mBgTex, 0);
        __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
        __ct__CMapSel(&self->mMapSel);
        __ct__CFade(&self->mFade);
        __ct__CFloorMap(&self->mFloorMap);

        self->mState = 0;
        self->mTimer = lbl_eu_80668728;
    }

    lbl_eu_80664790 = (u32)shim;
    Regist__8CProcessFP8CProcessb((CProcess*)shim, parent, 0);
    return (CMenuMapSelect*)lbl_eu_80664790;
}


// ---------------------------------------------------------------------------
// Target: func_80242354 -- singleton guard
// ---------------------------------------------------------------------------
extern "C" u32 func_80242354() {
    return lbl_eu_80664790 != 0;
}

// ---------------------------------------------------------------------------
// Target: func_80242368 -- world map input handler
// ---------------------------------------------------------------------------
void CMenuMapSelect::func_80242368() {
    CfPadDataLocal* padData = getCfPadData__Q22cf13CfGameManagerFv();
    u32 turboFlags = padData->mTurboPressButtonFlags;
    u32 pressedFlags = padData->mPadPressedFlags;

    // Extract trigger/stick direction bits from pressed button flags.
    // The bit positions differ between Classic Controller and Wiimote/Nunchuk.
    u32 triggerBit1, triggerBit2, triggerBit3;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        // Classic controller: bits 21, 22, 23 from right
        triggerBit1 = (pressedFlags >> 21) & 1;
        triggerBit2 = (pressedFlags >> 22) & 1;
        triggerBit3 = (pressedFlags >> 23) & 1;
    } else {
        // Wiimote/Nunchuk: bits 4, 5, 10 from right
        triggerBit1 = (pressedFlags >> 4) & 1;
        triggerBit2 = (pressedFlags >> 5) & 1;
        triggerBit3 = (pressedFlags >> 10) & 1;
    }

    // Extract button booleans from turbo flags using neg/or/srwi pattern.
    // (x | -x) >> 31 converts any non-zero value to 1, zero to 0.
    u32 cancelVal = turboFlags & 0x8004;
    u32 confirmVal = turboFlags & 0x2001;
    u32 menuVal = turboFlags & 0x4002;
    u32 dirVal = ((turboFlags >> 16) & 1) | ((turboFlags >> 3) & 1);

    u32 cancel = (u32)(-(s32)cancelVal | cancelVal) >> 31;
    u32 confirm = (u32)(-(s32)confirmVal | confirmVal) >> 31;
    u32 menu = (u32)(-(s32)menuVal | menuVal) >> 31;
    u32 dir = (u32)(-(s32)dirVal | dirVal) >> 31;

    // Process actions in priority order
    if (triggerBit1 != 0) {
        // Cancel/back action
        func_80244518(&this->mFade);
        this->mState = 4;
        func_80138078__FUl(3);
    } else if (triggerBit2 != 0) {
        // Show help
        this->mTitleAHelp.func_801C414C();
        func_8024371C(&this->mMapSel);
        this->mState = 3;
    } else if (cancel != 0) {
        // Cursor up
        func_80243768(&this->mMapSel);
    } else if (dir != 0) {
        // Cursor down
        func_80243838(&this->mMapSel);
    } else if (confirm != 0) {
        // Cursor left
        func_8024391C(&this->mMapSel);
    } else if (menu != 0) {
        // Cursor right
        func_802439CC(&this->mMapSel);
    } else if (triggerBit3 != 0) {
        // Confirm selection
        if (func_800FEDF8() != 0) {
            func_800FF914();
            func_80138078__FUl(6);
        }
        this->mState = 11;
    }
}

// ---------------------------------------------------------------------------
// Target: func_80242524
// ---------------------------------------------------------------------------
void func_80242524(){}

// ---------------------------------------------------------------------------
// Target: func_80242A28
// ---------------------------------------------------------------------------
void func_80242A28(){}

// ---------------------------------------------------------------------------
// IScnRender vtable this-adjusting thunk for cbRenderBefore.
//
// Entered with r3 = IScnRender subobject pointer (CMenuMapSelect base + 0x58).
// Convert back to the CMenuMapSelect base and tail-call the real cbRenderBefore
// (retail symbol at 0x802442FC, implemented in code_802405F4.cpp).
//
// Retail: subi r3, r3, 0x58; b cbRenderBefore__14CMenuMapSelectFv
// ---------------------------------------------------------------------------
void func_8024309C(CMenuMapSelect* self) {
    CMenuMapSelect* base = (CMenuMapSelect*)((u8*)self - 0x58);
    // Static dispatch (cbRenderBefore is virtual via IScnRender); the retargeted
    // receiver is the CMenuMapSelect base where the real body lives.
    base->CMenuMapSelect::cbRenderBefore();
}

// ---------------------------------------------------------------------------
// IScnRender vtable this-adjusting thunk for destructor.
// ---------------------------------------------------------------------------
void CMenuMapSelect::func_802430A4() {
    this->~CMenuMapSelect();
}
// Auto-scaffolded catalog TU for kyoshin/CSkipTimer
// High-level C/C++ reconstruction.

#include "kyoshin/CSkipTimer.hpp"

#include <cstdio>

// Retail linking: func_80137444 is mangled (C++ linkage), func_80137510 /
// func_80136A1C are unmangled in US retail (extern "C"). code_80135FDC.hpp
// declares func_80137510 with C++ linkage, so declare manually here.
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);

// --- shared data (retail split symbols) ---

// .sdata2 float constant used as the "animation reached frame" bound.
extern const float lbl_eu_80668C30;
// CSkipTimer2 vtable pointer stored at +0x00.
extern "C" void* lbl_eu_80539884[];
// CSkipTimer vtable pointer stored at +0x00.
extern "C" void* lbl_eu_80539878[];
// String pool used by func_8029F440 (pane name + sprintf format).
extern "C" char lbl_eu_80510568[];

// External callees.
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);

// func_8029F364 is matched upstream (no stub needed); exposed via declaration.
extern "C" void func_8029F364(CSkipTimer2*, u8);
extern "C" int func_802A04F0(CSkipTimer*);

// Unmatched same-unit siblings referenced as extern (linker resolves to retail
// address) so in-unit callers emit a direct `bl` instead of inlining a stub.
// Retail strips mangling for these func_ names in US, hence extern "C".
extern "C" void func_8029F82C(CSkipTimer2* self, u8 arg);
extern "C" void func_802A05E4(CSkipTimer* self);

// ============================================================================
// CSkipTimer2 constructor
// ============================================================================
CSkipTimer2::CSkipTimer2(void* parent) {
    mVtbl = lbl_eu_80539884;
    __ct__17UnkClass_8045F564Fv(&mMemRegion);
    mParent = parent;
    mField18 = 0;
    mAnimTransform = 0;
    mField20 = 0;
    mField21 = 0;
    mField22 = 0;
    mField23 = 1;
    mField24 = 0;
}

// ============================================================================
// CSkipTimer2 destructor
// ============================================================================
CSkipTimer2::~CSkipTimer2() {
    // member mMemRegion destroyed implicitly (retail emits external call)
}

// func_8029F2FC: reset the sub-controller.
void func_8029F2FC(CSkipTimer2* self) {
    self->mField20 = 0;
    if (self->mField18 != 0) {
        if (self->mField18 != 0) {
            self->mField18->vf2(1);
        }
        self->mField18 = 0;
    }
    self->mMemRegion.func_8045F778();
}

// func_8029F6EC: check forward-anim reached end -> state 5.
void func_8029F6EC(CSkipTimer2* self) {
    if (func_80137510(self->mAnimTransform, lbl_eu_80668C30) != 0) {
        self->mField21 = 5;
        func_8029F82C(self, self->mField24);
    }
}

// func_8029F73C: check forward-anim reached end -> ready state.
void func_8029F73C(CSkipTimer2* self) {
    if (func_80137444(self->mAnimTransform, lbl_eu_80668C30) != 0) {
        self->mField21 = 2;
        self->mField23 = 1;
    }
}

// func_8029F440: format slot name and stamp it into the layout pane.
void func_8029F440(CSkipTimer* self, int arg1, int arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_80510568[0x4a], arg1, arg2);
    func_80136A1C(self->mLayout, &lbl_eu_80510568[0x41], buf, 0);
}

// CSkipTimer constructor (not a harness target; scaffolding).
CSkipTimer::CSkipTimer() {
    mVtbl = lbl_eu_80539878;
    mParent = 0;
    mLayout = 0;
    mAnimTransform = 0;
    mAnimTransform20 = 0;
    mField24 = 0;
    mField28 = 0;
    mField29 = 0;
    mField2A = 0;
    mField2B = 1;
    mField2C = 0;
    mField2E = 0;
    mActive = 0;
}

// CSkipTimer destructor.
CSkipTimer::~CSkipTimer() {
    // Destruction order matches retail: mSkipTimer2 (0x70), CSysWin (0x34),
    // then mMemRegion (0x04, auto-destroyed last as first-declared member).
    reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0])->~CSkipTimer2();
    __dt__7CSysWinFv(&mSysWinData[0], -1);
}

// func_802A0234: forward-anim gate for CSkipTimer.
void func_802A0234(CSkipTimer* self) {
    if (func_80137444(self->mAnimTransform20, lbl_eu_80668C30) != 0) {
        self->mField29 = 2;
        func_802A05E4(self);
        // Function pointer prevents MWCC from inlining func_802A04F0
        // (retail keeps a direct `bl`).
        int (*fn)(CSkipTimer*) = func_802A04F0;
        int idx = fn(self);
        func_8029F364(reinterpret_cast<CSkipTimer2*>(&self->mSkipTimer2Data[0]),
                      static_cast<u8>(idx));
    }
}

// func_802A02D4: reverse-anim reached end -> clear skip state.
void func_802A02D4(CSkipTimer* self) {
    if (func_80137510(self->mAnimTransform20, lbl_eu_80668C30) != 0) {
        self->mField29 = 0;
        self->mField2B = 1;
    }
}

// func_802A04F0: map skip-timer index (mField2C) to an action id (1..7).
extern "C" int func_802A04F0(CSkipTimer* self) {
    switch (self->mField2C) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return 7;
    case 5:
        return 1;
    case 6:
    case 7:
    case 8:
    case 9:
        return 2;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        return 3;
    case 17:
        return 4;
    case 18:
        return 5;
    default:
        return 6;
    }
}

// --- remaining scaffolding stubs (not harness targets) ---

void func_8029F788(void* self) {
    if (*(u32*)((u8*)self + 0x14) != 0) {
        *(u8*)((u8*)self + 0x22) = 1;
        *(u8*)((u8*)self + 0x20) = 1;
    }
}

void func_8029F7A4(){}

void func_8029FBE0(){}
void func_8029FCDC(){}
void func_8029FDBC(){}
void func_8029FE30(){}
void func_8029FEBC(){}
void func_8029FF00(){}


void func_8029FF24(){}
void func_8029FF98(){}

void func_802A0008(void* obj) {
    unsigned char* bytes = (unsigned char*)obj;
    if (bytes[0x29] != 0) return;
    bytes[0x29] = 1;
    bytes[0x2b] = 0;
}

void func_802A0028(){}

void func_802A005C(){}

void func_802A0148(){}

void func_802A01F0(){}

void func_802A0298(){}

void func_802A02B8(){}

void func_802A0320(){}

void func_802A0368(){}

void func_802A03AC(){}

void func_802A041C(){}

void func_802A055C(){}

bool CSkipTimer::OnFileEvent(CEventFile* pEventFile) { return false; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_802A07D8(){}
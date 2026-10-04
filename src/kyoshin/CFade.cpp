// kyoshin/CFade
// FULL_MATCH: __ct__CFade, CFade_Update, CFade_IsReady, CFade_IsVisible,
// CFade_StepFadeIn, CFade_StepFadeOut, CFade_MarkLoaded.
// The dtor/OnFileEvent need stmw r30/r28 which requires -O4,s (walls #13);
// the source is byte-perfect apart from that fixed -O4,p save/restore split.

#include "kyoshin/CFade.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util.hpp"
#include "monolib/work/CEventFile.hpp"

u8 CFade::CFade_IsReady() { return mReady; }


u8 CFade::CFade_IsVisible() { return mVisible; }


// Retail marks the fade overlay loaded/ready once the layout is attached.
// noinline: retail keeps these as standalone `bl` targets (CFade_MarkLoaded is
// called from OnFileEvent; the others from CFade_Update). C linkage comes
// from the extern "C" declarations in CFade.hpp (unmangled retail reloc names).
__declspec(noinline) void CFade_MarkLoaded(CFade* self) {
    if (self->mLayout != nullptr) {
        self->mReady = 1;
        self->mIsLoaded = 1;
    }
}

// once the fade-in animation reaches the target frame, mark faded-in.
// once the fade-out animation rewinds, return to idle.
// noinline keeps the retail `bl` instead of inlining the body into
// CFade_Update. C linkage via CFade.hpp declarations (see above).
__declspec(noinline) void CFade_StepFadeIn(CFade* self) {
    if (advanceAnimTransform(self->mAnimTrans, lbl_eu_80668750) == 0) return;
    self->mFadeState = 2;
    self->mVisible = 1;
}

__declspec(noinline) void CFade_StepFadeOut(CFade* self) {
    if (AnimRewindFrame(self->mAnimTrans, lbl_eu_80668750) != 0) {
        self->mFadeState = 0;
        self->mVisible = 1;
    }
}

// The fade layout arc's file/branch/arc resource strings live at these offsets
// into the shared string pool (US/EU split1 .rodata):
//   +0x00 "/menu/jp/CFade.arc" (approx base)
//   +0x11 region tag for createRegion
//   +0x17 "arc"
//   +0x1B layout brlyt name
//   +0x2B animation brlan name
// Function-local optimize-for-size: retail compiles this TU at -O4,s (stmw
// r28/r30 multi-saves); the pragma reproduces that shape without changing the
// unit-level flags.
#pragma push
#pragma optimize_for_size on
bool CFade::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        // Cache the shared string pool base so MWCC keeps it in a callee-saved
        // register across the bl calls (retail: r30 + per-use addi offsets).
        char* str = lbl_eu_8050B5A0;

        // Reserve a scratch region inside the memory handle for the layout build,
        // then RAII-register it with CLibLayout (frees on scope exit).
        mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x400,
                                &str[0x11], 0);
        Class_8045F858 sp8(&mMemRegion);

        // Detach the loaded file buffer for the layout archive.
        u8* data = static_cast<u8*>(mFileHandle->getData());
        mtl::MemManager::setMemInitFlag(false);

        // Build the arc resource accessor and attach the archive buffer.
        mArcResAcc = CLibLayout::createArcResourceAccessor();
        mArcResAcc->Attach(data, &str[0x17]);

        // Load the fade layout and its animation transform.
        buildLayout(&mLayout, mArcResAcc, &str[0x1B]);
        bindLayoutAnimTransform(mLayout, &mAnimTrans, mArcResAcc, &str[0x2B]);

        // Enable the animation and kick off the first frame.
        mLayout->SetAnimationEnable(mAnimTrans, true);
        mLayout->Animate(0);

        // Mark the fade overlay loaded/ready now that the layout is attached.
        CFade_MarkLoaded(this);

        mFileHandle = nullptr;
        mMemRegion.validateHeap();
        return true;
    }
    return false;
}
#pragma pop

CFade::CFade() : CFadeVtblBase(), mMemRegion() {
    mFileHandle = nullptr;
    mArcResAcc = nullptr;
    mLayout = nullptr;
    mAnimTrans = nullptr;
    mIsLoaded = 0;
    mFadeState = 0;
    mReady = 0;
    mVisible = 1;
}

// destructor (member region + conditional delete are compiler-emitted).
#pragma push
#pragma optimize_for_size on
CFade::~CFade() {}
#pragma pop

// start an async read of the fade layout arc.
void CFade::CFade_StartLoad() {
    mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
        lbl_eu_8050B5A0, reinterpret_cast<IWorkEvent*>(this), 0, 0);
    CDeviceFile::setHandleFlag2(mFileHandle);
}

// per-frame update. Drive the fade-in/out animations, then refresh
// the layout's animation once loaded and not idle. The s32 copy of the byte
// state makes MWCC emit signed cmpi (retail), and the sparse switch gives the
// beq/fallthrough dispatch shape (if/else-if would invert the branches).
void CFade::CFade_Update() {
    if (mIsLoaded == 0) return;
    s32 state = mFadeState;
    if (state == 0) return;
    switch (state) {
    case 1:
        CFade_StepFadeIn(this);
        break;
    case 3:
        CFade_StepFadeOut(this);
        break;
    }
    mLayout->Animate(0);
}

// render the fade layout once loaded and fading.
void CFade::CFade_Draw(nw4r::lyt::DrawInfo* drawInfo) {
    if (mIsLoaded == 0) return;
    if (mFadeState == 0) return;
    drawLayout(mLayout, drawInfo, 0, 1);
}

// unload, freeing the file handle, layout and arc resources.
void CFade::CFade_Unload() {
    closeFileHandle(&mFileHandle);
    nw4r::lyt::Layout* layout = mLayout;
    mIsLoaded = 0;
    if (layout != nullptr) {
        delete layout;
        mLayout = nullptr;
    }
    releaseArcResourceAccessor(mArcResAcc);
    mArcResAcc = nullptr;
    mMemRegion.deleteRegion();
}

// start fading in if currently idle.
void CFade::CFade_FadeIn() {
    if (mFadeState == 0) {
        mFadeState = 1;
        mVisible = 0;
    }
}

// start fading out if fully faded in.
void CFade::CFade_FadeOut() {
    if (mFadeState == 2) {
        mFadeState = 3;
        mVisible = 0;
    }
}
// ============================================================================
// Retail data owned by this TU (dissolved blob; decomp emitted .text only).
// Defined here at EOF so -ipa codegen for the functions above is unaffected.
// NOTE (wave DL): the sdata2 tail ("CFade" pool + zeros after the 1.0f)
// stays out: only lbl_eu_80668750 carries a label and the section has no
// other references. sdata2 therefore rebuilds as 4/16 bytes (gate notes it).
// ============================================================================

// Cross-TU vtable slots (flat retail names; CTaskManager vtable idiom).
extern "C" void __dt__5CFadeFv();
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void WorkEvent4__10IWorkEventFv();
extern "C" void WorkEvent6__10IWorkEventFv();
extern "C" void WorkEvent7__10IWorkEventFv();
extern "C" void WorkEvent8__10IWorkEventFv();
extern "C" void WorkEvent9__10IWorkEventFv();
extern "C" void WorkEvent10__10IWorkEventFv();
extern "C" void WorkEvent11__10IWorkEventFv();
extern "C" void WorkEvent12__10IWorkEventFv();
extern "C" void WorkEvent13__10IWorkEventFv();
extern "C" void WorkEvent14__10IWorkEventFv();
extern "C" void WorkEvent15__10IWorkEventFv();
extern "C" void WorkEvent16__10IWorkEventFv();
extern "C" void WorkEvent17__10IWorkEventFv();
extern "C" void WorkEvent18__10IWorkEventFv();
extern "C" void WorkEvent19__10IWorkEventFv();
extern "C" void WorkEvent20__10IWorkEventFv();
extern "C" void WorkEvent21__10IWorkEventFv();
extern "C" void WorkEvent22__10IWorkEventFv();
extern "C" void WorkEvent23__10IWorkEventFv();
extern "C" void WorkEvent24__10IWorkEventFv();
extern "C" void WorkEvent25__10IWorkEventFv();
extern "C" void WorkEvent26__10IWorkEventFv();
extern "C" void WorkEvent27__10IWorkEventFv();
extern "C" void WorkEvent28__10IWorkEventFv();
extern "C" void WorkEvent29__10IWorkEventFv();
extern "C" void WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb();
extern u32 lbl_eu_806628E8[];  // sdata word pair (defined below).

// [.data] 0x80536EA8-0x80536F30 (136B): CFade vtable (sdata locator,
// null word, dtor, W1 + OnFileEvent + W3-W31; retail lays one label over
// the 34 words; the RTTI word at +0x88 carries its own label, below).
void* lbl_eu_80536EA8[34] = {
    (void*)&lbl_eu_806628E8, (void*)0,
    (void*)&__dt__5CFadeFv, (void*)&WorkEvent1__10IWorkEventFPvPCc,
    // W2 slot (OnFileEvent member address) is zero here: a member-function
    // address is unsourcable in C++ (10247); it needs a rule-side
    // inject_relocs entry like CTaskManager.o's three '<'-unspellable slots.
    // W3 likewise: no object defines the retail Fv spelling yet (CGame.o
    // carries the FPv arity instead -- signature dispute for its owner).
    (void*)0, (void*)0,
    (void*)&WorkEvent4__10IWorkEventFv, (void*)&OnPauseTrigger__10IWorkEventFb,
    (void*)&WorkEvent6__10IWorkEventFv,
    (void*)&WorkEvent7__10IWorkEventFv,
    (void*)&WorkEvent8__10IWorkEventFv,
    (void*)&WorkEvent9__10IWorkEventFv,
    (void*)&WorkEvent10__10IWorkEventFv,
    (void*)&WorkEvent11__10IWorkEventFv,
    (void*)&WorkEvent12__10IWorkEventFv,
    (void*)&WorkEvent13__10IWorkEventFv,
    (void*)&WorkEvent14__10IWorkEventFv,
    (void*)&WorkEvent15__10IWorkEventFv,
    (void*)&WorkEvent16__10IWorkEventFv,
    (void*)&WorkEvent17__10IWorkEventFv,
    (void*)&WorkEvent18__10IWorkEventFv,
    (void*)&WorkEvent19__10IWorkEventFv,
    (void*)&WorkEvent20__10IWorkEventFv,
    (void*)&WorkEvent21__10IWorkEventFv,
    (void*)&WorkEvent22__10IWorkEventFv,
    (void*)&WorkEvent23__10IWorkEventFv,
    (void*)&WorkEvent24__10IWorkEventFv,
    (void*)&WorkEvent25__10IWorkEventFv,
    (void*)&WorkEvent26__10IWorkEventFv,
    (void*)&WorkEvent27__10IWorkEventFv,
    (void*)&WorkEvent28__10IWorkEventFv,
    (void*)&WorkEvent29__10IWorkEventFv,
    (void*)&WorkEvent30__10IWorkEventFv,
    (void*)&WorkEvent31__10IWorkEventFv,
};

// [.data] 0x80536F30-0x80536F40 (16B): RTTI word + 12 zero pad bytes.
// Pragma-pinned to .data (plain zeros would bss-ify).
#pragma section data_type ".data" ".data"
extern __declspec(section ".data") u32 lbl_eu_80536F30[4] = {
    // RTTI word is zero here: __RTTI__10IWorkEvent cannot be named in source
    // (10322 vs the implicit type registration); needs a rule-side
    // inject_relocs entry like CTaskManager.o's unspellable slots.
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

// String labels (defined in the .rodata block below).
extern char lbl_eu_8050B5E8[], lbl_eu_8050B5F8[], lbl_eu_8050B608[], lbl_eu_8050B618[];
extern char lbl_eu_8050B628[], lbl_eu_8050B638[], lbl_eu_8050B648[], lbl_eu_8050B658[];
extern char lbl_eu_8050B668[], lbl_eu_8050B678[], lbl_eu_8050B688[], lbl_eu_8050B698[];
extern char lbl_eu_8050B6A8[], lbl_eu_8050B6B8[], lbl_eu_8050B6C8[], lbl_eu_8050B6D8[];
extern char lbl_eu_8050B6E8[], lbl_eu_8050B6F8[], lbl_eu_8050B708[], lbl_eu_8050B718[];
extern char lbl_eu_8050B728[], lbl_eu_8050B738[], lbl_eu_8050B748[], lbl_eu_8050B758[];
extern char lbl_eu_8050B768[], lbl_eu_8050B778[], lbl_eu_8050B788[];
// (rodata labels defined below; B708 is shared by two slots).
u32 lbl_eu_80536F40[28] = {
    (u32)&lbl_eu_8050B5E8,
    (u32)&lbl_eu_8050B5F8,
    (u32)&lbl_eu_8050B608,
    (u32)&lbl_eu_8050B618,
    (u32)&lbl_eu_8050B628,
    (u32)&lbl_eu_8050B638,
    (u32)&lbl_eu_8050B648,
    (u32)&lbl_eu_8050B658,
    (u32)&lbl_eu_8050B668,
    (u32)&lbl_eu_8050B678,
    (u32)&lbl_eu_8050B688,
    (u32)&lbl_eu_8050B698,
    (u32)&lbl_eu_8050B6A8,
    (u32)&lbl_eu_8050B6B8,
    (u32)&lbl_eu_8050B6C8,
    (u32)&lbl_eu_8050B6D8,
    (u32)&lbl_eu_8050B6E8,
    (u32)&lbl_eu_8050B6F8,
    (u32)&lbl_eu_8050B708,
    (u32)&lbl_eu_8050B708,
    (u32)&lbl_eu_8050B718,
    (u32)&lbl_eu_8050B728,
    (u32)&lbl_eu_8050B738,
    (u32)&lbl_eu_8050B748,
    (u32)&lbl_eu_8050B758,
    (u32)&lbl_eu_8050B768,
    (u32)&lbl_eu_8050B778,
    (u32)&lbl_eu_8050B788,
};
// [.rodata] 0x8050B5A0-0x8050B798 (0x1F8): fade layout arc/branch/anim
// strings + marker-name table. Retail holds one label per string; every
// start is 8-aligned so the toolchain places them with no padding.
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char gap_48_8050B5A0_rodata[0x48] = {
    0x6D,0x65,0x6E,0x75,0x2F,0x6A,0x70,0x2F,0x46,0x61,0x64,0x65,0x2E,0x61,0x72,0x63,
    0x00,0x43,0x46,0x61,0x64,0x65,0x00,0x61,0x72,0x63,0x00,0x6D,0x66,0x30,0x30,0x5F,
    0x66,0x61,0x64,0x65,0x2E,0x62,0x72,0x6C,0x79,0x74,0x00,0x6D,0x66,0x30,0x30,0x5F,
    0x66,0x61,0x64,0x65,0x5F,0x66,0x61,0x64,0x65,0x5F,0x69,0x6E,0x2E,0x62,0x72,0x6C,
    0x61,0x6E,0x00,0x00,0x00,0x00,0x00,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B5E8[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x30,0x30,0x30,0x30,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B5F8[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x30,0x31,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B608[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x30,0x32,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B618[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x30,0x33,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B628[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x30,0x34,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B638[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x30,0x34,0x30,0x32,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B648[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x30,0x35,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B658[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x30,0x36,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B668[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x30,0x37,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B678[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x30,0x38,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B688[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x30,0x39,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B698[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x31,0x30,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B6A8[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x31,0x31,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B6B8[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x31,0x32,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B6C8[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x31,0x32,0x30,0x32,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B6D8[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x31,0x33,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B6E8[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x31,0x34,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B6F8[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x31,0x35,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B708[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x31,0x36,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B718[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x31,0x37,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B728[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x31,0x38,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B738[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x31,0x39,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B748[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x32,0x30,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B758[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x32,0x31,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B768[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x32,0x32,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B778[0xF] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x32,0x33,0x30,0x31,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050B788[0x10] = {
    0x6D,0x61,0x72,0x6B,0x65,0x72,0x6C,0x69,0x73,0x74,0x32,0x34,0x30,0x31,0x00,0x00,
};

// [.sdata] 0x806628E8-0x806628F0 (8B): null word pair referenced by vtable[0].
#pragma section data_type ".sdata" ".sdata"
extern __declspec(section ".sdata") u32 lbl_eu_806628E8[2] = { 0x00000000, 0x00000000 };
#pragma section data_type ".data" ".data"

// [.sdata2] 0x80668750-0x80668760 (16B): 1.0f frame target (the "CFade" pool
// tail carries no label and has no references; only the float rebuilds here).
// [.sdata2] 1.0f frame target stays UNDEF (rule-side pool naming): defining
// it in-TU reschedules the StepFadeIn/Out float loads (100% -> 73.7%), and a
// scalar sdata2 def collides with this declaration (10322); an array def
// needs [0] reads with the same scheduling cost. The "CFade" pool tail
// carries no label and has no references.

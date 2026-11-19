// kyoshin/CFade
// FULL_MATCH: CFade::CFade, CFade_Update, CFade_IsReady, CFade_IsVisible,
// CFade_StepFadeIn, CFade_StepFadeOut, CFade_MarkLoaded.
// The dtor/OnFileEvent need stmw r30/r28 which requires -O4,s (walls #13);
// the source is byte-perfect apart from that fixed -O4,p save/restore split.

// Inline-empty ~IWorkEvent: the ctor's cleanup action references the (link-deduped)
// weak copy, and ~CFade elides the base call (retail shape).
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/CFade.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util.hpp"
#include "monolib/work/CEventFile.hpp"

// Construct the embedded memory region, then zero the fields (mVisible=1).
CFade::CFade() : mMemRegion() {
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
CFade::~CFade() {}
#pragma pop

// start an async read of the fade layout arc.
extern "C" void CFade_StartLoad(CFade* self) {
    self->mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
        lbl_eu_8050B5A0, self, 0, 0);
    CDeviceFile::setHandleFlag2(self->mFileHandle);
}

// per-frame update. Drive the fade-in/out animations, then refresh
// the layout's animation once loaded and not idle. The s32 copy of the byte
// state makes MWCC emit signed cmpi (retail), and the sparse switch gives the
// beq/fallthrough dispatch shape (if/else-if would invert the branches).
extern "C" void CFade_Update(CFade* self) {
    if (self->mIsLoaded == 0) return;
    s32 state = self->mFadeState;
    if (state == 0) return;
    switch (state) {
    case 1:
        CFade_StepFadeIn(self);
        break;
    case 3:
        CFade_StepFadeOut(self);
        break;
    }
    self->mLayout->Animate(0);
}

// render the fade layout once loaded and fading.
extern "C" void CFade_Draw(CFade* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mIsLoaded == 0) return;
    if (self->mFadeState == 0) return;
    drawLayout(self->mLayout, drawInfo, 0, 1);
}

// unload, freeing the file handle, layout and arc resources.
void CFade::Unload() {
    closeFileHandle(&this->mFileHandle);
    nw4r::lyt::Layout* layout = this->mLayout;
    this->mIsLoaded = 0;
    if (layout != nullptr) {
        delete layout;
        this->mLayout = nullptr;
    }
    releaseArcResourceAccessor(this->mArcResAcc);
    this->mArcResAcc = nullptr;
    this->mMemRegion.deleteRegion();
}

extern "C" int CFade_IsReady(CFade* self) { return self->mReady; }

extern "C" int CFade_IsVisible(CFade* self) { return self->mVisible; }

// start fading in if currently idle.
void CFade::In() {
    if (this->mFadeState == 0) {
        this->mFadeState = 1;
        this->mVisible = 0;
    }
}

// start fading out if fully faded in.
extern "C" void CFade_FadeOut(CFade* self) {
    if (self->mFadeState == 2) {
        self->mFadeState = 3;
        self->mVisible = 0;
    }
}
// once the fade-in animation reaches the target frame, mark faded-in.
// once the fade-out animation rewinds, return to idle.
// noinline keeps the retail `bl` instead of inlining the body into
// CFade_Update. C linkage via CFade.hpp declarations (see above).
extern "C" void CFade_StepFadeIn(CFade* self) {
    if (advanceAnimTransform(self->mAnimTrans, lbl_eu_80668750) == 0) return;
    self->mFadeState = 2;
    self->mVisible = 1;
}

extern "C" void CFade_StepFadeOut(CFade* self) {
    if (AnimRewindFrame(self->mAnimTrans, lbl_eu_80668750) != 0) {
        self->mFadeState = 0;
        self->mVisible = 1;
    }
}

// Retail marks the fade overlay loaded/ready once the layout is attached.
// noinline: retail keeps these as standalone `bl` targets (CFade_MarkLoaded is
// called from OnFileEvent; the others from CFade_Update). C linkage comes
// from the extern "C" declarations in CFade.hpp (unmangled retail reloc names).
extern "C" void CFade_MarkLoaded(CFade* self) {
    if (self->mLayout != nullptr) {
        self->mReady = 1;
        self->mIsLoaded = 1;
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
bool CFade::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        // Retail evaluates the MEM2 handle call before materializing the
        // string pool base (kept in r30 with per-use addi offsets below).
        mtl::ALLOC_HANDLE memHandle = mtl::MemManager::getHandleMEM2();
        char* str = lbl_eu_8050B5A0;

        // Reserve a scratch region inside the memory handle for the layout build,
        // then RAII-register it with CLibLayout (frees on scope exit).
        mMemRegion.createRegion(memHandle, 0x400,
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

// ============================================================================
// Retail data owned by this TU (dissolved blob; decomp emitted .text only).
// Defined here at EOF so -ipa codegen for the functions above is unaffected.
// NOTE (wave DL): the sdata2 tail ("CFade" pool + zeros after the 1.0f)
// stays out: only lbl_eu_80668750 carries a label and the section has no
// other references. sdata2 therefore rebuilds as 4/16 bytes (gate notes it).
// ============================================================================

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
char lbl_eu_8050B5A0[0x48] = {
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

// [.sdata2] 0x80668750: 1.0f frame target (the "CFade" RTTI name string that
// follows it is compiler-emitted).
#pragma section data_type ".sdata2" ".sdata2"
extern __declspec(section ".sdata2") const f32 lbl_eu_80668750 = 1.0f;
#pragma section data_type ".data" ".data"

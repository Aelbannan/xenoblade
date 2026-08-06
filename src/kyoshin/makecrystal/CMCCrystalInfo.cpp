// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCCrystalInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/makecrystal/CMCCrystalInfo.hpp"

namespace nw4r { namespace lyt { class Layout; class DrawInfo; class AnimTransform; } }

extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(
    nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);

extern "C" void func_8021B52C();
extern "C" void func_8021B5B4();
extern "C" void func_8021B63C();
extern "C" void func_8021B6C4();

extern "C" u32 getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" char lbl_eu_80508DF8[];

extern "C" void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dl__FPv(void*);

// The retail ctor is emitted under the unmangled symbol `__ct__CMCCrystalInfo`
// (not a mangled member name), so it is written as a C-linkage function that
// explicitly sets the vptr and constructs the UnkClass sub-objects.
extern "C" CMCCrystalInfo* __ct__CMCCrystalInfo(CMCCrystalInfo* self)
{
    *(void**)self = (void*)lbl_eu_80535CF8;   // vptr at +0x00
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion1);
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion2);

    self->mFileHandle1 = 0;
    self->mFileHandle2 = 0;
    self->mArcResAccessor = 0;
    self->mField30 = 0;
    self->mLayout = 0;
    self->mAnimTransform1 = 0;
    self->mAnimTransform2 = 0;
    self->mAnimTransform3 = 0;
    self->mAnimTransform4 = 0;
    self->mEnabled = 0;
    self->mState = 0;
    self->mIsActive = 0;
    self->mField51 = 1;
    return self;
}

// The retail dtor is emitted under the mangled symbol `__dt__14CMCCrystalInfoFv`.
// Written as a C-linkage function (CItemBoxInfo precedent) for byte-exact
// control over the null/delete-flag checks.
extern "C" CMCCrystalInfo* __dt__14CMCCrystalInfoFv(CMCCrystalInfo* self, s32 flags)
{
    if (self != NULL) {
        __dt__17UnkClass_8045F564Fv(&self->mMemRegion2, -1);
        __dt__17UnkClass_8045F564Fv(&self->mMemRegion1, -1);
        if (flags > 0) __dl__FPv(self);
    }
    return self;
}

void func_8021A718(CMCCrystalInfo* self)
{
    // Load the two crystal-info layout files (names at lbl_eu_80508DF8/+0x1a)
    u32 handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->mFileHandle1 = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        handle, &lbl_eu_80508DF8[0], self, 0, 0);
    handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->mFileHandle2 = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        handle, &lbl_eu_80508DF8[0x1a], self, 0, 0);
}

void func_8021A780(){}

void func_8021A840(CMCCrystalInfo* self, nw4r::lyt::DrawInfo* drawInfo)
{
    if (self->mEnabled) {
        func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(
            (nw4r::lyt::Layout*)self->mLayout, drawInfo, 0, 1);
    }
}

void func_8021A860(){}

// Converted to inline member function in header

void func_8021A8F4(CMCCrystalInfo* self)
{
    if (self->mState == 0) {
        self->mState = 1;
        self->mField51 = 0;
        return func_8021B52C();
    }
}

void func_8021A918(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 4;
        self->mField51 = 0;
        return func_8021B5B4();
    }
}

void func_8021A93C(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 6;
        self->mField51 = 0;
        return func_8021B6C4();
    }
}

void func_8021A960(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 7;
        self->mField51 = 0;
        return func_8021B63C();
    }
}

void func_8021A984(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 8;
        self->mField51 = 0;
        return func_8021B63C();
    }
}

void func_8021A9A8(){}

void func_8021AA9C(){}

void func_8021ADC4(){}

void func_8021AED0(){}

void func_8021AF74(CMCCrystalInfo* self)
{
    if (func_80137444((nw4r::lyt::AnimTransform*)self->mAnimTransform1, 1.0f)) {
        self->mState = 2;
        return func_8021B5B4();
    }
}

void func_8021AFC0(CMCCrystalInfo* self)
{
    if (func_80137444((nw4r::lyt::AnimTransform*)self->mAnimTransform2, 1.0f)) {
        self->mState = 3;
        self->mField51 = 1;
    }
}

void func_8021B00C(CMCCrystalInfo* self)
{
    if (func_80137510((nw4r::lyt::AnimTransform*)self->mAnimTransform2, 1.0f)) {
        self->mState = 5;
        return func_8021B52C();
    }
}

void func_8021B058(CMCCrystalInfo* self)
{
    if (func_80137510((nw4r::lyt::AnimTransform*)self->mAnimTransform1, 1.0f)) {
        self->mState = 0;
        self->mField51 = 1;
    }
}

void func_8021B0A4(CMCCrystalInfo* self)
{
    if (func_80137444((nw4r::lyt::AnimTransform*)self->mAnimTransform4, 1.0f)) {
        self->mField51 = 1;
        self->mState = 0;
    }
}

void func_8021B0F0(CMCCrystalInfo* self)
{
    if (func_80137444((nw4r::lyt::AnimTransform*)self->mAnimTransform3, 1.0f)) {
        self->mField51 = 1;
        self->mState = 3;
    }
}

void func_8021B13C(CMCCrystalInfo* self)
{
    if (func_80137510((nw4r::lyt::AnimTransform*)self->mAnimTransform3, 1.0f)) {
        self->mField51 = 1;
        self->mState = 3;
    }
}

void func_8021B188(){}

void func_8021B2E0(){}

void func_8021B42C(){}

void func_8021B500(void* this_) {
    unsigned int* p34 = reinterpret_cast<unsigned int*>(static_cast<char*>(this_) + 0x34);
    unsigned int* p30 = reinterpret_cast<unsigned int*>(static_cast<char*>(this_) + 0x30);
    if (*p34 != 0) {
        if (*p30 != 0) {
            *reinterpret_cast<unsigned char*>(static_cast<char*>(this_) + 0x48) = 1;
            *reinterpret_cast<unsigned char*>(static_cast<char*>(this_) + 0x50) = 1;
        }
    }
}

// Placeholder bodies for unmatched callee stubs. A genuinely empty body lets MWCC
// fold the call (same-TU definition), so the callers' `b`/`bl` would vanish.
// The real bodies are separate work items; these must carry a non-foldable effect
// while matched callers are verified.
static volatile u32 s_stubSink;

// noinline forces a real branch call in the matched callers instead of the
// stub body being inlined into them. extern "C" keeps the retail (unmangled)
// linker names for the callers' tail branches.
extern "C" __declspec(noinline) void func_8021B52C() { s_stubSink = 1; }

extern "C" __declspec(noinline) void func_8021B5B4() { s_stubSink = 1; }

extern "C" __declspec(noinline) void func_8021B63C() { s_stubSink = 1; }

extern "C" __declspec(noinline) void func_8021B6C4() { s_stubSink = 1; }

bool CMCCrystalInfo::OnFileEvent(CEventFile* pEventFile) { return false; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8021BBC4(){}

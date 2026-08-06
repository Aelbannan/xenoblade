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

void __ct__CMCCrystalInfo(){}

CMCCrystalInfo::~CMCCrystalInfo() {}

void func_8021A718(){}

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

void func_8021B0A4(){}

void func_8021B0F0(){}

void func_8021B13C(){}

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

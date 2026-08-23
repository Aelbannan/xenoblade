// Auto-scaffolded catalog TU for kyoshin/code_801A929C
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/CVec3.hpp"

// SDA/sdata2 float pool labels used by the direction helpers below.
extern const f32 lbl_eu_80667D60; // 0.0f
extern const f32 lbl_eu_80667D64; // 1.0f
extern const f32 lbl_eu_80667D68;
extern const f32 lbl_eu_80667D6C; // rad -> FIdx degrees
extern const f32 lbl_eu_80667D70; // FIdx degrees multiplier for SinFIdx
extern const f32 lbl_eu_8066A1F8; // pi
extern const f32 lbl_eu_8066A1FC; // 2*pi

// Global runtime-flag word (bit 0x2000 = bit 13 gates target selection).
extern u32 lbl_eu_80663E24;

// Retail C-symbol import: camera/scene gate query.
extern "C" int func_8007BAE4();

extern "C" void __dl__FPv(void*);

void func_801A9338(void* self) {
    *(unsigned short*)((char*)self + 0x2c) = 0xFFFF;
}

// ----------------------------------------------------------------------------
// func_801A9348 - compute a blend weight and attack direction toward a target.
// Weight falls linearly from 1 to 0 between the outer/inner radii; direction
// comes either from the camera-relative matrix transform or, when the shared
// target singleton is active, from wrapped atan2 angle differences.
// ----------------------------------------------------------------------------
// Source object carrying a transform matrix (+0xCC) and position (+0x10C).
struct DirSrc {
    u8 _00[0xCC];
    u8 mMatrix[0x40]; // +0xCC, Mtx44-style 3x4 matrix
    ml::CVec3 mPos;   // +0x10C
};

// Helper: view a raw byte block as an SDK matrix.
inline Mtx* mtxOf(u8* p) { return (Mtx*)p; }

// Shared target singleton view: current + previous positions and active flag.
struct DirTargetInfo {
    ml::CVec3 m00;  // +0x00
    ml::CVec3 m0C;  // +0x0C
    u8 mFlag18;     // +0x18
};

void func_801A9348(ml::CVec3* outDir, float* outWeight, DirSrc* src,
                   ml::CVec3* posB, ml::CVec3* targetPos,
                   float innerRadius, float outerRadius) {
    extern unsigned char lbl_eu_80664330;
    DirTargetInfo* pTarget = *(DirTargetInfo**)(&lbl_eu_80664330);

    // Choose the aim target point: blended point, singleton target, or source.
    u8 useTargetObj = 0;
    ml::CVec3 aim;
    if ((lbl_eu_80663E24 & 0x2000) == 0 && func_8007BAE4() == 0) {
        aim = (src->mPos + *posB) * lbl_eu_80667D68;
    } else if (pTarget != 0 && pTarget->mFlag18 != 0) {
        useTargetObj = 1;
        aim = pTarget->m00;
    } else {
        aim = src->mPos;
    }

    ml::CVec3 diff = *targetPos - aim;
    float mag = PSVECMag((const Vec*)&diff);

    // Linear falloff: 0 at/below innerRadius, 1 at/beyond outerRadius.
    *outWeight = lbl_eu_80667D64;
    if (mag < outerRadius) {
        if (mag < innerRadius) {
            *outWeight = lbl_eu_80667D60;
        } else {
            *outWeight =
                lbl_eu_80667D60 - (mag - innerRadius) / (outerRadius - innerRadius);
        }
    }

    if (useTargetObj != 0) {
        // Direction from the signed difference of two heading angles around
        // the target object, wrapped into [-pi, pi].
        ml::CVec3 v1 = pTarget->m0C - pTarget->m00;
        float angSelf = lbl_eu_80667D6C * nw4r::math::Atan2FIdx(v1.x, v1.z);
        ml::CVec3 v2 = *targetPos - pTarget->m0C;
        float angGoal = lbl_eu_80667D6C * nw4r::math::Atan2FIdx(v2.x, v2.z);
        float delta = angGoal - angSelf;
        while (lbl_eu_8066A1F8 <= delta) delta -= lbl_eu_8066A1FC;
        while (delta < -lbl_eu_8066A1F8) delta += lbl_eu_8066A1FC;
        outDir->x = -nw4r::math::SinFIdx(lbl_eu_80667D70 * delta);
    } else {
        // Transform the target through the source matrix and normalize; fall
        // back to the zero vector when the length squared matches exactly.
        ml::CVec3 dir;
        PSMTXMultVec(*mtxOf(src->mMatrix), (Vec*)targetPos, (Vec*)&dir);
        float lenSq = dir.x * dir.x + (dir.y * dir.y + dir.z * dir.z);
        if (lenSq == lbl_eu_80667D64) {
            dir = ml::CVec3::zero;
        } else {
            PSVECNormalize((Vec*)&dir, (Vec*)&dir);
        }
        outDir->x = dir.x;
    }
}

void func_801A96A0(void*){}

// ----------------------------------------------------------------------------
// func_801A9CCC - construct the 128-entry state work object: fill the header
// floats/flags, default-initialize every entry, then invoke the shared
// member-function callback (pointer-to-member-function blob in .data) on all
// 128 entries and publish the object as the singleton.
// ----------------------------------------------------------------------------
extern u32 lbl_eu_805333E0[3];

struct CtrlState {
    u8 _00[0x1C];
    f32 m1C; // +0x1C
    f32 m20; // +0x20
    u8 pad24[4];
    f32 m28; // +0x28
    s16 m2C; // +0x2C, init -1
    s16 m2E;
    s16 m30;
    u8 m32;
    u8 pad33[5];
};

class CtrlStateWork {
public:
    f32 m00; // +0x00
    f32 m04; // +0x04
    f32 m08;
    f32 m0C;
    f32 m10;
    f32 m14;
    u8 m18; // +0x18, cleared
    u8 m19;
    u8 m1A;
    u8 m1B;
    u16 m1C; // +0x1C, set to 1
    u8 pad1E[2];
    CtrlState mEntries[128]; // +0x20 .. +0x1C20
};

typedef void (CtrlState::*CtrlStateFn)();

// Word-level view so the three ptmf words load without a memcpy call.
union CtrlStateFnBits {
    CtrlStateFn fn;
    u32 raw[3];
};

CtrlStateWork* func_801A9CCC(CtrlStateWork* work) {
    work->m00 = lbl_eu_80667D64;
    work->m04 = lbl_eu_80667D60;
    work->m08 = lbl_eu_80667D64;
    work->m0C = lbl_eu_80667D64;
    work->m10 = lbl_eu_80667D64;
    work->m14 = lbl_eu_80667D64;
    work->m18 = 0;
    work->m19 = 0;
    work->m1A = 0;
    work->m1B = 0;
    work->m1C = 1;

    int i;
    for (i = 0; i < 128; i++) {
        CtrlState* it = &work->mEntries[i];
        it->m1C = lbl_eu_80667D60;
        it->m20 = lbl_eu_80667D64;
        it->m28 = lbl_eu_80667D60;
        it->m2C = -1;
        it->m2E = 0;
        it->m30 = 0;
        it->m32 = 0;
    }

    work->m1C = 1;
    work->m18 = 0;

    CtrlStateFnBits cb;
    cb.raw[0] = lbl_eu_805333E0[0];
    cb.raw[1] = lbl_eu_805333E0[1];
    cb.raw[2] = lbl_eu_805333E0[2];
    CtrlStateFnBits cbCall;
    cbCall.fn = cb.fn;
    CtrlState* end = &work->mEntries[128];
    for (CtrlState* it = work->mEntries; it != end; ++it) {
        (it->*cbCall.fn)();
    }

    // Singleton pointer (declared as raw bytes elsewhere in this TU).
    extern unsigned char lbl_eu_80664330;
    *(CtrlStateWork**)(&lbl_eu_80664330) = work;
    return work;
}

// C linkage inherited from the CfGameManager.hpp declaration; repeating
// extern "C" here trips MWCC error 10197.
void* __dt__801A9F78(void* self, int dealloc) {
    if (self != nullptr) {
        extern unsigned char lbl_eu_80664330;
        *(void**)(&lbl_eu_80664330) = 0;
        if (dealloc > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ----------------------------------------------------------------------------
// func_801A9FC0 - run the shared member-function callback over all 128 work
// entries and reset the entry-count/flag header fields.
// ----------------------------------------------------------------------------
struct CtrlEntry {
    u8 mRaw[0x38];
};

class CtrlWork {
public:
    u8 _00[0x18];
    u8 mField18; // +0x18, cleared
    u8 _19[3];
    u16 mField1C; // +0x1C, set to 1
    CtrlEntry mEntries[128]; // +0x20 .. +0x1C20
};

typedef void (CtrlEntry::*CtrlEntryFn)();

// 12-byte pointer-to-member-function in .data, invoked on every entry.
extern u32 lbl_eu_805333EC[3];

// Word-level view so the three ptmf words load without a memcpy call.
union CtrlEntryFnBits {
    CtrlEntryFn fn;
    u32 raw[3];
};

void func_801A9FC0(CtrlWork* work) {
    work->mField1C = 1;
    work->mField18 = 0;
    CtrlEntryFnBits cb;
    cb.raw[0] = lbl_eu_805333EC[0];
    cb.raw[1] = lbl_eu_805333EC[1];
    cb.raw[2] = lbl_eu_805333EC[2];
    CtrlEntry* end = &work->mEntries[128];
    for (CtrlEntry* it = work->mEntries; it != end; ++it) {
        (it->*cb.fn)();
    }
}

void func_801AA04C(){}

void func_801AA2A8(){}

int func_801AA960(int, int b, int c, int d) {
    // Else-if chain over the mode: retail presets r3=0, exits immediately on
    // a match, and funnels the last test through the shared epilogue.
    int result = 0;
    if (d == 1 && b == 1) result = 1;
    else if (d == 2 && b == 2) result = 1;
    else if (d == 3 && b == 3) result = 1;
    else if (d == 4 && b == 4) result = 1;
    else if (d == 5 && (unsigned int)c <= 2) result = 1;
    else if (d == 6 && (unsigned int)(c - 3) <= 2) result = 1;
    else if (d == 7 && (unsigned int)(c - 6) <= 2) result = 1;
    else if (d == 8 && (unsigned int)(c - 9) <= 2) result = 1;
    else if (d == 9 && (unsigned int)(c - 0xc) <= 2) result = 1;
    else if (d == 0xa && (unsigned int)(c - 0xf) <= 2) result = 1;
    else if (d == 0xb && (unsigned int)(c - 0x12) <= 2) result = 1;
    else if (d == 0xc) {
        // Nested form keeps retail's bnelr/bgtlr epilogue funnelling.
        if ((unsigned int)(c - 0x15) <= 2) result = 1;
    }
    return result;
}


void func_801AAAA0(){}

void func_801AAB64(){}

extern "C" void func_801AAB64(void* a, void* b, int c, float d);
extern "C" void func_801AAC70(void* self){ void* d; float dummy; func_801AAB64(self, d, 1, dummy); }

extern "C" void func_801AAC78(u8 v) {
    extern unsigned char lbl_eu_80664330;
    void* p = *(void**)(&lbl_eu_80664330);
    if (p == 0) return;
    *(u8*)((u8*)p + 0x1A) = v;
    if (v == 0) return;
    func_801AAB64(0, 0, 0, lbl_eu_80667D64);
}

extern "C" void func_801AACA8(unsigned char v) {
    extern unsigned char lbl_eu_80664330;
    void* p = *(void**)(&lbl_eu_80664330);
    if (p) *(unsigned char*)((u8*)p + 0x1b) = v;
}

void func_801AACBC(unsigned int* r3, unsigned int* r4) {
    extern unsigned char lbl_eu_80664330;
    unsigned int *p = (unsigned int *)&lbl_eu_80664330;
    if (!*p) return;
    unsigned int *dst = (unsigned int *)*p;
    unsigned int *src1 = r3;
    unsigned int *src2 = r4;
    dst[0] = src1[0];
    dst[1] = src1[1];
    dst[2] = src1[2];
    dst[3] = src2[0];
    dst[4] = src2[1];
    dst[5] = src2[2];
    ((unsigned char *)dst)[0x18] = 1;
}

extern "C" void func_801AAD08() {
    extern unsigned char lbl_eu_80664330;
    void* p = *(void**)(&lbl_eu_80664330);
    if (p) *(unsigned char*)((u8*)p + 0x18) = 0;
}

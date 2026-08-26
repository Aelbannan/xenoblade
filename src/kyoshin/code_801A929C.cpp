// Auto-scaffolded catalog TU for kyoshin/code_801A929C
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/plugin/ocBdat.hpp"  // getBdatStringColumnValue
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// SDA/sdata2 float pool labels used by the direction helpers below.
extern const f32 lbl_eu_80667D60; // 0.0f
extern const f32 lbl_eu_80667D64; // 1.0f
extern const f32 lbl_eu_80667D68;
extern const f32 lbl_eu_80667D6C; // rad -> FIdx degrees
extern const f32 lbl_eu_80667D70; // FIdx degrees multiplier for SinFIdx
extern const f32 lbl_eu_80667D78; // bdat column float scale
extern const f64 lbl_eu_80667D80; // 0x4330 double-conversion bias A
extern const f64 lbl_eu_80667D88; // 0x4330 double-conversion bias B
extern const f32 lbl_eu_8066A1F8; // pi
extern const f32 lbl_eu_8066A1FC; // 2*pi

// Global runtime-flag word (bit 0x2000 = bit 13 gates target selection).

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

    // Default-initialize all 128 entries.
    CtrlState* it = &work->mEntries[0];
    CtrlState* tail = &work->mEntries[128];
    while (it != tail) {
        it->m1C = lbl_eu_80667D64;
        it->m20 = lbl_eu_80667D60;
        it->m28 = lbl_eu_80667D64;
        it->m2C = -1;
        it->m2E = 0;
        it->m30 = 0;
        it->m32 = 0;
        ++it;
    }

    work->m1C = 1;
    work->m18 = 0;

    // Load the shared pointer-to-member-function callback and invoke it on
    // every entry.
    CtrlStateFnBits cb;
    cb.raw[0] = lbl_eu_805333E0[0];
    cb.raw[1] = lbl_eu_805333E0[1];
    cb.raw[2] = lbl_eu_805333E0[2];
    CtrlStateFn fn = cb.fn;
    CtrlStateFnBits cbCall;
    cbCall.fn = fn;
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
    u16 _1E; // padding so entries start at +0x20
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
    // Single aggregate copy of the 12-byte ptmf from .data.
    cb = *(CtrlEntryFnBits*)lbl_eu_805333EC;
    // Second ptmf copy: __ptmf_scall consumes a stack temp, matching retail.
    CtrlEntryFnBits cbCall;
    cbCall.fn = cb.fn;
    CtrlEntry* end = &work->mEntries[128];
    for (CtrlEntry* it = work->mEntries; it != end; ++it) {
        (it->*cbCall.fn)();
    }
}

// ----------------------------------------------------------------------------
// func_801AA04C - fill the ctrl-state work entries from the shared bdat table:
// for every row of the table's row range, read named columns (rodata name blob
// at lbl_eu_80503FA0) into one 0x38-byte entry slot of the caller's array.
// Integer columns are converted to float through the 0x43300000 double trick:
// the union high words are preset once up front, each value is placed (with
// the sign-bit xor for signed columns) into the low word, the double bias is
// subtracted, and the result scaled by lbl_eu_80667D78.
// ----------------------------------------------------------------------------

// Conversion scratch: w[0] = 0x43300000, w[1] holds the integer payload.
union BdatConv {
    u32 w[2];
    f64 d;
};

// Column-value view: retail spills every narrow-column call result to its own
// stack word, then re-reads it narrowed (stw/lhz or stw/lbz round-trip).
union ColVal {
    u32 v;
    u16 h[2];
    u8 b[4];
};

// Two views of the moving 0x38-byte slot: the three leading floats go through
// the head cursor, everything else through offsets 0x38.. of the base cursor
// (retail keeps both pointers live across the whole loop).
struct CtrlRecHead {
    f32 f00;
    f32 f04;
    f32 f08;
};

struct CtrlRecTail {
    u8 pad00[0x18];
    f32 f38;
    u8 pad3C[8];
    f32 f44;
    u16 f4C;
    u16 f4E;
    u16 f50;
    u8 f53;
    u8 f54;
    u8 f55;
    u8 f56;
    u8 f57;
};

// Bdat column-name blob (split1 rodata), byte-offset indexed.
extern char lbl_eu_80503FA0[];

// Bdat table handle owning the ctrl-state row range (import from split1).
struct BdatTable;
extern BdatTable* lbl_eu_806640B8;

void func_801AA04C(void* param) {
    BdatTable* tbl = lbl_eu_806640B8;
    BdatConv convA;
    BdatConv convB;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;
    s32 i = func_8003B41C(tbl);   // first row of the range
    s32 end = i + func_8003B1EC(tbl); // number of rows
    const char* cols = lbl_eu_80503FA0;

    CtrlRecHead* rec = (CtrlRecHead*)((u8*)param + i * 0x38);
    CtrlRecTail* tail = (CtrlRecTail*)((u8*)param + i * 0x38);

    for (; i < end; i++) {
        // Local struct: field stores keep stack homes across the calls
        // (retail stfs to sp), and the final copy is three lwz/stw moves.
        CtrlRecHead h;
        convA.w[1] = getBdatStringColumnValue(tbl, cols + 0x1b, i) ^ 0x8000;
        h.f00 = (f32)(convA.d - lbl_eu_80667D80) * lbl_eu_80667D78;
        convB.w[1] = getBdatStringColumnValue(tbl, cols + 0x20, i) ^ 0x8000;
        h.f04 = (f32)(convB.d - lbl_eu_80667D80) * lbl_eu_80667D78;
        convA.w[1] = getBdatStringColumnValue(tbl, cols + 0x2a, i) ^ 0x8000;
        h.f08 = (f32)(convA.d - lbl_eu_80667D80) * lbl_eu_80667D78;
        *rec = h;

        ColVal t31;
        t31.v = getBdatStringColumnValue(tbl, cols + 0x31, i);
        convB.w[1] = t31.h[0];
        tail->f38 = (f32)(convB.d - lbl_eu_80667D88);
        tail->f50 = (u16)i;
        ColVal t37;
        t37.v = getBdatStringColumnValue(tbl, cols + 0x37, i);
        tail->f4E = t37.h[0];
        ColVal t3d;
        t3d.v = getBdatStringColumnValue(tbl, cols + 0x3d, i);
        tail->f53 = t3d.b[0];
        ColVal t42;
        t42.v = getBdatStringColumnValue(tbl, cols + 0x42, i);
        tail->f56 = t42.b[0];
        ColVal t49;
        t49.v = getBdatStringColumnValue(tbl, cols + 0x49, i);
        convA.w[1] = t49.b[0];
        tail->f44 = (f32)(convA.d - lbl_eu_80667D88) * lbl_eu_80667D78;
        ColVal t50;
        t50.v = getBdatStringColumnValue(tbl, cols + 0x50, i);
        tail->f54 = t50.b[0];
        ColVal t57;
        t57.v = getBdatStringColumnValue(tbl, cols + 0x57, i);
        tail->f55 = t57.b[0];
        tail->f4C = 0xFFFF;

        rec = (CtrlRecHead*)((char*)rec + 0x38);
        tail = (CtrlRecTail*)((char*)tail + 0x38);
    }
}

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


// Bdat row queries; canonical extern "C" decls live in CfObjectEne.hpp /
// CItem.hpp, whose full include closure clashes with this TU.
extern "C" u32 func_8003B41C(void* bdat);   // bdat first row
extern "C" u32 func_8003B1EC(void* bdat);   // bdat row count
// Game-manager active gate; canonical member is cf::CfGameManager::func_80082900.
extern "C" u32 func_80082900__Q22cf13CfGameManagerFv();
// Sound-slot play entry (defined in CfSoundMan.cpp); C linkage so the call
// reloc binds to the retail-unmangled name.
extern "C" void func_801BFE58(s32 idx, u32 a, u32 b, float volume);

// Bdat table handle owning the ctrl-state row range (import from split1).
struct BdatTable;
extern BdatTable* lbl_eu_806640B8;

// View of one 0x38-byte ctrl-state entry, exposing the match key at +0x2E.
struct ScanEntry {
    u8 _00[0x2E];
    u16 m2E; // +0x2E
};

// ----------------------------------------------------------------------------
// func_801AAAA0 - scan the shared ctrl-state singleton's entry table starting
// at the bdat table's first-row index for the table's row count, returning
// whether any entry's key (+0x2E) equals id.
// ----------------------------------------------------------------------------
bool func_801AAAA0(u32 id) {
    if (func_80082900__Q22cf13CfGameManagerFv() == 0) {
        return false;
    }
    extern unsigned char lbl_eu_80664330;
    BdatTable* table = lbl_eu_806640B8;
    if (*(void**)(&lbl_eu_80664330) == nullptr || table == nullptr) {
        return false;
    }
    s32 row = func_8003B41C(table);   // first row of the range
    s32 count = func_8003B1EC(table); // number of rows
    // Re-read the singleton: scan its entry table from 'row' for 'count' rows.
    CtrlStateWork* work2 = *(CtrlStateWork**)(&lbl_eu_80664330);
    s32 endRow = row + count;
    ScanEntry* it = (ScanEntry*)&work2->mEntries[row];
    for (; row < endRow; row++) {
        if (id == it->m2E) {
            return true;
        }
        it = (ScanEntry*)((char*)it + 0x38);
    }
    return false;
}

// ----------------------------------------------------------------------------
// func_801AAB64 - apply a sound-volume update over the shared ctrl-state
// singleton's entry range (bdat table rows). For every entry whose key (+0x30)
// equals id (or when id == 0, all entries) and whose sound index (+0x2C) is
// valid, play index scaled by the entry's volume (+0x24) times scale. When
// store is set, persist scale and kind into the entry.
// The singleton global is re-read before each access, matching retail.
// ----------------------------------------------------------------------------
void func_801AAB64(u32 id, u32 kind, int store, float scale) {
    extern unsigned char lbl_eu_80664330;
    if (func_80082900__Q22cf13CfGameManagerFv() == 0) {
        return;
    }
    BdatTable* table = lbl_eu_806640B8;
    if (*(void**)(&lbl_eu_80664330) == nullptr || table == nullptr) {
        return;
    }
    s32 row = func_8003B41C(table);   // first row of the range
    s32 endRow = row + func_8003B1EC(table); // number of rows
    for (; row < endRow; row++) {
        // The singleton global is re-read for every statement (retail never
        // caches it across statements).
        if (id != 0 &&
            (*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m30 != id) {
            continue;
        }
        if ((*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m2C != -1) {
            func_801BFE58(
                1, (*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m2C,
                kind,
                scale * (*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m1C);
        }
        if (store != 0) {
            (*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m28 = scale;
            (*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m32 = kind;
        }
    }
}

void func_801AAC70(void* self){ float dummy; func_801AAB64(0, (u32)self, 1, dummy); }

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

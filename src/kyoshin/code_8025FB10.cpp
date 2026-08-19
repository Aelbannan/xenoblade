// Auto-scaffolded catalog TU for kyoshin/code_8025FB10
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "monolib/device/CDeviceVI.hpp"

#include <nw4r/ut/ut_TextWriterBase.h>
#include <revolution/GX.h>
#include <revolution/MTX.h>

#include "kyoshin/CSysWin.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/util/MemManager.hpp"

namespace {

struct GXCacheTextProjection {
    u8 padding[0x4bc];
    s16 width;
    s16 height;
};

#define VALIDATE_NW4R_POINTER(pointer, file, line, message)                    \
    {                                                                         \
        bool validMem1 = true;                                                 \
        bool validMem2 = true;                                                 \
        bool validIo = true;                                                   \
        bool validIo2 = true;                                                  \
        bool validRegs = true;                                                 \
        bool validRegs2 = true;                                                \
        u32 address = (u32)(pointer);                                          \
        if ((address & 0xFF000000) != 0x80000000 &&                            \
            (address & 0xFF800000) != 0x81000000) {                            \
            validMem1 = false;                                                 \
        }                                                                      \
        if (!validMem1 && (address & 0xF8000000) != 0x90000000) {              \
            validMem2 = false;                                                 \
        }                                                                      \
        if (!validMem2 && (address & 0xFF000000) != 0xC0000000) {              \
            validIo = false;                                                   \
        }                                                                      \
        if (!validIo && (address & 0xFF800000) != 0xC1000000) {                \
            validIo2 = false;                                                  \
        }                                                                      \
        if (!validIo2 && (address & 0xF8000000) != 0xD0000000) {               \
            validRegs = false;                                                 \
        }                                                                      \
        if (!validRegs && (address & 0xFFFFC000) != 0xE0000000) {              \
            validRegs2 = false;                                                \
        }                                                                      \
        if (!validRegs2) {                                                     \
            Panic__Q24nw4r2dbFPCciPCce(file, line, message, pointer);          \
        }                                                                      \
    }

extern "C" {
void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);

extern void* cacheInstance__9CDeviceGX;
int func_8044BE24__8CGXCacheFv(void*);
s16* func_8044BE1C__8CGXCacheFv(void*);

extern void* lbl_eu_80664860;
extern nw4r::ut::TagProcessorBase<wchar_t>* lbl_eu_8066486C;
const nw4r::ut::Font* func_80449160__10CFontLayerFv(void*, int);

extern char lbl_eu_805377CC[];
extern char lbl_eu_80537798[];
extern char lbl_eu_80537784[];
extern char lbl_eu_80537748[];
extern char lbl_eu_8052DC70[];
extern char lbl_eu_8052DC3C[];
extern char lbl_eu_8052DD84[];
extern char lbl_eu_8052DD50[];
extern char lbl_eu_8052DC28[];
extern char lbl_eu_8052DBF4[];
extern char lbl_eu_8053785C[];
extern char lbl_eu_80537828[];
extern char lbl_eu_80537818[];
extern char lbl_eu_805377E0[];
extern char lbl_eu_8052DCFC[];
extern char lbl_eu_8052DCC8[];
extern char lbl_eu_805378A0[];
extern char lbl_eu_8053786C[];
extern char lbl_eu_80537734[];
extern char lbl_eu_80537700[];
extern char lbl_eu_805376EC[];
extern char lbl_eu_805376B8[];

extern const f32 lbl_eu_806688D0;
extern const f32 lbl_eu_806688D4;
extern const f32 lbl_eu_806688D8;
extern const f32 lbl_eu_806688DC;
extern const f32 lbl_eu_806688E0;
extern const f32 lbl_eu_806688B8;
}

#define VALIDATE_NW4R_POINTER_COMPACT(pointer, file, line, message)            \
    {                                                                         \
        u32 address = (u32)(pointer);                                          \
        if (!((address & 0xFF000000) == 0x80000000 ||                          \
              (address & 0xFF800000) == 0x81000000 ||                          \
              (address & 0xF8000000) == 0x90000000 ||                          \
              (address & 0xFF000000) == 0xC0000000 ||                          \
              (address & 0xFF800000) == 0xC1000000 ||                          \
              (address & 0xF8000000) == 0xD0000000 ||                          \
              (address & 0xFFFFC000) == 0xE0000000)) {                         \
            Panic__Q24nw4r2dbFPCciPCce(file, line, message, pointer);          \
        }                                                                      \
    }

#define VALIDATE_NW4R_POINTER_FLAG(pointer, region, file, line, message)       \
    {                                                                         \
        bool valid = false;                                                    \
        if (region == 0x80000000 ||                                            \
            ((u32)(pointer) & 0xFF800000) == 0x81000000 ||                     \
            ((u32)(pointer) & 0xF8000000) == 0x90000000 ||                     \
            region == 0xC0000000 ||                                            \
            ((u32)(pointer) & 0xFF800000) == 0xC1000000 ||                     \
            ((u32)(pointer) & 0xF8000000) == 0xD0000000 ||                     \
            ((u32)(pointer) & 0xFFFFC000) == 0xE0000000) {                     \
            valid = true;                                                      \
        }                                                                      \
        if (!valid) {                                                         \
            Panic__Q24nw4r2dbFPCciPCce(file, line, message, pointer);          \
        }                                                                      \
    }

typedef nw4r::ut::TextWriterBase<wchar_t> WideTextWriter;

static inline void setTagProcessorChecked(
    WideTextWriter* writer,
    nw4r::ut::TagProcessorBase<wchar_t>* processor) {
    VALIDATE_NW4R_POINTER(writer, lbl_eu_805377CC, 151,
                          lbl_eu_80537798);
    VALIDATE_NW4R_POINTER(processor, lbl_eu_80537784, 152,
                          lbl_eu_80537748);
    writer->SetTagProcessor(processor);
}

static inline void setDrawFlagChecked(WideTextWriter* writer, u32 flag) {
    VALIDATE_NW4R_POINTER_COMPACT(writer, lbl_eu_8052DC70, 139,
                          lbl_eu_8052DC3C);
    writer->SetDrawFlag(flag);
}

static inline void setScaleChecked(WideTextWriter* writer, f32 x, f32 y) {
    VALIDATE_NW4R_POINTER_COMPACT(writer, lbl_eu_8052DD84, 171,
                          lbl_eu_8052DD50);
    writer->SetScale(x, y);
}

static inline void setCharSpaceChecked(WideTextWriter* writer, f32 space) {
    VALIDATE_NW4R_POINTER_COMPACT(writer, lbl_eu_8052DC28, 98,
                          lbl_eu_8052DBF4);
    writer->SetCharSpace(space);
}

static inline void setFontChecked(WideTextWriter* writer,
                                  const nw4r::ut::Font* font) {
    VALIDATE_NW4R_POINTER(writer, lbl_eu_8053785C, 65,
                          lbl_eu_80537828);
    VALIDATE_NW4R_POINTER(font, lbl_eu_80537818, 66,
                          lbl_eu_805377E0);
    writer->SetFont(*font);
}

static inline void validateTextColorPointer(WideTextWriter* writer) {
    VALIDATE_NW4R_POINTER_COMPACT(writer, lbl_eu_8052DCFC, 135,
                          lbl_eu_8052DCC8);
}

static inline void setCursorChecked(WideTextWriter* writer, u32 writerRegion,
                                    f32 x, f32 y, f32 z) {
    VALIDATE_NW4R_POINTER_FLAG(writer, writerRegion, lbl_eu_805378A0, 258,
                          lbl_eu_8053786C);
    writer->SetCursor(x, y, z);
}

static inline void printCheckedInitial(WideTextWriter* writer,
                                      u32 writerRegion,
                                      const wchar_t* text,
                                      u32 textRegion) {
    VALIDATE_NW4R_POINTER_FLAG(writer, writerRegion, lbl_eu_80537734, 256,
                          lbl_eu_80537700);
    VALIDATE_NW4R_POINTER(text, lbl_eu_805376EC, 257,
                          lbl_eu_805376B8);
    writer->Print(text, static_cast<int>(wcslen(text)));
}

static inline void printChecked(WideTextWriter* writer, u32 writerRegion,
                                const wchar_t* text, u32 textRegion) {
    VALIDATE_NW4R_POINTER_FLAG(writer, writerRegion, lbl_eu_80537734, 256,
                          lbl_eu_80537700);
    VALIDATE_NW4R_POINTER_FLAG(text, textRegion, lbl_eu_805376EC, 257,
                          lbl_eu_805376B8);
    writer->Print(text, static_cast<int>(wcslen(text)));
}

} // namespace

// ---------------------------------------------------------------------------
// Shared object layouts for this TU's accessor functions.
// ---------------------------------------------------------------------------

// Object touched by func_8026178C / func_80261844 / func_8026187C:
//   +0x870: u32 bitmap array (32 bits per entry)
//   +0x884: signed counter clamped to [0, 999]
//   +0x888: signed int array clamped to [0, 999999]
struct CUnk8025FB10 {
    u8 pad_0[0x870];
    u32 mBitmap[1];                 // +0x870
    u8 pad_0x874[0x10];
    s32 mCounter884;                // +0x884
    s32 mValues888[1];              // +0x888
};

// Linked-list node walked by func_8025FD60 (+0x18 = next pointer).
struct CUnkList8025FB10 {
    u8 pad_0[0x18];
    CUnkList8025FB10* field_18;     // +0x18
};

// Walk the +0x18 chain to its last node (defined below; called by the
// accessor family after their 16-level unroll).
extern "C" CUnkList8025FB10* func_8025FD60(CUnkList8025FB10* p);

// ID-table entry / walk node for the func_8025FB10 accessor family.  Rows of
// the +0x000 table are 0xC4 bytes (6 entries + 4 pad); each entry is 32
// bytes.  Entries also form two linked chains: +0x18 child chain (walked to
// its deepest node, 16 levels unrolled here then func_8025FD60) and +0x1C
// sibling chain (summed/maxed by the accessors).
struct CUnkItem8025FB10 {
    u8 pad_0[0x4];                  // +0x00
    u16 mId;                        // +0x04
    u8 pad_6[0x2];                  // +0x06
    s32 mValue;                     // +0x08 - summed by func_8025FB10 family
    s16 mValueC;                    // +0x0C - maxed by func_8025FDB8 family
    u8 pad_E[0x2];                  // +0x0E
    f32 mValue10;                   // +0x10 - maxed by func_80260010
    u8 pad_14[0x4];                 // +0x14
    CUnkItem8025FB10* mChild;       // +0x18
    CUnkItem8025FB10* mNext;        // +0x1C
};

struct CUnkGroup8025FB10 {
    CUnkItem8025FB10 mItems[6];     // 0xC0
    u32 mPad;                       // 0xC4
};

// Object shared by the func_8025FB10 accessor family:
//   +0x000: 11 groups x 0xC4 (66 ID entries)
//   +0x870: bitmap, one bit per reachable ID
struct CUnkObj8025FB10 {
    CUnkGroup8025FB10 mGroups[11];  // 0x000..0x86C
    u8 pad_86C[0x4];                // 0x86C..0x870
    u32 mBitmap[5];                 // +0x870
};

extern u8* lbl_eu_80664870;

// Inline helper: MWCC's inliner lowers the in-loop `return rec` to an
// unconditional `b` after the `bne` (branch-over-branch `bne next; b found`),
// which an inline `if (index == rec->mId) break;` loop folds into a single
// `beq found` (see MWCC_CASES btm_acl "bne next; b found").
static inline CUnkItem8025FB10* findRecByIndex(CUnkObj8025FB10* obj,
                                               int index) {
    for (int i = 0; i < 66; i++) {
        CUnkItem8025FB10* rec = &obj->mGroups[i / 6].mItems[i % 6];
        if (index == rec->mId) {
            return rec;
        }
    }
    return NULL;
}

// Target: us-80261c5c - if the ID's bitmap bit is clear return 0; else look
// the ID up in the 66-entry table (signed i/6, i%6 -> row/column), walk the
// entry's +0x18 child chain to its deepest node (retail unrolls 16 levels
// then delegates the remainder to func_8025FD60) and return the sum of the
// +0x1C sibling chain's +0x08 values.
s32 func_8025FB10(CUnkObj8025FB10* obj, int index) {
    if (!(obj->mBitmap[(u32)index >> 5] & (1 << (index & 0x1F)))) {
        return 0;
    }

    CUnkItem8025FB10* rec;
    if (index >= 0x9E) {
        rec = NULL;
    } else {
        for (int i = 0; i < 66; i++) {
            rec = &obj->mGroups[i / 6].mItems[i % 6];
            if (index != rec->mId) {
                rec = NULL;
                continue;
            }
            break;
        }
        rec = NULL;
    }
    if (rec == NULL) {
        return 0;
    }

    // Deepest node of the +0x18 child chain.  Retail unrolls 16 levels, then
    // delegates the remainder to func_8025FD60.  Even-level checks jump to
    // bottom result blocks (mr to r3); odd-level results flow straight in.
    CUnkItem8025FB10* n1 = rec->mChild;
    if (n1 == 0) goto L1;
    CUnkItem8025FB10* n2 = n1->mChild;
    if (n2 == 0) goto L2;
    CUnkItem8025FB10* result;
    CUnkItem8025FB10* n3 = n2->mChild;
    if (n3 == 0) { result = n2; goto SUM; }
    CUnkItem8025FB10* n4 = n3->mChild;
    if (n4 == 0) goto L4;
    CUnkItem8025FB10* n5 = n4->mChild;
    if (n5 == 0) { result = n4; goto SUM; }
    CUnkItem8025FB10* n6 = n5->mChild;
    if (n6 == 0) goto L6;
    CUnkItem8025FB10* n7 = n6->mChild;
    if (n7 == 0) { result = n6; goto SUM; }
    CUnkItem8025FB10* n8 = n7->mChild;
    if (n8 == 0) goto L8;
    CUnkItem8025FB10* n9 = n8->mChild;
    if (n9 == 0) { result = n8; goto SUM; }
    CUnkItem8025FB10* n10 = n9->mChild;
    if (n10 == 0) goto L10;
    CUnkItem8025FB10* n11 = n10->mChild;
    if (n11 == 0) { result = n10; goto SUM; }
    CUnkItem8025FB10* n12 = n11->mChild;
    if (n12 == 0) goto L12;
    CUnkItem8025FB10* n13 = n12->mChild;
    if (n13 == 0) { result = n12; goto SUM; }
    CUnkItem8025FB10* n14 = n13->mChild;
    if (n14 == 0) goto L14;
    CUnkItem8025FB10* n15 = n14->mChild;
    if (n15 == 0) { result = n14; goto SUM; }
    CUnkItem8025FB10* n16 = n15->mChild;
    if (n16 == 0) goto L16;
    result = (CUnkItem8025FB10*)func_8025FD60((CUnkList8025FB10*)n16);
    goto SUM;
L16: result = n15; goto SUM;
L14: result = n13; goto SUM;
L12: result = n11; goto SUM;
L10: result = n9; goto SUM;
L8: result = n7; goto SUM;
L6: result = n5; goto SUM;
L4: result = n3; goto SUM;
L2: result = n1; goto SUM;
L1: result = rec; goto SUM;
SUM:
    // child.
    s32 total = 0;
    do {
        total += result->mValue;
        result = result->mNext;
    } while (result != NULL);
    return total;
}

// Target: us-80261eac - walk the +0x18 linked list to its last node.
// Retail unrolls 5 levels then tail-calls itself. The nested-if shape with
// the n3-return kept flat reproduces retail's register rotation (n1=r4,
// n2=r5, n3=r3, n4=r4, n5=r3) and bottom ret-blocks (MWCC_CASES
// func_8025F290 pattern); the fully-flat or fully-nested forms differ.
// extern "C" keeps the self tail-call reloc name verbatim (reloc-site
// gate) - same as func_8025F290 in CPcKizunagram.cpp.
extern "C" CUnkList8025FB10* func_8025FD60(CUnkList8025FB10* p) {
    CUnkList8025FB10* n1 = p->field_18;
    if (n1 == 0) return p;
    CUnkList8025FB10* n2 = n1->field_18;
    if (n2 != 0) {
        CUnkList8025FB10* n3 = n2->field_18;
        if (n3 != 0) {
            CUnkList8025FB10* n4 = n3->field_18;
            if (n4 == 0) return n3;
            CUnkList8025FB10* n5 = n4->field_18;
            if (n5 != 0) {
                return func_8025FD60(n5);
            }
            return n4;
        }
        return n2;
    }
    return n1;
}

// Target: us-80261f04 - like func_8025FB10 but returns the maximum s16
// +0x0C value along the deepest child's +0x1C sibling chain (or 0 when the
// ID is absent).
s32 func_8025FDB8(CUnkObj8025FB10* obj, int index) {
    if (!(obj->mBitmap[(u32)index >> 5] & (1 << (index & 0x1F)))) {
        return 0;
    }

    CUnkItem8025FB10* rec;
    if (index >= 0x9E) {
        rec = NULL;
    } else {
        int i = 0;
        while (i < 66) {
            rec = &obj->mGroups[i / 6].mItems[i % 6];
            if (index != rec->mId) {
                rec = NULL;
                i++;
                continue;
            }
            break;
        }
    }
    if (rec == NULL) {
        return 0;
    }

    // Deepest node of the +0x18 child chain (16 unrolled levels, then
    // func_8025FD60); result blocks at the bottom like retail.
    CUnkItem8025FB10* result;
    CUnkItem8025FB10* n1 = rec->mChild;
    if (n1 == 0) goto S1;
    CUnkItem8025FB10* n2 = n1->mChild;
    if (n2 == 0) goto S2;
    CUnkItem8025FB10* n3 = n2->mChild;
    if (n3 == 0) { result = n2; goto SMAX; }
    CUnkItem8025FB10* n4 = n3->mChild;
    if (n4 == 0) goto S4;
    CUnkItem8025FB10* n5 = n4->mChild;
    if (n5 == 0) { result = n4; goto SMAX; }
    CUnkItem8025FB10* n6 = n5->mChild;
    if (n6 == 0) goto S6;
    CUnkItem8025FB10* n7 = n6->mChild;
    if (n7 == 0) { result = n6; goto SMAX; }
    CUnkItem8025FB10* n8 = n7->mChild;
    if (n8 == 0) goto S8;
    CUnkItem8025FB10* n9 = n8->mChild;
    if (n9 == 0) { result = n8; goto SMAX; }
    CUnkItem8025FB10* n10 = n9->mChild;
    if (n10 == 0) goto S10;
    CUnkItem8025FB10* n11 = n10->mChild;
    if (n11 == 0) { result = n10; goto SMAX; }
    CUnkItem8025FB10* n12 = n11->mChild;
    if (n12 == 0) goto S12;
    CUnkItem8025FB10* n13 = n12->mChild;
    if (n13 == 0) { result = n12; goto SMAX; }
    CUnkItem8025FB10* n14 = n13->mChild;
    if (n14 == 0) goto S14;
    CUnkItem8025FB10* n15 = n14->mChild;
    if (n15 == 0) { result = n14; goto SMAX; }
    CUnkItem8025FB10* n16 = n15->mChild;
    if (n16 == 0) goto S16;
    result = (CUnkItem8025FB10*)func_8025FD60((CUnkList8025FB10*)n16);
    goto SMAX;
S16: result = n15; goto SMAX;
S14: result = n13; goto SMAX;
S12: result = n11; goto SMAX;
S10: result = n9; goto SMAX;
S8: result = n7; goto SMAX;
S6: result = n5; goto SMAX;
S4: result = n3; goto SMAX;
S2: result = n1; goto SMAX;
S1: result = rec; goto SMAX;
SMAX:
    // Max of the +0x1C sibling chain's +0x0C halfwords from the deepest
    // child.
    s32 best = 0;
    CUnkItem8025FB10* node = result;
    do {
        if (best < node->mValueC) {
            best = node->mValueC;
        }
        node = node->mNext;
    } while (node != NULL);
    return best;
}

// Target: us-8026215c - like func_8025FB10 but returns the maximum f32
// +0x10 value along the deepest child's +0x1C sibling chain, else the shared
// sdata2 constant (lbl_eu_806688B8).
f32 func_80260010(CUnkObj8025FB10* obj, int index) {
    if (!(obj->mBitmap[(u32)index >> 5] & (1 << (index & 0x1F)))) {
        return lbl_eu_806688B8;
    }

    CUnkItem8025FB10* rec;
    if (index >= 0x9E) {
        rec = NULL;
    } else {
        int i = 0;
        while (i < 66) {
            rec = &obj->mGroups[i / 6].mItems[i % 6];
            if (index != rec->mId) {
                rec = NULL;
                i++;
                continue;
            }
            break;
        }
    }
    if (rec == NULL) {
        return lbl_eu_806688B8;
    }

    // Deepest node of the +0x18 child chain (16 unrolled levels, then
    // func_8025FD60); result blocks at the bottom like retail.
    CUnkItem8025FB10* result;
    CUnkItem8025FB10* n1 = rec->mChild;
    if (n1 == 0) goto M1;
    CUnkItem8025FB10* n2 = n1->mChild;
    if (n2 == 0) goto M2;
    CUnkItem8025FB10* n3 = n2->mChild;
    if (n3 == 0) { result = n2; goto MMAX; }
    CUnkItem8025FB10* n4 = n3->mChild;
    if (n4 == 0) goto M4;
    CUnkItem8025FB10* n5 = n4->mChild;
    if (n5 == 0) { result = n4; goto MMAX; }
    CUnkItem8025FB10* n6 = n5->mChild;
    if (n6 == 0) goto M6;
    CUnkItem8025FB10* n7 = n6->mChild;
    if (n7 == 0) { result = n6; goto MMAX; }
    CUnkItem8025FB10* n8 = n7->mChild;
    if (n8 == 0) goto M8;
    CUnkItem8025FB10* n9 = n8->mChild;
    if (n9 == 0) { result = n8; goto MMAX; }
    CUnkItem8025FB10* n10 = n9->mChild;
    if (n10 == 0) goto M10;
    CUnkItem8025FB10* n11 = n10->mChild;
    if (n11 == 0) { result = n10; goto MMAX; }
    CUnkItem8025FB10* n12 = n11->mChild;
    if (n12 == 0) goto M12;
    CUnkItem8025FB10* n13 = n12->mChild;
    if (n13 == 0) { result = n12; goto MMAX; }
    CUnkItem8025FB10* n14 = n13->mChild;
    if (n14 == 0) goto M14;
    CUnkItem8025FB10* n15 = n14->mChild;
    if (n15 == 0) { result = n14; goto MMAX; }
    CUnkItem8025FB10* n16 = n15->mChild;
    if (n16 == 0) goto M16;
    result = (CUnkItem8025FB10*)func_8025FD60((CUnkList8025FB10*)n16);
    goto MMAX;
M16: result = n15; goto MMAX;
M14: result = n13; goto MMAX;
M12: result = n11; goto MMAX;
M10: result = n9; goto MMAX;
M8: result = n7; goto MMAX;
M6: result = n5; goto MMAX;
M4: result = n3; goto MMAX;
M2: result = n1; goto MMAX;
M1: result = rec; goto MMAX;
MMAX:
    // Max of the +0x1C sibling chain's +0x10 floats from the deepest child.
    f32 best = lbl_eu_806688B8;
    CUnkItem8025FB10* node = result;
    do {
        if (best < node->mValue10) {
            best = node->mValue10;
        }
        node = node->mNext;
    } while (node != NULL);
    return best;
}

// Target: us-802623b0 - like func_8025FB10 but stores the sum through the
// third argument and returns whether the sum is non-zero.
bool func_80260264(CUnkObj8025FB10* obj, int index, s32* outSum) {
    s32 sum = 0;
    if (obj->mBitmap[(u32)index >> 5] & (1 << (index & 0x1F))) {
        CUnkItem8025FB10* rec;
        if (index >= 0x9E) {
            rec = NULL;
        } else {
            int i = 0;
            while (i < 66) {
                rec = &obj->mGroups[i / 6].mItems[i % 6];
                if (index != rec->mId) {
                    rec = NULL;
                    i++;
                    continue;
                }
                break;
            }
        }
        if (rec != NULL) {
            // Deepest node of the +0x18 child chain (16 unrolled levels,
            // then func_8025FD60); result blocks at the bottom.
            CUnkItem8025FB10* result;
            CUnkItem8025FB10* n1 = rec->mChild;
            if (n1 == 0) goto P1;
            CUnkItem8025FB10* n2 = n1->mChild;
            if (n2 == 0) goto P2;
            CUnkItem8025FB10* n3 = n2->mChild;
            if (n3 == 0) goto P3;
            CUnkItem8025FB10* n4 = n3->mChild;
            if (n4 == 0) goto P4;
            CUnkItem8025FB10* n5 = n4->mChild;
            if (n5 == 0) goto P5;
            CUnkItem8025FB10* n6 = n5->mChild;
            if (n6 == 0) goto P6;
            CUnkItem8025FB10* n7 = n6->mChild;
            if (n7 == 0) goto P7;
            CUnkItem8025FB10* n8 = n7->mChild;
            if (n8 == 0) goto P8;
            CUnkItem8025FB10* n9 = n8->mChild;
            if (n9 == 0) goto P9;
            CUnkItem8025FB10* n10 = n9->mChild;
            if (n10 == 0) goto P10;
            CUnkItem8025FB10* n11 = n10->mChild;
            if (n11 == 0) goto P11;
            CUnkItem8025FB10* n12 = n11->mChild;
            if (n12 == 0) goto P12;
            CUnkItem8025FB10* n13 = n12->mChild;
            if (n13 == 0) goto P13;
            CUnkItem8025FB10* n14 = n13->mChild;
            if (n14 == 0) goto P14;
            CUnkItem8025FB10* n15 = n14->mChild;
            if (n15 == 0) goto P15;
            CUnkItem8025FB10* n16 = n15->mChild;
            if (n16 == 0) goto P16;
            result = (CUnkItem8025FB10*)func_8025FD60((CUnkList8025FB10*)n16);
            goto PSUM;
        P16: result = n15; goto PSUM;
        P15: result = n14; goto PSUM;
        P14: result = n13; goto PSUM;
        P13: result = n12; goto PSUM;
        P12: result = n11; goto PSUM;
        P11: result = n10; goto PSUM;
        P10: result = n9; goto PSUM;
        P9: result = n8; goto PSUM;
        P8: result = n7; goto PSUM;
        P7: result = n6; goto PSUM;
        P6: result = n5; goto PSUM;
        P5: result = n4; goto PSUM;
        P4: result = n3; goto PSUM;
        P3: result = n2; goto PSUM;
        P2: result = n1; goto PSUM;
        P1: result = rec; goto PSUM;
        PSUM:
            // Sum of the +0x1C sibling chain's +0x08 values from the
            // deepest child.
            sum = 0;
            CUnkItem8025FB10* node = result;
            do {
                sum += node->mValue;
                node = node->mNext;
            } while (node != NULL);
        }
    }
    *outSum = sum;
    return sum != 0;
}

// Target: us-80262664 - dual accessor.  Stores the +0x08 sum chain total for
// the ID through outSum, and (only when that sum is non-zero) the +0x10
// float max through outFloat; returns whether the float max is non-zero.
bool func_80260518(CUnkObj8025FB10* obj, int index, s32* outSum, f32* outFloat) {
    s32 sum;
    if (!(obj->mBitmap[(u32)index >> 5] & (1 << (index & 0x1F)))) {
        sum = 0;
        goto SUMSTORE;
    }

    CUnkItem8025FB10* rec;
    if (index >= 0x9E) {
        rec = NULL;
    } else {
        rec = findRecByIndex(obj, index);
    }
    if (rec == NULL) {
        sum = 0;
        goto SUMSTORE;
    }

    // Deepest node of the +0x18 child chain (21 unrolled levels, then
    // func_8025FD60).  Odd levels reuse `result` so even-level null paths
    // need no copy (direct to the sum loop); odd-level nulls (result =
    // even node) land in bottom blocks, mirroring retail's rotation.
    CUnkItem8025FB10* result;
    CUnkItem8025FB10* n2;
    CUnkItem8025FB10* n4;
    CUnkItem8025FB10* n6;
    CUnkItem8025FB10* n8;
    CUnkItem8025FB10* n10;
    CUnkItem8025FB10* n12;
    CUnkItem8025FB10* n14;
    CUnkItem8025FB10* n16;
    CUnkItem8025FB10* n18;
    CUnkItem8025FB10* n20;
    result = rec->mChild;
    if (result == 0) goto D01;
    n2 = result->mChild;
    if (n2 == 0) goto DSUM;
    result = n2->mChild;
    if (result == 0) goto D03;
    n4 = result->mChild;
    if (n4 == 0) goto DSUM;
    result = n4->mChild;
    if (result == 0) goto D05;
    n6 = result->mChild;
    if (n6 == 0) goto DSUM;
    result = n6->mChild;
    if (result == 0) goto D07;
    n8 = result->mChild;
    if (n8 == 0) goto DSUM;
    result = n8->mChild;
    if (result == 0) goto D09;
    n10 = result->mChild;
    if (n10 == 0) goto DSUM;
    result = n10->mChild;
    if (result == 0) goto D11;
    n12 = result->mChild;
    if (n12 == 0) goto DSUM;
    result = n12->mChild;
    if (result == 0) goto D13;
    n14 = result->mChild;
    if (n14 == 0) goto DSUM;
    result = n14->mChild;
    if (result == 0) goto D15;
    n16 = result->mChild;
    if (n16 == 0) goto DSUM;
    result = n16->mChild;
    if (result == 0) goto D17;
    n18 = result->mChild;
    if (n18 == 0) goto DSUM;
    result = n18->mChild;
    if (result == 0) goto D19;
    n20 = result->mChild;
    if (n20 == 0) goto DSUM;
    result = n20->mChild;
    if (result == 0) goto D21;
    result = (CUnkItem8025FB10*)func_8025FD60((CUnkList8025FB10*)result);
    goto DSUM;
D21: result = n20; goto DSUM;
D19: result = n18; goto DSUM;
D17: result = n16; goto DSUM;
D15: result = n14; goto DSUM;
D13: result = n12; goto DSUM;
D11: result = n10; goto DSUM;
D09: result = n8; goto DSUM;
D07: result = n6; goto DSUM;
D05: result = n4; goto DSUM;
D03: result = n2; goto DSUM;
D01: result = rec; goto DSUM;
DSUM:
    // Sum of the +0x1C sibling chain's +0x08 values from the deepest child.
    sum = 0;
    CUnkItem8025FB10* node = result;
    do {
        sum += node->mValue;
        node = node->mNext;
    } while (node != NULL);

SUMSTORE:
    *outSum = sum;
    if (sum == 0) {
        return false;
    }

    // Second pass: +0x10 float max, stored through outFloat.
    f32 best;
    if (!(obj->mBitmap[(u32)index >> 5] & (1 << (index & 0x1F)))) {
        best = lbl_eu_806688B8;
        goto FSTORE;
    }

    CUnkItem8025FB10* recF;
    if (index >= 0x9E) {
        recF = NULL;
    } else {
        recF = findRecByIndex(obj, index);
    }
    if (recF == NULL) {
        best = lbl_eu_806688B8;
        goto FSTORE;
    }

    // Deepest node of the +0x18 child chain; same shape as above.
    CUnkItem8025FB10* resultF;
    CUnkItem8025FB10* f2;
    CUnkItem8025FB10* f4;
    CUnkItem8025FB10* f6;
    CUnkItem8025FB10* f8;
    CUnkItem8025FB10* f10;
    CUnkItem8025FB10* f12;
    CUnkItem8025FB10* f14;
    CUnkItem8025FB10* f16;
    CUnkItem8025FB10* f18;
    CUnkItem8025FB10* f20;
    resultF = recF->mChild;
    if (resultF == 0) goto E01;
    f2 = resultF->mChild;
    if (f2 == 0) goto EMAX;
    resultF = f2->mChild;
    if (resultF == 0) goto E03;
    f4 = resultF->mChild;
    if (f4 == 0) goto EMAX;
    resultF = f4->mChild;
    if (resultF == 0) goto E05;
    f6 = resultF->mChild;
    if (f6 == 0) goto EMAX;
    resultF = f6->mChild;
    if (resultF == 0) goto E07;
    f8 = resultF->mChild;
    if (f8 == 0) goto EMAX;
    resultF = f8->mChild;
    if (resultF == 0) goto E09;
    f10 = resultF->mChild;
    if (f10 == 0) goto EMAX;
    resultF = f10->mChild;
    if (resultF == 0) goto E11;
    f12 = resultF->mChild;
    if (f12 == 0) goto EMAX;
    resultF = f12->mChild;
    if (resultF == 0) goto E13;
    f14 = resultF->mChild;
    if (f14 == 0) goto EMAX;
    resultF = f14->mChild;
    if (resultF == 0) goto E15;
    f16 = resultF->mChild;
    if (f16 == 0) goto EMAX;
    resultF = f16->mChild;
    if (resultF == 0) goto E17;
    f18 = resultF->mChild;
    if (f18 == 0) goto EMAX;
    resultF = f18->mChild;
    if (resultF == 0) goto E19;
    f20 = resultF->mChild;
    if (f20 == 0) goto EMAX;
    resultF = f20->mChild;
    if (resultF == 0) goto E21;
    resultF = (CUnkItem8025FB10*)func_8025FD60((CUnkList8025FB10*)resultF);
    goto EMAX;
E21: resultF = f20; goto EMAX;
E19: resultF = f18; goto EMAX;
E17: resultF = f16; goto EMAX;
E15: resultF = f14; goto EMAX;
E13: resultF = f12; goto EMAX;
E11: resultF = f10; goto EMAX;
E09: resultF = f8; goto EMAX;
E07: resultF = f6; goto EMAX;
E05: resultF = f4; goto EMAX;
E03: resultF = f2; goto EMAX;
E01: resultF = recF; goto EMAX;
EMAX:
    // Max of the +0x1C sibling chain's +0x10 floats from the deepest child.
    best = lbl_eu_806688B8;
    CUnkItem8025FB10* nodeF = resultF;
    do {
        if (best < nodeF->mValue10) {
            best = nodeF->mValue10;
        }
        nodeF = nodeF->mNext;
    } while (nodeF != NULL);

FSTORE:
    *outFloat = best;
    return best != lbl_eu_806688B8;
}

// Target: us-80262bb8 - dual accessor.  Stores the +0x08 sum chain total for
// the ID through out1, and (only when that sum is non-zero) the +0x0C
// halfword max through out2; returns whether the second total is non-zero.
bool func_80260A6C(CUnkObj8025FB10* obj, int index, s32* outSum, s32* outMax) {
    s32 sum = 0;
    if (obj->mBitmap[(u32)index >> 5] & (1 << (index & 0x1F))) {
        CUnkItem8025FB10* rec;
        if (index >= 0x9E) {
            rec = NULL;
        } else {
            int i = 0;
            while (i < 66) {
                rec = &obj->mGroups[i / 6].mItems[i % 6];
                if (index != rec->mId) {
                    rec = NULL;
                    i++;
                    continue;
                }
                break;
            }
        }
        if (rec != NULL) {
            // Deepest node of the +0x18 child chain (16 unrolled levels,
            // then func_8025FD60); result blocks at the bottom.
            CUnkItem8025FB10* result;
            CUnkItem8025FB10* n1 = rec->mChild;
            if (n1 == 0) goto A1;
            CUnkItem8025FB10* n2 = n1->mChild;
            if (n2 == 0) goto A2;
            CUnkItem8025FB10* n3 = n2->mChild;
            if (n3 == 0) goto A3;
            CUnkItem8025FB10* n4 = n3->mChild;
            if (n4 == 0) goto A4;
            CUnkItem8025FB10* n5 = n4->mChild;
            if (n5 == 0) goto A5;
            CUnkItem8025FB10* n6 = n5->mChild;
            if (n6 == 0) goto A6;
            CUnkItem8025FB10* n7 = n6->mChild;
            if (n7 == 0) goto A7;
            CUnkItem8025FB10* n8 = n7->mChild;
            if (n8 == 0) goto A8;
            CUnkItem8025FB10* n9 = n8->mChild;
            if (n9 == 0) goto A9;
            CUnkItem8025FB10* n10 = n9->mChild;
            if (n10 == 0) goto A10;
            CUnkItem8025FB10* n11 = n10->mChild;
            if (n11 == 0) goto A11;
            CUnkItem8025FB10* n12 = n11->mChild;
            if (n12 == 0) goto A12;
            CUnkItem8025FB10* n13 = n12->mChild;
            if (n13 == 0) goto A13;
            CUnkItem8025FB10* n14 = n13->mChild;
            if (n14 == 0) goto A14;
            CUnkItem8025FB10* n15 = n14->mChild;
            if (n15 == 0) goto A15;
            CUnkItem8025FB10* n16 = n15->mChild;
            if (n16 == 0) goto A16;
            result = (CUnkItem8025FB10*)func_8025FD60((CUnkList8025FB10*)n16);
            goto ASUM;
        A16: result = n15; goto ASUM;
        A15: result = n14; goto ASUM;
        A14: result = n13; goto ASUM;
        A13: result = n12; goto ASUM;
        A12: result = n11; goto ASUM;
        A11: result = n10; goto ASUM;
        A10: result = n9; goto ASUM;
        A9: result = n8; goto ASUM;
        A8: result = n7; goto ASUM;
        A7: result = n6; goto ASUM;
        A6: result = n5; goto ASUM;
        A5: result = n4; goto ASUM;
        A4: result = n3; goto ASUM;
        A3: result = n2; goto ASUM;
        A2: result = n1; goto ASUM;
        A1: result = rec; goto ASUM;
        ASUM:
            // Sum of the +0x1C sibling chain's +0x08 values from the
            // deepest child.
            sum = 0;
            CUnkItem8025FB10* node = result;
            do {
                sum += node->mValue;
                node = node->mNext;
            } while (node != NULL);
        }
    }
    *outSum = sum;
    if (sum == 0) {
        return false;
    }

    // Second pass: +0x0C halfword max, stored through outMax.
    s32 max = 0;
    if (obj->mBitmap[(u32)index >> 5] & (1 << (index & 0x1F))) {
        CUnkItem8025FB10* rec;
        if (index >= 0x9E) {
            rec = NULL;
        } else {
            int i = 0;
            while (i < 66) {
                rec = &obj->mGroups[i / 6].mItems[i % 6];
                if (index != rec->mId) {
                    rec = NULL;
                    i++;
                    continue;
                }
                break;
            }
        }
        if (rec != NULL) {
            // Deepest node of the +0x18 child chain; same shape as above.
            CUnkItem8025FB10* result;
            CUnkItem8025FB10* n1 = rec->mChild;
            if (n1 == 0) goto B1;
            CUnkItem8025FB10* n2 = n1->mChild;
            if (n2 == 0) goto B2;
            CUnkItem8025FB10* n3 = n2->mChild;
            if (n3 == 0) goto B3;
            CUnkItem8025FB10* n4 = n3->mChild;
            if (n4 == 0) goto B4;
            CUnkItem8025FB10* n5 = n4->mChild;
            if (n5 == 0) goto B5;
            CUnkItem8025FB10* n6 = n5->mChild;
            if (n6 == 0) goto B6;
            CUnkItem8025FB10* n7 = n6->mChild;
            if (n7 == 0) goto B7;
            CUnkItem8025FB10* n8 = n7->mChild;
            if (n8 == 0) goto B8;
            CUnkItem8025FB10* n9 = n8->mChild;
            if (n9 == 0) goto B9;
            CUnkItem8025FB10* n10 = n9->mChild;
            if (n10 == 0) goto B10;
            CUnkItem8025FB10* n11 = n10->mChild;
            if (n11 == 0) goto B11;
            CUnkItem8025FB10* n12 = n11->mChild;
            if (n12 == 0) goto B12;
            CUnkItem8025FB10* n13 = n12->mChild;
            if (n13 == 0) goto B13;
            CUnkItem8025FB10* n14 = n13->mChild;
            if (n14 == 0) goto B14;
            CUnkItem8025FB10* n15 = n14->mChild;
            if (n15 == 0) goto B15;
            CUnkItem8025FB10* n16 = n15->mChild;
            if (n16 == 0) goto B16;
            result = (CUnkItem8025FB10*)func_8025FD60((CUnkList8025FB10*)n16);
            goto BMAX;
        B16: result = n15; goto BMAX;
        B15: result = n14; goto BMAX;
        B14: result = n13; goto BMAX;
        B13: result = n12; goto BMAX;
        B12: result = n11; goto BMAX;
        B11: result = n10; goto BMAX;
        B10: result = n9; goto BMAX;
        B9: result = n8; goto BMAX;
        B8: result = n7; goto BMAX;
        B7: result = n6; goto BMAX;
        B6: result = n5; goto BMAX;
        B5: result = n4; goto BMAX;
        B4: result = n3; goto BMAX;
        B3: result = n2; goto BMAX;
        B2: result = n1; goto BMAX;
        B1: result = rec; goto BMAX;
        BMAX:
            // Max of the +0x1C sibling chain's +0x0C halfwords.
            max = 0;
            CUnkItem8025FB10* node = result;
            do {
                if (max < node->mValueC) {
                    max = node->mValueC;
                }
                node = node->mNext;
            } while (node != NULL);
        }
    }
    *outMax = max;
    return max != 0;
}

// Target: us-802630fc - triple accessor.  Stores the +0x08 sum chain total
// for the ID through outSum, then (when non-zero) the +0x0C halfword max
// through outMax, then (when non-zero) the +0x10 float max through outFloat;
// returns whether the float max is non-zero.
bool func_80260FB0(CUnkObj8025FB10* obj, int index, s32* outSum, s32* outMax,
                   f32* outFloat) {
    s32 sum;
    s32 max;
    if (!(obj->mBitmap[(u32)index >> 5] & (1 << (index & 0x1F)))) {
        sum = 0;
        goto SUMSTORE;
    }

    CUnkItem8025FB10* rec;
    if (index >= 0x9E) {
        rec = NULL;
    } else {
        rec = findRecByIndex(obj, index);
    }
    if (rec == NULL) {
        sum = 0;
        goto SUMSTORE;
    }

    // Deepest node of the +0x18 child chain (21 unrolled levels, then
    // func_8025FD60).  Odd levels reuse `result` so even-level null paths
    // need no copy (direct to the sum loop); odd-level nulls (result =
    // even node) land in bottom blocks, mirroring retail's rotation.
    CUnkItem8025FB10* result;
    CUnkItem8025FB10* s2;
    CUnkItem8025FB10* s4;
    CUnkItem8025FB10* s6;
    CUnkItem8025FB10* s8;
    CUnkItem8025FB10* s10;
    CUnkItem8025FB10* s12;
    CUnkItem8025FB10* s14;
    CUnkItem8025FB10* s16;
    CUnkItem8025FB10* s18;
    CUnkItem8025FB10* s20;
    result = rec->mChild;
    if (result == 0) goto SS01;
    s2 = result->mChild;
    if (s2 == 0) goto SSUM;
    result = s2->mChild;
    if (result == 0) goto SS03;
    s4 = result->mChild;
    if (s4 == 0) goto SSUM;
    result = s4->mChild;
    if (result == 0) goto SS05;
    s6 = result->mChild;
    if (s6 == 0) goto SSUM;
    result = s6->mChild;
    if (result == 0) goto SS07;
    s8 = result->mChild;
    if (s8 == 0) goto SSUM;
    result = s8->mChild;
    if (result == 0) goto SS09;
    s10 = result->mChild;
    if (s10 == 0) goto SSUM;
    result = s10->mChild;
    if (result == 0) goto SS11;
    s12 = result->mChild;
    if (s12 == 0) goto SSUM;
    result = s12->mChild;
    if (result == 0) goto SS13;
    s14 = result->mChild;
    if (s14 == 0) goto SSUM;
    result = s14->mChild;
    if (result == 0) goto SS15;
    s16 = result->mChild;
    if (s16 == 0) goto SSUM;
    result = s16->mChild;
    if (result == 0) goto SS17;
    s18 = result->mChild;
    if (s18 == 0) goto SSUM;
    result = s18->mChild;
    if (result == 0) goto SS19;
    s20 = result->mChild;
    if (s20 == 0) goto SSUM;
    result = s20->mChild;
    if (result == 0) goto SS21;
    result = (CUnkItem8025FB10*)func_8025FD60((CUnkList8025FB10*)result);
    goto SSUM;
SS21: result = s20; goto SSUM;
SS19: result = s18; goto SSUM;
SS17: result = s16; goto SSUM;
SS15: result = s14; goto SSUM;
SS13: result = s12; goto SSUM;
SS11: result = s10; goto SSUM;
SS09: result = s8; goto SSUM;
SS07: result = s6; goto SSUM;
SS05: result = s4; goto SSUM;
SS03: result = s2; goto SSUM;
SS01: result = rec; goto SSUM;
SSUM:
    // Sum of the +0x1C sibling chain's +0x08 values from the deepest child.
    sum = 0;
    CUnkItem8025FB10* node = result;
    do {
        sum += node->mValue;
        node = node->mNext;
    } while (node != NULL);

SUMSTORE:
    *outSum = sum;
    if (sum == 0) {
        return false;
    }

    // Second pass: +0x0C halfword max, stored through outMax.
    if (!(obj->mBitmap[(u32)index >> 5] & (1 << (index & 0x1F)))) {
        max = 0;
        goto MAXSTORE;
    }

    CUnkItem8025FB10* recM;
    if (index >= 0x9E) {
        recM = NULL;
    } else {
        recM = findRecByIndex(obj, index);
    }
    if (recM == NULL) {
        max = 0;
        goto MAXSTORE;
    }

    // Deepest node of the +0x18 child chain; same shape as above.
    CUnkItem8025FB10* resultM;
    CUnkItem8025FB10* m2;
    CUnkItem8025FB10* m4;
    CUnkItem8025FB10* m6;
    CUnkItem8025FB10* m8;
    CUnkItem8025FB10* m10;
    CUnkItem8025FB10* m12;
    CUnkItem8025FB10* m14;
    CUnkItem8025FB10* m16;
    CUnkItem8025FB10* m18;
    CUnkItem8025FB10* m20;
    resultM = recM->mChild;
    if (resultM == 0) goto MM01;
    m2 = resultM->mChild;
    if (m2 == 0) goto MMAX;
    resultM = m2->mChild;
    if (resultM == 0) goto MM03;
    m4 = resultM->mChild;
    if (m4 == 0) goto MMAX;
    resultM = m4->mChild;
    if (resultM == 0) goto MM05;
    m6 = resultM->mChild;
    if (m6 == 0) goto MMAX;
    resultM = m6->mChild;
    if (resultM == 0) goto MM07;
    m8 = resultM->mChild;
    if (m8 == 0) goto MMAX;
    resultM = m8->mChild;
    if (resultM == 0) goto MM09;
    m10 = resultM->mChild;
    if (m10 == 0) goto MMAX;
    resultM = m10->mChild;
    if (resultM == 0) goto MM11;
    m12 = resultM->mChild;
    if (m12 == 0) goto MMAX;
    resultM = m12->mChild;
    if (resultM == 0) goto MM13;
    m14 = resultM->mChild;
    if (m14 == 0) goto MMAX;
    resultM = m14->mChild;
    if (resultM == 0) goto MM15;
    m16 = resultM->mChild;
    if (m16 == 0) goto MMAX;
    resultM = m16->mChild;
    if (resultM == 0) goto MM17;
    m18 = resultM->mChild;
    if (m18 == 0) goto MMAX;
    resultM = m18->mChild;
    if (resultM == 0) goto MM19;
    m20 = resultM->mChild;
    if (m20 == 0) goto MMAX;
    resultM = m20->mChild;
    if (resultM == 0) goto MM21;
    resultM = (CUnkItem8025FB10*)func_8025FD60((CUnkList8025FB10*)resultM);
    goto MMAX;
MM21: resultM = m20; goto MMAX;
MM19: resultM = m18; goto MMAX;
MM17: resultM = m16; goto MMAX;
MM15: resultM = m14; goto MMAX;
MM13: resultM = m12; goto MMAX;
MM11: resultM = m10; goto MMAX;
MM09: resultM = m8; goto MMAX;
MM07: resultM = m6; goto MMAX;
MM05: resultM = m4; goto MMAX;
MM03: resultM = m2; goto MMAX;
MM01: resultM = recM; goto MMAX;
MMAX:
    // Max of the +0x1C sibling chain's +0x0C halfwords.
    max = 0;
    CUnkItem8025FB10* nodeM = resultM;
    do {
        int v = nodeM->mValueC;
        if (max < v) {
            max = v;
        }
        nodeM = nodeM->mNext;
    } while (nodeM != NULL);

MAXSTORE:
    *outMax = max;
    if (max == 0) {
        return false;
    }

    // Third pass: +0x10 float max, stored through outFloat.
    f32 best;
    if (!(obj->mBitmap[(u32)index >> 5] & (1 << (index & 0x1F)))) {
        best = lbl_eu_806688B8;
        goto FSTORE;
    }

    CUnkItem8025FB10* recF;
    if (index >= 0x9E) {
        recF = NULL;
    } else {
        recF = findRecByIndex(obj, index);
    }
    if (recF == NULL) {
        best = lbl_eu_806688B8;
        goto FSTORE;
    }

    // Deepest node of the +0x18 child chain; same shape as above.
    CUnkItem8025FB10* resultF;
    CUnkItem8025FB10* g2;
    CUnkItem8025FB10* g4;
    CUnkItem8025FB10* g6;
    CUnkItem8025FB10* g8;
    CUnkItem8025FB10* g10;
    CUnkItem8025FB10* g12;
    CUnkItem8025FB10* g14;
    CUnkItem8025FB10* g16;
    CUnkItem8025FB10* g18;
    CUnkItem8025FB10* g20;
    resultF = recF->mChild;
    if (resultF == 0) goto GG01;
    g2 = resultF->mChild;
    if (g2 == 0) goto GMAX;
    resultF = g2->mChild;
    if (resultF == 0) goto GG03;
    g4 = resultF->mChild;
    if (g4 == 0) goto GMAX;
    resultF = g4->mChild;
    if (resultF == 0) goto GG05;
    g6 = resultF->mChild;
    if (g6 == 0) goto GMAX;
    resultF = g6->mChild;
    if (resultF == 0) goto GG07;
    g8 = resultF->mChild;
    if (g8 == 0) goto GMAX;
    resultF = g8->mChild;
    if (resultF == 0) goto GG09;
    g10 = resultF->mChild;
    if (g10 == 0) goto GMAX;
    resultF = g10->mChild;
    if (resultF == 0) goto GG11;
    g12 = resultF->mChild;
    if (g12 == 0) goto GMAX;
    resultF = g12->mChild;
    if (resultF == 0) goto GG13;
    g14 = resultF->mChild;
    if (g14 == 0) goto GMAX;
    resultF = g14->mChild;
    if (resultF == 0) goto GG15;
    g16 = resultF->mChild;
    if (g16 == 0) goto GMAX;
    resultF = g16->mChild;
    if (resultF == 0) goto GG17;
    g18 = resultF->mChild;
    if (g18 == 0) goto GMAX;
    resultF = g18->mChild;
    if (resultF == 0) goto GG19;
    g20 = resultF->mChild;
    if (g20 == 0) goto GMAX;
    resultF = g20->mChild;
    if (resultF == 0) goto GG21;
    resultF = (CUnkItem8025FB10*)func_8025FD60((CUnkList8025FB10*)resultF);
    goto GMAX;
GG21: resultF = g20; goto GMAX;
GG19: resultF = g18; goto GMAX;
GG17: resultF = g16; goto GMAX;
GG15: resultF = g14; goto GMAX;
GG13: resultF = g12; goto GMAX;
GG11: resultF = g10; goto GMAX;
GG09: resultF = g8; goto GMAX;
GG07: resultF = g6; goto GMAX;
GG05: resultF = g4; goto GMAX;
GG03: resultF = g2; goto GMAX;
GG01: resultF = recF; goto GMAX;
GMAX:
    // Max of the +0x1C sibling chain's +0x10 floats from the deepest child.
    best = lbl_eu_806688B8;
    CUnkItem8025FB10* nodeF = resultF;
    do {
        if (best < nodeF->mValue10) {
            best = nodeF->mValue10;
        }
        nodeF = nodeF->mNext;
    } while (nodeF != NULL);

FSTORE:
    *outFloat = best;
    return best != lbl_eu_806688B8;
}

// Target: us-802638d8 - test bit (index & 0x1F) of bitmap word (index >> 5)
// at +0x870. Returns 0/1.
u8 func_8026178C(CUnk8025FB10* obj, u32 index) {
    return (obj->mBitmap[index >> 5] & (1 << (index & 0x1F))) != 0;
}

void func_802617B8(){}

// Target: us-80263990 - set mValues888[index] to value, clamped to [0, 999999].
void func_80261844(CUnk8025FB10* obj, int index, int value) {
    s32* pValue = &obj->mValues888[index];
    *pValue = value;
    if (value < 0) {
        *pValue = 0;
    } else if (value > 0xF423F) {
        *pValue = 0xF423F;
    }
}

// Target: us-802639c8 - add delta to the +0x884 counter, clamped to [0, 999].
void func_8026187C(CUnk8025FB10* obj, int delta) {
    s32 value = obj->mCounter884 + delta;
    obj->mCounter884 = value;
    if (value < 0) {
        obj->mCounter884 = 0;
    } else if (value > 0x3e7) {
        obj->mCounter884 = 0x3e7;
    }
}

void func_802618AC(u8* obj, int value) {
    int* field = (int*)(obj + 0x884);
    *field = value;
    if (value < 0) *field = 0;
    else if (value > 0x3e7) *field = 0x3e7;
}

void func_eu_80263A24(){}

// Target: us-80263b68 - init the shared text globals and allocate the tag
// processor + text buffer on the lib-layout alloc handle. The ctor is called
// by its literal retail symbol (unmangled __ct__CTagProcessor) so the call
// reloc name matches retail; its return value (this) flows back into tagMem
// so MWCC keeps it in r3 across the call like retail.
int lbl_eu_80664864;
int lbl_eu_80664868;
int lbl_eu_80662980;
int lbl_eu_80664874;

void func_802618D8(u8* arg) {
    lbl_eu_80664864 = 0;
    lbl_eu_80664868 = 0;
    lbl_eu_80662980 = -1;
    lbl_eu_80664874 = 0;
    lbl_eu_80664860 = arg;

    u32 handle = getAllocHandle__10CLibLayoutFv();
    u8* tagMem = (u8*)mtl::MemManager::allocate(0x858, handle);
    if (tagMem != NULL) {
        tagMem = (u8*)__ct__CTagProcessor(tagMem);
    }
    lbl_eu_8066486C = (nw4r::ut::TagProcessorBase<wchar_t>*)tagMem;

    u32 handle2 = getAllocHandle__10CLibLayoutFv();
    lbl_eu_80664870 = (u8*)mtl::MemManager::allocate_head(handle2, 0x400, 0x20);
}

void func_80261944(int arg) {
    lbl_eu_80664864 = arg;
    lbl_eu_80664868 = arg;
    lbl_eu_80662980 = -1;
    lbl_eu_80664874 = 0;
}

void func_80261960(){}

void func_80261A80(){}

void __dt__80261B1C(){}

void func_80261B98(const wchar_t* text, f32 x, f32 y) {
    Mtx44 identity;
    Mtx44 projection;

    PSMTXIdentity(identity);
    GXLoadPosMtxImm(identity, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    if (!func_8044BE24__8CGXCacheFv(cacheInstance__9CDeviceGX)) {
        f32 width = static_cast<f32>(CDeviceVI::getRenderModeObj()->fbWidth);
        f32 height = static_cast<f32>(CDeviceVI::getRenderModeObj()->efbHeight);
        C_MTXOrtho(projection, lbl_eu_806688D0, height,
                   lbl_eu_806688D0, width, lbl_eu_806688D0,
                   lbl_eu_806688D4);
    } else {
        GXCacheTextProjection* cache =
            static_cast<GXCacheTextProjection*>(cacheInstance__9CDeviceGX);
        s16 cacheWidth = cache->width;
        f32 renderWidth =
            static_cast<f32>(CDeviceVI::getRenderModeObj()->fbWidth);
        s16* rect = func_8044BE1C__8CGXCacheFv(cache);
        f32 rectRatio = static_cast<f32>(rect[2]) / renderWidth;
        f32 right = static_cast<f32>(cacheWidth) * rectRatio;

        cache = static_cast<GXCacheTextProjection*>(
            cacheInstance__9CDeviceGX);
        cacheWidth = cache->width;
        renderWidth =
            static_cast<f32>(CDeviceVI::getRenderModeObj()->fbWidth);
        rect = func_8044BE1C__8CGXCacheFv(cache);
        f32 rectRatioLeft = static_cast<f32>(rect[0]) / renderWidth;
        f32 left = static_cast<f32>(cacheWidth) * rectRatioLeft;

        s16 cacheHeight = cache->height;
        f32 renderHeight =
            static_cast<f32>(CDeviceVI::getRenderModeObj()->efbHeight);
        rect = func_8044BE1C__8CGXCacheFv(cache);
        f32 rectRatioBottom = static_cast<f32>(rect[3]) / renderHeight;
        f32 bottom = static_cast<f32>(cacheHeight) * rectRatioBottom;

        cache = static_cast<GXCacheTextProjection*>(
            cacheInstance__9CDeviceGX);
        cacheHeight = cache->height;
        renderHeight =
            static_cast<f32>(CDeviceVI::getRenderModeObj()->efbHeight);
        rect = func_8044BE1C__8CGXCacheFv(cache);
        f32 rectRatioTop = static_cast<f32>(rect[1]) / renderHeight;
        f32 top = static_cast<f32>(cacheHeight) * rectRatioTop;

        C_MTXOrtho(projection, top, bottom, left, right,
                   lbl_eu_806688D0, lbl_eu_806688D4);
    }
    GXSetProjection(projection, GX_ORTHOGRAPHIC);

    WideTextWriter writer;
    writer.SetupGX();
    u32 writerRegion = (u32)&writer & 0xFF000000;

    setTagProcessorChecked(&writer, lbl_eu_8066486C);
    setDrawFlagChecked(&writer, 0x110);
    setScaleChecked(&writer, lbl_eu_806688D8, lbl_eu_806688DC);
    setCharSpaceChecked(&writer, lbl_eu_806688D0);

    const nw4r::ut::Font* font = func_80449160__10CFontLayerFv(
        static_cast<u8*>(lbl_eu_80664860) + 0x1c4, 1);
    setFontChecked(&writer, font);
    validateTextColorPointer(&writer);
    writer.SetTextColor(nw4r::ut::Color(0, 0, 0, 255));

    setCursorChecked(&writer, writerRegion, x - lbl_eu_806688D8,
                     y - lbl_eu_806688D8, lbl_eu_806688E0);
    u32 textRegion = (u32)text & 0xFF000000;
    printCheckedInitial(&writer, writerRegion, text, textRegion);
    setCursorChecked(&writer, writerRegion, x + lbl_eu_806688D8,
                     y - lbl_eu_806688D8, lbl_eu_806688E0);
    printChecked(&writer, writerRegion, text, textRegion);
    setCursorChecked(&writer, writerRegion, x - lbl_eu_806688D8,
                     y + lbl_eu_806688D8, lbl_eu_806688E0);
    printChecked(&writer, writerRegion, text, textRegion);
    setCursorChecked(&writer, writerRegion, x + lbl_eu_806688D8,
                     y + lbl_eu_806688D8, lbl_eu_806688E0);
    printChecked(&writer, writerRegion, text, textRegion);

    validateTextColorPointer(&writer);
    writer.SetTextColor(nw4r::ut::Color(255, 255, 255, 255));
    setCursorChecked(&writer, writerRegion, x, y, lbl_eu_806688D0);
    printChecked(&writer, writerRegion, text, textRegion);
    setCursorChecked(&writer, writerRegion, x, y, lbl_eu_806688D0);
    printChecked(&writer, writerRegion, text, textRegion);
    setCursorChecked(&writer, writerRegion, x, y, lbl_eu_806688D0);
    printChecked(&writer, writerRegion, text, textRegion);
}

#undef VALIDATE_NW4R_POINTER_FLAG
#undef VALIDATE_NW4R_POINTER_COMPACT
#undef VALIDATE_NW4R_POINTER

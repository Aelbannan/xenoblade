#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

class CEventFile;
class CFileHandle;

namespace nw4r { namespace lyt { class DrawInfo; class AnimTransform; class ArcResourceAccessor; class Layout; class Pane; } }
namespace nw4r { namespace math { struct VEC3; } }

// C++-mangled imports from code_80135FDC.cpp: the unmangled identifiers mangle
// to the retail symbols func_801390E0__FPP11CFileHandle and
// func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor (see CItemBoxLine.hpp).
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);

// Fake SI interface for the nw4r::lyt::Layout deleting-destructor dispatch at
// vtable slot 2 (2 hidden RTTI prefix slots, so the first declared virtual is
// at +0x8). Real virtual dispatch reproduces the retail `lwz r12,0(r3);
// lwz r12,8(r12); mtctr; bcctrl` sequence; the manual `(*(void***)layout)[2]`
// cast emits a scratch r5 vtable load (see MWCC_REFERENCE: manual vtable cast
// -> real virtual dispatch, r12 ABI register). CCur.cpp CBaseCurVt5 precedent.
struct CItemBoxLayoutDtorVt {
    virtual void destroy(u32 flags);  // slot 2 => +0x8, arg in r4
};

struct CItemBoxInfoState {
    u8 _00[0x04];
    UnkClass_8045F564 memRegion1;   // 0x04
    UnkClass_8045F564 memRegion2;   // 0x14
    CFileHandle* fileHandle1;       // 0x24
    CFileHandle* fileHandle2;       // 0x28
    nw4r::lyt::ArcResourceAccessor* arcResourceAccessor;     // 0x2C - nw4r::lyt::ArcResourceAccessor
    nw4r::lyt::ArcResourceAccessor* resource;                // 0x30 - second ArcResourceAccessor (released by func_80139124)
    nw4r::lyt::Layout* layout;
    nw4r::lyt::AnimTransform* animTransform1;   // 0x38 - nw4r::lyt::AnimTransform
    nw4r::lyt::AnimTransform* animTransform2;   // 0x3C - nw4r::lyt::AnimTransform
    u8 _40[0x50];
    u8 active;
    u8 _91[3];
    s32 state;
    u8 current;
    u8 visible;
    u8 _9A[0x26];
    s16 values[12];
};

struct CItemBoxInfo {
    CItemBoxInfoState state;
    ~CItemBoxInfo();
    bool OnFileEvent(CEventFile*);

    u8 getItemBoxState();
    void advanceItemBoxState();
    void setItemBoxIndex(unsigned char index, short value);
};

// Free-function helper (retail: tryActivateItemBox__FP12CItemBoxInfo)
void tryActivateItemBox(CItemBoxInfo* info);

// Recovered free functions (retail: getItemBoxState__FP12CItemBoxInfo)
u8 getItemBoxState(CItemBoxInfo* self);

struct CItemBoxInfo2 {
    CItemBoxInfoState state;
    ~CItemBoxInfo2();
    bool OnFileEvent(CEventFile*);

    void drawItemBox2Layout(nw4r::lyt::DrawInfo* drawInfo);
    void advanceItemBox2State();
};

u8 getItemBox2State(CItemBoxInfo2* self);

struct CItemBoxInfoEntry {
    u16 itemId;
    u16 _02;
    u32 value;
    u8 state;

};

// 8-byte color (4 s16 channels) used by the item-box layout renderer.
struct CItemBoxQuadColor {
    s16 r, g, b, a;
    CItemBoxQuadColor() : r(0), g(0), b(0), a(0) {}
    CItemBoxQuadColor(s16 _r, s16 _g, s16 _b, s16 _a) : r(_r), g(_g), b(_b), a(_a) {}
};

// 4 selection/vertex colors (0x20 bytes) built per-slot in func_801E2928.
struct CItemBoxQuad {
    CItemBoxQuadColor col[4];
};

// 6-byte slot table: a u32 (bytes 0..3) + u16 (bytes 4..5), read from two
// consecutive .sdata2 constants (lbl_eu_8066805C + 80668060, etc.).
union CItemBoxSlotBytes {
    struct { u32 a; u16 b; } ab;
    u8 bytes[6];
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" char lbl_eu_805063BC[];
extern "C" void func_80136B4C(nw4r::lyt::Layout*, const char*, const char*, u32);
extern "C" int func_8026178C(void*, u32);
extern "C" u32 func_8025FB10(void*, u32);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void* func_8009EC9C(u32);
extern "C" void func_80124270(void*, u32);
extern "C" u8 func_801361E8(u32, const char*, u32);
extern "C" u16 func_80139358(u32);
extern "C" u32 func_801392E4(void*);
extern "C" void func_801D8B08(CItemBoxInfo*);
extern "C" void func_801D85D8(CItemBoxInfo*);
extern "C" void func_801D59C0(u32*, void*, void*);
extern "C" void func_801D1F9C(void*, u32);
extern "C" void func_801C4B60(void*, u32, u32, u32, u32);
extern "C" void __as__11_GXColorS10FRC11_GXColorS10(void* dst, const void* src);
extern "C" void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern "C" char lbl_eu_8052E590[];
extern "C" char lbl_eu_8052E568[];
extern "C" void func_801D4C9C(CItemBoxInfo*);
extern "C" void func_801D4D64(CItemBoxInfo*);
extern "C" void func_801E17EC(CItemBoxInfo2*);
extern "C" void func_801E4090(CItemBoxInfo2*);
extern "C" void func_801E3B9C(CItemBoxInfo2*);
extern "C" void func_801E2558(u32*, void*, void*);
extern "C" u32 func_801E92B8(void*, void*);
extern "C" u32 func_801DFD60(void*, void*, u32);
extern "C" u32 func_801DF610(void*, void*, u32, void*);
extern "C" double lbl_eu_80668028;
extern "C" void func_801E1868(CItemBoxInfo2*);
extern "C" void func_801E18B4(CItemBoxInfo2*);
extern "C" void func_801E1930(CItemBoxInfo2*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" void* lbl_eu_80534B28[];
extern "C" void* lbl_eu_80534A90[];
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void copyVEC3(void*, void*);
// C-linkage position-set helper (retail-unmangled func_80137924; same
// signature as CSysWin.hpp / CMenuPlayAward.hpp).
extern "C" void func_80137924(nw4r::math::VEC3*, nw4r::lyt::Pane*, nw4r::lyt::Pane*, nw4r::lyt::Pane*);
// More flat retail (unmangled) helpers used by the item-box renderers.
extern "C" void func_80137B44(nw4r::lyt::Layout*, const char*, u32);
extern "C" char* func_80136190(char*, char*, u32);
extern "C" char* func_8013639C(void*, char*);
extern "C" u32 func_801392C0();
extern "C" u32 func_801392B4(u32);
extern "C" void func_801E40E8(CItemBoxInfo2*);
extern "C" void func_801E27D0(u8*, void*, void*, void*);
extern "C" void* CItem_initItemImplInstances(void*);
extern "C" void func_801D62F8(void*, u32, const void*);
extern "C" void func_801D8930(CItemBoxInfo*);
extern "C" void func_801E37C4(CItemBoxInfo2*, void*, void*);

// 0x24-byte item-box slot record (built by func_801E27D0 and copied by the
// renderers): count byte, string pointer, per-slot text pointers, a 4-byte
// gap, per-slot byte values, and a tail whose byte 1 doubles as the build
// counter. Whole-struct copies of this size compile to the retail mtctr
// 8-byte-pair copy loop.
struct CItemBoxSlotRecord {
    u8 count;      // 0x00
    u8 _01[3];     // 0x01
    u32 str;       // 0x04
    u32 text[4];   // 0x08
    u8 _18[4];     // 0x18
    u8 vals[4];    // 0x1C
    u8 tail[4];    // 0x20 (tail[1] = counter at 0x21)
};


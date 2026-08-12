#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CBaseCur.hpp"

namespace nw4r {
namespace lyt {
    class Layout;
    class Pane;
    class AnimTransform;
    class ArcResourceAccessor;
    class DrawInfo;
}
}

// Full `this` struct for harness decomp of CCollepedia.cpp
// Entry within the sub-block array (stride 0xA)
struct CCollepediaEntry {
    u8 _00[0x08];
    u16 field_08; // u16 at offset +0x08 within entry
};

// Sub-range of the CCollepedia class from +0x00 to +0xE8
struct CCollepediaFull {
    u8 _00[0xD8];
    u8 field_D8;
    u8 field_D9;
    u8 _DA[0xE8 - 0xDA];
    u8 field_E8_start; // sub-array starts at +0xE8
};

// Container with a layout pointer at +0x04 (used by func_80253970)
struct LayoutContainer {
    u8 _00[0x04];
    nw4r::lyt::Layout* mpLayout; // at +0x04
};

// Cursor class derived from CBaseCur, used by CCollepedia for cursor display.
// Vtable at lbl_eu_80537474 (3 entries, size 0x18 with padding).
class CCLPCur : public CBaseCur {
public:
    CCLPCur(nw4r::lyt::ArcResourceAccessor* pAccessor);
};

// CCollepedia is a non-virtual class in the decomp (layout is flat, no vtable ptr).
// Virtual destructor semantics are handled by __dt__<addr> thunks.
// The vtable pointer is stored at +0x00 as a regular field.
// CCollepedia is a non-virtual class in the decomp (layout is flat, no vtable ptr).
// Virtual destructor semantics are handled by __dt__<addr> thunks.
// The vtable pointer is stored at +0x00 as a regular field.
//
// NOTE: offset 0x0C is used as both a 4-byte state machine (via individual byte
// stores in state functions) and as a pointer in some rendering functions.
// The struct exposes it as a pointer; state functions access individual bytes
// via reinterpret_cast.
struct CCollepedia {
    /* 0x00 */ int field_0; // vtable ptr (set by ctor)
    /* 0x04 */ nw4r::lyt::Layout* field_4;
    /* 0x08 */ nw4r::lyt::Layout* field_8; // used as Layout* in rendering, AnimTransform* in state checks
    /* 0x0C */ nw4r::lyt::AnimTransform* field_c_ptr; // state machine (low bytes) or AnimTransform*
    /* 0x10 */ nw4r::lyt::AnimTransform* field_10_ptr;
    /* 0x14 */ u8 _14[0x24 - 0x14];
    /* 0x24 */ void* field_24; // CFileHandle* - file handle 1
    /* 0x28 */ void* field_28; // CFileHandle* - file handle 2
    /* 0x2C */ void* field_2C; // CFileHandle* - archive file handle
    /* 0x30 */ nw4r::lyt::ArcResourceAccessor* field_30;
    /* 0x34 */ nw4r::lyt::ArcResourceAccessor* field_34;
    /* 0x38 */ nw4r::lyt::Layout* field_38;
    /* 0x3C */ nw4r::lyt::AnimTransform* field_3C;
    /* 0x40 */ nw4r::lyt::AnimTransform* field_40; // AnimTransform* at 0x40
    /* 0x44 */ nw4r::lyt::AnimTransform* field_44; // AnimTransform* at 0x44
    /* 0x48 */ u8 field_48;
    /* 0x49 */ u8 field_49;
    /* 0x4A */ u8 _4A[0x4C - 0x4A];
    /* 0x4C */ u32 field_4C;
    /* 0x50 */ u8 field_50;
    /* 0x51 */ u8 field_51;
    /* 0x52 */ u8 _52[0x54 - 0x52];
    /* 0x54 */ u8 field_54[0x9C - 0x54]; // sub-object at 0x54 (used by func_801D216C)
    /* 0x9C */ u8 field_9C; // CSysWin sub-object starts here
    /* 0x9D */ u8 _9D[0xD8 - 0x9D];
    /* 0xD8 */ u8 field_D8;
    /* 0xD9 */ u8 field_D9;
    /* 0xDA */ u8 field_DA;
    /* 0xDB */ u8 _DB; // padding
    /* 0xDC */ float field_DC[3]; // VEC3 copied from pane position (0xDC-0xE7)
    /* 0xE8 */ u8 field_E8; // sub-array starts here
    /* 0xE9 */ u8 _E9[0x28F0 - 0xE9];
    /* 0x28F0 */ nw4r::lyt::Layout* field_28F0; // Layout* at 0x28F0 (used by func_80254B64)
    /* 0x28F4 */ u8 _28F4[0x28F9 - 0x28F4];
    /* 0x28F9 */ u8 field_28F9;
    /* 0x28FA */ u8 field_28FA;

    ~CCollepedia();
};

// Abstract struct for CSysWin vtable dispatch at slot 34 (offset 0x88)
// MWCC adds 2 implicit entries (RTTI/dtor) before first user function.
struct CSysWinProxy {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0; // vtable slot 34 = offset 0x88 (after 2 implicit entries)
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" nw4r::math::VEC3* code80135FDC_setVec3(float*, float, float, float);
extern "C" void __dl__FPv(void*);
extern "C" u32 CSysWin_isReady(u8*);
extern "C" u32 CSysWin_getUnk34(u8*);
extern "C" int CSysWin_isActive(u8*);
extern "C" u32 func_801B481C();
extern "C" u32 func_80158068(u16);
extern "C" char lbl_eu_8050C6E8[];
extern "C" void* lbl_eu_806640A8;
extern "C" void* lbl_eu_80664104;
extern "C" const float lbl_eu_80668800;
extern "C" const float lbl_eu_806687F0;
extern "C" const float lbl_eu_806687F4;
extern "C" const float lbl_eu_806687F8;
extern "C" const float lbl_eu_806687FC;
extern "C" void func_80138078__FUl(u32);
extern "C" void* lbl_eu_80537474[];
extern "C" void func_801D20B0(CBaseCur*, nw4r::lyt::DrawInfo*);
extern "C" void func_8022B7C8(void*, nw4r::lyt::DrawInfo*);
extern "C" u32 func_80136254(char*, char*, u16);
extern "C" void func_8013E2E0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern "C" void func_80137924(nw4r::math::VEC3*, nw4r::lyt::Pane*,
                               nw4r::lyt::Pane*, nw4r::lyt::Pane*);
extern "C" char* lbl_eu_806647DC;
extern "C" void* lbl_eu_806647D8;
extern "C" u32 func_8009EC6C(u16);
extern "C" u16 lbl_eu_8050C6A0[];
extern "C" void func_801D216C(void*, u8);
extern "C" u32 lbl_eu_806640EC;
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__6CCur07Fv(void*, int);
extern "C" void func_8022B9B4(void*, void*, int);
extern "C" void func_8022BFC8(CSysWin*, u8);
extern "C" void func_8022B8B8(void*);
extern "C" void func_8022B8E4(void*);
extern "C" void func_8022B7F4(void*);
extern "C" void func_8003AA8C__5CBdatFUl(u32);
extern "C" void func_801390E0__FPP11CFileHandle(void*);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" void func_8009EC18(u16, u32);
extern "C" u32 func_801587E8(u16);
extern "C" void* CItem_initItemImplInstances();
extern "C" void func_80158118(void*, u16, u32);
extern "C" void func_8013B428__FUl(u32);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u32 func_800A9D90();
extern "C" float func_801895EC();
extern "C" void func_80043738(u32, const char*, void*, u32, u32, u32);
extern "C" void func_80124270(void*, u32);
extern "C" void copyVEC3(void*, void*);
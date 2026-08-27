#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r { namespace lyt { class Layout; class ArcResourceAccessor; } }
class CScn;

/*
 * Pause menu screen controller.
 *
 * Explicit layout (no single-inheritance from CProcess): the CProcess base
 * region is embedded 0x00-0x57, the IScnRender secondary subobject lives at
 * +0x5c, the scene pointer at +0x60, a layout region at +0x64, the nw4r
 * layout at +0x7c, and the current state byte at +0x84. The dtor destroys the
 * UnkClass_8045F564 region and then tears down the CProcess subobject OOL.
 *
 * Offsets (MWCC auto-installs CMenuPause's own vtable at 0x00, so the
 * declared fields below sit at +0x04):
 *   0x00  CMenuPause vtable (own virtuals)
 *   0x04  embedded CProcess region (mBProcess, 0x58 bytes -> 0x5c)
 *   0x5c  IScnRender subobject
 *   0x60  CScn* mScene
 *   0x64  UnkClass_8045F564 mMemRegion   (0x10 bytes -> 0x74)
 *   0x74  field bytes
 *   0x7c  nw4r::lyt::Layout* mLayout
 *   0x80  u32 field_0x80
 *   0x84  u8 mState
 */
class CMenuPause {
public:
    virtual ~CMenuPause();
    CMenuPause();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    unsigned long isInitialized();

    u8 mBProcess[0x58];              // 0x04 (auto vtable at 0x00)
    void* mRenderVtable;             // 0x5c IScnRender subobject vtable ptr
    CScn* mScene;                    // 0x60
    UnkClass_8045F564 mMemRegion;    // 0x64
    u8 mField74[0x7c - 0x74];        // 0x74..0x7b
    nw4r::lyt::Layout* mLayout;      // 0x7c
    u32 mField80;                    // 0x80
    u8 mState;                       // 0x84
};

// Global singleton root from which func_80252538 / isInitialized read.
extern CMenuPause* lbl_eu_806647C8;

// C-linkage imports (retail symbol names -- these identifiers are pre-mangled
// or unmangled in retail, so they must keep C linkage to bind to the literal
// name).
extern const f32 lbl_eu_806687DC;

// u16 -> f32 conversion helper (CfTFile.cpp convention): build the
// 0x43300000-prefixed bit pattern in memory and subtract the named 2^52
// sdata2 magic (lbl_eu_806687D0) so the pool reloc matches retail instead of
// an MWCC-synthesised @N entry.
union CMenuPauseF64Conv {
    u32 w[2];
    double d;
};

// Mirror of the font object returned by CDeviceFont::getFontInfo.
// vtable slot 9 (offset 0x24) yields the u32 bound into the layout's font
// pane. Never instantiated, so no vtable is emitted; a genuine virtual call
// makes MWCC emit the retail r12 dispatch sequence.
struct CMenuPauseFontView {
    virtual void sf0() = 0;
    virtual void sf1() = 0;
    virtual void sf2() = 0;
    virtual void sf3() = 0;
    virtual void sf4() = 0;
    virtual void sf5() = 0;
    virtual void sf6() = 0;
    virtual void sf7() = 0;
    virtual void sf8() = 0;
    virtual u32 sf9() = 0; // vtable slot 9 (offset 0x24)
};

// Texture object returned by ArcResourceAccessor::GetResource for the 'timg'
// tag. mChain (+0x08) -> mDims (+0x00) carries the 2D dimension header.
struct CMenuPauseTexDims {
    u16 field_0x0; // +0x00
    u16 field_0x2; // +0x02
};
struct CMenuPauseTexChain {
    CMenuPauseTexDims* mDims; // +0x00
};
struct CMenuPauseTexObj {
    u8 _0[0x8];
    CMenuPauseTexChain* mChain; // +0x08
};

extern "C" {
    void __dt__8CProcessFv(void*, int);
    void func_80138078__FUl(u32);
    void func_80188890(u32);
    int isClassicController__Q22cf13CfGameManagerFv(int arg);
    void func_801BFB34(f32, int, int, int);
    void func_eu_8013C8E8();
    void func_800853C8__Q22cf13CfGameManagerFv();
    void setPresentationFlag__Q22cf13CfGameManagerFv(bool);

    // CMenuPause factory constructor imports.
    void __ct__8CProcessFv(void*);
    void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
    u32 getWorkMem__17CWorkThreadSystemFv();
    void* allocate__Q23mtl10MemManagerFUlUl(u32, u32);
    void Regist__8CProcessFP8CProcessb(void*, void*, bool);
    extern u32 __ptmf_null[];
    extern u32 lbl_eu_8052BF70[];
    extern char lbl_eu_805371A0[];
    extern char lbl_eu_8050C5C8[];
    extern const f32 lbl_eu_806687D8;
    extern const f64 lbl_eu_806687D0;

    // Init() resource/layout helpers.
    void* getHandleMEM2__Q23mtl10MemManagerFv();
    void* __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
    void __dt__14Class_8045F858Fv(void*, int);
    void setMemInitFlag__Q23mtl10MemManagerFb(bool);
    void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
    void validateHeap__17UnkClass_8045F564Fv(void*);
    void addRenderCB__4CScnFP10IScnRenderUlUl(void*, void*, u32, u32);
    nw4r::lyt::ArcResourceAccessor* func_801355F4();
}
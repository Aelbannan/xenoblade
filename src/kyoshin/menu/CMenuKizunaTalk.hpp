#pragma once

#include <types.h>
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

class CProcess;
class CScn;
namespace nw4r {
namespace lyt {
    class Layout;
    class AnimTransform;
    class ArcResourceAccessor;
}
}

// IUIWindow region (0x6C): CProcess base (0x00-0x3B) + vtable + window fields.
// The dtor is declared (defined by the retail __dt__9IUIWindowFv) so the
// derived dtor emits the base-destruction call; it is deliberately
// NON-virtual so MWCC keeps the vtable pointer at +0x10 (retail layout).
class IUIWindow {
public:
    ~IUIWindow();
    u8 _00[0x10];                  // 0x00 - CProcess link-list region
    u32 mField10;                  // 0x10 - vtable
    u8 _14[0x54 - 0x14];           // 0x14 - ptmf slots
    nw4r::lyt::Layout* mLayout;    // 0x54
    u32 mField58;                  // 0x58
    u32 mField5C;                  // 0x5C
    s32 mField60;                  // 0x60
    u8 mField64;                   // 0x64
    u8 mField65;                   // 0x65
    u8 mField66;                   // 0x66
    u8 mField67;                   // 0x67
    u32 mField68;                  // 0x68
};

// IWorkEvent / IScnRender base subobjects: 4-byte vtable slots into the retail
// composite vtable (+0x24 / +0xAC, see the ctor). Plain PODs - the retail
// subobject vtables are stored by the ctor, not by MWCC.
struct IWorkEvent {
    u32 mVtable;
};
struct IScnRender {
    u32 mVtable;
};

// Minimal CScn view: only the render-callback members are used by this TU
// (retail symbols addRenderCB__4CScnFP10IScnRenderUlUl /
// removeRenderCB__4CScnFP10IScnRender). Defined OOL in monolib/src/scn/CScn.
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};

// Layout vtable view: MWCC inserts an offset-to-top + RTTI prefix (2 entries)
// so the first declared virtual sits at vtable + 0x8. All-pure so MWCC emits
// no vtable for the view itself.
class CLayoutView {
public:
    virtual void vf02(int flag) = 0;   // vtable + 0x8 - release/unbind
};

// CCur18 vtable view: MWCC prefix again, so user virtual 1 sits at +0xC.
class CCur18View {
public:
    virtual void vf02() = 0;           // vtable + 0x8
    virtual void vf03() = 0;           // vtable + 0xC - per-frame update
    virtual void vf04(void* arg) = 0;  // vtable + 0x10 - cursor selection update
};

// CSysWin vtable view exposing the layout-build virtual at +0x88 (slot 34).
// Same shape as the CSysWinView classes in CSysWinSave.hpp / CCol6System.hpp,
// renamed to avoid ODR collisions.
class KizunaSysWinView {
public:
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v0A() = 0;
    virtual void v0B() = 0;
    virtual void v0C() = 0;
    virtual void v0D() = 0;
    virtual void v0E() = 0;
    virtual void v0F() = 0;
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
    virtual void v1A() = 0;
    virtual void v1B() = 0;
    virtual void v1C() = 0;
    virtual void v1D() = 0;
    virtual void v1E() = 0;
    virtual void v1F() = 0;
    virtual void v20() = 0;  // index 32 -> vtable +0x88
};

// Field view over a CCur18-shaped blob: four words then two lone bytes
// (+0x14/+0x15) copied individually (retail lbz/stb pairs).
struct KizunaCurFields {
    u8 _00[0x4];
    u32 f_04;
    u32 f_08;
    u32 f_0c;
    u32 f_10;
    u8 f_14;
    u8 f_15;
};

// func_801BCF48 stack scratch laid out like the retail frame:
// cursor @sp+0x08, case-2 CSysWin @sp+0x20, case-0 CSysWin @sp+0x5c.
struct KizunaScratch {
    u8 mCur[0x18];
    u8 mWin2[0x3C];
    u8 mWin0[0x3C];
};

/*
 * Kizuna (friendship) talk screen - singleton, created by func_801BCEBC.
 *
 * Retail layout (ctor / dtor / Term):
 *   0x00  IUIWindow base (0x6C)
 *   0x6C  IWorkEvent base (composite vtable + 0x24)
 *   0x70  IScnRender base (composite vtable + 0xAC)
 *   0x74  CScn* owning scene (render-callback target)
 *   0x78  UnkClass_8045F564 storage (0x10 bytes)
 *   0x88/0x8C  CFileHandle* (talk-list file / shared archive)
 *   0x90  nw4r::lyt::ArcResourceAccessor*
 *   0x94  nw4r::lyt::AnimTransform*
 *   0x98  character id (BDAT affinity index)
 *   0x9C  page byte (signed; clamps in func_801BD2F8)
 *   0xA0  move state
 *   0xA4  window state (switch in func_801BD594)
 *   0xA8  CCur18 cursor storage (0x18 bytes)
 *   0xC0  CSysWin panel storage (0x3C bytes)
 *   sizeof = 0xFC
 *
 * The class is deliberately NON-polymorphic: the retail ctor stores the
 * composite / subobject vtables by hand and the retail dtor emits no vtable
 * store, so the dtor is non-virtual and the bases carry no virtuals.
 */
class CMenuKizunaTalk : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    CScn* mScene;                        // 0x74
    u8 mMemRegion[0x10];                 // 0x78 - UnkClass_8045F564 storage
    CFileHandle* mFile1;                 // 0x88
    CFileHandle* mFile2;                 // 0x8C
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;  // 0x90
    nw4r::lyt::AnimTransform* mAnim;     // 0x94
    u32 mCharId;                         // 0x98
    s8 mField9C;                         // 0x9C
    u32 mFieldA0;                        // 0xA0
    u8 mFieldA4;                         // 0xA4
    u8 _padA5[3];                        // 0xA5 - padding before mCur
    u8 mCur[0x18];                       // 0xA8 - CCur18 cursor storage
    u8 mSysWin[0x3C];                    // 0xC0 - CSysWin panel storage

    ~CMenuKizunaTalk();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void OnFileEvent();
};

// C-linkage imports not covered by a single conflict-free committed header
// (the CSysWin / UnkClass_8045F564 dtors and the CDeviceVI / CProcess base
// ctor); the rest come from CItemBoxGrid.hpp / functions.hpp.
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
extern "C" void __ct__8CProcessFv(void*);
extern "C" void waitForDrawDone__9CDeviceVIFv();

// Flat cf pad-data view for the kizuna talk input handling (offsets match
// retail, same as CSysPadData / KizunaPadData views elsewhere).
struct KizunaTalkPadData {
    u8 _00[0x4];
    u32 mPressedFlags;          // +0x04 CPad::mPressedButtonFlags
    u8 _08[0x104 - 0x8];
    u32 mTurboFlags;            // +0x104 CfPadData::mTurboPressButtonFlags
};

// C-ABI helper imports for the kizuna talk input / file-event handlers. The
// retail symbols are unmangled, so they must stay C-linkage (the C++-linkage
// declarations in CSysWinSelect.hpp/CSysWinSave.hpp would mangle them).
extern "C" KizunaTalkPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" void func_8022C1B4(void* out, void* syswin, u8 sel);
extern "C" u16 func_8013A7D0(u8 a, u8 b);
extern "C" int func_8006A6D0();
extern "C" void setBdatEntry__5CBdatFUlPv(u32 value, void* data);
extern "C" void* func_8003AA34();

// Layout animation frame-target check (CSysWin.cpp / CCol6System.hpp decl).
int advanceAnimTransform(nw4r::lyt::AnimTransform* anim, float frame);

#include "kyoshin/CItemBoxGrid.hpp"
// func_801361E8 comes from CItemBoxGrid.hpp's wide-view (u32) decl - the
// callee's lbz already zero-extends and callers keep the raw result (mr),
// masking only at use. The old rename guard + local redeclaration are gone.

// u32-word pair / f64 view used for MWCC's 0x43300000 int->float conversion
// (subtracts the retail .sdata2 magic double lbl_eu_80667E70).
union KizunaF64Conv {
    u32 w[2];
    f64 d;
};

// Minimal nw4r texture view returned by the arc accessor's texture lookup:
// +0x08 points at the texture header (u16 height/width pair).
struct KizunaTexHeader {
    u16 mHeight;   // 0x00
    u16 mWidth;    // 0x02
};
struct KizunaTextureView {
    u8 _00[0x8];
    KizunaTexHeader* mHeader;   // 0x08
};

// Global data imports (MWCC does not mangle global-scope data names).
extern char lbl_eu_80505118[];            // shared-archive path string (.rodata)
extern CMenuKizunaTalk* lbl_eu_80664420;  // singleton instance pointer (.sbss)
extern u8* lbl_eu_80664424;               // BDAT character table pointer (.sbss)
extern const f32 lbl_eu_80667E68;         // .sdata2 float (anim check)
extern const f32 lbl_eu_80667E6C;         // .sdata2 float (anim frame reset 0.0f)
extern const f64 lbl_eu_80667E70;         // .sdata2 double (0x43300000 int->float magic)
extern char lbl_eu_8052D238[];            // CTTask_IUIWindow vtable (.data)
extern char lbl_eu_80533A98[];            // CMenuKizunaTalk composite vtable (.data)
extern u32 __ptmf_null[3];                // null pointer-to-member-function

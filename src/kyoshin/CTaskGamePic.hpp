#pragma once

#include <types.h>
#include <revolution/GX.h>
#include <revolution/tpl/TPL.h>

#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"

#include "monolib/scn/CScn.hpp"

// CProcess base constructor imported from another TU (retail C-linkage symbol
// name - do not let C++ mangle its parameter list).
extern "C" void __ct__8CProcessFv(CProcess* ths);

// null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// 3-word view of a pointer-to-member-function slot (mMoveFunc/mDrawFunc).
struct PTMF3 {
    u32 w[3];
};

// Retail vtable regions for CTaskGamePic (imports; .data). lbl_eu_80538AD8 is
// the final CTaskGamePic vtable; lbl_eu_80538BC0 is the interim
// CTTask<CTaskGamePic> vtable (overwritten by the derived vptr in the ctor).
extern char lbl_eu_80538AD8[];
extern char lbl_eu_80538BC0[];

// Vec4 helper + its parameter struct are declared once in include/functions.hpp
// (retail unmangled CTaskGame_setVec4 symbol).
#include "functions.hpp"

// cbRenderBefore imports: CView::fitRectToView's retail symbol is the
// pre-mangled global name (member `this` unused in the body, rect in r3 /
// view in r4 - see monolib/src/core/CView.cpp); the C++ member declaration
// in CView.hpp passes this in r3 (wrong call shape), and a plain global
// declaration would be re-mangled, so the import is pinned with C linkage
// (same scheme as kyoshin/cf/CTaskREvent.hpp, which uses the raw name).
class CView;
namespace ml {
struct CRect;
}
extern "C" void fitRectToView__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect, CView* view);

// Bottom edge of an ml::CRect (retail C-linkage helper defined in this unit).
extern "C" s16 GamePicRectBottom(const ml::CRect* r);

// Texture/palette file data read by cbRenderBefore: the loaded TPL image is
// a palette header, its descriptor, and the first texture header.
struct CTaskGamePicTexData {
    TPLPalette mPalette;        // 0x00
    TPLDescriptor mDescriptor;  // 0x0C
    TPLHeader mTextureHeader;   // 0x14
};

// First-word view of the stack CDrawGX so cbRenderBefore can clear the
// texture-cache flag bit 27 (mFlags is private in the shared CDrawGX.hpp,
// which is outside this TU's writable scope).
struct CDrawGXFlagWord {
    u32 mFlags; // 0x0
};

// Generic task wrapper - canonical monolib template (declared-only members so
// the unit cpp can emit the retail out-of-line Move/Draw/dtor symbols via
// explicit `template<>` specializations).
#include "monolib/work/CTTask.hpp"

class CTaskGamePic : public CTTask<CTaskGamePic>, public IWorkEvent, public IScnRender {
public:
    CTaskGamePic();
    virtual ~CTaskGamePic();
    void Init();
    void Term();
    void Move();
    virtual void cbRenderBefore(CScn* scene);
    void create();
    void Draw();
    virtual bool OnFileEvent(CEventFile* pEvent);

    // 0x0-0x54: CTTask<CTaskGamePic>
    // 0x54: IWorkEvent vptr, 0x58: IScnRender vptr
    CScn* mScene;                     // 0x5C
    CFileHandle* mFileHandle;         // 0x60 async file handle
    u8* field_64;                     // 0x64 palette / loaded data
    u8* field_68;                     // 0x68 palette / loaded data (alias of 0x64)
    GXTexObj mTexObj;                 // 0x6C loaded GX texture object (0x20: 0x6C..0x8B)
    u8 field_8C;                      // 0x8C texture-ready flag (ctor zeroes it; cbRenderBefore gates on it)
    u8 field_8D[3];                   // 0x8D..0x8F gap after GXTexObj
    // 0x90-0xC4: texture/palette parameter block (read/written by GamePicShiftTexParams)
    u32 param_90;
    u32 param_94;
    u32 param_98;
    u32 param_9C;
    u32 param_A0;
    u32 param_A4;
    u32 param_A8;
    u32 param_AC;
    u32 param_B0;
    u32 param_B4;
    u32 param_B8;
    u32 param_BC;
    u32 param_C0;
    u32 param_C4;
}; // size: 0xC8
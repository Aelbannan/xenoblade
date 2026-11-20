#pragma once

#include "kyoshin/CSystemWindowApi.hpp"
#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/IUIWindow.hpp"
#include "kyoshin/QstData.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

namespace nw4r { namespace lyt {
    class Layout;
    class AnimTransform;
}}
class CScn;

// Mirror of the font object returned by CDeviceFont::getFontInfo. vtable
// slot 0x24 (index 9) yields the u32 bound into the layout's font pane via
// ApplyFontToTxtPanes. Never instantiated, so no vtable is emitted; a genuine
// virtual call makes MWCC emit retail's r12 dispatch sequence
// (CSimpleFontObj / CMenuPTGaugeFont scheme).
class CMenuQstCntFont {
public:
    virtual ~CMenuQstCntFont();   // 0x00 (3 dtor slots)
    virtual void vf0C();          // 0x0C
    virtual void vf10();          // 0x10
    virtual void vf14();          // 0x14
    virtual void vf18();          // 0x18
    virtual void vf1C();          // 0x1C
    virtual void vf20();          // 0x20
    virtual u32 getFontHandle();  // 0x24
};

// 8-byte quest entry record (used at 0x94 and 0x194 inside CMenuQstCnt).
struct QstEntry {
    QstEntry(int a = 0, int b = 0, u8 c = 0, u8 d = 0, u8 e = 0);
    QstEntry(const QstEntry& other);
    QstEntry& operator=(const QstEntry& other);

    u16 f0;   // 0x00 quest id
    u16 f2;   // 0x02
    u8  f4;   // 0x04
    u8  f5;   // 0x05
    u8  f6;   // 0x06
};

/*
 * Quest-status menu object (size 0x1A0).
 *
 * Real MI class, from the retail composite vtable __vt__11CMenuQstCnt
 * (0x80536128): IUIWindow at +0x00 (0x6C), IWorkEvent at +0x6C and
 * IScnRender at +0x70; the +0x24 / +0xAC sub-vtables and the base list are
 * compiler output.
 *
 *   0x00  IUIWindow base (Move/Draw ptmf hooks at +0x3C/+0x48, layout/state
 *         members at +0x54..+0x68)
 *   0x6C  IWorkEvent base
 *   0x70  IScnRender base (render callback subobject registered in Init)
 *   0x74  mScene      - owning scene
 *   0x78  mMemRegion  - embedded MEM2 scratch region (0x10)
 *   0x88  mAnim88     - open animation
 *   0x8C  mState8C
 *   0x90  mValue90    - layout value (0.0f)
 *   0x94  mEntries[32]- the quest-entry page records
 *   0x194 mSelEntry   - the selected quest record
 *   0x19C mField19C   - ready flag
 */
class CMenuQstCnt : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    CMenuQstCnt(CScn* scene, short a, short b, u8 c, u8 d, u8 e);
    virtual ~CMenuQstCnt();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();
    void RefreshDisplay();

    CScn* mScene;                        // 0x74 owning scene
    UnkClass_8045F564 mMemRegion;        // 0x78 embedded MEM2 region
    nw4r::lyt::AnimTransform* mAnim88;   // 0x88 open animation
    int mState8C;                        // 0x8C
    f32 mValue90;                        // 0x90
    QstEntry mEntries[32];               // 0x94
    QstEntry mSelEntry;                  // 0x194
    u8  mField19C;                       // 0x19C
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" int isLandTelopIdle();
extern "C" int getLandTelopE0(void);
extern "C" void __construct_array(void*, void*, void*, u32, u32);

extern "C" void Regist__8CProcessFP8CProcessb(void*, void*, bool);
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" int PTNotice_IsActive_3C10();

extern "C" int isQuestLogMenuActive();
extern "C" u32 gmPauseFlags;
extern "C" f32 lbl_eu_8066856C;
extern "C" f32 lbl_eu_80668568;
extern "C" void* lbl_eu_80664094;
extern "C" void* lbl_eu_806640A8;
extern "C" u32 lbl_eu_80573D18[];
extern "C" char lbl_eu_80509AB4[];

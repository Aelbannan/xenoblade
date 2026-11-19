#pragma once

#include <types.h>

namespace nw4r {
namespace lyt {
class Layout;
class AnimTransform;
class ArcResourceAccessor;
class Pane;
}
}
class CScn;
class CProcess;
class CEventFile;

#include "monolib/work/CProcess.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/IUIWindow.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// One entry of the internal message/queue array (stride 8).
struct UnkBattleEntry {
    u8 flag;   // +0x00
    u8 _pad[3];
    u32 param; // +0x04
};

/*
 * Battle-end (result) screen task.
 *
 * Real MI class, from the retail composite vtable __vt__14CMenuBattleEnd
 * (0x80537AB0): IUIWindow at +0x00 (0x6C, brings the CTTask<IUIWindow> Move/
 * Draw ptmf hooks and the layout/state members from +0x54), IWorkEvent at
 * +0x6C and IScnRender at +0x70 - the +0x24 / +0xAC sub-vtables and the base
 * list (IScnRender@0x70, IWorkEvent@0x6C) are the compiler's own output.
 *
 *   0x00  IUIWindow base
 *   0x6C  IWorkEvent base
 *   0x70  IScnRender base (render callback subobject registered in Init)
 *   0x74  mScene      - owning scene
 *   0x78  mMemRegion  - embedded MEM2 scratch region (0x10)
 *   0x88  mAnim88 / mAnim8C - the two selectable animations
 *   0x90  mAnim90     - currently playing animation
 *   0x94  mEntries[4] - queued {flag,param} requests
 *   0xB4  mState (s8), 0xB5 mMode, 0xB8 mParam
 * retail sizeof = 0xBC (what createBattleEndMenu allocates).
 */
class CMenuBattleEnd : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    CMenuBattleEnd(CScn* scene, u8 mode, u32 param);
    virtual ~CMenuBattleEnd();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    CScn* mScene;                          // 0x74 owning scene
    UnkClass_8045F564 mMemRegion;          // 0x78 embedded MEM2 region
    nw4r::lyt::AnimTransform* mAnim88;     // 0x88
    nw4r::lyt::AnimTransform* mAnim8C;     // 0x8C
    nw4r::lyt::AnimTransform* mAnim90;     // 0x90 currently playing
    UnkBattleEntry mEntries[4];            // 0x94 (0x20 bytes)
    s8  mState;                            // 0xB4 (retail compares signed)
    u8  mMode;                             // 0xB5
    u8  _padB6[2];
    u32 mParam;                            // 0xB8
};

// C-Linkage entry points: the retail symbols are unmangled (factory and the
// queue helpers are globals, not members).
extern "C" {
CMenuBattleEnd* createBattleEndMenu(CProcess* parent, CScn* scene, u8 mode, u32 param);
void advanceBattleEndQueue(CMenuBattleEnd* obj);
void queueBattleEndEntry(CMenuBattleEnd* obj, u8 flag, u32 param);
void __ct__17UnkClass_8045F564Fv(void*);
void __dt__17UnkClass_8045F564Fv(void*, int);
void deleteRegion__17UnkClass_8045F564Fv(void*);
void addRenderCB__4CScnFP10IScnRenderUlUl(CScn*, IScnRender*, u32, u32);
void removeRenderCB__4CScnFP10IScnRender(CScn*, IScnRender*);
void waitForDrawDone__9CDeviceVIFv();
void Regist__8CProcessFP8CProcessb(void*, void*, bool);
}

// .sbss globals shared with the retail binary (global-scope plain externs keep
// the unmangled symbol; no extern "C" needed outside a namespace).
extern u32 gmPauseFlags;
extern CMenuBattleEnd* lbl_eu_80664898;

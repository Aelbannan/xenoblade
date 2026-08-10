#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/core/CViewRoot.hpp"
#include "monolib/work/CWorkThread.hpp"

// Minimal CView surface for this TU. The read-only CView header declares
// func_8043DC20 arg-less (retail signature is unsigned long), so the members
// used here are declared with their true retail signatures; the head layout
// mirrors retail CView (vptr + CWorkThread base, mWorkID at +0x4C).
class CView {
public:
    static CView* getCurrentView();   // getCurrentView__5CViewFv
    void func_8043DC20(u32 arg);      // func_8043DC20__5CViewFUl

    u8 _00[0x4C];   // +0x00
    u32 mWorkID;    // +0x4C
};

// Camera item in the scene pool's camera list (pool kind 4 via func_8048C698).
// Head layout mirrors CScnItemCamera: vtable at +0x00, mIndex (camera id) at +0x0C.
struct CScnCameraItem {
    virtual void vf0() = 0;   // vtable slot 0 (+0x04)
    virtual void vf1() = 0;   // vtable slot 1 (+0x0C)
    virtual void vf2() = 0;   // vtable slot 2 (+0x10)
    virtual void vf3() = 0;   // vtable slot 3 (+0x14) - invoked by func_8049B024

    u32 mParent;   // +0x04
    u16 mType;     // +0x08
    s32 mIndex;    // +0x0C - camera id / index into CScnCameraMan::mViews
};

// Node in the camera circular list: next link at +0x00, item at +0x08.
struct CScnCameraNode {
    CScnCameraNode* next;   // +0x00
    u32 _04;                // +0x04
    CScnCameraItem* item;   // +0x08
};

// List handle returned by the scene-item-pool accessor (func_8048C698, kind 4);
// the circular-list sentinel lives at +0x04.
struct CScnCameraList {
    u32 _00;                  // +0x00
    CScnCameraNode* sentinel; // +0x04
};

// Parameter object for the camera manager: view id at +0x4C, pool at +0x60.
// The opaque field at +0x54 is handed to the view release/attach callbacks
// (func_8043DC20 and the vtable+0xB8 slot) as a pointer.
struct CScnCameraParam {
    u8 _00[0x4C];         // +0x00
    u32 mViewId;          // +0x4C - current view id
    u8 _50[4];            // +0x50
    u32 field_0x54;       // +0x54 - opaque; its address is passed to view callbacks
    u8 _58[8];            // +0x58 .. +0x60
    u8* mPool;            // +0x60 - scene item pool
};

// Work event payload for WorkEvent3 (event id 0x15 = camera select).
struct CScnCameraEvent {
    s32 mId;        // +0x00
    void* pData;    // +0x04
    u32 dataSize;   // +0x08
};

// C-ABI scene-item-pool accessor (defined with C linkage in CScnItemPool.cpp):
// returns the pool slot (list) for the given item kind (4 = cameras).
extern "C" CScnCameraList* func_8048C698(u8* pool, int kind);

// CView primary-vtable mirror: 32 IWorkEvent + 7 CWorkThread + 11 CView virtual
// slots. MWCC vtables carry an 8-byte RTTI header, so the virtual declared at
// index N is loaded at vtable+8+4*N; the retail call in this TU targets
// vtable+0xB8 (= declared index 44, CView_UnkVirtualFunc4). The read-only CView
// header declares that slot arg-less, so this abstract mirror (never
// instantiated) expresses the retail call shape with its real argument.
class CViewVt46 {
public:
    virtual void v00() = 0;   // IWorkEvent slots 0..31
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
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
    virtual void v32() = 0;   // CWorkThread slots 32..38
    virtual void v33() = 0;
    virtual void v34() = 0;
    virtual void v35() = 0;
    virtual void v36() = 0;
    virtual void v37() = 0;
    virtual void v38() = 0;
    virtual void v39() = 0;   // CView slots 39..44
    virtual void v40() = 0;
    virtual void v41() = 0;
    virtual void v42() = 0;
    virtual void v43() = 0;
    virtual void vfB8(void* arg) = 0;  // vtable+0xB8 (CView_UnkVirtualFunc4)
};

// Scene camera manager: keeps a camera-id -> view-id table and resolves the
// current camera through the scene pool's camera item list. The retail vtable
// is a plain data symbol (lbl_eu_8056EA40), so the class is __declspec(novtable)
// and the destructor re-points the vptr manually (same pattern as CScnLightMan).
class __declspec(novtable) CScnCameraMan : public IWorkEvent {
public:
    CScnCameraMan();
    virtual ~CScnCameraMan();
    virtual bool WorkEvent1(UNKTYPE* r4, const char* r5);
    virtual bool WorkEvent3(UNKTYPE* r4);

    // +0x00: vtable (compiler-managed)
    CScnCameraParam* mParam;   // +0x04
    u32 mViews[10];            // +0x08 - camera id -> view id table
    u32 mCount;                // +0x30
    s32 mCamId;                // +0x34
};

// Retail CScriptCode line parser. The retail symbol is the Fv (no-arg) mangled
// name but the function takes (self, pData, dataSize) - CScriptCode.cpp defines
// it with C linkage and the exact name, so the call site must too.
extern "C" void func_8043A1DC__11CScriptCodeFv(void* self, u8* pData, u32 dataSize);
// Retail CScnCameraMan vtable data symbol (stored manually - class is novtable).
extern "C" char lbl_eu_8056EA40[];

// .sdata2 constants shared by the free camera-item functions in this TU.
// The retail values live in the retail data blob; referencing them by name
// keeps the @sda21 relocs identical (no pooling drift).
extern const f32 lbl_eu_8066AB48;    // +0x00 (float pair with 8066AB4C)
extern const f32 lbl_eu_8066AB50;    // 0.0f
extern const f32 lbl_eu_8066AB54;    // 1.0f
extern const f32 lbl_eu_8066AB58;    // viewport-width scale
extern const double lbl_eu_8066AB60; // s16->f32 magic double (2^52 + 2^31)
extern const f32 lbl_eu_8066AB68;    // 0.0f
extern const f32 lbl_eu_8066AB6C;    // aspect multiplier

// .sdata globals for this TU.
extern u32 lbl_eu_80663A08;  // camera-select command pointer (compared in WorkEvent1)
extern u32 lbl_eu_80663A0C;  // mViews init sentinel (0xFFFFFFFF)

// Tail mirror of CGXCache: the read-only CGXCache.hpp header stops before the
// scissor-rect fields, so the s16 scissor deltas used by the camera aspect
// computation are declared here (cacheInstance + 0x4BC / + 0x4BE).
struct CGXCacheTail {
    u8 _00[0x4BC];
    s16 mScissorDeltaX;  // +0x4BC
    s16 mScissorDeltaY;  // +0x4BE
};

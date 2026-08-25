// Auto-scaffolded catalog TU for monolib/src/scn/CScnCameraMan
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "libs/monolib/src/scn/CScnCameraMan.hpp"
#include <revolution/MTX.h>
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScnItemCamera.hpp"

CScnCameraMan::CScnCameraMan() {}

CScnCameraMan::~CScnCameraMan() {
    *(void**)this = (void*)lbl_eu_8056EA40;
    // Release every view this camera manager had registered (vtable+0xB8 with
    // the param block), then free the object when the deleting flag is set.
    for (s16 i = 0; i < mCount; i++) {
        CView* view = CViewRoot::getView(mViews[i]);
        if (view != NULL) {
            CScnCameraParam* param = mParam;
            if (param != NULL) {
                param = (CScnCameraParam*)((u8*)param + 0x54);
            }
            ((CViewVt46*)view)->vfB8(param);
        }
    }
}

// Registers `view` at mViews[idx]: first releases the previously registered
// view (vtable+0xB8 with the param block) when it is still valid and differs
// from the new one, then hands the new view the param block and stores its id.
void func_8049AED4(CScnCameraMan* cam, CView* view, u32 idx) {
    u32 vid = cam->mViews[idx];
    if (vid != 0xFFFFFFFF && vid != view->mWorkID) {
        CView* v = CViewRoot::getView(vid);
        if (v != NULL) {
            CScnCameraParam* param = cam->mParam;
            if (param != NULL) {
                param = (CScnCameraParam*)((u8*)param + 0x54);
            }
            ((CViewVt46*)v)->vfB8(param);
        }
    }
    CScnCameraParam* param = cam->mParam;
    if (param != NULL) {
        param = (CScnCameraParam*)((u8*)param + 0x54);
    }
    view->func_8043DC20((u32)param);
    cam->mViews[idx] = view->mWorkID;
}

// Finds the camera item whose view id matches the current view's work id.
// The scan lives in a same-TU helper (inlined by MWCC): the return-inside-loop
// shape is what makes the inlined body emit retail's bne-continue/b-found
// branch pair instead of a folded single beq (see MWCC_CASES btm_bda_to_acl).
static CScnCameraItem* findCamByViewId(CScnCameraMan* cam, CView* view) {
    CScnCameraParam* p = cam->mParam;
    u8* pool = p->mPool;
    CScnCameraNode* node;
    CScnCameraNode* sentinel = func_8048C698(pool, 4)->sentinel;
    node = sentinel->next;
    while (node != sentinel) {
        CScnCameraItem* item = node->item;
        u32 viewId = cam->mViews[item->mIndex];
        if (viewId == view->mWorkID) {
            return item;
        }
        node = node->next;
    }
    return NULL;
}

// Records the camera item matching the current view as the current camera.
bool func_8049AF80(CScnCameraMan* cam) {
    CView* view = CView::getCurrentView();
    CScnCameraItem* item = findCamByViewId(cam, view);
    if (item == NULL) {
        return false;
    }
    cam->mCamId = item->mIndex;
    return true;
}

// TU-local lookup helper; small enough that MWCC inlines it into callers.
// Returns NULL when no item matches (the caller still dereferences the
// result, mirroring retail).
static CScnCameraItem* findCamById(CScnCameraNode* sentinel, s32 id) {
    for (CScnCameraNode* node = sentinel->next; node != sentinel; node = node->next) {
        CScnCameraItem* item = node->item;
        if (id == item->mIndex) {
            return item;
        }
    }
    return NULL;
}

// Id-first variant used by WorkEvent3: extracting the sentinel from the list
// inside the helper pins the retail register coloring (sentinel=r4, matched
// item carried out in r3).
static CScnCameraItem* findCamById2(s32 id, CScnCameraList* list) {
    CScnCameraNode* node;
    CScnCameraNode* sentinel = list->sentinel;
    node = sentinel->next;
    while (node != sentinel) {
        CScnCameraItem* item = node->item;
        if (id == item->mIndex) {
            return item;
        }
        node = node->next;
    }
    return NULL;
}

// Looks up the camera item whose camera id equals the current camera id and
// refreshes it through its vtable+0x14 slot.
void func_8049B024(CScnCameraMan* cam) {
    s32 id = cam->mCamId;
    if (id < 0) {
        id = cam->mCamId;
    }

    CScnCameraList* list = func_8048C698(cam->mParam->mPool, 4);
    CScnCameraItem* item = findCamById(list->sentinel, id);
    item->vf3();
}

// Returns the CView work thread for mViews[id] when it is currently a CView
// (thread type in the CView range, 0x30..0x35); otherwise walks the camera
// list and repeats the check. Falls back to the current camera id when `id`
// is negative.
CWorkThread* func_8049B0A0(CScnCameraMan* cam, s32 id) {
    if (id < 0) {
        id = (s16)cam->mCamId;
    }

    CScnCameraList* list = func_8048C698(cam->mParam->mPool, 4);
    CScnCameraNode* sentinel = list->sentinel;
    CScnCameraNode* node = sentinel->next;
    while (node != sentinel) {
        u32 viewId = cam->mViews[id];
        if (viewId != 0xFFFFFFFF) {
            CWorkThread* thread = CWorkUtil::getWorkThread(viewId);
            if (thread != NULL) {
                if (thread->mType >= CWorkThread::THREAD_CVIEW &&
                    thread->mType < CWorkThread::THREAD_CVIEW_MAX) {
                    return thread;
                }
                return NULL;
            }
        }
        node = node->next;
    }
    return NULL;
}

// Searches the scene pool's camera list for the item whose camera id equals
// `id` (falling back to the current camera id when `id` is negative).
CScnCameraItem* func_8049B158(CScnCameraMan* cam, s32 id) {
    if (id < 0) {
        id = cam->mCamId;
    }

    CScnCameraList* list = func_8048C698(cam->mParam->mPool, 4);
    CScnCameraNode* sentinel = list->sentinel;
    CScnCameraNode* node = sentinel->next;
    while (node != sentinel) {
        CScnCameraItem* item = node->item;
        if (id == item->mIndex) {
            return item;
        }
        node = node->next;
    }
    return NULL;
}

// Same camera-id lookup as func_8049B158.
CScnCameraItem* func_8049B1CC(CScnCameraMan* cam, s32 id) {
    if (id < 0) {
        id = cam->mCamId;
    }

    CScnCameraList* list = func_8048C698(cam->mParam->mPool, 4);
    CScnCameraNode* node;
    CScnCameraNode* sentinel = list->sentinel;
    node = sentinel->next;
    while (node != sentinel) {
        CScnCameraItem* item = node->item;
        if (id == item->mIndex) {
            return item;
        }
        node = node->next;
    }
    return NULL;
}

// Finds the camera item whose view id (cam->mViews[mIndex]) matches the
// parameter's current view id.
// NOTE (byte-matching): MWCC's allocation here is shape-sensitive - the view
// table read must be a named local compared against param->mViewId in that
// order, and sentinel must be declared before node, or sentinel/node land in
// r6/r7 instead of the retail r5/r6.
CScnCameraItem* func_8049B240(CScnCameraMan* cam, CScnCameraParam* param) {
    CScnCameraParam* p = cam->mParam;
    u8* pool = p->mPool;
    CScnCameraNode* sentinel = func_8048C698(pool, 4)->sentinel;
    CScnCameraNode* node = sentinel->next;
    while (node != sentinel) {
        CScnCameraItem* item = node->item;
        u32 viewId = cam->mViews[item->mIndex];
        if (viewId == param->mViewId) {
            return item;
        }
        node = node->next;
    }
    return NULL;
}

// Same view-id camera lookup as func_8049B240.
// NOTE (byte-matching): residual vs retail is purely MWCC register coloring:
// retail colors node=r5/sentinel=r6 with the mParam load hoisted ahead of the
// callee-saves; every tried declaration/body shape yields either
// sentinel=r5/node=r6 (this form, fewest mismatches) or sentinel=r6/node=r7.
// Instruction sequence, control flow, size and relocs match exactly.
CScnCameraItem* func_8049B2C4(CScnCameraMan* cam, CScnCameraParam* param) {
    CScnCameraParam* p = cam->mParam;
    CScnCameraList* list = func_8048C698(p->mPool, 4);
    CScnCameraNode* node = list->sentinel->next;
    CScnCameraNode* sentinel = list->sentinel;
    while (node != sentinel) {
        CScnCameraItem* item = node->item;
        u32 viewId = cam->mViews[item->mIndex];
        if (viewId == param->mViewId) {
            return item;
        }
        node = node->next;
    }
    return NULL;
}

// Handles the camera-select work event (id 0x15): when the current camera id
// matches a camera item in the pool, runs the script data attached to the event.
// NOTE (byte-matching): routing the scan through findCamById lets MWCC inline
// its return-inside-loop shape, carrying the matched item in the volatile r3
// with the NULL fallback sunk after the loop (retail layout).
bool CScnCameraMan::WorkEvent3(UNKTYPE* r4) {
    CScnCameraEvent* ev = (CScnCameraEvent*)r4;
    if (ev->mId == 0x15) {
        s32 camId = mCamId;
        if (camId < 0) {
            camId = mCamId;
        }

        CScnCameraItem* item = findCamById2(camId, func_8048C698(mParam->mPool, 4));
        if (item != NULL) {
            func_8043A1DC__11CScriptCodeFv(this, (u8*)ev->pData, ev->dataSize);
            return true;
        }
    }
    return false;
}

// Not yet decompiled (retail 0x8049F164): keeps the pool camera selected.
bool CScnCameraMan::WorkEvent1(UNKTYPE* r4, const char* r5) {
    return false;
}

extern u32 lbl_eu_80663A08;
extern "C" void func_8043A70C__11CScriptCodeFv(void* a, void* b);
extern "C" void func_8049B3FC() { func_8043A70C__11CScriptCodeFv((void*)lbl_eu_80663A08, 0); }

// Retail s16->f32 magic double (2^52 + 2^31 = 0x4330000080000000; owned by
// CGXCache.cpp's pool range). Referenced as an extern so this TU emits no
// local .sdata2 (retail shape).
extern const double lbl_eu_8066AB60;

// (f32) casts pool TU-local magic doubles; the union helper pins the retail
// symbol and keeps .sdata2 empty.
static inline f32 s16ToF_ab60(s16 v) {
    union { double d; u32 w[2]; } c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v ^ 0x80000000u;
    return (f32)(c.d - lbl_eu_8066AB60);
}

extern "C" void func_8043A57C__11CScriptCodeFv(void* self);
extern "C" void func_8049B408() { func_8043A57C__11CScriptCodeFv((void*)lbl_eu_80663A08); }

void func_8049B59C(){}

// Builds the camera item's perspective projection matrix from its fov / near /
// far planes. The horizontal fov is widened for 16:9 by scaling the aspect
// (scissor-rect ratio x fixed multiplier) with CDeviceVI::getWidthScale().
void func_8049B764(Mtx44 mtx, CScnItemCamera* item) {
    CGXCacheTail* cache = (CGXCacheTail*)CDeviceGX::getCacheInstance();
    f32 aspect = lbl_eu_8066AB6C * (s16ToF_ab60(cache->mScissorDeltaX) / s16ToF_ab60(cache->mScissorDeltaY));
    C_MTXPerspective(mtx, item->mFovY, aspect * CDeviceVI::getWidthScale(), item->mNearZ, item->mFarZ);
}

void func_8049B834(){}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056EA40-0x8056EAD8 (0x98 = 152B): CScnCameraMan vtable.
namespace CScnCameraManBlob {
extern "C" void* __dt__13CScnCameraManFv();
extern "C" void WorkEvent1__13CScnCameraManFPvPCc();
extern "C" void WorkEvent3__13CScnCameraManFPv();
extern "C" void OnFileEvent__10IWorkEventFP10CEventFile();
extern "C" void WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb();
extern "C" void WorkEvent6__10IWorkEventFv();
extern "C" void WorkEvent7__10IWorkEventFv();
extern "C" void WorkEvent8__10IWorkEventFv();
extern "C" void WorkEvent9__10IWorkEventFv();
extern "C" void WorkEvent10__10IWorkEventFv();
extern "C" void WorkEvent11__10IWorkEventFv();
extern "C" void WorkEvent12__10IWorkEventFv();
extern "C" void WorkEvent13__10IWorkEventFv();
extern "C" void WorkEvent14__10IWorkEventFv();
extern "C" void WorkEvent15__10IWorkEventFv();
extern "C" void WorkEvent16__10IWorkEventFv();
extern "C" void WorkEvent17__10IWorkEventFv();
extern "C" void WorkEvent18__10IWorkEventFv();
extern "C" void WorkEvent19__10IWorkEventFv();
extern "C" void WorkEvent20__10IWorkEventFv();
extern "C" void WorkEvent21__10IWorkEventFv();
extern "C" void WorkEvent22__10IWorkEventFv();
extern "C" void WorkEvent23__10IWorkEventFv();
extern "C" void WorkEvent24__10IWorkEventFv();
extern "C" void WorkEvent25__10IWorkEventFv();
extern "C" void WorkEvent26__10IWorkEventFv();
extern "C" void WorkEvent27__10IWorkEventFv();
extern "C" void WorkEvent28__10IWorkEventFv();
extern "C" void WorkEvent29__10IWorkEventFv();
extern "C" void WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
extern "C" u32 __RTTI__10IWorkEvent;
}
extern "C" u32 lbl_eu_80663A10[2];  // RTTI locator (owned by CScnItemCamera.cpp)
extern "C" u32 lbl_eu_8056EA40[38] = {
    (u32)&lbl_eu_80663A10, 0x00000000,
    (u32)&CScnCameraManBlob::__dt__13CScnCameraManFv,
    (u32)&CScnCameraManBlob::WorkEvent1__13CScnCameraManFPvPCc,
    (u32)&CScnCameraManBlob::OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&CScnCameraManBlob::WorkEvent3__13CScnCameraManFPv,
    (u32)&CScnCameraManBlob::WorkEvent4__10IWorkEventFv,
    (u32)&CScnCameraManBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&CScnCameraManBlob::WorkEvent6__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent7__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent8__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent9__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent10__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent11__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent12__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent13__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent14__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent15__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent16__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent17__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent18__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent19__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent20__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent21__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent22__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent23__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent24__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent25__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent26__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent27__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent28__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent29__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent30__10IWorkEventFv,
    (u32)&CScnCameraManBlob::WorkEvent31__10IWorkEventFv,
    (u32)&CScnCameraManBlob::__RTTI__10IWorkEvent,
    0x00000000, 0x00000000, 0x00000000,
};

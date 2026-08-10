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

// Finds the camera item whose view id (cam->mViews[mIndex]) matches the current
// view's work id; records it as the current camera and returns success.
bool func_8049AF80(CScnCameraMan* cam) {
    CView* view = CView::getCurrentView();
    CScnCameraList* list = func_8048C698(cam->mParam->mPool, 4);
    CScnCameraNode* sentinel = list->sentinel;
    CScnCameraNode* node = sentinel->next;
    CScnCameraItem* item;
    while (node != sentinel) {
        item = node->item;
        if (cam->mViews[item->mIndex] == view->mWorkID) {
            break;
        }
        node = node->next;
    }
    if (item != NULL) {
        cam->mCamId = item->mIndex;
        return true;
    }
    return false;
}

// Looks up the camera item whose camera id equals the current camera id and
// refreshes it through its vtable+0x14 slot.
void func_8049B024(CScnCameraMan* cam) {
    s32 id = cam->mCamId;
    if (id < 0) {
        id = cam->mCamId;
    }

    CScnCameraList* list = func_8048C698(cam->mParam->mPool, 4);
    CScnCameraNode* sentinel = list->sentinel;
    CScnCameraNode* node;
    CScnCameraItem* item = NULL;
    for (node = sentinel->next; node != sentinel; node = node->next) {
        item = node->item;
        if (id == item->mIndex) {
            break;
        }
    }
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
CScnCameraItem* func_8049B240(CScnCameraMan* cam, CScnCameraParam* param) {
    CScnCameraList* list = func_8048C698(cam->mParam->mPool, 4);
    CScnCameraNode* sentinel = list->sentinel;
    CScnCameraNode* node = sentinel->next;
    while (node != sentinel) {
        CScnCameraItem* item = node->item;
        if (cam->mViews[item->mIndex] == param->mViewId) {
            return item;
        }
        node = node->next;
    }
    return NULL;
}

// Same view-id camera lookup as func_8049B240.
CScnCameraItem* func_8049B2C4(CScnCameraMan* cam, CScnCameraParam* param) {
    CScnCameraParam* p = cam->mParam;
    CScnCameraList* list = func_8048C698(p->mPool, 4);
    CScnCameraNode* node = list->sentinel->next;
    CScnCameraNode* sentinel = list->sentinel;
    while (node != sentinel) {
        CScnCameraItem* item = node->item;
        if (cam->mViews[item->mIndex] == param->mViewId) {
            return item;
        }
        node = node->next;
    }
    return NULL;
}

// Handles the camera-select work event (id 0x15): when the current camera id
// matches a camera item in the pool, runs the script data attached to the event.
bool CScnCameraMan::WorkEvent3(UNKTYPE* r4) {
    CScnCameraEvent* ev = (CScnCameraEvent*)r4;
    if (ev->mId != 0x15) {
        return false;
    }
    s32 camId = mCamId;
    if (camId < 0) {
        camId = mCamId;
    }

    CScnCameraList* list = func_8048C698(mParam->mPool, 4);
    CScnCameraNode* sentinel = list->sentinel;
    CScnCameraNode* node = sentinel->next;
    CScnCameraItem* item;
    while (node != sentinel) {
        item = node->item;
        if (camId == item->mIndex) {
            break;
        }
        node = node->next;
    }
    if (item != NULL) {
        func_8043A1DC__11CScriptCodeFv(this, (u8*)ev->pData, ev->dataSize);
        return true;
    }
    return false;
}

// Not yet decompiled (retail 0x8049F164): keeps the pool camera selected.
bool CScnCameraMan::WorkEvent1(UNKTYPE* r4, const char* r5) {
    return false;
}

void func_8049B3FC(void){}

// Retail s16->f32 magic double (2^52 + 2^31 = 0x4330000080000000). Defined in
// this TU so MWCC's int->float conversion pool can reference the retail label.
const double lbl_eu_8066AB60 = 4503601774854144.0;

extern void func_8043A57C__11CScriptCodeFv();
void func_8049B408(u8* self){ func_8043A57C__11CScriptCodeFv(); }

void func_8049B59C(){}

// Builds the camera item's perspective projection matrix from its fov / near /
// far planes. The horizontal fov is widened for 16:9 by scaling the aspect
// (scissor-rect ratio x fixed multiplier) with CDeviceVI::getWidthScale().
void func_8049B764(Mtx44 mtx, CScnItemCamera* item) {
    CGXCacheTail* cache = (CGXCacheTail*)CDeviceGX::getCacheInstance();
    f32 aspect = lbl_eu_8066AB6C * ((f32)cache->mScissorDeltaX / (f32)cache->mScissorDeltaY);
    C_MTXPerspective(mtx, item->mFovY, aspect * CDeviceVI::getWidthScale(), item->mNearZ, item->mFarZ);
}

void func_8049B834(){}

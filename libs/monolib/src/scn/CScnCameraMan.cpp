// Auto-scaffolded catalog TU for monolib/src/scn/CScnCameraMan
// Replace stubs with high-level C/C++ during decomp.

// Inline-empty ~IWorkEvent so MWCC elides the base-dtor call in the
// CScnCameraMan destructor (retail shape - see IWorkEvent.hpp).
#define IWORK_EVENT_INLINE_DTOR
#include <types.h>
#include "libs/monolib/src/scn/CScnCameraMan.hpp"
#include <revolution/MTX.h>
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScnItemCamera.hpp"

// Installs the retail vtable (novtable class), stores the camera parameter
// block, fills every view slot with the "no view" sentinel, then creates the
// ten default cameras (0..9) in the scene pool.
CScnCameraMan::CScnCameraMan(CScnCameraParam* param) {
    *(void**)this = (void*)lbl_eu_8056EA40;
    mParam = param;
    mCamId = 0;
    // The sentinel is read through a pointer that may alias the view table /
    // counter (retail reloads it after every store and keeps every mCount
    // writeback; direct reads get CSE'd into one load).
    // Sentinel read through a pointer (retail reloads it per slot); the
    // member-counter loop fully unrolls with one mCount writeback per slot.
    // Best-known shape: unrolls x10 like retail; residual vs retail is
    // MWCC post-unroll strength reduction (retail keeps slwi/add addressing
    // and an addi counter chain; every source form folds to displacements
    // plus hoisted constants - cf. KB func_800A26A4 note).
    unsigned short n = 0;
    for (; n < 10; n++) {
        mCount = n;
        mViews[mCount] = lbl_eu_80663A0C;
    }
    mCount = 10;
    for (int i = 0; i < 10; i++) {
        func_8049F9A8((CScnCameraItemHost*)mParam, i);
    }
}

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
    view->enqueueContextMsg((u32)param);
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
// list and repeats the check. Falls back to the current camera id (sign-
// extended from s16, like retail's extsh) when `id` is negative. Every path
// through the getWorkThread call exits the function, so the loop only ever
// iterates on views that are unregistered (0xFFFFFFFF) - this is what lets
// MWCC keep sentinel/node in volatile registers.
// Returns the CView work thread for mViews[id] when it is currently a CView
// (thread type in the CView range); otherwise walks the camera list and
// repeats the check. Falls back to the current camera id (sign-extended from
// s16, like retail's extsh) when `id` is negative.
// NOTE (byte-matching): the second comparison must be COMMUTED (constant on
// the left); writing `type < THREAD_CVIEW_MAX` lets MWCC fuse the pair into
// subi/cmpli, which retail does not do.
CWorkThread* func_8049B0A0(CScnCameraMan* cam, s32 id) {
    if (id < 0) {
        id = (s16)cam->mCamId;
    }

    CScnCameraList* list = func_8048C698(cam->mParam->mPool, 4);
    CScnCameraNode* node;
    CScnCameraNode* sentinel = list->sentinel;
    node = sentinel->next;
    CWorkThread* thread;
    while (node != sentinel) {
        u32 viewId = cam->mViews[id];
        if (viewId != 0xFFFFFFFF) {
            // Every path through the getWorkThread call exits the function,
            // so the loop only iterates on unregistered views.
            thread = CWorkUtil::getWorkThread(viewId);
            if (thread != NULL) {
                int type = thread->mType;
                if (type < CWorkThread::THREAD_CVIEW) {
                    return NULL;
                }
                if (CWorkThread::THREAD_CVIEW_MAX > type) {
                    goto keep;
                }
            }
            return NULL;
        }
        node = node->next;
    }
    return NULL;
keep:
    return thread;
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

// Handles the camera-set work event: `r5` must be the camera-select command
// pointer (lbl_eu_80663A08). Finds the pool camera item matching mCamId, then
// pushes the event's slot values into it: euler rotation (scaled by a shared
// factor and converted via CCamUtil), fov, near/far planes (each followed by
// a depth-range rebuild), position, and distance.
// NOTE (byte-matching): best-known state is 78 mismatches / exact 396B size.
// Known residuals vs retail:
// - payload/item callee-saved colors swapped (retail ev->r29, item->r31;
//   every tried declaration order yields the opposite pair or worse).
// - retail sinks the `return false` tail after the body with a single bne;
//   this shape inlines the false block after the compare instead. A nested
//   single-return guard reproduces retail's far branch but regresses the
//   call/camId scheduling (-8B).
// - retail carries a dead `cmpwi cr0,r30,0` (mCamId vs 0, result unused);
//   no source idiom found that folds to a bare dead compare.
// - reloc names for func_8049EB60/F6D4/F824 are C++-mangled until those
//   definitions adopt their plain retail symbol names.
bool CScnCameraMan::WorkEvent1(UNKTYPE* payload, const char* r5) {
    CEvent1* ev = (CEvent1*)payload;
    if (r5 != (const char*)lbl_eu_80663A08) {
        return false;
    }

    s32 camId = mCamId;

    CScnCameraList* list = func_8048C698(mParam->mPool, 4);
    CScnItemCamera* item = NULL;
    for (CScnCameraNode* node = list->sentinel->next; node != list->sentinel;
         node = node->next) {
        CScnCameraItem* cur = node->item;
        if (camId == cur->mIndex) {
            item = (CScnItemCamera*)cur;
            break;
        }
    }

    if (item == NULL) {
        return false;
    }

    f32 scale = lbl_eu_8066AB48;

    // Slot 2 holds the target euler vector; scale every component, then copy
    // member-wise into the non-const argument (retail uses lfs/stfs).
    ml::CVec3* src = (ml::CVec3*)getPtrParam__7CEvent1Fv(ev, 2);
    ml::CVec3 scaled;
    scaled.x = src->x * scale;
    scaled.y = src->y * scale;
    scaled.z = src->z * scale;

    ml::CVec3 dir;
    dir.x = scaled.x;
    dir.y = scaled.y;
    dir.z = scaled.z;

    ml::CVec3 rot;
    ml::CCamUtil::getXYZ2ZXY(rot, dir);

    CScnItemCameraCamTail* cam = (CScnItemCameraCamTail*)item;
    cam->field_0x60 = *(u32*)&rot.x;
    cam->field_0x64 = *(u32*)&rot.y;
    cam->field_0x68 = *(u32*)&rot.z;

    cam->mFovY = getFloatParam__7CEvent1Fv(ev, 3);
    func_8049EB60(item);

    f32 farZ = getFloatParam__7CEvent1Fv(ev, 5);
    cam->mNearZ = getFloatParam__7CEvent1Fv(ev, 4);
    cam->mFarZ = farZ;
    func_8049EB60(item);

    // Slot 1 holds the new position; slot 6 the camera distance.
    func_8049F6D4(item, (ml::CVec3*)getPtrParam__7CEvent1Fv(ev, 1));
    func_8049F824(item, getFloatParam__7CEvent1Fv(ev, 6));
    return true;
}

extern u32 lbl_eu_80663A08;
extern "C" void func_8043A70C__11CScriptCodeFv(void* a, void* b);
extern "C" void func_8049B3FC() { func_8043A70C__11CScriptCodeFv((void*)lbl_eu_80663A08, 0); }

// Retail s16->f32 magic double (2^52 + 2^31 = 0x4330000080000000; owned by
// CGXCache.cpp's pool range). Plain (f32) casts below expand to the same
// xoris/lfd/fsubs idiom; the cast constant pools anonymously (@6139).
extern const double lbl_eu_8066AB60;

extern "C" void func_8043A57C__11CScriptCodeFv(void* self);
extern "C" void func_8049B408() { func_8043A57C__11CScriptCodeFv((void*)lbl_eu_80663A08); }

// MWCC's built-in (f32)(s16) cast pools the 0x4330000080000000 magic constant
// anonymously; assembling the double by hand keeps the retail reference to
// lbl_eu_8066AB60.

// Projects a world position onto the camera item's screen rectangle: transforms
// `pos` into camera space (view matrix at +0xCC), rejects points behind the
// near plane, divides by z, runs the projected point through the coefficient
// block at +0x150 (three plane-style rows), then rescales into screen space
// using the aspect ratio, the VI width scale, and the s16 screen offsets.
// NOTE (byte-matching): locals are declared so MWCC's reverse-declaration
// slot assignment yields the retail stack layout (v lowest, conversions last).
bool func_8049B59C(ml::CVec3* out, CScnItemCameraProject* item, ml::CVec3* pos) {
    // s16 -> f32 through the 0x43300000/xoris double trick; hand-assembled so
    // the magic constant keeps its retail reference to lbl_eu_8066AB60.
    union {
        double d;
        u32 w[2];
    } convB, convA;
    ml::CVec3 dir;
    ml::CVec3 result;
    ml::CVec3 v;
    out->x = lbl_eu_8066AB50;
    out->y = lbl_eu_8066AB50;
    out->z = lbl_eu_8066AB50;

    PSMTXMultVec(item->mViewMtx, (Vec*)pos, (Vec*)&v);

    // Points in front of the camera have negative z; the body runs when the
    // point is at or beyond the near plane.
    dir.x = v.x;
    dir.y = v.y;
    dir.z = v.z;

    bool ret;
    if (!(v.z >= -item->mNearZ)) {
        return false;
    }
    // Materializing the return flag up front lets MWCC hoist li r3,1 above
    // the getWidthScale call like retail.
    ret = true;

    f32 inv = lbl_eu_8066AB54 / v.z;
    dir.x = v.x * inv;
    dir.y = v.y * inv;
    dir.z = inv;

    // Three plane rows (dot with the z-divided camera-space point);
    // parentheses pin MWCC's multiply/fma association to retail's.
    const f32* m = item->mProj;
    result.z = m[12] + (inv * m[11] + (dir.x * m[9] + dir.y * m[10]));
    result.y = m[8] + (inv * m[7] + (dir.x * m[5] + dir.y * m[6]));
    result.x = m[4] + (inv * m[3] + (dir.x * m[1] + dir.y * m[2]));
    *out = result;

    // Read once up front: MWCC parks the aspect in f31 across getWidthScale.
    f32 aspect = item->mAspect;
    f32 sx = aspect * CDeviceVI::getWidthScale();
    out->x = out->x / sx;

    f32 ny = item->mAspect * out->y;
    out->y = ny;

    convA.w[1] = (u32)(s32)item->mOffsetX ^ 0x80000000;
    convA.w[0] = 0x43300000;
    out->x = lbl_eu_8066AB58 * (f32)(convA.d - lbl_eu_8066AB60) *
             (lbl_eu_8066AB54 - out->x);
    convB.w[1] = (u32)(s32)item->mOffsetY ^ 0x80000000;
    convB.w[0] = 0x43300000;
    out->y = (f32)(convB.d - lbl_eu_8066AB60) * lbl_eu_8066AB58 *
             (lbl_eu_8066AB54 + out->y);
    return ret;
}

// Builds the camera item's perspective projection matrix from its fov / near /
// far planes. The horizontal fov is widened for 16:9 by scaling the aspect
// (scissor-rect ratio x fixed multiplier) with CDeviceVI::getWidthScale().
void func_8049B764(Mtx44 mtx, CScnItemCamera* item) {
    // Volatile reads pin the depth loads at the top of the function (retail
    // parks them in f31/f30 across the width-scale call).
    const CScnItemCameraFrustumVt* vt = (const CScnItemCameraFrustumVt*)item;
    f32 farZ = vt->mFarZ;
    f32 nearZ = vt->mNearZ;

    CGXCacheTail* cache = (CGXCacheTail*)CDeviceGX::getCacheInstance();
    f32 aspect = ((f32)cache->mScissorDeltaX / (f32)cache->mScissorDeltaY) *
                 lbl_eu_8066AB6C;
    C_MTXPerspective(mtx, item->mFovY, aspect * CDeviceVI::getWidthScale(),
                     nearZ, farZ);
}

// Computes a distance-based falloff factor for `pos` relative to camera `item`
// (1 at <= fMin, linearly down to 0 at >= fMax) into *outFall, and the x
// component of the view-space direction to `pos` (normalized, zero-safe) into
// *outDirX.
void func_8049B834(f32* outDirX, f32* outFall, CScnCameraItem* item,
                   ml::CVec3* pos, f32 fMin, f32 fMax) {
    // Difference between the target point and the camera position; magnitude
    // is sign-independent. operator- keeps its temp and the named copy in
    // separate stack slots (retail shape).
    ml::CVec3 diff = *pos - item->mPos;
    f32 dist = PSVECMag((const Vec*)&diff);

    *outFall = lbl_eu_8066AB68;
    if (dist < fMax) {
        if (dist < fMin) {
            *outFall = lbl_eu_8066AB54;
        } else {
            // Linear falloff between the near and far distances.
            *outFall = lbl_eu_8066AB54 - (dist - fMin) / (fMax - fMin);
        }
    }

    ml::CVec3 dir;
    PSMTXMultVec(item->mMtx, (Vec*)pos, (Vec*)&dir);
    f32 sqLen = dir.y * dir.y + dir.x * dir.x + dir.z * dir.z;
    if (sqLen == lbl_eu_8066AB68) {
        dir = ml::CVec3::zero;
    } else {
        PSVECNormalize((Vec*)&dir, (Vec*)&dir);
    }
    *outDirX = dir.x;
}

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

// CDeviceFont - font device work thread.
//
// The device owns a set of CDeviceFontLayer child work threads (one per font
// id). The four lookup wrappers below search the children list for the layer
// bound to a font id and dispatch a method on it. The retail symbol-map
// entries for those wrappers carry decompiler-guessed Fv signatures even
// though the bodies take a font id in r3 and pass the remaining argument
// registers through to the matched layer method, so they are defined as
// extern "C" with the exact retail names (same pattern as
// CDeviceFontLoader.cpp / CDeviceFontLayer.cpp).

#include "libs/monolib/src/device/CDeviceFont.hpp"
#include <decomp.h>

#include <string.h>
#include <wchar.h>

#include <revolution/ENC.h>
#include <revolution/GX.h>
#include <revolution/MTX.h>
#include <nw4r/ut/ut_TextWriterBase.h>

#include "monolib/work/CWorkUtil.hpp"
#include "monolib/core/CFontLayer.hpp"

// ---- Retail data labels (monolibdata2 dissolve) ----
// Foreign RTTI / base-list objects referenced by the CDeviceFont vtable and
// the reslist<IDeviceFontInfo> sub-vtables.
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" u32 __RTTI__11CWorkThread;
extern "C" u32 lbl_eu_806635F0;     // CWorkThread base-list
// Foreign rodata RTTI name strings referenced by the .sdata locators.
extern "C" u32 lbl_eu_80522D68;
extern "C" u32 lbl_eu_80522D7C;
// Foreign IWorkEvent / CWorkThread virtual thunks referenced by the vtable.
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__10IWorkEventFP10CEventFile();
extern "C" void WorkEvent3__10IWorkEventFPv();
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
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl();
// In-TU C++ members referenced by the vtable (declared extern "C" so the
// reloc name is the retail mangled name; resolves at link to the C++ member
// definition with the same symbol string).
extern "C" void __dt__11CDeviceFontFv();
extern "C" void wkUpdate__11CDeviceFontFv();
extern "C" void wkRender__11CDeviceFontFv();
extern "C" bool wkStandbyLogin__11CDeviceFontFv();
extern "C" bool wkStandbyLogout__11CDeviceFontFv();
// In-TU reslist<IDeviceFontInfo> deleting destructors (defined later).
extern "C" void __dt__reslist_IDeviceFontInfo();
extern "C" void __dt___reslist_base_IDeviceFontInfo();

// === .rodata size=0x70 align=8 ===
extern "C" __declspec(align(8)) const char lbl_eu_80522D90[12] = {
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x6F,0x6E,0x74,0x00,
};
extern "C" const char lbl_eu_80522D9C[28] = {
    0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x3C,0x49,0x44,0x65,0x76,0x69,0x63,0x65,0x46,
    0x6F,0x6E,0x74,0x49,0x6E,0x66,0x6F,0x20,0x2A,0x3E,0x00,0x00,
};
extern "C" const char lbl_eu_80522DB8[36] = {
    0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x49,0x44,
    0x65,0x76,0x69,0x63,0x65,0x46,0x6F,0x6E,0x74,0x49,0x6E,0x66,0x6F,0x20,0x2A,0x3E,
    0x00,0x00,0x00,0x00,
};
extern "C" const char lbl_eu_80522DDC[36] = {
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x6F,0x6E,0x74,0x4C,0x61,0x79,0x65,0x72,
    0x00,0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x6F,0x6E,0x74,0x4C,0x6F,0x61,0x64,
    0x65,0x72,0x00,0x00,
};

// === .sdata size=0x20 align=8 ===
// RTTI locators {name-ptr, base-list} + font-path string pointers.
extern "C" const char* lbl_eu_806636F8 = (const char*)&lbl_eu_80522D68;
extern "C" const char* lbl_eu_806636FC = (const char*)&lbl_eu_80522D7C;
extern "C" u32 lbl_eu_8056C700[];  // forward (incomplete): see .data note
extern "C" u32 lbl_eu_8056C728[];  // forward (incomplete): see .data note
extern "C" u32 lbl_eu_80663700[2] = { (u32)&lbl_eu_80522D90, (u32)&lbl_eu_8056C700 };
extern "C" u32 lbl_eu_80663708[2] = { (u32)&lbl_eu_80522D9C, (u32)&lbl_eu_8056C728 };
extern "C" u32 lbl_eu_80663710[2] = { (u32)&lbl_eu_80522DB8, 0x00000000 };

// === .data size=0xE0 align=8 ===
// NOTE: defined AFTER CDeviceFont::CDeviceFont below, via INCOMPLETE-type
// forward declarations here. With complete types visible at the ctor,
// MWCC commons the three label addresses the ctor takes (C660/C71C/C734)
// into one .data section-base group (...data.0 + offsets); retail
// materializes each independently.
extern "C" u32 lbl_eu_8056C660[];
extern "C" u32 lbl_eu_8056C71C[];
extern "C" u32 lbl_eu_8056C734[];

// === .sbss size=0x10 align=8 (zero-fill) ===
CDeviceFont* lbl_eu_80665678 = 0;
u32 lbl_eu_8066567C = 0;
u32 lbl_eu_80665680 = 0;
u32 lbl_eu_80665684 = 0;
DECOMP_FORCEACTIVE(cdffont, lbl_eu_80665678, lbl_eu_8066567C, lbl_eu_80665680, lbl_eu_80665684);

// ---- CDeviceFont constructor (0x80454FB0) ----
CDeviceFont::CDeviceFont(const char* pName, CWorkThread* pParent)
    : CDeviceBase(pName, pParent, 0x200) {
    // The retail class is non-virtual in this TU: store the retail rodata
    // vtable manually (CDeviceFileDvd pattern).
    *(u32**)this = (u32*)lbl_eu_8056C660;
    // mInfoList construction (inlined reslist<IDeviceFontInfo> base + derived
    // ctors: base sets its vtable, then the derived vtable overwrites it).
    mInfoList.m_vtable = lbl_eu_8056C734;
    mInfoList.mList = 0;
    mInfoList.mCapacity = 0;
    mInfoList.field_0x1C = 0;
    mInfoList.mStartNodePtr = &mInfoList.mStartNode;
    mInfoList.mStartNodePtr->mNext = mInfoList.mStartNodePtr;
    mInfoList.mStartNodePtr->mPrev = mInfoList.mStartNodePtr;
    mInfoList.m_vtable = lbl_eu_8056C71C;
    mFontId = 0;
    field_0x1EC = 0;
    lbl_eu_80665678 = this;
    // reserve(handle, 8): allocate the 8-node array and clear each mNext.
    mInfoList.mList = (CDeviceFontInfoListNode*)mtl::MemManager::allocate_array(
        0x60, mAllocHandle);
    for (int i = 0; i < 8; i++) {
        mInfoList.mList[i].mNext = 0;
    }
    mInfoList.mCapacity = 8;
    lbl_eu_8066567C = 0;
}

// === .data definitions (moved below the ctor: see incomplete-decl note) ===
// CDeviceFont vtable (160B): IWorkEvent dispatcher + CWorkThread overrides.
extern "C" u32 lbl_eu_8056C660[40] = {
    (u32)&lbl_eu_80663700, 0x00000000, (u32)&__dt__11CDeviceFontFv, (u32)&WorkEvent1__10IWorkEventFPvPCc,
    (u32)&OnFileEvent__10IWorkEventFP10CEventFile, (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv, (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv, (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv, (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv, (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv, (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv, (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv, (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv, (u32)&wkUpdate__11CDeviceFontFv, (u32)&wkRender__11CDeviceFontFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__11CDeviceFontFv, (u32)&wkStandbyLogout__11CDeviceFontFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
// reslist<IDeviceFontInfo> vtable (28B): {RTTI, base, dtor, ...}.
extern "C" u32 lbl_eu_8056C700[7] = {
    (u32)&__RTTI__10IWorkEvent, 0x00000000, (u32)&__RTTI__11CWorkThread, 0x00000000,
    (u32)&lbl_eu_806635F0, 0x00000000, 0x00000000,
};
// reslist<IDeviceFontInfo> sub-vtable (12B).
extern "C" u32 lbl_eu_8056C71C[3] = {
    (u32)&lbl_eu_80663708, 0x00000000, (u32)&__dt__reslist_IDeviceFontInfo,
};
// _reslist_base<IDeviceFontInfo> sub-vtable (12B).
extern "C" u32 lbl_eu_8056C728[3] = {
    (u32)&lbl_eu_80663710, 0x00000000, 0x00000000,
};
// _reslist_base<IDeviceFontInfo> tail (12B).
extern "C" u32 lbl_eu_8056C734[3] = {
    (u32)&lbl_eu_80663710, 0x00000000, (u32)&__dt___reslist_base_IDeviceFontInfo,
};

// ---------------------------------------------------------------------------
// reslist<IDeviceFontInfo> deleting destructors (retail emits these under the
// old unmangled template names, so they are written as free functions with
// the exact retail identifiers; the item type never needs destroying).
// ---------------------------------------------------------------------------

// _reslist_base<IDeviceFontInfo>::~_reslist_base(int deleting): clear the
// ring (unlink every node by nulling mNext), reset the sentinel, free the
// node array unless externally owned, then free the object when deleting > 0.
CDeviceFontInfoReslist* __dt___reslist_base_IDeviceFontInfo(
    CDeviceFontInfoReslist* t, int deleting) {
    if (t != 0) {
        t->m_vtable = lbl_eu_8056C734;
        CDeviceFontInfoListNode* node = t->mStartNodePtr->mNext;
        while (node != t->mStartNodePtr) {
            CDeviceFontInfoListNode* prev = node;
            node = node->mNext;
            prev->mNext = 0;
        }
        t->mStartNodePtr->mNext = t->mStartNodePtr;
        t->mStartNodePtr->mPrev = t->mStartNodePtr;
        if (t->field_0x1C == 0) {
            if (t->mList != 0) {
                delete[] t->mList;
                t->mList = 0;
            }
        }
        if (deleting > 0) {
            delete t;
        }
    }
    return t;
}

// reslist<IDeviceFontInfo>::~reslist(int deleting): the base dtor body is
// inlined into the derived dtor; retail keeps both null checks.
CDeviceFontInfoReslist* __dt__reslist_IDeviceFontInfo(
    CDeviceFontInfoReslist* t, int deleting) {
    if (t != 0) {
        if (t != 0) {
            t->m_vtable = lbl_eu_8056C734;
            CDeviceFontInfoListNode* node = t->mStartNodePtr->mNext;
            while (node != t->mStartNodePtr) {
                CDeviceFontInfoListNode* prev = node;
                node = node->mNext;
                prev->mNext = 0;
            }
            t->mStartNodePtr->mNext = t->mStartNodePtr;
            t->mStartNodePtr->mPrev = t->mStartNodePtr;
            if (t->field_0x1C == 0) {
                if (t->mList != 0) {
                    delete[] t->mList;
                    t->mList = 0;
                }
            }
        }
        if (deleting > 0) {
            delete t;
        }
    }
    return t;
}

// ---- CDeviceFont destructor (0x80455204) ----
CDeviceFont::~CDeviceFont() {
    // Destroy the font-info list: the inlined reslist<IDeviceFontInfo> dtor
    // null-checks twice (derived reslist dtor, then base), clears the ring
    // and frees the node array unless externally owned. The CWorkThread base
    // dtor call and the deleting-flag delete are emitted implicitly.
    CDeviceFontInfoReslist* list = &mInfoList;
    lbl_eu_80665678 = 0;
    if (list != 0) {
        if (list != 0) {
            list->m_vtable = lbl_eu_8056C734;
            CDeviceFontInfoListNode* node = list->mStartNodePtr->mNext;
            while (node != list->mStartNodePtr) {
                CDeviceFontInfoListNode* prev = node;
                node = node->mNext;
                prev->mNext = 0;
            }
            list->mStartNodePtr->mNext = list->mStartNodePtr;
            list->mStartNodePtr->mPrev = list->mStartNodePtr;
            if (list->field_0x1C == 0) {
                if (list->mList != 0) {
                    delete[] list->mList;
                    list->mList = 0;
                }
            }
        }
    }
}

CDeviceFont* CDeviceFont::getInstance() {
    return lbl_eu_80665678;
}

// Getter for the device's current font id (used as the default lookup key by
// the layer-search wrappers below).
u32 CDeviceFont::func_804525D4() {
    CDeviceFont* font = lbl_eu_80665678;
    if (font == 0) {
        return 0;
    }
    return font->mFontId;
}

extern "C" {

// Tail-call targets of the lookup wrappers: retail relocs to the Fv-mangled
// names even though the calls pass trailing arguments through in r4+.
void func_80453BB4__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 a1,
                                         u32 a2, u32 a3);
void func_80453FF0__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 a1);
void func_804541F8__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 a1);
void func_8045438C__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 a1);

// Find the font layer for `fontId` (0 selects the device's current font id)
// and tail-call the layer's func_80453BB4 with the passed-through args.
// Loop locals declared first (see func_804529D4): MWCC allocates in
// declaration order, and retail keeps child/candidate in r7, the walk node
// in r8 and the start sentinel in r9 (r4-r6 hold the passed-through args).
void func_8045271C__11CDeviceFontFv(u32 fontId, u32 a1, u32 a2, u32 a3) {
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CDeviceFontLayer*>* node;
    _reslist_node<CDeviceFontLayer*>* startNode;

    if (lbl_eu_80665678 == 0) return;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    // Volatile re-read: retail reloads the singleton before the children
    // walk; the volatile deref stops MWCC from CSE-ing it into the
    // null-check load above.
    CDeviceFont* font = *(CDeviceFont* volatile*)&lbl_eu_80665678;

    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = node->mItem;
        // candidate starts as `child`; zero it for null children and for
        // layers whose thread type is not a font layer.
        candidate = child;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType != CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == fontId) {
            layer = candidate;
            goto found;
        }
        node = node->mNext;
    }
    layer = 0;
found:
    if (layer == 0) return;
    func_80453BB4__16CDeviceFontLayerFv(layer, a1, a2, a3);
}

// Same lookup as func_8045271C, dispatching to the layer's func_80453FF0.
// The single trailing argument rides along to the layer method in r4 (which
// is why the walk colors child/candidate r5, node r6, sentinel r7).
void func_8045283C__11CDeviceFontFv(u32 fontId, u32 a1) {
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CDeviceFontLayer*>* node;
    _reslist_node<CDeviceFontLayer*>* startNode;

    if (lbl_eu_80665678 == 0) return;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    // Volatile re-read (see func_8045271C); separate variables keep the
    // null-check load out of the walk pointer's register.
    CDeviceFont* font = *(CDeviceFont* volatile*)&lbl_eu_80665678;

    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = node->mItem;
        // candidate starts as `child`; zero it for null children and for
        // layers whose thread type is not a font layer.
        candidate = child;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType != CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == fontId) {
            layer = candidate;
            goto found;
        }
        node = node->mNext;
    }
    layer = 0;
found:
    if (layer == 0) return;
    func_80453FF0__16CDeviceFontLayerFv(layer, a1);
}

// Same lookup as func_8045271C, dispatching to the layer's func_804541F8
// (no trailing arguments: retail colors child/candidate r4, node r5,
// sentinel r6).
void func_804528C4__11CDeviceFontFv(u32 fontId) {
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CDeviceFontLayer*>* node;
    _reslist_node<CDeviceFontLayer*>* startNode;

    if (lbl_eu_80665678 == 0) return;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    // Volatile re-read (see func_8045271C).
    CDeviceFont* font = *(CDeviceFont* volatile*)&lbl_eu_80665678;

    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = node->mItem;
        // candidate starts as `child`; zero it for null children and for
        // layers whose thread type is not a font layer.
        candidate = child;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType != CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == fontId) {
            layer = candidate;
            goto found;
        }
        node = node->mNext;
    }
    layer = 0;
found:
    if (layer == 0) return;
    layer->func_804541F8();
}

// Same lookup as func_8045271C, dispatching to the layer's func_8045438C
// with one trailing argument passed through in r4.
void func_8045294C__11CDeviceFontFv(u32 fontId, u32 a1) {
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CDeviceFontLayer*>* node;
    _reslist_node<CDeviceFontLayer*>* startNode;

    if (lbl_eu_80665678 == 0) return;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    // Volatile re-read (see func_8045271C).
    CDeviceFont* font = *(CDeviceFont* volatile*)&lbl_eu_80665678;

    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = node->mItem;
        // candidate starts as `child`; zero it for null children and for
        // layers whose thread type is not a font layer.
        candidate = child;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType != CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == fontId) {
            layer = candidate;
            goto found;
        }
        node = node->mNext;
    }
    layer = 0;
found:
    if (layer == 0) return;
    func_8045438C__16CDeviceFontLayerFv(layer, a1);
}

// ---------------------------------------------------------------------------
// Layer-method imports for the dispatch wrappers below. The retail symbol-map
// entries carry decompiler-guessed Fv suffixes even though the real
// signatures take arguments, so the calls reference the literal retail names
// (same arrangement as the layer unit's own extern "C" definitions).
// ---------------------------------------------------------------------------
void func_80454508__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 a1);
void func_80454B70__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 flag);
u32 func_80453F78__16CDeviceFontLayerFv(CDeviceFontLayer* layer);
u32 func_80454684__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 index);

// The retail layer ctor is emitted under an unmangled (C-linkage) name, so
// it cannot be expressed as a C++ member ctor; declare it extern "C" and
// call it directly after the raw allocation (placement-new style). The
// parent is the device (a CWorkThread at runtime).
extern "C" CDeviceFontLayer* __ct__CDeviceFontLayer(CDeviceFontLayer* self,
                                                    const char* name,
                                                    CDeviceFont* parent);

// The retail CFontLayer ctor is emitted under an unmangled (C-linkage) name,
// so it cannot be expressed as a C++ member ctor; declare it extern "C" and
// call it directly after the raw 4-byte allocation (placement-new style).
extern "C" CFontLayer* __ct__CFontLayer(CFontLayer* self);

// Same lookup as func_8045271C, dispatching to the layer's func_80454508.
void func_804529D4__11CDeviceFontFv(u32 fontId, u32 a1) {
    if (lbl_eu_80665678 == 0) return;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    // Re-read the singleton through a volatile view: retail reloads the
    // pointer before the children walk (other work threads may replace the
    // device), and the volatile deref stops MWCC from CSE-ing it into the
    // null-check load above.
    CDeviceFont* font = *(CDeviceFont* volatile*)&lbl_eu_80665678;

    // Declare loop locals first: MWCC assigns registers in declaration
    // order, and retail keeps the child/layer value in the lowest register
    // (r5), the walk node in r6 and the start sentinel in r7.
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CDeviceFontLayer*>* node;
    _reslist_node<CDeviceFontLayer*>* startNode;
    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = node->mItem;
        // candidate starts as `child`; zero it for null children and for
        // layers whose thread type is not a font layer.
        candidate = child;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType != CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == fontId) {
            layer = candidate;
            goto found;
        }
        node = node->mNext;
    }
    layer = 0;
found:
    if (layer == 0) return;
    func_80454508__16CDeviceFontLayerFv(layer, a1);
}

// Same lookup as func_8045271C, dispatching to the layer's func_80454B70
// with the passed-through flag.
void func_80452CF8__11CDeviceFontFv(u32 fontId, u32 flag) {
    if (lbl_eu_80665678 == 0) return;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    CDeviceFont* font = *(CDeviceFont* volatile*)&lbl_eu_80665678;

    // Declare loop locals first: MWCC assigns registers in declaration
    // order, and retail keeps the child/layer value in the lowest register
    // (r5), the walk node in r6 and the start sentinel in r7.
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CDeviceFontLayer*>* node;
    _reslist_node<CDeviceFontLayer*>* startNode;
    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = node->mItem;
        // candidate starts as `child`; zero it for null children and for
        // layers whose thread type is not a font layer.
        candidate = child;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType != CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == fontId) {
            layer = candidate;
            goto found;
        }
        node = node->mNext;
    }
    layer = 0;
found:
    if (layer == 0) return;
    func_80454B70__16CDeviceFontLayerFv(layer, flag);
}

// Same lookup as func_8045271C, but without the fontId-defaulting or the
// singleton reload: mark the bound layer's 0x2F0 flag and report success.
// (Called from CFontLayer's destructor with the object pointer as the key.)
u32 func_80452690__11CDeviceFontFv(u32 key) {
    CDeviceFont* font = lbl_eu_80665678;
    if (font == 0) return 0;

    // Declare loop locals first (see func_804529D4): retail keeps the
    // child/layer value in r4, the walk node in r5, the sentinel in r6.
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CDeviceFontLayer*>* node;
    _reslist_node<CDeviceFontLayer*>* startNode;
    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = node->mItem;
        // candidate starts as `child`; zero it for null children and for
        // layers whose thread type is not a font layer.
        candidate = child;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType != CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == key) {
            layer = candidate;
            goto found;
        }
        node = node->mNext;
    }
    layer = 0;
found:
    if (layer == 0) return 0;
    layer->mFlag2F0 = 1;
    return 1;
}

// Same lookup as func_8045271C, tail-calling the layer's func_80453F78.
u32 func_804527A4__11CDeviceFontFv(u32 fontId) {
    if (lbl_eu_80665678 == 0) return 0;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    CDeviceFont* font = *(CDeviceFont* volatile*)&lbl_eu_80665678;

    // Declare loop locals first (see func_804529D4).
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CDeviceFontLayer*>* node;
    _reslist_node<CDeviceFontLayer*>* startNode;
    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = node->mItem;
        // candidate starts as `child`; zero it for null children and for
        // layers whose thread type is not a font layer.
        candidate = child;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType != CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == fontId) {
            layer = candidate;
            goto found;
        }
        node = node->mNext;
    }
    layer = 0;
found:
    if (layer == 0) return 0;
    return func_80453F78__16CDeviceFontLayerFv(layer);
}

// Same lookup as func_8045271C, tail-calling the layer's func_80454684 with
// the passed-through index and returning its result.
u32 func_80452B78__11CDeviceFontFv(u32 fontId, u32 index) {
    if (lbl_eu_80665678 == 0) return 0;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    CDeviceFont* font = *(CDeviceFont* volatile*)&lbl_eu_80665678;

    // Declare loop locals first (see func_804529D4).
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CDeviceFontLayer*>* node;
    _reslist_node<CDeviceFontLayer*>* startNode;
    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = node->mItem;
        // candidate starts as `child`; zero it for null children and for
        // layers whose thread type is not a font layer.
        candidate = child;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType != CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == fontId) {
            layer = candidate;
            goto found;
        }
        node = node->mNext;
    }
    layer = 0;
found:
    if (layer == 0) return 0;
    return func_80454684__16CDeviceFontLayerFv(layer, index);
}

}

// Create and register a font-layer work thread under this device: allocate
// the layer from work memory, construct it with the shared layer name, link
// it into the device's children list and record the owning device back on
// the layer. Returns 1 on success, 0 when the device is not created yet.
u32 CDeviceFont::func_804525F0() {
    CDeviceFont* font = lbl_eu_80665678;
    if (font == 0) return 0;

    const char* name = lbl_eu_80522DDC;
    CDeviceFontLayer* layer = (CDeviceFontLayer*)mtl::MemManager::allocate(
        0x2f8, CWorkThreadSystem::getWorkMem());
    if (layer != 0) {
        layer = __ct__CDeviceFontLayer(layer, name, font);
    }
    CWorkUtil::entryWork((CWorkThread*)layer, (CWorkThread*)font, false);
    layer->field_0x1F8 = (u32)this;
    return 1;
}

// ---- func_80452C10 (0x8045580C) ----
// Find the font-info provider for this device: first pass looks for the info
// whose identity query (0xC slot) matches this device and that is available
// (0x34 slot returns 0); second pass falls back to the first available info
// regardless of identity. Returns the matching provider or null. (The fontId
// and layout parameters are unused in the body, as in retail.)
IDeviceFontInfo* CDeviceFont::func_80452C10(u32 fontId,
                                            nw4r::lyt::Layout* layout) {
    CDeviceFont* font = lbl_eu_80665678;
    if (font == 0) return 0;

    CDeviceFontInfoListNode* node = font->mInfoList.mStartNodePtr->mNext;
    while (node != lbl_eu_80665678->mInfoList.mStartNodePtr) {
        if (node->mItem->func_804535F4() == 0 &&
            node->mItem->func_80453654() == (s32)this) {
            return node->mItem;
        }
        node = node->mNext;
    }
    node = lbl_eu_80665678->mInfoList.mStartNodePtr->mNext;
    while (node != lbl_eu_80665678->mInfoList.mStartNodePtr) {
        if (node->mItem->func_804535F4() == 0) {
            return node->mItem;
        }
        node = node->mNext;
    }
    return 0;
}

// Walk the font-info list looking for the provider whose identity query
// (0xC slot) matches this device (and whose 0x34-slot availability check
// returned 0). Returns the matching provider or null. The item pointer is
// re-read from the node at every use (retail reloads node->mItem per vtable
// call).
IDeviceFontInfo* CDeviceFont::func_eu_804558F4() {
    CDeviceFont* font = lbl_eu_80665678;
    if (font == 0) return 0;

    CDeviceFontInfoListNode* node = font->mInfoList.mStartNodePtr->mNext;
    while (node != lbl_eu_80665678->mInfoList.mStartNodePtr) {
        if (node->mItem->func_804535F4() == 0 &&
            node->mItem->func_80453654() == (s32)this) {
            return node->mItem;
        }
        node = node->mNext;
    }
    return 0;
}

// Create a font-info provider, push it onto the device's info list (first
// free slot; setItem is guarded by the retail null check + try/catch frame
// marker), and flag the list dirty (field_0x1EC = 2).
u32 CDeviceFont::func_80452D80() {
    if (lbl_eu_80665678 == 0) return 0;

    CDeviceFontInfoExt* info = CDeviceFontInfoExt::create();

    CDeviceFont* font = lbl_eu_80665678;
    // Locals in retail allocation order: slot index, sentinel pointer,
    // capacity (signed: retail emits a signed cmpw).
    // Locals in retail order: slot index, sentinel pointer, capacity
    // (signed: retail emits a signed cmpw).
    int i = 0;
    CDeviceFontInfoListNode* sentinel = font->mInfoList.mStartNodePtr;
    int capacity = font->mInfoList.mCapacity;
    for (; i < capacity; i++) {
        if (font->mInfoList.mList[i].mNext == 0) break;
    }
    CDeviceFontInfoListNode* temp = &font->mInfoList.mList[i];
    {
        IDeviceFontInfo** itemPtr = &temp->mItem;
        if (itemPtr != 0) {
            try {
                *itemPtr = info;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = sentinel;
    temp->mPrev = sentinel->mPrev;
    sentinel->mPrev->mNext = temp;
    sentinel->mPrev = temp;
    // retail re-reads the singleton here (fresh sda21 reloc)
    lbl_eu_80665678->field_0x1EC = 2;
    return 1;
}

// ---- wkUpdate (0x80455AE8) ----
void CDeviceFont::wkUpdate() {
    // Locals declared in allocation order: retail keeps the walk node in r29
    // and the loader name in r30 (this in r31).
    const char* name;
    CDeviceFontInfoListNode* node;

    // First pass: run the per-frame update hook (vtable 0x2C) on every
    // font-info provider still in the list.
    node = mInfoList.mStartNodePtr->mNext;
    // Volatile sentinel reads: retail reloads mStartNodePtr every iteration
    // (the loop bodies call through vtables, so the list may change); MWCC
    // otherwise keeps the sentinel in a callee-saved register across the
    // second loop, dropping the reload (one instruction short of retail).
    while (node != *(CDeviceFontInfoListNode* volatile*)&mInfoList.mStartNodePtr) {
        node->mItem->func_804535C0();
        node = node->mNext;
    }

    // Second pass: drop the first provider whose 0x38 slot reports done.
    // The `if (item != 0) delete item;` guard is what produces retail's
    // double null-check around the virtual dtor call.
    node = *(CDeviceFontInfoListNode* volatile*)&mInfoList.mStartNodePtr->mNext;
    while (node != *(CDeviceFontInfoListNode* volatile*)&mInfoList.mStartNodePtr) {
        if (node->mItem->func_80453608() != 0) {
            IDeviceFontInfo* item = node->mItem;
            if (item != 0) {
                delete item;
            }
            node->mItem = 0;
            CDeviceFontInfoListNode* prev = node->mPrev;
            CDeviceFontInfoListNode* next = node->mNext;
            prev->mNext = next;
            next->mPrev = prev;
            node->mNext = 0;
            break;
        }
        node = node->mNext;
    }

    switch (field_0x1EC) {
    case 0:
        // Not started: once the file device is up, spin up the font-loader
        // work thread ("FontLoader", the shared name + 0x11) and give it the
        // MenuFont path (JP) or the EU font path, then mark the state 1.
        if (CDeviceFile::getInstance() != 0) {
            if (CDeviceFile::isInitialized()) {
                // (retail keeps base in r3 and folds the offset into r30
                // before the getWorkMem call).
                name = &lbl_eu_80522DDC[0x11];
                CDeviceFontLoader* loader =
                    (CDeviceFontLoader*)mtl::MemManager::allocate(
                        0x210, CWorkThreadSystem::getWorkMem());
                if (loader != 0) {
                    loader = (CDeviceFontLoader*)__ct__CDeviceFontLoader(
                        loader, name, this);
                }
                CWorkUtil::entryWork((CWorkThread*)loader, (CWorkThread*)this,
                                     false);
                if (func_eu_8044A600()) {
                    func_80454F30__17CDeviceFontLoaderFv(loader, (void*)1,
                                                          lbl_eu_806636FC);
                } else {
                    func_80454F30__17CDeviceFontLoaderFv(loader, (void*)1,
                                                          lbl_eu_806636F8);
                }
                field_0x1EC = 1;
            }
        }
        break;
    case 2:
        // Info-list handoff pending: once the exception clears, hand the
        // list to the first provider that accepts it (0xC slot returns 0)
        // and mark the state 3.
        if (CWorkRoot::getException() == 0) {
            CDeviceFontInfoListNode* infoNode =
                lbl_eu_80665678->mInfoList.mStartNodePtr->mNext;
            while (infoNode != lbl_eu_80665678->mInfoList.mStartNodePtr) {
                if (infoNode->mItem->func_80453654() == 0) {
                    infoNode->mItem->func_804535DC();
                    break;
                }
                infoNode = infoNode->mNext;
            }
            field_0x1EC = 3;
        }
        break;
    }
}

CDeviceFontInfo::~CDeviceFontInfo() {}

// Pointer-validation macros mirroring the nw4r db asserts inlined by retail
// (same shapes as src/kyoshin/code_8025FB10.cpp).
#define VALIDATE_NW4R_POINTER(pointer, file, line, message)                    \
    {                                                                         \
        bool validMem1 = true;                                                \
        bool validMem2 = true;                                                \
        bool validIo = true;                                                  \
        bool validIo2 = true;                                                 \
        bool validRegs = true;                                                \
        bool validRegs2 = true;                                               \
        u32 address = (u32)(pointer);                                         \
        if ((address & 0xFF000000) != 0x80000000 &&                           \
            (address & 0xFF800000) != 0x81000000) {                           \
            validMem1 = false;                                                \
        }                                                                     \
        if (!validMem1 && (address & 0xF8000000) != 0x90000000) {             \
            validMem2 = false;                                                \
        }                                                                     \
        if (!validMem2 && (address & 0xFF000000) != 0xC0000000) {             \
            validIo = false;                                                  \
        }                                                                     \
        if (!validIo && (address & 0xFF800000) != 0xC1000000) {               \
            validIo2 = false;                                                 \
        }                                                                     \
        if (!validIo2 && (address & 0xF8000000) != 0xD0000000) {              \
            validRegs = false;                                                \
        }                                                                     \
        if (!validRegs && (address & 0xFFFFC000) != 0xE0000000) {             \
            validRegs2 = false;                                               \
        }                                                                     \
        if (!validRegs2) {                                                    \
            nw4r::db::Panic(file, line, message, pointer);                    \
        }                                                                     \
    }

#define VALIDATE_NW4R_POINTER_FLAG(pointer, region, file, line, message)       \
    {                                                                         \
        bool valid = false;                                                   \
        if (region == 0x80000000 ||                                           \
            ((u32)(pointer) & 0xFF800000) == 0x81000000 ||                     \
            ((u32)(pointer) & 0xF8000000) == 0x90000000 ||                     \
            region == 0xC0000000 ||                                           \
            ((u32)(pointer) & 0xFF800000) == 0xC1000000 ||                     \
            ((u32)(pointer) & 0xF8000000) == 0xD0000000 ||                     \
            ((u32)(pointer) & 0xFFFFC000) == 0xE0000000) {                     \
            valid = true;                                                     \
        }                                                                     \
        if (!valid) {                                                         \
            nw4r::db::Panic(file, line, message, pointer);                    \
        }                                                                     \
    }

typedef nw4r::ut::TextWriterBase<wchar_t> WideTextWriter;
extern "C" void* getInstance__8CDesktopFv();

// ---- wkRender (0x80455D2C): draws the exception-layer text ---------------
// While this device owns the work-root exception, fade in a centered UTF-16
// string over a shadowed quad (4 offset black passes + alpha-scaled gray
// center pass) using an orthographic full-screen projection.
void CDeviceFont::wkRender() {
    // Exception handoff: while the desktop is up and we own the work-root
    // exception, count frames; release the exception once the counter wraps
    // negative (retail: addic./ble after the increment).
    if (getInstance__8CDesktopFv() != 0) {
        if (CWorkRoot::getException() == (CException*)this) {
            if (++lbl_eu_8066567C <= 0) {
                CWorkRoot::setException(0);
            }
        }
    }

    if (lbl_eu_80665678 == 0) {
        return;
    }

    CDeviceVI::setFlag0(false);

    // Frame counter cycling at 2000 frames.
    lbl_eu_80665684++;
    if ((s32)lbl_eu_80665684 >= 2000) {
        lbl_eu_80665684 = 0;
    }

    // Fade-in alpha ramp over the first 1000 counted frames. Natural
    // int->double cast: the compiler generates the 0x43300000 magic-double
    // sequence itself (matching retail) instead of a manual union build.
    f32 fade = lbl_eu_8066A3D0 *
                   static_cast<f32>(static_cast<f64>(
                       static_cast<s32>(lbl_eu_80665684 - 1000)) /
                                   lbl_eu_8066A3D4) +
               lbl_eu_8066A3CC;
    if (fade < lbl_eu_8066A3D8) {
        fade = lbl_eu_8066A3CC - fade;
    }
    if (fade >= lbl_eu_8066A3DC) {
        if (fade > lbl_eu_8066A3CC) {
            fade = lbl_eu_8066A3CC;
        }
    } else {
        fade = lbl_eu_8066A3DC;
    }

    // Convert the shared UTF-8 source into the UTF-16 render buffer.
    u32 destLen = 0x200;
    memset(lbl_eu_80657750, 0, 0x400);
    // lbl_eu_80665680 is the shared UTF-8 source text pointer (retail passes
    // it as the converter's src argument, not a gate flag).
    if (ENCConvertStringUtf8ToUtf16((u16*)lbl_eu_80657750, &destLen,
                                    (const u8*)lbl_eu_80665680,
                                    (u32*)NULL) != ENC_OK) {
        return;
    }

    WideTextWriter writer;
    writer.SetupGX();

    u32 writerRegion = (u32)&writer & 0xFF000000;

    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DC70, 139, lbl_eu_8052DC3C);
    writer.SetDrawFlag(0x110);

    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DD84, 171, lbl_eu_8052DD50);
    writer.SetScale(lbl_eu_8066A3C8, lbl_eu_8066A3E0);

    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DC28, 98, lbl_eu_8052DBF4);
    writer.SetCharSpace(lbl_eu_8066A3E4);

    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DD84, 171, lbl_eu_8052DD50);
    writer.SetCharSpace(lbl_eu_8066A3C8);

    // Fetch the bound font twice like retail (null check + use).
    nw4r::lyt::Layout* layout;  // unused second argument, uninitialized in retail
    if (func_80452C10(0, layout)->func_80453624() == NULL) {
        return;
    }
    const nw4r::ut::Font* font = static_cast<const nw4r::ut::Font*>(
        func_80452C10(0, layout)->func_80453624());

    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8053785C, 65, lbl_eu_80537828);
    VALIDATE_NW4R_POINTER(font, lbl_eu_80537818, 66, lbl_eu_805377E0);
    writer.SetFont(*font);

    GXRenderModeObj* rmo;
    f32 scaledW;
    f32 scaledH;

    rmo = CDeviceVI::getRenderModeObj();
    scaledW =
        lbl_eu_8066A3F0 * static_cast<f32>(static_cast<f64>(rmo->fbWidth) -
                                           lbl_eu_8066A3C0);
    rmo = CDeviceVI::getRenderModeObj();
    scaledH =
        lbl_eu_8066A3F0 * static_cast<f32>(static_cast<f64>(rmo->efbHeight) -
                                           lbl_eu_8066A3C0);

    writer.SetupGX();

    // One shared matrix buffer: identity position matrix, then overwritten
    // with the orthographic projection.
    Mtx44 mtx;
    PSMTXIdentity(mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    f32 width;
    rmo = CDeviceVI::getRenderModeObj();
    width =
        static_cast<f32>(static_cast<f64>(rmo->fbWidth) - lbl_eu_8066A3C0);
    rmo = CDeviceVI::getRenderModeObj();
    C_MTXOrtho(mtx, lbl_eu_8066A3D8,
               static_cast<f32>(static_cast<f64>(rmo->efbHeight) -
                                lbl_eu_8066A3C0),
               lbl_eu_8066A3D8, width, lbl_eu_8066A3D8, lbl_eu_8066A3F4);
    GXSetProjection(mtx, GX_ORTHOGRAPHIC);

    // Shadow pass: four black copies around the final position.
    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DCFC, 135, lbl_eu_8052DCC8);
    writer.SetTextColor(nw4r::ut::Color(0, 0, 0, 255));

    const wchar_t* text = lbl_eu_80657750;
    u32 textRegion = (u32)text & 0xFF000000;

    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_805378A0, 258,
                               lbl_eu_8053786C);
    writer.SetCursor(scaledW - lbl_eu_8066A3C8, scaledH - lbl_eu_8066A3C8,
                     lbl_eu_8066A3F8);
    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_80537734, 256,
                               lbl_eu_80537700);
    VALIDATE_NW4R_POINTER(text, lbl_eu_805376EC, 257, lbl_eu_805376B8);
    writer.Print(text, static_cast<int>(wcslen(text)));

    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_805378A0, 258,
                               lbl_eu_8053786C);
    writer.SetCursor(scaledW + lbl_eu_8066A3C8, scaledH - lbl_eu_8066A3C8,
                     lbl_eu_8066A3F8);
    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_80537734, 256,
                               lbl_eu_80537700);
    VALIDATE_NW4R_POINTER_FLAG(text, textRegion, lbl_eu_805376EC, 257,
                               lbl_eu_805376B8);
    writer.Print(text, static_cast<int>(wcslen(text)));

    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_805378A0, 258,
                               lbl_eu_8053786C);
    writer.SetCursor(scaledW - lbl_eu_8066A3C8, scaledH + lbl_eu_8066A3C8,
                     lbl_eu_8066A3F8);
    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_80537734, 256,
                               lbl_eu_80537700);
    VALIDATE_NW4R_POINTER_FLAG(text, textRegion, lbl_eu_805376EC, 257,
                               lbl_eu_805376B8);
    writer.Print(text, static_cast<int>(wcslen(text)));

    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_805378A0, 258,
                               lbl_eu_8053786C);
    writer.SetCursor(scaledW + lbl_eu_8066A3C8, scaledH + lbl_eu_8066A3C8,
                     lbl_eu_8066A3F8);
    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_80537734, 256,
                               lbl_eu_80537700);
    VALIDATE_NW4R_POINTER_FLAG(text, textRegion, lbl_eu_805376EC, 257,
                               lbl_eu_805376B8);
    writer.Print(text, static_cast<int>(wcslen(text)));

    // Center pass: gray level and alpha both follow the fade ramp.
    u8 gray = static_cast<u8>(lbl_eu_8066A3FC * fade);
    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DCFC, 135, lbl_eu_8052DCC8);
    writer.SetTextColor(nw4r::ut::Color(gray, gray, gray, 255));

    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_805378A0, 258,
                               lbl_eu_8053786C);
    writer.SetCursor(scaledW, scaledH, lbl_eu_8066A3D8);
    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_80537734, 256,
                               lbl_eu_80537700);
    VALIDATE_NW4R_POINTER_FLAG(text, textRegion, lbl_eu_805376EC, 257,
                               lbl_eu_805376B8);
    writer.Print(text, static_cast<int>(wcslen(text)));
}

// ---- wkStandbyLogin (0x804570FC) ----
bool CDeviceFont::wkStandbyLogin() {
    if (CDevice::isColdStartReady()) {
        CDeviceFontLayer::func_80454DE4();

        // Register a fresh ROM font-info provider in the first free list
        // slot (reslist::push_back with the retail setItem try/catch frame).
        if (lbl_eu_80665678 != 0) {
            IDeviceFontInfo* info = CDeviceFontInfoRom::create();
            CDeviceFont* font = lbl_eu_80665678;

            int i = 0;
            int capacity = font->mInfoList.mCapacity;
            CDeviceFontInfoListNode* startNode =
                font->mInfoList.mStartNodePtr;
            for (; i < capacity; i++) {
                if (font->mInfoList.mList[i].mNext == 0) break;
            }
            CDeviceFontInfoListNode* temp = &font->mInfoList.mList[i];
            IDeviceFontInfo** itemPtr = &temp->mItem;
            if (itemPtr != 0) {
                try {
                    *itemPtr = info;
                } catch (...) {
                    throw;
                }
            }
            temp->mNext = startNode;
            temp->mPrev = startNode->mPrev;
            startNode->mPrev->mNext = temp;
            startNode->mPrev = temp;
        }

        // Allocate the 4-byte font layer and register it as the exception
        // layer.
        CFontLayer* layer = (CFontLayer*)mtl::MemManager::allocate(
            4, mtl::MemManager::getHandleMEM1());
        if (layer != 0) {
            layer = __ct__CFontLayer(layer);
        }
        mFontId = (u32)layer;
        CWorkRoot::setException((CException*)this);
        return CWorkThread::wkStandbyLogin();
    }
    return false;
}

// ---- wkStandbyLogout (0x804571FC) ----
bool CDeviceFont::wkStandbyLogout() {
    // Delete the exception font layer (0x1E8) if one was created.
    CFontLayer* layer = (CFontLayer*)mFontId;
    if (layer != 0) {
        delete layer;
        mFontId = 0;
    }

    // Log out only when the children list is empty and the work/CLib
    // systems are already gone. Goto-gate keeps ONE shared 'return false'
    // exit (retail merges the li r3,0 tails; per-check duplication differs).
    if (!mChildren.empty()) goto logout_fail;
    if (CWorkSystem::getInstance() != 0) goto logout_fail;
    if (CLib::getInstance() != 0) goto logout_fail;

    CDeviceFontLayer::func_80454E2C();

    // Delete every font-info provider still in the list, then clear the ring.
    CDeviceFontInfoListNode* node = mInfoList.mStartNodePtr->mNext;
    while (node != mInfoList.mStartNodePtr) {
        IDeviceFontInfo* item = node->mItem;
        if (item != 0) {
            delete item;
            node->mItem = 0;
        }
        node = node->mNext;
    }
    node = mInfoList.mStartNodePtr->mNext;
    while (node != mInfoList.mStartNodePtr) {
        CDeviceFontInfoListNode* prev = node;
        node = node->mNext;
        prev->mNext = 0;
    }
    mInfoList.mStartNodePtr->mNext = mInfoList.mStartNodePtr;
    mInfoList.mStartNodePtr->mPrev = mInfoList.mStartNodePtr;
    return CWorkThread::wkStandbyLogout();

logout_fail:
    return false;
}

extern "C" void func_eu_80457318(u32 val) {
    extern u32 lbl_eu_80665680; // sdata2: font device config/state value
    lbl_eu_80665680 = val;
}

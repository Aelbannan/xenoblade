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

#include "monolib/work/CWorkUtil.hpp"
#include "monolib/core/CFontLayer.hpp"

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

// Find the font layer for `fontId` (0 selects the device's current font id)
// and tail-call the layer's func_80453BB4 with the passed-through args.
void func_8045271C__11CDeviceFontFv(u32 fontId, u32 a1, u32 a2, u32 a3) {
    if (lbl_eu_80665678 == 0) return;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    // Re-read the singleton through a volatile view: retail reloads the
    // pointer before the children walk (other work threads may replace the
    // device), and the volatile deref stops MWCC from CSE-ing it into the
    // null-check load above.
    CDeviceFont* font = *(CDeviceFont* volatile*)&lbl_eu_80665678;

    _reslist_node<CDeviceFontLayer*>* startNode = font->mChildren.mStartNodePtr;
    _reslist_node<CDeviceFontLayer*>* node = startNode->mNext;
    CDeviceFontLayer* layer;
    while (node != startNode) {
        CDeviceFontLayer* child = node->mItem;
        CDeviceFontLayer* candidate;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType == CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = child;
        } else {
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
    layer->func_80453BB4();
}

// Same lookup as func_8045271C, dispatching to the layer's func_80453FF0.
void func_8045283C__11CDeviceFontFv(u32 fontId, u32 a1, u32 a2, u32 a3) {
    if (lbl_eu_80665678 == 0) return;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    CDeviceFontLayer* layer = 0;
    _reslist_node<CDeviceFontLayer*>* startNode =
        lbl_eu_80665678->mChildren.mStartNodePtr;
    _reslist_node<CDeviceFontLayer*>* node = startNode->mNext;
    while (node != startNode) {
        CDeviceFontLayer* child = node->mItem;
        CDeviceFontLayer* candidate;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType == CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = child;
        } else {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == fontId) {
            layer = candidate;
            break;
        }
        node = node->mNext;
    }
    if (layer == 0) return;
    layer->func_80453FF0();
}

// Same lookup as func_8045271C, dispatching to the layer's func_804541F8.
void func_804528C4__11CDeviceFontFv(u32 fontId, u32 a1, u32 a2, u32 a3) {
    if (lbl_eu_80665678 == 0) return;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    CDeviceFontLayer* layer = 0;
    _reslist_node<CDeviceFontLayer*>* startNode =
        lbl_eu_80665678->mChildren.mStartNodePtr;
    _reslist_node<CDeviceFontLayer*>* node = startNode->mNext;
    while (node != startNode) {
        CDeviceFontLayer* child = node->mItem;
        CDeviceFontLayer* candidate;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType == CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = child;
        } else {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == fontId) {
            layer = candidate;
            break;
        }
        node = node->mNext;
    }
    if (layer == 0) return;
    layer->func_804541F8();
}

// Same lookup as func_8045271C, dispatching to the layer's func_8045438C.
void func_8045294C__11CDeviceFontFv(u32 fontId, u32 a1, u32 a2, u32 a3) {
    if (lbl_eu_80665678 == 0) return;
    if (fontId == 0) fontId = lbl_eu_80665678->mFontId;

    CDeviceFontLayer* layer = 0;
    _reslist_node<CDeviceFontLayer*>* startNode =
        lbl_eu_80665678->mChildren.mStartNodePtr;
    _reslist_node<CDeviceFontLayer*>* node = startNode->mNext;
    while (node != startNode) {
        CDeviceFontLayer* child = node->mItem;
        CDeviceFontLayer* candidate;
        if (child == 0) {
            candidate = 0;
        } else if (child->mType == CDeviceFontLayer::TYPE_FONT_LAYER) {
            candidate = child;
        } else {
            candidate = 0;
        }
        if (candidate != 0 && candidate->field_0x1F8 == fontId) {
            layer = candidate;
            break;
        }
        node = node->mNext;
    }
    if (layer == 0) return;
    layer->func_8045438C();
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
    CDeviceFontInfoListNode* sentinel = font->mInfoList.mStartNodePtr;
    u32 capacity = font->mInfoList.mCapacity;
    u32 i;
    for (i = 0; i < capacity; i++) {
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
    font->field_0x1EC = 2;
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
    for (node = *(CDeviceFontInfoListNode* volatile*)&mInfoList.mStartNodePtr->mNext;
         node != *(CDeviceFontInfoListNode* volatile*)&mInfoList.mStartNodePtr;
         node = node->mNext) {
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

void CDeviceFont::wkRender() {}

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
    // systems are already gone.
    if (!mChildren.empty()) return false;
    if (CWorkSystem::getInstance() != 0) return false;
    if (CLib::getInstance() != 0) return false;

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
}

extern "C" void func_eu_80457318(u32 val) {
    extern u32 lbl_eu_80665680; // sdata2: font device config/state value
    lbl_eu_80665680 = val;
}

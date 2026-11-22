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
#include "monolib/device/CDeviceFontLoader.hpp"
#include "monolib/core/CFontLayer.hpp"

// ---- Retail data labels (monolibdata2 dissolve) ----
// Foreign RTTI / base-list objects referenced by the CDeviceFont vtable and
// the reslist<CDeviceFontInfo> sub-vtables.
// Foreign rodata RTTI name strings referenced by the .sdata locators.
extern "C" u32 lbl_eu_80522D68;
extern "C" u32 lbl_eu_80522D7C;

// name-guess: sdata pointer to the "font/font_eu5.brfna" pool entry.
const char* sEuFontPath = (const char*)&lbl_eu_80522D68;
const char* sMenuFontPath = (const char*)&lbl_eu_80522D7C; // name-guess: "font/MenuFont.brfna" pool entry

// === .sbss size=0x10 align=8 (zero-fill) ===
CDeviceFont* spDeviceFont = 0;
u32 sDeviceFontInitCount = 0;
u32 spDeviceFontSharedText = 0;
u32 sDeviceFontFrameCount = 0;

// ---- CDeviceFont constructor (0x80454FB0) ----
CDeviceFont::CDeviceFont(const char* pName, CWorkThread* pParent)
    : CDeviceBase(pName, pParent, 0x200) {
    mFontId = 0;
    field_0x1EC = 0;
    spDeviceFont = this;
    // reserve(handle, 8): allocate the 8-node array and clear each mNext.
    mInfoList.mList = (CDeviceFontInfoListNode*)mtl::MemManager::allocate_array(
        0x60, mAllocHandle);
    for (int i = 0; i < 8; i++) {
        mInfoList.mList[i].mNext = 0;
    }
    mInfoList.mCapacity = 8;
    sDeviceFontInitCount = 0;
}

CDeviceFont::~CDeviceFont() {
    spDeviceFont = 0;
}

CDeviceFont* CDeviceFont::getInstance() {
    return spDeviceFont;
}

u32 CDeviceFont::getFontId() {
    CDeviceFont* font = spDeviceFont;
    if (font == 0) {
        return 0;
    }
    return font->mFontId;
}

extern "C" {

// Tail-call targets of the lookup wrappers: retail relocs to the Fv-mangled
// names even though the calls pass trailing arguments through in r4+.
void queueText__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 a1,
                                         u32 a2, u32 a3);
void setFontScale__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 a1);

// Find the font layer for `fontId` (0 selects the device's current font id)
// and tail-call the layer's queueText with the passed-through args.
// Loop locals declared first (see dispatchLayer4508): MWCC allocates in
// declaration order, and retail keeps child/candidate in r7, the walk node
// in r8 and the start sentinel in r9 (r4-r6 hold the passed-through args).
void dispatchLayerBB4__11CDeviceFontFv(u32 fontId, u32 a1, u32 a2, u32 a3) {
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CWorkThread*>* node;
    _reslist_node<CWorkThread*>* startNode;

    if (spDeviceFont == 0) return;
    if (fontId == 0) fontId = spDeviceFont->mFontId;

    // Volatile re-read: retail reloads the singleton before the children
    // walk; the volatile deref stops MWCC from CSE-ing it into the
    // null-check load above.
    CDeviceFont* font = *(CDeviceFont* volatile*)&spDeviceFont;

    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = (CDeviceFontLayer*)node->mItem;
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
    queueText__16CDeviceFontLayerFv(layer, a1, a2, a3);
}

// Same lookup as dispatchLayerBB4, dispatching to the layer's setBackgroundColor.
// The single trailing argument rides along to the layer method in r4 (which
// is why the walk colors child/candidate r5, node r6, sentinel r7).
void dispatchLayer3FF0__11CDeviceFontFv(u32 fontId, u32 a1) {
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CWorkThread*>* node;
    _reslist_node<CWorkThread*>* startNode;

    if (spDeviceFont == 0) return;
    if (fontId == 0) fontId = spDeviceFont->mFontId;

    // Volatile re-read (see dispatchLayerBB4); separate variables keep the
    // null-check load out of the walk pointer's register.
    CDeviceFont* font = *(CDeviceFont* volatile*)&spDeviceFont;

    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = (CDeviceFontLayer*)node->mItem;
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
    layer->setBackgroundColor((const ml::CCol4*)a1);
}

// Same lookup as dispatchLayerBB4, dispatching to the layer's setFontScale
// (no trailing arguments: retail colors child/candidate r4, node r5,
// sentinel r6).
void dispatchLayer41F8__11CDeviceFontFv(u32 fontId) {
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CWorkThread*>* node;
    _reslist_node<CWorkThread*>* startNode;

    if (spDeviceFont == 0) return;
    if (fontId == 0) fontId = spDeviceFont->mFontId;

    // Volatile re-read (see dispatchLayerBB4).
    CDeviceFont* font = *(CDeviceFont* volatile*)&spDeviceFont;

    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = (CDeviceFontLayer*)node->mItem;
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
    layer->setFontScale();
}

// Same lookup as dispatchLayerBB4, dispatching to the layer's setFontId
// with one trailing argument passed through in r4.
void dispatchLayer438C__11CDeviceFontFv(u32 fontId, u32 a1) {
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CWorkThread*>* node;
    _reslist_node<CWorkThread*>* startNode;

    if (spDeviceFont == 0) return;
    if (fontId == 0) fontId = spDeviceFont->mFontId;

    // Volatile re-read (see dispatchLayerBB4).
    CDeviceFont* font = *(CDeviceFont* volatile*)&spDeviceFont;

    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = (CDeviceFontLayer*)node->mItem;
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
    layer->setFontId(a1);
}

// ---------------------------------------------------------------------------
// Layer-method imports for the dispatch wrappers below. The retail symbol-map
// entries carry decompiler-guessed Fv suffixes even though the real
// signatures take arguments, so the calls reference the literal retail names
// (same arrangement as the layer unit's own extern "C" definitions).
// ---------------------------------------------------------------------------
void queueLineMode__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 a1);
void replayQueue__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 flag);
u32 getScaledTextHeight__16CDeviceFontLayerFv(CDeviceFontLayer* layer);
u32 getFontSlotData__16CDeviceFontLayerFv(CDeviceFontLayer* layer, u32 index);

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

// Same lookup as dispatchLayerBB4, dispatching to the layer's queueLineMode.
void dispatchLayer4508__11CDeviceFontFv(u32 fontId, u32 a1) {
    if (spDeviceFont == 0) return;
    if (fontId == 0) fontId = spDeviceFont->mFontId;

    // Re-read the singleton through a volatile view: retail reloads the
    // pointer before the children walk (other work threads may replace the
    // device), and the volatile deref stops MWCC from CSE-ing it into the
    // null-check load above.
    CDeviceFont* font = *(CDeviceFont* volatile*)&spDeviceFont;

    // Declare loop locals first: MWCC assigns registers in declaration
    // order, and retail keeps the child/layer value in the lowest register
    // (r5), the walk node in r6 and the start sentinel in r7.
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CWorkThread*>* node;
    _reslist_node<CWorkThread*>* startNode;
    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = (CDeviceFontLayer*)node->mItem;
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
    queueLineMode__16CDeviceFontLayerFv(layer, a1);
}

// Same lookup as dispatchLayerBB4, dispatching to the layer's replayQueue
// with the passed-through flag.
void dispatchLayer4B70__11CDeviceFontFv(u32 fontId, u32 flag) {
    if (spDeviceFont == 0) return;
    if (fontId == 0) fontId = spDeviceFont->mFontId;

    CDeviceFont* font = *(CDeviceFont* volatile*)&spDeviceFont;

    // Declare loop locals first: MWCC assigns registers in declaration
    // order, and retail keeps the child/layer value in the lowest register
    // (r5), the walk node in r6 and the start sentinel in r7.
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CWorkThread*>* node;
    _reslist_node<CWorkThread*>* startNode;
    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = (CDeviceFontLayer*)node->mItem;
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
    replayQueue__16CDeviceFontLayerFv(layer, flag);
}

// Same lookup as dispatchLayerBB4, but without the fontId-defaulting or the
// singleton reload: mark the bound layer's 0x2F0 flag and report success.
// (Called from CFontLayer's destructor with the object pointer as the key.)
u32 notifyLayerDestroy__11CDeviceFontFv(u32 key) {
    CDeviceFont* font = spDeviceFont;
    if (font == 0) return 0;

    // Declare loop locals first (see dispatchLayer4508): retail keeps the
    // child/layer value in r4, the walk node in r5, the sentinel in r6.
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CWorkThread*>* node;
    _reslist_node<CWorkThread*>* startNode;
    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = (CDeviceFontLayer*)node->mItem;
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

// Same lookup as dispatchLayerBB4, tail-calling the layer's getScaledTextHeight.
u32 getLayerStatus__11CDeviceFontFv(u32 fontId) {
    if (spDeviceFont == 0) return 0;
    if (fontId == 0) fontId = spDeviceFont->mFontId;

    CDeviceFont* font = *(CDeviceFont* volatile*)&spDeviceFont;

    // Declare loop locals first (see dispatchLayer4508).
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CWorkThread*>* node;
    _reslist_node<CWorkThread*>* startNode;
    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = (CDeviceFontLayer*)node->mItem;
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
    return getScaledTextHeight__16CDeviceFontLayerFv(layer);
}

// Same lookup as dispatchLayerBB4, tail-calling the layer's getFontSlotData with
// the passed-through index and returning its result.
u32 getLayerData__11CDeviceFontFv(u32 fontId, u32 index) {
    if (spDeviceFont == 0) return 0;
    if (fontId == 0) fontId = spDeviceFont->mFontId;

    CDeviceFont* font = *(CDeviceFont* volatile*)&spDeviceFont;

    // Declare loop locals first (see dispatchLayer4508).
    CDeviceFontLayer* layer;
    CDeviceFontLayer* child;
    CDeviceFontLayer* candidate;
    _reslist_node<CWorkThread*>* node;
    _reslist_node<CWorkThread*>* startNode;
    startNode = font->mChildren.mStartNodePtr;
    node = startNode->mNext;
    while (node != startNode) {
        child = (CDeviceFontLayer*)node->mItem;
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
    return getFontSlotData__16CDeviceFontLayerFv(layer, index);
}

}

// Create and register a font-layer work thread under this device: allocate
// the layer from work memory, construct it with the shared layer name, link
// it into the device's children list and record the owning device back on
// the layer. Returns 1 on success, 0 when the device is not created yet.
u32 CDeviceFont::func_804525F0() {
    CDeviceFont* font = spDeviceFont;
    if (font == 0) return 0;

    const char* name = "CDeviceFontLayer";
    CDeviceFontLayer* layer = (CDeviceFontLayer*)mtl::MemManager::allocate(
        0x2f8, CWorkThreadSystem::getWorkMem());
    if (layer != 0) {
        layer = __ct__CDeviceFontLayer(layer, name, font);
    }
    CWorkUtil::entryWork((CWorkThread*)layer, (CWorkThread*)font, false);
    layer->field_0x1F8 = (u32)this;
    return 1;
}

// ---- getFontInfo (0x8045580C) ----
// Find the font-info provider for this device: first pass looks for the info
// whose identity query (0xC slot) matches this device and that is available
// (0x34 slot returns 0); second pass falls back to the first available info
// regardless of identity. Returns the matching provider or null. (The fontId
// and layout parameters are unused in the body, as in retail.)
void* CDeviceFont::getFontInfo(u32 fontId,
                                            nw4r::lyt::Layout* layout) {
    CDeviceFont* font = spDeviceFont;
    if (font == 0) return 0;

    CDeviceFontInfoListNode* node = font->mInfoList.mStartNodePtr->mNext;
    while (node != spDeviceFont->mInfoList.mStartNodePtr) {
        if (node->mItem->isStateNonZero() == 0 &&
            node->mItem->getFlags() == (s32)fontId) {
            return (CDeviceFontInfo*)node->mItem;
        }
        node = node->mNext;
    }
    node = spDeviceFont->mInfoList.mStartNodePtr->mNext;
    while (node != spDeviceFont->mInfoList.mStartNodePtr) {
        if (node->mItem->isStateNonZero() == 0) {
            return (CDeviceFontInfo*)node->mItem;
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
CDeviceFontInfo* CDeviceFont::findFontByDevice() {
    CDeviceFont* font = spDeviceFont;
    if (font == 0) return 0;

    CDeviceFontInfoListNode* node = font->mInfoList.mStartNodePtr->mNext;
    while (node != spDeviceFont->mInfoList.mStartNodePtr) {
        if (node->mItem->isStateNonZero() == 0 &&
            node->mItem->getFlags() == (s32)this) {
            return (CDeviceFontInfo*)node->mItem;
        }
        node = node->mNext;
    }
    return 0;
}

// Create a font-info provider
// free slot; setItem is guarded by the retail null check + try/catch frame
// marker), and flag the list dirty (field_0x1EC = 2).
u32 CDeviceFont::createFontInfo() {
    if (spDeviceFont == 0) return 0;

    CDeviceFontInfoExt* info = CDeviceFontInfoExt::create();

    CDeviceFont* font = spDeviceFont;
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
                *itemPtr = (IDeviceFontInfo*)info;
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
    spDeviceFont->field_0x1EC = 2;
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
        node->mItem->advanceState();
        node = node->mNext;
    }

    // Second pass: drop the first provider whose 0x38 slot reports done.
    // The `if (item != 0) delete item;` guard is what produces retail's
    // double null-check around the virtual dtor call.
    node = *(CDeviceFontInfoListNode* volatile*)&mInfoList.mStartNodePtr->mNext;
    while (node != *(CDeviceFontInfoListNode* volatile*)&mInfoList.mStartNodePtr) {
        if (node->mItem->isStateReady() != 0) {
            CDeviceFontInfo* item = (CDeviceFontInfo*)node->mItem;
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
                name = "CDeviceFontLoader";
                CDeviceFontLoader* loader = new (CWorkThreadSystem::getWorkMem())
                    CDeviceFontLoader(name, this);
                CWorkUtil::entryWork((CWorkThread*)loader, (CWorkThread*)this,
                                     false);
                if (func_eu_8044A600()) {
                    setFontPath__17CDeviceFontLoaderFv(loader, 1,
                                                          sMenuFontPath);
                } else {
                    setFontPath__17CDeviceFontLoaderFv(loader, 1,
                                                          sEuFontPath);
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
                spDeviceFont->mInfoList.mStartNodePtr->mNext;
            while (infoNode != spDeviceFont->mInfoList.mStartNodePtr) {
                if (infoNode->mItem->getFlags() == 0) {
                    infoNode->mItem->initState();
                    break;
                }
                infoNode = infoNode->mNext;
            }
            field_0x1EC = 3;
        }
        break;
    }
}


// Pointer-validation macros mirroring the nw4r db asserts inlined by retail
// (same shapes as src/kyoshin/code_8025FB10.cpp).
#define VALIDATE_NW4R_POINTER(pointer, file, line, message)                    \
    {                                                                         \
        bool validMem1 = true;                                                \
        bool validMem2 = true;                                                \
        bool validIo = true;                                                  \
        bool validIo2 = true;                                                 \
        bool validRegs = true;                                                \
        u32 address = (u32)(pointer);                                         \
        if ((address & 0xFF000000) != 0x80000000) {                           \
            validMem1 = false;                                                \
        }                                                                     \
        if (!validMem1 && (address & 0xFF800000) != 0x81000000) {              \
            validMem2 = false;                                                \
        }                                                                     \
        if (!validMem2 && (address & 0xF8000000) != 0x90000000) {              \
            validIo = false;                                                  \
        }                                                                     \
        if (!validIo && (address & 0xFF000000) != 0xC0000000) {                \
            validIo2 = false;                                                 \
        }                                                                     \
        if (!validIo2 && (address & 0xFF800000) != 0xC1000000) {               \
            validRegs = false;                                                \
        }                                                                     \
        if (!validRegs) {                                                     \
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
            ((u32)(pointer) & 0xFF800000) == 0xC1000000) {                     \
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
    // Exception handoff:
    // exception, count frames; release the exception once the counter wraps
    // negative (retail: addic./ble after the increment).
    if (getInstance__8CDesktopFv() != 0) {
        if (CWorkRoot::getException() == (CException*)this) {
            if ((s32)++sDeviceFontInitCount < 1) {
                CWorkRoot::setException(0);
            }
        }
    }

    if (spDeviceFont == 0) {
        return;
    }

    CDeviceVI::setFlag0(false);

    // Frame counter cycling at 2000 frames.
    sDeviceFontFrameCount++;
    if ((s32)sDeviceFontFrameCount >= 2000) {
        sDeviceFontFrameCount = 0;
    }

    // Fade-in alpha ramp over the first 1000 counted frames.
    f32 fade = 0.6f *
                   static_cast<f32>(static_cast<f64>(static_cast<s32>(
                       sDeviceFontFrameCount - 1000)) /
                                   1000.0f) +
               0.4f;
    if (fade < 0.0f) {
        fade = 0.4f - fade;
    }
    if (fade >= 0.3f) {
        if (fade > 0.4f) {
            fade = 0.4f;
        }
    } else {
        fade = 0.3f;
    }

    // Convert the shared UTF-8 source into the UTF-16 render buffer.
    u32 destLen = 0x200;
    memset(lbl_eu_80657750, 0, 0x400);
    // spDeviceFontSharedText is the shared UTF-8 source text pointer (retail passes
    // it as the converter's src argument, not a gate flag).
    if (ENCConvertStringUtf8ToUtf16((u16*)lbl_eu_80657750, &destLen,
                                    (const u8*)spDeviceFontSharedText,
                                    (u32*)NULL) != ENC_OK) {
        return;
    }

    WideTextWriter writer;
    writer.SetupGX();

    u32 writerRegion = (u32)&writer & 0xFF000000;

    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DC70, 139, lbl_eu_8052DC3C);
    writer.SetDrawFlag(0x110);

    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DD84, 171, lbl_eu_8052DD50);
    writer.SetScale(1.0f, 1.2658228f);

    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DC28, 98, lbl_eu_8052DBF4);
    writer.SetCharSpace(2.0f);

    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DD84, 171, lbl_eu_8052DD50);
    writer.SetCharSpace(1.0f);

    // Fetch the bound font twice like retail (null check + use).
    nw4r::lyt::Layout* layout;  // unused second argument, uninitialized in retail
    if (((CDeviceFontInfo*)getFontInfo(0, layout))->getFont() == NULL) {
        return;
    }
    const nw4r::ut::Font* font = static_cast<const nw4r::ut::Font*>(
        ((CDeviceFontInfo*)getFontInfo(0, layout))->getFont());

    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8053785C, 65, lbl_eu_80537828);
    VALIDATE_NW4R_POINTER(font, lbl_eu_80537818, 66, lbl_eu_805377E0);
    writer.SetFont(*font);

    GXRenderModeObj* rmo;
    f32 scaledW;
    f32 scaledH;

    rmo = CDeviceVI::getRenderModeObj();
    scaledW = 0.5f * static_cast<f32>(static_cast<f64>(rmo->fbWidth));
    rmo = CDeviceVI::getRenderModeObj();
    scaledH = 0.5f * static_cast<f32>(static_cast<f64>(rmo->efbHeight));

    writer.SetupGX();

    // One shared matrix buffer: identity position matrix, then overwritten
    // with the orthographic projection.
    Mtx44 mtx;
    PSMTXIdentity(mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    f32 width;
    rmo = CDeviceVI::getRenderModeObj();
    width = static_cast<f32>(static_cast<f64>(rmo->fbWidth));
    rmo = CDeviceVI::getRenderModeObj();
    C_MTXOrtho(mtx, 0.0f,
               static_cast<f32>(static_cast<f64>(rmo->efbHeight)),
               0.0f, width, 0.0f, -16777216.0f);
    GXSetProjection(mtx, GX_ORTHOGRAPHIC);

    // Shadow pass: four black copies around the final position.
    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DCFC, 135, lbl_eu_8052DCC8);
    writer.SetTextColor(nw4r::ut::Color(0, 0, 0, 255));

    const wchar_t* text = lbl_eu_80657750;
    u32 textRegion = (u32)text & 0xFF000000;

    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_805378A0, 258,
                               lbl_eu_8053786C);
    writer.SetCursor(scaledW - 1.0f, scaledH - 1.0f,
                     -1.0f);
    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_80537734, 256,
                               lbl_eu_80537700);
    VALIDATE_NW4R_POINTER(text, lbl_eu_805376EC, 257, lbl_eu_805376B8);
    writer.Print(text, static_cast<int>(wcslen(text)));

    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_805378A0, 258,
                               lbl_eu_8053786C);
    writer.SetCursor(scaledW + 1.0f, scaledH - 1.0f,
                     -1.0f);
    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_80537734, 256,
                               lbl_eu_80537700);
    VALIDATE_NW4R_POINTER_FLAG(text, textRegion, lbl_eu_805376EC, 257,
                               lbl_eu_805376B8);
    writer.Print(text, static_cast<int>(wcslen(text)));

    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_805378A0, 258,
                               lbl_eu_8053786C);
    writer.SetCursor(scaledW - 1.0f, scaledH + 1.0f,
                     -1.0f);
    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_80537734, 256,
                               lbl_eu_80537700);
    VALIDATE_NW4R_POINTER_FLAG(text, textRegion, lbl_eu_805376EC, 257,
                               lbl_eu_805376B8);
    writer.Print(text, static_cast<int>(wcslen(text)));

    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_805378A0, 258,
                               lbl_eu_8053786C);
    writer.SetCursor(scaledW + 1.0f, scaledH + 1.0f,
                     -1.0f);
    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_80537734, 256,
                               lbl_eu_80537700);
    VALIDATE_NW4R_POINTER_FLAG(text, textRegion, lbl_eu_805376EC, 257,
                               lbl_eu_805376B8);
    writer.Print(text, static_cast<int>(wcslen(text)));

    // Center pass: gray level and alpha both follow the fade ramp.
    u8 gray = static_cast<u8>(255.0f * fade);
    VALIDATE_NW4R_POINTER(&writer, lbl_eu_8052DCFC, 135, lbl_eu_8052DCC8);
    writer.SetTextColor(nw4r::ut::Color(gray, gray, gray, 255));

    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_805378A0, 258,
                               lbl_eu_8053786C);
    writer.SetCursor(scaledW, scaledH, 0.0f);
    VALIDATE_NW4R_POINTER_FLAG(&writer, writerRegion, lbl_eu_80537734, 256,
                               lbl_eu_80537700);
    VALIDATE_NW4R_POINTER_FLAG(text, textRegion, lbl_eu_805376EC, 257,
                               lbl_eu_805376B8);
    writer.Print(text, static_cast<int>(wcslen(text)));
}

// ---- wkStandbyLogin (0x804570FC) ----
bool CDeviceFont::wkStandbyLogin() {
    if (CDevice::isColdStartReady()) {
        CDeviceFontLayer::allocFontHeap();

        // Register a fresh ROM font-info provider in the first free list
        // slot (reslist::push_back with the retail setItem try/catch frame).
        if (spDeviceFont != 0) {
            CDeviceFontInfo* info = CDeviceFontInfoRom::create();
            CDeviceFont* font = spDeviceFont;

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
                    *itemPtr = (IDeviceFontInfo*)info;
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

    CDeviceFontLayer::freeFontHeap();

    // Delete every font-info provider still in the list, then clear the ring.
    CDeviceFontInfoListNode* node = mInfoList.mStartNodePtr->mNext;
    while (node != mInfoList.mStartNodePtr) {
        CDeviceFontInfo* item = (CDeviceFontInfo*)node->mItem;
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

extern "C" void CDeviceFont_SetSharedText(u32 val) {
    extern u32 spDeviceFontSharedText; // sdata2: font device config/state value
    spDeviceFontSharedText = val;
}

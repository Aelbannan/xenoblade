// Auto-scaffolded catalog TU for kyoshin/menu/CMenuArtsSet
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuArtsSet.hpp"
#include <cstddef>
#include <nw4r/lyt.h>

// C++-linkage draw helper (mangles to the retail symbol).
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// Arts archive path string (accessed via sda21 small-data relocation).
extern char lbl_eu_8050AC70[];

void __ct__CMenuArtsSet(){}

extern "C" unsigned long func_8022F530() {
    extern unsigned long lbl_eu_80664740;
    return lbl_eu_80664740 != 0;
}

void func_8022F544(){}

void cbRenderBefore__12CMenuArtsSetFv(void* self);
void func_8022FA48(void* self) {
    // Thunk: adjust this down from sub-object at offset 0x58
    CMenuArtsSet* obj = (CMenuArtsSet*)((char*)self - offsetof(CMenuArtsSet, mSubObj58_start));
    cbRenderBefore__12CMenuArtsSetFv(obj);
}

void __dt__12CMenuArtsSetFv(void* self);
void func_8022FA50(void* self) {
    // Thunk: adjust this down from sub-object at offset 0x58
    CMenuArtsSet* obj = (CMenuArtsSet*)((char*)self - offsetof(CMenuArtsSet, mSubObj58_start));
    __dt__12CMenuArtsSetFv(obj);
}

// Sub-object ctors (retail unmangled C symbols). extern "C" keeps the
// definition symbol and call-site relocs on the retail name; noinline keeps
// CArtsList ctor/OnFileEvent emitting real bl's (retail never inlines them).
extern "C" __declspec(noinline) void __ct__8022FA58(SArtsSub8022FA58* self, u32 a, u32 b) {
    self->field_0x00 = a;
    self->field_0x04 = b;
    self->field_0x08 = 0;
    self->field_0x0C = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x15 = 0;
    self->field_0x16 = 1;
    self->field_0x17 = 0;
    self->field_0x18 = 0;
    self->field_0x21 = 0;
}

extern "C" void* __dt__8022FA90(void* self, int flags) {
    if (self && flags > 0) {
        ::operator delete(self);
    }
    return self;
}

// 0x124 sub-object rebuild (retail C symbol); stub for a sibling target.
// noinline: CArtsList::OnFileEvent keeps a real bl.
extern "C" __declspec(noinline) void func_8022FAD0(SArtsSub8022FA58* self) {}

// Switch-case handlers used by func_8022FDF4 (same TU, defined below).
void func_8022FF74(SArts2FF74* self);
extern "C" __declspec(noinline) void func_80230070(SArts30070* self);

// C-linkage + noinline so callers (func_80233760) emit a real bl to the
// retail symbol instead of inlining the body.
extern "C" __declspec(noinline) void func_8022FD9C(SArts2FDF4* self) {
    if (self->field_0x8) {
        if (self->field_0x8)
            self->field_0x8->v2(1);
        self->field_0x8 = 0;
    }
}

extern "C" __declspec(noinline) void func_8022FDF4(SArts2FDF4* self) {
    switch (self->field_0x18) {
        case 0: func_8022FF74((SArts2FF74*)self); break;
        case 1: func_80230070((SArts30070*)self); break;
    }
    self->field_0x8->v14(0);
}

// 2-arg C++ overload of func_80124270 (separate mangled symbol), used by func_80230D18.
void func_80124270(void*, u32);

// AnimTransform frame-check helper (defined in COption.cpp / CArtsInfo.cpp).
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
// State-progression byte table (sdata, 8 bytes). Fixed-size decl: incomplete
// array types are not sdata-eligible and force lis/addi instead of sda21.
extern u8 lbl_eu_806628A8[8];
// Switch-case handlers used by func_8022FDF4 (same TU, defined below).
void func_8022FF74(SArts2FF74* self);
extern "C" __declspec(noinline) void func_80230070(SArts30070* self);

void func_8022FE58(SArtsSub8022FA58* self) {
    self->field_0x16 = 0;
    self->field_0x17 = 1;
    u8 next = self->field_0x14 + 1;
    self->field_0x14 = next;
    if ((s8)next >= self->field_0x21) {
        self->field_0x14 = 0;
    }
    return func_80230160(self);
}

// C-linkage + noinline: func_802346BC/func_80234780 keep a real bl to this
// retail symbol instead of inlining the body.
extern "C" __declspec(noinline) void func_8022FE90(SArtsSub8022FA58* self) {
    self->field_0x16 = 0;
    self->field_0x17 = 0;
    u8 v = self->field_0x14 - 1;
    self->field_0x14 = v;
    if ((s8)v < 0) {
        self->field_0x14 = (u8)(self->field_0x21 - 1);
    }
    return func_80230160(self);
}

// vtable +0x200 slot on the +0x17C manager sub-object, result discarded.
// NOTE: the header's SArtsManager vtable is mid-edit — v128 currently maps
// to +0x208 (MWCC +2 leading slots); the retail +0x200 slot needs 2 fewer
// fillers before v126. Manual casts add an extra indirection.
// C-linkage + noinline so callers emit a real bl (retail shape).
extern "C" __declspec(noinline) int func_8022FEC4(SArtsSub8022FA58* self) {
    SArtsManagerRoot* root =
        (SArtsManagerRoot*)func_8009EC9C(func_8023040C(self, self->field_0x14));
    SArtsManager* obj = &root->mObj17C;
    return obj->v128();
}

extern "C" __declspec(noinline) void func_8022FF00(SArtsSub8022FA58* self, int arg2) {
    SArtsManagerRoot* root =
        (SArtsManagerRoot*)func_8009EC9C(func_8023040C(self, self->field_0x14));
    SArtsManager* obj = &root->mObj17C;
    s32 prev = obj->v128();               // vtable +0x200
    obj->v126(prev - arg2);               // vtable +0x1F8
    func_80230160(self);
}

// Layout-out completion handler (case-0 sibling of func_8022FDF4's switch):
// run the AnimTransform at 0x0C; when the 0x16 flag is clear, drive both
// sub-panes via the 0x08 driver, reset the 0x10 float, mark the 0x18 state,
// then load the 0x17-dependent window label through the driver's field_0x10
// pane interface and show it. (0x0C/0x10 roles are swapped vs the matched
// func_80230070 twin.)
__declspec(noinline) void func_8022FF74(SArts2FF74* self) {
    float duration = lbl_eu_80668648;
    func_80137444(self->field_0x0C, duration);
    if (self->field_0x16 == 0) {
        self->field_0x08->v11(self->field_0x0C, 0);
        self->field_0x08->v11(self->field_0x10, 1);
        self->field_0x10->field_0x10 = lbl_eu_8066864C;
        self->field_0x18 = 1;
        if (self->field_0x17 != 0) {
            func_80124270(((SArts3CObj*)self->field_0x08->field_0x10)->v13(lbl_eu_8050AC70 + 0x8F, 1), 1);
        } else {
            func_80124270(((SArts3CObj*)self->field_0x08->field_0x10)->v13(lbl_eu_8050AC70 + 0x7F, 1), 1);
        }
    }
}

// Layout-in completion handler (case-1 sibling of func_8022FDF4's switch):
// when the AnimTransform at 0x10 finishes, drive both sub-panes via the
// 0x08 driver, reset the 0x0C float, mark the 0x18/0x16 state, then load
// both window labels through the driver's field_0x10 pane interface.
// C-linkage + noinline: func_8022FDF4 keeps a real bl (retail never inlines).
extern "C" __declspec(noinline) void func_80230070(SArts30070* self) {
    float duration = lbl_eu_80668648;
    if (func_80137444(self->field_0x10, duration) != 0) {
        self->field_0x08->v11(self->field_0x10, 0);
        self->field_0x08->v11(self->field_0x0C, 1);
        self->field_0x0C->field_0x10 = lbl_eu_8066864C;
        self->field_0x18 = 0;
        self->field_0x16 = 1;
        func_80124270(((SArts3CObj*)self->field_0x08->field_0x10)->v13(lbl_eu_8050AC70 + 0x7F, 1), 0);
        func_80124270(((SArts3CObj*)self->field_0x08->field_0x10)->v13(lbl_eu_8050AC70 + 0x8F, 1), 0);
    }
}



// Build the arts-selection list: clear the cursor, then append each of the
// first `count` arts entries that pass the (v>8) / (v==4 && locked) filters.
#pragma push
#pragma optimize_for_size on
void func_80230374(SArtsSub8022FA58* self) {
    self->field_0x21 = 0;
    u8 count = code80135FDC_getByte_64077();
    for (u8 i = 0; i < count; i++) {
        u8 v = func_801392B4(i);
        if (v > 8) continue;
        if (v == 4 && func_800A32BC(func_8009EC9C(v)) != 0) continue;
        u8 idx = self->field_0x21;
        self->field_0x19[idx] = v;
        self->field_0x21 = idx + 1;
    }
}
#pragma pop

extern "C" __declspec(noinline) u8 func_8023040C(SArtsSub8022FA58* self, u32 idx) {
    if (idx >= self->field_0x21) {
        return 0;
    }
    return self->field_0x19[idx];
}

extern "C" __declspec(noinline) void __ct__8023042C(SArts3042C* self, u32 a, u32 b) {
    self->field_0x00 = a;
    self->field_0x04 = b;
    self->field_0x08 = 0;
    self->field_0x0C = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1C = 0;
    self->field_0x20 = 4;
    self->field_0x21 = 4;
    self->field_0x22 = 0;
    self->field_0x23 = 1;
    self->field_0x24 = 0;
    self->field_0x25 = 2;
    self->field_0x26 = 0;
    self->field_0x27 = 0;
    self->field_0x28 = 1;
}

extern "C" void* __dt__80230484(void* self, int flags) {
    if (self && flags > 0) {
        ::operator delete(self);
    }
    return self;
}

// Load the layout-pair object (twin of func_80231A48): bind the 0x08/0x14
// layout drivers to the arc accessor, attach the four AnimTransforms, park
// both panes via v11, reset the state machine via v14, show the two label
// panes (v13 at +0x3C) then refresh the cursor drivers (func_802316F8).
#pragma optimize_for_size on
void func_802304C4(SArts304C4* self) {
    func_80136E84((nw4r::lyt::Layout**)&self->field_0x08, self->field_0x04,
                  lbl_eu_8050AC70 + 0x109);
    func_80136F08((nw4r::lyt::Layout*)self->field_0x08, &self->field_0x0C,
                  self->field_0x04, lbl_eu_8050AC70 + 0x122);
    func_80136F08((nw4r::lyt::Layout*)self->field_0x08, &self->field_0x10,
                  self->field_0x04, lbl_eu_8050AC70 + 0x140);
    self->field_0x08->v11(self->field_0x10, 0);
    self->field_0x08->v11(self->field_0x0C, 1);
    self->field_0x08->v14(0);
    func_80124270(self->field_0x08->field_0x10->v13(lbl_eu_8050AC70 + 0x15C, 1), 0);
    func_80124270(self->field_0x08->field_0x10->v13(lbl_eu_8050AC70 + 0x16A, 1), 0);
    func_80136E84((nw4r::lyt::Layout**)&self->field_0x14, self->field_0x04,
                  lbl_eu_8050AC70 + 0x178);
    func_80136F08((nw4r::lyt::Layout*)self->field_0x14, &self->field_0x18,
                  self->field_0x04, lbl_eu_8050AC70 + 0x190);
    func_80136F08((nw4r::lyt::Layout*)self->field_0x14, &self->field_0x1C,
                  self->field_0x04, lbl_eu_8050AC70 + 0x1AD);
    self->field_0x14->v11(self->field_0x1C, 0);
    self->field_0x14->v11(self->field_0x18, 1);
    self->field_0x14->v14(0);
    func_80124270(self->field_0x14->field_0x10, 0);
    func_802316F8((SArtsSub8022FA58*)self);
}
#pragma optimize_for_size off

// Release helper for the mSubObj148 pair (called from func_80233760).
// Doubly-nested guard reproduces retail's duplicated dead beq (MWCC CSEs
// the re-tested condition into one cmpwi + two beq).
extern "C" __declspec(noinline) void func_8023066C(SArts3066C* self) {
    if (self->field_0x08 != 0) {
        if (self->field_0x08 != 0) {
            self->field_0x08->v2(1);
        }
        self->field_0x08 = 0;
    }
    if (self->field_0x14 != 0) {
        if (self->field_0x14 != 0) {
            self->field_0x14->v2(1);
        }
        self->field_0x14 = 0;
    }
}

// State-machine advance for the 0x148 sub-object: while enabled (0x22), run
// the current phase handler selected by 0x24 (layout-in/out pair), then the
// secondary one selected by 0x25, and finally reset both +0x38 pointees via
// their vtable slot (arg 0).
// Phase handlers driven by func_802306F0 (defined later in this TU; C-linkage
// retail names declared in the header).
void func_802306F0(SArts306F0* self) {
    if (self->field_0x22 != 0) {
        switch (self->field_0x24) {
            case 0: func_80231480((SArts31480*)self); break;
            case 1: func_8023150C((SArts3150C*)self); break;
        }
        switch (self->field_0x25) {
            case 2: func_802315BC((SArts315BC*)self); break;
            case 3: func_80231648((SArts31648*)self); break;
        }
        self->field_0x08->v12(0);
        self->field_0x14->v12(0);
    }
}

extern "C" __declspec(noinline) void func_802307A4(SArtsDrawBox* self, nw4r::lyt::DrawInfo* info) {
    if (self->field_0x22 != 0 && self->field_0x28 != 0) {
        func_80137038(self->mLayout08, info, 0, 1);
        func_80137038(self->mLayout14, info, 0, 1);
    }
}

// C-linkage + noinline: func_80234FDC calls this and retail keeps a real
// bl (not an inline).
extern "C" __declspec(noinline) void func_8023080C(SArtsSub8022FA58* self, u8 val) {
    if (self->field_0x27 != 0) {
        func_80124270(((SArts080C*)(self->field_0x08))->field_0x10, 0);
    } else {
        self->field_0x22 = val;
        return func_802316F8(self);
    }
}

// Cursor-store helpers used by func_80233DC0/func_80233E9C etc. C-linkage +
// noinline: retail keeps real bl calls (never inlined).
extern "C" __declspec(noinline) void func_8023082C(SArtsSub8022FA58* self, int a) {
    self->field_0x20 = (s8)(self->field_0x20 + 1);
    if ((s8)self->field_0x20 >= 9) {
        self->field_0x20 = 0;
    }
    if (a != 0 && self->field_0x20 == 4) {
        self->field_0x20 = 5;
    }
    return func_802316F8(self);
}

extern "C" __declspec(noinline) void func_80230870(SArtsSub8022FA58* self, int a) {
    self->field_0x20 = (s8)(self->field_0x20 - 1);
    if ((s8)self->field_0x20 < 0) {
        self->field_0x20 = 8;
    }
    if (a != 0 && self->field_0x20 == 4) {
        self->field_0x20 = 3;
    }
    return func_802316F8(self);
}

extern "C" __declspec(noinline) void func_802308B0(SArtsSub8022FA58* self, u8 v){}

// Drive both pointees when the 0x23 state is set: reset 0x24, show the
// first pane, hide the second. C-linkage + noinline: func_80233F78 keeps a
// real bl (retail symbol) instead of inlining the pane-call pair.
extern "C" __declspec(noinline) void func_80230D18(SArts30D18* self) {
    if (self->field_0x23 != 0) {
        self->field_0x24 = 0;
        func_80124270(self->field_0x08->field_0x10, 1);
        func_80124270(self->field_0x14->field_0x10, 0);
    }
}

extern "C" __declspec(noinline) void func_80230D74(SArtsSub8022FA58* self, u32 val){}

void func_80230FF0(SArtsSubDElem* self, u8 val, u32 idx, u32 sub, u32 off) {
    SArtsSubDElem* e = &self[idx];
    u8* d = e->data + sub * 8;
    d[off] = val;
    e->field_0x30 = 1;
}

// Cursor/state helper (retail C symbol); stub for a sibling target. The
// return value feeds func_80234D68 / func_80235124, so it stays a u8 stub.
extern "C" __declspec(noinline) u8 func_80231014(SArtsSub8022FA58* self) { return 0; }

// Arts-list availability query: ask the arts manager to refresh, then pick
// the arts element for the current character, clamp the 0x20 cursor
// (4 -> none), read the state byte from the element's data window (offset 8
// when the byte-table entry and mode byte are both valid), translate it
// through the 0x1C8/0x1FE string table, and - when the learn-arts flag grid
// reports the entry set - return the high byte of the 16-bit value at the
// flagged offset.
extern "C" __declspec(noinline) u8 func_80231220(SArtsSub8022FA58* self) {
    SArtsSubDElem* elems;
    SArtsManagerRoot* root;
    u8 arts;
    u8 result;
    root = (SArtsManagerRoot*)func_8009EC9C(self->field_0x26);
    root->mObj17C.v157();
    elems = &root->mElemsE8[0];
    arts = (u8)func_800A32BC(root);
    result = 0;
    u8 idx = self->field_0x20;
    if ((s32)idx != 4) {
        if ((u32)idx >= 4) {
            idx -= 1;
        }
        int b = (lbl_eu_806628A8[0] != 0) || (self->field_0x26 != 1);
        u8 off = (b == 0) * 8;
        u8 v = elems[arts].data[idx + off];
        char* base = lbl_eu_8050AC70;
        u32 r = func_8013600C(base + 0x1C8, base + 0x1FE, v);
        if (func_801F9268((u8*)elems, arts, (u8)r) != 0) {
            u16 w = *(const u16*)&((u8*)&elems[arts])[(u8)r << 1];
            result = *(const u8*)&w;
        }
    }
    return result;
}

// Arts-list availability check: ask the arts manager (vtable +0x27C) to
// recompute the list, then scan the 8-byte window at data[off..off+7] of the
// current arts element for all-zero state. The window starts at data[8] when
// the arts id is valid (byte-table entry 0 and mode byte 1), else at data[0].
// Returns 1 when all scanned bytes are zero.
// C-linkage + noinline: retail keeps a real bl from func_80234844.
// optimize_for_size: retail's two-register prologue uses stmw (-O4,s).
#pragma optimize_for_size on
extern "C" __declspec(noinline) u32 func_80231320(SArtsSub8022FA58* self) {
    SArtsManagerRoot* root = (SArtsManagerRoot*)func_8009EC9C(self->field_0x26);
    root->mObj17C.v157();
    SArtsSubDElem* base = &root->mElemsE8[0];
    u8 arts = (u8)func_800A32BC(root);
    int b = (lbl_eu_806628A8[0] != 0) || (self->field_0x26 != 1);
    int off = (b != 0) * 8;
    SArtsSubDElem* e = &base[arts];
    for (int i = 0; i < 8; i++) {
        if (e->data[off + i] != 0) return 0;
    }
    return 1;
}
#pragma optimize_for_size off

// Toggle the 0x27 busy flag: while busy, clear it and park both panes
// (first visible, second hidden); otherwise set it, stash the old 0x20
// cursor into 0x21, and run the layout-out animation.
// C-linkage + noinline: func_80233F78 keeps real bl relocs to the retail
// symbols (an inline would fold the body and duplicate the pane calls).
extern "C" __declspec(noinline) void func_802313E0(SArts313E0* self) {
    if (self->field_0x27 != 0) {
        self->field_0x27 = 0;
        func_80124270(self->field_0x08->field_0x10, 1);
        func_80124270(self->field_0x14->field_0x10, 0);
    } else {
        self->field_0x27 = 1;
        self->field_0x21 = self->field_0x20;
        func_8023185C(self);
        func_80124270(self->field_0x14->field_0x10, 1);
    }
}

extern "C" __declspec(noinline) void func_80231464(SArtsSub8022FA58* self) {
    if (self->field_0x20 != 4) {
        return;
    }
    self->field_0x20 = 3;
    return func_802316F8(self);
}

// func_80231848 is defined later in this TU (writes the 0x44/0x48 floats of
// the pane-like object passed to func_802316F8). C-linkage retail name.
extern "C" void func_80231848(CMenuArtsSet* self, const SArtsVec2* src);

// Refresh the arts-list cursor: copy the 9-word label block from
// lbl_eu_8050ABB4, look up the pane for the current 0x20 cursor entry, scale
// its translate by the 0xA6 label pane's scale, move the 0x08 sub-pane onto
// it, then write the precomputed 9-entry position table (entry 4 uses the
// smaller lbl_eu_80668654 value) into the pane's layout offsets via
// func_80231848.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_802316F8(SArtsSub8022FA58* self) {
    nw4r::math::VEC3 pos;
    SArts316F8Block tmp;
    tmp = lbl_eu_8050ABB4;
    SArtsVec2 vecs[9] = {
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668654, lbl_eu_80668654}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668650, lbl_eu_80668650},
    };
    nw4r::lyt::Pane* pane =
        (nw4r::lyt::Pane*)((SArts3CObj*)((SArts3150CDriver*)self->field_0x00)->field_0x10)
            ->v13((void*)tmp.w[self->field_0x20], 1);
    func_801375A0(&pos, pane);
    pane = (nw4r::lyt::Pane*)((SArts3CObj*)((SArts3150CDriver*)self->field_0x00)->field_0x10)
               ->v13((void*)(lbl_eu_8050AC70 + 0xA6), 1);
    pos.x *= pane->GetScale().x;
    func_801D2150((nw4r::lyt::Pane*)((SArts3150CDriver*)self->field_0x08)->field_0x10, &pos);
    func_80231848((CMenuArtsSet*)((SArts3150CDriver*)self->field_0x08)->field_0x10,
                  &vecs[self->field_0x20]);
}
#pragma optimize_for_size off

// Run the layout-out animation; when the busy flag (0x23) is clear, drive
// both sub-panels via vtable slot 0x2C (v11) then bump the 0x24 state.
// Layout-in driver twin of func_8023150C's shape (0x08/0x0C/0x10 fields).
// noinline: retail keeps real bl calls from the state machine.
__declspec(noinline) void func_80231480(SArts31480* self) {
    float duration = lbl_eu_80668648;
    func_80137444(self->field_0x0C, duration);
    if (self->field_0x23 == 0) {
        self->field_0x08->v11(self->field_0x0C, 0);
        self->field_0x08->v11(self->field_0x10, 1);
        self->field_0x10->field_0x10 = lbl_eu_8066864C;
        self->field_0x24 = 1;
    }
}

// Twin driver: run the AnimTransform at 0x10; when the frame check reports
// the animation finished, drive both sub-panes (v11 at +0x2C), reset the
// 0x0C float, show the panes, and bump the 0x23/0x24 state.
__declspec(noinline) void func_8023150C(SArts3150C* self) {
    float duration = lbl_eu_80668648;
    if (func_80137444(self->field_0x10, duration) != 0) {
        self->field_0x08->v11(self->field_0x10, 0);
        self->field_0x08->v11(self->field_0x0C, 1);
        self->field_0x0C->field_0x10 = lbl_eu_8066864C;
        func_80124270(self->field_0x08->field_0x10, 0);
        func_80124270(self->field_0x14->field_0x10, 1);
        self->field_0x24 = 2;
        self->field_0x23 = 1;
    }
}

// Same driver as func_80231480 but for the offset-0x14 sub-object; the
// 0x24 state is bumped to 3 instead of 1.
__declspec(noinline) void func_802315BC(SArts315BC* self) {
    float duration = lbl_eu_80668648;
    func_80137444(self->field_0x18, duration);
    if (self->field_0x23 == 0) {
        self->field_0x14->v11(self->field_0x18, 0);
        self->field_0x14->v11(self->field_0x1C, 1);
        self->field_0x1C->field_0x10 = lbl_eu_8066864C;
        self->field_0x24 = 3;
    }
}

// Layout-out completion driver for the mSubObj148+0x2C twin: run the
// AnimTransform at 0x1C; when finished, drive both panes via the 0x14
// driver's v11 slot, reset the 0x18 float, show/hide via the 0x08/0x14 pane
// pointees, and mark state 0x24=0 / busy 0x23=1.
__declspec(noinline) void func_80231648(SArts31648* self) {
    float duration = lbl_eu_80668648;
    if (func_80137444(self->field_0x1C, duration) != 0) {
        self->field_0x14->v11(self->field_0x1C, 0);
        self->field_0x14->v11(self->field_0x18, 1);
        self->field_0x18->field_0x10 = lbl_eu_8066864C;
        func_80124270(self->field_0x08->field_0x10, 1);
        func_80124270(self->field_0x14->field_0x10, 0);
        self->field_0x24 = 0;
        self->field_0x23 = 1;
    }
}

// Writes the 0x44/0x48 floats (pane scale view) of the object passed in.
// noinline + C linkage: func_802316F8 keeps a real bl to the retail symbol.
extern "C" __declspec(noinline) void func_80231848(CMenuArtsSet* self, const SArtsVec2* src) {
    self->mField44 = src->x;
    self->mField48 = src->y;
}

#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_8023185C(SArts313E0* self) {
    nw4r::math::VEC3 pos;
    SArts316F8Block tmp;
    tmp = lbl_eu_8050ABD8;
    SArtsVec2 vecs[9] = {
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668654, lbl_eu_80668654}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668650, lbl_eu_80668650}, {lbl_eu_80668650, lbl_eu_80668650},
        {lbl_eu_80668650, lbl_eu_80668650},
    };
    nw4r::lyt::Pane* pane =
        (nw4r::lyt::Pane*)((SArts3CObj*)((SArts3150CDriver*)self->field_0x00)->field_0x10)
            ->v13((void*)tmp.w[self->field_0x21], 1);
    func_801375A0(&pos, pane);
    pane = (nw4r::lyt::Pane*)((SArts3CObj*)((SArts3150CDriver*)self->field_0x00)->field_0x10)
               ->v13((void*)(lbl_eu_8050AC70 + 0xA6), 1);
    pos.x *= pane->GetScale().x;
    func_801D2150((nw4r::lyt::Pane*)self->field_0x14->field_0x10, &pos);
    func_80231848((CMenuArtsSet*)self->field_0x14->field_0x10, &vecs[self->field_0x21]);
}
#pragma optimize_for_size off

extern "C" __declspec(noinline) void __ct__802319AC(SArts319AC* self, u32 a, u32 b, u32 c) {
    self->field_0x00 = a;
    self->field_0x04 = b;
    self->field_0x08 = 0;
    self->field_0x0C = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1C = 0;
    self->field_0x20 = 0;
    self->field_0x21 = 0;
    self->field_0x22 = 0;
    self->field_0x23 = 1;
    self->field_0x24 = 0;
    self->field_0x25 = 2;
    self->field_0x26 = 0;
    self->field_0x28 = c;
    self->field_0x12C = 0;
    self->field_0x12D = 0;
    self->field_0x12E = 1;
}

extern "C" void* __dt__80231A08(void* self, int flags) {
    if (self && flags > 0) {
        ::operator delete(self);
    }
    return self;
}

// Layout-pair init twin of func_802304C4 with different label strings
// (+0x202/+0x210): after parking both panes, reset the pane translate of
// each layout's SArts3CObj to (0,0) via func_80231848, then refresh the
// cursor drivers (func_80232B88).
void func_80231A48(SArts304C4* self) {
    func_80136E84((nw4r::lyt::Layout**)&self->field_0x08, self->field_0x04,
                  lbl_eu_8050AC70 + 0x109);
    func_80136F08((nw4r::lyt::Layout*)self->field_0x08, &self->field_0x0C,
                  self->field_0x04, lbl_eu_8050AC70 + 0x122);
    func_80136F08((nw4r::lyt::Layout*)self->field_0x08, &self->field_0x10,
                  self->field_0x04, lbl_eu_8050AC70 + 0x140);
    self->field_0x08->v11(self->field_0x10, 0);
    self->field_0x08->v11(self->field_0x0C, 1);
    self->field_0x08->v14(0);
    func_80124270(self->field_0x08->field_0x10->v13(lbl_eu_8050AC70 + 0x202, 1), 0);
    func_80124270(self->field_0x08->field_0x10->v13(lbl_eu_8050AC70 + 0x210, 1), 0);
    SArtsVec2 v1 = {lbl_eu_80668658, lbl_eu_80668658};
    func_80231848((CMenuArtsSet*)self->field_0x08->field_0x10, &v1);
    func_80136E84((nw4r::lyt::Layout**)&self->field_0x14, self->field_0x04,
                  lbl_eu_8050AC70 + 0x178);
    func_80136F08((nw4r::lyt::Layout*)self->field_0x14, &self->field_0x18,
                  self->field_0x04, lbl_eu_8050AC70 + 0x190);
    func_80136F08((nw4r::lyt::Layout*)self->field_0x14, &self->field_0x1C,
                  self->field_0x04, lbl_eu_8050AC70 + 0x1AD);
    self->field_0x14->v11(self->field_0x1C, 0);
    self->field_0x14->v11(self->field_0x18, 1);
    self->field_0x14->v14(0);
    func_80124270(self->field_0x14->field_0x10, 0);
    SArtsVec2 v2 = {lbl_eu_80668658, lbl_eu_80668658};
    func_80231848((CMenuArtsSet*)self->field_0x14->field_0x10, &v2);
    func_80232B88((SArts327B0*)self);
}

// Release both SArts2FObj pointees via their vtable slot 0x08 (v2), then
// null each field (retail keeps the redundant null-check branch).
// C-linkage + noinline so func_80233760 emits a real bl.
extern "C" __declspec(noinline) void func_80231C30(SArts3066C* self) {
    if (self->field_0x08 != 0) {
        if (self->field_0x08 != 0) {
            self->field_0x08->v2(1);
        }
        self->field_0x08 = 0;
    }
    if (self->field_0x14 != 0) {
        if (self->field_0x14 != 0) {
            self->field_0x14->v2(1);
        }
        self->field_0x14 = 0;
    }
}

// Same-TU phase handlers (defined below); retail keeps real bl calls.
extern "C" __declspec(noinline) void func_80232910(SArts31480* self);
extern "C" __declspec(noinline) void func_80232A4C(SArts315BC* self);

// State-machine advance for the +0x2C twin of func_802306F0: while enabled
// (0x22), run the current phase handler selected by 0x24 (layout-in/out pair,
// func_80232910/func_8023299C), then the secondary one selected by 0x25
// (func_80232A4C/func_80232AD8), and finally reset both +0x38 pointees via
// their vtable slot (arg 0).
void func_80231CB4(SArts306F0* self) {
    if (self->field_0x22 != 0) {
        switch (self->field_0x24) {
            case 0: func_80232910((SArts31480*)self); break;
            case 1: func_8023299C((SArts3150C*)self); break;
        }
        switch (self->field_0x25) {
            case 2: func_80232A4C((SArts315BC*)self); break;
            case 3: func_80232AD8((SArts31648*)self); break;
        }
        self->field_0x08->v12(0);
        self->field_0x14->v12(0);
    }
}

extern "C" __declspec(noinline) void func_80231D68(SArtsDrawBox* self, nw4r::lyt::DrawInfo* info) {
    if (self->field_0x22 != 0 && self->field_0x12E != 0) {
        func_80137038(self->mLayout08, info, 0, 1);
        func_80137038(self->mLayout14, info, 0, 1);
    }
}

// Cursor-driver refresh helpers (defined below in this TU; C-linkage retail
// names). Declared here because func_80231DD0 / func_80232000 call them.
extern "C" void func_80232B88(SArts327B0* self);
extern "C" void func_80232C78(SArts327B0* self);

// Scroll the arts-table cursor up one entry: decrement the 0x20 row cursor,
// and when it wraps past 0 borrow into the 0x21 page offset. When both wrap,
// jump to the previous 5-row page (0x20 = 4, 0x21 = count-5) or, for counts
// below 5, park the cursor with a saturating 0x20 = count-1. Then refresh the
// scrollbar and both cursor drivers.
extern "C" __declspec(noinline) void func_80231DD0(SArts322BC* self) {
    u8 a = self->field_0x20 - 1;
    self->field_0x20 = a;
    if ((s8)a < 0) {
        self->field_0x20 = 0;
        int b = self->field_0x21 - 1;
        self->field_0x21 = b;
        if ((s8)(u8)b < 0) {
            u8 c = self->field_0x12C;
            if (c > 5) {
                self->field_0x20 = 4;
                self->field_0x21 = c - 5;
            } else {
                self->field_0x21 = 0;
                // Saturating decrement: MWCC's addic/subi/subfe/andc idiom.
                self->field_0x20 = c > 0 ? c - 1 : 0;
            }
        }
    }
    u16 val = (u16)(s8)self->field_0x21;
    u8* sb = self->field_0x28;
    func_801F3850(sb, val);
    func_80232B88((SArts327B0*)self);
    func_80232C78((SArts327B0*)self);
}

extern "C" __declspec(noinline) void func_80231E8C(SArts322BC* self) {
    u8 count = self->field_0x12C;
    if (count >= 5) {
        // Row cursor forward; on overflow park at row 4 and step the page
        // offset, resetting both when the page runs past count-5.
        u8 a = self->field_0x20 + 1;
        self->field_0x20 = a;
        if ((s8)a >= 5) {
            self->field_0x20 = 4;
            u8 b = self->field_0x21 + 1;
            self->field_0x21 = b;
            if ((s8)b > (s32)(count - 5)) {
                self->field_0x20 = 0;
                self->field_0x21 = 0;
            }
        }
    } else {
        u8 a = self->field_0x20 + 1;
        self->field_0x20 = a;
        if ((s8)a >= (s32)count) {
            self->field_0x20 = 0;
            self->field_0x21 = 0;
        }
    }
    // Named in retail evaluation order: cursor byte first, scrollbar second.
    u16 val = (u16)(s8)self->field_0x21;
    u8* sb = self->field_0x28;
    func_801F3850(sb, val);
    func_80232B88((SArts327B0*)self);
    func_80232C78((SArts327B0*)self);
}

// Cursor-driver refresh helpers (defined below in this TU; C-linkage retail
// names). Declared here because func_80231F60 calls them.
extern "C" void func_80232B88(SArts327B0* self);
extern "C" void func_80232C78(SArts327B0* self);

// Scroll the arts-table cursor up one 5-row page: when the 0x12C count is
// below 5 the cursor is cleared entirely; otherwise step 0x21 back by 5 and
// wrap a negative offset into the previous page via 0x20. Then refresh the
// scrollbar and both cursor drivers.
extern "C" __declspec(noinline) void func_80231F60(SArts322BC* self) {
    if (self->field_0x12C >= 5) {
        u8 a = self->field_0x21 - 5;
        self->field_0x21 = a;
        if ((s8)a < 0) {
            u8 t = a + 4;
            self->field_0x20 = t;
            self->field_0x21 = 0;
            if ((s8)self->field_0x20 < 0) {
                self->field_0x20 = 0;
            }
        }
    } else {
        self->field_0x20 = 0;
        self->field_0x21 = 0;
    }
    // Named in retail evaluation order: cursor byte first (lands in the
    // arg-2 register), scrollbar pointer second.
    u16 c = self->field_0x21;
    u8* sb = self->field_0x28;
    func_801F3850(sb, c);
    func_80232B88((SArts327B0*)self);
    func_80232C78((SArts327B0*)self);
}

// Scroll the arts-table cursor down one 5-row page: when the 0x12C count is
// at least 5, step the 0x21 page offset forward by 5 and wrap any overshoot
// back into the 0x20 row cursor (clamped at 4); otherwise the cursor is
// simply cleared. Then refresh the scrollbar and both cursor drivers.
extern "C" __declspec(noinline) void func_80232000(SArts322BC* self) {
    u8 c = self->field_0x12C;
    if (c >= 5) {
        u8 a = self->field_0x21 + 5;
        self->field_0x21 = a;
        if ((s8)a > (s32)(c - 5)) {
            u8 t = (u8)(a - (c - 5));
            self->field_0x20 = t;
            self->field_0x21 = (u8)(c - 5);
            if ((s8)t >= 5) self->field_0x20 = 4;
        }
    } else {
        self->field_0x20 = c - 1;
        self->field_0x21 = 0;
        if ((s8)(u8)(c - 1) < 0) self->field_0x20 = 0;
    }
    u16 val = (u16)(s8)self->field_0x21;
    u8* sb = self->field_0x28;
    func_801F3850(sb, val);
    func_80232B88((SArts327B0*)self);
    func_80232C78((SArts327B0*)self);
}

#pragma push
#pragma optimize_for_size on
// Rebuild the arts table for a character: clear the cursor and row count,
// fetch the character's arts-availability list (copied per-character block),
// then for each available art append a 16-byte row to the 0x2C table with
// its id/name/flag fields, finally refreshing the scrollbar and cursors.
extern "C" __declspec(noinline) void func_802320C0(SArts322BC* self, u8 arg) {
    self->field_0x20 = 0;
    self->field_0x21 = 0;
    self->field_0x26 = arg;
    self->field_0x12C = 0;
    SArtsManagerRoot* root = (SArtsManagerRoot*)func_8009EC9C(arg);
    SArtsSubDElem* elems = &root->mElemsE8[0];
    SArts320C0Block tmp;
    tmp = lbl_eu_8050ABFC;
    u32 charIdx = func_800A32BC(root);
    SArts320C0Entry* p = (SArts320C0Entry*)&tmp + arg;
    // Demo/default override: first character gets a fixed starting art.
    if (lbl_eu_806628A8[0] == 0 && arg == 1) {
        p->id = 3;
    }
    for (u8 i = 0; i < p->count; i++) {
        u32 artId = func_8013600C(lbl_eu_8050AC70 + 0x1C8, lbl_eu_8050AC70 + 0x1FE,
                                  p->id + i);
        if (func_801F9268((u8*)elems, charIdx, (u8)artId) == 0) {
            continue;
        }
        const u8* elemBytes = (const u8*)&elems[charIdx];
        u16 w = *(const u16*)&elemBytes[(u8)artId << 1];
        if (*(const u8*)&w == 0) {
            continue;
        }
        u8 n = self->field_0x12C++;
        u8* row = &self->mTable[n << 4];
        row[0] = p->id + i;
        row[1] = artId;
        row[2] = *(const u8*)&w;
        *(u16*)&row[4] = func_8013606C(lbl_eu_8050AC70 + 0x1C8, lbl_eu_8050AC70 + 0x1D0,
                                       p->id + i);
        *(u16*)&row[6] = func_8013606C(lbl_eu_8050AC70 + 0x1C8, lbl_eu_8050AC70 + 0x1D5,
                                       p->id + i);
        *(u32*)&row[0xC] =
            (u32)func_80136190((char*)lbl_eu_8050AC70 + 0x1C8, lbl_eu_8050AC70 + 0x5B,
                               p->id + i);
        const u32 lo = ((const u8*)&w)[1];
        row[8] = ((lo >> 7) == 0);
        row[9] = ((lo >> 6) == 0);
    }
    func_801F36BC(self->field_0x28, 5, self->field_0x12C);
    func_801F3850(self->field_0x28, (u16)(s8)self->field_0x21);
    func_80232C78((SArts327B0*)self);
    func_80232B88((SArts327B0*)self);
}
#pragma pop

extern "C" __declspec(noinline) u8 func_802322BC(SArts322BC* self) {
    if (self->field_0x12C != 0) {
        s32 off = (self->field_0x21 + self->field_0x20) << 4;
        return self->mTable[off];
    }
    return 0;
}

extern "C" __declspec(noinline) u8 func_802322F4(SArts322BC* self, int key) {
    u8 count = self->field_0x12C;
    if (count == 0) return 0;
    if (key == -1) {
        int o = (((int)self->field_0x21 + (int)self->field_0x20) << 4);
        return *(self->mTable + o + 2);
    }
    for (u8 i = 0; i < count; i++) {
        if ((int)self->mTable[i * 16] == key) {
            return self->mTable[i * 16 + 2];
        }
    }
    return 0;
}

// Arts-table percent query: for the current cursor row (key == -1) or the
// row whose id byte matches key, compute a percentage from the row's value
// byte (func_8013606C string-table lookup, +1) times the arts count byte
// (func_8013600C lookup keyed by the buffer formatted from the row's
// second byte), divided by 100 when positive.
#pragma optimize_for_size on
extern "C" __declspec(noinline) u16 func_80232370(SArts322BC* self, int key) {
    u8 count = self->field_0x12C;
    u16 result = 0;
    if (count != 0) {
        if (key == -1) {
            char buf[0x20];
            u8* row = &self->mTable[((int)self->field_0x21 + (int)self->field_0x20) * 16];
            u16 v = func_8013606C(lbl_eu_8050AC70 + 0x21E, lbl_eu_8050AC70 + 0x22D, row[2] + 1);
            sprintf(buf, lbl_eu_8050AC70 + 0x233, row[1]);
            u32 raw = func_8013600C(lbl_eu_8050AC70 + 0x23C, buf, self->field_0x26);
            s32 prod = (int)(u16)v * (int)(u8)raw;
            if (prod > 0) prod /= 100;
            result = (u16)prod;
        } else {
            char buf[0x20];
            for (u8 i = 0; i < count; i++) {
                u8* row = &self->mTable[i * 16];
                if (key == (int)row[0]) {
                    u16 v = func_8013606C(lbl_eu_8050AC70 + 0x21E, lbl_eu_8050AC70 + 0x22D, row[2] + 1);
                    sprintf(buf, lbl_eu_8050AC70 + 0x233, row[1]);
                    u32 raw = func_8013600C(lbl_eu_8050AC70 + 0x23C, buf, self->field_0x26);
                    s32 prod = (int)(u16)v * (int)(u8)raw;
                    if (prod > 0) prod /= 100;
                    result = (u16)prod;
                    break;
                }
            }
        }
    }
    return result;
}
#pragma optimize_for_size off

void func_802324C4(){}

// Arts-table row availability check: for key -1 use the current cursor row
// ((0x21 + 0x20) << 4), else scan for the row whose id byte equals key.
// A row qualifies when its state byte (row[2]) is 4 with a clear flag at
// row[8], or 7 with a clear flag at row[9].
int func_80232638(SArts322BC* self, int key) {
    if (key == -1) {
        s32 off = ((int)self->field_0x21 + (int)self->field_0x20) << 4;
        u8 t = self->mTable[off + 2];
        if (t == 4) {
            if (self->mTable[off + 8] != 0) return 0;
            return 1;
        }
        if (t == 7) {
            if (self->mTable[off + 9] != 0) return 0;
            return 1;
        }
        return 0;
    }
    u8 count = self->field_0x12C;
    for (u8 i = 0; i < count; i++) {
        s32 off = i << 4;
        if ((int)self->mTable[off] == key) {
            u8 t = self->mTable[off + 2];
            if (t == 4) {
                if (self->mTable[off + 8] != 0) return 0;
                return 1;
            }
            if (t == 7) {
                if (self->mTable[off + 9] != 0) return 0;
                return 1;
            }
            return 0;
        }
    }
    return 0;
}

extern "C" __declspec(noinline) int func_8023270C(SArts3270C* self) {
    int r = 0;
    if (self->field_0x20 == 0 && self->field_0x21 == 0) {
        r = 1;
    }
    return r;
}

extern "C" void func_80232B88(SArts327B0* self);
extern "C" void func_80232C78(SArts327B0* self);

extern "C" __declspec(noinline) int func_80232734(SArts322BC* self) {
    u8 count = self->field_0x12C;
    if (count > 5) {
        s32 r = 0;
        if (self->field_0x20 == 4 && (s8)self->field_0x21 == (s32)count - 5) {
            r = 1;
        }
        return r;
    }
    // Saturating decrement of the page cursor (count-1 clamped at 0).
    s32 last = count != 0 ? count - 1 : 0;
    s32 r = 0;
    if ((s8)self->field_0x20 == last && (s8)self->field_0x21 == 0) {
        r = 1;
    }
    return r;
}

extern "C" __declspec(noinline) void func_802327B0(SArts327B0* self) {
    extern void func_801F3850(void*, u32);
    self->field_0x20 = 0;
    self->field_0x21 = 0;
    func_801F3850(self->field_0x28, 0);
    func_80232B88(self);
    func_80232C78(self);
}

// Clamp the arts-table cursor: for counts above 5 the cursor sits at row 4
// with a 0x21 offset; otherwise 0x21 is cleared and 0x20 is the count
// saturated at 0. Then refresh the scrollbar and both cursor drivers.
extern "C" __declspec(noinline) void func_80232800(SArts322BC* self) {
    u8 count = self->field_0x12C;
    if (count > 5) {
        self->field_0x20 = 4;
        self->field_0x21 = count - 5;
    } else {
        self->field_0x21 = 0;
        self->field_0x20 = count - (count > 0);
    }
    func_801F3850(self->field_0x28, (u16)(s8)self->field_0x21);
    func_80232B88((SArts327B0*)self);
    func_80232C78((SArts327B0*)self);
}

// Toggle the 0x12D busy flag and drive both pointees: when clearing, the
// first pane is shown and the second hidden; when setting, the reverse.
// C-linkage + noinline: func_80233F78 keeps a real bl (retail symbol).
extern "C" __declspec(noinline) void func_80232888(SArts32888* self) {
    if (self->field_0x12D != 0) {
        self->field_0x12D = 0;
        func_80124270(self->field_0x08->field_0x10, 1);
        func_80124270(self->field_0x14->field_0x10, 0);
    } else {
        self->field_0x12D = 1;
        func_80124270(self->field_0x08->field_0x10, 0);
        func_80124270(self->field_0x14->field_0x10, 1);
    }
}

// Twin of func_80231480 (same 0x08/0x0C/0x10 driver layout).
extern "C" __declspec(noinline) void func_80232910(SArts31480* self) {
    float duration = lbl_eu_80668648;
    func_80137444(self->field_0x0C, duration);
    if (self->field_0x23 == 0) {
        self->field_0x08->v11(self->field_0x0C, 0);
        self->field_0x08->v11(self->field_0x10, 1);
        self->field_0x10->field_0x10 = lbl_eu_8066864C;
        self->field_0x24 = 1;
    }
}

// Twin of func_8023150C (identical object view): run the AnimTransform at
// 0x10; when finished, drive both panes via the 0x08 driver, reset the 0x0C
// float, show the 0x14 pane, and mark state 0x24=2 / busy 0x23=1.
extern "C" __declspec(noinline) void func_8023299C(SArts3150C* self) {
    float duration = lbl_eu_80668648;
    if (func_80137444(self->field_0x10, duration) != 0) {
        self->field_0x08->v11(self->field_0x10, 0);
        self->field_0x08->v11(self->field_0x0C, 1);
        self->field_0x0C->field_0x10 = lbl_eu_8066864C;
        func_80124270(self->field_0x08->field_0x10, 0);
        func_80124270(self->field_0x14->field_0x10, 1);
        self->field_0x24 = 2;
        self->field_0x23 = 1;
    }
}

// Twin of func_802315BC (offset-0x14 driver layout, state bump to 3).
extern "C" __declspec(noinline) void func_80232A4C(SArts315BC* self) {
    float duration = lbl_eu_80668648;
    func_80137444(self->field_0x18, duration);
    if (self->field_0x23 == 0) {
        self->field_0x14->v11(self->field_0x18, 0);
        self->field_0x14->v11(self->field_0x1C, 1);
        self->field_0x1C->field_0x10 = lbl_eu_8066864C;
        self->field_0x24 = 3;
    }
}

// Twin of func_80231648 (identical body, separate retail symbol).
extern "C" __declspec(noinline) void func_80232AD8(SArts31648* self) {
    float duration = lbl_eu_80668648;
    if (func_80137444(self->field_0x1C, duration) != 0) {
        self->field_0x14->v11(self->field_0x1C, 0);
        self->field_0x14->v11(self->field_0x18, 1);
        self->field_0x18->field_0x10 = lbl_eu_8066864C;
        func_80124270(self->field_0x08->field_0x10, 1);
        func_80124270(self->field_0x14->field_0x10, 0);
        self->field_0x24 = 0;
        self->field_0x23 = 1;
    }
}

// Refresh the arts-table cursor: copy the 5-word label block from
// lbl_eu_8050AC4C, look up the pane for the current 0x20 cursor entry, scale
// its translate by the 0xA6 label pane's scale, then position both the
// 0x08/0x14 sub-panes onto it.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80232B88(SArts327B0* self) {
    nw4r::math::VEC3 pos;
    SArtsB88Block tmp;
    tmp = lbl_eu_8050AC4C;
    nw4r::lyt::Pane* pane =
        (nw4r::lyt::Pane*)((SArts3CObj*)self->field_0x00->field_0x10)
            ->v13((void*)tmp.w[self->field_0x20], 1);
    func_801375A0(&pos, pane);
    pane = (nw4r::lyt::Pane*)((SArts3CObj*)self->field_0x00->field_0x10)
               ->v13((void*)(lbl_eu_8050AC70 + 0xA6), 1);
    pos.x *= pane->GetScale().x;
    func_801D2150((nw4r::lyt::Pane*)self->field_0x08->field_0x10, &pos);
    func_801D2150((nw4r::lyt::Pane*)self->field_0x14->field_0x10, &pos);
}
#pragma optimize_for_size off

extern "C" __declspec(noinline) void func_80232C78(SArts327B0* self) { func_80124270(self, 1); }

// CArtsList constructor (retail symbol __ct__CArtsList). Stores the retail
// vtable, builds the sub-objects, then re-copies the CScrollBar/CArtsInfo/
// CSysWin bodies from stack temps (each copy skips the vtable: a 16-byte
// mem-region copy via __ct__UnkClass_8011C974 plus memberwise fields), and
// finally primes the shared arts-mode byte table. Temp buffers are declared
// largest-first so MWCC assigns them the retail stack slots (0x88/0x48/0x8).
// optimize_for_size: retail's 3-reg prologue uses the _savegpr_29 form
// (-O4,s signature; plain -O4,p emits individual stw).
#pragma optimize_for_size on
CArtsList* __ct__CArtsList(CArtsList* self) {
    u8 tempInfo[0x74];
    u8 tempSb[0x40];
    u8 tempW[0x3C];

    self->mVtbl = (void*)lbl_eu_80536908;
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1C = 0;
    self->field_0x20 = 0;
    self->field_0x24 = 0;
    self->field_0x28 = 0;
    self->field_0x2C = 0;
    self->field_0x30 = 0;
    self->field_0x31 = 1;
    __ct__CScrollBar(&self->mScrollBar, 0);
    __ct__CArtsInfo(&self->mSubObj74);
    __ct__CSysWin(&self->mSysWinE8, 0);
    __ct__8022FA58((SArtsSub8022FA58*)&self->mSubObj124, 0, 0);
    __ct__8023042C(&self->mSubObj148, 0, 0);
    __ct__802319AC(&self->mSubObj174, 0, 0, 0);
    self->field_0x2A4 = 0;
    self->field_0x2A5 = 0;
    self->field_0x2A6 = 0;

    // Temp CScrollBar(4): copy its body into the member (re-inits the
    // direction byte, which the member ctor above set to 0).
    __ct__CScrollBar((CScrollBar*)tempSb, 4);
    __ct__UnkClass_8011C974(&self->mScrollBar.mMemRegion,
                            &((CScrollBar*)tempSb)->mMemRegion);
    self->mScrollBar.mFileHandle = ((CScrollBar*)tempSb)->mFileHandle;
    self->mScrollBar.mAccessor = ((CScrollBar*)tempSb)->mAccessor;
    self->mScrollBar.mLayout = ((CScrollBar*)tempSb)->mLayout;
    self->mScrollBar.mAnimTransform = ((CScrollBar*)tempSb)->mAnimTransform;
    self->mScrollBar.mReady = ((CScrollBar*)tempSb)->mReady;
    self->mScrollBar.mVisible = ((CScrollBar*)tempSb)->mVisible;
    self->mScrollBar.mState = ((CScrollBar*)tempSb)->mState;
    self->mScrollBar.mActive = ((CScrollBar*)tempSb)->mActive;
    self->mScrollBar.mAnimOffset = ((CScrollBar*)tempSb)->mAnimOffset;
    self->mScrollBar.mScrollPosY = ((CScrollBar*)tempSb)->mScrollPosY;
    self->mScrollBar.mScrollRatio = ((CScrollBar*)tempSb)->mScrollRatio;
    self->mScrollBar.mThumbHeight = ((CScrollBar*)tempSb)->mThumbHeight;
    self->mScrollBar.mContentHeight = ((CScrollBar*)tempSb)->mContentHeight;
    self->mScrollBar.mDirection = ((CScrollBar*)tempSb)->mDirection;
    __dt__10CScrollBarFv((CScrollBar*)tempSb, -1);

    // Temp CArtsInfo: copy its body (after the vtable) into the member.
    __ct__CArtsInfo(tempInfo);
    __ct__UnkClass_8011C974(&self->mSubObj74.data[4], tempInfo + 4);
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x14 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x14;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x18 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x18;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x1C =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x1C;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x20 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x20;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x24 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x24;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x28 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x28;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x2C =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x2C;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x30 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x30;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x34 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x34;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x38 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x38;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x3C =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x3C;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x40 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x40;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x44 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x44;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x48 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x48;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x49 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x49;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x4C =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x4C;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x50 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x50;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x54 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x54;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x55 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x55;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x56 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x56;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x58 =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x58;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->field_0x5A =
            ((SArtsInfoBody*)(tempInfo + 4))->field_0x5A;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->mCursor.field_0x04 =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x04;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->mCursor.field_0x08 =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x08;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->mCursor.field_0x0C =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x0C;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->mCursor.field_0x10 =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x10;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->mCursor.field_0x14 =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x14;
        ((SArtsInfoBody*)((u8*)&self->mSubObj74 + 4))->mCursor.field_0x15 =
            ((SArtsInfoBody*)(tempInfo + 4))->mCursor.field_0x15;
    __dt__9CArtsInfoFv(tempInfo, -1);

    // Temp CSysWin(0): copy its body (after the vtable) into the member.
    __ct__CSysWin(tempW, 0);
    __ct__UnkClass_8011C974(&self->mSysWinE8.mMemRegion, &((CSysWin*)tempW)->mMemRegion);
    self->mSysWinE8.mFileHandle = ((CSysWin*)tempW)->mFileHandle;
    self->mSysWinE8.mTagProcessor = ((CSysWin*)tempW)->mTagProcessor;
    self->mSysWinE8.mArcAccessor = ((CSysWin*)tempW)->mArcAccessor;
    self->mSysWinE8.mLayout = ((CSysWin*)tempW)->mLayout;
    self->mSysWinE8.mAnimTrans = ((CSysWin*)tempW)->mAnimTrans;
    self->mSysWinE8.field_28 = ((CSysWin*)tempW)->field_28;
    self->mSysWinE8.field_2C = ((CSysWin*)tempW)->field_2C;
    self->mSysWinE8.field_30 = ((CSysWin*)tempW)->field_30;
    self->mSysWinE8.field_34 = ((CSysWin*)tempW)->field_34;
    self->mSysWinE8.field_35 = ((CSysWin*)tempW)->field_35;
    self->mSysWinE8.field_36 = ((CSysWin*)tempW)->field_36;
    self->mSysWinE8.field_37 = ((CSysWin*)tempW)->field_37;
    self->mSysWinE8.field_38 = ((CSysWin*)tempW)->field_38;
    self->mSysWinE8.field_39 = ((CSysWin*)tempW)->field_39;
    __dt__7CSysWinFv(tempW, -1);

    lbl_eu_806628A8[0] = 1;
    return self;
}
#pragma optimize_for_size off

// Retail dtor: stmw/lmw frame via optimize_for_size; the four member dtors
// (+0xE8 CSysWin, +0x74 CArtsInfo, +0x34 CScrollBar, +0x4 UnkClass_8045F564)
// are emitted by the auto-generated body now that the member classes declare
// their destructors.
#pragma push
#pragma optimize_for_size on
CArtsList::~CArtsList() {}
#pragma pop

void func_8023352C(CArtsList* self) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->field_0x14 = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)handle, lbl_eu_8050AC70 + 0x2ba, self, 0, 0);
    func_801F34F4(&self->mScrollBar);
    func_80235814(&self->mSubObj74);
    ((SArtsWinE8*)&self->mSysWinE8)->v32();
}

void func_8023359C(CMenuArtsSet* self) {
    if (self->field_0x28 != 0 && self->field_0x2C != 0) {
        switch (self->field_0x2C) {
            case 1: func_80234EB8(self); break;
            case 2: func_80234F7C(self); break;
            case 4: func_80234FDC(self); break;
            case 5: func_8023506C((SArts3506C*)self); break;
            case 6: func_802350B8(self); break;
        }
        self->field_0x1C->v14(0);
        func_801F3540(self->field_0x34);
        func_8023587C(&self->mSubObj74);
        func_8022B748(&self->mSubObjE8);
        func_8022FDF4((SArts2FDF4*)&self->mSubObj124);
        func_802306F0((SArts306F0*)&self->mSubObj148);
        func_80231CB4((SArts306F0*)((u8*)&self->mSubObj148 + 0x2C));
    }
}

// Draw the arts-selection menu: when the menu is active (0x28) and a state
// machine is running (0x2C), draw the main layout, the draw object at 0x34,
// then - when the arts info and syswin are idle and the list cursor is not
// armed (0x139 clear) - mark the 0x170/0x2A2 flags. The 0x12C layout is
// drawn when the 0x139 cursor byte is set, followed by the 0x148/0x174
// box pair, the arts info, and the syswin.
#pragma optimize_for_size on
void func_80233674(CMenuArtsSet* self, nw4r::lyt::DrawInfo* info) {
    if (self->field_0x28 == 0) return;
    if (self->field_0x2C == 0) return;
    func_80137038((nw4r::lyt::Layout*)self->field_0x1C, info, 0, 1);
    func_801F35B0(&self->field_0x34, info);
    u8 v = 0;
    if (CSysWin_getUnk34(&self->mSubObjE8) == 0) {
        if (self->mSubObj124.field_0x15 == 0) {
            if (func_80235F50(&self->mSubObj74) == 0) {
                v = 1;
            }
        }
    }
    self->mSubObj148.field_0x28 = v;
    self->field_0x2A2 = v;
    if (self->mSubObj124.field_0x15 != 0) {
        func_80137038((nw4r::lyt::Layout*)self->mSubObj124.field_0x08, info, 0, 1);
    }
    func_802307A4((SArtsDrawBox*)&self->mSubObj148, info);
    func_80231D68((SArtsDrawBox*)((u8*)&self->mSubObj148 + 0x2C), info);
    func_80235958(&self->mSubObj74, info);
    func_8022B7C8(&self->mSubObjE8, info);
}
#pragma optimize_for_size off

// Teardown: release the file handle, reset the ready flag, drop the three
// sub-objects, release the field_0x1C pane pair (nested null guards reproduce
// retail's double beq on one CR0 test - same D2-inline artifact as COption),
// release the arc accessor, and clean up the mem region / scroll bar /
// info / syswin sub-objects.
void func_80233760(CMenuArtsSet* self) {
    func_801390E0(&self->field_0x14);
    self->field_0x28 = 0;
    func_8022FD9C((SArts2FDF4*)&self->mSubObj124);
    func_8023066C((SArts3066C*)&self->mSubObj148);
    func_80231C30((SArts3066C*)((u8*)&self->mSubObj148 + 0x2C));
    if (self->field_0x1C != 0) {
        if (self->field_0x1C != 0) {
            self->field_0x1C->v0(1);
        }
        self->field_0x1C = 0;
    }
    func_80139124(self->field_0x18);
    self->field_0x18 = 0;
    self->mMemRegion.func_8045F778();
    func_801F35DC(&self->field_0x34);
    func_802359CC(&self->mSubObj74);
    func_8022B7F4(&self->mSubObjE8);
}

u8 func_8023380C(CArtsList* self) {
    if (CScrollBar_isVisible(&self->mScrollBar) == 0) return 0;
    if (func_80235A90(&self->mSubObj74) == 0) return 0;
    if (CSysWin_isReady(&self->mSysWinE8) != 0) {
        return self->field_0x30;
    }
    return 0;
}

u8 CMenuArtsSet::func_80233880() { return mField31; }

// True while the arts menu is busy: either the 0x2A1/0x16F flags are set,
// the CArtsInfo or CSysWin sub-object is active, or the raw 0x2A6 flag.
u8 func_80233888(SArts33888* self) {
    if (self->field_0x2A1 != 0) return 1;
    if (self->field_0x16F != 0) return 1;
    if (func_80235F50(&self->field_0x74) != 0) return 1;
    if (CSysWin_getUnk34(&self->field_0xE8) != 0) return 1;
    return self->field_0x2A6;
}

u8 func_8023390C(CMenuArtsSet* self) {
    if (func_80235F50(&self->mSubObj74) != 0) return 1;
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return 1;
    return self->field_0x2A6;
}

void func_80235F50(void* self);
void CMenuArtsSet::func_80233968() { func_80235F50(&mSubObj74); }

void func_80233970(CMenuArtsSet* self) {
    if (self->field_0x2C != 0) return;
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return;
    self->field_0x2C = 1;
    self->mField31 = 0;
    func_80235AA0(&self->mSubObj74);
    func_80235124(self);
}

#pragma optimize_for_size on
void func_802339D4(CMenuArtsSet* self) {
    if (self->field_0x2C != 3) return;
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return;
    self->field_0x2C = 4;
    self->mField31 = 0;
    func_8023080C(&self->mSubObj148, 0);
    self->field_0x196 = 0;
    func_80232B88((SArts327B0*)((u8*)&self->mSubObj148 + 0x2C));
    func_80235AC0(&self->mSubObj74);
    func_80138078__FUl(6);
}
#pragma optimize_for_size off

// Arts-menu cursor-up handler (list-page twin of func_80233DC0, gated by
// the 0x2A1 busy flag and the CArtsInfo window instead of the 0x16B armed
// flag): when the arts-info window is running, step it backwards and play
// the cursor sound. Otherwise, when the list busy flag (0x16F) is set and
// the entry pane is visible, clear the 0x148 busy state and re-clamp the
// 0x174 list cursor; when the cursor is already at the top, re-arm the
// entry; otherwise page the list up. When 0x16F is clear, the 0x16A armed
// flag picks the same three-way step on the list cursor. Each step ends
// with the cursor sound (1).
void func_80233A50(CMenuArtsSet* self) {
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return;
    if (self->field_0x2A1 != 0) return;
    if (func_80235F50(&self->mSubObj74) != 0) {
        if (func_80235A98(&self->mSubObj74) != 0) {
            func_80235EF0(&self->mSubObj74);
            func_80138078__FUl(1);
        }
        return;
    }
    if (func_80235F50(&self->mSubObj74) != 0) return;
    if (self->mSubObj148.field_0x27 != 0) {
        if (func_801C4648((nw4r::lyt::Pane*)((SArts313E0*)&self->mSubObj148)
                              ->field_0x08->field_0x10) != 0) {
            func_8023080C(&self->mSubObj148, 0);
            func_80232800(&self->mList174);
            self->field_0x196 = 1;
            func_80232B88((SArts327B0*)&self->mList174);
            func_80235124(self);
        } else if (func_8023270C((SArts3270C*)&self->mList174) != 0) {
            self->field_0x196 = 0;
            func_80232B88((SArts327B0*)&self->mList174);
            func_8023080C(&self->mSubObj148, 1);
            func_80235124(self);
        } else {
            func_80231DD0(&self->mList174);
            func_80235124(self);
        }
        func_80138078__FUl(1);
        return;
    }
    if (self->mSubObj148.field_0x22 != 0) {
        func_8023080C(&self->mSubObj148, 0);
        func_80232800(&self->mList174);
        self->field_0x196 = 1;
        func_80232B88((SArts327B0*)&self->mList174);
        func_80235124(self);
    } else if (func_8023270C((SArts3270C*)&self->mList174) != 0) {
        self->field_0x196 = 0;
        func_80232B88((SArts327B0*)&self->mList174);
        func_8023080C(&self->mSubObj148, 1);
        func_80235124(self);
    } else {
        func_80231DD0(&self->mList174);
        func_80235124(self);
    }
    func_80138078__FUl(1);
}

// Arts-menu cursor-down handler: mirror of func_80233A50, but the arts-info
// window is stepped forwards (func_80235F14) and the list cursor is
// re-clamped with func_802327B0, checked against the page end with
// func_80232734, and paged with func_80231E8C.
void func_80233C08(CMenuArtsSet* self) {
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return;
    if (self->field_0x2A1 != 0) return;
    if (func_80235F50(&self->mSubObj74) != 0) {
        if (func_80235A98(&self->mSubObj74) != 0) {
            func_80235F14(&self->mSubObj74);
            func_80138078__FUl(1);
        }
        return;
    }
    if (func_80235F50(&self->mSubObj74) != 0) return;
    if (self->mSubObj148.field_0x27 != 0) {
        if (func_801C4648((nw4r::lyt::Pane*)((SArts313E0*)&self->mSubObj148)
                              ->field_0x08->field_0x10) != 0) {
            func_8023080C(&self->mSubObj148, 0);
            func_802327B0((SArts327B0*)&self->mList174);
            self->field_0x196 = 1;
            func_80232B88((SArts327B0*)&self->mList174);
            func_80235124(self);
        } else if (func_80232734((SArts322BC*)&self->mList174) != 0) {
            self->field_0x196 = 0;
            func_80232B88((SArts327B0*)&self->mList174);
            func_8023080C(&self->mSubObj148, 1);
            func_80235124(self);
        } else {
            func_80231E8C(&self->mList174);
            func_80235124(self);
        }
        func_80138078__FUl(1);
        return;
    }
    if (self->mSubObj148.field_0x22 != 0) {
        func_8023080C(&self->mSubObj148, 0);
        func_802327B0((SArts327B0*)&self->mList174);
        self->field_0x196 = 1;
        func_80232B88((SArts327B0*)&self->mList174);
        func_80235124(self);
    } else if (func_80232734((SArts322BC*)&self->mList174) != 0) {
        self->field_0x196 = 0;
        func_80232B88((SArts327B0*)&self->mList174);
        func_8023080C(&self->mSubObj148, 1);
        func_80235124(self);
    } else {
        func_80231E8C(&self->mList174);
        func_80235124(self);
    }
    func_80138078__FUl(1);
}

// Cursor-up handler: when the arts info and syswin are idle and the list is
// armed (0x16B), step the arts cursor backwards via the 0x148 sub-object
// (func_80230870) or, when the cursor is at the top (pane check) or the
// 0x16A flag is clear, page the list up (func_80231F60). Plays the cursor
// sound after any move.
void func_80233DC0(CMenuArtsSet* self) {
    if (func_80235F50(&self->mSubObj74) != 0) return;
    if (self->mSubObj148.field_0x23 == 0) return;
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return;
    if (self->mSubObj148.field_0x27 != 0) {
        if (func_801C4648((nw4r::lyt::Pane*)((SArts080C*)self->mSubObj148.field_0x08)->field_0x10) != 0) {
            func_80230870(&self->mSubObj148, self->mSubObj148.field_0x27);
            func_80235124(self);
            goto L_Sound;
        }
        func_80231F60((SArts322BC*)((u8*)&self->mSubObj148 + 0x2C));
        func_80235124(self);
        goto L_Sound;
    }
    if (self->mSubObj148.field_0x22 != 0) {
        func_80230870(&self->mSubObj148, self->field_0x2A1);
        func_80235124(self);
        goto L_Sound;
    }
    func_80231F60((SArts322BC*)((u8*)&self->mSubObj148 + 0x2C));
    func_80235124(self);
L_Sound:
    func_80138078__FUl(1);
}

// Cursor-down handler: mirror of func_80233DC0 but stepping the arts cursor
// forwards (func_8023082C) and paging the list down (func_80232000).
void func_80233E9C(CMenuArtsSet* self) {
    if (func_80235F50(&self->mSubObj74) != 0) return;
    if (self->mSubObj148.field_0x23 == 0) return;
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return;
    if (self->mSubObj148.field_0x27 != 0) {
        if (func_801C4648((nw4r::lyt::Pane*)((SArts080C*)self->mSubObj148.field_0x08)->field_0x10) != 0) {
            func_8023082C(&self->mSubObj148, self->mSubObj148.field_0x27);
            func_80235124(self);
            goto L_Sound;
        }
        func_80232000((SArts322BC*)((u8*)&self->mSubObj148 + 0x2C));
        func_80235124(self);
        goto L_Sound;
    }
    if (self->mSubObj148.field_0x22 != 0) {
        func_8023082C(&self->mSubObj148, self->field_0x2A1);
        func_80235124(self);
        goto L_Sound;
    }
    func_80232000((SArts322BC*)((u8*)&self->mSubObj148 + 0x2C));
    func_80235124(self);
L_Sound:
    func_80138078__FUl(1);
}

// Arts-menu idle/advance handler: dispatch on the sub-object busy states.
// While the syswin or arts-info window is armed, drive its progress;
// otherwise handle the list-cursor busy flags (0x2A1 / 0x16F) or the armed
// window state (0x2A6/0x16B), playing the advance sound (6) on each menu
// step. The sound is shared by the 0x16B body and the 0x2A6-clear path;
// the 0x16B-clear path exits without it (retail L_80235FA0 block).
void func_80233F78(CMenuArtsSet* self) {
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) {
        if (CSysWin_isActive(&self->mSubObjE8) != 0) {
            func_8022B8E4(&self->mSubObjE8);
            self->field_0x2A6 = 0;
        }
        return;
    }
    if (func_80235F50(&self->mSubObj74) != 0) {
        if (func_80235A98(&self->mSubObj74) != 0) {
            func_80235D24(&self->mSubObj74);
            self->field_0x2A6 = 0;
        }
        return;
    }
    if (self->field_0x2A1 != 0) {
        func_80232888((SArts32888*)((u8*)&self->mSubObj148 + 0x2C));
        if (self->field_0x196 != 0) {
            self->field_0x196 = 0;
            func_80232B88((SArts327B0*)((u8*)&self->mSubObj148 + 0x2C));
        }
        func_80138078__FUl(6);
        return;
    }
    if (self->mSubObj148.field_0x27 != 0) {   // absolute 0x16F busy flag
        func_802313E0((SArts313E0*)&self->mSubObj148);
        if (self->field_0x196 != 0) {
            func_8023080C(&self->mSubObj148, 0);
        }
        func_80138078__FUl(6);
        return;
    }
    if (self->field_0x2A6 != 0) {
        if (self->mSubObj148.field_0x23 != 0) {   // absolute 0x16B armed flag
            func_80230D18((SArts30D18*)&self->mSubObj148);
            self->field_0x196 = 1;
            func_80232B88((SArts327B0*)((u8*)&self->mSubObj148 + 0x2C));
            self->field_0x2A6 = 0;
            func_80235124(self);
        } else {
            return;   // 0x16B clear: no sound, exit
        }
    }
    func_80138078__FUl(6);
}

// Arts-menu per-frame advance: dispatch on the help-window / arts-info /
// list busy states and the 0x16A/0x16F mode flags, arming help strings or
// stepping the 0x174 list as each state resolves.
#pragma optimize_for_size on
void func_802340C4(CMenuArtsSet* self) {
    if (self->field_0x2C != 3) return;
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) {
        if (CSysWin_isActive(&self->mSubObjE8) != 0) {
            func_8022B8E4(&self->mSubObjE8);
            self->field_0x2A6 = 0;
            func_80138078__FUl(3);
        }
        return;
    }
    if (func_80235F50(&self->mSubObj74) != 0) {
        if (func_80235A98(&self->mSubObj74) != 0) {
            if (func_80235F3C(&self->mSubObj74) != 0) {
                if (((SArts34D14*)self)->field_0x16A != 0) {
                    // Result of func_80232370 feeds func_8022FF00 directly
                    // (zero-extended into the argument register, no temp).
                    func_8022FF00((SArtsSub8022FA58*)&self->mSubObj124,
                                  func_80232370(&self->mList174,
                                                func_80231014(&self->mSubObj148)));
                    func_802324C4(&self->mList174,
                                  func_80231014(&self->mSubObj148));
                } else {
                    func_8022FF00((SArtsSub8022FA58*)&self->mSubObj124,
                                  func_80232370(&self->mList174, -1));
                    func_802324C4(&self->mList174, -1);
                }
                func_80138078__FUl(0x96);
                func_80235DD8(&self->mSubObj74);
            } else {
                func_80138078__FUl(6);
                func_80235D24(&self->mSubObj74);
            }
            self->field_0x2A6 = 0;
            self->mField31 = 0;
            self->field_0x2C = 6;
        }
        return;
    }
    SArts34D14* ext = (SArts34D14*)self;
    if (((SArts34C84*)self)->field_0x2A1 != 0) {
        if (ext->field_0x168 == 4) return;
        func_80232888((SArts32888*)&self->mList174);
        self->field_0x196 = 0;
        func_80232B88((SArts327B0*)&self->mList174);
        u32 cur = func_802322BC(&self->mList174);
        if ((u32)func_80231014(&self->mSubObj148) == cur) {
            func_80138078__FUl(6);
            return;
        }
        func_80230D74(&self->mSubObj148, func_802322BC(&self->mList174));
        func_80235124(self);
        func_80138078__FUl(0x15);
        return;
    }
    if (self->mSubObj148.field_0x27 != 0) {   // abs 0x16F busy flag
        if (ext->field_0x168 == 4) return;
        int matched = 0;
        u32 cur = func_802322BC(&self->mList174);
        if ((u32)func_80231014(&self->mSubObj148) == cur &&
            func_801C4648((nw4r::lyt::Pane*)((SArts313E0*)&self->mSubObj148)
                              ->field_0x14->field_0x10) == 0) {
            matched = 1;
        }
        if (matched == 0) {
            if (self->field_0x196 != 0) {
                func_80230D74(&self->mSubObj148,
                              func_802322BC(&self->mList174));
            } else {
                func_80230D74(&self->mSubObj148, -1);
            }
        }
        func_802313E0((SArts313E0*)&self->mSubObj148);
        self->field_0x196 = 0;
        func_80232B88((SArts327B0*)&self->mList174);
        // matched==0 falls through first in retail; the matched arm is jumped
        // over and plays only the advance sound.
        if (matched == 0) {
            func_80235124(self);
            func_80138078__FUl(0x15);
            return;
        }
        func_80138078__FUl(6);
        return;
    }
    if (self->field_0x2A6 != 0) {
        if (ext->field_0x169 == 0) return;   // abs 0x16B armed flag
        self->mSubObj148.field_0x23 = 0;     // abs 0x16B
        self->field_0x196 = 1;
        func_80232B88((SArts327B0*)&self->mList174);
        self->field_0x2A6 = 0;
        func_80230D74(&self->mSubObj148, func_802322BC(&self->mList174));
        func_80235124(self);
        func_80138078__FUl(0x15);
        return;
    }
    // Main dispatch: pick the list key (current selection, or -1 when the
    // 0x16A flag is clear), then arm a help window or step the info state.
    int key;
    if (ext->field_0x16A != 0) {
        // Signed-typed read: retail emits cmpi (not cmpli) for this check.
        if ((s8)ext->field_0x168 == 4) {
            func_80234844(self);
            return;
        }
        key = func_80231014(&self->mSubObj148);
        if (key == 0) return;
    } else {
        key = -1;
    }
    if (func_802322F4(&self->mList174, key) >= 10) {
        char* name = func_80136190(lbl_eu_8050AC70 + 0x50, lbl_eu_8050AC70 + 0x5b,
                                   0x35);
        func_8022B9B4((CSysWin*)&self->mSubObjE8, name, 0);
        func_8022BFC8((CSysWin*)&self->mSubObjE8, 1);
        func_8022B8B8((CSysWin*)&self->mSubObjE8);
    } else if (func_80232638(&self->mList174, key) != 0) {
        // Selectable entry: pick the help id from the learn-state table.
        int id;
        if (lbl_eu_806628A8[0] == 0 &&
            func_8023040C((SArtsSub8022FA58*)&self->mSubObj124,
                          ext->mSubObj124.field_0x14) == 1) {
            id = 0x38;
        } else {
            id = 0x36;
            if (func_802322F4(&self->mList174, key) == 7) id = 0x37;
        }
        char* name = func_80136190(lbl_eu_8050AC70 + 0x50, lbl_eu_8050AC70 + 0x5b,
                                   id);
        func_8022B9B4((CSysWin*)&self->mSubObjE8, name, 0);
        func_8022BFC8((CSysWin*)&self->mSubObjE8, 1);
        func_8022B8B8((CSysWin*)&self->mSubObjE8);
    } else {
        u16 pct = func_80232370(&self->mList174, key);
        // Retail computes the advance arm as the fall-through (>= compare).
        if ((u32)func_8022FEC4((SArtsSub8022FA58*)&self->mSubObj124) >= pct) {
            func_80235AE0(&self->mSubObj74);
        } else {
            char* name = func_80136190(lbl_eu_8050AC70 + 0x50,
                                       lbl_eu_8050AC70 + 0x5b, 0x34);
            func_8022B9B4((CSysWin*)&self->mSubObjE8, name, 0);
            func_8022BFC8((CSysWin*)&self->mSubObjE8, 1);
            func_8022B8B8((CSysWin*)&self->mSubObjE8);
        }
    }
    func_80138078__FUl(3);
}
#pragma optimize_for_size off

// Arts-menu advance (page-down): when the menu is idle (0x2A6/0x2A1/0x16F
// clear, CArtsInfo inactive, state 3, 0x124 sub-object armed at 0x16), step
// the list cursor with func_8022FE58, drive both list sub-panels with the
// current entry, refresh the driver state, and play the advance sound.
void func_802346BC(CMenuArtsSet* self) {
    if (self->field_0x2A6 != 0) return;
    if (func_80235F50(&self->mSubObj74) != 0) return;
    if (self->field_0x2C != 3) return;
    if (self->mSubObj124.field_0x16 == 0) return;
    if (self->field_0x2A1 != 0) return;
    if (self->mSubObj148.field_0x27 != 0) return;
    func_8022FE58((SArtsSub8022FA58*)&self->mSubObj124);
    u8 v = func_8023040C((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    func_802308B0(&self->mSubObj148, v);
    v = func_8023040C((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    func_802320C0((SArts322BC*)((u8*)&self->mSubObj148 + 0x2C), v);
    func_80235124(self);
    func_80138078__FUl(10);
}

// Twin of func_802346BC but stepping the cursor with func_8022FE90
// (backwards instead of forwards).
// Scroll-down input handler (cursor-back twin of func_802346BC): guarded by
// the shared busy flags, then step the arts cursor back and refresh both
// list sub-panels with the entry under the new cursor.
void func_80234780(CMenuArtsSet* self) {
    if (self->field_0x2A6 != 0) return;
    if (func_80235F50(&self->mSubObj74) != 0) return;
    if (self->field_0x2C != 3) return;
    if (self->mSubObj124.field_0x16 == 0) return;
    if (self->field_0x2A1 != 0) return;
    if (self->mSubObj148.field_0x27 != 0) return;
    func_8022FE90((SArtsSub8022FA58*)&self->mSubObj124);
    u8 v = func_8023040C((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    func_802308B0(&self->mSubObj148, v);
    v = func_8023040C((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    func_802320C0((SArts322BC*)((u8*)&self->mSubObj148 + 0x2C), v);
    func_80235124(self);
    func_80138078__FUl(10);
}

// Arts-list confirm: when the menu is idle and the current arts entry
// (index 0x138) is the locked arts id (1), arm the window if the arts
// manager reports the entry unavailable, else toggle the shared
// availability byte, refresh both list sub-panels with the entry, and play
// the confirm sound.
extern "C" __declspec(noinline) void func_80234A08(CMenuArtsSet* self); // defined below (window-arm chain)
#pragma optimize_for_size on
void func_80234844(CMenuArtsSet* self) {
    if (self->field_0x2A6 != 0) return;
    if (func_80235F50(&self->mSubObj74) != 0) return;
    if (self->field_0x2A1 != 0) return;
    if (self->mSubObj148.field_0x27 != 0) return;
    u8 v = func_8023040C((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    if (v != 1) return;
    if (func_800A32BC(func_8009EC9C(1)) != 0) return;
    if (func_80231320(&self->mSubObj148) != 0) {
        func_80234A08(self);
        return;
    }
    lbl_eu_806628A8[0] = (lbl_eu_806628A8[0] ^ 1) != 0;
    func_802308B0(&self->mSubObj148, v);
    func_802320C0((SArts322BC*)((u8*)&self->mSubObj148 + 0x2C), v);
    func_80235124(self);
    func_80138078__FUl(0xA);
}
#pragma optimize_for_size off

// Scroll-up input handler: guarded by the shared busy flags plus the
// 0x168 mode / 0x16A flag pair, skip when the current entry is the locked
// arts id while the byte-table gate is clear, then play either the
// list-end sound or the move sound, reset the sub-list, and refresh.
void func_80234928(CMenuArtsSet* self) {
    if (self->field_0x2A6 != 0) return;
    if (func_80235F50(&self->mSubObj74) != 0) return;
    SArts34D14* ext = (SArts34D14*)self;
    if (ext->field_0x16A == 0) return;                 // abs 0x16A
    if (ext->field_0x168 == 4) return;                 // abs 0x168
    if (((SArts34C84*)self)->field_0x2A1 != 0) return;
    if (((SArts34C84*)self)->field_0x16F != 0) return;
    u8 v = func_8023040C((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    // Indexed byte-table probe: lbl_eu_806628A8[v] (retail lbz @sda21(r0)).
    if (v == 1 && lbl_eu_806628A8[v] == 0) return;
    if (func_80231014(&self->mSubObj148) != 0) {
        func_80138078__FUl(0x77);
    } else {
        func_80138078__FUl(5);
    }
    func_80230D74(&self->mSubObj148, 0);
    func_80235124(self);
}

void CMenuArtsSet::func_802349F8(u8 val) { mSubObj124.field_0x15 = val; }

void CMenuArtsSet::func_80234A00() { ((void(*)(void*))func_80231320)((char*)this + 0x148); }

// Arm the CSysWin sub-object with a new label pair and mark the 0x2A6 flag.
// Guarded by the syswin/info busy checks; the string pair is built from the
// shared arts archive path base (lbl_eu_8050AC70).
// C-linkage + noinline: retail keeps a real bl from func_80234844.
extern "C" __declspec(noinline) void func_80234A08(CMenuArtsSet* self) {
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return;
    if (func_80235F50(&self->mSubObj74) != 0) return;
    char* base = lbl_eu_8050AC70;
    char* name = func_80136190(base + 0x50, base + 0x5b, 0x3c);
    func_8022B9B4((CSysWin*)&self->mSubObjE8, name, 0);
    func_8022BFC8((CSysWin*)&self->mSubObjE8, 1);
    func_8022B8B8((CSysWin*)&self->mSubObjE8);
    self->field_0x2A6 = 1;
}

// Arts-menu confirm handler: when the arts-info window is idle, branch on
// the 0x16A armed flag and the 0x2A1 busy flag. In the armed state, check
// whether the current list entry (func_802322BC on the 0x174 list) matches
// the selected arts entry (func_80231014 on the 0x148 sub-object) and the
// entry pane is hidden - if so, mark it matched and, when matched, re-arm
// via func_802313E0 + func_80230D74. When 0x16A is clear or 0x2A1 is set,
// toggle the 0x2A1 busy state through func_80232888 on the 0x174 list
// object and either re-arm (entries differ) or re-show the entry pane.
#pragma optimize_for_size on
void func_80234A94(CMenuArtsSet* self) {
    u32 cur;
    int matched;
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return;
    if (func_80235F50(&self->mSubObj74) != 0) return;
    if (self->mSubObj148.field_0x22 != 0 && self->field_0x2A1 == 0) {
        if (self->mSubObj148.field_0x20 == 4) return;
        if (self->mSubObj148.field_0x27 != 0) {
            matched = 0;
            cur = func_802322BC(&self->mList174);
            u8 r = func_80231014(&self->mSubObj148);
            if (r == cur) {
                if (func_801C4648((nw4r::lyt::Pane*)((SArts313E0*)&self->mSubObj148)
                                      ->field_0x14->field_0x10) == 0) {
                    matched = 1;
                }
            }
            if (matched == 0) {
                if (self->field_0x196 != 0) {
                    func_80230D74(&self->mSubObj148, func_802322BC(&self->mList174));
                } else {
                    func_80230D74(&self->mSubObj148, 0xFFFFFFFF);
                }
            }
            func_802313E0((SArts313E0*)&self->mSubObj148);
            self->field_0x196 = 0;
            func_80232B88((SArts327B0*)&self->mList174);
            if (matched == 0) {
                func_80235124(self);
                func_80138078__FUl(0x15);
            } else {
                func_80138078__FUl(6);
            }
            return;
        }
        func_802313E0((SArts313E0*)&self->mSubObj148);
        func_80138078__FUl(2);
        return;
    }
    if (self->field_0x2A1 != 0) {
        if (self->mSubObj148.field_0x20 == 4) return;
        func_80232888((SArts32888*)&self->mList174);
        self->field_0x196 = 0;
        func_80232B88((SArts327B0*)&self->mList174);
        cur = func_802322BC(&self->mList174);
        u8 r = func_80231014(&self->mSubObj148);
        if (r == cur) {
            func_80138078__FUl(6);
            return;
        }
        func_80230D74(&self->mSubObj148, func_802322BC(&self->mList174));
        func_80235124(self);
        func_80138078__FUl(0x15);
        return;
    }
    func_80232888((SArts32888*)&self->mList174);
    func_8023080C(&self->mSubObj148, 1);
    func_80231464(&self->mSubObj148);
    func_80235124(self);
    func_80138078__FUl(2);
}
#pragma optimize_for_size off

int func_80234C84(SArts34C84* self) {
    if (self->field_0x16F != 0) {
        return 1;
    }
    return self->field_0x2A1;
}

int func_80234CA0(SArts34D14* self) {
    if (self->field_0x16A == 0) return 0;
    if ((int)self->field_0x168 != 4) return 0;
    u8 v = func_8023040C(&self->mSubObj124, self->mSubObj124.field_0x14);
    if (v != 1) return 0;
    return func_800A32BC((void*)func_8009EC9C(v)) == 0;
}

int func_80234D14(SArts34D14* self) {
    u8 r = func_8023040C(&self->mSubObj124, self->mSubObj124.field_0x14);
    if (r == 1 && lbl_eu_806628A8[r] == 0) return 1;
    return 0;
}

// Arts-menu action-id query: returns the sound/action id for the current
// list state. The syswin / arts-info busy guards, the 0x139 armed flag, and
// the 0x16F/0x2A1 busy pair each short-circuit to a fixed id; otherwise the
// id depends on whether more than one arts entry is unlocked and on the
// 0x148 sub-object / 0x168 mode / 0x16A flag state.
#pragma optimize_for_size on
extern "C" u8 func_80234D68(CMenuArtsSet* self) {
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return 0;
    if (func_80235F50(&self->mSubObj74) != 0) return 0;
    if (self->mSubObj124.field_0x15 != 0) return 0x42;
    if (func_80234C84((SArts34C84*)self) != 0) return 0x40;
    int more = code80135FDC_getByte_64077() > 1;
    if (self->mSubObj148.field_0x22 != 0) {          // abs 0x16A
        if (func_80231014(&self->mSubObj148) == 0) {
            u8 r = 0x49;
            if (more) r = 0x48;
            return r;
        }
        if ((int)((SArts34D14*)self)->field_0x168 != 4) {  // abs 0x168
            if (func_80234D14((SArts34D14*)self) != 0) {
                u8 r = 0x47;
                if (more) r = 0x46;
                return r;
            }
            u8 r = 0x41;
            if (more) r = 0x3F;
            return r;
        }
        if (func_80234CA0((SArts34D14*)self) != 0) {
            u8 r = 0x44;
            if (more) r = 0x43;
            return r;
        }
        u8 r = 0x00;
        if (more) r = 0x45;
        return r;
    }
    u8 r = 0x47;
    if (more) r = 0x46;
    return r;
}
#pragma optimize_for_size off

// Confirm/advance handler: when idle and armed, run the layout-in animation
// driver at field_0x20 via the field_0x1C interface, mark state 2, refresh
// both list sub-panels with the current entry, scroll the bar in with the
// 3-float init vector, then kick the scrollbar tick.
void func_80234EB8(CMenuArtsSet* self) {
    if (func_80137444(self->field_0x20, lbl_eu_80668648) == 0) return;
    self->field_0x1C->v11(self->field_0x20, 0);
    self->field_0x1C->v11(self->field_0x24, 1);
    self->field_0x2C = 2;
    u8 v = func_8023040C((SArtsSub8022FA58*)&self->mSubObj124, self->mSubObj124.field_0x14);
    func_802320C0((SArts322BC*)((u8*)&self->mSubObj148 + 0x2C), v);
    float vec[3];
    vec[0] = lbl_eu_80668678;
    vec[1] = lbl_eu_8066867C;
    vec[2] = lbl_eu_8066864C;
    func_801F3670(self->field_0x34, vec);
    func_801F367C(self->field_0x34);
}

extern "C" __declspec(noinline) void func_80234F7C(CMenuArtsSet* self) {
    if (func_80137444(self->field_0x24, lbl_eu_80668648) != 0) {
        self->field_0x2C = 3;
        self->mField31 = 1;
        func_8023080C(&self->mSubObj148, 1);
        func_80235124(self);
    }
}

// State-4 handler: when the layout-in animation has finished, drive both
// sub-panes via the field_0x1C interface, advance the state to 5, reset the
// 0x148 sub-object, and request the scroll bar scroll-out.
// noinline: func_8023359C keeps a real bl to the retail symbol.
__declspec(noinline) void func_80234FDC(CMenuArtsSet* self) {
    float duration = lbl_eu_80668648;
    if (func_80137510(self->field_0x24, duration) != 0) {
        self->field_0x1C->v11(self->field_0x24, 0);
        self->field_0x1C->v11(self->field_0x20, 1);
        self->field_0x2C = 5;
        func_8023080C(&self->mSubObj148, 0);
        func_801F369C(self->field_0x34);
    }
}

extern "C" __declspec(noinline) void func_8023506C(SArts3506C* self) {
    if (func_80137510(self->mAnim20, lbl_eu_80668648) != 0) {
        self->field_0x31 = 1;
        self->field_0x2C = 0;
    }
}

extern "C" __declspec(noinline) void func_802350B8(CMenuArtsSet* self) {
    if (func_80235A98(&self->mSubObj74) != 0) {
        self->mField31 = 1;
        self->field_0x2C = 3;
        func_80235124(self);
    }
}

void func_80235108(SArts35108* self) {
    if (self->field_0x1C != 0) {
        self->field_0x30 = 1;
        self->field_0x28 = 1;
    }
}

// Refresh the arts-info fields from the active source: pick the selected
// arts id via the 0x124 list cursor, then - depending on the 0x16A/0x16F/
// 0x196/0x2A6 mode flags - read the id/key/percent triple either from the
// 0x148 sub-object or the 0x174 arts table, and store all four into the
// CArtsInfo before refreshing its cursor driver.
extern "C" __declspec(noinline) void func_80235124(CMenuArtsSet* self) {
    u8 id = 0;   // arts-table row id / sub-object result
    u8 key = 0;  // key byte fed to func_802322F4/func_80232370
    u16 pct = 0; // percent value from func_80232370
    u8 sel = func_8023040C((SArtsSub8022FA58*)&self->mSubObj124,
                           self->mSubObj124.field_0x14);
    if (self->mSubObj148.field_0x22 != 0) {
        if (self->mSubObj148.field_0x27 != 0) {
            if (self->field_0x196 != 0) {
                id = func_802322BC(&self->mList174);
                key = func_802322F4(&self->mList174, -1);
                pct = func_80232370(&self->mList174, -1);
            } else {
                id = func_80231014(&self->mSubObj148);
                key = func_80231220(&self->mSubObj148);
            }
        } else if (self->field_0x2A6 != 0) {
            id = func_802322BC(&self->mList174);
            key = func_802322F4(&self->mList174, -1);
            pct = func_80232370(&self->mList174, -1);
        } else {
            id = func_80231014(&self->mSubObj148);
            key = func_802322F4(&self->mList174, id);
            pct = func_80232370(&self->mList174, id);
        }
    } else {
        id = func_802322BC(&self->mList174);
        key = func_802322F4(&self->mList174, -1);
        pct = func_80232370(&self->mList174, -1);
    }
    func_80235E84(&self->mSubObj74, sel);
    func_80235E8C(&self->mSubObj74, id);
    func_80235E94(&self->mSubObj74, key);
    func_80235E9C(&self->mSubObj74, pct);
    func_80235EA4(&self->mSubObj74);
    func_80232C78((SArts327B0*)&self->mList174);
}

// CArtsList::OnFileEvent (retail OnFileEvent__9CArtsListFP10CEventFile):
// file-load completion for the arts list. Resizes the mem region, attaches
// the loaded arc data to the accessor, binds the layout/anims, re-inits the
// three sub-objects (0x124 list, 0x148 layout pair, 0x174 arts table) from
// stack temps (declared largest-first so MWCC assigns the retail slots
// 0x60/0x38/0xC/0x8), then drops the file handle and frees the region guard.
int CArtsList::OnFileEvent(CEventFile* pEventFile) {
    SArts319AC temp319AC;
    SArtsSub8022FA58Short tempFA58;
    SArts3042C temp3042C;

    if (field_0x14 != pEventFile->mFileHandle) {
        return 0;
    }
    void* mem2 = getHandleMEM2__Q23mtl10MemManagerFv();
    mMemRegion.createRegion((int)mem2, 0x16000, lbl_eu_8050AC70 + 0x2CF, 0);
    Class_8045F858 regionGuard(&mMemRegion);
    u8* fileData = field_0x14->mData;
    field_0x14->mData = 0;
    func_80434A4C__Q23mtl10MemManagerFb(false);
    field_0x18 = createArcResourceAccessor__10CLibLayoutFv();
    Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(field_0x18, fileData,
                                                   lbl_eu_8050AC70 + 0x2D9);
    func_80136E84((nw4r::lyt::Layout**)&field_0x1C, field_0x18,
                  lbl_eu_8050AC70 + 0x2DD);
    func_80136F08((nw4r::lyt::Layout*)field_0x1C, &field_0x20, field_0x18,
                  lbl_eu_8050AC70 + 0x2F7);
    func_80136F08((nw4r::lyt::Layout*)field_0x1C, &field_0x24, field_0x18,
                  lbl_eu_8050AC70 + 0x314);
    SArts3CObj* pane = field_0x1C->field_0x10;
    u32 fontHandle = ((SDevFontV*)func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
                          1, (nw4r::lyt::Layout*)field_0x1C))
                         ->v7();
    func_8013676C(pane, fontHandle);
    void* text = func_801355BC();
    if (text != 0) {
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x6F, (u32)text);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x336, (u32)text);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x33F, (u32)text);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x348, (u32)text);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x351, (u32)text);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x35A, (u32)text);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x100, (u32)text);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x363, (u32)text);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x36C, (u32)text);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x375, (u32)text);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x37E, (u32)text);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)field_0x1C,
                                                lbl_eu_8050AC70 + 0x387, (u32)text);
    }
    field_0x1C->v11(field_0x24, 0);
    field_0x1C->v11(field_0x20, 1);
    field_0x1C->v14(0);
    __ct__8022FA58((SArtsSub8022FA58*)&tempFA58, (u32)field_0x1C, (u32)field_0x18);
    mSubObj124 = tempFA58;
    func_8022FAD0((SArtsSub8022FA58*)&mSubObj124);
    __ct__8023042C(&temp3042C, (u32)field_0x1C, (u32)field_0x18);
    mSubObj148 = temp3042C;
    func_802304C4((SArts304C4*)&mSubObj148);
    func_802308B0((SArtsSub8022FA58*)&mSubObj148,
                  func_8023040C((SArtsSub8022FA58*)&mSubObj124, mSubObj124.field_0x14));
    __ct__802319AC(&temp319AC, (u32)field_0x1C, (u32)field_0x18, (u32)&mScrollBar);
    mSubObj174 = temp319AC;
    func_80231A48((SArts304C4*)&mSubObj174);
    func_80235108((SArts35108*)this);
    field_0x14 = 0;
    mMemRegion.func_8045F810();
    return 1;
}

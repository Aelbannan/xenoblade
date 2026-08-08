// Auto-scaffolded catalog TU for kyoshin/COption
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/COption.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80296898.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"
#include <nw4r/lyt.h>
#include <cstring>

// External labels and functions used by COption methods
extern float lbl_eu_80668C10;
extern float lbl_eu_80668C14;
extern float lbl_eu_80668C18;
extern float lbl_eu_80668C1C;
extern char lbl_eu_805103D8[];

void func_80138078(u32);

u32 func_80137444(nw4r::lyt::AnimTransform*, float);
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
int func_8029E3F8(COption* self);

u8 func_8029C790(void* self) { return static_cast<COptionFull*>(self)->field_2B; }


// Show pane "name1", hide pane "name2", then activate cursor.
extern "C" void func_8029C03C(COption* self) {
    nw4r::lyt::Pane* pane = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x50, true);
    func_80124270(pane, 1);
    pane = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x5E, true);
    func_80124270(pane, 0);
    func_801D2174((CBaseCur*)self);
}

// Hide pane "name1", show pane "name2", then activate cursor.
extern "C" void func_8029C0C4(COption* self) {
    nw4r::lyt::Pane* pane = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x50, true);
    func_80124270(pane, 0);
    pane = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x5E, true);
    func_80124270(pane, 1);
    func_801D2174((CBaseCur*)self);
}

// Finish the option-panel intro animation at +0x10: when it has finished, mark
// inactive, run the shared cursor routine, and hide both pane labels.
extern "C" void func_8029C14C(COption* self) {
    f32 target = lbl_eu_80668C10;
    if (func_80137444(self->mpAnimTrans1, target) != 0) {
        self->mActive = 0;
        func_801D21CC(self);
        nw4r::lyt::Pane* pane = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x50, true);
        func_80124270(pane, 0);
        pane = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x5E, true);
        func_80124270(pane, 0);
    }
}


void func_8029C5C8(COption* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_0x28 != 0) {
        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(self->mSubObj), drawInfo, 0, 1);
        func_801F35B0(&self->mScrollBar, drawInfo);
        if (CSysWin_getUnk34(self->mSysWin) == 0) {
            if (self->field_0x2E != 0) {
                func_801D20B0(&self->mSubCur2, drawInfo);
            } else {
                func_801D20B0(&self->mSubCur1, drawInfo);
            }
        }
        func_8022B7C8(&self->mSysWin, drawInfo);
        func_801D20B0(&self->mSubCur3, drawInfo);
    }
}

void func_8029C66C(){}
// Returns the option-window visibility byte only if the embedded scroll bar
// is visible AND the system window is ready; otherwise 0.
extern "C" u8 func_8029C734(COptionWindow* self) {
    if (!CScrollBar_isVisible(&self->mScrollBar)) return 0;
    if (CSysWin_isReady(&self->mSysWin) == 0) return 0;
    return self->field_0x2A;
}
u8 func_8029C798(void* self) { return static_cast<COptionFull*>(self)->field_30; }

u8 func_8029C7A0(void* self) { return static_cast<COptionFull*>(self)->field_32; }

// func_8029C7A8: cursor-left. In state 3/9 with the system window busy, decrement
// the sub-page counter 0x31; otherwise (window idle) decrement the column 0x2C and
// borrow from the row 0x2D, wrapping to the bottom-right cell at the origin.
void func_8029C7A8(COption* self) {
    if (self->field_0x29 == 3 || self->field_0x29 == 9) {
        if (self->field_0x30 == 0) {
            if (CSysWin_getUnk34(self->mSysWin) != 0) {
                if (CSysWin_isActive(self->mSysWin) != 0) {
                    self->field_0x31 = self->field_0x31 - 1;
                    if ((s8)self->field_0x31 < 0) {
                        self->field_0x31 = 1;
                    }
                    func_8029E254(self);
                    func_80138078(1);
                }
            } else if (self->field_0x2E == 0) {
                self->field_0x2C = self->field_0x2C - 1;
                if ((s8)self->field_0x2C < 0) {
                    self->field_0x2C = 0;
                    self->field_0x2D = (s8)((u8)self->field_0x2D - 1);
                    if ((s8)self->field_0x2D < 0) {
                        self->field_0x2C = 6;
                        self->field_0x2D = 0xe;
                    }
                }
                func_8029D420(self);
                func_8029E254(self);
                s8 selIdx = self->field_0x2D;
                func_801F3850(&self->mScrollBar, (u16)selIdx);
                func_80138078(1);
            }
        }
    }
}

// func_8029C8C4: cursor-right. In state 3/9 with the system window busy, increment
// the sub-page counter 0x31 (wrapping to 0); otherwise (window idle) increment the
// column 0x2C and advance the row 0x2D, wrapping to the top-left cell after 0xe.
void func_8029C8C4(COption* self) {
    if (self->field_0x29 == 3 || self->field_0x29 == 9) {
        if (self->field_0x30 == 0) {
            if (CSysWin_getUnk34(self->mSysWin) != 0) {
                if (CSysWin_isActive(self->mSysWin) != 0) {
                    self->field_0x31 = self->field_0x31 + 1;
                    if ((s8)self->field_0x31 >= 2) {
                        self->field_0x31 = 0;
                    }
                    func_8029E254(self);
                    func_80138078(1);
                }
            } else if (self->field_0x2E == 0) {
                self->field_0x2C = self->field_0x2C + 1;
                if ((s8)self->field_0x2C >= 7) {
                    self->field_0x2C = 6;
                    self->field_0x2D = (s8)((u8)self->field_0x2D + 1);
                    if ((s8)self->field_0x2D > 0xe) {
                        self->field_0x2C = 0;
                        self->field_0x2D = 0;
                    }
                }
                func_8029D420(self);
                func_8029E254(self);
                s8 selIdx = self->field_0x2D;
                func_801F3850(&self->mScrollBar, (u16)selIdx);
                func_80138078(1);
            }
        }
    }
}

void func_8029C9E8(){}

void func_8029D990(COption* self);

// func_8029CABC: cursor-down. In state 3/9 with the system window idle, run the
// D990 handler when the 0x2E mode flag is set; otherwise bump the row counter
// 0x2D by 7 and wrap the excess into column 0x2C when it exceeds 0xe.
void func_8029CABC(COption* self) {
    if ((self->field_0x29 == 3 || self->field_0x29 == 9) && self->field_0x30 == 0
        && CSysWin_getUnk34(self->mSysWin) == 0) {
        if (self->field_0x2E != 0) {
            func_8029D990(self);
        } else {
            u8 row = (u8)self->field_0x2D + 7;
            self->field_0x2D = (s8)row;
            if ((s8)row > 0xe) {
                self->field_0x2C = row - 0xe;
                self->field_0x2D = 0xe;
                if ((s8)self->field_0x2C >= 7) {
                    self->field_0x2C = 6;
                }
            }
            func_8029D420(self);
            func_8029E254(self);
            s8 selIdx = self->field_0x2D;
            func_801F3850(&self->mScrollBar, (u16)selIdx);
        }
        func_80138078(1);
    }
}

void func_8029CB9C(COption* self) {
    if (self->field_0x29 == 0) {
        self->field_0x29 = 1;
        self->field_0x2B = 0;
        func_801F36BC(&self->mScrollBar, 7, 0x15);
        f32 vec[3];
        vec[0] = lbl_eu_80668C14;
        vec[1] = lbl_eu_80668C18;
        vec[2] = lbl_eu_80668C1C;
        func_801F3670(&self->mScrollBar, vec);
        s8 selIdx = self->field_0x2D;
        func_801F3850(&self->mScrollBar, (u16)selIdx);
        func_801F367C(&self->mScrollBar);
    }
}

// When state 3 is active, advance to state 4, hide sub-cursors 1/2, reset the
// scroll bar, and play selection sound 6.
extern "C" void func_8029CC30(COption* self) {
    if (self->field_0x29 == 3) {
        self->field_0x29 = 4;
        self->field_0x2B = 0;
        func_801F369C(&self->mScrollBar);
        func_801D216C(&self->mSubCur1, 0);
        func_801D216C(&self->mSubCur2, 0);
        func_80138078(6);
    }
}

// func_8029CC9C: advance/cancel option-panel state. In state 3/9 with the system
// window busy and active, flip the state to 7/10, reset the scroll bar and show
// sub-cursor 1, then play sound 6/3. Otherwise toggle the 0x2E mode flag and sync
// the sub-cursors via func_8029E254.
void func_8029CC9C(COption* self) {
    if (self->field_0x29 == 3 || self->field_0x29 == 9) {
        if (self->field_0x30 == 0) {
            if (CSysWin_getUnk34(self->mSysWin) != 0) {
                if (CSysWin_isActive(self->mSysWin) != 0) {
                    self->field_0x29 = (self->field_0x29 == 3) ? 7 : 10;
                    self->field_0x2B = 0;
                    func_8022B8E4(self->mSysWin);
                    func_801D216C(&self->mSubCur1, 1);
                    func_801D216C(&self->mSubCur3, 0);
                    {
                    s8 f31 = (s8)self->field_0x31;
                    self->field_0x33 = 0;
                    if (f31 != 0) {
                        func_80138078(6);
                    } else {
                        func_80138078(3);
                    }
                    }
                    self->field_0xFC = 0;
                }
            } else {
                if (self->field_0x2E != 0) {
                    self->field_0x2E = 0;
                    func_8029E254(self);
                } else {
                    self->field_0x2E = 1;
                    func_8029E254(self);
                    self->field_0x2F = func_8029D7E8(self);
                }
                func_80138078(3);
            }
        }
    }
}

void func_8029CDB0(){}

void func_8029CF7C(){}

// Return the option-window navigation code: 0 when the system window busies,
// 0x6D when the 0x2E flag is set, else 0x6C/0x6E depending on func_8029E3F8.
extern "C" u8 func_8029D054(COption* self) {
    if (CSysWin_getUnk34(self->mSysWin)) return 0;
    if (self->field_0x2E) return 0x6D;
    return func_8029E3F8(self) ? 0x6E : 0x6C;
}

// Finish the option-panel animation at +0x20: when it has reached the target
// frame, set state 2 and run the shared tail handler func_8029E1CC.
extern "C" void func_8029D0C0(COption* self) {
    if (func_80137444(self->mAnimTransform20, lbl_eu_80668C10)) {
        self->field_0x29 = 2;
        func_8029E1CC(self);
    }
}

// Check if an animation transform has finished; if so, finalize state
// and activate sub-cursors before calling the shared tail handler.
extern "C" void func_8029D10C(COption* self) {
    if (func_80137444(self->mAnimTransform24, lbl_eu_80668C10) != 0) {
        self->field_0x29 = 3;
        self->field_0x2B = 1;
        func_801D216C(&self->mSubCur1, 1);
        func_801D216C(&self->mSubCur2, 1);
        func_8029E254(self);
    }
}

// Finish the +0x24 animation: set state 5 and run the shared handler func_8029E144.
extern "C" void func_8029D178(COption* self) {
    if (func_80137510(self->mAnimTransform24, lbl_eu_80668C10)) {
        self->field_0x29 = 5;
        func_8029E144(self);
    }
}

// Finish the +0x20 animation (different checker): clear state 0x29 and set 0x2B.
extern "C" void func_8029D1C4(COption* self) {
    if (func_80137510(self->mAnimTransform20, lbl_eu_80668C10)) {
        self->field_0x29 = 0;
        self->field_0x2B = 1;
    }
}

// CCur19 constructor: run the CBaseCur base constructor, then install the
// CCur19 vtable at +0x00.
extern "C" void __ct__CCur19(CBaseCur* _this, void* accessor) {
    __ct__8CBaseCurFv(_this);
    _this->mVtable = (void*)lbl_eu_805396D0;
}

// CCur19 destructor: tear down the CBaseCur base, then free the object only
// when the delete flag is positive (standard MWCC member-destructor shape).
extern "C" void* __dt__8029BF18(void* _this, int flags) {
    if (_this) {
        __dt__8CBaseCurFv(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// If the embedded system window is active, set state 3 and run the shared
// activation tail with sub-cursor 1 disabled and sub-cursor 3 enabled.
extern "C" void func_8029D210(COption* self) {
    if (CSysWin_isActive(self->mSysWin)) {
        self->field_0x29 = 3;
        self->field_0x2B = 1;
        func_801D216C(&self->mSubCur1, 0);
        func_801D216C(&self->mSubCur3, 1);
        func_8029E254(self);
    }
}

void func_8029D278(){}

// Same as func_8029D210 but sets state 9.
extern "C" void func_8029D2F0(COption* self) {
    if (CSysWin_isActive(self->mSysWin)) {
        self->field_0x29 = 9;
        self->field_0x2B = 1;
        func_801D216C(&self->mSubCur1, 0);
        func_801D216C(&self->mSubCur3, 1);
        func_8029E254(self);
    }
}

// If the system window is active and the signed 0x31 flag is clear, set state 3
// and re-init the config singleton before running func_8029D420.
extern "C" void func_8029D358(COption* self) {
    if (CSysWin_isActive(self->mSysWin)) {
        self->field_0x29 = 3;
        self->field_0x2B = 1;
        if ((s8)self->field_0x31 == 0) {
            lbl_eu_80577308.init();
            func_8029D420(self);
        }
    }
}

// If the +0x18 source is live, flag it, sync the singleton config (0x40 bytes)
// from getInstance() into this TU's global, then run func_8029D420.
extern "C" void func_8029D3C0(COption* self) {
    if (self->mpAnimTrans1 == 0) return;
    self->field_0x2A = 1;
    self->field_0x28 = 1;
    std::memcpy(&lbl_eu_80577308, Class_80296898::getInstance(), 0x40);
    func_8029D420(self);
}

extern "C" void __declspec(noinline) func_8029D420(COption* self) {}

// non-eliminable so sibling tail calls (e.g. func_8029D7E8) keep a real branch.
extern "C" u8 __declspec(noinline) func_8029D634(COption*, u8) { return 0; }

// Sum the bytes at 0x2C/0x2D into a u8 and pass to the state handler func_8029D634.
extern "C" u8 func_8029D7E8(COption* self) {
    return func_8029D634(self, (u8)(self->field_0x2C + self->field_0x2D));
}

void func_8029D7FC(){}

// Look up a signed-index table entry via the sum of the signed 0x2D/0x2C bytes.
extern "C" u8 func_8029D96C(COption* self) {
    return lbl_eu_8053948C[(s8)self->field_0x2C + (s8)self->field_0x2D];
}

void __declspec(noinline) func_8029D990(COption* self) {
    u8 amount = func_8029D96C(self);
    int cell = (s8)self->field_0x2C + (s8)self->field_0x2D;
    if ((unsigned)cell <= 0x14) {
        u8* cfg = (u8*)&lbl_eu_80577308;
        switch (cell) {
            case 0x00: cfg[0x1c] = (u8)(cfg[0x1c] + 1); if (cfg[0x1c] >= amount) cfg[0x1c] = 0; break;
            case 0x01: cfg[0x0f] = (u8)(cfg[0x0f] + 2); if (cfg[0x0f] > amount) cfg[0x0f] = 8; break;
            case 0x02: cfg[0x20] = (u8)(cfg[0x20] + 1); if (cfg[0x20] >= amount) cfg[0x20] = 0; break;
            case 0x03: cfg[0x21] = (u8)(cfg[0x21] + 1); if (cfg[0x21] >= amount) cfg[0x21] = 0; break;
            case 0x04: cfg[0x23] = (u8)(cfg[0x23] + 1); if (cfg[0x23] >= amount) cfg[0x23] = 0; break;
            case 0x05: cfg[0x22] = (u8)(cfg[0x22] + 1); if (cfg[0x22] >= amount) cfg[0x22] = 0; break;
            case 0x06: cfg[0x24] = (u8)(cfg[0x24] + 1); if (cfg[0x24] >= amount) cfg[0x24] = 0; break;
            case 0x07: cfg[0x25] = (u8)(cfg[0x25] + 1); if (cfg[0x25] >= amount) cfg[0x25] = 0; break;
            case 0x08: cfg[0x00] = (u8)(cfg[0x00] + 1); if (cfg[0x00] >= amount) cfg[0x00] = 0; break;
            case 0x09: cfg[0x01] = (u8)(cfg[0x01] + 1); if (cfg[0x01] >= amount) cfg[0x01] = 0; break;
            case 0x0a: cfg[0x02] = (u8)(cfg[0x02] + 1); if (cfg[0x02] >= amount) cfg[0x02] = 0; break;
            case 0x0b: cfg[0x03] = (u8)(cfg[0x03] + 1); if (cfg[0x03] >= amount) cfg[0x03] = 0; break;
            case 0x0c: cfg[0x04] = (u8)(cfg[0x04] + 1); if (cfg[0x04] >= amount) cfg[0x04] = 0; break;
            case 0x0d: cfg[0x05] = (u8)(cfg[0x05] + 1); if (cfg[0x05] >= amount) cfg[0x05] = 0; break;
            case 0x0e: cfg[0x06] = (u8)(cfg[0x06] + 1); if (cfg[0x06] >= amount) cfg[0x06] = 0; break;
            case 0x0f: cfg[0x07] = (u8)(cfg[0x07] + 1); if (cfg[0x07] >= amount) cfg[0x07] = 0; break;
            case 0x10: cfg[0x10] = (u8)(cfg[0x10] + 1); if (cfg[0x10] >= amount) cfg[0x10] = 0; break;
            case 0x11: cfg[0x11] = (u8)(cfg[0x11] + 1); if (cfg[0x11] >= amount) cfg[0x11] = 0; break;
            case 0x12: cfg[0x30] = (u8)(cfg[0x30] + 1); if (cfg[0x30] >= amount) cfg[0x30] = 0; break;
            case 0x13: cfg[0x32] = (u8)(cfg[0x32] + 1); if (cfg[0x32] >= amount) cfg[0x32] = 0; break;
            case 0x14: cfg[0x31] = (u8)(cfg[0x31] + 1); if (cfg[0x31] >= amount) cfg[0x31] = 0; break;
        }
    }
    func_8029D420(self);
    func_8029C0C4(reinterpret_cast<COption*>(&self->mSubCur2));
}

void func_8029DD6C(COption* self){
    u8 amount = func_8029D96C(self);
    int cell = (s8)self->field_0x2C + (s8)self->field_0x2D;
    if ((unsigned)cell <= 0x14) {
        u8* cfg = (u8*)&lbl_eu_80577308;
        switch (cell) {
            case 0x00: if (cfg[0x1c] == 0) cfg[0x1c] = (u8)(amount - 1); else cfg[0x1c] = (u8)(cfg[0x1c] - 1); break;
            case 0x01: if (cfg[0x0f] == 8) cfg[0x0f] = amount; else cfg[0x0f] = (u8)(cfg[0x0f] - 2); break;
            case 0x02: if (cfg[0x20] == 0) cfg[0x20] = (u8)(amount - 1); else cfg[0x20] = (u8)(cfg[0x20] - 1); break;
            case 0x03: if (cfg[0x21] == 0) cfg[0x21] = (u8)(amount - 1); else cfg[0x21] = (u8)(cfg[0x21] - 1); break;
            case 0x04: if (cfg[0x23] == 0) cfg[0x23] = (u8)(amount - 1); else cfg[0x23] = (u8)(cfg[0x23] - 1); break;
            case 0x05: if (cfg[0x22] == 0) cfg[0x22] = (u8)(amount - 1); else cfg[0x22] = (u8)(cfg[0x22] - 1); break;
            case 0x06: if (cfg[0x24] == 0) cfg[0x24] = (u8)(amount - 1); else cfg[0x24] = (u8)(cfg[0x24] - 1); break;
            case 0x07: if (cfg[0x25] == 0) cfg[0x25] = (u8)(amount - 1); else cfg[0x25] = (u8)(cfg[0x25] - 1); break;
            case 0x08: if (cfg[0x00] == 0) cfg[0x00] = (u8)(amount - 1); else cfg[0x00] = (u8)(cfg[0x00] - 1); break;
            case 0x09: if (cfg[0x01] == 0) cfg[0x01] = (u8)(amount - 1); else cfg[0x01] = (u8)(cfg[0x01] - 1); break;
            case 0x0a: if (cfg[0x02] == 0) cfg[0x02] = (u8)(amount - 1); else cfg[0x02] = (u8)(cfg[0x02] - 1); break;
            case 0x0b: if (cfg[0x03] == 0) cfg[0x03] = (u8)(amount - 1); else cfg[0x03] = (u8)(cfg[0x03] - 1); break;
            case 0x0c: if (cfg[0x04] == 0) cfg[0x04] = (u8)(amount - 1); else cfg[0x04] = (u8)(cfg[0x04] - 1); break;
            case 0x0d: if (cfg[0x05] == 0) cfg[0x05] = (u8)(amount - 1); else cfg[0x05] = (u8)(cfg[0x05] - 1); break;
            case 0x0e: if (cfg[0x06] == 0) cfg[0x06] = (u8)(amount - 1); else cfg[0x06] = (u8)(cfg[0x06] - 1); break;
            case 0x0f: if (cfg[0x07] == 0) cfg[0x07] = (u8)(amount - 1); else cfg[0x07] = (u8)(cfg[0x07] - 1); break;
            case 0x10: if (cfg[0x10] == 0) cfg[0x10] = (u8)(amount - 1); else cfg[0x10] = (u8)(cfg[0x10] - 1); break;
            case 0x11: if (cfg[0x11] == 0) cfg[0x11] = (u8)(amount - 1); else cfg[0x11] = (u8)(cfg[0x11] - 1); break;
            case 0x12: if (cfg[0x30] == 0) cfg[0x30] = (u8)(amount - 1); else cfg[0x30] = (u8)(cfg[0x30] - 1); break;
            case 0x13: if (cfg[0x32] == 0) cfg[0x32] = (u8)(amount - 1); else cfg[0x32] = (u8)(cfg[0x32] - 1); break;
            case 0x14: if (cfg[0x31] == 0) cfg[0x31] = (u8)(amount - 1); else cfg[0x31] = (u8)(cfg[0x31] - 1); break;
        }
    }
    func_8029D420(self);
    func_8029C03C(reinterpret_cast<COption*>(&self->mSubCur2));
}

// Configure the +0x1C sub-object animation state: idle then attach the +0x20
// transform twice (read/writable) before leaving idle mode.
extern "C" void func_8029E144(COption* self) {
    COptionSub* sub = self->mSubObj;
    sub->v7();
    sub->v5(self->mAnimTransform20);
    sub->v9(self->mAnimTransform20, 1);
    sub->v12(0);
}

// Configure the +0x1C sub-object animation state using the +0x24 transform.
extern "C" void func_8029E1CC(COption* self) {
    COptionSub* sub = self->mSubObj;
    sub->v7();
    sub->v5(self->mAnimTransform24);
    sub->v9(self->mAnimTransform24, 1);
    sub->v12(0);
}

extern "C" void __declspec(noinline) func_8029E254(COption* self) {}

// Returns "config changed": 1 when any probed byte of the config singleton
// differs from the TU local copy. The final 0x31 comparison is emitted inline
// (the last one falls through to the return).
int func_8029E3F8(COption* self) {
    u8* b = (u8*)Class_80296898::getInstance();
    u8* g = (u8*)&lbl_eu_80577308;
    if (g[0x1c] != b[0x1c]) return 1;
    if (g[0x0f] != b[0x0f]) return 1;
    if (g[0x20] != b[0x20]) return 1;
    if (g[0x21] != b[0x21]) return 1;
    if (g[0x23] != b[0x23]) return 1;
    if (g[0x22] != b[0x22]) return 1;
    if (g[0x24] != b[0x24]) return 1;
    if (g[0x25] != b[0x25]) return 1;
    if (g[0x00] != b[0x00]) return 1;
    if (g[0x01] != b[0x01]) return 1;
    if (g[0x02] != b[0x02]) return 1;
    if (g[0x03] != b[0x03]) return 1;
    if (g[0x04] != b[0x04]) return 1;
    if (g[0x05] != b[0x05]) return 1;
    if (g[0x06] != b[0x06]) return 1;
    if (g[0x07] != b[0x07]) return 1;
    if (g[0x10] != b[0x10]) return 1;
    if (g[0x11] != b[0x11]) return 1;
    if (g[0x30] != b[0x30]) return 1;
    if (g[0x32] != b[0x32]) return 1;
    return g[0x31] == b[0x31];
}

// COption::~COption() - complete-object destructor (virtual). Destroys the
// embedded sub-objects in reverse declaration order, then conditionally frees
// the object. The null-guard on mSubCur2 reproduces MWCC's addic-register
// check before destroying the polymorphic CBaseCur sub-object (D2-inlined-into-
// D1 artifact).
COption::~COption() {
    __dt__6CCur18Fv(&mSubCur3, -1);
    __dt__7CSysWinFv(mSysWin, -1);
    CBaseCur* subCur2 = &mSubCur2;
    if (subCur2) {
        __dt__8CBaseCurFv(subCur2, 0);
    }
    __dt__6CCur18Fv(&mSubCur1, -1);
    __dt__10CScrollBarFv(&mScrollBar, -1);
    UnkClass_8045F564* memRegion = reinterpret_cast<UnkClass_8045F564*>(&mArcResAcc);
    __dt__17UnkClass_8045F564Fv(memRegion, -1);
}

bool COption::OnFileEvent(CEventFile* pEventFile) {
    COptionMem* mem = reinterpret_cast<COptionMem*>(this);
    if (mem->mFHandle != pEventFile->mFileHandle) return false;

    // Set up the scratch heap region, then attach the freshly-exchanged file
    // data as the nw4r layout archive and build the layout + two animations.
    mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
    mem->mRegion.createRegion(mem2, 0x10000, &lbl_eu_805103D8[0xf7], 0);
    Class_8045F858 memHost(&mem->mRegion);
    void* fileData = mem->mFHandle->getData();
    mtl::MemManager::func_80434A4C(false);
    mem->mArcAcc = createArcResourceAccessor__10CLibLayoutFv();
    mem->mArcAcc->Attach(fileData, &lbl_eu_805103D8[0xff]);
    func_80136E84(&mem->mpLayout, mem->mArcAcc, &lbl_eu_805103D8[0x103]);
    func_80136F08(mem->mpLayout, &mem->mAnim0, mem->mArcAcc, &lbl_eu_805103D8[0x11a]);
    func_80136F08(mem->mpLayout, &mem->mAnim1, mem->mArcAcc, &lbl_eu_805103D8[0x134]);

    // Bind the font: take the layout root pane, ask the font object for its
    // pane, and push it back onto the root.
    void* rootPane = *(void**)((char*)mem->mpLayout + 0x10);
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mem->mpLayout);
    u32 fontResult = ((u32 (*)(void*))(((void**)fontObj)[0x24 / 4]))(fontObj);
    func_8013676C((nw4r::lyt::Pane*)rootPane, fontResult);

    func_8029E144(this);

    // Build sub-cursor 1 (CCur18) on the stack and copy it into +0x78.
    u8 tempA[0x20];
    __ct__CCur18(tempA, func_801355F4());
    func_8018B0FC(&mSubCur1, tempA);
    __dt__6CCur18Fv(tempA, -1);
    ((void (*)(void*))(((void**)mSubCur1.mVtable)[0x08 / 4]))(&mSubCur1);

    // Build sub-cursor 2 (CCur19) on the stack and copy it into +0x90.
    u8 tempB[0x20];
    __ct__CCur19((CBaseCur*)tempB, mem->mArcAcc);
    func_8018B0FC(&mSubCur2, tempB);
    __dt__8CBaseCurFv(tempB, 0);
    ((void (*)(void*))(((void**)mSubCur2.mVtable)[0x08 / 4]))(&mSubCur2);

    // Build sub-cursor 3 (CCur18) on the stack and copy it into +0xE4.
    u8 tempC[0x20];
    __ct__CCur18(tempC, func_801355F4());
    func_8018B0FC(&mSubCur3, tempC);
    __dt__6CCur18Fv(tempC, -1);
    ((void (*)(void*))(((void**)mSubCur3.mVtable)[0x08 / 4]))(&mSubCur3);

    func_8029D3C0(this);
    mem->mFHandle = 0;
    mem->mRegion.func_8045F810();
    return true;
}


// --- hard-symbol stubs (scaffold_hard_symbols) ---

// sinit: constructor run for the Class_80296898 global at lbl_eu_80577308.
// Tail-calls the global constructor with the object address.
extern "C" void sinit_8029E7D8(){
    func_80296A04__FP14Class_80296898(&lbl_eu_80577308);
}

extern "C" void func_8029BF68() {}
extern "C" void func_8029C35C() {}
extern "C" void func_8029C4F4() {}

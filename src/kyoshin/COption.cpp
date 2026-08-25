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
#include <cstdio>

// External labels and functions used by COption methods
extern const float lbl_eu_80668C10;
extern const float lbl_eu_80668C14;
extern const float lbl_eu_80668C18;
extern const float lbl_eu_80668C1C;
extern char lbl_eu_805103D8[];

void func_80138078(u32);

u32 func_80137444(nw4r::lyt::AnimTransform*, float);
u32 func_80137510(nw4r::lyt::AnimTransform*, float);

u8 func_8029C790(void* self) { return static_cast<COptionFull*>(self)->field_2B; }


// Show pane "name1", hide pane "name2", then activate cursor.
#pragma optimize_for_size on
extern "C" void func_8029C03C(COption* self) {
    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x50, true), 1);
    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x5E, true), 0);
    func_801D2174((CBaseCur*)self);
}
#pragma optimize_for_size off

// Hide pane "name1", show pane "name2", then activate cursor.
#pragma optimize_for_size on
extern "C" void __declspec(noinline) func_8029C0C4(COption* self) {
    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x50, true), 0);
    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x5E, true), 1);
    func_801D2174((CBaseCur*)self);
}
#pragma optimize_for_size off

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


#pragma optimize_for_size on
void func_8029C5C8(COption* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_0x28 != 0) {
        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(self->mSubObj), drawInfo, 0, 1);
        func_801F35B0(&self->mScrollBar, drawInfo);
        if (CSysWin_getUnk34(self->mSysWin) == 0) {
            if (self->field_0x2E == 0) {
                func_801D20B0(&self->mSubCur1, drawInfo);
            } else {
                func_801D20B0(&self->mSubCur2, drawInfo);
            }
        }
        func_8022B7C8(&self->mSysWin, drawInfo);
        func_801D20B0(&self->mSubCur3, drawInfo);
    }
}
#pragma optimize_for_size off

void func_8029C66C(COption* self) {
    COptionMem* mem = reinterpret_cast<COptionMem*>(self);
    func_801390E0(&mem->mFHandle);
    nw4r::lyt::Layout* layout = mem->mpLayout;
    self->field_0x28 = 0;
    // The outer guard plus delete's own null-check reproduce retail's two beq's
    // on the same CR0 test (MWCC D2-inlined-into-D1 artifact).
    if (layout != 0) {
        delete layout;
        mem->mpLayout = 0;
    }
    func_80139124(mem->mArcAcc);
    mem->mArcAcc = 0;
    func_801F35DC(&self->mScrollBar);
    reinterpret_cast<CCursor18*>(&self->mSubCur1)->vf3();
    reinterpret_cast<CCursor18*>(&self->mSubCur2)->vf3();
    func_8022B7F4(&self->mSysWin);
    reinterpret_cast<CCursor18*>(&self->mSubCur3)->vf3();
    reinterpret_cast<UnkClass_8045F564*>(&self->mArcResAcc)->func_8045F778();
}
// Returns the option-window visibility byte only if the embedded scroll bar
// is visible AND the system window is ready; otherwise 0.
extern "C" u8 func_8029C734(COptionWindow* self) {
    if (CScrollBar_isVisible(&self->mScrollBar)) {
        if (CSysWin_isReady(&self->mSysWin) != 0) {
            return self->field_0x2A;
        }
    }
    return 0;
}
u8 func_8029C798(COptionFull* self) { return self->field_30; }

u8 func_8029C7A0(COptionFull* self) { return self->field_32; }

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

// func_8029C9E8: cursor cancel/up handler. In state 3/9 with the system window
// idle, either run the config step-down handler (0x2E mode) or decrement the
// row 0x2D by 7, borrowing 6 columns into 0x2C when it underflows.
extern "C" void func_8029C9E8(COption* self) {
    if ((self->field_0x29 == 3 || self->field_0x29 == 9) && self->field_0x30 == 0
        && CSysWin_getUnk34(self->mSysWin) == 0) {
        if (self->field_0x2E != 0) {
            func_8029DD6C(self);
        } else {
            self->field_0x2D = (s8)((u8)self->field_0x2D - 7);
            if ((s8)self->field_0x2D < 0) {
                self->field_0x2C = (u8)self->field_0x2D + 6;
                self->field_0x2D = 0;
                if ((s8)self->field_0x2C < 0) {
                    self->field_0x2C = 0;
                }
            }
            func_8029D420(self);
            func_8029E254(self);
            func_801F3850(self->mScrollBar, (u16)self->field_0x2D);
        }
        func_80138078(1);
    }
}

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
// the sub-cursors via func_8029E254. stmw/lmw frame needs optimize_for_size.
#pragma optimize_for_size on
extern "C" void func_8029CC9C(COption* self) {
    if (self->field_0x29 == 3 || self->field_0x29 == 9) {
        if (self->field_0x30 == 0) {
            if (CSysWin_getUnk34(self->mSysWin) != 0) {
                if (CSysWin_isActive(self->mSysWin) != 0) {
                    self->field_0x29 = (self->field_0x29 == 3) ? 7 : 10;
                    self->field_0x2B = 0;
                    func_8022B8E4(self->mSysWin);
                    func_801D216C(&self->mSubCur1, 1);
                    func_801D216C(&self->mSubCur3, 0);
                    u8 flag = self->field_0x31;
                    self->field_0x33 = 0;
                    if ((s8)flag == 0) {
                        func_80138078(3);
                    } else {
                        func_80138078(6);
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
#pragma optimize_for_size off

// func_8029CDB0: option-panel accept/right handler. In state 3/9 with the
// system window busy and active, advance to state 7/10 (arg==0 only); when
// the window is idle and 0x2E mode is set, clear it (arg==0 only); otherwise
// when the config changed (func_8029E3F8), reload the three config-panel
// messages into the window and set state 6. The single func_80138078(6)
// tail is shared by the 0x2E-clear and config-change paths (retail jumps to
// it with `b .L_8029F64C`). stmw/lmw frame needs optimize_for_size.
#pragma optimize_for_size on
void func_8029CDB0(COption* self, int arg) {
    if (self->field_0x29 != 3 && self->field_0x29 != 9) return;
    if (self->field_0x30 != 0) return;
    if (CSysWin_getUnk34(self->mSysWin) != 0) {
        if (CSysWin_isActive(self->mSysWin) != 0 && arg == 0) {
            self->field_0x29 = (self->field_0x29 == 3) ? 7 : 10;
            self->field_0x2B = 0;
            self->field_0x31 = 1;
            func_8022B8E4(self->mSysWin);
            func_801D216C(&self->mSubCur1, 1);
            func_801D216C(&self->mSubCur3, 0);
            self->field_0x33 = 1;
            self->field_0xFC = 0;
            func_80138078(6);
        }
        return;
    }
    if (self->field_0x2E != 0) {
        if (arg != 0) return;
        self->field_0x2E = 0;
        func_8029E254(self);
    } else {
        if (func_8029E3F8(self) != 0) {
            self->field_0x29 = 6;
            self->field_0x2B = 0;
            self->field_0x31 = 0;
            char* s1 = func_80136190(lbl_eu_805103D8 + 0x7f, lbl_eu_805103D8 + 0x88,
                                     self->field_0x34 != 0 ? 0x70 : 0x67);
            char* s2 = func_80136190(lbl_eu_805103D8 + 0x7f, lbl_eu_805103D8 + 0x88, 0x71);
            char* s3 = func_80136190(lbl_eu_805103D8 + 0x7f, lbl_eu_805103D8 + 0x88, 0x72);
            func_8022B9B4(self->mSysWin, s1, 0);
            func_8022BF6C(self->mSysWin, s2, s3);
            func_8022BFC8(reinterpret_cast<CSysWin*>(self->mSysWin), 0);
            func_8022B8B8(self->mSysWin);
        } else {
            self->field_0x30 = 1;
        }
        if (arg != 0) {
            self->field_0x32 = 1;
        }
    }
    func_80138078(6);
}
#pragma optimize_for_size off

// func_8029CF7C: open the config dialog. When the system window is idle and
// not in 0x2E mode, load the three config-panel messages, set up the window
// and play the open sound. stmw/lmw frame needs optimize_for_size.
#pragma optimize_for_size on
extern "C" void func_8029CF7C(COption* self) {
    if (CSysWin_getUnk34(self->mSysWin) == 0 && self->field_0x2E == 0) {
        self->field_0x29 = 8;
        self->field_0x2B = 0;
        self->field_0x31 = 0;
        const char* base = lbl_eu_805103D8;
        char* s1 = func_80136190(base + 0x7f, base + 0x88, 0x6d);
        char* s2 = func_80136190(base + 0x7f, base + 0x88, 0x6e);
        char* s3 = func_80136190(base + 0x7f, base + 0x88, 0x6f);
        func_8022B9B4(self->mSysWin, s1, 0);
        func_8022BF6C(self->mSysWin, s2, s3);
        func_8022BFC8(reinterpret_cast<CSysWin*>(self->mSysWin), 0);
        func_8022B8B8(self->mSysWin);
        func_80138078(3);
    }
}
#pragma optimize_for_size off

// Return the option-window navigation code: 0 when the system window busies,
// 0x6D when the 0x2E flag is set, else 0x6C/0x6E depending on func_8029E3F8.
extern "C" u8 func_8029D054(COption* self) {
    if (CSysWin_getUnk34(self->mSysWin)) return 0;
    if (self->field_0x2E) return 0x6D;
    return func_8029E3F8(self) ? 0x6E : 0x6C;
}

// Finish the option-panel animation at +0x20: when it has reached the target
// frame, set state 2 and run the shared tail handler func_8029E1CC.
extern "C" void __declspec(noinline) func_8029D0C0(COption* self) {
    if (func_80137444(self->mAnimTransform20, lbl_eu_80668C10)) {
        self->field_0x29 = 2;
        func_8029E1CC(self);
    }
}

// Check if an animation transform has finished; if so, finalize state
// and activate sub-cursors before calling the shared tail handler.
extern "C" void __declspec(noinline) func_8029D10C(COption* self) {
    if (func_80137444(self->mAnimTransform24, lbl_eu_80668C10) != 0) {
        self->field_0x29 = 3;
        self->field_0x2B = 1;
        func_801D216C(&self->mSubCur1, 1);
        func_801D216C(&self->mSubCur2, 1);
        func_8029E254(self);
    }
}

// Finish the +0x24 animation: set state 5 and run the shared handler func_8029E144.
extern "C" void __declspec(noinline) func_8029D178(COption* self) {
    if (func_80137510(self->mAnimTransform24, lbl_eu_80668C10)) {
        self->field_0x29 = 5;
        func_8029E144(self);
    }
}

// Finish the +0x20 animation (different checker): clear state 0x29 and set 0x2B.
extern "C" void __declspec(noinline) func_8029D1C4(COption* self) {
    if (func_80137510(self->mAnimTransform20, lbl_eu_80668C10)) {
        self->field_0x29 = 0;
        self->field_0x2B = 1;
    }
}

// CCur19 constructor: run the CBaseCur base constructor, then install the
// CCur19 vtable at +0x00. Returns this (retail emits the mr r3, r31
// return-this after the vtable load).
extern "C" __declspec(noinline) void* __ct__CCur19(CBaseCur* _this, void* accessor) {
    __ct__8CBaseCurFv(_this);
    _this->mVtable = (void*)lbl_eu_805396D0;
    return _this;
}

// CCur19 destructor: tear down the CBaseCur base, then free the object only
// when the delete flag is positive (standard MWCC member-destructor shape).
#pragma optimize_for_size on  // -O4,s: retail stmw r30 frame
extern "C" void* __dt__8029BF18(void* _this, int flags) {
    if (_this) {
        __dt__8CBaseCurFv(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}
#pragma optimize_for_size off

// COption constructor (retail C-linkage symbol __ct__COption; MWCC leaves
// __-prefixed global functions unmangled). Stores the vtable, constructs the
// embedded region, zero-inits the state bytes (0x2B=1, 0x34=arg) and builds
// the five embedded widgets: scroll bar, three cursors, system window.
// stmw/lmw frame needs optimize_for_size (same shape as ~COption).
#pragma optimize_for_size on
COption* __ct__COption(COption* _this, u8 arg) {
    _this->mVtable = (void*)lbl_eu_80539638;
    __ct__17UnkClass_8045F564Fv(reinterpret_cast<UnkClass_8045F564*>(&_this->mArcResAcc));
    // +0x14..+0x24 are pointers in the retail early layout (see COptionMem).
    COptionMem* mem = reinterpret_cast<COptionMem*>(_this);
    mem->mFHandle = 0;
    mem->mArcAcc = 0;
    _this->mSubObj = 0;
    _this->mAnimTransform20 = 0;
    _this->mAnimTransform24 = 0;
    _this->field_0x28 = 0;
    _this->field_0x29 = 0;
    _this->field_0x2A = 0;
    _this->field_0x2B = 1;
    _this->field_0x2C = 0;
    _this->field_0x2D = 0;
    _this->field_0x2E = 0;
    _this->field_0x2F = 0;
    _this->field_0x30 = 0;
    _this->field_0x31 = 0;
    _this->field_0x32 = 0;
    _this->field_0x33 = 0;
    _this->field_0x34 = arg;
    __ct__CScrollBar(reinterpret_cast<CScrollBar*>(_this->mScrollBar), 0);
    __ct__CCur18(&_this->mSubCur1, 0);
    __ct__CCur19(&_this->mSubCur2, 0);
    __ct__CSysWin(reinterpret_cast<CSysWin*>(_this->mSysWin), 0);
    __ct__CCur18(&_this->mSubCur3, 0);
    _this->field_0xFC = 0;
    _this->field_0x100 = 0;
    return _this;
}
#pragma optimize_for_size off

// If the embedded system window is active, set state 3 and run the shared
// activation tail with sub-cursor 1 disabled and sub-cursor 3 enabled.
extern "C" void __declspec(noinline) func_8029D210(COption* self) {
    if (CSysWin_isActive(self->mSysWin)) {
        self->field_0x29 = 3;
        self->field_0x2B = 1;
        func_801D216C(&self->mSubCur1, 0);
        func_801D216C(&self->mSubCur3, 1);
        func_8029E254(self);
    }
}

// func_8029D278: option-panel state machine driven by the +0xFC state word.
// State 0 waits for the config singleton to change: re-sync the TU-local copy
// (with the retail dead second arg), then when the game is in a resettable
// mode clear the voice/enum object lists. State 1 waits for the work system
// pack to finish; state 2 activates the window and shows cursor 1.
// stmw/lmw frame needs optimize_for_size.
#pragma optimize_for_size on
extern "C" void __declspec(noinline) func_8029D278(COption* self) {
    if (self->field_0xFC == 0) {
        if ((s8)self->field_0x31 == 0) {
            u8 cur = Class_80296898::getInstance()->mConfigData[0x0C];
            func_80296AE8__FPUc((u8*)&lbl_eu_80577308, 1);
            if (cur != Class_80296898::getInstance()->mConfigData[0x0C]) {
                if (cf::CfGameManager::func_8007E1B4() != 0
                    && (lbl_eu_80663E28 & 0x01000000u) == 0) {
                    VoiceSource* vs = func_eu_800874D0();
                    while (vs != 0) {
                        func_800BF2F8(reinterpret_cast<cf::CfObject*>(vs));
                        vs = func_eu_800874D8(vs);
                    }
                    CfEnumObject* eo = func_eu_800874CC();
                    while (eo != 0) {
                        func_800BF2F8(reinterpret_cast<cf::CfObject*>(eo));
                        eo = func_eu_800874D4(eo);
                    }
                }
            }
            if (cur != Class_80296898::getInstance()->mConfigData[0x0C]) {
                lbl_eu_80664A40 = Class_80296898::getInstance()->mConfigData[0x0C];
            }
        }
        self->field_0xFC = 1;
        self->field_0x100 = 0;
    } else if (self->field_0xFC == 1) {
        if (CWorkSystemPack::func_804DE08C() != 0) {
            self->field_0xFC = 2;
        }
    } else if (self->field_0xFC == 2) {
        if (CSysWin_isActive(self->mSysWin) != 0) {
            self->field_0x29 = 3;
            self->field_0x2B = 1;
            if (self->field_0x33 == 0) {
                self->field_0x30 = 1;
            }
        }
    }
}
#pragma optimize_for_size off

// Same as func_8029D210 but sets state 9.
extern "C" void __declspec(noinline) func_8029D2F0(COption* self) {
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
extern "C" void __declspec(noinline) func_8029D358(COption* self) {
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
extern "C" void __declspec(noinline) func_8029D3C0(COption* self) {
    if (self->field_18 == 0) return;  // retail checks the word at +0x18, not mpAnimTrans1
    self->field_0x2A = 1;
    self->field_0x28 = 1;
    std::memcpy(&lbl_eu_80577308, Class_80296898::getInstance(), 0x40);
    func_8029D420(self);
}

// func_8029D420: refresh the option-panel text rows. For each of the 7 rows,
// resolve the pane name via the +0x1C sub-object's message source, show/hide
// the pane from the per-row enable table, then bind the row's three text
// cells from the message/value tables. Ends with the footer row from the
// signed (0x2C + 0x2D) table entry. stmw/lmw frame needs optimize_for_size.
#pragma optimize_for_size on
extern "C" void __declspec(noinline) func_8029D420(COption* self) {
    const u16* tblA = (const u16*)&lbl_eu_80539338[0x00];
    const u16* tblB = (const u16*)&lbl_eu_80539338[0x2c];
    const u8* tblC = &lbl_eu_80539338[0x58];
    const u8* tblD = &lbl_eu_80539338[0x16c];
    char buf[0x28];
    for (u8 i = 0; i < 7; i++) {
        u8 idx = (u8)(i + (u8)self->field_0x2D);
        sprintf(buf, &lbl_eu_805103D8[0x8d], i + 1);
        nw4r::lyt::Pane* pane =
            reinterpret_cast<COptionSubData*>(self->mSubObj)->mMsgObj->v0D(buf, 1);
        u16 a = tblA[idx];
        if (a != 0) {
            func_80124270(pane, 1);
            sprintf(buf, &lbl_eu_805103D8[0x99], i + 1);
            char* s = func_80136190(&lbl_eu_805103D8[0x7f], &lbl_eu_805103D8[0x88], a);
            func_80136B4C(reinterpret_cast<nw4r::lyt::Layout*>(self->mSubObj), buf, s, 0);
        } else {
            func_80124270(pane, 0);
        }
        u16 b = tblB[idx];
        sprintf(buf, &lbl_eu_805103D8[0xa5], i + 1);
        char* s2 = func_80136190(&lbl_eu_805103D8[0x7f], &lbl_eu_805103D8[0x88], b);
        func_80136B4C(reinterpret_cast<nw4r::lyt::Layout*>(self->mSubObj), buf, s2, 0);
        u8 v = func_8029D634(self, idx);
        u16 c = *(const u16*)(tblC + idx * 12 + (u16)v * 2);
        sprintf(buf, &lbl_eu_805103D8[0xb2], i + 1);
        char* s3 = func_80136190(&lbl_eu_805103D8[0x7f], &lbl_eu_805103D8[0x88], c);
        func_80136B4C(reinterpret_cast<nw4r::lyt::Layout*>(self->mSubObj), buf, s3, 0);
        u8 d = tblD[idx];
        sprintf(buf, &lbl_eu_805103D8[0xbe], i + 1);
        nw4r::lyt::Pane* pane2 =
            reinterpret_cast<COptionSubData*>(self->mSubObj)->mMsgObj->v0D(buf, 1);
        func_80124270(pane2, d);
    }
    u16 footer = tblB[(s8)self->field_0x2C + (s8)self->field_0x2D];
    char* s4 = func_80136190(&lbl_eu_805103D8[0x7f], &lbl_eu_805103D8[0xcb], footer);
    func_80136B4C(reinterpret_cast<nw4r::lyt::Layout*>(self->mSubObj),
                  &lbl_eu_805103D8[0xd0], s4, 0);
}
#pragma optimize_for_size off

// func_8029D634: read the config byte for the option cell index 0..0x14
// (inverse of func_8029D7FC). Cell 1 maps the frame-count byte 0x0F through
// a 5-step table (8,10,12,14,16 -> 0..4). The address is re-derived inside
// every case body (retail emits lis/addi per case); the inner switch's no-match
// path falls to the shared `return 0` tail.
extern "C" u8 __declspec(noinline) func_8029D634(COption* self, u8 index) {
    switch (index) {
        case 0x00: return ((u8*)&lbl_eu_80577308)[0x1c];
        case 0x01:
            switch (((u8*)&lbl_eu_80577308)[0x0f]) {
                case 0x08: return 0;
                case 0x0a: return 1;
                case 0x0c: return 2;
                case 0x0e: return 3;
                case 0x10: return 4;
            }
            break;
        case 0x02: return ((u8*)&lbl_eu_80577308)[0x20];
        case 0x03: return ((u8*)&lbl_eu_80577308)[0x21];
        case 0x04: return ((u8*)&lbl_eu_80577308)[0x23];
        case 0x05: return ((u8*)&lbl_eu_80577308)[0x22];
        case 0x06: return ((u8*)&lbl_eu_80577308)[0x24];
        case 0x07: return ((u8*)&lbl_eu_80577308)[0x25];
        case 0x08: return ((u8*)&lbl_eu_80577308)[0x00];
        case 0x09: return ((u8*)&lbl_eu_80577308)[0x01];
        case 0x0a: return ((u8*)&lbl_eu_80577308)[0x02];
        case 0x0b: return ((u8*)&lbl_eu_80577308)[0x03];
        case 0x0c: return ((u8*)&lbl_eu_80577308)[0x04];
        case 0x0d: return ((u8*)&lbl_eu_80577308)[0x05];
        case 0x0e: return ((u8*)&lbl_eu_80577308)[0x06];
        case 0x0f: return ((u8*)&lbl_eu_80577308)[0x07];
        case 0x10: return ((u8*)&lbl_eu_80577308)[0x10];
        case 0x11: return ((u8*)&lbl_eu_80577308)[0x11];
        case 0x12: return ((u8*)&lbl_eu_80577308)[0x30];
        case 0x13: return ((u8*)&lbl_eu_80577308)[0x32];
        case 0x14: return ((u8*)&lbl_eu_80577308)[0x31];
    }
    return 0;
}

// Sum the bytes at 0x2C/0x2D into a u8 and pass to the state handler func_8029D634.
extern "C" u8 __declspec(noinline) func_8029D7E8(COption* self) {
    return func_8029D634(self, (u8)(self->field_0x2C + self->field_0x2D));
}

// func_8029D7FC: write the config byte for the option cell at
// (s8)field_0x2C + (s8)field_0x2D. Cell indices 0..0x14 map onto the
// singleton config bytes (same table as func_8029D634 / func_8029D990). The
// switch's own bounds check emits the retail `cmplwi/bgtlr` guard. The
// address is re-derived inside every case body (retail emits lis/addi per
// case - a hoisted cfg local would change the shape).
extern "C" void func_8029D7FC(COption* self, u8 value) {
    int cell = (s8)self->field_0x2C + (s8)self->field_0x2D;
    switch (cell) {
        case 0x00: ((u8*)&lbl_eu_80577308)[0x1c] = value; break;
        case 0x01: ((u8*)&lbl_eu_80577308)[0x0f] = value; break;
        case 0x02: ((u8*)&lbl_eu_80577308)[0x20] = value; break;
        case 0x03: ((u8*)&lbl_eu_80577308)[0x21] = value; break;
        case 0x04: ((u8*)&lbl_eu_80577308)[0x23] = value; break;
        case 0x05: ((u8*)&lbl_eu_80577308)[0x22] = value; break;
        case 0x06: ((u8*)&lbl_eu_80577308)[0x24] = value; break;
        case 0x07: ((u8*)&lbl_eu_80577308)[0x25] = value; break;
        case 0x08: ((u8*)&lbl_eu_80577308)[0x00] = value; break;
        case 0x09: ((u8*)&lbl_eu_80577308)[0x01] = value; break;
        case 0x0a: ((u8*)&lbl_eu_80577308)[0x02] = value; break;
        case 0x0b: ((u8*)&lbl_eu_80577308)[0x03] = value; break;
        case 0x0c: ((u8*)&lbl_eu_80577308)[0x04] = value; break;
        case 0x0d: ((u8*)&lbl_eu_80577308)[0x05] = value; break;
        case 0x0e: ((u8*)&lbl_eu_80577308)[0x06] = value; break;
        case 0x0f: ((u8*)&lbl_eu_80577308)[0x07] = value; break;
        case 0x10: ((u8*)&lbl_eu_80577308)[0x10] = value; break;
        case 0x11: ((u8*)&lbl_eu_80577308)[0x11] = value; break;
        case 0x12: ((u8*)&lbl_eu_80577308)[0x30] = value; break;
        case 0x13: ((u8*)&lbl_eu_80577308)[0x32] = value; break;
        case 0x14: ((u8*)&lbl_eu_80577308)[0x31] = value; break;
    }
}

// Look up a signed-index table entry via the sum of the signed 0x2D/0x2C bytes.
extern "C" u8 __declspec(noinline) func_8029D96C(COption* self) {
    return lbl_eu_8053948C[(s8)self->field_0x2C + (s8)self->field_0x2D];
}

// func_8029D990: cursor-down config handler (0x2E mode). Increment the config
// byte for the current cell (s8 0x2C + s8 0x2D), clamping against the per-cell
// max from func_8029D96C; cells 0x0A-0x0D decrement with wrap instead. Then
// refresh the panel text and re-show the cursor. The config base address is
// re-derived inside every case body (retail emits lis/addi per case - a hoisted
// pointer would change the shape); the switch's own bounds check is the retail
// cmplwi/bgt guard that skips to the shared tail. The decrement cases must go
// through a typed Class_80296898* local with compound `--` (not the flat u8*
// byte-cast form of the increment cases): the typed access colors the base
// r5 / value r4 like retail, where the flat form swaps them (base r4 / value
// r5) across the if/else branch.
void __declspec(noinline) func_8029D990(COption* self) {
    // Mask the u8 call result eagerly (retail emits clrlwi r3,r3,24 right after
    // the bl; a plain `u8 amount = ...` lets MWCC sink the truncation into each
    // case body, adding a per-case rlwinm that retail does not have).
    u32 amount = (u32)func_8029D96C(self) & 0xFF;
    int cell = (s8)self->field_0x2C + (s8)self->field_0x2D;
    switch (cell) {
        case 0x00:
            ((u8*)&lbl_eu_80577308)[0x1c] = (u8)(((u8*)&lbl_eu_80577308)[0x1c] + 1);
            if (((u8*)&lbl_eu_80577308)[0x1c] >= amount) ((u8*)&lbl_eu_80577308)[0x1c] = 0;
            break;
        case 0x01:
            ((u8*)&lbl_eu_80577308)[0x0f] = (u8)(((u8*)&lbl_eu_80577308)[0x0f] + 2);
            if (((u8*)&lbl_eu_80577308)[0x0f] > amount) ((u8*)&lbl_eu_80577308)[0x0f] = 8;
            break;
        case 0x02:
            ((u8*)&lbl_eu_80577308)[0x20] = (u8)(((u8*)&lbl_eu_80577308)[0x20] + 1);
            if (((u8*)&lbl_eu_80577308)[0x20] >= amount) ((u8*)&lbl_eu_80577308)[0x20] = 0;
            break;
        case 0x03:
            ((u8*)&lbl_eu_80577308)[0x21] = (u8)(((u8*)&lbl_eu_80577308)[0x21] + 1);
            if (((u8*)&lbl_eu_80577308)[0x21] >= amount) ((u8*)&lbl_eu_80577308)[0x21] = 0;
            break;
        case 0x04:
            ((u8*)&lbl_eu_80577308)[0x23] = (u8)(((u8*)&lbl_eu_80577308)[0x23] + 1);
            if (((u8*)&lbl_eu_80577308)[0x23] >= amount) ((u8*)&lbl_eu_80577308)[0x23] = 0;
            break;
        case 0x05:
            ((u8*)&lbl_eu_80577308)[0x22] = (u8)(((u8*)&lbl_eu_80577308)[0x22] + 1);
            if (((u8*)&lbl_eu_80577308)[0x22] >= amount) ((u8*)&lbl_eu_80577308)[0x22] = 0;
            break;
        case 0x06:
            ((u8*)&lbl_eu_80577308)[0x24] = (u8)(((u8*)&lbl_eu_80577308)[0x24] + 1);
            if (((u8*)&lbl_eu_80577308)[0x24] >= amount) ((u8*)&lbl_eu_80577308)[0x24] = 0;
            break;
        case 0x07:
            ((u8*)&lbl_eu_80577308)[0x25] = (u8)(((u8*)&lbl_eu_80577308)[0x25] + 1);
            if (((u8*)&lbl_eu_80577308)[0x25] >= amount) ((u8*)&lbl_eu_80577308)[0x25] = 0;
            break;
        case 0x08:
            ((u8*)&lbl_eu_80577308)[0x00] = (u8)(((u8*)&lbl_eu_80577308)[0x00] + 1);
            if (((u8*)&lbl_eu_80577308)[0x00] >= amount) ((u8*)&lbl_eu_80577308)[0x00] = 0;
            break;
        case 0x09:
            ((u8*)&lbl_eu_80577308)[0x01] = (u8)(((u8*)&lbl_eu_80577308)[0x01] + 1);
            if (((u8*)&lbl_eu_80577308)[0x01] >= amount) ((u8*)&lbl_eu_80577308)[0x01] = 0;
            break;
        case 0x0a: {
            Class_80296898* saved = &lbl_eu_80577308;
            if (saved->mSlotFlags[2] == 0) saved->mSlotFlags[2] = amount - 1;
            else saved->mSlotFlags[2]--;
            break;
        }
        case 0x0b: {
            Class_80296898* saved = &lbl_eu_80577308;
            if (saved->mSlotFlags[3] == 0) saved->mSlotFlags[3] = amount - 1;
            else saved->mSlotFlags[3]--;
            break;
        }
        case 0x0c: {
            Class_80296898* saved = &lbl_eu_80577308;
            if (saved->mSlotFlags[4] == 0) saved->mSlotFlags[4] = amount - 1;
            else saved->mSlotFlags[4]--;
            break;
        }
        case 0x0d: {
            Class_80296898* saved = &lbl_eu_80577308;
            if (saved->mSlotFlags[5] == 0) saved->mSlotFlags[5] = amount - 1;
            else saved->mSlotFlags[5]--;
            break;
        }
        case 0x0e:
            ((u8*)&lbl_eu_80577308)[0x06] = (u8)(((u8*)&lbl_eu_80577308)[0x06] + 1);
            if (((u8*)&lbl_eu_80577308)[0x06] >= amount) ((u8*)&lbl_eu_80577308)[0x06] = 0;
            break;
        case 0x0f:
            ((u8*)&lbl_eu_80577308)[0x07] = (u8)(((u8*)&lbl_eu_80577308)[0x07] + 1);
            if (((u8*)&lbl_eu_80577308)[0x07] >= amount) ((u8*)&lbl_eu_80577308)[0x07] = 0;
            break;
        case 0x10:
            ((u8*)&lbl_eu_80577308)[0x10] = (u8)(((u8*)&lbl_eu_80577308)[0x10] + 1);
            if (((u8*)&lbl_eu_80577308)[0x10] >= amount) ((u8*)&lbl_eu_80577308)[0x10] = 0;
            break;
        case 0x11:
            ((u8*)&lbl_eu_80577308)[0x11] = (u8)(((u8*)&lbl_eu_80577308)[0x11] + 1);
            if (((u8*)&lbl_eu_80577308)[0x11] >= amount) ((u8*)&lbl_eu_80577308)[0x11] = 0;
            break;
        case 0x12:
            ((u8*)&lbl_eu_80577308)[0x30] = (u8)(((u8*)&lbl_eu_80577308)[0x30] + 1);
            if (((u8*)&lbl_eu_80577308)[0x30] >= amount) ((u8*)&lbl_eu_80577308)[0x30] = 0;
            break;
        case 0x13:
            ((u8*)&lbl_eu_80577308)[0x32] = (u8)(((u8*)&lbl_eu_80577308)[0x32] + 1);
            if (((u8*)&lbl_eu_80577308)[0x32] >= amount) ((u8*)&lbl_eu_80577308)[0x32] = 0;
            break;
        case 0x14:
            ((u8*)&lbl_eu_80577308)[0x31] = (u8)(((u8*)&lbl_eu_80577308)[0x31] + 1);
            if (((u8*)&lbl_eu_80577308)[0x31] >= amount) ((u8*)&lbl_eu_80577308)[0x31] = 0;
            break;
    }
    func_8029D420(self);
    func_8029C0C4(reinterpret_cast<COption*>(&self->mSubCur2));
}

extern "C" void func_8029DD6C(COption* self){
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
// transform twice (read/writable) before leaving idle mode. Each call reloads
// mSubObj from memory (retail shape) - a cached local would claim r30.
extern "C" void __declspec(noinline) func_8029E144(COption* self) {
    self->mSubObj->v7();
    self->mSubObj->v5(self->mAnimTransform20);
    self->mSubObj->v9(self->mAnimTransform20, 1);
    self->mSubObj->v12(0);
}

// Configure the +0x1C sub-object animation state using the +0x24 transform.
extern "C" __declspec(noinline) void func_8029E1CC(COption* self) {  // noinline: -ipa would fold this into func_8029D0C0/D10C call sites (retail keeps bl)
    self->mSubObj->v7();
    self->mSubObj->v5(self->mAnimTransform24);
    self->mSubObj->v9(self->mAnimTransform24, 1);
    self->mSubObj->v12(0);
}

// func_8029E254: refresh the option cursor panes. When the system window is
// busy, copy its current state into cursor 3; otherwise show the config row
// value (0x2E mode -> sub-cursor 2, normal -> sub-cursor 1) as a message pane
// resolved from the +0x1C sub-object's message source. The sub-object and its
// message source are re-read from memory at every use (retail shape).
// stmw/lmw frame needs optimize_for_size. Locals are declared at function
// scope in reverse-allocation order so MWCC places them at the retail stack
// offsets (0x50/0x30/0x20/0x14/0x08) and overlaps the branch-exclusive
// sprintf buffers.
#pragma optimize_for_size on
extern "C" void __declspec(noinline) func_8029E254(COption* self) {
    char bufMode[0x20];       // 0x2E-mode sprintf buffer -> sp+0x50
    char bufCfg[0x20];        // normal-mode sprintf buffer -> sp+0x30
    u8 tmp[0x10];             // window-state copy -> sp+0x20
    nw4r::math::VEC3 tmp2;    // 0x2E-mode pane layout out -> sp+0x14
    nw4r::math::VEC3 tmp3;    // normal-mode pane layout out -> sp+0x08
    if (CSysWin_getUnk34(self->mSysWin) != 0) {
        func_8022C1B4(tmp, self->mSysWin, self->field_0x31);
        reinterpret_cast<COptionCurView*>(&self->mSubCur3)->vf04(tmp);
    } else if (self->field_0x2E != 0) {
        sprintf(bufMode, &lbl_eu_805103D8[0xdc], (s8)self->field_0x2C + 1);
        nw4r::lyt::Pane* p1 =
            reinterpret_cast<COptionSubData*>(self->mSubObj)->mMsgObj->v0D(bufMode, 1);
        nw4r::lyt::Pane* p2 = reinterpret_cast<COptionSubData*>(self->mSubObj)
            ->mMsgObj->v0D(&lbl_eu_805103D8[0xe8], 1);
        func_80137924(&tmp2, p1, p2,
                      reinterpret_cast<nw4r::lyt::Pane*>(
                          reinterpret_cast<COptionSubData*>(self->mSubObj)->mMsgObj));
        reinterpret_cast<COptionCurView*>(&self->mSubCur2)->vf04((u8*)&tmp2);
    } else {
        sprintf(bufCfg, &lbl_eu_805103D8[0xa5], (s8)self->field_0x2C + 1);
        nw4r::lyt::Pane* p1 =
            reinterpret_cast<COptionSubData*>(self->mSubObj)->mMsgObj->v0D(bufCfg, 1);
        nw4r::lyt::Pane* p2 = reinterpret_cast<COptionSubData*>(self->mSubObj)
            ->mMsgObj->v0D(&lbl_eu_805103D8[0xe8], 1);
        func_80137924(&tmp3, p1, p2,
                      reinterpret_cast<nw4r::lyt::Pane*>(
                          reinterpret_cast<COptionSubData*>(self->mSubObj)->mMsgObj));
        reinterpret_cast<COptionCurView*>(&self->mSubCur1)->vf04((u8*)&tmp3);
    }
}
#pragma optimize_for_size off

// Returns "config changed": 1 when any probed byte of the config singleton
// differs from the TU local copy. The final 0x31 comparison is emitted inline
// (the last one falls through to the return). The slot-flag 0x00 read goes
// through the typed symbol so MWCC reuses the lis base (@l reloc on r6) like
// retail; the rest use the flat u8* view.
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
    if (lbl_eu_80577308.mSlotFlags[0x00] != b[0x00]) return 1;
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
    return b[0x31] != g[0x31];
}

// COption::~COption() - complete-object destructor (retail __dt__7COptionFv).
// Written as the retail C-linkage free function (CSysWinSave precedent) so the
// sub-object dtors can pass the explicit -1 embedded-member flags, the addic.
// null-guard on mSubCur2 (MWCC D2-inlined-into-D1 artifact) survives, and the
// delete is gated on the incoming flags word like the retail D0 shape. The
// stmw/lmw frame needs optimize_for_size (same shape as ~CSysWin).
#pragma optimize_for_size on
extern "C" void* __dt__7COptionFv(COption* _this, int flags) {
    if (_this != 0) {
        __dt__6CCur18Fv(&_this->mSubCur3, -1);
        __dt__7CSysWinFv(_this->mSysWin, -1);
        CBaseCur* subCur2 = &_this->mSubCur2;
        if (subCur2 != 0) {
            __dt__8CBaseCurFv(subCur2, 0);
        }
        __dt__6CCur18Fv(&_this->mSubCur1, -1);
        __dt__10CScrollBarFv(reinterpret_cast<CScrollBar*>(&_this->mScrollBar), -1);
        // Member-dtor call emits the retail __dt__17UnkClass_8045F564Fv with r4=-1.
        UnkClass_8045F564* memRegion = reinterpret_cast<UnkClass_8045F564*>(&_this->mArcResAcc);
        memRegion->~UnkClass_8045F564();
        if (flags > 0) {
            __dl__FPv(_this);
        }
    }
    return _this;
}
#pragma optimize_for_size off

// File-event callback: the option layout arc finished loading. Open a scratch
// region (RAII Class_8045F858 guard), detach the file buffer, attach it to a
// fresh ArcResourceAccessor, build the layout + two animations, bind the font,
// then construct the three embedded cursors on the stack, copy their bodies,
// and tear the temps down. stmw/lmw frame needs optimize_for_size.
#pragma optimize_for_size on
bool COption::OnFileEvent(CEventFile* pEventFile) {
    COptionMem* mem = reinterpret_cast<COptionMem*>(this);
    if (mem->mFHandle == pEventFile->mFileHandle) {
        // Set up the scratch heap region, then attach the freshly-exchanged file
        // data as the nw4r layout archive and build the layout + two animations.
        mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
        mem->mRegion.createRegion(mem2, 0x10000, &lbl_eu_805103D8[0xf7], 0);
        Class_8045F858 memHost(&mem->mRegion);
        u8 tempA[0x18];   // CCur18 build temp (cursor 1)
        u8 tempB[0x18];   // CCur19 build temp (cursor 2)
        u8 tempC[0x18];   // CCur18 build temp (cursor 3)
        void* fileData = mem->mFHandle->getData();
        mtl::MemManager::func_80434A4C(false);
        mem->mArcAcc = createArcResourceAccessor__10CLibLayoutFv();
        mem->mArcAcc->Attach(fileData, &lbl_eu_805103D8[0xff]);
        func_80136E84(&mem->mpLayout, mem->mArcAcc, &lbl_eu_805103D8[0x103]);
        func_80136F08(mem->mpLayout, &mem->mAnim0, mem->mArcAcc, &lbl_eu_805103D8[0x11a]);
        func_80136F08(mem->mpLayout, &mem->mAnim1, mem->mArcAcc, &lbl_eu_805103D8[0x134]);

        // Bind the font: take the layout root pane, ask the font object for its
        // pane, and push it back onto the root.
        nw4r::lyt::Pane* rootPane = *(nw4r::lyt::Pane**)((u8*)mem->mpLayout + 0x10);
        void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mem->mpLayout);
        func_8013676C(rootPane, reinterpret_cast<COptionFontView*>(fontObj)->vf7());

        func_8029E144(this);

        // Build sub-cursor 1 (CCur18) on the stack and copy it into +0x78.
        __ct__CCur18(tempA, func_801355F4());
        func_8018B0FC(&mSubCur1, tempA);
        __dt__6CCur18Fv(tempA, -1);
        reinterpret_cast<COptionCurTearView*>(&mSubCur1)->vf0();

        // Build sub-cursor 2 (CCur19) on the stack and copy it into +0x90.
        __ct__CCur19((CBaseCur*)tempB, mem->mArcAcc);
        func_8018B0FC(&mSubCur2, tempB);
        __dt__8CBaseCurFv(tempB, 0);
        reinterpret_cast<COptionCurTearView*>(&mSubCur2)->vf0();

        // Build sub-cursor 3 (CCur18) on the stack and copy it into +0xE4.
        __ct__CCur18(tempC, func_801355F4());
        func_8018B0FC(&mSubCur3, tempC);
        __dt__6CCur18Fv(tempC, -1);
        reinterpret_cast<COptionCurTearView*>(&mSubCur3)->vf0();

        func_8029D3C0(this);
        mem->mFHandle = 0;
        mem->mRegion.func_8045F810();
        return true;
    }
    return false;
}
#pragma optimize_for_size off


// --- hard-symbol stubs (scaffold_hard_symbols) ---

// sinit: constructor run for the Class_80296898 global at lbl_eu_80577308.
// Tail-calls the global constructor with the object address.
extern "C" void sinit_8029E7D8(){
    func_80296A04__FP14Class_80296898(&lbl_eu_80577308);
}

// func_8029BF68: load the option layout arc. Build the layout + two animation
// transforms from the arc accessor, unbind any stale animations, run the shared
// cursor routine, then hide/show the two pane labels. stmw/lmw frame needs
// optimize_for_size (same shape as ~COption / func_8029C5C8).
#pragma optimize_for_size on
extern "C" void func_8029BF68(COption* self) {
    func_80136E84(&self->mpLayout, self->mArcResAcc, lbl_eu_805103D8);
    func_80136F08(self->mpLayout, &self->mpAnimTrans0, self->mArcResAcc, &lbl_eu_805103D8[0x18]);
    func_80136F08(self->mpLayout, &self->mpAnimTrans1, self->mArcResAcc, &lbl_eu_805103D8[0x35]);
    self->mpLayout->UnbindAllAnimation();
    func_801D21CC(self);
    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_805103D8[0x50], true), 0);
    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_805103D8[0x5E], true), 0);
}
#pragma optimize_for_size off
// func_8029C35C: (re)build the option-panel widgets. Reads the option layout
// arc as a work event into +0x14, then re-constructs the embedded scroll bar
// (+0x38) and system window (+0xA8) by building stack temps and copying their
// non-vtable bodies (16-byte mem region via __ct__UnkClass_8011C974, then
// memberwise), and finally dispatches the window layout-build virtual. All
// temp/member accesses are written inline so MWCC emits sp/r31-relative
// loads/stores (a cached pointer local would claim callee-saved registers).
extern "C" void func_8029C35C(COption* self) {
    mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
    reinterpret_cast<COptionMem*>(self)->mFHandle =
        (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
            (u32)mem2, &lbl_eu_805103D8[0x6c], reinterpret_cast<IWorkEvent*>(self), 0, 0);

    u8 tempSb[0x40];
    __ct__CScrollBar(reinterpret_cast<CScrollBar*>(tempSb), 1);
    __ct__UnkClass_8011C974((u8*)self->mScrollBar + 4, tempSb + 4);
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mFileHandle =
        reinterpret_cast<CScrollBar*>(tempSb)->mFileHandle;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mAccessor =
        reinterpret_cast<CScrollBar*>(tempSb)->mAccessor;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mLayout =
        reinterpret_cast<CScrollBar*>(tempSb)->mLayout;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mAnimTransform =
        reinterpret_cast<CScrollBar*>(tempSb)->mAnimTransform;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mReady =
        reinterpret_cast<CScrollBar*>(tempSb)->mReady;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mVisible =
        reinterpret_cast<CScrollBar*>(tempSb)->mVisible;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mState =
        reinterpret_cast<CScrollBar*>(tempSb)->mState;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mActive =
        reinterpret_cast<CScrollBar*>(tempSb)->mActive;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mAnimOffset =
        reinterpret_cast<CScrollBar*>(tempSb)->mAnimOffset;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mScrollPosY =
        reinterpret_cast<CScrollBar*>(tempSb)->mScrollPosY;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mScrollRatio =
        reinterpret_cast<CScrollBar*>(tempSb)->mScrollRatio;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mThumbHeight =
        reinterpret_cast<CScrollBar*>(tempSb)->mThumbHeight;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mContentHeight =
        reinterpret_cast<CScrollBar*>(tempSb)->mContentHeight;
    reinterpret_cast<CScrollBar*>(self->mScrollBar)->mDirection =
        reinterpret_cast<CScrollBar*>(tempSb)->mDirection;
    __dt__10CScrollBarFv(reinterpret_cast<CScrollBar*>(tempSb), -1);

    func_801F34F4(reinterpret_cast<CScrollBar*>(self->mScrollBar));

    u8 tempW[0x3C];
    __ct__CSysWin(reinterpret_cast<CSysWin*>(tempW), 2);
    __ct__UnkClass_8011C974((u8*)self->mSysWin + 4, tempW + 4);
    reinterpret_cast<CSysWin*>(self->mSysWin)->mFileHandle =
        reinterpret_cast<CSysWin*>(tempW)->mFileHandle;
    reinterpret_cast<CSysWin*>(self->mSysWin)->mTagProcessor =
        reinterpret_cast<CSysWin*>(tempW)->mTagProcessor;
    reinterpret_cast<CSysWin*>(self->mSysWin)->mArcAccessor =
        reinterpret_cast<CSysWin*>(tempW)->mArcAccessor;
    reinterpret_cast<CSysWin*>(self->mSysWin)->mLayout =
        reinterpret_cast<CSysWin*>(tempW)->mLayout;
    reinterpret_cast<CSysWin*>(self->mSysWin)->mAnimTrans =
        reinterpret_cast<CSysWin*>(tempW)->mAnimTrans;
    reinterpret_cast<CSysWin*>(self->mSysWin)->field_28 =
        reinterpret_cast<CSysWin*>(tempW)->field_28;
    reinterpret_cast<CSysWin*>(self->mSysWin)->field_2C =
        reinterpret_cast<CSysWin*>(tempW)->field_2C;
    reinterpret_cast<CSysWin*>(self->mSysWin)->field_30 =
        reinterpret_cast<CSysWin*>(tempW)->field_30;
    reinterpret_cast<CSysWin*>(self->mSysWin)->field_34 =
        reinterpret_cast<CSysWin*>(tempW)->field_34;
    reinterpret_cast<CSysWin*>(self->mSysWin)->field_35 =
        reinterpret_cast<CSysWin*>(tempW)->field_35;
    reinterpret_cast<CSysWin*>(self->mSysWin)->field_36 =
        reinterpret_cast<CSysWin*>(tempW)->field_36;
    reinterpret_cast<CSysWin*>(self->mSysWin)->field_37 =
        reinterpret_cast<CSysWin*>(tempW)->field_37;
    reinterpret_cast<CSysWin*>(self->mSysWin)->field_38 =
        reinterpret_cast<CSysWin*>(tempW)->field_38;
    reinterpret_cast<CSysWin*>(self->mSysWin)->field_39 =
        reinterpret_cast<CSysWin*>(tempW)->field_39;
    __dt__7CSysWinFv(reinterpret_cast<CSysWin*>(tempW), -1);

    reinterpret_cast<COptionSysWinView*>(&self->mSysWin)->v20();
    self->field_0x2A = 0;
}

// func_8029C4F4: option-panel state tick. When the panel is live (0x28), jump
// through the state table (1,2,4,5,6,7,8,10; 0/3/9 fall through) to the
// per-state animation/window handlers, then always idle the +0x1C sub-object
// and update the scroll bar, the three cursors and the system window.
extern "C" void func_8029C4F4(COption* self) {
    if (self->field_0x28 != 0) {
        switch (self->field_0x29) {
        case 1: func_8029D0C0(self); break;
        case 2: func_8029D10C(self); break;
        case 4: func_8029D178(self); break;
        case 5: func_8029D1C4(self); break;
        case 6: func_8029D210(self); break;
        case 7: func_8029D278(self); break;
        case 8: func_8029D2F0(self); break;
        case 10: func_8029D358(self); break;
        default: break;
        }
        self->mSubObj->v12(0);
        func_801F3540(self->mScrollBar);
        func_801D202C(&self->mSubCur1);
        func_801D202C(&self->mSubCur2);
        func_8022B748(self->mSysWin);
        func_801D202C(&self->mSubCur3);
    }
}

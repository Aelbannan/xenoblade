// Auto-scaffolded catalog TU for kyoshin/CQuestWindow
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CQuestWindow.hpp"
#include "kyoshin/code_80135FDC.hpp"              // func_80137444 (anim frame wait)
#include "kyoshin/cf/CfPadTask.hpp"               // cf::CfPadTask::setInputDisableTime
#include "kyoshin/CTagProcessor.hpp"              // CTagProcessor (tag alloc)
// Implicit-r3 form: retail call sites that do NOT set r3 (the def in
// ocBdat.cpp takes no args); header decl is the 1-arg caller-tuned form.
extern "C" void* func_8003AA34(void);
#include "monolib/device/CDeviceFile.hpp"         // readFile / readCommonArchiveFile
#include "monolib/device/CDeviceFont.hpp"         // CDeviceFont::getFontInfo
#include "monolib/device/CDeviceVI.hpp"          // waitForDrawDone
#include "monolib/util/MemManager.hpp"            // mtl::MemManager::allocate / getHandleMEM2
#include "monolib/work/CWorkThreadSystem.hpp"     // CWorkThreadSystem::getWorkMem
#include <new>
#include <revolution/gx/GXPixel.h>                 // GXSetZMode

// func_80122BB0 calls func_801242FC, which is defined later in this TU.
// extern "C": the retail symbol is unmangled, so both the definition and the
// call reloc must carry C linkage (CCur.cpp precedent).
extern "C" void func_801242FC(QuestWinObj* self);

u32 func_80122448(void) {
    extern u32 lbl_eu_80663FD0;
    return lbl_eu_80663FD0;
}

// (lbl_eu_80663FD0 != 0) - retail lwz sda21; subic; subfe
extern "C" bool func_80122450() { extern u32 lbl_eu_80663FD0; return lbl_eu_80663FD0 != 0; }

int lbl_eu_80663D1C;

int func_80042864() {
    return lbl_eu_80663D1C != 0;
}

extern u32 lbl_eu_80663FD0;
void func_80122460(){ lbl_eu_80663FD0 = 0; }

// Install the retail Move-hook ptmf (+0x3C) only while no party / other menu
// is active (func_80293C10, func_80192BD0) and the window's own idle gates
// (0x88/0x8C) are both clear.
void func_80122654(QuestWinObj* self) {
    if (func_80293C10() == 0 && func_80192BD0() == 0 &&
        self->field_0x88 == 0 && self->field_0x8C == 0) {
        self->mMoveFunc = lbl_eu_8052D074;
    }
}

// ---------------------------------------------------------------------------
// func_801226C8 (us-801231a4): quest-window state driver. Gate on the scene
// being active, dispatch the window state byte (0..3), then always advance the
// layout animation and run the cursor update.
// ---------------------------------------------------------------------------
void func_801226C8(QuestWinObj* self) {
    if (func_8013BE50() == 0) return;
    switch (self->field_0xBC) {
    case 0:
        setPresentationFlag__Q22cf13CfGameManagerFv(true);
        self->field_0xBC = 1;
        func_801D216C(&self->mCursor, 0);
        func_8012435C(self);
        break;
    case 1:
        func_80122BB0(self);
        break;
    case 2:
        func_80122C08(self);
        break;
    case 3:
        func_80122EF8(self);
        break;
    }
    self->mpLayout->Animate(0);
    func_801D202C(&self->mCursor);
}

// ---------------------------------------------------------------------------
// func_8012278C (us-80123268): quest-window file-event handler, dispatched by
// CQuestWindow::OnFileEvent with this - 0x6C. The event's file handle picks
// the branch: the window arc (field_0x88) builds the whole layout - scratch
// region (RAII Class_8045F858 guard), tag processor, resource accessor,
// layout + both anim transforms, font bind, tag-processor pane links, cursor
// rebuild - then refreshes the content (func_801231C4). The common archive
// (field_0x8C) attaches the quest-name file pointer to +0xD0 and refreshes
// too. Both branches clear the consumed handle; unmatched events return false.
// ---------------------------------------------------------------------------
// C linkage comes from the header's extern "C" block (retail name unmangled).
bool func_8012278C(CQuestWindow* self, CEventFile* event) {
    CFileHandle* evt = event->mFileHandle;
    if (self->field_0x88 == evt) {
        // Scratch heap region (RAII Class_8045F858 guard), then detach the
        // file buffer for the arc resource accessor (CQstLogList idiom).
        u8 regionBuf[8];
        u32 mem2 = mtl::MemManager::getHandleMEM2();
        self->mMemRegion.createRegion(mem2, 0x10000, &lbl_eu_804FEC84[0], 0);
        __ct__14Class_8045F858FP17UnkClass_8045F564(regionBuf, &self->mMemRegion);

        u8* fileData = (u8*)self->field_0x88->getData();
        mtl::MemManager::setMemInitFlag(false);
        CTagProcessor* tag = (CTagProcessor*)mtl::MemManager::allocate(
            0x858, getAllocHandle__10CLibLayoutFv());
        if (tag != 0) {
            // Retail keeps the tag in r3 across the ctor call: __ct__CTagProcessor
            // returns its object pointer, which feeds the +0x90 store directly.
            tag = (CTagProcessor*)__ct__CTagProcessor(tag);
        }
        self->field_0x90 = (CQuestWinRes*)tag;
        self->field_0x94 = createArcResourceAccessor__10CLibLayoutFv();
        self->field_0x94->Attach(fileData, &lbl_eu_804FEC84[0xd]);
        func_80136E84(&self->mpLayout, self->field_0x94, &lbl_eu_804FEC84[0x11]);
        func_80136F08(self->mpLayout, &self->mAnimA, self->field_0x94,
                      &lbl_eu_804FEC84[0x29]);
        func_80136F08(self->mpLayout, &self->mAnimB, self->field_0x94,
                      &lbl_eu_804FEC84[0x44]);

        // Bind the font handle into the layout's root pane.
        nw4r::lyt::Pane* rootPane = self->mpLayout->GetRootPane();
        u8* fontObj = (u8*)CDeviceFont::getFontInfo(1, self->mpLayout);
        u32 fontResult = reinterpret_cast<CQuestWinFontView*>(fontObj)->v7();
        func_8013676C(rootPane, fontResult);

        char* fontStr = (char*)func_801355BC();
        if (fontStr != 0) {
            func_801368C0(self->mpLayout, &lbl_eu_804FEC84[0x61], (u32)fontStr);
            func_801368C0(self->mpLayout, &lbl_eu_804FEC84[0x6c], (u32)fontStr);
            func_801368C0(self->mpLayout, &lbl_eu_804FEC84[0x77], (u32)fontStr);
        }
        char* s =
            func_80136190(&lbl_eu_804FEC84[0x7f], &lbl_eu_804FEC84[0x88], 3);
        func_80136B4C(self->mpLayout, &lbl_eu_804FEC84[0x6c], s, 0);

        // TextBox::mpTagProcessor at +0xF8 (MonolithSoft nw4r extension).
        struct TextBoxLayout {
            u8 _[0xF8];
            void* mpTagProcessor;
        };
        {
            TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
                self->mpLayout->GetRootPane()->FindPaneByName(
                    &lbl_eu_804FEC84[0x8d], true));
            t->mpTagProcessor = self->field_0x90;
        }
        {
            TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
                self->mpLayout->GetRootPane()->FindPaneByName(
                    &lbl_eu_804FEC84[0x96], true));
            t->mpTagProcessor = self->field_0x90;
        }

        func_8012429C((QuestWinObj*)self);
        self->mAnimA->SetFrame(lbl_eu_8066713C);
        self->mpLayout->Animate(0);

        // Build the cursor on the stack, copy its body into the member region
        // (skipping the +0x00 vtable pointer) and destroy the temp.
        u8 tmpCur[0x18];
        __ct__CCur18(tmpCur, func_801355F4());
        CCur18Data* curDst = reinterpret_cast<CCur18Data*>(&self->mCursor[0]);
        CCur18Data* curSrc = reinterpret_cast<CCur18Data*>(tmpCur);
        curDst->f_04 = curSrc->f_04;
        curDst->f_08 = curSrc->f_08;
        curDst->f_0c = curSrc->f_0c;
        curDst->f_10 = curSrc->f_10;
        curDst->f_14 = curSrc->f_14;
        curDst->f_15 = curSrc->f_15;
        __dt__6CCur18Fv(tmpCur, -1);
        reinterpret_cast<CCur18View*>(&self->mCursor[0])->vf02();

        func_801231C4(self);
        self->field_0x88 = 0;
        self->mMemRegion.func_8045F810();
        __dt__14Class_8045F858Fv(regionBuf, -1);
        return true;
    }
    if (self->field_0x8C == evt) {
        u8* fileData = (u8*)self->field_0x8C->getData();
        func_8003AA78__5CBdatFUlPv(2, fileData);
        func_8003AA34();
        self->field_0xD0 =
            (u32)getFP__FPCc(lbl_eu_8052CFF4[func_80138138(self->field_B8)]);
        func_801231C4(self);
        self->field_0x8C = 0;
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// CQuestWindow::cbRenderBefore (us-8012355c): gate on the task flags / scene
// activity, then draw the layout through a stack DrawInfo, plus the cursor
// when the +0xC0 gate is non-negative.
// ---------------------------------------------------------------------------
void CQuestWindow::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000)) return;
    if (func_8013BE50() == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls
    // to match the retail direct calls (a C++ local would virtual-dispatch
    // its scope-exit destructor and bloat the body).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137038(mpLayout, (nw4r::lyt::DrawInfo*)&drawInfo[0], 0, 1);
    if (field_0xC0 >= 0) {
        func_801D20B0(&mCursor[0], (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    }
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
}

// ---------------------------------------------------------------------------
// __ct__CQuestWindow (us-80122c40): quest-window constructor. Retail C-ABI
// symbol (unmangled), so it is written as a global function (CSysWinSave /
// COption idiom). Constructs the CProcess base, writes the temp vtable then
// the composite vtable (+0x10) with the +0x24/+0xAC dispatch slots, installs
// the null Move/Draw ptmfs, initialises the scalar fields, constructs the
// embedded UnkClass_8045F564 / CCur18, bumps the window counter and finishes
// with the initial Move hook (lbl_eu_8052D068).
// ---------------------------------------------------------------------------
extern "C" void* __ct__CQuestWindow(CQuestWindow* self, u32 arg1, u32 arg2, u32 arg3) {
    u32 padFlags;
    s32 isClassic;
    __ct__8CProcessFv((CProcess*)self);
    self->mVtable = (u32)lbl_eu_8052D238;

    // Null ptmf into both callback slots (12-byte struct copies: MWCC emits
    // the lwzu base-establish + load-both/store-swapped idiom retail shows).
    const CTTask<IUIWindow>::MoveFunc* nullPtmf =
        reinterpret_cast<const CTTask<IUIWindow>::MoveFunc*>(__ptmf_null);
    self->mMoveFunc = *nullPtmf;
    self->mDrawFunc = *nullPtmf;

    self->mpLayout = 0;
    self->field_58 = 0;
    self->field_5C = 0;
    self->field_60 = -1;
    self->field_64 = 0;
    self->field_65 = 0;
    self->field_66 = 0;
    self->field_67 = 1;
    self->field_68 = 0;

    self->mVtable = (u32)lbl_eu_8052D0F0;
    self->field_6C = (u32)lbl_eu_8052D0F0 + 0x24;
    self->field_70 = (u32)lbl_eu_8052D0F0 + 0xAC;
    self->mpScn = (CScn*)arg1;
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);
    self->field_0x88 = 0;
    self->field_0x8C = 0;
    self->field_0x90 = 0;
    self->field_0x94 = 0;
    self->mAnimA = 0;
    self->mAnimB = 0;
    __ct__CCur18(&self->mCursor[0], 0);
    self->field_B8 = arg2;
    self->field_0xBC = 0;
    self->field_0xC0 = 0;
    self->field_0xC4 = 0;
    self->field_0xC8 = (u8)arg3;
    self->field_0xCC = 0;
    self->field_0xD0 = 0;
    self->field_0xDC = 0;
    self->field_0xE0 = 0;
    self->field_0xE4 = 0;
    self->field_0xE8 = 0;
    self->field_0xD4 = 0;
    self->field_0xD6 = 0;
    self->field_0xD8 = 0;
    self->field_0xDA = 0;

    lbl_eu_80663FD0 = lbl_eu_80663FD0 + 1;
    setPresentationFlag__Q22cf13CfGameManagerFv(true);
    code80135FDC_postIncByte_64080();
    self->field_0xE8 = getEnabledInputFlags__Q22cf13CfGameManagerFv();

    // Classic-pad pad-flag mask when the detected pad type is CLASSIC.
    isClassic = isClassicController__Q22cf13CfGameManagerFv(-1);
    padFlags = isClassic != 0 ? 0x61800C : 0x1803C;
    enablePadFlags__Q22cf13CfGameManagerFUlb(-1, 0);
    enablePadFlags__Q22cf13CfGameManagerFUlb(padFlags, 1);

    // Install the initial Move hook (12-byte struct copy).
    self->mMoveFunc = *reinterpret_cast<const CTTask<IUIWindow>::MoveFunc*>(
        lbl_eu_8052D068);
    return self;
}

// ---------------------------------------------------------------------------
// func_80122B2C (us-80123608): quest-window factory. The singleton guard
// (lbl_eu_80663FD4) returns 0 when the window already exists; otherwise the
// 0xEC-byte object is allocated on the work heap, constructed (retail C-ABI
// __ct__CQuestWindow) and registered under `parent`.
// ---------------------------------------------------------------------------
CQuestWindow* func_80122B2C(CProcess* parent, u32 arg1, u32 arg2, u32 arg3) {
    if (lbl_eu_80663FD4 != 0) {
        return 0;
    }
    mtl::ALLOC_HANDLE mem = CWorkThreadSystem::getWorkMem();
    CQuestWindow* obj = (CQuestWindow*)mtl::MemManager::allocate(0xec, mem);
    if (obj != 0) {
        obj = (CQuestWindow*)__ct__CQuestWindow(obj, arg1, arg2, arg3);
    }
    lbl_eu_80663FD4 = reinterpret_cast<QuestWindowState*>(obj);
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return reinterpret_cast<CQuestWindow*>(lbl_eu_80663FD4);
}

// Window open animation: wait until anim A (+0x98) reaches the completion
// frame, then run the layout's anim switch (func_801242FC), show the cursor
// at +0xA0 and mark the window running (state byte +0xBC = 2).
void func_80122BB0(QuestWinObj* self) {
    if (func_80137444(self->mAnimA, lbl_eu_80667140) != 0) {
        func_801242FC(self);
        func_801D216C(&self->mCursor, 1);
        self->field_0xBC = 2;
    }
}

// Empty stubs must stay noinline: their bodies are visible in this TU and
// MWCC would otherwise inline them into func_801226C8's switch, collapsing
// the case-2/3 dispatch and the func_8012435C call (retail keeps the calls).
// ---------------------------------------------------------------------------
// func_80122C08 (us-801236e4): quest-window input handling for the open
// window. Confirm button closes it; the turbo/edge flags on the cf pad move
// the cursor selection (field_0xC0, clamped per state byte). Always advances
// anim B at the tail.
// ---------------------------------------------------------------------------
__attribute__((noinline)) void func_80122C08(QuestWinObj* self) {
    if (self->field_0xC8 != 0) {
        CPadView* pad = (CPadView*)getCurrentPad__Q22cf13CfGameManagerFv();
        int classic = isClassicController__Q22cf13CfGameManagerFv(-1) != 0;
        u32 confirm = (classic != 0) ? ((pad->field_04 >> 21) & 1)
                                     : ((pad->field_04 >> 4) & 1);
        if (confirm != 0) {
            func_8012429C(self);
            func_80138078__FUl(3);
            self->field_0xBC = 3;
            func_801D216C(&self->mCursor, 0);
        }
    } else if (self->field_0xC4 != 0) {
        CPadView* pad = (CPadView*)getCfPadData__Q22cf13CfGameManagerFv();
        int classic = isClassicController__Q22cf13CfGameManagerFv(-1) != 0;
        u32 dLeft, dRight, confirm;
        if (classic != 0) {
            u32 turbo = pad->field_104;
            u32 pressed = pad->field_04;
            dLeft = (turbo & 0x8004) != 0;
            dRight = (((turbo >> 15) & 1) | ((turbo >> 28) & 1)) != 0;
            confirm = (pressed >> 21) & 1;
        } else {
            u32 turbo = pad->field_104;
            u32 pressed = pad->field_04;
            dLeft = (turbo & 0x8004) != 0;
            dRight = (((turbo >> 15) & 1) | ((turbo >> 28) & 1)) != 0;
            confirm = (pressed >> 4) & 1;
        }
        if (confirm != 0) {
            if (self->field_0xC0 >= 0) {
                self->field_0xC0 = 0;
                self->field_0x60 = 1;
                func_80138078__FUl(3);
                self->field_0xBC = 3;
                func_801D216C(&self->mCursor, 0);
                func_8012429C(self);
            }
        } else if (dLeft != 0) {
            self->field_0xC0 = self->field_0xC0 - 1;
            if (self->field_0xC0 < 0) self->field_0xC0 = 0;
            func_8012435C(self);
            func_80138078__FUl(1);
        } else if (dRight != 0) {
            self->field_0xC0 = self->field_0xC0 + 1;
            if (self->field_0xC0 > 0) self->field_0xC0 = 0;
            func_8012435C(self);
            func_80138078__FUl(1);
        }
    } else {
        CPadView* pad = (CPadView*)getCfPadData__Q22cf13CfGameManagerFv();
        int classic = isClassicController__Q22cf13CfGameManagerFv(-1) != 0;
        u32 dLeft, dRight, confirm;
        if (classic != 0) {
            u32 turbo = pad->field_104;
            u32 pressed = pad->field_04;
            dLeft = (turbo & 0x8004) != 0;
            dRight = (((turbo >> 15) & 1) | ((turbo >> 28) & 1)) != 0;
            confirm = (pressed >> 21) & 1;
        } else {
            u32 turbo = pad->field_104;
            u32 pressed = pad->field_04;
            dLeft = (turbo & 0x8004) != 0;
            dRight = (((turbo >> 15) & 1) | ((turbo >> 28) & 1)) != 0;
            confirm = (pressed >> 4) & 1;
        }
        if (confirm != 0) {
            if (self->field_0xC0 >= 0) {
                self->field_0x60 = self->field_0xC0 + 1;
                if (self->field_0xC0 != 0) {
                    code80135FDC_setByte_6405B();
                }
                func_80138078__FUl(3);
                self->field_0xBC = 3;
                func_801D216C(&self->mCursor, 0);
                func_8012429C(self);
            }
        } else if (dLeft != 0) {
            self->field_0xC0 = self->field_0xC0 - 1;
            if (self->field_0xC0 < 1) self->field_0xC0 = 1;
            func_8012435C(self);
            func_80138078__FUl(1);
        } else if (dRight != 0) {
            self->field_0xC0 = self->field_0xC0 + 1;
            if (self->field_0xC0 > 1) self->field_0xC0 = 0;
            func_8012435C(self);
            func_80138078__FUl(1);
        }
    }
    func_80137444(self->mAnimB, lbl_eu_80667140);
}

// ---------------------------------------------------------------------------
// func_80122EF8 (us-801239d4): quest-window close transition. Waits for the
// open animation, releases the party member slot, runs the per-member sound
// loop (indexed by field_0xDC), then plays the quest-complete jingle and the
// close-sequence sounds for the window's quest id.
// ---------------------------------------------------------------------------
__attribute__((noinline)) void func_80122EF8(QuestWinObj* self) {
    if (func_80137510(self->mAnimA, lbl_eu_80667140) == 0) return;
    if (self->field_0xCC > 0) {
        func_800451D8(self->field_0xCC, getPlayer__Q22cf13CfGameManagerFi(0));
    }
    self->field_64 = 1;
    if (self->field_0xC8 == 0) return;
    if (self->field_0xDC != 0) {
        f64 two52 = lbl_eu_80667160;
        for (int i = 1; i <= 8; i++) {
            void* obj = func_8009EC9C((u16)i);
            if (isResourceFlagSet__Q22cf13CfGameManagerFv(i) != 0 ||
                (i == 4 && func_800A32BC() != 0)) {
                char* p = (char*)obj + 0x3534;
                int v = 0x64;
                if (func_8026178C(p, 0x89) != 0) {
                    v = func_8025FB10(p, 0x89) + 0x64;
                }
                // int->double via the 0x4330 magic-high-word idiom: bits
                // (0x43300000|n) as f64 equal n+2^52; retail preloads the 2^52
                // correction constant (lbl_eu_80667160, f29) before the loop.
                union {
                    u32 w[2];
                    f64 d;
                } cvt;
                cvt.w[0] = 0x43300000;
                cvt.w[1] = (u32)(self->field_0xDC * v);
                f64 val = (cvt.d - two52) * lbl_eu_8066714C;
                s32 result =
                    (s32)(val + ((val > lbl_eu_8066713C) ? lbl_eu_80667150
                                                          : lbl_eu_80667158));
                func_800A21F8(obj, (u16)result, 0, 0);
            }
        }
    }
    func_80157184((s32)(func_801571FC() + self->field_0xE0));
    if (self->field_0xE4 != 1 && self->field_0xD4 != 0) {
        func_8013E2E0(self->field_0xD4, self->field_0xD6, self->field_0xD8,
                      0, 1, 1, 0, 1, 0);
    }
    u8 r = func_801361E8(lbl_eu_80573D18[func_80138138(self->field_B8)],
                         &lbl_eu_804FEC84[0xa2], self->field_B8);
    if (r == 0 || r == 3) {
        func_8013B428__FUl(0xb4);
        func_8013B428__FUl(0xb5);
        func_8013B428__FUl(0xb6);
        func_8013B428__FUl(0xb7);
        func_8013B428__FUl(0xb8);
    }
    if (self->field_B8 - 0x100 <= 4) {
        func_8009D018(self->field_B8 + 0x704, 2);
        char* msg = func_80136190(&lbl_eu_804FEC84[0xac], &lbl_eu_804FEC84[0x88],
                                  0x81);
        func_8013D55C(msg, 0, 0);
        // 5-byte patch table (f32 bits + trailing byte) indexed by (id-0x100).
        union {
            struct {
                u32 w;
                u8 b;
            };
            u8 bytes[5];
        } data;
        data.w = lbl_eu_80667144;
        data.b = lbl_eu_80667148;
        s16 s = (s16)func_8009CF8C(0x7fc);
        s = (s16)(s + (s8)data.bytes[self->field_B8 - 0x100]);
        func_8009D018(0x7fc, s);
        if (func_8015D310() != 0) {
            func_8015D3A0();
            func_8013E030();
        }
    }
}

// ---------------------------------------------------------------------------
// func_801231C4 (us-80123ca0): quest-window content refresh. Reads the quest
// row (lbl_eu_80573D18) for the pane values, toggles pane visibility from the
// quest-type byte, binds the shared 'timg' resource to the cursor panes, runs
// the per-slot name loops (C8==0 / C8!=0 variants with jump-table switches on
// the slot type) and finally re-registers the render callback.
// ---------------------------------------------------------------------------
// base is a preprocessor alias (not a held register): the retail re-materializes
// the string-base address six times (r22/r24/r29/r31) instead of pinning one
// register, so the inline &lbl_eu_804FEC84[x] form reproduces that. C linkage
// comes from the header's extern "C" block (retail name unmangled).
#define base (lbl_eu_804FEC84)
void func_801231C4(CQuestWindow* self) {
    if (self->field_0x94 == 0 || self->field_0xD0 == 0) return;
    u32 questId = self->field_B8;
    u32 questRow = lbl_eu_80573D18[func_80138138(questId)];

    self->field_0xDC =
        (func_80136254((const void*)questRow, &base[0xb5], questId) & 0xFFFF) * 10;
    self->field_0xE0 =
        (func_80136254((const void*)questRow, &base[0xc0], questId) & 0xFFFF) * 10;
    self->field_0xE4 = func_801361E8(questRow, &base[0xcd], questId);
    u8 v = func_801361E8(questRow, &base[0xa2], questId);
    if (v == 1) {
        func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0xd8], true), 0);
        func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0xe1], true), 0);
        func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0xea], true), 1);
        func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0xf5], true), 1);
    } else {
        func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0xd8], true), 1);
        func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0xe1], true), 1);
        func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0xea], true), 0);
        func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0xf5], true), 0);
    }
    char* res = 0;
    switch (v) {
    case 0:
        res = (char*)self->field_0x94->GetResource(0x74696D67, &base[0x100], 0);
        break;
    case 1:
        res = (char*)self->field_0x94->GetResource(0x74696D67, &base[0x114], 0);
        break;
    case 3:
        res = (char*)self->field_0x94->GetResource(0x74696D67, &base[0x128], 0);
        break;
    }
    if (res != 0) {
        func_80137E7C(self->mpLayout, &base[0x13c], res);
    }
    if ((questId - 0x100) <= 4) {
        if ((func_8009CF8C(questId + 0x220) & 0xFF) == 0) {
            func_8009D018(questId + 0x220, 1);
        }
    }
    self->field_0xC4 = func_801361E8(questRow, &base[0x144], questId);
    char* s;
    if (self->field_0xC8 != 0) {
        s = func_80136190(&base[0x14a], &base[0x88], 0x38);
    } else {
        s = func_80136190(&base[0x14a], &base[0x88], 0x2e);
    }
    func_80136B4C(self->mpLayout, &base[0x154], s, 0);
    s = func_8013639C((const void*)questRow, &base[0x15d], questId);
    func_80136B4C(self->mpLayout, &base[0x163], s, 0);
    u32 v172 = func_80136254((const void*)questRow, &base[0x172], questId);
    s = (char*)func_80138DA4(
        func_8013639C((const void*)lbl_eu_80664098, &base[0x88], v172 & 0xFFFF));
    func_80136B4C(self->mpLayout, &base[0x179], s, 0);
    s = func_8013639C((const void*)self->field_0xD0, &base[0x182], questId);
    func_80136B4C(self->mpLayout, &base[0x96], s, (u32)self->field_0x90);
    char* cur;
    if (self->field_0xC8 != 0) {
        if ((func_8009CF8C(questId + 0x220) & 1) != 0) {
            cur = func_8013639C((const void*)self->field_0xD0, &base[0x18a], questId);
        } else {
            cur = func_8013639C((const void*)self->field_0xD0, &base[0x195], questId);
        }
    } else {
        if (func_801361E8(questRow, &base[0x1a0], questId) != 0) {
            cur = func_8013639C((const void*)self->field_0xD0, &base[0x1ad], questId);
        } else {
            cur = func_80136190(&base[0x14a], &base[0x88], 0x34);
        }
    }
    func_80136B4C(self->mpLayout, &base[0x8d], cur, (u32)self->field_0x90);
    func_80136B4C(self->mpLayout, &base[0x1b7], &base[0x1c4], 0);
    func_80136B4C(self->mpLayout, &base[0x1c5], &base[0x1c4], 0);
    func_80136B4C(self->mpLayout, &base[0x1d2], &base[0x1c4], 0);
    if (self->field_0xC8 != 0) {
        if (func_8009ECF0() == questId) {
            func_8009ECFC(0);
        }
        u8 vcd = func_801361E8(questRow, &base[0xcd], questId);
        if (vcd == 1) {
            s = func_80136190(&base[0x14a], &base[0x88], 0x33);
            func_80136B4C(self->mpLayout, &base[0x1b7], s, 0);
            func_80136910(self->mpLayout, &base[0x61], 0);
            func_80136910(self->mpLayout, &base[0x77], 0);
        } else {
            func_80136910(self->mpLayout, &base[0x61], self->field_0xE0);
            func_80136910(self->mpLayout, &base[0x77], self->field_0xDC);
            ml::FixStr<32> buf2(false);
            buf2.mString[0] = 0;
            buf2.mLength = 0;
            u8 flg = (u8)(func_8009CF8C(questId + 0x220) & 1);
            u8 i = 0;
            char buf[0x20];
            do {
                // Retail duplicates the whole sprintf per branch (the ternary
                // would fold the two format strings into one address select).
                if (flg != 0) {
                    sprintf(buf, &base[0x1df], i + 1);
                } else {
                    sprintf(buf, &base[0x1ea], i + 1);
                }
                u32 v30 = func_80136254((const void*)questRow, buf, questId);
                if ((v30 & 0xFFFF) != 0) {
                    u32 v22 = func_801392E4(v30 & 0xFFFF);
                    u32 v5 = func_80139358(v30 & 0xFFFF);
                    switch (v22 & 0xFFFF) {
                    case 0: cur = func_8013639C((const void*)lbl_eu_806640F4, &base[0x88], v5 & 0xFFFF); break;
                    case 1: cur = func_8013639C((const void*)lbl_eu_806640D8, &base[0x88], v5 & 0xFFFF); break;
                    case 2: cur = func_8013639C((const void*)lbl_eu_806640F8, &base[0x88], v5 & 0xFFFF); break;
                    case 3: cur = func_8013639C((const void*)lbl_eu_806640FC, &base[0x88], v5 & 0xFFFF); break;
                    case 4: cur = func_8013639C((const void*)lbl_eu_80664104, &base[0x88], v5 & 0xFFFF); break;
                    case 5: cur = func_8013639C((const void*)lbl_eu_80664108, &base[0x88], v5 & 0xFFFF); break;
                    case 6: cur = func_8013639C((const void*)lbl_eu_8066410C, &base[0x88], v5 & 0xFFFF); break;
                    case 7: cur = func_8013639C((const void*)lbl_eu_80664110, &base[0x88], v5 & 0xFFFF); break;
                    }
                    sprintf(buf, &base[0x1f5], i + 1);
                    if ((v22 & 0xFFFF) == 3) {
                        u8 v204 = func_801361E8(lbl_eu_806640EC, &base[0x204], v30 & 0xFFFF);
                        s = func_80136190(&base[0x7f], &base[0x88], 0x1e - (v204 - 1));
                        buf2.format(&base[0x20d], cur, s);
                    } else {
                        buf2.format(&base[0x212], cur);
                    }
                    func_80136B4C(self->mpLayout, buf, buf2.mString, 0);
                    ((u16*)&self->field_0xD4)[self->field_0xDA] = (u16)v30;
                    self->field_0xDA = self->field_0xDA + 1;
                }
                if (i == 0) {
                    cur = func_80136190(&base[0x14a], &base[0x88], 0x33);
                    func_80136B4C(self->mpLayout, &base[0x1b7], cur, 0);
                }
                i++;
            } while (i < 3);
        }
    } else {
        u8 vcd = func_801361E8(questRow, &base[0xcd], questId);
        if (vcd == 3) {
            s = func_80136190(&base[0x14a], &base[0x88], 0x32);
            func_80136B4C(self->mpLayout, &base[0x1b7], s, 0);
            func_80136910(self->mpLayout, &base[0x61], self->field_0xE0);
            func_80136910(self->mpLayout, &base[0x77], self->field_0xDC);
        } else if (vcd == 1) {
            s = func_80136190(&base[0x14a], &base[0x88], 0x33);
            func_80136B4C(self->mpLayout, &base[0x1b7], s, 0);
            func_80136910(self->mpLayout, &base[0x61], 0);
            func_80136910(self->mpLayout, &base[0x77], 0);
        } else {
            func_80136910(self->mpLayout, &base[0x61], self->field_0xE0);
            func_80136910(self->mpLayout, &base[0x77], self->field_0xDC);
            ml::FixStr<32> buf2(false);
            buf2.mString[0] = 0;
            buf2.mLength = 0;
            u8 i = 0;
            char buf3[0x20];
            do {
                sprintf(buf3, &base[0x1ea], i + 1);
                u32 v31 = func_80136254((const void*)questRow, buf3, questId);
                if ((v31 & 0xFFFF) != 0) {
                    u32 v30 = func_801392E4(v31 & 0xFFFF);
                    u32 v5 = func_80139358(v31 & 0xFFFF);
                    switch (v30 & 0xFFFF) {
                    case 0: cur = func_8013639C((const void*)lbl_eu_806640F4, &base[0x88], v5 & 0xFFFF); break;
                    case 1: cur = func_8013639C((const void*)lbl_eu_806640D8, &base[0x88], v5 & 0xFFFF); break;
                    case 2: cur = func_8013639C((const void*)lbl_eu_806640F8, &base[0x88], v5 & 0xFFFF); break;
                    case 3: cur = func_8013639C((const void*)lbl_eu_806640FC, &base[0x88], v5 & 0xFFFF); break;
                    case 4: cur = func_8013639C((const void*)lbl_eu_80664104, &base[0x88], v5 & 0xFFFF); break;
                    case 5: cur = func_8013639C((const void*)lbl_eu_80664108, &base[0x88], v5 & 0xFFFF); break;
                    case 6: cur = func_8013639C((const void*)lbl_eu_8066410C, &base[0x88], v5 & 0xFFFF); break;
                    case 7: cur = func_8013639C((const void*)lbl_eu_80664110, &base[0x88], v5 & 0xFFFF); break;
                    }
                    sprintf(buf3, &base[0x1f5], i + 1);
                    if ((v30 & 0xFFFF) == 3) {
                        u8 v204 = func_801361E8(lbl_eu_806640EC, &base[0x204], v31 & 0xFFFF);
                        s = func_80136190(&base[0x7f], &base[0x88], 0x1e - (v204 - 1));
                        buf2.format(&base[0x20d], cur, s);
                    } else {
                        buf2.format(&base[0x212], cur);
                    }
                    func_80136B4C(self->mpLayout, buf3, buf2.mString, 0);
                }
                if (i == 0) {
                    cur = func_80136190(&base[0x14a], &base[0x88], 0x33);
                    func_80136B4C(self->mpLayout, &base[0x1b7], cur, 0);
                }
                i++;
            } while (i < 3);
        }
    }
    // Common tail: button labels, classic-pad hint, timg resource panes.
    s = func_80136190(&base[0x14a], &base[0x88], 0x30);
    func_80136B4C(self->mpLayout, &base[0x215], s, 0);
    s = func_80136190(&base[0x14a], &base[0x88], 0x31);
    func_80136B4C(self->mpLayout, &base[0x21d], s, 0);
    s = func_80136190(&base[0x14a], &base[0x88], self->field_0xC8 != 0 ? 0x2f : 0x30);
    func_80136B4C(self->mpLayout, &base[0x224], s, 0);
    s = func_80136190(&base[0x22d], &base[0x23b], 0x2b);
    func_80136B4C(self->mpLayout, &base[0x240], s, 0);
    func_80136B4C(self->mpLayout, &base[0x24b], s, 0);
    const char* cp = isClassicController__Q22cf13CfGameManagerFv(-1) != 0 ? &base[0x256] : &base[0x25f];
    char* resName = func_80138F78(func_8013606C(&base[0x22d], cp, 0x2b));
    void* res2 = func_801355F4()->GetResource(0x74696D67, resName, 0);
    if (res2 != 0) {
        func_80137E7C(self->mpLayout, &base[0x268], res2);
        func_80137E7C(self->mpLayout, &base[0x271], res2);
        struct ResData {
            u8 pad[8];
            u16* pData;
        };
        u16* data = ((ResData*)res2)->pData;
        u16 a = data[1];
        u16 b = data[0];
        nw4r::lyt::Pane* pane = self->mpLayout->GetRootPane()->FindPaneByName(&base[0x268], true);
        if (pane != 0) {
            nw4r::math::VEC2 vec;
            vec.x = (f32)(u32)a;
            vec.y = (f32)(u32)b;
            func_80124288((u8*)pane, (const float*)&vec);
        }
        pane = self->mpLayout->GetRootPane()->FindPaneByName(&base[0x271], true);
        if (pane != 0) {
            nw4r::math::VEC2 vec2;
            vec2.x = (f32)(u32)a;
            vec2.y = (f32)(u32)b;
            func_80124288((u8*)pane, (const float*)&vec2);
        }
    }
    if (self->field_0xC8 != 0) {
        u8 flg = (u8)(func_8009CF8C(questId + 0x220) & 1);
        u8 i = 0;
        s32 c26 = 0xc6;
        char buf4[0x20];
        do {
            if (flg != 0) {
                sprintf(buf4, &base[0x27a], i + 1);
            } else {
                sprintf(buf4, &base[0x289], i + 1);
            }
            u32 v27 = func_80136254((const void*)questRow, buf4, questId);
            if (flg != 0) {
                sprintf(buf4, &base[0x298], i + 1);
            } else {
                sprintf(buf4, &base[0x2a6], i + 1);
            }
            u8 v2 = func_801361E8(questRow, buf4, questId);
            if ((v27 & 0xFFFF) != 0) {
                func_8009D018((v27 & 0xFFFF) + 0x608, v2);
            }
            if (i == 0) {
                if (flg != 0) {
                    sprintf(buf4, &base[0x2b4]);
                } else {
                    sprintf(buf4, &base[0x2c0]);
                }
                u32 v30 = func_80136254((const void*)questRow, buf4, questId);
                u8 v27b = (u8)func_8013732C(
                    func_80136254((const void*)questRow, &base[0x172], questId) & 0xFFFF);
                if ((v30 & 0xFFFF) != 0) {
                    s32 v21 = (s32)(v27b & 0xFF) + 0x21;
                    s32 v4 = (s32)func_8009CF8C((u32)v21) + (v30 & 0xFFFF);
                    if (v4 < 0) v4 = 0;
                    if (v4 > 0x2710) v4 = 0x2710;
                    func_8009D018((u32)v21, (u32)v4);
                    struct PlayerView {
                        u8 pad[0x8C];
                        u16 field_8C;
                    };
                    u16 pid = ((PlayerView*)getPlayer__Q22cf13CfGameManagerFi(0))->field_8C;
                    func_8013DB6C(4, pid, v27b & 0xFF, v30 & 0xFFFF);
                    if ((v30 & 0xFFFF) != 0) {
                        self->field_0xCC = (u32)c26;
                        func_80138078__FUl(0x34);
                    }
                }
            }
            i++;
        } while (i < 4);
    }
    func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0x2cc], true), 0);
    func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0x2d7], true), 0);
    func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0x2e3], true), 0);
    func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0x2ec], true), 0);
    if (self->field_0xC8 == 0) {
        if (self->field_0xC4 == 0) {
            func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0x2cc], true), 1);
        } else {
            func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0x2d7], true), 1);
        }
        func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0x2ec], true), 1);
        func_80138078__FUl(0x19);
    } else {
        func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0x2d7], true), 1);
        func_80124270((void*)self->mpLayout->GetRootPane()->FindPaneByName(&base[0x2e3], true), 1);
        func_80138078__FUl(0x1c);
        self->field_0xC0 = 0;
    }
    IScnRender* render = reinterpret_cast<IScnRender*>(self);
    if (self != 0) render = reinterpret_cast<IScnRender*>((u8*)self + 0x70);
    self->mpScn->addRenderCB(render, 0x10, 0);
}

// Pane-flag toggle: clear bit 0 of the pane's +0xBB flag byte (nw4r
// visible bit) and OR in the caller's flag byte (0/1 = hide/show).
void func_80124270(PaneFlagRef* self, u32 flag) {
    self->mFlag = (self->mFlag & 0xFE) | (flag & 0xFF);
}

// retail: lfs f1,0(r4); lfs f0,4(r4); stfs f1,0x4c(r3); stfs f0,0x50(r3)
// const source is required: non-const forces load/store interleaving (aliasing)
extern "C" void func_80124288(u8* self, const float* src){
    ((float*)(self + 0x4C))[0] = src[0];
    ((float*)(self + 0x4C))[1] = src[1];
}

// Bind anim B (+0x9C) disabled then anim A (+0x98) enabled onto the layout
// (func_8012429C; mirror image of func_801242FC).
void func_8012429C(QuestWinObj* self) {
    self->mpLayout->SetAnimationEnable(self->mAnimB, false);
    self->mpLayout->SetAnimationEnable(self->mAnimA, true);
}

// Bind anim A (+0x98) disabled then anim B (+0x9C) enabled onto the layout
// (func_801242FC; mirror image of func_8012429C). extern "C" keeps the
// unmangled retail symbol name (see the forward declaration above).
extern "C" void func_801242FC(QuestWinObj* self) {
    self->mpLayout->SetAnimationEnable(self->mAnimA, false);
    self->mpLayout->SetAnimationEnable(self->mAnimB, true);
}

// ---------------------------------------------------------------------------
// func_8012435C (us-80124e38): position the quest-window cursor over the
// selected pane. Reads two pane names per cursor slot from the float tables
// (their 4 bytes spell the name), resolves them via FindPaneByName, derives a
// point with func_80137924 and moves the cursor there.
// ---------------------------------------------------------------------------
__attribute__((noinline)) void func_8012435C(QuestWinObj* self) {
    if (self->field_0xC0 < 0) return;
    if (self->field_0xC8 == 0) {
        if (self->field_0xC4 == 0) {
            // Cursor-slot name built from the two float tables (raw bytes).
            f32 arr[2];
            arr[0] = lbl_eu_80667178[self->field_0xC0];
            arr[1] = lbl_eu_8066717C[self->field_0xC0];
            nw4r::lyt::Pane* p1 = self->mpLayout->GetRootPane()->FindPaneByName(
                (const char*)&arr[self->field_0xC0], true);
            nw4r::lyt::Pane* p2 = self->mpLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_804FEC84[0x2f7], true);
            nw4r::math::VEC3 pos;
            func_80137924(&pos, p1, p2, self->mpLayout->GetRootPane());
            reinterpret_cast<CCur18View*>(&self->mCursor)->vf04(&pos);
        } else {
            nw4r::lyt::Pane* p1 = self->mpLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_804FEC84[0x306], true);
            nw4r::lyt::Pane* p2 = self->mpLayout->GetRootPane()->FindPaneByName(
                &lbl_eu_804FEC84[0x2f7], true);
            nw4r::math::VEC3 pos;
            func_80137924(&pos, p1, p2, self->mpLayout->GetRootPane());
            reinterpret_cast<CCur18View*>(&self->mCursor)->vf04(&pos);
        }
    } else {
        nw4r::lyt::Pane* p1 = self->mpLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_804FEC84[0x306], true);
        nw4r::lyt::Pane* p2 = self->mpLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_804FEC84[0x2f7], true);
        nw4r::math::VEC3 pos;
        func_80137924(&pos, p1, p2, self->mpLayout->GetRootPane());
        reinterpret_cast<CCur18View*>(&self->mCursor)->vf04(&pos);
    }
}

void* __dt__12CQuestWindowFv(CQuestWindow* self, int flags);
extern "C" void cbRenderBefore__12CQuestWindowFv(CQuestWindow* self);
void OnFileEvent__12CQuestWindowFP10CEventFile(void* self) { ((void(*)(void*))func_8012278C)((char*)self - 0x6c); }

void func_801245D4(void* self) { ((void(*)(void*))__dt__12CQuestWindowFv)((char*)self - 0x6c); }

void func_801245DC(void* self) { ((void(*)(void*))cbRenderBefore__12CQuestWindowFv)((char*)self - 0x70); }

extern "C" void func_801245E4(void* self) { ((void(*)(void*))__dt__12CQuestWindowFv)((char*)self - 0x70); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CTTask<T> is declared in kyoshin/CTaskGameEff.hpp (via harness_catalog.hpp);
// specializations below emit the retail Move/Draw/dtor symbols.
// Test the +0x3C/+0x48 ptmf hook and dispatch through __ptmf_scall when
// non-null; the dtor body is empty (MWCC emits the null check, CProcess
// base-dtor call and conditional operator delete).
template<> CTTask<IUIWindow>::~CTTask() {}

// Retail ~IUIWindow: the empty CTTask<IUIWindow> base dtor (same TU) is
// inlined, so the only call is the direct ~CProcess; the double null-guard
// around it is the D2-inlined-into-D1 MWCC artifact (CSimpleEveTalkWin
// pattern) plus the deleting-wrapper check. novtable (header) suppresses the
// implicit vptr store.
IUIWindow::~IUIWindow() {}

template<> void CTTask<IUIWindow>::Move() {
    if (mMoveFunc) {
        (static_cast<IUIWindow*>(this)->*mMoveFunc)();
    }
}
template<> void CTTask<IUIWindow>::Draw() {
    if (mDrawFunc) {
        (static_cast<IUIWindow*>(this)->*mDrawFunc)();
    }
}

// Retail: lwz lbl_eu_80663FD4@sda21; null-check; lbz +0xDA; addic/subfe
// setnz (byte != 0). Returns 1 while the quest-window state byte is non-zero.
bool func_8012246C() {
    QuestWindowState* p = lbl_eu_80663FD4;
    if (p != 0) {
        return p->field_0xDA != 0;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// CQuestWindow::Init (us-80122f6c): request the common-archive quest-name arc
// (name chosen by field_B8 through lbl_eu_8052CF80) and the window arc file,
// feeding the IWorkEvent subobject at +0x6C to both loaders.
// ---------------------------------------------------------------------------
void CQuestWindow::Init() {
    // IWorkEvent subobject at +0x6C; the retail keeps the conditional
    // this-adjust (null this -> null IWorkEvent) as mr/beq/addi.
    IWorkEvent* evt = (IWorkEvent*)this;
    if (this != 0) {
        evt = (IWorkEvent*)((u8*)this + 0x6c);
    }
    field_0x8C = (CFileHandle*)CDeviceFile::readCommonArchiveFile(
        func_800A9D90(),
        lbl_eu_8052CF80[func_80138138(field_B8)], evt, 0, 0);
    IWorkEvent* evt2 = (IWorkEvent*)this;
    if (this != 0) {
        evt2 = (IWorkEvent*)((u8*)this + 0x6c);
    }
    field_0x88 = (CFileHandle*)CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), lbl_eu_80661EB8, evt2, 0, 0);
}

// ---------------------------------------------------------------------------
// CQuestWindow::Term (us-80123000): wait for the VI draw to complete, drop the
// render callback and both file handles, release the layout / +0x90 object and
// arc accessor, clear the singleton, then re-enable pad flags and run the
// cf-side close bookkeeping.
// ---------------------------------------------------------------------------
void CQuestWindow::Term() {
    CDeviceVI::waitForDrawDone();
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&field_70);
    }
    mpScn->removeRenderCB(render);
    func_801390E0(&field_0x88);
    func_801390E0(&field_0x8C);
    CBdat::getEntry(2);
    reinterpret_cast<CCursor18*>(&mCursor[0])->vf3();
    if (mpLayout != 0) {
        delete mpLayout;
        mpLayout = 0;
    }
    if (field_0x90 != 0) {
        delete field_0x90;
        field_0x90 = 0;
    }
    func_80139124(field_0x94);
    field_0x94 = 0;
    lbl_eu_80663FD4 = 0;
    mMemRegion.func_8045F778();
    enablePadFlags__Q22cf13CfGameManagerFUlb(-1, 0);
    enablePadFlags__Q22cf13CfGameManagerFUlb(field_0xE8, 1);
    func_8013B980();
    if (func_801B481C() == 0) {
        if (code80135FDC_getByte_64080() == 0) {
            setPresentationFlag__Q22cf13CfGameManagerFv(false);
        }
        func_80135550();
    }
    lbl_eu_80663FD0 = lbl_eu_80663FD0 - 1;
}

// ---------------------------------------------------------------------------
// ~CQuestWindow (us-80122e8c): complete-object destructor. The retail dtor
// takes (this, flags) with a conditional operator delete and re-stores the
// composite vtable plus the +0x24/+0xAC dispatch slots before the teardown
// calls, so it is written as the C-ABI global __dt__12CQuestWindowFv
// (COption / CSysWinSave idiom - a non-polymorphic member dtor cannot carry
// the flags parameter, and a C++ virtual class would emit a local vtable
// instead of referencing the retail lbl_eu_8052D0F0).
// ---------------------------------------------------------------------------
void* __dt__12CQuestWindowFv(CQuestWindow* _this, int flags) {
    if (_this != 0) {
        _this->mVtable = (u32)lbl_eu_8052D0F0;
        _this->field_6C = (u32)lbl_eu_8052D0F0 + 0x24;
        _this->field_70 = (u32)lbl_eu_8052D0F0 + 0xAC;
        cf::CfPadTask::setInputDisableTime(lbl_eu_80667138);
        __dt__6CCur18Fv(&_this->mCursor[0], -1);
        __dt__17UnkClass_8045F564Fv(&_this->mMemRegion, -1);
        __dt__9IUIWindowFv(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

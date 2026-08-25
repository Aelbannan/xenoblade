// CTalkWindow - talk-window process (unit kyoshin/CTalkWindow).
//
// Matching session: dtor, Term, cbRenderBefore, func_8012CC78 (factory) and
// sinit_8012DE98 (static float-table initializer).

#include "kyoshin/CTalkWindow.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/device/CDeviceVI.hpp"

#include <revolution/gx/GXPixel.h>
#include <new>
#include <string.h>

// Global-view declarations so the catalog thunks (func_8012DF78/80/88) can
// address the member dtor / render callback by their retail symbol names
// (the definitions below emit those symbols). extern "C" keeps the emitted
// reloc names unmangled.
extern "C" void* __dt__11CTalkWindowFv(CTalkWindow* self, int flags);
extern "C" void cbRenderBefore__11CTalkWindowFv(void* self);

void Draw__11CTalkWindowFv() {}

// Show/hide a talk-layout pane flag. Retail always clears the bit-7 field of
// the 0xBB byte together with the flag write (MWCC merges the two bitfield
// stores into a single read-modify-write), so every site writes both fields.
#define twSetPaneFlag(paneExpr, setBit)                                    \
    do {                                                                   \
        CTalkWinPane* twFlagPane = (paneExpr);                             \
        twFlagPane->mFlagHigh = 0;                                         \
        if (setBit) twFlagPane->mFlag |= 1; else twFlagPane->mFlag &= ~1u; \
    } while (0)

extern u32 lbl_eu_80664044;
extern "C" void func_8012BDD0() { lbl_eu_80664044 = 0; }

// ---------------------------------------------------------------------------
// __ct__CTalkWindow (us-8012c710)
// Runs CProcess's ctor then fills the IUIWindow region by hand (the temp
// vtable at +0x10 is written first, then the CTalkWindow composite vtable),
// copies the two null ptmf callback slots, constructs the embedded
// UnkClass_8045F564 region, and initialises the animation / state fields.
// The retail symbol carries no class-length mangling, so it stays a plain
// C-ABI global (CSystemWindow / CSysWinSave idiom); the definition inherits
// C linkage from the header declaration.
// ---------------------------------------------------------------------------
CTalkWindow* __ct__CTalkWindow(CTalkWindow* _this, u32 arg1, u32 arg2,
                               u8* buf, u32 arg3, u32 arg4, u32 arg5) {
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(_this));

    _this->mVtable = (u32)lbl_eu_8052D238;
    // Post-increment derefs of a local pointer make MWCC fold the first
    // access into `lwzu` (single base register via @ha/@l, offsets on the
    // rest) instead of an addi-materialised pointer (cf. MWCC_CASES
    // btm_sco_init lwzu shape). The stores into _this can alias the non-const
    // global, forcing the 4..6/4..5 reloads between the two callback slots.
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    _this->ptmf0[1] = w1;
    _this->ptmf0[0] = w0;
    u32 w2 = *src++;
    _this->ptmf0[2] = w2;
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    _this->ptmf1[1] = w0;
    _this->ptmf1[0] = w1;
    w2 = *src++;
    _this->ptmf1[2] = w2;

    _this->mpLayout = 0;
    _this->field_58 = 0;
    _this->field_5C = 0;
    _this->field_60 = -1;
    _this->field_64 = 0;
    _this->field_65 = 0;
    _this->field_66 = 0;
    _this->field_67 = 1;
    _this->field_68 = 0;

    _this->mVtable = (u32)lbl_eu_8052DFA8;
    _this->field_6C = (u32)lbl_eu_8052DFA8 + 0x24;
    _this->mScnRender = (u32)lbl_eu_8052DFA8 + 0xac;
    _this->mScene = reinterpret_cast<CScn*>(arg1);

    __ct__17UnkClass_8045F564Fv(
        reinterpret_cast<UnkClass_8045F564*>(&_this->mMemRegion[0]));

    _this->field_88 = 0;
    _this->field_8C = 0;
    _this->field_90 = 0;
    _this->field_94 = 0;
    _this->field_98 = 0;
    _this->field_9C = reinterpret_cast<u32>(buf);
    _this->field_A0 = static_cast<u8>(arg3);
    _this->field_A1 = static_cast<u8>(arg4);
    _this->field_A4 = 0;
    _this->field_A8 = 0;
    _this->field_AC = 0;
    _this->field_B0 = 1;
    _this->field_B4 = static_cast<u8>(arg5);
    _this->field_68 = arg2;
    return _this;
}

// ---------------------------------------------------------------------------
// func_8012D8C0 (us-8012e390)
// Window-state 1 driver: play the intro anim (field_88); when the talk-enable
// flag (field_B4) is set, run the character-voice playback through the talk
// source (with the page-id / talk-source checks), then switch the enabled
// animations to state 2.
// ---------------------------------------------------------------------------
void func_8012D8C0(CTalkWindow* self) {
    f32 frame = lbl_eu_80667284;
    if (func_80137444(self->field_88, frame) == 0) return;
    if (self->field_B4 != 0) {
        if (code80135FDC_getByte_64058() == 0) {
            if (func_800B708C(self->field_68) != 0) {
                CTalkWinTalkSrc* src =
                    reinterpret_cast<CTalkWinTalkSrc*>(func_800BBC0C());
                // Re-read mTalkC4 at each use: retail reloads the field
                // around the voice call (memory-clobber), and keeping it in
                // a local would shrink the body by two loads.
                if (src->mTalkC4 != 0) {
                    // Talk-stop flag: leave the intro playing.
                    if ((src->mTalkC4->field_270 & 0x80) != 0) return;
                    src->mVoice->play(1, 0);
                    int page = func_8004C5EC(src->mTalkC4);
                    // Two separate compares branching to a shared call site
                    // (not a fused range test): retail keeps
                    // `cmpwi 0x21 / cmpwi 0x26` with blt/ble gates.
                    if (page < 0x21) goto do_b9d4;
                    if (page <= 0x26) goto skip_b9d4;
do_b9d4:
                    func_8004B9D4(src->mTalkC4, 0x1f, 0, -1, 0);
skip_b9d4:;
                }
            }
        } else if (code80135FDC_getByte_64059() != 0) {
            if (func_800B708C(self->field_68) != 0) {
                CTalkWinTalkSrc* src =
                    reinterpret_cast<CTalkWinTalkSrc*>(func_800BBC0C());
                src->mVoice->play(1, 0);
            }
        }
    }
    self->mpLayout->SetAnimationEnable(self->field_90, false);
    self->mpLayout->SetAnimationEnable(self->field_94, false);
    self->mpLayout->SetAnimationEnable(self->field_98, false);
    self->mpLayout->SetAnimationEnable(self->field_88, false);
    self->mpLayout->SetAnimationEnable(self->field_8C, true);
    self->field_B0 = 2;
}

// ---------------------------------------------------------------------------
// func_8012DA6C (us-8012e53c)
// Window-state 2 driver: find the three page panes, blend the tag-processor
// animation (func_801276F4 / func_8012615C depending on the confirm button),
// then dispatch the result: state 4 accepts the selection, otherwise the
// voice/page logic runs (or the page-loop flag flips).
// ---------------------------------------------------------------------------
void func_8012DA6C(CTalkWindow* self) {
    nw4r::lyt::Pane* root = self->mpLayout->GetRootPane();
    nw4r::lyt::Pane* p1 =
        root->FindPaneByName(&lbl_eu_804FFCA4[0xf1], 1);
    nw4r::lyt::Pane* p2 =
        root->FindPaneByName(lbl_eu_8052DF70[self->field_A4], 1);
    nw4r::lyt::Pane* p3 =
        root->FindPaneByName(&lbl_eu_804FFCA4[0xb0], 1);

    int state;
    if (func_8013BF78() != 0) {
        state = func_801276F4(self->field_5C, p1, p2, p3);
    } else {
        CTalkPadView* pad = reinterpret_cast<CTalkPadView*>(
            cf::CfGameManager::getCurrentPad());
        int confirm;
        // Co-op (Classic) vs single-player (Wiimote) confirm-press bits.
        if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0)
            confirm = (pad->field_04 & 0x00600000) != 0;
        else
            confirm = (pad->field_04 & 0x00000030) != 0;
        if (confirm != 0)
            state = func_801276F4(self->field_5C, p1, p2, p3);
        else
            state = func_8012615C(self->field_5C, p1, p2, p3);
    }

    if (state == 4) {
        // Selection accepted: play the confirm voice and switch to state 4.
        self->mpLayout->SetAnimationEnable(self->field_88, false);
        self->mpLayout->SetAnimationEnable(self->field_8C, false);
        self->mpLayout->SetAnimationEnable(self->field_98, false);
        self->mpLayout->SetAnimationEnable(self->field_90, false);
        self->mpLayout->SetAnimationEnable(self->field_94, true);
        reinterpret_cast<CTalkAnimFrame*>(self->field_94)->mFrame =
            lbl_eu_80667280;
        func_80138078(0x2b);
        self->field_B0 = 4;
    } else if (state == 0 || self->field_65 != 0 || self->field_66 != 0) {
        // Page-advance path: clear the pending flag, run the character-voice
        // playback, then switch the enabled animations to state 3.
        if (self->field_65 != 0) self->field_65 = 0;
        if (self->field_B4 != 0) {
            if (code80135FDC_getByte_64058() == 0) {
                if (func_800B708C(self->field_68) != 0) {
                    CTalkWinTalkSrc* src =
                        reinterpret_cast<CTalkWinTalkSrc*>(func_800BBC0C());
                    CTalkWinTalkC4* talkC4 = src->mTalkC4;
                    if (talkC4 != 0) {
                        src->mVoice->play(0, 0);
                        int page = func_8004C5EC(talkC4);
                        if (page < 0x21 || page > 0x26) {
                            func_8004B9D4(talkC4, 1, 0, -1, 0);
                        }
                    }
                }
            } else if (code80135FDC_getByte_64059() != 0) {
                if (func_800B708C(self->field_68) != 0) {
                    CTalkWinTalkSrc* src =
                        reinterpret_cast<CTalkWinTalkSrc*>(func_800BBC0C());
                    src->mVoice->play(0, 0);
                }
            }
        }
        self->mpLayout->SetAnimationEnable(self->field_94, false);
        self->mpLayout->SetAnimationEnable(self->field_88, false);
        self->mpLayout->SetAnimationEnable(self->field_8C, false);
        self->mpLayout->SetAnimationEnable(self->field_98, false);
        self->mpLayout->SetAnimationEnable(self->field_90, true);
        reinterpret_cast<CTalkAnimFrame*>(self->field_90)->mFrame =
            lbl_eu_80667280;
        switch (self->field_A4) {
        case 0:
            func_80138078(0x2a);
            break;
        case 1:
            func_80138078(0x73);
            break;
        case 3:
            func_80138078(0x75);
            break;
        default:
            break;
        }
        self->field_B0 = 3;
    } else {
        // Page-loop flag flips (only reached when field_65/field_66 are clear).
        if (state == 1) {
            if (self->field_65 == 0) self->field_65 = 1;
        } else if (state == 3) {
            self->field_67 = 0;
            if (self->field_AC != 0) self->field_65 = 1;
        }
    }

    func_80137444(self->field_8C, lbl_eu_80667284);
}

// ---------------------------------------------------------------------------
// func_8012D3D8 (us-8012dea8)
// Place the four corner message panes around the reference pane (0xb0) and
// lay out the remaining page panes with scale-derived offsets, using the
// page-name table (lbl_eu_8052DF70) for the final pane.
// ---------------------------------------------------------------------------
void func_8012D3D8(CTalkWindow* self) {
    nw4r::lyt::Pane* root = self->mpLayout->GetRootPane();
    nw4r::lyt::Pane* ref =
        root->FindPaneByName(&lbl_eu_804FFCA4[0xb0], 1);

    f32 tx = ref->GetTranslate().x;
    f32 ty = ref->GetTranslate().y;
    f32 tz = ref->GetTranslate().z;
    f32 sx = ref->GetScale().x;
    f32 sy = ref->GetScale().y;

    nw4r::lyt::Pane* p2 =
        root->FindPaneByName(&lbl_eu_804FFCA4[0xb9], 1);
    p2->SetTranslate(nw4r::math::VEC3(tx - lbl_eu_806672AC,
                                      lbl_eu_806672AC + ty, tz));
    p2->SetScale(nw4r::math::VEC2(sx, sy));

    nw4r::lyt::Pane* p3 =
        root->FindPaneByName(&lbl_eu_804FFCA4[0xc7], 1);
    p3->SetTranslate(nw4r::math::VEC3(lbl_eu_806672AC + tx,
                                      lbl_eu_806672AC + ty, tz));
    p3->SetScale(nw4r::math::VEC2(sx, sy));

    nw4r::lyt::Pane* p4 =
        root->FindPaneByName(&lbl_eu_804FFCA4[0xd5], 1);
    p4->SetTranslate(nw4r::math::VEC3(lbl_eu_806672AC + tx,
                                      ty - lbl_eu_806672AC, tz));
    p4->SetScale(nw4r::math::VEC2(sx, sy));

    nw4r::lyt::Pane* p5 =
        root->FindPaneByName(&lbl_eu_804FFCA4[0xe3], 1);
    p5->SetTranslate(nw4r::math::VEC3(tx - lbl_eu_806672AC,
                                      ty - lbl_eu_806672AC, tz));
    p5->SetScale(nw4r::math::VEC2(sx, sy));

    nw4r::lyt::Pane* p6 =
        root->FindPaneByName(lbl_eu_8052DF70[self->field_A4], 1);
    p6->SetTranslate(nw4r::math::VEC3(tx - lbl_eu_806672AC,
                                      ty - lbl_eu_806672AC, tz));
    p6->SetScale(nw4r::math::VEC2(sx, sy));

    // Centre-pane scale: pane6's scale.x scaled to a layout offset, flipped
    // when the root pane sits on the negative side of the origin.
    f32 g = p6->GetScale().x / lbl_eu_806672B0 * lbl_eu_80667290;
    if (root->GetTranslate().x <= 0.0f) {
        g *= lbl_eu_8066727C;
    }

    nw4r::lyt::Pane* p7 =
        root->FindPaneByName(&lbl_eu_804FFCA4[0x11e], 1);
    p7->SetTranslate(nw4r::math::VEC3(
        g, p6->GetScale().y * lbl_eu_80667290 + p6->GetTranslate().y +
               p7->GetScale().y * lbl_eu_80667290 - lbl_eu_806672B4,
        p6->GetTranslate().z));

    nw4r::lyt::Pane* p8 =
        root->FindPaneByName(&lbl_eu_804FFCA4[0xf1], 1);
    p8->SetTranslate(nw4r::math::VEC3(
        g, lbl_eu_806672B4 + (p6->GetTranslate().y -
                               p6->GetScale().y * lbl_eu_80667290 -
                               p8->GetScale().y * lbl_eu_80667290),
        p6->GetTranslate().z));

    nw4r::lyt::Pane* p9 =
        root->FindPaneByName(&lbl_eu_804FFCA4[0xfc], 1);
    p9->SetTranslate(p8->GetTranslate());

    nw4r::lyt::Pane* p10 =
        root->FindPaneByName(&lbl_eu_804FFCA4[0x108], 1);
    p10->SetTranslate(nw4r::math::VEC3(
        p8->GetTranslate().x, p8->GetTranslate().y - lbl_eu_806672B8,
        p8->GetTranslate().z));

    nw4r::lyt::Pane* p11 =
        root->FindPaneByName(&lbl_eu_804FFCA4[0x113], 1);
    p11->SetTranslate(nw4r::math::VEC3(
        p8->GetTranslate().x, p8->GetTranslate().y - lbl_eu_806672BC,
        p8->GetTranslate().z));
}

void func_8012DF78(void* self) { ((void(*)(void*))__dt__11CTalkWindowFv)((char*)self - 0x6c); }

void func_8012DF80(void* self) { ((void(*)(void*))cbRenderBefore__11CTalkWindowFv)((char*)self - 0x70); }

void func_8012DF88(void* self) { ((void(*)(void*))__dt__11CTalkWindowFv)((char*)self - 0x70); }

extern u32 lbl_eu_80664044;
extern "C" bool func_8012CD24() {
    u32 v = lbl_eu_80664044;
    return ((-v) & ~v) >> 31;
}

extern "C" void func_8012CD38(CTalkWindow* self) {
    CTalkWinSrc* src =
        reinterpret_cast<CTalkWinSrc*>(func_800B708C(self->field_68));
    if (src == 0) {
        IScnRender* render = reinterpret_cast<IScnRender*>(self);
        if (self != 0) {
            render = reinterpret_cast<IScnRender*>(&self->mScnRender);
        }
        self->mScene->removeRenderCB(render);
        self->field_64 = 1;
        self->field_B0 = 0;
        return;
    }

    // Local declaration order mirrors the retail stack layout. MWCC assigns
    // ascending slots in reverse decl order: the corner-UV table (declared
    // last) takes the top region (+0x178..+0x274), then world down to sy2
    // (+0x16c..+0xc). Indices 0..7 = pane1-B1..B4, pane2-B1..B4.
    nw4r::math::VEC3 world;    // 0x16c
    nw4r::math::VEC3 screen;   // 0x160
    nw4r::math::VEC3 trans1;   // 0x154
    nw4r::math::VEC3 trans2;   // 0x148
    nw4r::math::VEC3 tmp;      // 0x13c
    nw4r::math::VEC3 table;    // 0x130
    f32 scaleY;                // 0x12c
    f32 scaleX;                // 0x128
    f32 sx1;                   // 0x120
    f32 sx2;                   // 0x118
    f32 sy1;                   // 0x14
    f32 sy2;                   // 0xc
    nw4r::math::VEC2 cornerUVs[8][4];  // 0x178
    f32 dx;

    // Resolve the talk-source anchor: the named-position lookup (0x120)
    // yields strided components (+0xc/+0x1c/+0x2c), else the +0xAC fallback.
    const nw4r::math::VEC3* wsrc;
    CTalkWinPosObj* obj = src->vfn120(&lbl_eu_804FFCA4[0x1e3]);
    if (obj != 0) {
        // Load the strided components in retail's z,y,x order.
        f32 tz = obj->field_0x2C;
        f32 ty = obj->field_0x1C;
        f32 tx = obj->field_0x0C;
        tmp.x = tx;
        tmp.y = ty;
        tmp.z = tz;
        wsrc = &tmp;
    } else {
        wsrc = src->vfnAC();
    }
    world.x = wsrc->x;
    world.y = wsrc->y;
    world.z = wsrc->z;

    // Project the world anchor through the scene camera into screen space.
    CTalkWinPose* pose = func_80496264(self->mScene, -1);
    func_8049B59C(&screen, pose, &world);

    screen.z = lbl_eu_80667280;
    screen.y = (screen.y - lbl_eu_8066728C) * lbl_eu_8066727C;
    screen.x = screen.x - lbl_eu_80667288;
    world = screen;  // pre-clamp copy (dx uses world.x later)

    nw4r::lyt::Pane* pane = self->mpLayout->GetRootPane()->
        FindPaneByName(lbl_eu_8052DF70[self->field_A4], 1);
    screen.y = screen.y + pane->GetScale().y * lbl_eu_80667290 +
               lbl_eu_80667294;
    scaleX = pane->GetScale().x;
    scaleY = pane->GetScale().y;

    if (self->field_A1 != 0) {
        screen.y *= lbl_eu_8066727C;
    }
    if (self->field_A8 != 0) {
        // Override the projected position with the page-position table entry.
        table = reinterpret_cast<nw4r::math::VEC3*>(lbl_eu_80573A70)
                    [self->field_A8];
        screen = table;
    }

    // Clamp the projected point inside the pane bounds (expanded forms match
    // the retail fmadds/fnmsubs folds).
    if (screen.x - scaleX * lbl_eu_80667290 < lbl_eu_80667298) {
        screen.x = screen.x +
                   (lbl_eu_80667298 - (screen.x - scaleX * lbl_eu_80667290));
    } else if (screen.x + scaleX * lbl_eu_80667290 > lbl_eu_80667288) {
        screen.x = screen.x -
                   ((screen.x + scaleX * lbl_eu_80667290) - lbl_eu_80667288);
    }
    if (screen.y + scaleY * lbl_eu_80667290 > lbl_eu_8066728C) {
        screen.y = screen.y -
                   ((screen.y + scaleY * lbl_eu_80667290) - lbl_eu_8066728C);
    } else if (screen.y - scaleY * lbl_eu_80667290 < lbl_eu_8066729C) {
        screen.y = screen.y +
                   (lbl_eu_8066729C - (screen.y - scaleY * lbl_eu_80667290));
    }

    if (self->field_A1 != 0 && self->field_A8 == 0) {
        screen.y += lbl_eu_806672A0;
    }

    self->mpLayout->GetRootPane()->SetTranslate(screen);
    dx = world.x - screen.x;
    if (lbl_eu_806672A4 <= world.x && world.x <= lbl_eu_806672A8) {
        dx = lbl_eu_80667280;
    }

    // Corner-picture UV flips: the sign of dx (and the pane translate for the
    // centred case) picks one of the four corner rotations.
    nw4r::lyt::Pane* p1 = self->mpLayout->GetRootPane()->
        FindPaneByName(&lbl_eu_804FFCA4[0x11e], 1);
    if (p1 != 0) {
        trans1 = p1->GetTranslate();
        sx1 = p1->GetScale().x;
        sy1 = p1->GetScale().y;
        if (dx > lbl_eu_80667280) {
            cornerUVs[0][0] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667284);
            cornerUVs[0][1] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667284);
            cornerUVs[0][2] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667280);
            cornerUVs[0][3] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667280);
            reinterpret_cast<nw4r::lyt::Picture*>(p1)->SetTexCoord(0, cornerUVs[0]);
            goto p1done;
        }
        if (dx < lbl_eu_80667280) {
            cornerUVs[1][0] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667284);
            cornerUVs[1][1] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667284);
            cornerUVs[1][2] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667280);
            cornerUVs[1][3] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667280);
            reinterpret_cast<nw4r::lyt::Picture*>(p1)->SetTexCoord(0, cornerUVs[1]);
            goto p1done;
        }
        if (trans1.x <= lbl_eu_80667280) {
            cornerUVs[2][0] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667284);
            cornerUVs[2][1] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667284);
            cornerUVs[2][2] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667280);
            cornerUVs[2][3] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667280);
            reinterpret_cast<nw4r::lyt::Picture*>(p1)->SetTexCoord(0, cornerUVs[2]);
        } else {
            cornerUVs[3][0] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667284);
            cornerUVs[3][1] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667284);
            cornerUVs[3][2] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667280);
            cornerUVs[3][3] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667280);
            reinterpret_cast<nw4r::lyt::Picture*>(p1)->SetTexCoord(0, cornerUVs[3]);
        }
p1done:;
    }

    nw4r::lyt::Pane* p2 = self->mpLayout->GetRootPane()->
        FindPaneByName(&lbl_eu_804FFCA4[0x12c], 1);
    if (p2 != 0) {
        trans2 = p2->GetTranslate();
        sx2 = p2->GetScale().x;
        sy2 = p2->GetScale().y;
        if (dx > lbl_eu_80667280) {
            cornerUVs[4][0] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667280);
            cornerUVs[4][1] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667280);
            cornerUVs[4][2] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667284);
            cornerUVs[4][3] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667284);
            reinterpret_cast<nw4r::lyt::Picture*>(p2)->SetTexCoord(0, cornerUVs[4]);
            goto p2done;
        }
        if (dx < lbl_eu_80667280) {
            cornerUVs[5][0] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667280);
            cornerUVs[5][1] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667280);
            cornerUVs[5][2] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667284);
            cornerUVs[5][3] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667284);
            reinterpret_cast<nw4r::lyt::Picture*>(p2)->SetTexCoord(0, cornerUVs[5]);
            goto p2done;
        }
        if (trans2.x <= lbl_eu_80667280) {
            cornerUVs[6][0] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667280);
            cornerUVs[6][1] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667280);
            cornerUVs[6][2] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667284);
            cornerUVs[6][3] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667284);
            reinterpret_cast<nw4r::lyt::Picture*>(p2)->SetTexCoord(0, cornerUVs[6]);
        } else {
            cornerUVs[7][0] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667280);
            cornerUVs[7][1] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667280);
            cornerUVs[7][2] = nw4r::math::VEC2(lbl_eu_80667280, lbl_eu_80667284);
            cornerUVs[7][3] = nw4r::math::VEC2(lbl_eu_80667284, lbl_eu_80667284);
            reinterpret_cast<nw4r::lyt::Picture*>(p2)->SetTexCoord(0, cornerUVs[7]);
        }
p2done:;
    }
}

// ---------------------------------------------------------------------------
// CTalkWindow::Init (us-8012c8a8)
// Open a scratch region (Class_8045F858 RAII guard), allocate and prime the
// tag processor, then dispatch on its +0x814 mode byte. Mode 7 builds the
// full talk layout (5 animations, font binding, message panes, corner
// pictures), wires the page panes and registers the render callback.
//
// Matching notes from the retail ASM (for the next iteration):
//  - Retail holds `this` in r25 (_savegpr_25) and the tag processor in r29
//    across the whole case-7 body; the decomp currently copies this into
//    r29 first, shifting every later register assignment.
//  - Every pane show/hide compiles to `lbz 0xbb; rlwinm r0,r0,0,24,30
//    [; ori 1]; stb` - i.e. (old & 0x7F) [| 1]: bit 7 of the flag byte is
//    ALWAYS cleared. That shape needs two merged bitfield writes on CTalkWinPane
//    (a :7 field + a :1 field both assigned per site); plain `u8 mFlag |= 1`
//    gives lbz/ori/stb without the mask.
//  - Font binding order: retail loads GetRootPane() BEFORE calling
//    func_80452C10(1, layout) (hoist into its own statement).
//  - The lbl_eu_804FFCA4 blob base is materialised once into r30 before the
//    five func_80136F08 calls, then rematerialised (r30/r28/r27/r30 again)
//    at each section boundary after Animate() - the source likely redeclares
//    a base pointer per scope rather than sharing one.
//  - Member calls on reinterpret_cast<UnkClass_8045F564*> (createRegion /
//    func_8045F810) add null-check branches retail lacks; extern "C"
//    pre-mangled declarations avoid that but shift other allocation.
// ---------------------------------------------------------------------------
void CTalkWindow::Init() {
    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion[0])->createRegion(
        mtl::MemManager::getHandleMEM2(), 0x4000, lbl_eu_804FFCA4, 0);
    Class_8045F858 guard(reinterpret_cast<UnkClass_8045F564*>(&mMemRegion[0]));

    lbl_eu_80664044++;

    void* tagMem = mtl::MemManager::allocate(
        0x858, getAllocHandle__10CLibLayoutFv());
    if (tagMem != 0) {
        tagMem = __ct__CTagProcessor(tagMem);
    }
    field_5C = reinterpret_cast<nw4r::lyt::AnimTransform*>(tagMem);
    CTalkWinTagProc* tagProc = reinterpret_cast<CTalkWinTagProc*>(tagMem);
    tagProc->setMessage(lbl_eu_80667278, lbl_eu_80667278,
                        reinterpret_cast<u8*>(field_9C), field_68);

    // Early-init dispatch on the tag-processor mode byte (+0x814). Modes 0-6
    // are small setups; mode 7 builds the layout; 3/8 just mark ready.
    u8 type = tagProc->field_0x814;
    switch (type) {
    case 0:
        func_80135464(tagProc->field_0x81A, 0, tagProc->field_0x81C,
                      lbl_eu_8066727C, lbl_eu_8066727C);
        field_64 = 1;
        break;
    case 1:
        func_8013DA60(tagProc->field_0x818, 0, 0);
        field_64 = 1;
        break;
    case 2: {
        CTalkWinSrc* s2 = reinterpret_cast<CTalkWinSrc*>(
            func_800B708C(field_68));
        if (s2 != 0) {
            s2->vfn158(0);
        }
        field_64 = 1;
        break;
    }
    case 3:
    case 8:
        field_64 = 1;
        break;
    case 4: {
        CTalkWinSrc* s4 = reinterpret_cast<CTalkWinSrc*>(
            func_800B708C(field_68));
        func_8013E204(s4->field_0x8C);
        field_64 = 1;
        break;
    }
    case 5:
        func_8013E104(tagProc->field_0x818);
        field_64 = 1;
        break;
    case 6:
        func_801342B0();
        field_64 = 1;
        break;
    case 7: {
        // Full layout build: page index, layout + 5 animations from the
        // shared arc (re-acquired per build call), font binding, then the
        // message / corner panes.
        field_A4 = tagProc->field_0x816;

        func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
            &mpLayout, func_801355F4(), &lbl_eu_804FFCA4[0xc]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            mpLayout, &field_88, func_801355F4(), &lbl_eu_804FFCA4[0x23]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            mpLayout, &field_8C, func_801355F4(), &lbl_eu_804FFCA4[0x3d]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            mpLayout, &field_90, func_801355F4(), &lbl_eu_804FFCA4[0x58]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            mpLayout, &field_94, func_801355F4(), &lbl_eu_804FFCA4[0x73]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            mpLayout, &field_98, func_801355F4(), &lbl_eu_804FFCA4[0x92]);

        void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
            1, mpLayout);
        func_8013676C(mpLayout->GetRootPane(),
                      reinterpret_cast<CTalkFontObj*>(fontObj)->getFontHandle());

        // Prime the message-window pane and bind the four corner panes.
        CTalkWinPane* paneB0 = reinterpret_cast<CTalkWinPane*>(
            mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_804FFCA4[0xb0], 1));
        paneB0->vfunc_0x78();
        paneB0->vfunc_0x74(0x20);

        char* msg = func_80138DA4(
            reinterpret_cast<CTalkWinSrc*>(
                func_800B708C(field_68))->getText());
        func_80136B4C(mpLayout, &lbl_eu_804FFCA4[0xb0], msg, 0);
        func_80136B4C(mpLayout, &lbl_eu_804FFCA4[0xb9], msg, 0);
        func_80136B4C(mpLayout, &lbl_eu_804FFCA4[0xc7], msg, 0);
        func_80136B4C(mpLayout, &lbl_eu_804FFCA4[0xd5], msg, 0);
        func_80136B4C(mpLayout, &lbl_eu_804FFCA4[0xe3], msg, 0);

        // Page-select pane: attach the tag processor and name the sub-panes.
        CTalkWinPane* paneF1 = reinterpret_cast<CTalkWinPane*>(
            mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_804FFCA4[0xf1], 1));
        paneF1->vfunc_0x78();
        paneF1->vfunc_0x74(0x400);
        paneF1->mFlag |= 1;
        paneF1->field_F8 = reinterpret_cast<u32>(field_5C);
        func_80136B4C(mpLayout, &lbl_eu_804FFCA4[0xfc], &lbl_eu_804FFCA4[0x107], 0);
        func_80136B4C(mpLayout, &lbl_eu_804FFCA4[0x108], &lbl_eu_804FFCA4[0x107], 0);
        func_80136B4C(mpLayout, &lbl_eu_804FFCA4[0x113], &lbl_eu_804FFCA4[0x107], 0);

        func_80127764(field_5C,
                      mpLayout->GetRootPane()->FindPaneByName(
                          &lbl_eu_804FFCA4[0xf1], 1),
                      mpLayout->GetRootPane()->FindPaneByName(
                          lbl_eu_8052DF70[field_A4], 1),
                      mpLayout->GetRootPane()->FindPaneByName(
                          &lbl_eu_804FFCA4[0xb0], 1), 0);

        func_8012CD38(this);
        func_8012D3D8(this);

        // Hide the arrow panes, then reveal the active side.
        reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
            FindPaneByName(&lbl_eu_804FFCA4[0x11e], 1))->mFlag &= ~1;
        reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
            FindPaneByName(&lbl_eu_804FFCA4[0x12c], 1))->mFlag &= ~1;
        if (field_A1 != 0) {
            reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                FindPaneByName(&lbl_eu_804FFCA4[0x11e], 1))->mFlag |= 1;
        } else {
            reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                FindPaneByName(&lbl_eu_804FFCA4[0x12c], 1))->mFlag |= 1;
        }

        // Default animation set: intro anim active, page anims off.
        if (field_A0 == 0) {
            lbl_eu_80664040 = 0;
            mpLayout->SetAnimationEnable(field_8C, false);
            mpLayout->SetAnimationEnable(field_90, false);
            mpLayout->SetAnimationEnable(field_94, false);
            mpLayout->SetAnimationEnable(field_98, false);
            mpLayout->SetAnimationEnable(field_88, true);
            reinterpret_cast<CTalkAnimFrame*>(field_88)->mFrame =
                lbl_eu_80667280;
        }
        mpLayout->Animate(0);

        // Page-mode banner panes + the shared 'timg' page texture.
        u8 opt = tagProc->field_0x815;
        reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
            FindPaneByName(&lbl_eu_804FFCA4[0x13b], 1))->mFlag |= 1;
        reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
            FindPaneByName(&lbl_eu_804FFCA4[0x141], 1))->mFlag &= ~1;
        void* res = func_801355F4()->GetResource(
            0x74696d67, &lbl_eu_804FFCA4[0x147], 0);
        func_801355F4()->GetResource(
            0x74696d67, &lbl_eu_804FFCA4[0x161], 0);
        func_80138078(0x29);

        // Mode-specific page setup (retail keeps the type==0 guard as dead
        // code: type is always 7 on this path).
        if (type != 0) {
            if ((opt & 0x4) != 0) {
                if (field_A4 == 0) {
                    reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                        FindPaneByName(&lbl_eu_804FFCA4[0x13b], 1))->mFlag |= 1;
                    reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                        FindPaneByName(&lbl_eu_804FFCA4[0x141], 1))->mFlag &= ~1;
                    res = func_801355F4()->GetResource(
                        0x74696d67, &lbl_eu_804FFCA4[0x147], 0);
                    func_801355F4()->GetResource(
                        0x74696d67, &lbl_eu_804FFCA4[0x161], 0);
                    func_80138078(0x29);
                } else if (field_A4 == 1) {
                    reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                        FindPaneByName(&lbl_eu_804FFCA4[0x13b], 1))->mFlag &= ~1;
                    reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                        FindPaneByName(&lbl_eu_804FFCA4[0x141], 1))->mFlag |= 1;
                    res = func_801355F4()->GetResource(
                        0x74696d67, &lbl_eu_804FFCA4[0x17b], 0);
                    func_801355F4()->GetResource(
                        0x74696d67, &lbl_eu_804FFCA4[0x195], 0);
                    lbl_eu_80664040 = 1;
                    reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                        FindPaneByName(&lbl_eu_804FFCA4[0x11e], 1))->mFlag &= ~1;
                    reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                        FindPaneByName(&lbl_eu_804FFCA4[0x12c], 1))->mFlag &= ~1;
                    func_80138078(0x72);
                } else if (field_A4 == 3) {
                    reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                        FindPaneByName(&lbl_eu_804FFCA4[0x13b], 1))->mFlag |= 1;
                    reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                        FindPaneByName(&lbl_eu_804FFCA4[0x141], 1))->mFlag &= ~1;
                    res = func_801355F4()->GetResource(
                        0x74696d67, &lbl_eu_804FFCA4[0x1af], 0);
                    func_801355F4()->GetResource(
                        0x74696d67, &lbl_eu_804FFCA4[0x1c9], 0);
                    func_80138078(0x74);
                }
            }
            if ((opt & 0x1) != 0) {
                lbl_eu_80664040 = 1;
                reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                    FindPaneByName(&lbl_eu_804FFCA4[0x11e], 1))->mFlag &= ~1;
                reinterpret_cast<CTalkWinPane*>(mpLayout->GetRootPane()->
                    FindPaneByName(&lbl_eu_804FFCA4[0x12c], 1))->mFlag &= ~1;
            }
            if ((opt & 0x2) != 0) {
                field_A8 = tagProc->field_0x817;
            }
        }

        // Bind the page texture to the two arrow panes.
        if (res != 0) {
            func_80137E7C(mpLayout, &lbl_eu_804FFCA4[0x11e], res);
            func_80137E7C(mpLayout, &lbl_eu_804FFCA4[0x12c], res);
        }

        IScnRender* render = reinterpret_cast<IScnRender*>(this);
        if (this != 0) {
            render = reinterpret_cast<IScnRender*>(&mScnRender);
        }
        mScene->addRenderCB(render, 0xa, 0);
        break;
    }
    default:
        break;
    }

    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion[0])->func_8045F810();
}

// ---------------------------------------------------------------------------
// CTalkWindow::Move (us-8012d374)
// Per-frame driver: gate on the task/global flags, run the shared
// func_8012CD38 update, dispatch the state machine (1..5), then always
// advance the layout animations.
// ---------------------------------------------------------------------------
void CTalkWindow::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000)) return;
    if (func_8013BE50() == 0) return;
    if (cf::CfGameManager::func_800829B8() != 0) return;
    func_8012CD38(this);

    // Signed switch: retail emits a beq dispatch chain with signed cmpwi.
    switch ((int)field_B0) {
    case 1:
        func_8012D8C0(this);
        break;
    case 2:
        func_8012DA6C(this);
        break;
    case 3:
        if (func_80137444(field_90, lbl_eu_80667284) != 0) {
            IScnRender* render = reinterpret_cast<IScnRender*>(this);
            if (this != 0) {
                render = reinterpret_cast<IScnRender*>(&mScnRender);
            }
            mScene->removeRenderCB(render);
            field_64 = 1;
            field_B0 = 0;
        }
        break;
    case 4: {
        if (func_80137444(field_94, lbl_eu_80667284) == 0) break;
        mpLayout->SetAnimationEnable(field_90, false);
        mpLayout->SetAnimationEnable(field_94, false);
        mpLayout->SetAnimationEnable(field_88, false);
        mpLayout->SetAnimationEnable(field_8C, false);
        mpLayout->SetAnimationEnable(field_98, true);
        reinterpret_cast<CTalkAnimFrame*>(field_98)->mFrame = lbl_eu_80667280;
        // Declared p2-first: MWCC allocates the two pane locals in reverse
        // declaration order, matching retail's r28/r29 split.
        nw4r::lyt::Pane* p2;
        nw4r::lyt::Pane* p1;
        p1 = mpLayout->GetRootPane()->
            FindPaneByName(&lbl_eu_804FFCA4[0xf1], 1);
        p2 = mpLayout->GetRootPane()->FindPaneByName(
            lbl_eu_8052DF70[field_A4], 1);
        nw4r::lyt::Pane* p3 = mpLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_804FFCA4[0xb0], 1);
        func_80127E74(field_5C, p1, p2, p3);
        func_8012D3D8(this);
        field_B0 = 5;
        break;
    }
    case 5:
        if (func_80137444(field_98, lbl_eu_80667284) != 0) {
            mpLayout->SetAnimationEnable(field_90, false);
            mpLayout->SetAnimationEnable(field_94, false);
            mpLayout->SetAnimationEnable(field_88, false);
            mpLayout->SetAnimationEnable(field_98, false);
            mpLayout->SetAnimationEnable(field_8C, true);
            reinterpret_cast<CTalkAnimFrame*>(field_8C)->mFrame =
                lbl_eu_80667280;
            field_B0 = 2;
        }
        break;
    default:
        break;
    }

    mpLayout->Animate(0);
}

// ---------------------------------------------------------------------------
// ~CTalkWindow (us-8012c828)
// Complete-object destructor: destroy the embedded UnkClass_8045F564 region
// (flags -1), then the CProcess base at +0 (guarded by the nested double
// null-check - an MWCC D2-inlined-into-D1 artifact), then conditional
// operator delete when flags > 0. Same shape as ~CSysWinSave.
// ---------------------------------------------------------------------------
extern "C" void* __dt__11CTalkWindowFv(CTalkWindow* _this, int flags) {
    if (_this != 0) {
        __dt__17UnkClass_8045F564Fv(
            reinterpret_cast<UnkClass_8045F564*>(&_this->mMemRegion[0]), -1);
        if (_this != 0) {
            if (_this != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(_this), 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// ---------------------------------------------------------------------------
// CTalkWindow::Term (us-8012d2c4)
// Wait for VI draw completion, drop the instance counter, detach the render
// callback, delete the two layout objects and release the layout-build
// scratch region.
// ---------------------------------------------------------------------------
void CTalkWindow::Term() {
    CDeviceVI::waitForDrawDone();
    lbl_eu_80664044--;
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mScnRender);
    }
    mScene->removeRenderCB(render);
    if (field_5C != 0) {
        delete field_5C;
        field_5C = 0;
    }
    if (mpLayout != 0) {
        delete mpLayout;
        mpLayout = 0;
    }
    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion[0])->func_8045F778();
}

// ---------------------------------------------------------------------------
// CTalkWindow::cbRenderBefore (us-8012d6a0)
// Gate prefix (task busy / global bit / scene-active / talk source) then draw
// the layout through a stack DrawInfo.
// ---------------------------------------------------------------------------
void CTalkWindow::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0)
        return;
    if (func_800B708C((int)field_68) == 0)
        return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls
    // to match the retail direct calls (a C++ local would virtual-dispatch
    // its scope-exit destructor and bloat the body).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137038(mpLayout, (nw4r::lyt::DrawInfo*)&drawInfo[0], 0, 1);
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
}

// ---------------------------------------------------------------------------
// func_8012CC78 (us-8012d744)
// Talk-window factory: copy the message source into a zeroed stack buffer,
// allocate the 0xB8-byte CTalkWindow on the work heap, run the ctor (retail
// C-ABI __ct__CTalkWindow) and register it under the caller's parent.
// ---------------------------------------------------------------------------
extern "C" CTalkWindow* func_8012CC78(CProcess* parent, u32 arg1, u32 arg2,
                                      const u8* msgSrc, u32 arg3, u32 arg4,
                                      u32 arg5) {
    u8 buf[0x800];
    memset(buf, 0, 0x800);
    func_80136400((const char*)msgSrc, (u16*)buf, 0x400);

    u32 workMem = CWorkThreadSystem::getWorkMem();
    CTalkWindow* obj =
        (CTalkWindow*)mtl::MemManager::allocate(0xb8, workMem);
    if (obj != 0) {
        obj = __ct__CTalkWindow(obj, arg1, arg2, buf, arg3, arg4, arg5);
    }
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return obj;
}

// ---------------------------------------------------------------------------
// sinit_8012DE98 (us-8012e968)
// Static initializer for the talk-window float table lbl_eu_80573A70 (48
// floats). MWCC emits the constant loads from the shared .sdata2 pool, so the
// source references the retail SDA2 constants by name.
// ---------------------------------------------------------------------------
extern f32 lbl_eu_80573A70[48];
extern f32 lbl_eu_80667280;   // A - 39 uses
extern f32 lbl_eu_80667298;   // B - 3 uses
extern f32 lbl_eu_806672C0;   // C - 3 uses
extern f32 lbl_eu_80667288;   // D - 3 uses
extern f32 lbl_eu_806672C4;   // E - 3 uses

extern "C" void sinit_8012DE98() {
    lbl_eu_80573A70[0] = lbl_eu_80667280;
    lbl_eu_80573A70[1] = lbl_eu_80667280;
    lbl_eu_80573A70[2] = lbl_eu_80667280;
    lbl_eu_80573A70[3] = lbl_eu_80667298;
    lbl_eu_80573A70[4] = lbl_eu_806672C0;
    lbl_eu_80573A70[5] = lbl_eu_80667280;
    lbl_eu_80573A70[6] = lbl_eu_80667280;
    lbl_eu_80573A70[7] = lbl_eu_806672C0;
    lbl_eu_80573A70[8] = lbl_eu_80667280;
    lbl_eu_80573A70[9] = lbl_eu_80667288;
    lbl_eu_80573A70[10] = lbl_eu_806672C0;
    lbl_eu_80573A70[11] = lbl_eu_80667280;
    lbl_eu_80573A70[12] = lbl_eu_80667298;
    lbl_eu_80573A70[13] = lbl_eu_80667280;
    lbl_eu_80573A70[14] = lbl_eu_80667280;
    lbl_eu_80573A70[15] = lbl_eu_80667280;
    lbl_eu_80573A70[16] = lbl_eu_80667280;
    lbl_eu_80573A70[17] = lbl_eu_80667280;
    lbl_eu_80573A70[18] = lbl_eu_80667288;
    lbl_eu_80573A70[19] = lbl_eu_80667280;
    lbl_eu_80573A70[20] = lbl_eu_80667280;
    lbl_eu_80573A70[21] = lbl_eu_80667298;
    lbl_eu_80573A70[22] = lbl_eu_806672C4;
    lbl_eu_80573A70[23] = lbl_eu_80667280;
    lbl_eu_80573A70[24] = lbl_eu_80667280;
    lbl_eu_80573A70[25] = lbl_eu_806672C4;
    lbl_eu_80573A70[26] = lbl_eu_80667280;
    lbl_eu_80573A70[27] = lbl_eu_80667288;
    lbl_eu_80573A70[28] = lbl_eu_806672C4;
    lbl_eu_80573A70[29] = lbl_eu_80667280;
    lbl_eu_80573A70[30] = lbl_eu_80667280;
    lbl_eu_80573A70[31] = lbl_eu_80667280;
    lbl_eu_80573A70[32] = lbl_eu_80667280;
    lbl_eu_80573A70[33] = lbl_eu_80667280;
    lbl_eu_80573A70[34] = lbl_eu_80667280;
    lbl_eu_80573A70[35] = lbl_eu_80667280;
    lbl_eu_80573A70[36] = lbl_eu_80667280;
    lbl_eu_80573A70[37] = lbl_eu_80667280;
    lbl_eu_80573A70[38] = lbl_eu_80667280;
    lbl_eu_80573A70[39] = lbl_eu_80667280;
    lbl_eu_80573A70[40] = lbl_eu_80667280;
    lbl_eu_80573A70[41] = lbl_eu_80667280;
    lbl_eu_80573A70[42] = lbl_eu_80667280;
    lbl_eu_80573A70[43] = lbl_eu_80667280;
    lbl_eu_80573A70[44] = lbl_eu_80667280;
    lbl_eu_80573A70[45] = lbl_eu_80667280;
    lbl_eu_80573A70[46] = lbl_eu_80667280;
    lbl_eu_80573A70[47] = lbl_eu_80667280;
}

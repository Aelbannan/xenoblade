// Auto-scaffolded catalog TU for kyoshin/CKizunagram
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CKizunagram.hpp"
#include "kyoshin/code_80135FDC.hpp"  // layout/anim/font helpers (extern "C" pre-mangled names)
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/util/MemManager.hpp"
#include <revolution/TPL.h>
#include <new>

// mtl::MemManagerF::allocate is a member (this, size, kind); the shared header
// declares the two-arg caller form used elsewhere, so route this call through
// an explicitly-typed function pointer.
typedef void* (*KizunaAllocFn)(void*, u32, u32);

// PS kernels defined further down in this TU.
extern "C" void func_80258F5C(float* dst, const float* a, const float* b);
extern "C" void func_80258F80(float* dst, const float* src, float scale);

// --- C-linkage / mangled-symbol callees used by the target functions ---
// func_80137444 is C++-linkage (mangled retail), func_80137510 / func_80231848 /
// copyVEC2 are unmangled in retail (extern "C"), and func_80136E84 is referenced
// by its mangled retail identifier (see CSaveLoad for the same pattern).
extern const float lbl_eu_80668828;
extern const float lbl_eu_80668834;
extern const float lbl_eu_80668830;
extern const float lbl_eu_8066884C;
extern const float lbl_eu_80668850;
extern const float lbl_eu_80668854;
extern const float lbl_eu_80668858;
extern const float lbl_eu_80668848;
extern const float lbl_eu_8066885C;
extern const float lbl_eu_80668860;
extern const float lbl_eu_80668864;
extern const float lbl_eu_80668868;
extern const float lbl_eu_8066886C;
extern const float lbl_eu_80668870;
extern u32 lbl_eu_80668838;
extern u32 lbl_eu_8066883C;
extern char lbl_eu_8050CB20[];

void func_80138078(u32 number);

// Same-TU display-state helper consumed by func_8025AB04 / func_8025AB84
// (declaration moved to CKizunagram.hpp C-linkage imports).

// Forward declarations for callees used in func_8025CAB4
// func_8025CE00 is target 5 (takes the display self); the tail calls pass the
// same self pointer. func_8025CE78 / func_8025CF1C are still-unknown stubs.
// noinline: -ipa would fold the empty stub into func_8025CAB4's case-1 tail
// call, turning the retail `b func_8025CE78` into a bare return.
extern "C" __declspec(noinline) void func_8025CE78(UnkKizunaSelfCE78* self);
extern "C" void func_80257F9C(UnkKizunaSelf57D90* self, u32 a);
extern "C" __declspec(noinline) void func_8025CF1C(void* self) {
    if (*(u8*)((char*)self + 0x8C) != 0) {
        func_80257F9C((UnkKizunaSelf57D90*)((char*)self + 0xAC), 1);
    } else {
        func_80257F9C((UnkKizunaSelf57D90*)((char*)self + 0xAC), 0);
    }
}

// Object with a dispatch byte at 0x3A - drives display state in CKizunagram
struct UnkKizunaDisp {
    u8 _00[0x3A];
    u8 field_0x3A;
};

void func_8025C870() {}


// Toggle both pane entries each frame: walk the +0x08 child to its +0x10
// sub-object, pull its slot-15 layout pane for each id in a 2-word table, and
// repaint the pane matching the given phase byte.
extern "C" __declspec(noinline) void func_80257F9C(UnkKizunaSelf57D90* self, u32 a) {
    u32 paneIds[2] = { lbl_eu_80668838, lbl_eu_8066883C };
    for (u8 i = 0; i < 2; i++) {
        UnkKizunaRes59344* res =
            ((UnkKizunaMid59344*)self->field8)->field10->target(paneIds[i], 1);
        func_80124270((nw4r::lyt::Pane*)res, (u32)(i == a));
    }
}

// ctor: retail __ct__CKizunaRadar (unmangled). Stores the manual vtable
// label, the r4 arg, then zero/float fields.
// noinline: keeps -O4,s IPA from folding these small ctors into callers like
// CKizunagram::OnFileEvent (retail calls them out-of-line).
extern "C" __declspec(noinline) void __ct__CKizunaRadar(CKizunaRadar* self, void* arg) {
    f32 idle = lbl_eu_80668828;
    *(void**)self = (void*)lbl_eu_80537608;
    self->mArg = arg;
    self->mField08 = 0;
    self->mField0C = 0;
    self->mField10 = 0;
    self->mField14 = idle;
    self->mField18 = idle;
}

CKizunaRadar::~CKizunaRadar() {}

CKizunaCur::~CKizunaCur() {}

// noinline keeps -O4,s IPA from folding this into same-TU callers
// (func_8025CF40 calls it out-of-line in retail).
extern "C" __declspec(noinline) void __ct__CKizunaLine(CKizunaLine* self, u32 arg4, u32 arg8, u8 arg3D) {
    f32 idle = lbl_eu_80668828;
    self->mVtable = lbl_eu_805375F0;
    self->field4 = arg4;
    self->field8 = arg8;
    self->field0C = 0;
    self->field10 = 0;
    self->field14 = 0;
    self->field15 = 1;
    self->field16 = 0;
    self->field17 = 0;
    self->field24 = 0;
    self->field26 = 0;
    self->field28 = idle;
    self->field2C = idle;
    self->field30 = idle;
    self->field34 = 0;
    self->field36 = 0;
    self->field38 = idle;
    self->field3C = 0;
    self->field3D = arg3D;
    self->field40 = idle;
}

CKizunaLine::~CKizunaLine() {}

// Release/null a +0x08 child object: if non-null, finalize it (vtable slot 2)
// with a 1 flag, then clear the pointer.
// extern "C" + noinline keeps same-TU callers emitting an unmangled `bl
// func_80257D90` (retail links this symbol unmangled) instead of inlining.
extern "C" __declspec(noinline) void func_80257D90(UnkKizunaSelf57D90* self) {
    // Retail re-tests field8 inside the guard: the duplicated condition CSEs to
    // one cmpwi with two beq's (dead second branch — the btm_sec pattern).
    if (self->field8 != 0) {
        if (self->field8 != 0) {
            self->field8->target2(1);
        }
        self->field8 = 0;
    }
}

// Same shape as func_80257D90 (sibling release/null helper).
extern "C" __declspec(noinline) void func_80257F44(UnkKizunaSelf57D90* self) {
    // Same retail shape as func_80257D90: duplicated guard test CSEs to one
    // cmpwi with two beq's; zeroing inside the outer if.
    if (self->field8 != 0) {
        if (self->field8 != 0) {
            self->field8->target2(1);
        }
        self->field8 = 0;
    }
}

// Build the current-line layout (+0x08) from the shared arc string at +0x27,
// attach its anim transform at +0x0C, then publish both via the layout's
// vtable slots 11 / 14.
extern "C" __declspec(noinline) void func_80257E58(UnkKizunaSelf57E58* self) {
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        (nw4r::lyt::Layout**)&self->field8, self->field4, lbl_eu_8050CB20 + 0x27);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        (nw4r::lyt::Layout*)self->field8, &self->field0C, self->field4, lbl_eu_8050CB20 + 0x3f);
    self->field8->slot11((u32)self->field0C, 1);
    self->field8->slot14(0);
}

// us-8025a308 (0x802580CC): (re)build the kizuna-line layout from the shared
// arc resource, bind its animation and the device font, then either populate
// the current-kizuna display (id != 0) or publish the idle cursor position,
// and finally refresh every label/texture pane from the BDAT tables.
extern "C" __declspec(noinline) void func_802580CC(UnkKizunaLineBuild* self) {
    char nameBuf[0x20];
    char* str = lbl_eu_8050CB20;

    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        (nw4r::lyt::Layout**)&self->layout,
        (nw4r::lyt::ArcResourceAccessor*)self->acc, str + 0x5c);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        (nw4r::lyt::Layout*)self->layout, &self->anim,
        (nw4r::lyt::ArcResourceAccessor*)self->acc, str + 0x70);

    // Bind the device font to the layout's root pane.
    nw4r::lyt::Pane* rootPane =
        (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)->field10;
    void* font = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
        1, (nw4r::lyt::Layout*)self->layout);
    u32 bound = ((UnkKizunaFontView*)font)->vf7();
    func_8013676C(rootPane, bound);

    self->layout->slot11((u32)self->anim, 1);
    self->layout->slot14(0);

    // Reset then hide the shared line pane.
    ((UnkKizunaMid59344*)self->layout)
        ->field10->target((int)(str + 0x8b), 1)
        ->fieldB8 = 0;
    func_80124270(
        (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)->field10->target(
            (int)(str + 0x8b), 1),
        0);

    // Region-dependent line color overrides.
    void* lytv = self->layout;
    u8 lang = (u8)lbl_eu_80664184;
    if (lang == 0x19) {
        func_80139A18(lytv, str + 0x93, &lbl_eu_80664838, &lbl_eu_80664840);
        func_80137CD4(lytv, str + 0x93, (int)0xF9FFF2FF, 0x94AA8CFF);
    } else if (lang == 0x1A) {
        func_80139A18(lytv, str + 0x93, &lbl_eu_80664848, &lbl_eu_80664850);
        func_80137CD4(lytv, str + 0x93, -1, 0x8CA0EBFF);
    } else {
        switch (func_801372B4(lang)) {
        case 0:
        case 1:
            func_80139A18(lytv, str + 0x93, &lbl_eu_806647E8, &lbl_eu_806647F0);
            func_80137CD4(lytv, str + 0x93, -1, 0xEEB88AFF);
            break;
        case 2:
            func_80139A18(lytv, str + 0x93, &lbl_eu_806647F8, &lbl_eu_80664800);
            func_80137CD4(lytv, str + 0x93, -1, 0xE0C072FF);
            break;
        case 3:
            func_80139A18(lytv, str + 0x93, &lbl_eu_80664808, &lbl_eu_80664810);
            func_80137CD4(lytv, str + 0x93, -1, 0xE1F8A8FF);
            break;
        case 4:
            func_80139A18(lytv, str + 0x93, &lbl_eu_80664818, &lbl_eu_80664820);
            func_80137CD4(lytv, str + 0x93, -1, 0xB4DEFFFF);
            break;
        case 5:
            func_80139A18(lytv, str + 0x93, &lbl_eu_80664828, &lbl_eu_80664830);
            func_80137CD4(lytv, str + 0x93, -1, 0xBCBAE6FF);
            break;
        }
    }

    // Fresh overlay pane appended under the shared root.
    nw4r::lyt::Pane* newPane = NULL;
    void* paneMem = allocate__Q23mtl10MemManagerFUlUl(
        0xD8, getAllocHandle__10CLibLayoutFv());
    if (paneMem != NULL) {
        newPane = new (paneMem) nw4r::lyt::Pane();
    }
    newPane->SetName(str + 0x9a);
    func_80124270(newPane, 1);
    nw4r::lyt::Pane* parent =
        (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)->field10->target(
            (int)(str + 0xa4), 1);
    parent->AppendChild(newPane);

    u16 id;
    if (self->field3D == 0) {
        id = (u16)func_8009ECE0();
    } else {
        id = 0;
    }

    if (id != 0) {
        self->field26 = id;
        UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(lbl_eu_80664098);
        UnkKizunaObj59344* rootP =
            ((UnkKizunaMid59344*)self->layout)->field10;

        // Pull both anchor panes and shift posA by the BDAT-scaled delta of
        // posB so posA ends up at the mirrored display position.
        UnkKizunaRes59344* resA = rootP->target((int)(str + 0xa4), 1);
        UnkKizunaVec3 posA = resA->pos;
        UnkKizunaRes59344* resB = rootP->target((int)(str + 0x9a), 1);
        UnkKizunaVec3 posB = resB->pos;

        s16 vx = (s16)func_80136330(mgr, str + 0xaf, id);
        s16 vy = (s16)func_80136330(mgr, str + 0xb8, id);
        UnkKizunaVec3 delta;
        delta.x = (f32)vx;
        delta.y = (f32)vy;
        delta.z = lbl_eu_80668828;
        nw4r::math::VEC3Sub((nw4r::math::VEC3*)&posB.x,
                            (const nw4r::math::VEC3*)&posB.x,
                            (const nw4r::math::VEC3*)&delta.x);
        func_80258F5C(&posA.x, &posA.x, &posB.x);

        resA = rootP->target((int)(str + 0xa4), 1);
        copyVEC3((f32*)&resA->pos, (f32*)&posA);

        char* texRowName = func_8013639C(mgr, str + 0xca, id);
        if (strcmp(texRowName, str + 0xd3) != 0) {
            char* tplName =
                func_80138F78((u32)(u16)func_80136254(mgr, str + 0xd4, id));
            TPLPalette* pal =
                (TPLPalette*)self->texSrc->getTex(0x74696D67, (u32)tplName, 0);
            if (pal != 0) {
                nw4r::lyt::Picture* pic = NULL;
                void* picMem = allocate__Q23mtl10MemManagerFUlUl(
                    0xF0, getAllocHandle__10CLibLayoutFv());
                if (picMem != NULL) {
                    nw4r::lyt::TexMap texMap(pal, 0);
                    pic = new (picMem) nw4r::lyt::Picture(texMap);
                }
                char picNameBuf[0x20];
                sprintf(picNameBuf, str + 0xe0, texRowName);
                pic->SetName(picNameBuf);

                // Position the picture from the same BDAT columns (z fixed).
                s16 px = (s16)func_80136330(mgr, str + 0xaf, id);
                s16 py = (s16)func_80136330(mgr, str + 0xb8, id);
                UnkKizunaVec3 picPos;
                picPos.x = (f32)px;
                picPos.y = (f32)py;
                picPos.z = lbl_eu_80668828;
                copyVEC3((f32*)&((KizunaPaneTranslateView*)pic)->translate,
                         (f32*)&picPos);

                func_80124270(pic, 1);
                nw4r::lyt::Pane* picParent =
                    (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)
                        ->field10->target((int)(str + 0x9a), 1);
                picParent->AppendChild(pic);
            }
        }
    } else {
        // No current kizuna: reset state, sum the two anchor positions and
        // publish the constant-scaled result as the idle cursor position.
        self->field24 = 0;
        self->field26 = 0;
        UnkKizunaObj59344* rootP =
            ((UnkKizunaMid59344*)self->layout)->field10;
        UnkKizunaRes59344* res = rootP->target((int)(str + 0xa4), 1);
        UnkKizunaVec3 posBase = res->pos;
        UnkKizunaVec3 posCur = res->pos; // retail keeps this dead copy
        UnkKizunaRes59344* resAdd =
            ((UnkKizunaObj59344*)res)->target((int)(str + 0xe3), 1);
        UnkKizunaVec3 posAdd = resAdd->pos;
        func_80258F5C(&posBase.x, &posBase.x, &posAdd.x);
        UnkKizunaRes59344* resScale =
            ((UnkKizunaObj59344*)res)->target((int)(str + 0xe3), 1);
        f32 scalePair[2];
        func_80127BC4(scalePair, resScale->scale);
        f32 scaled[3];
        func_80258F80(scaled, &posBase.x, lbl_eu_80668830);
        copyVEC3(&self->result18.x, scaled);
        self->field24 = 1;
        UnkKizunaVec3 out = self->result18;
        (void)posCur;
        func_80259394((UnkKizunaSelf59394*)self, &out);
    }

    lbl_eu_80664858 = 0;
    lbl_eu_8066485A = 0;
    func_80259820(reinterpret_cast<UnkKizunaSelf9820*>(self));

    // Pick the progress message tag from the game-progress byte at 0x20.
    u32 prog20 = (u16)func_8009CF8C(0x20);
    const char* msg;
    if (prog20 >= 0x64) {
        if (prog20 >= 0x89) {
            msg = (prog20 >= 0x113) ? str + 0xec : str + 0xfe;
        } else {
            msg = (prog20 >= 0x80) ? str + 0x110 : str + 0x122;
        }
    } else if (prog20 >= 0x2a) {
        msg = (prog20 >= 0x45) ? str + 0x134 : str + 0x146;
    } else if (prog20 >= 0xb) {
        msg = str + 0x158;
    } else {
        msg = str + 0x146;
    }

    void* msgTex = self->acc->getTex(0x74696D67, (u32)msg, 0);
    if (msgTex != 0) {
        func_80137E7C(self->layout, str + 0x16a, msgTex);
    }

    // Hide the five fixed number/name pane pairs.
    for (u32 i = 1; i <= 5; i++) {
        sprintf(nameBuf, str + 0x171, (u8)i);
        func_80124270(
            (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)->field10->target(
                (int)nameBuf, 1),
            0);
        sprintf(nameBuf, str + 0x17c, (u8)i);
        func_80124270(
            (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)->field10->target(
                (int)nameBuf, 1),
            0);
    }

    // Fixed per-layout color/tag overrides, then per-row entries.
    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(lbl_eu_80664098);
    nw4r::lyt::Layout* lytL = (nw4r::lyt::Layout*)self->layout;
    u32 rowCount = func_8003B1EC(mgr);
    func_80136B4C(lytL, str + 0x189, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x197, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x1a5, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x1b3, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x1c1, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x1cf, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x1de, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x1ed, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x1fc, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x20b, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x21a, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x229, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x238, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x247, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x256, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x265, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x274, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x283, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x292, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x2a1, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x2b0, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x2bf, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x2ce, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x2dd, str + 0xd3, 0);
    func_80136B4C(lytL, str + 0x2ec, str + 0xd3, 0);

    for (u32 j = 1;; j++) {
        if ((s32)(u16)j > (s32)rowCount) {
            break;
        }
        u32 rowProg = (u16)func_80136254(mgr, str + 0xc1, (u16)j);
        if (rowProg != 0 && func_8009CF8C(rowProg + 0xa20) != 0) {
            char* nm = func_8013639C(mgr, str + 0xca, (u16)j);
            if (strcmp(nm, str + 0xd3) != 0) {
                u32 colv = (u16)func_80136254(mgr, str + 0x2fb, (u16)j);
                u32 state =
                    func_801372B4(func_8013600C(str + 0x303, str + 0x310, colv));
                sprintf(nameBuf, str + 0x171, (u8)state);
                func_80124270(
                    (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)
                        ->field10->target((int)nameBuf, 1),
                    1);
                sprintf(nameBuf, str + 0x17c, (u8)state);
                func_80124270(
                    (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)
                        ->field10->target((int)nameBuf, 1),
                    1);
                char* label =
                    func_80136190(str + 0x316, str + 0x321, (u8)state + 0x24);
                sprintf(nameBuf, str + 0x326, (u8)state);
                func_80136B4C(lytL, nameBuf, label, 0);
                sprintf(nameBuf, str + 0x336, (u8)state);
                func_80136B4C(lytL, nameBuf, label, 0);
                sprintf(nameBuf, str + 0x347, (u8)state);
                func_80136B4C(lytL, nameBuf, label, 0);
                sprintf(nameBuf, str + 0x358, (u8)state);
                func_80136B4C(lytL, nameBuf, label, 0);
                sprintf(nameBuf, str + 0x369, (u8)state);
                func_80136B4C(lytL, nameBuf, label, 0);
            }
        }
    }

    // Texture panes for each affinity rank, plus its five progress digits.
    for (u32 k = 1; k <= 5; k++) {
        sprintf(nameBuf, str + 0x17c, (u8)k);
        UnkKizunaRes59344* visPane =
            ((UnkKizunaMid59344*)self->layout)->field10->target((int)nameBuf, 1);
        if (func_801C4648((nw4r::lyt::Pane*)visPane)) {
            u32 prog = func_8009CF8C((u8)k + 0x21);
            const char* texTag = NULL;
            if (prog >= 6000) {
                if (prog < 8000) {
                    texTag = str + 0x3b9;
                } else if (prog < 10001) {
                    texTag = str + 0x3ce;
                }
            } else if (prog >= 4000) {
                texTag = str + 0x3a4;
            } else if (prog >= 2000) {
                texTag = str + 0x38f;
            } else {
                texTag = str + 0x37a;
            }
            if (texTag != NULL) {
                void* tpl = self->acc->getTex(0x74696D67, (u32)texTag, 0);
                if (tpl != 0) {
                    sprintf(nameBuf, str + 0x171, (u8)k);
                    func_80137E7C(self->layout, nameBuf, tpl);
                }
            }
            for (u32 d = 0; d < 5; d++) {
                sprintf(nameBuf, str + 0x3e3, (u8)(d + 1), (u8)k);
                UnkKizunaRes59344* pane =
                    ((UnkKizunaMid59344*)self->layout)->field10->target(
                        (int)nameBuf, 1);
                func_80124270((nw4r::lyt::Pane*)pane,
                              (prog - d * 2000) / 2000);
            }
        }
    }
}

// us-8025b198 (0x8025B198): game-local PSVECAdd kernel — dst[i] = a[i] + b[i], i in 0..2.
// Retail is a pure paired-single body (psq_l/ps_add/psq_st, no frame, arg order r3=dst,
// r4=a, r5=b) that MWCC cannot emit from scalar C++ (no ps_add intrinsic; scalar C++
// yields 9 lfs/fadds/stfs). Isolated Gekko paired-single backend (PLAN.md 17.6).
#if defined(__MWERKS__) && !defined(NONMATCHING)
extern "C" asm void func_80258F5C(register float* dst, register const float* a,
                                  register const float* b) {
    nofralloc
    psq_l f0, 0(r4), 0, 0
    psq_l f1, 0(r5), 0, 0
    ps_add f2, f0, f1
    psq_l f0, 8(r4), 1, 0
    psq_l f1, 8(r5), 1, 0
    psq_st f2, 0(r3), 0, 0
    ps_add f2, f0, f1
    psq_st f2, 8(r3), 1, 0
    blr
}
#else
extern "C" void func_80258F5C(float* dst, const float* a, const float* b) {
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
}
#endif

// us-8025b1bc (0x8025B1BC): game-local PSVECScale kernel — dst = src * scale (3 floats).
// Retail is a pure paired-single body (psq_l/ps_muls0/psq_st, no frame, arg order r3=dst,
// r4=src, f1=scale). MWCC has no ps_muls0 intrinsic and scalar C++ yields
// lfs/fmuls/stfs. Isolated Gekko paired-single backend (PLAN.md 17.6).
#if defined(__MWERKS__) && !defined(NONMATCHING)
extern "C" asm void func_80258F80(register float* dst, register const float* src,
                                  register float scale) {
    nofralloc
    psq_l f0, 0(r4), 0, 0
    ps_muls0 f2, f0, f1
    psq_l f0, 8(r4), 1, 0
    psq_st f2, 0(r3), 0, 0
    ps_muls0 f2, f0, f1
    psq_st f2, 8(r3), 1, 0
    blr
}
#else
extern "C" void func_80258F80(float* dst, const float* src, float scale) {
    dst[0] = src[0] * scale;
    dst[1] = src[1] * scale;
    dst[2] = src[2] * scale;
}
#endif

__declspec(noinline) void func_80258F9C(UnkKizunaSelf58F9C* self) {
    if (self->field0C != 0) {
        if (func_80259DE8(reinterpret_cast<UnkKizunaSelf59DE8*>(self))) {
            if (func_8025A11C(reinterpret_cast<UnkKizunaSelfA11C*>(self))) {
                // mode-specific line-state handler
                switch (self->field34) {
                case 0:
                    func_8025AAE0(self);
                    break;
                case 1:
                    func_8025AB04(reinterpret_cast<UnkKizunaSelfAB*>(self));
                    break;
                case 2:
                    func_8025AB84(reinterpret_cast<UnkKizunaSelfAB*>(self));
                    break;
                case 3:
                    func_8025AC04(self);
                    break;
                }
                if (self->field14 == 0) {
                    self->field16 = 1;
                }
            }
        }
        // countdown / finish pass, then notify the child (slot 14)
        switch (self->field14) {
        case 1:
            func_80259C5C(reinterpret_cast<UnkKizunaSelf59C5C*>(self));
            break;
        case 3:
            func_80259D44(reinterpret_cast<UnkKizunaSelf59D44*>(self));
            break;
        }
        self->field0C->callSlot14(0);
    }
}

// Retail walks a context-string list: separate condition-temp locals (the
// while-test copies cur/getContextStrPtr into fresh slots each pass) drive
// func_801276C8; the body splits off the next token and frees its pane.
extern "C" __declspec(noinline) void func_80259098(UnkKizunaSelf59B18* self) {
    if (self->field0C == 0) return;
    // declare ctx first: MWCC assigns it r31 (retail), res gets r30
    u8* ctx;
    UnkKizunaCtxPane59098* res =
        (UnkKizunaCtxPane59098*)((UnkKizunaLayoutSub57AFC*)self->field0C)
            ->field10->slot15(lbl_eu_8050CB20 + 0x9a, 1);
    ctx = (u8*)&res->ctx;
    const wchar_t* cur = getContextStr(ctx);
    // local declaration order drives MWCC stack-slot assignment (retail:
    // cur@0x14, tmp@0x10, condCur@0xc, condEnd@0x8)
    const wchar_t* tmp;
    const wchar_t* condCur;
    const wchar_t* condEnd;
    while (condEnd = (const wchar_t*)getContextStrPtr(ctx), condCur = cur,
           func_801276C8(&condCur, &condEnd)) {
        tmp = cur;
        func_801276E0(&tmp, 0);
        const wchar_t* s = func_80127670(&cur);
        nw4r::lyt::Pane* child = res->slot15((const char*)s + 0xbc, 1);
        ((nw4r::lyt::Pane*)res)->RemoveChild(child);
        // duplicated guard reproduces retail's paired beq (CSE'd load)
        if (child != 0) {
            if (child != 0) {
                ((UnkKizunaPaneFinalize*)child)->target2(1);
            }
        }
        cur = tmp;
    }
    nw4r::lyt::Pane* root =
        (nw4r::lyt::Pane*)((UnkKizunaLayoutSub57AFC*)self->field0C)
            ->field10->slot15(lbl_eu_8050CB20 + 0xa4, 1);
    root->RemoveChild((nw4r::lyt::Pane*)res);
    if (res != 0) {
        if (res != 0) {
            ((UnkKizunaPaneFinalize*)res)->target2(1);
        }
    }
    if (self->field0C != 0) {
        if (self->field0C != 0) {
            ((UnkKizunaPaneFinalize*)self->field0C)->target2(1);
        }
        self->field0C = 0;
    }
}

// Set an anim child state flag (1), publish it to the object at +0x0C's
// vtable slot 8, then write the shared "idle" float constant at +0x40.
void func_80259228(UnkKizunaSelf59228* self) {
    self->field14 = 1;
    self->field15 = 0;
    self->field0C->target8(self->field10);
    self->field40 = lbl_eu_80668828;
}

// Same shape as func_80259228 but with state flag 3 and a different idle float.
extern "C" __declspec(noinline) void func_80259280(UnkKizunaSelf59228* self) {
    self->field14 = 3;
    self->field15 = 0;
    self->field0C->target8(self->field10);
    self->field40 = lbl_eu_80668848;
}

// True when the anim's current frame has reached (its frame count - 1).
__declspec(noinline) bool func_802592D8(UnkKizunaSelf592D8* self) {
    return self->field10->GetFrame() >= (f32)(self->field10->GetFrameSize() - 1);
}

// Dispatch: walk +0x0C -> +0x10, call vtable slot 15 with (0x8B, 1), then
// return a 2-word struct built from the result's +0x44 / +0x48 words.
// extern "C" + noinline keeps callers (func_8025CAE4/CB50) emitting real
// unmangled bl branches (retail links func_80259344 unmangled) without
// inlining - the CKizunaTalkList pattern for same-TU callees.
extern "C" __declspec(noinline) UnkKizunaPair func_80259344(UnkKizunaSelf59344* self) {
    UnkKizunaRes59344* res = self->field0C->field10->target((int)(lbl_eu_8050CB20 + 0x8b), 1);
    UnkKizunaPair out = *(UnkKizunaPair*)((u8*)res + 0x44);
    return out;
}

// us-8025b6d8: walk the current kizuna line's tag-context list. For each item
// compute the doubled offset of its position from the two reference panes;
// while inside the near radius, snap the marker to the item (optionally
// selecting it via func_8025AA38 when well inside), play the confirm sound
// once, and return. After the list is exhausted, ease toward the registered
// pane position instead.
extern "C" __declspec(noinline) int func_8025949C(UnkKizunaSelf5949C* self) {
    self->field24 = 0;
    self->field26 = 0;
    // retail reloads field0C per access (no cached child pointer)
    UnkKizunaRes59344* linePane =
        self->field0C->field10->target((int)(lbl_eu_8050CB20 + 0xa4), 1);
    UnkKizunaRes59344* tagPane =
        self->field0C->field10->target((int)(lbl_eu_8050CB20 + 0x9a), 1);
    // the second pane embeds a tag processor at +0x10 driving the iteration
    u8* ctx = (u8*)&reinterpret_cast<UnkKizunaCtxPane59098*>(tagPane)->ctx;
    UnkKizunaVec3 linePos = linePane->pos;   // saved before the walk
    UnkKizunaVec3 tagPos = tagPane->pos;
    const wchar_t* cur = getContextStr(ctx);
    f32 c50 = lbl_eu_80668850;
    f32 nearThr = (c50 + c50) * (c50 + c50);
    // local declaration order drives MWCC stack-slot assignment (retail:
    // cur@0x10, condCur@0xc, condEnd@0x8)
    const wchar_t* condCur;
    const wchar_t* condEnd;
    while (condEnd = (const wchar_t*)getContextStrPtr(ctx), condCur = cur,
           func_801276C8(&condCur, &condEnd)) {
        UnkKizunaVec3 work = linePos;   // refreshed every pass
        const UnkKizunaCtxItem* item =
            reinterpret_cast<const UnkKizunaCtxItem*>(func_80127670(&cur));
        // retail materializes the item position copy (@0x6c) before the
        // delta temp (@0x48), then adds into work in place
        UnkKizunaVec3 itemPos = item->pos;
        UnkKizunaVec3 delta;
        func_80258F5C(&delta.x, &tagPos.x, &itemPos.x);
        func_80258F5C(&work.x, &work.x, &delta.x);
        // retail negates y before x
        f32 ny = -work.y;
        f32 nx = -work.x;
        f32 distSq = ny * ny + nx * nx;
        if (distSq <= nearThr) {
            UnkKizunaVec3 scaled;
            f32 snapA[2] = { lbl_eu_80668834, lbl_eu_80668834 };
            const UnkKizunaCtxItem* it2 =
                reinterpret_cast<const UnkKizunaCtxItem*>(func_80127670(&cur));
            func_80231848((UnkKizunaFunc31848Obj*)it2,
                          reinterpret_cast<const UnkKizunaPair*>(&snapA[0]));
            func_80258F80(&scaled.x, &work.x, lbl_eu_80668830);
            copyVEC3(&self->field18.x, &scaled.x);
            // threshold recomputed from the constant at the use site
            // (retail does not cache it across the loop)
            if (distSq <=
                (lbl_eu_80668854 + lbl_eu_80668854) *
                    (lbl_eu_80668854 + lbl_eu_80668854)) {
                const UnkKizunaCtxItem* it3 =
                    reinterpret_cast<const UnkKizunaCtxItem*>(func_80127670(&cur));
                // the item's name lives at +0xBC of the context item
                self->field26 =
                    func_8025AA38(self, reinterpret_cast<const char*>(it3) + 0xBC);
            }
            if (self->field3C == 0) {
                self->field3C = 1;
                func_80138078(0x7b);
            }
            return 1;
        } else {
            f32 far_[2] = { lbl_eu_8066884C, lbl_eu_8066884C };
            const UnkKizunaCtxItem* it4 =
                reinterpret_cast<const UnkKizunaCtxItem*>(func_80127670(&cur));
            func_80231848((UnkKizunaFunc31848Obj*)it4,
                          reinterpret_cast<const UnkKizunaPair*>(&far_[0]));
            func_801276E0(&cur, 0);
        }
    }
    // list exhausted: ease toward the pane's registered position.
    // retail copies linePos into its own slot (@0x60) before fetching
    // the registered pane, then adds the registered pos in place.
    UnkKizunaVec3 eased = linePos;
    UnkKizunaRes59344* reg1 = ((UnkKizunaObj59344*)linePane)->target(
        (int)(lbl_eu_8050CB20 + 0xe3), 1);
    UnkKizunaVec3 regPos = reg1->pos;
    func_80258F5C(&eased.x, &eased.x, &regPos.x);
    UnkKizunaRes59344* reg2 = ((UnkKizunaObj59344*)linePane)->target(
        (int)(lbl_eu_8050CB20 + 0xe3), 1);
    f32 sz[2];
    func_80127BC4(sz, reg2->scale);
    f32 rad = lbl_eu_8066884C * (sz[0] - lbl_eu_80668858);
    f32 distSq2 = -eased.y * -eased.y + -eased.x * -eased.x;
    if (distSq2 <= (rad + rad) * (rad + rad)) {
        UnkKizunaVec3 scaled;
        func_80258F80(&scaled.x, &eased.x, lbl_eu_80668830);
        copyVEC3(&self->field18.x, &scaled.x);
        u8 played = self->field3C;
        self->field24 = 1;
        if (played == 0) {
            self->field3C = 1;
            func_80138078(0x7b);
        }
        return 1;
    }
    self->field3C = 0;
    return 0;
}

// us-8025ba5c (0x80259820): refresh the kizuna entry panes for the selected
// id. Shows the shared line pane, then for every BDAT row whose display name
// matches the current selection's name (or its region-shifted +3 variant)
// with matching anchor position and unlocked progress flag, rewrites the
// row's five text tags from the per-state formatted strings.
// Same-TU callee defined below (retail links both unmangled).
extern "C" void func_80259B18(UnkKizunaSelf59B18* self);
extern "C" __declspec(noinline) void func_80259AF4(char* dest, const char* src);

__declspec(noinline) void func_80259820(UnkKizunaSelf9820* self) {
    func_80259B18(reinterpret_cast<UnkKizunaSelf59B18*>(self));
    if (self->field26 == 0) {
        return;
    }

    func_80124270(
        (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->field0C)->field10->target(
            (int)(lbl_eu_8050CB20 + 0x3f5), 1),
        1);

    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(lbl_eu_80664098);
    char* curName = func_8013639C(mgr, lbl_eu_8050CB20 + 0xca, self->field26);

    // 4-byte (3 chars + NUL) name copies compared as words
    char nameCur[4];
    func_80259AF4(nameCur, curName);

    void* fp = getFP__FPCc(lbl_eu_8050CB20 + 0x402);
    u16 count = func_8003B1EC(fp);

    // single base pointer for the shared arc string table (CSE of the above)
    char* str = &lbl_eu_8050CB20[0];

    char nameRow[4];
    char nameRowAlt[4];
    char fmtBuf[0x20];
    for (u16 i = 1; (u32)i <= (u32)count; i++) {
        char* rowName = func_8013639C(fp, str + 0x411, i);
        func_80259AF4(nameRow, rowName);
        func_80259AF4(nameRowAlt, rowName + 3);

        // word compare: current name equals the row name or its offset-3 form
        if (*(s32*)nameCur != *(s32*)nameRow && *(s32*)nameCur != *(s32*)nameRowAlt) {
            continue;
        }

        UnkKizunaRes59344* resA = self->field0C->field10->target((int)curName, 1);
        UnkKizunaRes59344* resB = self->field0C->field10->target((int)rowName, 1);
        if (resA == NULL || resB == NULL) {
            continue;
        }
        u8 state = (u8)func_8009CF8C(i + 0x608);
        if (!state) {
            continue;
        }

        // matching position selects the offset-0x417 tag variant
        UnkKizunaVec3 posA = resA->pos;
        char* tag = str + 0x421;
        UnkKizunaVec3 posB = resB->pos;
        bool samePos = posA.x == posB.x && posA.y == posB.y && posA.z == posB.z;
        if (samePos) {
            tag = str + 0x417;
        }
        char* tagName = func_8013639C(fp, tag, i);
        sprintf(fmtBuf, str + 0x42b, state);
        char* newName = func_8013639C(fp, fmtBuf, i);
        func_80136B4C((nw4r::lyt::Layout*)self->field0C, tagName, newName, 0);
        sprintf(fmtBuf, str + 0x435, tagName);
        func_80136B4C((nw4r::lyt::Layout*)self->field0C, fmtBuf, newName, 0);
        sprintf(fmtBuf, str + 0x439, tagName);
        func_80136B4C((nw4r::lyt::Layout*)self->field0C, fmtBuf, newName, 0);
        sprintf(fmtBuf, str + 0x43d, tagName);
        func_80136B4C((nw4r::lyt::Layout*)self->field0C, fmtBuf, newName, 0);
        sprintf(fmtBuf, str + 0x441, tagName);
        func_80136B4C((nw4r::lyt::Layout*)self->field0C, fmtBuf, newName, 0);
    }
}

extern "C" __declspec(noinline) void func_80259AF4(char* dest, const char* src) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; dest[3] = 0; }

// us-8025bd54 (0x8025BD54): if the shared arc layout's named root pane is
// visible, hide it and re-apply the 5 kizuna pane-name groups for indices
// 1..0x11 to the layout.
// noinline: retail callers (func_80259820) branch to it out-of-line.
extern "C" __declspec(noinline) void func_80259B18(UnkKizunaSelf59B18* self) {
    // retail buffer is 0x1c bytes; size drives MWCC frame allocation
    char buf[0x1C];
    nw4r::lyt::Pane* pane = (nw4r::lyt::Pane*)((UnkKizunaLayoutSub57AFC*)self->field0C)
                                ->field10->slot15(lbl_eu_8050CB20 + 0x3f5, 1);
    if (func_801C4648(pane) != 0) {
        func_80124270(pane, 0);
        for (u8 i = 1; i <= 0x12; i++) {
            sprintf(buf, lbl_eu_8050CB20 + 0x445, i);
            func_80136B4C(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x454, i);
            func_80136B4C(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x464, i);
            func_80136B4C(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x474, i);
            func_80136B4C(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x484, i);
            func_80136B4C(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
        }
    }
}

// Countdown advance: tick the +0x40 timer; when it reaches the limit switch to
// mode 2 and publish via the mid object's slots 7/11. Then scale the remaining
// time into the "line" pane result's byte at +0xB8 and mark it visible.
void func_80259C5C(UnkKizunaSelf59C5C* self) {
    self->field40 = self->field40 + lbl_eu_80668834;
    if (self->field40 >= lbl_eu_80668848) {
        self->field14 = 2;
        self->field15 = 1;
        self->field0C->slot7(self->field10);
        self->field0C->slot11(self->field10, 1);
        self->field40 = lbl_eu_80668848;
    }
    int cnt = (int)(lbl_eu_8066885C * (self->field40 / lbl_eu_80668848));
    UnkKizunaRes59344* res =
        self->field0C->field10->target((int)(lbl_eu_8050CB20 + 0x8b), 1);
    res->fieldB8 = (u8)cnt;
    func_80124270((nw4r::lyt::Pane*)res, 1);
}

// Place the kizuna marker: fetch the "line" pane result, copy its position
// and size, add the requested offset, clamp into the scaled pane bounds, and
// write the clamped position back.
extern "C" __declspec(noinline) void func_80259394(UnkKizunaSelf59394* self,
                                                   UnkKizunaVec3* arg) {
    UnkKizunaRes59344* res =
        self->field0C->field10->target((int)(lbl_eu_8050CB20 + 0xa4), 1);
    UnkKizunaVec3 pos = res->pos;      // word-copy of the animated position
    f32 size[2];
    func_80127BC4(size, res->scale);  // copy the pane's current size pair
    // Statement order is load-bearing for byte-identity (MWCC scheduling):
    // x offset, both C1-scaled bounds, y offset, then the two C2 bounds.
    // The `arg` parameter must stay non-const: const-qualified TBAA changes
    // the store/commit schedule and breaks the match. Residual vs retail is
    // pure float register coloring only (0 structural, no reloc drift).
    pos.x = pos.x + arg->x;
    f32 minX, minY, maxX, maxY;
    minX = lbl_eu_8066884C * size[0];
    minY = lbl_eu_8066884C * size[1];
    pos.y += arg->y;
    maxX = lbl_eu_80668830 * minX;
    maxY = lbl_eu_80668830 * minY;
    if (pos.x > minX) pos.x = minX;
    if (pos.x < maxX) pos.x = maxX;
    if (pos.y > minY) pos.y = minY;
    if (pos.y < maxY) pos.y = maxY;
    copyVEC3(&res->pos.x, &pos.x);
}


// Build the line layout from the shared arc string(+0x4C), then copy the
// animated position at +0x4C of the slot-15 result into the +0x14 Vec2.
// extern "C" + noinline keeps same-TU callers (CKizunagram::OnFileEvent)
// emitting retail's unmangled `bl` without inlining.
extern "C" __declspec(noinline) void func_80257AFC(UnkKizunaSelfAFC* self) {
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &self->field8, self->field4, lbl_eu_8050CB20);
    UnkKizunaLCBRes57AFC* res =
        ((UnkKizunaLayoutSub57AFC*)self->field8)->field10->slot15(lbl_eu_8050CB20 + 0x13, 1);
    copyVEC2(&self->field14[0], res->vec);
}

// us-8025c024: walk up to 16 kizuna BDAT rows starting at the shared row
// cursor. For each row not yet unlocked and not a duplicate of the current
// entry, build a textured Picture pane (0xf0-byte allocation from the layout
// heap), position/name it, attach it to the current-line parent pane, and
// tint it when appropriate. Returns true once the cursor passes the row limit.
__declspec(noinline) bool func_80259DE8(UnkKizunaSelf59DE8* self) {
    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(lbl_eu_80664098);
    u16 limit = (u16)(func_8003B1EC(mgr) - 0x64);
    // Unsigned 64-bit subtract; its high word gates the legacy tint below.
    u64 sub = 0x155;
    u64 chk = func_8009CF8C(0x20) - sub;
    const char* str = lbl_eu_8050CB20;
    f32 scale = lbl_eu_8066884C;
    f32 posZ = lbl_eu_80668828;
    u32 i = 0;
    const u32 feConst = 0xfe;
    const u32 texTag = 0x74696d67;
    const u32 tintColor = 0x777777ff;

    for (; i < 0x10; i++) {
        u16 cnt = lbl_eu_80664858;
        if (cnt >= limit) {
            return true;
        }
        u16 id = cnt + 0x65;
        u16 curId = (self->field3D == 0) ? (u16)func_8009ECE0() : 0;
        if (curId == id) {
            goto next;
        }
        {
            u32 tinted = 0;
            if ((u32)(chk >> 32) != 0 && id == 0x30a) {
                tinted = func_8009CF8C(0x54b) >= feConst;
            }
            char* unlockStr = func_80136254(mgr, str + 0xc1, id);
            if (func_8009CF8C((u32)(u16)(u32)unlockStr + 0xa20) == 0) {
                goto next;
            }
            char* nameStr = func_8013639C(mgr, str + 0xca, id);
            if (strcmp(nameStr, str + 0xd3) == 0) {
                goto next;
            }
            u32 texName =
                (u32)func_80138F78((u32)(u16)(u32)func_80136254(mgr, str + 0xd4, id));
            void* pal = self->field08->getTex(texTag, texName, 0);
            nw4r::lyt::Picture* pic = NULL;
            if (pal != 0) {
                void* raw =
                    allocate__Q23mtl10MemManagerFUlUl(0xf0, getAllocHandle__10CLibLayoutFv());
                if (raw != 0) {
                    nw4r::lyt::TexMap texMap(reinterpret_cast<TPLPalette*>(pal), 0);
                    pic = new (raw) nw4r::lyt::Picture(texMap);
                }
            }
            {
                f32 pairA[2] = { scale, scale };
                func_80231848(reinterpret_cast<UnkKizunaFunc31848Obj*>(pic),
                              reinterpret_cast<const UnkKizunaPair*>(&pairA[0]));
                char name[0x40];
                sprintf(name, str + 0xe0, nameStr);
                pic->SetName(name);
                s16 px = (s16)func_80136330(mgr, str + 0xaf, id);
                s16 py = (s16)func_80136330(mgr, str + 0xb8, id);
                UnkKizunaVec3 v;
                v.x = (f32)px;
                v.y = (f32)py;
                v.z = posZ;
                copyVEC3((f32*)&((KizunaPaneTranslateView*)pic)->translate, &v.x);
                func_80124270(reinterpret_cast<nw4r::lyt::Pane*>(pic), 1);
                nw4r::lyt::Pane* parent = reinterpret_cast<nw4r::lyt::Pane*>(
                    self->field0C->field10->target((int)(str + 0x9a), 1));
                parent->AppendChild(reinterpret_cast<nw4r::lyt::Pane*>(pic));
            }
            if (tinted != 0) {
                func_80137C1C(pic, tintColor);
            } else if (func_8009CF8C(0x20) >= 0x167) {
                int w = func_80138E1C(id);
                if ((u8)(w + 0xfe) <= 1) {
                    func_80137C1C(pic, tintColor);
                }
            }
        }
    next:
        lbl_eu_80664858 = lbl_eu_80664858 + 1;
    }
    return false;
}

// Update pass: while the +0x08 child is live, advance the anim at +0x0C unless
// the +0x10 byte suppresses it, then notify the child via vtable slot 14.
__declspec(noinline) void func_80257EE0(UnkKizunaSelf57EE0* self) {
    if (self->field8) {
        if (self->field10 == 0) {
            func_80137444(self->field0C, lbl_eu_80668834);
        }
        self->field8->callSlot14(0);
    }
}

extern "C" __declspec(noinline) void __ct__CKizunaCur(CKizunaCur* self,
                                 nw4r::lyt::ArcResourceAccessor* accessor) {
    self->mVtable = lbl_eu_805375FC;
    self->mArcResAcc = accessor;
    self->mField08 = 0;
    self->mField0C = 0;
    self->mField10 = 0;
    self->mField11 = 1;
    self->mField12 = 0;
}

// Selects the kizuna entry matching the given context-item name (the item's
// +0xBC string): walk all BDAT rows, compare each row's name against it, and
// return the first row id whose unlock progress flag (row value + 0xa20) is set.
extern "C" __declspec(noinline) u16 func_8025AA38(UnkKizunaSelf5949C* self,
                                                  const char* name) {
    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(lbl_eu_80664098);
    u16 count = (u16)func_8003B1EC(mgr);
    for (u16 id = 1; (u32)id <= count; id++) {
        if (strcmp(func_8013639C(mgr, lbl_eu_8050CB20 + 0xca, id),
                   name) == 0 &&
            func_8009CF8C((u32)(u16)(u32)func_80136254(mgr, lbl_eu_8050CB20 + 0xc1,
                                                       id) + 0xa20) != 0) {
            return id;
        }
    }
    return 0;
}

// retail: if (field26) { field34=1; field36=field26; field38=const }
extern "C" void func_8025AAE0(void* self) {
    u16 v = *(u16*)((char*)self + 0x26);
    if (v != 0) {
        *(u8*)((char*)self + 0x34) = 1;
        *(u16*)((char*)self + 0x36) = v;
        *(float*)((char*)self + 0x38) = lbl_eu_80668828;
    }
}

void func_8025AB04(UnkKizunaSelfAB* self) {
    f32 f = self->field38 + lbl_eu_80668834;
    self->field38 = f;
    if (f >= lbl_eu_80668860) {
        self->field34 = 2;
        self->field38 = lbl_eu_80668828;
        func_8025AC1C(self, 1);
    }
    if (self->field26 == 0) {
        self->field34 = 3;
        func_8025AC1C(self, 0);
    }
}

void func_8025AB84(UnkKizunaSelfAB* self) {
    f32 f = self->field38 + lbl_eu_80668834;
    self->field38 = f;
    if (f >= lbl_eu_80668864) {
        self->field34 = 1;
        self->field38 = lbl_eu_80668828;
        func_8025AC1C(self, 0);
    }
    if (self->field26 == 0) {
        self->field34 = 3;
        func_8025AC1C(self, 0);
    }
}

extern const float lbl_eu_80668828;
void CKizunagram_resetFields(u8* self){
    *(u8*)(self + 0x34) = 0;
    *(u16*)(self + 0x36) = 0;
    *(float*)(self + 0x38) = lbl_eu_80668828;
}

// us-8025ce58 (0x8025AC1C): rewrite the kizuna entry pane text for every BDAT
// row whose display name matches the current selection's name (or its
// region-shifted +3 variant), formatting the per-state string for the row's
// progress state and binding the 'timg' texture onto the resulting pane.
__declspec(noinline) void func_8025AC1C(UnkKizunaSelfAB* self, u32 a) {
    if (self->field36 == 0) {
        return;
    }
    if (a > 1) {
        return;
    }

    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(lbl_eu_80664098);
    char* curName = func_8013639C(mgr, &lbl_eu_8050CB20[0xca], self->field36);

    // 4-byte (3 chars + NUL) name copies compared as words
    char nameCur[4];
    func_8025B5D4(nameCur, curName);

    void* fp = getFP__FPCc(&lbl_eu_8050CB20[0x402]);
    u16 count = (u16)func_8003B1EC(fp);

    // shared arc string table base (CSE'd into a register across the loop)
    char* str = &lbl_eu_8050CB20[0];

    // fourcc of the texture tag ('timg'), hoisted out of the loop by MWCC
    const int timg = 0x74696d67;

    char nameRow[4];
    char nameRowAlt[4];
    char fmtBuf[0x20];
    for (u32 i = 1; (u16)i <= count; i++) {
        char* rowName = func_8013639C(fp, str + 0x411, i);
        func_8025B5D4(nameRow, rowName);
        func_8025B5D4(nameRowAlt, rowName + 3);

        // word compare: current name equals the row name or its offset-3 form
        if (*(s32*)nameCur != *(s32*)nameRow && *(s32*)nameCur != *(s32*)nameRowAlt) {
            continue;
        }

        nw4r::lyt::Pane* paneA =
            (nw4r::lyt::Pane*)self->field0C->field10->target((int)curName, 1);
        nw4r::lyt::Pane* paneB =
            (nw4r::lyt::Pane*)self->field0C->field10->target((int)rowName, 1);
        if (paneA == NULL || paneB == NULL) {
            continue;
        }

        u8 state = (u8)func_8009CF8C(i + 0x608);
        if (state != 0) {
            // Per-state label: pick the format string from the row's tag name.
            // (the tag lookup is repeated in every arm, matching retail)
            if (state == 1) {
                char* label = func_8013639C(fp, str + 0x417, i);
                if (strcmp(label, str + 0x494) == 0 || strcmp(label, str + 0x4ba) == 0) {
                    sprintf(fmtBuf, str + 0x7ef, a);
                } else if (strcmp(label, str + 0x4c7) == 0) {
                    sprintf(fmtBuf, str + 0x80a, a);
                } else if (strcmp(label, str + 0x4ed) == 0 || strcmp(label, str + 0x513) == 0) {
                    sprintf(fmtBuf, str + 0x825, a);
                } else if (strcmp(label, str + 0x3f5) == 0) {
                    sprintf(fmtBuf, str + 0x840, a);
                } else if (strcmp(label, str + 0x539) == 0 || strcmp(label, str + 0x55f) == 0) {
                    sprintf(fmtBuf, str + 0x85b, a);
                } else if (strcmp(label, str + 0x56c) == 0) {
                    sprintf(fmtBuf, str + 0x876, a);
                }
            } else if (state == 2) {
                char* label = func_8013639C(fp, str + 0x417, i);
                if (strcmp(label, str + 0x494) == 0 || strcmp(label, str + 0x4ba) == 0) {
                    sprintf(fmtBuf, str + 0x891, a);
                } else if (strcmp(label, str + 0x4c7) == 0) {
                    sprintf(fmtBuf, str + 0x8ac, a);
                } else if (strcmp(label, str + 0x4ed) == 0 || strcmp(label, str + 0x513) == 0) {
                    sprintf(fmtBuf, str + 0x8c7, a);
                } else if (strcmp(label, str + 0x3f5) == 0) {
                    sprintf(fmtBuf, str + 0x8e2, a);
                } else if (strcmp(label, str + 0x539) == 0 || strcmp(label, str + 0x55f) == 0) {
                    sprintf(fmtBuf, str + 0x8fd, a);
                } else if (strcmp(label, str + 0x56c) == 0) {
                    sprintf(fmtBuf, str + 0x918, a);
                }
            } else if (state == 3) {
                char* label = func_8013639C(fp, str + 0x417, i);
                if (strcmp(label, str + 0x494) == 0 || strcmp(label, str + 0x4ba) == 0) {
                    sprintf(fmtBuf, str + 0x933, a);
                } else if (strcmp(label, str + 0x4c7) == 0) {
                    sprintf(fmtBuf, str + 0x94e, a);
                } else if (strcmp(label, str + 0x4ed) == 0 || strcmp(label, str + 0x513) == 0) {
                    sprintf(fmtBuf, str + 0x969, a);
                } else if (strcmp(label, str + 0x3f5) == 0) {
                    sprintf(fmtBuf, str + 0x984, a);
                } else if (strcmp(label, str + 0x539) == 0 || strcmp(label, str + 0x55f) == 0) {
                    sprintf(fmtBuf, str + 0x99f, a);
                } else if (strcmp(label, str + 0x56c) == 0) {
                    sprintf(fmtBuf, str + 0x9ba, a);
                }
            } else if (state == 4) {
                char* label = func_8013639C(fp, str + 0x417, i);
                if (strcmp(label, str + 0x494) == 0 || strcmp(label, str + 0x4ba) == 0) {
                    sprintf(fmtBuf, str + 0x9d5, a);
                } else if (strcmp(label, str + 0x4c7) == 0) {
                    sprintf(fmtBuf, str + 0x9f0, a);
                } else if (strcmp(label, str + 0x4ed) == 0 || strcmp(label, str + 0x513) == 0) {
                    sprintf(fmtBuf, str + 0xa0b, a);
                } else if (strcmp(label, str + 0x3f5) == 0) {
                    sprintf(fmtBuf, str + 0xa26, a);
                } else if (strcmp(label, str + 0x539) == 0 || strcmp(label, str + 0x55f) == 0) {
                    sprintf(fmtBuf, str + 0xa41, a);
                } else if (strcmp(label, str + 0x56c) == 0) {
                    sprintf(fmtBuf, str + 0xa5c, a);
                }
            } else if (state == 5) {
                char* label = func_8013639C(fp, str + 0x417, i);
                if (strcmp(label, str + 0x494) == 0 || strcmp(label, str + 0x4ba) == 0) {
                    sprintf(fmtBuf, str + 0xa77, a);
                } else if (strcmp(label, str + 0x4c7) == 0) {
                    sprintf(fmtBuf, str + 0xa92, a);
                } else if (strcmp(label, str + 0x4ed) == 0 || strcmp(label, str + 0x513) == 0) {
                    sprintf(fmtBuf, str + 0xaad, a);
                } else if (strcmp(label, str + 0x3f5) == 0) {
                    sprintf(fmtBuf, str + 0xac8, a);
                } else if (strcmp(label, str + 0x539) == 0 || strcmp(label, str + 0x55f) == 0) {
                    sprintf(fmtBuf, str + 0xae3, a);
                } else if (strcmp(label, str + 0x56c) == 0) {
                    sprintf(fmtBuf, str + 0xafe, a);
                }
            }

            // Bind the shared 'timg' texture resource onto the refreshed pane.
            void* tex = self->field04->getTex(timg, fmtBuf, 0);
            if (tex != NULL) {
                func_80137F88(paneB, (u32)tex);
            }
        }
    }
}

void CKizunagram_copyString(unsigned char* dst, const unsigned char* src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = 0;
}

extern "C" __declspec(noinline) void __ct__CKizunaInfo(CKizunaInfo* self,
                                  nw4r::lyt::ArcResourceAccessor* accessor) {
    self->mArcResAcc = accessor;
    self->mVtable = lbl_eu_805375E4;
    self->mField08 = 0;
    self->mField0C = 0;
    self->mField10 = 0;
    self->mField14 = 0;
    self->mField15 = 1;
    self->mField16 = 1;
    self->mField18 = 0;
}

CKizunaInfo::~CKizunaInfo() {}

// us-8025d8ac (0x8025B670): build the kizuna line layout + two anim
// transforms, bind the font to the root pane, publish the 4 font-format
// values and 6 localized labels, then reset the +0x18 counter.
// extern "C" + noinline for OnFileEvent's unmangled call shape (see func_80257AFC).
extern "C" __declspec(noinline) void func_8025B670(UnkKizunaSelfB670* self) {
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &self->field8, self->field4, lbl_eu_8050CB20 + 0xb19);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->field8, &self->field0C, self->field4, lbl_eu_8050CB20 + 0xb2c);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->field8, &self->field10, self->field4, lbl_eu_8050CB20 + 0xb42);
    nw4r::lyt::Pane* rootPane = ((UnkKizunaLayoutView*)self->field8)->field10;
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->field8);
    u32 fontResult = static_cast<UnkKizunaFontView*>(fontObj)->vf7();
    func_8013676C(rootPane, fontResult);
    u32 val = func_801355BC();
    func_801368C0(self->field8, lbl_eu_8050CB20 + 0xb5d, val);
    func_801368C0(self->field8, lbl_eu_8050CB20 + 0xb6b, val);
    func_801368C0(self->field8, lbl_eu_8050CB20 + 0xb79, val);
    func_801368C0(self->field8, lbl_eu_8050CB20 + 0xb87, val);
    self->field8->UnbindAllAnimation();
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xb95,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xb), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xb9f,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xc), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xba9,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xd), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbb3,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xe), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbbd,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xf), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbc7,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x10), 0);
    self->field18 = 0xFFFF;
    func_8025BA38((UnkKizunaSelf57D90*)self, 0);
}

// Per-frame display dispatch: while the current-line child (+0x08) is live,
// run the mode-specific handler for the +0x14 mode byte, then notify the
// child via vtable slot 14 (retail: cmpwi chain over modes 1/2/4/5).
extern "C" __declspec(noinline) void func_8025B870(UnkKizunaSelfC21C* self) {
    if (self->field8) {
        switch (self->field14) {
        case 1:
            func_8025C16C(self);
            break;
        case 2:
            func_8025C21C(self);
            break;
        case 4:
            func_8025C298(self);
            break;
        case 5:
            func_8025C348(self);
            break;
        }
        self->field8->target14(0);
    }
}

// Finalize then release/null a +0x08 child: if non-null, finalize it (vtable
// slot 2) with a 1 flag, then clear the pointer.
// The nested identical guards reproduce retail's duplicated test: both beqs
// share the single loaded pointer (MWCC CSEs the load).
extern "C" __declspec(noinline) void func_8025B900(UnkKizunaSelf57D90* self) {
    if (self->field8 != 0) {
        if (self->field8 != 0) {
            self->field8->target2(1);
        }
        self->field8 = 0;
    }
}

// Set display mode 1 for the current-line child: publish the frame value via
// vtable slots 7/11 of the child at +0x08, and clear the animation flags.
extern "C" __declspec(noinline) void func_8025B958(UnkKizunaSelfB958* self) {
    self->field14 = 1;
    self->field15 = 0;
    self->field16 = 0;
    self->field8->slot7(self->field0C);
    self->field8->slot11(self->field0C, 1);
}

// Same shape as func_8025B958 but display mode 4 and value from +0x10.
extern "C" __declspec(noinline) void func_8025B9C8(UnkKizunaSelfB958* self) {
    self->field14 = 4;
    self->field15 = 0;
    self->field16 = 0;
    self->field8->slot7(self->field10);
    self->field8->slot11(self->field10, 1);
}

// Kizuna-line display refresh for a selected kizuna id. Guards on the cached
// id at +0x18, then either resets all text panes to blanks/zero (v == 0) or
// rebinds every pane from the BDAT progress tables (name/color/heart counts).
extern "C" __declspec(noinline) void func_8025BA38(UnkKizunaSelf57D90* selfArg,
                                                    u16 v) {
    // Callers hand us embedded sub-objects typed as UnkKizunaSelf57D90; the
    // real layout also carries the cached-id u16 at +0x18.
    UnkKizunaSelfBA38* self = (UnkKizunaSelfBA38*)selfArg;
    if (self->field18 == v) {
        return;
    }
    self->field18 = v;
    if (v == 0) {
        // Reset path: blank all ten text panes with the shared empty string.
        func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbd1, lbl_eu_8050CB20 + 0xd3, 0);
        func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbdc, lbl_eu_8050CB20 + 0xd3, 0);
        func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbe7, lbl_eu_8050CB20 + 0xd3, 0);
        func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbf2, lbl_eu_8050CB20 + 0xd3, 0);
        func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xb87, lbl_eu_8050CB20 + 0xd3, 0);
        func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbfd, lbl_eu_8050CB20 + 0xd3, 0);
        func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xc0b, lbl_eu_8050CB20 + 0xd3, 0);
        func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xb5d, lbl_eu_8050CB20 + 0xd3, 0);
        func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xb79, lbl_eu_8050CB20 + 0xd3, 0);
        func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xb6b, lbl_eu_8050CB20 + 0xd3, 0);
        // Hide the root pane fetched through slot 15 of the layout child.
        func_80124270(reinterpret_cast<nw4r::lyt::Pane*>(
                          reinterpret_cast<UnkKizunaLayoutSub57AFC*>(self->field8)
                              ->field10->slot15(lbl_eu_8050CB20 + 0xc16, 1)),
                      0);
        return;
    }

    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(lbl_eu_80664098);

    // Progress-column color selector: look up the row value then map it to a
    // color index; the low byte picks which timg texture tag to bind.
    char* progRow = func_80136254(mgr, lbl_eu_8050CB20 + 0x2fb, v);
    u8 colIdx = func_8013600C(lbl_eu_8050CB20 + 0x303, lbl_eu_8050CB20 + 0x310,
                              (u16)(u32)progRow);
    char* texName = NULL;
    switch (func_801372B4(colIdx)) {
    case 1:
        texName = ((UnkKizunaAccBA38*)func_801355F4())
                      ->getTex(0x74696d67, lbl_eu_8050CB20 + 0xc22, 0);
        break;
    case 2:
        texName = ((UnkKizunaAccBA38*)func_801355F4())
                      ->getTex(0x74696d67, lbl_eu_8050CB20 + 0xc3d, 0);
        break;
    case 3:
        texName = ((UnkKizunaAccBA38*)func_801355F4())
                      ->getTex(0x74696d67, lbl_eu_8050CB20 + 0xc58, 0);
        break;
    case 4:
        texName = ((UnkKizunaAccBA38*)func_801355F4())
                      ->getTex(0x74696d67, lbl_eu_8050CB20 + 0xc73, 0);
        break;
    case 5:
        texName = ((UnkKizunaAccBA38*)func_801355F4())
                      ->getTex(0x74696d67, lbl_eu_8050CB20 + 0xc8e, 0);
        break;
    }
    if (texName != NULL) {
        // Bind the color texture and show the tinted root pane.
        func_80137E7C(self->field8, lbl_eu_8050CB20 + 0xc16, texName);
        func_80124270(
            reinterpret_cast<nw4r::lyt::Pane*>(
                reinterpret_cast<UnkKizunaLayoutSub57AFC*>(self->field8)
                    ->field10->slot15(lbl_eu_8050CB20 + 0xc16, 1)),
            1);
    }

    // Progress color name for this save column and the kizuna's own color.
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbd1,
                  func_8013639C(lbl_eu_806640A8, lbl_eu_8050CB20 + 0x321, colIdx), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbdc,
                  func_8013639C(mgr, lbl_eu_8050CB20 + 0x321, v), 0);

    // Bond-stage label: stage indexes a per-language label table; stages above
    // 10 clamp to the first label.
    char* stageLabel;
    switch ((u8)func_80138E1C(v)) {
    case 0:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x16);
        break;
    case 1:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x17);
        break;
    case 2:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x18);
        break;
    case 3:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x19);
        break;
    case 4:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x1a);
        break;
    case 5:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x1b);
        break;
    case 6:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x1c);
        break;
    case 7:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x1d);
        break;
    case 8:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x1e);
        break;
    case 9:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x18);
        break;
    case 10:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x19);
        break;
    default:
        stageLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x16);
        break;
    }
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbe7, stageLabel, 0);

    // Heart-level icon name from the heart-state table (0 hides the icon).
    char* heartLabel;
    switch ((u8)func_80138E90(v)) {
    case 0:
        heartLabel = NULL;
        break;
    case 1:
        heartLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x20);
        break;
    case 2:
        heartLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x21);
        break;
    case 3:
        heartLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x22);
        break;
    case 4:
        heartLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x23);
        break;
    case 5:
        heartLabel = func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x24);
        break;
    }
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbf2, heartLabel, 0);

    // Encounter counter, both names, formatted "current/max" heart counts.
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(
        self->field8, lbl_eu_8050CB20 + 0xb87,
        (u8)func_80136254(mgr, lbl_eu_8050CB20 + 0xca9, v));
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xbfd,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x15), 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xc0b,
                  func_8013639C(mgr, lbl_eu_8050CB20 + 0xcb1, v), 0);
    u8 heartsCur = func_801361E8((u32)mgr, lbl_eu_8050CB20 + 0xcc9, v);
    u8 heartsMax = func_801361E8((u32)mgr, lbl_eu_8050CB20 + 0xcc1, v);
    char buf[0x14];
    sprintf(buf, lbl_eu_8050CB20 + 0xcc9, heartsCur);
    func_80136A1C(self->field8, lbl_eu_8050CB20 + 0xb5d, buf, 0);
    sprintf(buf, lbl_eu_8050CB20 + 0xcc9, heartsMax);
    func_80136A1C(self->field8, lbl_eu_8050CB20 + 0xb79, buf, 0);
    func_80136B4C(self->field8, lbl_eu_8050CB20 + 0xb6b,
                  func_80136190(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x14), 0);
}

// Mode-2 update: when the +0x0C anim transform reaches its last frame,
// switch the mode byte to 2, set the +0x16 flag, and publish state through
// the child at +0x08 (slots 14/8 with the transform id, slots 7/11 with
// the +0x10 transform and a 1 flag).
extern "C" __declspec(noinline) void func_8025C16C(UnkKizunaSelfC21C* self) {
    if (func_80137444((nw4r::lyt::AnimTransform*)self->field0C, lbl_eu_80668834) != 0) {
        self->field14 = 2;
        self->field16 = 1;
        self->field8->target14(0);
        self->field8->target8(self->field0C);
        self->field8->slot7((u32)self->field10);
        self->field8->slot11((u32)self->field10, 1);
    }
}

extern "C" __declspec(noinline) void func_8025C21C(UnkKizunaSelfC21C* self) {
    if (func_80137444(self->field10, lbl_eu_80668834) != 0) {
        // anim reached its last frame: switch to mode 3, publish via the child.
        self->field14 = 3;
        self->field15 = 1;
        self->field8->target14(0);
        self->field8->target8((u32)self->field10);
    }
}

// Mode-4 update: same shape as func_8025C16C but mode byte 5 and the
// +0x10 transform published through slots 8/7/11.
extern "C" __declspec(noinline) void func_8025C298(UnkKizunaSelfC21C* self) {
    if (func_80137510(self->field10, lbl_eu_80668834) != 0) {
        // anim reached its last frame: switch to mode 5, publish via the child.
        self->field14 = 5;
        self->field16 = 1;
        self->field8->target14(0);
        self->field8->target8((u32)self->field10);
        self->field8->slot7(self->field0C);
        self->field8->slot11(self->field0C, 1);
    }
}

// func_8025C348 (us-8025e494): kizuna-line update gate. Retail: reads the
// global float lbl_eu_80668834 (sdata), calls func_80137510([self+0xC], f1)
// (declared CArtsInfo.hpp, defined code_80135FDC.cpp — the old comment's
// "no split symbols" blocker is stale); on non-zero return: field14=0,
// field15=1, then [self+8] virtual slot 56 (target14) twice with arg 0,
// then slot 32 (target8) with arg [self+0xC].
extern "C" __declspec(noinline) void func_8025C348(UnkKizunaSelfC21C* self) {
    if (func_80137510((nw4r::lyt::AnimTransform*)self->field0C, lbl_eu_80668834) != 0) {
        self->field14 = 0;
        self->field15 = 1;
        self->field8->target14(0);
        self->field8->target14(0);
        self->field8->target8(self->field0C);
    }
}

// Retail __ct__CKizunagram (unmangled): manual vtable label, both memory
// regions, the scalar state block, then the four embedded sub-objects and the
// trailing flag bytes (the ctor argument lands at +0xDE).
extern "C" CKizunagram* __ct__CKizunagram(CKizunagram* self, int arg) {
    *(void**)self = (void*)lbl_eu_80537550;
    __ct__17UnkClass_8045F564Fv(&self->mMemRegionA);
    __ct__17UnkClass_8045F564Fv(&self->mMemRegionB);
    f32 idle = lbl_eu_80668828;
    self->field28 = 0;
    self->field2C = 0;
    self->field30 = 0;
    self->field34 = 0;
    self->field38 = 0;
    self->field39 = 0;
    self->field3A = 0;
    self->field3B = 0;
    self->field3C = 1;
    self->field40 = idle;
    self->field44 = idle;
    self->field48 = idle;
    __ct__CKizunaInfo(&self->mInfo, 0);
    __ct__CKizunaLine(&self->mLine, 0, 0, 0);
    __ct__CKizunaCur(&self->mCur, 0);
    __ct__CKizunaRadar(&self->mRadar, 0);
    self->fieldDC = 0;
    self->fieldDD = 1;
    self->fieldDE = (u8)arg;
    return self;
}

CKizunagram::~CKizunagram() {}

// Load both bind files into the +0x28 / +0x2C file handles (self is the
// IWorkEvent receiver), flagging each handle after it is read. Locals let MWCC
// reuse the readFile return register for the flag calls (no reload).
void func_8025C510(UnkKizunaSelfC510* self) {
    CFileHandle* h2 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                            lbl_eu_8050CB20 + 0xcd1,
                                            (IWorkEvent*)self, 0, 0);
    self->field2C = h2;
    CDeviceFile::setHandleFlag2(h2);
    CFileHandle* h1 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                            lbl_eu_8050CB20 + 0xcf0,
                                            (IWorkEvent*)self, 0, 0);
    self->field28 = h1;
    CDeviceFile::setHandleFlag2(h1);
}


// Draw pass: render the shared layout, then (when enabled and in modes 3/6)
// the three conditional sub-layouts.
void func_8025C61C(UnkKizunaSelfC61C* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field74 != 0) {
        func_80137038(self->field74, drawInfo, 0, 1);
    }
    if (self->field38 != 0) {
        if (self->field39 == 3 || self->field39 == 6) {
            if (self->fieldDD != 0 && self->field8E != 0 && self->field54 != 0) {
                func_80137038(self->field54, drawInfo, 0, 1);
            }
            if (self->fieldB4 != 0) {
                func_80137038(self->fieldB4, drawInfo, 0, 1);
            }
            if (self->fieldC8 != 0) {
                func_80137038(self->fieldC8, drawInfo, 0, 1);
            }
        }
    }
}

// Destruction: release both file handles, clear the anim flag byte, then
// release each child object (0x4C / 0x68 / 0xAC / 0xC0) and the two resource
// accessors, and tear down the two memory regions at +0x08 / +0x18.
void func_8025C6F0(UnkKizunaSelfC6F0* self) {
    func_801390E0(&self->field28);
    func_801390E0(&self->field2C);
    self->field38 = 0;
    func_8025B900(&self->sub4C);
    func_80259098((UnkKizunaSelf59B18*)&self->sub68);
    func_80257F44(&self->subAC);
    func_80257D90(&self->subC0);
    func_80139124(self->field30);
    func_80139124(self->field34);
    self->mRegA.func_8045F778();
    self->mRegB.func_8045F778();
}

unsigned char CKizunagram_getField7E(u8* this_ptr) {
    if (*(unsigned char*)(this_ptr + 0x7e) != 0) {
        return *(unsigned char*)(this_ptr + 0x3b);
    } else {
        return 0;
    }
}

unsigned char CKizunagram_checkFields(u8* arg1)
{
    if (*(unsigned char*)(arg1 + 0x61) == 0)
        return 0;
    if (*(unsigned char*)(arg1 + 0x7d) == 0)
        return 0;
    if (*(unsigned char*)(arg1 + 0xbd) != 0)
        return *(unsigned char*)(arg1 + 0x3c);
    return 0;
}

extern "C" void func_8025C7D0(UnkKizunaSelfC7FC* self) {
    if (self->field39 != 0) return;
    self->field39 = 1;
    self->field3C = 0;
    ((u8*)self)[0x38] = 1;
    func_8025B958(&self->sub4C);
}

void func_8025C7FC(UnkKizunaSelfC7FC* self, int arg4) {
    if (self->field39 == 3) {
        // switch to mode 4, reset the sub-flag, re-publish all children.
        self->field39 = 4;
        self->field3C = 0;
        func_8025B9C8(&self->sub4C);
        func_80259280(&self->sub68);
        func_80257F9C(&self->subAC, 0xff);
        if (arg4 != 0) {
            func_80138078(6);
        }
    }
}


// Build the line layout from the shared arc string at +0x4C, then copy the
// animated position at +0x4C of the slot-15 result into the +0x14 Vec2.
// Each display reset: place a fixed color into the +0x68 sub-object, clear
// its state byte, reset the two line panes, and step the sub-anim.
extern "C" __declspec(noinline) void func_8025C874(UnkKizunaSelfC874* self) {
    // Array member forces a block copy on assignment (retail's interleaved
    // lwz/stw shape); last-declared tmp sits at sp+8, v spans sp+0x14+.
    struct VecA { u32 a[3]; };
    VecA v;
    VecA tmp;
    VecA* src = reinterpret_cast<VecA*>(code80135FDC_setVec3(
        reinterpret_cast<float*>(&tmp.a[0]), lbl_eu_80668828,
        lbl_eu_80668868, lbl_eu_80668828));
    v = *src;
    func_80259394(reinterpret_cast<UnkKizunaSelf59394*>(&self->sub68),
                  reinterpret_cast<UnkKizunaVec3*>(&v));
    func_8025949C(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68));
    self->field3A = 0;
    func_80257F9C(&self->subAC, 0);
    func_8025BA38(&self->sub4C, self->field8E);
    func_80259820(reinterpret_cast<UnkKizunaSelf9820*>(&self->sub68));
}

// Same display reset as func_8025C874 but with the second color constant.
extern "C" __declspec(noinline) void func_8025C904(UnkKizunaSelfC874* self) {
    // Array member forces a block copy on assignment (retail's interleaved
    // lwz/stw shape); last-declared tmp sits at sp+8, v spans sp+0x14+.
    struct VecA { u32 a[3]; };
    VecA v;
    VecA tmp;
    VecA* src = reinterpret_cast<VecA*>(code80135FDC_setVec3(
        reinterpret_cast<float*>(&tmp.a[0]), lbl_eu_80668828,
        lbl_eu_8066886C, lbl_eu_80668828));
    v = *src;
    func_80259394(reinterpret_cast<UnkKizunaSelf59394*>(&self->sub68),
                  reinterpret_cast<UnkKizunaVec3*>(&v));
    func_8025949C(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68));
    self->field3A = 0;
    func_80257F9C(&self->subAC, 0);
    func_8025BA38(&self->sub4C, self->field8E);
    func_80259820(reinterpret_cast<UnkKizunaSelf9820*>(&self->sub68));
}

// Same display reset as func_8025C874 but with the color constants swapped
// (retail loads f2 first then fmr f3, so the source order is x/y/z with the
// shared constant in y/z).
extern "C" __declspec(noinline) void func_8025C994(UnkKizunaSelfC874* self) {
    // u32-array view forces retail's word-copy shape; declaring v first puts
    // it at sp+0x14 and the setVec3 temp at sp+8, matching retail's layout.
    struct VecW { u32 w[3]; };
    VecW v;
    VecW tmp;
    v = *reinterpret_cast<VecW*>(code80135FDC_setVec3(
        reinterpret_cast<f32*>(&tmp.w[0]), lbl_eu_8066886C,
        lbl_eu_80668828, lbl_eu_80668828));
    func_80259394(reinterpret_cast<UnkKizunaSelf59394*>(&self->sub68),
                  reinterpret_cast<UnkKizunaVec3*>(&v));
    func_8025949C(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68));
    self->field3A = 0;
    func_80257F9C(&self->subAC, 0);
    func_8025BA38(&self->sub4C, self->field8E);
    func_80259820(reinterpret_cast<UnkKizunaSelf9820*>(&self->sub68));
}

void func_8025CA24(){}

// Dispatch on display-state byte at +0x3A; each case is a tail call.
// Same display reset as func_8025C874 but with the color constants swapped
// (retail loads f2 first then fmr f3, so the source order is x/y/z with the
// shared constant in y/z).
extern "C" __declspec(noinline) void func_8025CA24(UnkKizunaSelfC874* self) {
    // u32-array view forces retail's word-copy shape (lwz/stw, not lfs/stfs);
    // declaring v first puts it at sp+0x14 and the setVec3 temp at sp+8
    // (matches retail's slot assignment; see func_8025C994)
    struct VecW { u32 w[3]; };
    VecW v;
    VecW tmp;
    v = *reinterpret_cast<VecW*>(code80135FDC_setVec3(
        reinterpret_cast<f32*>(&tmp.w[0]), lbl_eu_80668868,
        lbl_eu_80668828, lbl_eu_80668828));
    func_80259394(reinterpret_cast<UnkKizunaSelf59394*>(&self->sub68),
                  reinterpret_cast<UnkKizunaVec3*>(&v));
    func_8025949C(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68));
    self->field3A = 0;
    func_80257F9C(&self->subAC, 0);
    func_8025BA38(&self->sub4C, self->field8E);
    func_80259820(reinterpret_cast<UnkKizunaSelf9820*>(&self->sub68));
}

void func_8025CAB4(UnkKizunaDisp* self) {
    switch (self->field_0x3A) {
    case 0:
        return func_8025CE00((UnkKizunaSelfCE00*)self);
    case 1:
        return func_8025CE78((UnkKizunaSelfCE78*)self);
    case 2:
        return func_8025CF1C(self);
    }
}

// Pulse the anim at +0x78 to frame 1.0, notify the child object (+0x74, via
// the embedded func_80259344 sub at +0x68) on vtable slot 14, rebuild the
// two-word pair, and copy it into the +0xB4 object's child.
void func_8025CAE4(UnkKizunaSelfCAE4* self) {
    func_80137444(self->field78, lbl_eu_80668834);
    ((UnkKizunaObjSlot14*)self->sub.field0C)->callSlot14(0);
    UnkKizunaPair p = func_80259344(&self->sub);
    func_80231848(self->fieldB4->field10, &p);
}

// Same shape as func_8025CAE4 but the anim step uses func_80137510 instead.
void func_8025CB50(UnkKizunaSelfCAE4* self) {
    func_80137510(self->field78, lbl_eu_80668834);
    ((UnkKizunaObjSlot14*)self->sub.field0C)->callSlot14(0);
    UnkKizunaPair p = func_80259344(&self->sub);
    func_80231848(self->fieldB4->field10, &p);
}

int CKizunagram_stub(void* self) { return 0; }

struct CKizunagramState {
    u8 _00[0x8C];
    u8 field8C;
};

extern "C" u8 func_8025CBC4(CKizunagramState* self) {
    return self->field8C;
}


// retail: lfs f0,const; li r0,0; stb 0x34; sth 0x36; stfs 0x38
extern "C" void func_8025AC04(void* self) {
    *(u8*)((char*)self + 0x34) = 0;
    *(u16*)((char*)self + 0x36) = 0;
    *(float*)((char*)self + 0x38) = lbl_eu_80668828;
}

// retail: lbz 0xdd; xori; subic; subfe; stb - toggle (x^1)!=0 under -O4,s
extern "C" void func_8025CC70(void* self) {
    *(u8*)((char*)self + 0xDD) = (u8)((*(u8*)((char*)self + 0xDD) ^ 1) != 0);
}

// retail: if (field_62) { field_39 = 2; tail func_80259228(self+0x68) }
// noinline: retail keeps these dispatch handlers out-of-line; without the
// attribute MWCC folds their bodies into func_8025C580's switch arms.
extern "C" __declspec(noinline) void func_8025CC88(void* self) {
    if (*(u8*)((char*)self + 0x62) != 0) {
        *(u8*)((char*)self + 0x39) = 2;
        func_80259228((UnkKizunaSelf59228*)((char*)self + 0x68));
    }
}

// Gate: when both the +0x61 and +0x7D bytes are set, raise the +0x39/+0x3C
// state and tail-call the +0xAC sub-object with the +0x8C flag.
extern "C" __declspec(noinline) void func_8025CCA8(void* self) {
    if (*(u8*)((u8*)self + 0x61) == 0)
        return;
    if (*(u8*)((u8*)self + 0x7D) == 0)
        return;
    *((u8*)self + 0x39) = 3;
    *((u8*)self + 0x3C) = 1;
    if (*(u8*)((u8*)self + 0x8C) != 0)
        func_80257F9C((UnkKizunaSelf57D90*)((u8*)self + 0xAC), 1);
    else
        func_80257F9C((UnkKizunaSelf57D90*)((u8*)self + 0xAC), 0);
}

void CKizunagram_setField39(u8* ptr) {
    if (ptr[0x62] != 0) {
        ptr[0x39] = 5;
    }
}

void CKizunagram_resetState(u8* b) {
    if (!b[0x61]) return;
    if (!b[0x7d]) return;
    b[0x39] = 0;
    b[0x3c] = 1;
    b[0x38] = 0;
}

__declspec(noinline) void func_8025CD40(void* self) {}

// If the sub-state at +0x68 reports active, scale the stored int bit patterns
// at +0x80..0x88 down by the constant and publish the results. The u32 words
// are routed through a stack array so MWCC emits the retail lwz/stw/lfs
// bit-cast round-trips for +0x88 without consuming its result.
extern "C" __declspec(noinline) void func_8025CE00(UnkKizunaSelfCE00* self) {
    if (func_8025949C(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68)) != 0) {
        u32 raw[3];
        raw[0] = self->field80;
        raw[1] = self->field84;
        f32 denom = lbl_eu_80668870;
        // retail computes the +0x44 division before the dead +0x88 store
        f32 v44 = *(f32*)&raw[1] / denom;
        raw[2] = self->field88;   // dead store (never read) - retail keeps it
        self->field48 = denom;
        self->field3A = 1;
        self->field44 = v44;
        self->field40 = *(f32*)&raw[0] / denom;
    }
}


// Move the line target down by the +0x48 step; once it reaches the floor,
// latch state 2 and refresh the info/line displays.
extern "C" __declspec(noinline) void func_8025CE78(UnkKizunaSelfCE78* self) {
    // u32 words force retail's lwz/stw struct copy of the setVec3 result
    // (see func_8025CA24); v lands at sp+0x14 and the setVec3 temp at sp+8.
    struct VecW { u32 w[3]; };
    VecW v;
    VecW tmp;
    v = *reinterpret_cast<VecW*>(code80135FDC_setVec3(
        reinterpret_cast<f32*>(&tmp.w[0]), self->field40, self->field44,
        lbl_eu_80668828));
    func_80259394(reinterpret_cast<UnkKizunaSelf59394*>(&self->sub68),
                  reinterpret_cast<UnkKizunaVec3*>(&v));
    f32 f = self->field48 - lbl_eu_80668834;
    self->field48 = f;
    if (f <= lbl_eu_80668828) {
        self->field3A = 2;
        func_8025949C(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68));
        func_8025BA38(&self->sub4C, self->field8E);
        func_80259820(reinterpret_cast<UnkKizunaSelf9820*>(&self->sub68));
    }
}

void func_8025CF1C(){}

// us-8025f08c (0x8025CF40): when both +0x30/+0x34 pointers are present, build a
// fresh CKizunaLine, copy it into the +0x68 line state (field-by-field,
// including the +0x18..+0x20 pad words the ctor leaves uninitialized), then
// rebuild the line display and re-fetch the two cur/line panes via slot 15.
// extern "C" + noinline for OnFileEvent's unmangled call shape (see func_80257AFC).
extern "C" __declspec(noinline) void func_8025CF40(UnkKizunaSelfCF40* self) {
// explicit == 0 early-return reproduces retail's beq / bne-b branch pair
    if (self->field30 == 0 || self->field34 == 0)
        return;
    self->field3B = 1;
        CKizunaLine line;
        __ct__CKizunaLine(&line, self->field30, self->field34, self->fieldDE);
        self->lineState.field4 = line.field4;
        self->lineState.field8 = line.field8;
        self->lineState.field0C = line.field0C;
        self->lineState.field10 = line.field10;
        self->lineState.field14 = line.field14;
        self->lineState.field15 = line.field15;
        self->lineState.field16 = line.field16;
        self->lineState.field17 = line.field17;
        // retail loads both pad words before storing them in reverse
        { u32 t18 = line.field18; u32 t1C = line.field1C;
        self->lineState.field1C = t1C;
        self->lineState.field18 = t18; }
        self->lineState.field20 = line.field20;
        self->lineState.field24 = line.field24;
        self->lineState.field26 = line.field26;
        self->lineState.field28 = line.field28;
        self->lineState.field2C = line.field2C;
        self->lineState.field30 = line.field30;
        self->lineState.field34 = line.field34;
        self->lineState.field36 = line.field36;
        self->lineState.field38 = line.field38;
        self->lineState.field3C = line.field3C;
        self->lineState.field3D = line.field3D;
        self->lineState.field40 = line.field40;
        func_802580CC((UnkKizunaLineBuild*)&self->lineState);
        self->fieldCC =
            (u32)((UnkKizunaMid59344*)self->lineState.field0C)->field10->target(
                (int)(lbl_eu_8050CB20 + 0xa4), 1);
        self->fieldD0 =
            (u32)((UnkKizunaMid59344*)self->lineState.field0C)->field10->target(
                (int)(lbl_eu_8050CB20 + 0x8b), 1);
}

// File-load completion callback for the kizuna chart: whichever bind file
// finished (line-data handle at +0x28 or current-kizuna handle at +0x2C) gets
// its buffer detached and re-attached into a fresh ArcResourceAccessor built
// under a scratch MEM2 region (RAII Class_8045F858 guard), then the matching
// sub-display is rebuilt. Sub-objects are constructed into stack temps and
// copied member-wise into place (the manual vtable word is not part of the
// copy). Retail builds this with the block stmw/lmw save, so the function is
// compiled under optimize_for_size like COption::OnFileEvent.
#pragma optimize_for_size on

// Word-wise view of the radar build temp: retail moves the float members as
// integer bits (lwz/stw), so the copy aliases them through u32 fields.
struct KizunaRadarWords {
    u8 _00[0x04];
    u32 w04; // mArg
    u32 w08;
    u32 w0C;
    u32 w10;
    u32 w14; // mField14 bits
    u32 w18; // mField18 bits
};

bool CKizunagram::OnFileEvent(CEventFile* pEventFile) {
    // Sub-object build temps live on raw buffers: the classes are polymorphic,
    // so a typed local would make MWCC emit a vtable store + dtor call that
    // retail (built via free __ct__ functions) does not have.
    if (field28 == (u32)pEventFile->mFileHandle) {
        u8 infoBuf[0x1C];
        u8 curBuf[0x14];
        u8 radarBuf[0x1C];
        mMemRegionA.createRegion((int)mtl::MemManager::getHandleMEM2(), 0x40000,
                                 lbl_eu_8050CB20 + 0xd07, 0);
        Class_8045F858 host(&mMemRegionA);
        void* fileData = ((CFileHandle*)field28)->getData();
        mtl::MemManager::func_80434A4C(false);
        // Store the accessor straight into the field and re-load it from there
        // at each use - retail never keeps it live in a register across calls.
        field30 = (u32)createArcResourceAccessor__10CLibLayoutFv();
        ((nw4r::lyt::ArcResourceAccessor*)field30)->Attach(fileData,
                                                           lbl_eu_8050CB20 + 0xd13);

        // Info display: build temp, copy body (manual vtable word excluded),
        // then build its layout.
        __ct__CKizunaInfo((CKizunaInfo*)infoBuf,
                          (nw4r::lyt::ArcResourceAccessor*)field30);
        mInfo.mArcResAcc = ((CKizunaInfo*)infoBuf)->mArcResAcc;
        mInfo.mField08 = ((CKizunaInfo*)infoBuf)->mField08;
        mInfo.mField0C = ((CKizunaInfo*)infoBuf)->mField0C;
        mInfo.mField10 = ((CKizunaInfo*)infoBuf)->mField10;
        mInfo.mField14 = ((CKizunaInfo*)infoBuf)->mField14;
        mInfo.mField15 = ((CKizunaInfo*)infoBuf)->mField15;
        mInfo.mField16 = ((CKizunaInfo*)infoBuf)->mField16;
        mInfo.mField18 = ((CKizunaInfo*)infoBuf)->mField18;
        func_8025B670((UnkKizunaSelfB670*)&mInfo);

        // Current-kizuna cursor: build temp, copy body, then init + publish.
        __ct__CKizunaCur((CKizunaCur*)curBuf,
                         (nw4r::lyt::ArcResourceAccessor*)field30);
        mCur.mArcResAcc = ((CKizunaCur*)curBuf)->mArcResAcc;
        mCur.mField08 = ((CKizunaCur*)curBuf)->mField08;
        mCur.mField0C = ((CKizunaCur*)curBuf)->mField0C;
        mCur.mField10 = ((CKizunaCur*)curBuf)->mField10;
        mCur.mField11 = ((CKizunaCur*)curBuf)->mField11;
        mCur.mField12 = ((CKizunaCur*)curBuf)->mField12;
        func_80257E58((UnkKizunaSelf57E58*)&mCur);
        func_80257F9C((UnkKizunaSelf57D90*)&mCur, 0xFF);

        // Radar: both sides aliased as words - retail moves the float fields
        // as integer bits and stores the last two words in reverse order.
        __ct__CKizunaRadar((CKizunaRadar*)radarBuf,
                           (nw4r::lyt::ArcResourceAccessor*)field30);
        KizunaRadarWords* rw = (KizunaRadarWords*)&mRadar;
        const KizunaRadarWords* rt = (const KizunaRadarWords*)radarBuf;
        rw->w04 = rt->w04;
        rw->w08 = rt->w08;
        rw->w0C = rt->w0C;
        rw->w10 = rt->w10;
        { u32 t14 = rt->w14; u32 t18 = rt->w18;
        rw->w18 = t18; rw->w14 = t14; }
        func_80257AFC((UnkKizunaSelfAFC*)&mRadar);

        if (func_8009ECE0() == 0 || fieldDE != 0) {
            fieldDC = 4;
        }
        func_8025CF40((UnkKizunaSelfCF40*)this);
        field28 = 0;
        mMemRegionA.func_8045F810();
        return true;
    }
    if (field2C == (u32)pEventFile->mFileHandle) {
        mMemRegionB.createRegion((int)mtl::MemManager::getHandleMEM2(), 0x100,
                                 lbl_eu_8050CB20 + 0xd17, 0);
        // Retail quirk: the branch-2 guard is still constructed against
        // mMemRegionA (r31+8), not the region just created.
        Class_8045F858 host(&mMemRegionA);
        void* fileData = ((CFileHandle*)field2C)->getData();
        mtl::MemManager::func_80434A4C(false);
        field34 = (u32)createArcResourceAccessor__10CLibLayoutFv();
        ((nw4r::lyt::ArcResourceAccessor*)field34)->Attach(fileData,
                                                           lbl_eu_8050CB20 + 0xd13);
        func_8025CF40((UnkKizunaSelfCF40*)this);
        field2C = 0;
        mMemRegionB.func_8045F810();
        return true;
    }
    return false;
}
#pragma optimize_for_size off

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// sinit_8025D304: init the 14-color kizuna line palette via func_801C4B60.
void sinit_8025D304() {
    func_801C4B60(&lbl_eu_806647E8, 0x21, 0x1c, 0x15, 0x0);
    func_801C4B60(&lbl_eu_806647F0, 0xb5, 0xb1, 0xab, 0xff);
    func_801C4B60(&lbl_eu_806647F8, 0x21, 0x1e, 0x15, 0x0);
    func_801C4B60(&lbl_eu_80664800, 0xb5, 0xb1, 0xab, 0xff);
    func_801C4B60(&lbl_eu_80664808, 0x17, 0x21, 0x15, 0x0);
    func_801C4B60(&lbl_eu_80664810, 0xb3, 0xaf, 0x97, 0xff);
    func_801C4B60(&lbl_eu_80664818, 0x1c, 0x24, 0x2d, 0x0);
    func_801C4B60(&lbl_eu_80664820, 0xb5, 0xb1, 0xab, 0xff);
    func_801C4B60(&lbl_eu_80664828, 0x1c, 0x20, 0x31, 0x0);
    func_801C4B60(&lbl_eu_80664830, 0xb5, 0xb1, 0xab, 0xff);
    func_801C4B60(&lbl_eu_80664838, 0x1c, 0x2c, 0x1a, 0x0);
    func_801C4B60(&lbl_eu_80664840, 0xe5, 0xea, 0xde, 0xff);
    func_801C4B60(&lbl_eu_80664848, 0x21, 0x1e, 0x15, 0x0);
    func_801C4B60(&lbl_eu_80664850, 0xb5, 0xb1, 0xab, 0xff);
}

// Position the kizuna line panes: scale the +0x14 Vec2 by (const - the +0x10
// pane's scale) into the "line" pane's size, then scale the +0x0C pane's
// translate by the ratio of the two reference-pane size deltas into the line
// pane's translate. Finally pulse the layout anim.
__declspec(noinline) void func_80257B6C(UnkKizunaSelf57B6C* self) {
    if (self->field8 != 0) {
        if (self->field10 != 0) {
            nw4r::math::VEC2 sc = self->field10->GetScale();
            f32 sx = lbl_eu_8066882C - sc.x;
            f32 sy = lbl_eu_8066882C - sc.y;
            f32 tmp[2];
            func_80127BC4(tmp, self->field14);
            tmp[0] *= sx;
            tmp[1] *= sy;
            nw4r::lyt::Pane* pane = self->field8->GetRootPane()->FindPaneByName(
                lbl_eu_8050CB20 + 0x13, 1);
            copyVEC2(const_cast<f32*>(&pane->GetSize().width), tmp);
        }
        if (self->field0C != 0) {
            const char* strbase = lbl_eu_8050CB20;
            nw4r::lyt::Pane* c = self->field0C;
            nw4r::math::VEC3 tr = c->GetTranslate();
            tr.x *= lbl_eu_80668830;
            tr.y *= lbl_eu_80668830;
            nw4r::lyt::Pane* paneA = self->field8->GetRootPane()->FindPaneByName(
                strbase + 0x13, 1);
            f32 sa[2];
            func_80127BC4(sa, const_cast<f32*>(&paneA->GetSize().width));
            nw4r::lyt::Pane* paneB = self->field8->GetRootPane()->FindPaneByName(
                strbase + 0x1d, 1);
            f32 sb[2];
            func_80127BC4(sb, const_cast<f32*>(&paneB->GetSize().width));
            f32 sc2[2];
            func_80127BC4(sc2, const_cast<f32*>(&c->GetSize().width));
            sb[0] -= sa[0];
            sb[1] -= sa[1];
            f32 rx = sb[0] / sc2[0];
            f32 ry = sb[1] / sc2[1];
            tr.x *= rx;
            tr.y *= ry;
            nw4r::lyt::Pane* paneC = self->field8->GetRootPane()->FindPaneByName(
                strbase + 0x13, 1);
            copyVEC3(const_cast<f32*>(&paneC->GetTranslate().x), &tr.x);
        }
        self->field8->Animate(0);
    }
}

// us-8025a11c: reveal up to 16 newly-unlocked kizuna entries starting at the
// shared row cursor lbl_eu_8066485A. For each row, resolve its named pane and
// toggle its visibility per unlock state; unlocked rows get their texture tag
// rewritten from per-state string tables, then the texture is bound to the
// pane. Returns true once the cursor passes the table row count.
extern "C" __declspec(noinline) bool func_8025A11C(UnkKizunaSelfA11C* self) {
    // declaration/statement order mirrors retail's register assignment
    // (self=r28, str=r26, fp=r31, count=r30, i=r29, tag-const=r27)
    char buf[0x24];

    func_8003AA34();
    const char* str = lbl_eu_8050CB20;
    void* fp = getFP__FPCc(str + 0x402);
    u32 count = func_8003B1EC(fp) & 0xffff;

    for (u32 i = 0; i < 0x10; i++) {
        u16 cnt = lbl_eu_8066485A;
        if ((u32)(u16)cnt >= count) {
            return true;
        }
        u32 id = ((u16)cnt + 1) & 0xffff;
        char* rowName = func_8013639C(fp, str + 0x411, id);
        nw4r::lyt::Pane* pane =
            (nw4r::lyt::Pane*)self->field0C->field10->target((int)rowName, 1);
        u32 state = func_8009CF8C(id + 0x608) & 0xff;
        if (pane != NULL) {
            func_80124270(pane, state != 0);
        }
        if (state == 0) {
            // still locked: just advance the cursor
            lbl_eu_8066485A = lbl_eu_8066485A + 1;
            continue;
        }

        // Per-state tag: pick the formatted texture name from this row's tag.
        char* tag = func_8013639C(fp, str + 0x417, id);
        switch (state) {
        case 1:
            if (strcmp(tag, str + 0x494) == 0) {
                sprintf(buf, str + 0x4a1);
            } else if (strcmp(tag, str + 0x4ba) == 0) {
                sprintf(buf, str + 0x4a1);
            } else if (strcmp(tag, str + 0x4c7) == 0) {
                sprintf(buf, str + 0x4d4);
            } else if (strcmp(tag, str + 0x4ed) == 0) {
                sprintf(buf, str + 0x4fa);
            } else if (strcmp(tag, str + 0x513) == 0) {
                sprintf(buf, str + 0x4fa);
            } else if (strcmp(tag, str + 0x3f5) == 0) {
                sprintf(buf, str + 0x520);
            } else if (strcmp(tag, str + 0x539) == 0) {
                sprintf(buf, str + 0x546);
            } else if (strcmp(tag, str + 0x55f) == 0) {
                sprintf(buf, str + 0x546);
            } else if (strcmp(tag, str + 0x56c) == 0) {
                sprintf(buf, str + 0x579);
            } else {
                sprintf(buf, str + 0x592);
            }
            break;
        case 2:
            if (strcmp(tag, str + 0x494) == 0) {
                sprintf(buf, str + 0x597);
            } else if (strcmp(tag, str + 0x4ba) == 0) {
                sprintf(buf, str + 0x597);
            } else if (strcmp(tag, str + 0x4c7) == 0) {
                sprintf(buf, str + 0x5b0);
            } else if (strcmp(tag, str + 0x4ed) == 0) {
                sprintf(buf, str + 0x5c9);
            } else if (strcmp(tag, str + 0x513) == 0) {
                sprintf(buf, str + 0x5c9);
            } else if (strcmp(tag, str + 0x3f5) == 0) {
                sprintf(buf, str + 0x5e2);
            } else if (strcmp(tag, str + 0x539) == 0) {
                sprintf(buf, str + 0x5fb);
            } else if (strcmp(tag, str + 0x55f) == 0) {
                sprintf(buf, str + 0x5fb);
            } else if (strcmp(tag, str + 0x56c) == 0) {
                sprintf(buf, str + 0x614);
            } else {
                sprintf(buf, str + 0x592);
            }
            break;
        case 3:
            if (strcmp(tag, str + 0x494) == 0) {
                sprintf(buf, str + 0x62d);
            } else if (strcmp(tag, str + 0x4ba) == 0) {
                sprintf(buf, str + 0x62d);
            } else if (strcmp(tag, str + 0x4c7) == 0) {
                sprintf(buf, str + 0x646);
            } else if (strcmp(tag, str + 0x4ed) == 0) {
                sprintf(buf, str + 0x65f);
            } else if (strcmp(tag, str + 0x513) == 0) {
                sprintf(buf, str + 0x65f);
            } else if (strcmp(tag, str + 0x3f5) == 0) {
                sprintf(buf, str + 0x678);
            } else if (strcmp(tag, str + 0x539) == 0) {
                sprintf(buf, str + 0x691);
            } else if (strcmp(tag, str + 0x55f) == 0) {
                sprintf(buf, str + 0x691);
            } else if (strcmp(tag, str + 0x56c) == 0) {
                sprintf(buf, str + 0x6aa);
            } else {
                sprintf(buf, str + 0x592);
            }
            break;
        case 4:
            if (strcmp(tag, str + 0x494) == 0) {
                sprintf(buf, str + 0x6c3);
            } else if (strcmp(tag, str + 0x4ba) == 0) {
                sprintf(buf, str + 0x6c3);
            } else if (strcmp(tag, str + 0x4c7) == 0) {
                sprintf(buf, str + 0x6dc);
            } else if (strcmp(tag, str + 0x4ed) == 0) {
                sprintf(buf, str + 0x6f5);
            } else if (strcmp(tag, str + 0x513) == 0) {
                sprintf(buf, str + 0x6f5);
            } else if (strcmp(tag, str + 0x3f5) == 0) {
                sprintf(buf, str + 0x70e);
            } else if (strcmp(tag, str + 0x539) == 0) {
                sprintf(buf, str + 0x727);
            } else if (strcmp(tag, str + 0x55f) == 0) {
                sprintf(buf, str + 0x727);
            } else if (strcmp(tag, str + 0x56c) == 0) {
                sprintf(buf, str + 0x740);
            } else {
                sprintf(buf, str + 0x592);
            }
            break;
        case 5:
            if (strcmp(tag, str + 0x494) == 0) {
                sprintf(buf, str + 0x759);
            } else if (strcmp(tag, str + 0x4ba) == 0) {
                sprintf(buf, str + 0x759);
            } else if (strcmp(tag, str + 0x4c7) == 0) {
                sprintf(buf, str + 0x772);
            } else if (strcmp(tag, str + 0x4ed) == 0) {
                sprintf(buf, str + 0x78b);
            } else if (strcmp(tag, str + 0x513) == 0) {
                sprintf(buf, str + 0x78b);
            } else if (strcmp(tag, str + 0x3f5) == 0) {
                sprintf(buf, str + 0x7a4);
            } else if (strcmp(tag, str + 0x539) == 0) {
                sprintf(buf, str + 0x7bd);
            } else if (strcmp(tag, str + 0x55f) == 0) {
                sprintf(buf, str + 0x7bd);
            } else if (strcmp(tag, str + 0x56c) == 0) {
                sprintf(buf, str + 0x7d6);
            } else {
                sprintf(buf, str + 0x592);
            }
            break;
        }

        if (pane != NULL) {
            void* tex = self->texSrc->getTex(0x74696D67, (u32)buf, 0);
            if (tex != 0) {
                func_80137F88(pane, (u32)tex);
            }
        }
        lbl_eu_8066485A = lbl_eu_8066485A + 1;
    }
    return false;
}

// Per-frame update: dispatch on the mode byte at +0x39, then step every child
// (line display, kizuna sub, cur layout, line panes).
void func_8025C580(UnkKizunaSelfC580* self) {
    switch (self->field39) {
    case 1:
        func_8025CC88(self);
        break;
    case 2:
        func_8025CCA8(self);
        break;
    case 4:
        func_8025CCF8(self);
        break;
    case 5:
        func_8025CD10(self);
        break;
    case 6:
        func_8025CD40(self);
        break;
    }
    func_8025B870(&self->sub4C);
    func_80258F9C(reinterpret_cast<UnkKizunaSelf58F9C*>(&self->sub68));
    func_80257EE0(&self->subAC);
    func_80257B6C(&self->subC0);
}

// Countdown timer at +0x40: decrement; when it hits zero reset the state
// bytes, then publish the remaining scaled count into the slot-15 pane result.
void func_80259D44(UnkKizunaSelf59D44* self) {
    self->field40 = self->field40 - lbl_eu_80668834;
    if (self->field40 <= lbl_eu_80668828) {
        self->field14 = 4;
        self->field15 = 1;
        self->field40 = lbl_eu_80668828;
    }
    int cnt = (int)(lbl_eu_8066885C * (self->field40 / lbl_eu_80668848));
    UnkKizunaRes59344* res =
        ((UnkKizunaMid59344*)self->field0C)->field10->target(
            (int)(lbl_eu_8050CB20 + 0x8b), 1);
    res->fieldB8 = (u8)cnt;
}

// Kizuna-line status query: when the anim at +0x78 sits at frame 0, refine the
// answer with whether the +0x68 sub-anim has finished.
int func_8025CBCC(UnkKizunaSelfCBCC* self) {
    // Sense: constant status when the anim sits AT frame 0; sub-anim query
    // runs while it is still moving.
    if (self->field8C != 0) {
        bool atZero = (lbl_eu_80668828 == self->field78->GetFrame());
        if (atZero) {
            return 0x54;
        }
        bool finished = func_802592D8((UnkKizunaSelf592D8*)&self->sub68);
        return finished ? 0x55 : 0x53;
    } else {
        bool atZero = (lbl_eu_80668828 == self->field78->GetFrame());
        if (atZero) {
            return 0x57;
        }
        bool finished = func_802592D8((UnkKizunaSelf592D8*)&self->sub68);
        return finished ? 0x58 : 0x56;
    }
}

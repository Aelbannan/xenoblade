// Auto-scaffolded catalog TU for kyoshin/CKizunagram
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/CKizunagram.hpp"
#include "kyoshin/CTitle.hpp"  // setGXColorS10 (canonical C++ declaration)
#include "kyoshin/code_80135FDC.hpp"  // layout/anim/font helpers (extern "C" pre-mangled names)

#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/util/MemManager.hpp"
#include <revolution/TPL.h>
#include <new>
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// mtl::MemManagerF::allocate is a member (this, size, kind); the shared header
// declares the two-arg caller form used elsewhere, so route this call through
// an explicitly-typed function pointer.
typedef void* (*KizunaAllocFn)(void*, u32, u32);

// PS kernels defined further down in this TU.
extern "C" void kizAddVec3(float* dst, const float* a, const float* b);
extern "C" void kizScaleVec3(float* dst, const float* src, float scale);

// --- C-linkage / mangled-symbol callees used by the target functions ---
// advanceAnimTransform is C++-linkage (mangled retail), AnimRewindFrame / CMenuArtsSet_setPaneScale /
// copyVEC2 are unmangled in retail (extern "C"), and buildLayout is referenced
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


// Same-TU display-state helper consumed by lineTickSelIn / lineTickSelOut
// (declaration moved to CKizunagram.hpp C-linkage imports).

// Forward declarations for callees used in tickKizMove
// publishKizScaledVals is target 5 (takes the display self); the tail calls pass the
// same self pointer. slideKizDown / tickCurByFlag are still-unknown stubs.
// call, turning the retail `b slideKizDown` into a bare return.
extern "C" void slideKizDown(UnkKizunaSelfCE78* self);
extern "C" void setCurPanePhase(UnkKizunaSelf57D90* self, u32 a);

// TU-local view of tickCurByFlag's object: ready flag at +0x8C with the
// cursor-phase sub-object at +0xAC (same offsets the mode-3 tryKizMode3
// path addresses raw). Exact layout, member access only.
struct TickCurByFlagSelf {
    u8 _00[0x8C];
    u8 field8C;                // +0x8C
    u8 _8D[0xAC - 0x8D];
    UnkKizunaSelf57D90 subAC;  // +0xAC
};
extern "C" void tickCurByFlag(void* self) {
    TickCurByFlagSelf* v = (TickCurByFlagSelf*)self;
    if (v->field8C != 0) {
        setCurPanePhase(&v->subAC, 1);
    } else {
        setCurPanePhase(&v->subAC, 0);
    }
}

// Object with a dispatch byte at 0x3A - drives display state in CKizunagram
struct UnkKizunaDisp {
    u8 _00[0x3A];
    u8 field_0x3A;
};

// TU-local view of the chart-state bytes probed by kizChartReady /
// kizChartOpen (gate bytes at +0x61/+0x7D/+0x7E/+0xBD, result bytes at
// +0x3B/+0x3C). Exact layout, member access only.
struct KizChartState {
    u8 _00[0x3b];
    u8 f3B;                   // +0x3B (kizChartReady result)
    u8 f3C;                   // +0x3C (kizChartOpen result)
    u8 _3D[0x61 - 0x3d];
    u8 f61;                   // +0x61
    u8 _62[0x7d - 0x62];
    u8 f7D;                   // +0x7D
    u8 f7E;                   // +0x7E (kizChartReady gate)
    u8 _7F[0xbd - 0x7f];
    u8 fBD;                   // +0xBD
};

extern "C" void kizOpenWinNop() {}


// Toggle both pane entries each frame: walk the +0x08 child to its +0x10
// sub-object, pull its slot-15 layout pane for each id in a 2-word table, and
// repaint the pane matching the given phase byte.
extern "C" void setCurPanePhase(UnkKizunaSelf57D90* self, u32 a) {
    u32 paneIds[2] = { lbl_eu_80668838, lbl_eu_8066883C };
    for (u8 i = 0; i < 2; i++) {
        UnkKizunaRes59344* res =
            ((UnkKizunaMid59344*)self->field8)->field10->target(paneIds[i], 1);
        setPaneVisible((nw4r::lyt::Pane*)res, (u32)(i == a));
    }
}

// ctor: retail __ct__CKizunaRadar (unmangled). Stores the manual vtable
// label, the r4 arg, then zero/float fields.
// CKizunagram::OnFileEvent (retail calls them out-of-line).
extern "C" void __ct__CKizunaRadar(CKizunaRadar* self, void* arg) {
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

// (rebuildKizLine calls it out-of-line in retail).
extern "C" void __ct__CKizunaLine(CKizunaLine* self, u32 arg4, u32 arg8, u8 arg3D) {
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

// Release/null a +0x08 child object: if non-null, finalize it (vtable slot 2) with a 1
// flag, then clear the pointer.
extern "C" void releaseRadarChild(UnkKizunaSelf57D90* self) {
    // Retail re-tests field8 inside the guard: the duplicated condition CSEs to
    // one cmpwi with two beq's (dead second branch -- the btm_sec pattern).
    if (self->field8 != 0) {
        if (self->field8 != 0) {
            self->field8->target2(1);
        }
        self->field8 = 0;
    }
}

// Same shape as releaseRadarChild (sibling release/null helper).
extern "C" void releaseCurChild(UnkKizunaSelf57D90* self) {
    // Same retail shape as releaseRadarChild: duplicated guard test CSEs to one
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
extern "C" void buildCurLayout(UnkKizunaSelf57E58* self) {
    buildLayout((nw4r::lyt::Layout**)&self->field8, self->field4, lbl_eu_8050CB20 + 0x27);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->field8, &self->field0C, self->field4, lbl_eu_8050CB20 + 0x3f);
    self->field8->slot11((u32)self->field0C, 1);
    self->field8->slot14(0);
}

// us-8025a308 (0x802580CC): (re)build the kizuna-line layout from the shared
// arc resource, bind its animation and the device font, then either populate
// the current-kizuna display (id != 0) or publish the idle cursor position,
// and finally refresh every label/texture pane from the BDAT tables.
extern "C" void buildKizLineLayout(UnkKizunaLineBuild* self) {
    char nameBuf[0x20];

    buildLayout((nw4r::lyt::Layout**)&self->layout, (nw4r::lyt::ArcResourceAccessor*)self->acc, "mf40_kiz_line.brlyt");
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->layout, &self->anim, (nw4r::lyt::ArcResourceAccessor*)self->acc, "mf40_kiz_line_scale1.brlan");

    // Bind the device font to the layout's root pane.
    nw4r::lyt::Pane* rootPane =
        (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)->field10;
    void* font = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
        1, (nw4r::lyt::Layout*)self->layout);
    u32 bound = ((UnkKizunaFontView*)font)->vf7();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)bound);

    self->layout->slot11((u32)self->anim, 1);
    self->layout->slot14(0);

    // Reset then hide the shared line pane.
    ((UnkKizunaMid59344*)self->layout)
        ->field10->target((int)("nul_all"), 1)
        ->fieldB8 = 0;
    setPaneVisible(
        (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)->field10->target(
            (int)("nul_all"), 1),
        0);

    // Region-dependent line color overrides.
    void* lytv = self->layout;
    u8 lang = (u8)lbl_eu_80664184;
    if (lang == 0x19) {
        PaneMatSetTevColorsByName((nw4r::lyt::Layout*)lytv, "pic_bg", &kizLineTevColorRegion19First, &kizLineTevColorRegion19Second);
        PaneSetVtxColorPairsByName((nw4r::lyt::Layout*)lytv, "pic_bg", (int)0xF9FFF2FF, 0x94AA8CFF);
    } else if (lang == 0x1A) {
        PaneMatSetTevColorsByName((nw4r::lyt::Layout*)lytv, "pic_bg", &kizLineTevColorRegion1AFirst, &kizLineTevColorRegion1ASecond);
        PaneSetVtxColorPairsByName((nw4r::lyt::Layout*)lytv, "pic_bg", -1, 0x8CA0EBFF);
    } else {
        switch (MapValueToRank6(lang)) {
        case 0:
        case 1:
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)lytv, "pic_bg", &kizLineTevColorRank01First, &kizLineTevColorRank01Second);
            PaneSetVtxColorPairsByName((nw4r::lyt::Layout*)lytv, "pic_bg", -1, 0xEEB88AFF);
            break;
        case 2:
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)lytv, "pic_bg", &kizLineTevColorRank2First, &kizLineTevColorRank2Second);
            PaneSetVtxColorPairsByName((nw4r::lyt::Layout*)lytv, "pic_bg", -1, 0xE0C072FF);
            break;
        case 3:
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)lytv, "pic_bg", &kizLineTevColorRank3First, &kizLineTevColorRank3Second);
            PaneSetVtxColorPairsByName((nw4r::lyt::Layout*)lytv, "pic_bg", -1, 0xE1F8A8FF);
            break;
        case 4:
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)lytv, "pic_bg", &kizLineTevColorRank4First, &kizLineTevColorRank4Second);
            PaneSetVtxColorPairsByName((nw4r::lyt::Layout*)lytv, "pic_bg", -1, 0xB4DEFFFF);
            break;
        case 5:
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)lytv, "pic_bg", &kizLineTevColorRank5First, &kizLineTevColorRank5Second);
            PaneSetVtxColorPairsByName((nw4r::lyt::Layout*)lytv, "pic_bg", -1, 0xBCBAE6FF);
            break;
        }
    }

    // Fresh overlay pane appended under the shared root.
    nw4r::lyt::Pane* newPane = NULL;
    void* paneMem = mtl::MemManager::allocate(
        0xD8, CLibLayout::getAllocHandle());
    if (paneMem != NULL) {
        newPane = new (paneMem) nw4r::lyt::Pane();
    }
    newPane->SetName("face_icon");
    setPaneVisible(newPane, 1);
    nw4r::lyt::Pane* parent =
        (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)->field10->target(
            (int)("nul_scroll"), 1);
    parent->AppendChild(newPane);

    u16 id;
    if (self->field3D == 0) {
        id = (u16)CtrlObjectParam_GetWorkTailValue();
    } else {
        id = 0;
    }

    if (id != 0) {
        self->field26 = id;
        UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(::cf::CfBdat::spFldNpcListFileData);
        UnkKizunaObj59344* rootP =
            ((UnkKizunaMid59344*)self->layout)->field10;

        // Pull both anchor panes and shift posA by the BDAT-scaled delta of
        // posB so posA ends up at the mirrored display position.
        UnkKizunaRes59344* resA = rootP->target((int)("nul_scroll"), 1);
        UnkKizunaVec3 posA = resA->pos;
        UnkKizunaRes59344* resB = rootP->target((int)("face_icon"), 1);
        UnkKizunaVec3 posB = resB->pos;

        s16 vx = (s16)BdatGetS16Direct(mgr, "rlt_posX", id);
        s16 vy = (s16)BdatGetS16Direct(mgr, "rlt_posY", id);
        UnkKizunaVec3 delta;
        delta.x = (f32)vx;
        delta.y = (f32)vy;
        delta.z = lbl_eu_80668828;
        nw4r::math::VEC3Sub((nw4r::math::VEC3*)&posB.x,
                            (const nw4r::math::VEC3*)&posB.x,
                            (const nw4r::math::VEC3*)&delta.x);
        kizAddVec3(&posA.x, &posA.x, &posB.x);

        resA = rootP->target((int)("nul_scroll"), 1);
        copyVEC3((f32*)&resA->pos, (f32*)&posA);

        char* texRowName = BdatGetPtrDirect(mgr, "rlt_face", id);
        if (strcmp(texRowName, "") != 0) {
            char* tplName =
                MakeTplNameSysFile(BdatGetU16Direct(mgr, "rlt_texture", id));
            TPLPalette* pal =
                (TPLPalette*)self->texSrc->getTex(0x74696D67, (u32)tplName, 0);
            if (pal != 0) {
                nw4r::lyt::Picture* pic = NULL;
                void* picMem = mtl::MemManager::allocate(
                    0xF0, CLibLayout::getAllocHandle());
                if (picMem != NULL) {
                    nw4r::lyt::TexMap texMap(pal, 0);
                    pic = new (picMem) nw4r::lyt::Picture(texMap);
                }
                char picNameBuf[0x20];
                sprintf(picNameBuf, "%s", texRowName);
                pic->SetName(picNameBuf);

                // Position the picture from the same BDAT columns (z fixed).
                s16 px = (s16)BdatGetS16Direct(mgr, "rlt_posX", id);
                s16 py = (s16)BdatGetS16Direct(mgr, "rlt_posY", id);
                UnkKizunaVec3 picPos;
                picPos.x = (f32)px;
                picPos.y = (f32)py;
                picPos.z = lbl_eu_80668828;
                copyVEC3((f32*)&((KizunaPaneTranslateView*)pic)->translate,
                         (f32*)&picPos);

                setPaneVisible(pic, 1);
                nw4r::lyt::Pane* picParent =
                    (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)
                        ->field10->target((int)("face_icon"), 1);
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
        UnkKizunaRes59344* res = rootP->target((int)("nul_scroll"), 1);
        UnkKizunaVec3 posBase = res->pos;
        UnkKizunaVec3 posCur = res->pos; // retail keeps this dead copy
        UnkKizunaRes59344* resAdd =
            ((UnkKizunaObj59344*)res)->target((int)("pic_pcbs"), 1);
        UnkKizunaVec3 posAdd = resAdd->pos;
        kizAddVec3(&posBase.x, &posBase.x, &posAdd.x);
        UnkKizunaRes59344* resScale =
            ((UnkKizunaObj59344*)res)->target((int)("pic_pcbs"), 1);
        f32 scalePair[2];
        TagCopyVec2f(scalePair, resScale->scale);
        f32 scaled[3];
        kizScaleVec3(scaled, &posBase.x, -1.0f);
        copyVEC3(&self->result18.x, scaled);
        self->field24 = 1;
        UnkKizunaVec3 out = self->result18;
        (void)posCur;
        placeKizMarker((UnkKizunaSelf59394*)self, &out);
    }

    sKizRowCursor = 0;
    sKizRevealCursor = 0;
    refreshKizEntryPanes(reinterpret_cast<UnkKizunaSelf9820*>(self));

    // Pick the progress message tag from the game-progress byte at 0x20.
    u32 prog20 = (u16)CtrlRemote_TouchBitByArg(0x20);
    const char* msg;
    if (prog20 >= 0x64) {
        if (prog20 >= 0x89) {
            msg = (prog20 >= 0x113) ? "mf40_kiz_pc06.tpl" : "mf40_kiz_pc05.tpl";
        } else {
            msg = (prog20 >= 0x80) ? "mf40_kiz_pc04.tpl" : "mf40_kiz_pc03.tpl";
        }
    } else if (prog20 >= 0x2a) {
        msg = (prog20 >= 0x45) ? "mf40_kiz_pc02.tpl" : "mf40_kiz_pc00.tpl";
    } else if (prog20 >= 0xb) {
        msg = "mf40_kiz_pc01.tpl";
    } else {
        msg = "mf40_kiz_pc00.tpl";
    }

    void* msgTex = self->acc->getTex(0x74696D67, (u32)msg, 0);
    if (msgTex != 0) {
        PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->layout, "pic_pc", (TPLPalette*)msgTex);
    }

    // Hide the five fixed number/name pane pairs.
    for (u32 i = 1; i <= 5; i++) {
        sprintf(nameBuf, "pc_map%02d", (u8)i);
        setPaneVisible(
            (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)->field10->target(
                (int)nameBuf, 1),
            0);
        sprintf(nameBuf, "mapicon_%02d", (u8)i);
        setPaneVisible(
            (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)->field10->target(
                (int)nameBuf, 1),
            0);
    }

    // Fixed per-layout color/tag overrides, then per-row entries.
    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(::cf::CfBdat::spFldNpcListFileData);
    nw4r::lyt::Layout* lytL = (nw4r::lyt::Layout*)self->layout;
    u32 rowCount = Bdat_GetMaxRow_B1EC(mgr);
    LayoutSetTextBoxFmtValue(lytL, "txt_area01_00", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area02_00", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area03_00", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area04_00", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area05_00", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area01_00u", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area02_00u", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area03_00u", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area04_00u", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area05_00u", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area01_00o", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area02_00o", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area03_00o", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area04_00o", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area05_00o", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area01_00r", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area02_00r", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area03_00r", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area04_00r", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area05_00r", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area01_00l", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area02_00l", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area03_00l", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area04_00l", "", 0);
    LayoutSetTextBoxFmtValue(lytL, "txt_area05_00l", "", 0);

    for (u32 j = 1;; j++) {
        if ((s32)(u16)j > (s32)rowCount) {
            break;
        }
        u32 rowProg = (u16)BdatGetU16Direct(mgr, "rlt_meet", (u16)j);
        if (rowProg != 0 && CtrlRemote_TouchBitByArg(rowProg + 0xa20) != 0) {
            char* nm = BdatGetPtrDirect(mgr, "rlt_face", (u16)j);
            if (strcmp(nm, "") != 0) {
                u32 colv = (u16)BdatGetU16Direct(mgr, "rlt_lnd", (u16)j);
                u32 state =
                    MapValueToRank6(BdatGetU8ByTableKey("landmarklist", "mapID", colv));
                sprintf(nameBuf, "pc_map%02d", (u8)state);
                setPaneVisible(
                    (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)
                        ->field10->target((int)nameBuf, 1),
                    1);
                sprintf(nameBuf, "mapicon_%02d", (u8)state);
                setPaneVisible(
                    (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->layout)
                        ->field10->target((int)nameBuf, 1),
                    1);
                char* label =
                    BdatTouchStringCell("MNU_relate", "name", (u8)state + 0x24);
                sprintf(nameBuf, "txt_area%02d_00", (u8)state);
                LayoutSetTextBoxFmtValue(lytL, nameBuf, label, 0);
                sprintf(nameBuf, "txt_area%02d_00u", (u8)state);
                LayoutSetTextBoxFmtValue(lytL, nameBuf, label, 0);
                sprintf(nameBuf, "txt_area%02d_00o", (u8)state);
                LayoutSetTextBoxFmtValue(lytL, nameBuf, label, 0);
                sprintf(nameBuf, "txt_area%02d_00r", (u8)state);
                LayoutSetTextBoxFmtValue(lytL, nameBuf, label, 0);
                sprintf(nameBuf, "txt_area%02d_00l", (u8)state);
                LayoutSetTextBoxFmtValue(lytL, nameBuf, label, 0);
            }
        }
    }

    // Texture panes for each affinity rank, plus its five progress digits.
    for (u32 k = 1; k <= 5; k++) {
        sprintf(nameBuf, "mapicon_%02d", (u8)k);
        UnkKizunaRes59344* visPane =
            ((UnkKizunaMid59344*)self->layout)->field10->target((int)nameBuf, 1);
        if (isPaneVisible((nw4r::lyt::Pane*)visPane)) {
            u32 prog = CtrlRemote_TouchBitByArg((u8)k + 0x21);
            const char* texTag = NULL;
            if (prog >= 6000) {
                if (prog < 8000) {
                    texTag = "mf40_kiz_pclin_b.tpl";
                } else if (prog < 10001) {
                    texTag = "mf40_kiz_pclin_a.tpl";
                }
            } else if (prog >= 4000) {
                texTag = "mf40_kiz_pclin_c.tpl";
            } else if (prog >= 2000) {
                texTag = "mf40_kiz_pclin_d.tpl";
            } else {
                texTag = "mf40_kiz_pclin_e.tpl";
            }
            if (texTag != NULL) {
                void* tpl = self->acc->getTex(0x74696D67, (u32)texTag, 0);
                if (tpl != 0) {
                    sprintf(nameBuf, "pc_map%02d", (u8)k);
                    PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->layout, (const char*)nameBuf, (TPLPalette*)tpl);
                }
            }
            for (u32 d = 0; d < 5; d++) {
                sprintf(nameBuf, "pic_est%02d_m%02d", (u8)(d + 1), (u8)k);
                UnkKizunaRes59344* pane =
                    ((UnkKizunaMid59344*)self->layout)->field10->target(
                        (int)nameBuf, 1);
                setPaneVisible((nw4r::lyt::Pane*)pane,
                              prog - d * 2000 >= 2000);
            }
        }
    }
}

// us-8025b198 (0x8025B198): game-local PSVECAdd kernel -- dst[i] = a[i] + b[i], i in 0..2.
// Retail is a pure paired-single body (psq_l/ps_add/psq_st, no frame, arg order r3=dst,
// r4=a, r5=b) that MWCC cannot emit from scalar C++ (no ps_add intrinsic; scalar C++
// yields 9 lfs/fadds/stfs). Isolated Gekko paired-single backend (PLAN.md 17.6).
#if defined(__MWERKS__) && !defined(NONMATCHING)
extern "C" asm void kizAddVec3(register float* dst, register const float* a,
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
extern "C" void kizAddVec3(float* dst, const float* a, const float* b) {
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
}
#endif

// us-8025b1bc (0x8025B1BC): game-local PSVECScale kernel -- dst = src * scale (3 floats).
// Retail is a pure paired-single body (psq_l/ps_muls0/psq_st, no frame, arg order r3=dst,
// r4=src, f1=scale). MWCC has no ps_muls0 intrinsic and scalar C++ yields
// lfs/fmuls/stfs. Isolated Gekko paired-single backend (PLAN.md 17.6).
#if defined(__MWERKS__) && !defined(NONMATCHING)
extern "C" asm void kizScaleVec3(register float* dst, register const float* src,
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
extern "C" void kizScaleVec3(float* dst, const float* src, float scale) {
    dst[0] = src[0] * scale;
    dst[1] = src[1] * scale;
    dst[2] = src[2] * scale;
}
#endif

extern "C" void tickLineState(UnkKizunaSelf58F9C* self) {
    if (self->field0C != 0) {
        if (buildKizEntryPanes(reinterpret_cast<UnkKizunaSelf59DE8*>(self))) {
            if (revealKizEntries(reinterpret_cast<UnkKizunaSelfA11C*>(self))) {
                // mode-specific line-state handler
                switch (self->field34) {
                case 0:
                    lineStartSelect(self);
                    break;
                case 1:
                    lineTickSelIn(reinterpret_cast<UnkKizunaSelfAB*>(self));
                    break;
                case 2:
                    lineTickSelOut(reinterpret_cast<UnkKizunaSelfAB*>(self));
                    break;
                case 3:
                    lineClearSelect(self);
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
            lineCountUp(reinterpret_cast<UnkKizunaSelf59C5C*>(self));
            break;
        case 3:
            lineCountDown(reinterpret_cast<UnkKizunaSelf59D44*>(self));
            break;
        }
        self->field0C->callSlot14(0);
    }
}

// Retail walks a context-string list: separate condition-temp locals (the
// while-test copies cur/getContextStrPtr into fresh slots each pass) drive
// TagProcCompareIds; the body splits off the next token and frees its pane.
extern "C" void freeLinePanes(UnkKizunaSelf59B18* self) {
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
           TagProcCompareIds(&condCur, &condEnd)) {
        tmp = cur;
        TagCtxAdvanceHead(&tmp, 0);
        const wchar_t* s = TagCtxGetNodeHeader(&cur);
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
extern "C" void lineEnterMode1(UnkKizunaSelf59228* self) {
    self->field14 = 1;
    self->field15 = 0;
    self->field0C->target8(self->field10);
    self->field40 = lbl_eu_80668828;
}

// Same shape as lineEnterMode1 but with state flag 3 and a different idle float.
extern "C" void lineEnterMode3(UnkKizunaSelf59228* self) {
    self->field14 = 3;
    self->field15 = 0;
    self->field0C->target8(self->field10);
    self->field40 = lbl_eu_80668848;
}

// True when the anim's current frame has reached (its frame count - 1).
bool kizAnimAtEnd(UnkKizunaSelf592D8* self) {
    return self->field10->GetFrame() >= (f32)(self->field10->GetFrameSize() - 1);
}

// Dispatch: walk +0x0C -> +0x10, call vtable slot 15 with (0x8B, 1), then return a
// 2-word struct built from the result's +0x44 / +0x48 words.
extern "C" UnkKizunaPair getLinePanePair(UnkKizunaSelf59344* self) {
    UnkKizunaRes59344* res = self->field0C->field10->target((int)(lbl_eu_8050CB20 + 0x8b), 1);
    UnkKizunaPair out = *(UnkKizunaPair*)((u8*)res + 0x44);
    return out;
}

// us-8025b6d8: walk the current kizuna line's tag-context list. For each item
// compute the doubled offset of its position from the two reference panes;
// while inside the near radius, snap the marker to the item (optionally
// selecting it via findKizByName when well inside), play the confirm sound
// once, and return. After the list is exhausted, ease toward the registered
// pane position instead.
extern "C" int tickKizMarker(UnkKizunaSelf5949C* self) {
    self->field24 = 0;
    self->field26 = 0;
    // retail reloads field0C per access (no cached child pointer)
    UnkKizunaRes59344* linePane =
        self->field0C->field10->target((int)("nul_scroll"), 1);
    UnkKizunaRes59344* tagPane =
        self->field0C->field10->target((int)("face_icon"), 1);
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
           TagProcCompareIds(&condCur, &condEnd)) {
        UnkKizunaVec3 work = linePos;
        const UnkKizunaCtxItem* item =
            reinterpret_cast<const UnkKizunaCtxItem*>(TagCtxGetNodeHeader(&cur));
        // retail materializes the item position copy (@0x6c) before the
        // delta temp (@0x48), then adds into work in place
        UnkKizunaVec3 itemPos = item->pos;
        UnkKizunaVec3 delta;
        kizAddVec3(&delta.x, &tagPos.x, &itemPos.x);
        kizAddVec3(&work.x, &work.x, &delta.x);
        // retail negates y before x
        f32 ny = -work.y;
        f32 nx = -work.x;
        f32 distSq = ny * ny + nx * nx;
        if (distSq <= nearThr) {
            UnkKizunaVec3 scaled;
            f32 snapA[2] = { lbl_eu_80668834, lbl_eu_80668834 };
            const UnkKizunaCtxItem* it2 =
                reinterpret_cast<const UnkKizunaCtxItem*>(TagCtxGetNodeHeader(&cur));
            CMenuArtsSet_setPaneScale((UnkKizunaFunc31848Obj*)it2,
                          reinterpret_cast<const UnkKizunaPair*>(&snapA[0]));
            kizScaleVec3(&scaled.x, &work.x, lbl_eu_80668830);
            copyVEC3(&self->field18.x, &scaled.x);
            // threshold recomputed from the constant at the use site
            // (retail does not cache it across the loop)
            if (distSq <=
                (lbl_eu_80668854 + lbl_eu_80668854) *
                    (lbl_eu_80668854 + lbl_eu_80668854)) {
                const UnkKizunaCtxItem* it3 =
                    reinterpret_cast<const UnkKizunaCtxItem*>(TagCtxGetNodeHeader(&cur));
                // the item's name lives at +0xBC of the context item
                self->field26 =
                    findKizByName(self, reinterpret_cast<const char*>(it3) + 0xBC);
            }
            if (self->field3C == 0) {
                self->field3C = 1;
                playUISound(0x7b);
            }
            return 1;
        } else {
            f32 far_[2] = { lbl_eu_8066884C, lbl_eu_8066884C };
            const UnkKizunaCtxItem* it4 =
                reinterpret_cast<const UnkKizunaCtxItem*>(TagCtxGetNodeHeader(&cur));
            CMenuArtsSet_setPaneScale((UnkKizunaFunc31848Obj*)it4,
                          reinterpret_cast<const UnkKizunaPair*>(&far_[0]));
            TagCtxAdvanceHead(&cur, 0);
        }
    }
    // list exhausted: ease toward the pane's registered position.
    // retail copies linePos into its own slot (@0x60) before fetching
    // the registered pane, then adds the registered pos in place.
    UnkKizunaVec3 eased = linePos;
    UnkKizunaRes59344* reg1 = ((UnkKizunaObj59344*)linePane)->target(
        (int)("pic_pcbs"), 1);
    UnkKizunaVec3 regPos = reg1->pos;
    kizAddVec3(&eased.x, &eased.x, &regPos.x);
    UnkKizunaRes59344* reg2 = ((UnkKizunaObj59344*)linePane)->target(
        (int)("pic_pcbs"), 1);
    f32 sz[2];
    TagCopyVec2f(sz, reg2->scale);
    f32 rad = lbl_eu_8066884C * (sz[0] - lbl_eu_80668858);
    f32 distSq2 = -eased.y * -eased.y + -eased.x * -eased.x;
    if (distSq2 <= (rad + rad) * (rad + rad)) {
        UnkKizunaVec3 scaled;
        kizScaleVec3(&scaled.x, &eased.x, lbl_eu_80668830);
        copyVEC3(&self->field18.x, &scaled.x);
        u8 played = self->field3C;
        self->field24 = 1;
        if (played == 0) {
            self->field3C = 1;
            playUISound(0x7b);
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
extern "C" void resetKizPaneTags(UnkKizunaSelf59B18* self);
extern "C" void copyKizName3(char* dest, const char* src);

void refreshKizEntryPanes(UnkKizunaSelf9820* self) {
    resetKizPaneTags(reinterpret_cast<UnkKizunaSelf59B18*>(self));
    if (self->field26 == 0) {
        return;
    }

    setPaneVisible(
        (nw4r::lyt::Pane*)((UnkKizunaMid59344*)self->field0C)->field10->target(
            (int)("txt_relate01"), 1),
        1);

    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(::cf::CfBdat::spFldNpcListFileData);
    char* curName = BdatGetPtrDirect(mgr, "rlt_face", self->field26);

    // 4-byte (3 chars + NUL) name copies compared as words
    char nameCur[4];
    copyKizName3(nameCur, curName);

    void* fp = getFP("JNL_relatelist");
    u16 count = Bdat_GetMaxRow_B1EC(fp);

    // single base pointer for the shared arc string table (CSE of the above)
    char* str = "mf40_kiz_rdr.brlyt";

    char nameRow[4];
    char nameRowAlt[4];
    char fmtBuf[0x20];
    for (u16 i = 1; (u32)i <= (u32)count; i++) {
        char* rowName = BdatGetPtrDirect(fp, str + 0x411, i);
        copyKizName3(nameRow, rowName);
        copyKizName3(nameRowAlt, rowName + 3);

        // word compare: current name equals the row name or its offset-3 form
        if (*(s32*)nameCur != *(s32*)nameRow && *(s32*)nameCur != *(s32*)nameRowAlt) {
            continue;
        }

        UnkKizunaRes59344* resA = self->field0C->field10->target((int)curName, 1);
        UnkKizunaRes59344* resB = self->field0C->field10->target((int)rowName, 1);
        if (resA == NULL || resB == NULL) {
            continue;
        }
        u8 state = (u8)CtrlRemote_TouchBitByArg(i + 0x608);
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
        char* tagName = BdatGetPtrDirect(fp, tag, i);
        sprintf(fmtBuf, str + 0x42b, state);
        char* newName = BdatGetPtrDirect(fp, fmtBuf, i);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->field0C, tagName, newName, 0);
        sprintf(fmtBuf, str + 0x435, tagName);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->field0C, fmtBuf, newName, 0);
        sprintf(fmtBuf, str + 0x439, tagName);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->field0C, fmtBuf, newName, 0);
        sprintf(fmtBuf, str + 0x43d, tagName);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->field0C, fmtBuf, newName, 0);
        sprintf(fmtBuf, str + 0x441, tagName);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->field0C, fmtBuf, newName, 0);
    }
}

extern "C" void copyKizName3(char* dest, const char* src) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; dest[3] = 0; }

// us-8025bd54 (0x8025BD54): if the shared arc layout's named root pane is
// visible, hide it and re-apply the 5 kizuna pane-name groups for indices
// 1..0x11 to the layout.
extern "C" void resetKizPaneTags(UnkKizunaSelf59B18* self) {
    // retail buffer is 0x1c bytes; size drives MWCC frame allocation
    char buf[0x1C];
    nw4r::lyt::Pane* pane = (nw4r::lyt::Pane*)((UnkKizunaLayoutSub57AFC*)self->field0C)
                                ->field10->slot15(lbl_eu_8050CB20 + 0x3f5, 1);
    if (isPaneVisible(pane) != 0) {
        setPaneVisible(pane, 0);
        for (u8 i = 1; i <= 0x12; i++) {
            sprintf(buf, lbl_eu_8050CB20 + 0x445, i);
            LayoutSetTextBoxFmtValue(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x454, i);
            LayoutSetTextBoxFmtValue(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x464, i);
            LayoutSetTextBoxFmtValue(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x474, i);
            LayoutSetTextBoxFmtValue(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
            sprintf(buf, lbl_eu_8050CB20 + 0x484, i);
            LayoutSetTextBoxFmtValue(self->field0C, buf, lbl_eu_8050CB20 + 0xd3, 0);
        }
    }
}

// Countdown advance: tick the +0x40 timer; when it reaches the limit switch to
// mode 2 and publish via the mid object's slots 7/11. Then scale the remaining
// time into the "line" pane result's byte at +0xB8 and mark it visible.
extern "C" void lineCountUp(UnkKizunaSelf59C5C* self) {
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
    setPaneVisible((nw4r::lyt::Pane*)res, 1);
}

// Place the kizuna marker: fetch the "line" pane result, copy its position
// and size, add the requested offset, clamp into the scaled pane bounds, and
// write the clamped position back.
extern "C" void placeKizMarker(UnkKizunaSelf59394* self,
                                                   UnkKizunaVec3* arg) {
    UnkKizunaRes59344* res =
        self->field0C->field10->target((int)("nul_scroll"), 1);
    UnkKizunaVec3 pos = res->pos;      // word-copy of the animated position
    f32 size[2];
    TagCopyVec2f(size, res->scale);  // copy the pane's current size pair
    // Statement order is load-bearing for byte-identity (MWCC scheduling):
    // x offset, both C1-scaled bounds, y offset, then the two C2 bounds.
    // The `arg` parameter must stay non-const: const-qualified TBAA changes
    // the store/commit schedule and breaks the match. Residual vs retail is
    // pure float register coloring only (0 structural, no reloc drift).
    pos.x = pos.x + arg->x;
    f32 minX, minY, maxX, maxY;
    minX = 0.5f * size[0];
    minY = 0.5f * size[1];
    pos.y += arg->y;
    maxX = -1.0f * minX;
    maxY = -1.0f * minY;
    if (pos.x > minX) pos.x = minX;
    if (pos.x < maxX) pos.x = maxX;
    if (pos.y > minY) pos.y = minY;
    if (pos.y < maxY) pos.y = maxY;
    copyVEC3(&res->pos.x, &pos.x);
}


// Build the line layout from the shared arc string(+0x4C), then copy the animated
// position at +0x4C of the slot-15 result into the +0x14 Vec2.
extern "C" void buildRadarLayout(UnkKizunaSelfAFC* self) {
    buildLayout(&self->field8, self->field4, lbl_eu_8050CB20);
    UnkKizunaLCBRes57AFC* res =
        ((UnkKizunaLayoutSub57AFC*)self->field8)->field10->slot15(lbl_eu_8050CB20 + 0x13, 1);
    copyVEC2(&self->field14[0], res->vec);
}

// us-8025c024: walk up to 16 kizuna BDAT rows starting at the shared row
// cursor. For each row not yet unlocked and not a duplicate of the current
// entry, build a textured Picture pane (0xf0-byte allocation from the layout
// heap), position/name it, attach it to the current-line parent pane, and
// tint it when appropriate. Returns true once the cursor passes the row limit.
extern "C" bool buildKizEntryPanes(UnkKizunaSelf59DE8* self) {
    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(::cf::CfBdat::spFldNpcListFileData);
    u16 limit = (u16)(Bdat_GetMaxRow_B1EC(mgr) - 0x64);
    // Unsigned 64-bit subtract; its high word gates the legacy tint below.
    u64 sub = 0x155;
    u64 chk = CtrlRemote_TouchBitByArg(0x20) - sub;
    f32 scale = 0.5f;
    f32 posZ = lbl_eu_80668828;
    u32 i = 0;
    const u32 feConst = 0xfe;
    const u32 texTag = 0x74696d67;
    const u32 tintColor = 0x777777ff;

    for (; i < 0x10; i++) {
        u16 cnt = sKizRowCursor;
        if (cnt >= limit) {
            return true;
        }
        u16 id = cnt + 0x65;
        u16 curId = (self->field3D == 0) ? (u16)CtrlObjectParam_GetWorkTailValue() : 0;
        if (curId == id) {
            goto next;
        }
        {
            u32 tinted = 0;
            if ((u32)(chk >> 32) != 0 && id == 0x30a) {
                tinted = CtrlRemote_TouchBitByArg(0x54b) >= feConst;
            }
            u16 unlockId = BdatGetU16Direct(mgr, "rlt_meet", id);
            if (CtrlRemote_TouchBitByArg(unlockId + 0xa20) == 0) {
                goto next;
            }
            char* nameStr = BdatGetPtrDirect(mgr, "rlt_face", id);
            if (strcmp(nameStr, "") == 0) {
                goto next;
            }
            u32 texName =
                (u32)MakeTplNameSysFile(BdatGetU16Direct(mgr, "rlt_texture", id));
            void* pal = self->field08->getTex(texTag, texName, 0);
            nw4r::lyt::Picture* pic = NULL;
            if (pal != 0) {
                void* raw =
                    mtl::MemManager::allocate(0xf0, CLibLayout::getAllocHandle());
                if (raw != 0) {
                    nw4r::lyt::TexMap texMap(reinterpret_cast<TPLPalette*>(pal), 0);
                    pic = new (raw) nw4r::lyt::Picture(texMap);
                }
            }
            {
                f32 pairA[2] = { scale, scale };
                CMenuArtsSet_setPaneScale(reinterpret_cast<UnkKizunaFunc31848Obj*>(pic),
                              reinterpret_cast<const UnkKizunaPair*>(&pairA[0]));
                char name[0x40];
                sprintf(name, "%s", nameStr);
                pic->SetName(name);
                s16 px = (s16)BdatGetS16Direct(mgr, "rlt_posX", id);
                s16 py = (s16)BdatGetS16Direct(mgr, "rlt_posY", id);
                UnkKizunaVec3 v;
                v.x = (f32)px;
                v.y = (f32)py;
                v.z = posZ;
                copyVEC3((f32*)&((KizunaPaneTranslateView*)pic)->translate, &v.x);
                setPaneVisible(reinterpret_cast<nw4r::lyt::Pane*>(pic), 1);
                nw4r::lyt::Pane* parent = reinterpret_cast<nw4r::lyt::Pane*>(
                    self->field0C->field10->target((int)("face_icon"), 1));
                parent->AppendChild(reinterpret_cast<nw4r::lyt::Pane*>(pic));
            }
            if (tinted != 0) {
                PaneSetVtxColorAll(pic, tintColor);
            } else if (CtrlRemote_TouchBitByArg(0x20) >= 0x167) {
                int w = BdatGetSexFlag(id);
                if ((u8)(w + 0xfe) <= 1) {
                    PaneSetVtxColorAll(pic, tintColor);
                }
            }
        }
    next:
        sKizRowCursor = sKizRowCursor + 1;
    }
    return false;
}

// Update pass: while the +0x08 child is live, advance the anim at +0x0C unless
// the +0x10 byte suppresses it, then notify the child via vtable slot 14.
void tickCurAnim(UnkKizunaSelf57EE0* self) {
    if (self->field8) {
        if (self->field10 == 0) {
            advanceAnimTransform(self->field0C, lbl_eu_80668834);
        }
        self->field8->callSlot14(0);
    }
}

extern "C" void __ct__CKizunaCur(CKizunaCur* self,
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
extern "C" u16 findKizByName(UnkKizunaSelf5949C* self,
                                                  const char* name) {
    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(::cf::CfBdat::spFldNpcListFileData);
    u16 count = (u16)Bdat_GetMaxRow_B1EC(mgr);
    for (u16 id = 1; (u32)id <= count; id++) {
        if (strcmp(BdatGetPtrDirect(mgr, lbl_eu_8050CB20 + 0xca, id),
                   name) == 0 &&
            CtrlRemote_TouchBitByArg((u16)BdatGetU16Direct(mgr, lbl_eu_8050CB20 + 0xc1,
                                                       id) + 0xa20) != 0) {
            return id;
        }
    }
    return 0;
}

// retail: if (field26) { field34=1; field36=field26; field38=const }
// (same UnkKizunaSelfAB object the tickLineState dispatch passes to the
// SelIn/SelOut siblings - see the reinterpret_casts at the call sites.)
extern "C" void lineStartSelect(void* self) {
    UnkKizunaSelfAB* sel = (UnkKizunaSelfAB*)self;
    u16 v = sel->field26;
    if (v != 0) {
        sel->field34 = 1;
        sel->field36 = v;
        sel->field38 = lbl_eu_80668828;
    }
}

extern "C" void lineTickSelIn(UnkKizunaSelfAB* self) {
    f32 f = self->field38 + lbl_eu_80668834;
    self->field38 = f;
    if (f >= lbl_eu_80668860) {
        self->field34 = 2;
        self->field38 = lbl_eu_80668828;
        rewriteKizEntryText(self, 1);
    }
    if (self->field26 == 0) {
        self->field34 = 3;
        rewriteKizEntryText(self, 0);
    }
}

extern "C" void lineTickSelOut(UnkKizunaSelfAB* self) {
    f32 f = self->field38 + lbl_eu_80668834;
    self->field38 = f;
    if (f >= lbl_eu_80668864) {
        self->field34 = 1;
        self->field38 = lbl_eu_80668828;
        rewriteKizEntryText(self, 0);
    }
    if (self->field26 == 0) {
        self->field34 = 3;
        rewriteKizEntryText(self, 0);
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
void rewriteKizEntryText(UnkKizunaSelfAB* self, u32 a) {
    if (self->field36 == 0) {
        return;
    }
    if (a > 1) {
        return;
    }

    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(::cf::CfBdat::spFldNpcListFileData);
    char* curName = BdatGetPtrDirect(mgr, "rlt_face", self->field36);

    // 4-byte (3 chars + NUL) name copies compared as words
    char nameCur[4];
    copyKizTag3((unsigned char*)nameCur, (const unsigned char*)curName);

    void* fp = getFP("JNL_relatelist");
    u16 count = (u16)Bdat_GetMaxRow_B1EC(fp);

    // shared arc string table base (CSE'd into a register across the loop)
    char* str = "mf40_kiz_rdr.brlyt";

    // fourcc of the texture tag ('timg'), hoisted out of the loop by MWCC
    const int timg = 0x74696d67;

        char nameRow[4];
        char nameRowAlt[4];
        char fmtBuf[0x20];
        for (u32 i = 1; (u16)i <= count; i++) {
            char* rowName = BdatGetPtrDirect(fp, str + 0x411, i);
            copyKizTag3((unsigned char*)nameRow, (const unsigned char*)rowName);
            copyKizTag3((unsigned char*)nameRowAlt, (const unsigned char*)(rowName + 3));

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

        u8 state = (u8)CtrlRemote_TouchBitByArg(i + 0x608);
        if (state != 0) {
            // Per-state label: pick the format string from the row's tag name.
            // (the tag lookup is repeated in every arm, matching retail)
            if (state == 1) {
                char* label = BdatGetPtrDirect(fp, str + 0x417, i);
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
                char* label = BdatGetPtrDirect(fp, str + 0x417, i);
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
                char* label = BdatGetPtrDirect(fp, str + 0x417, i);
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
                char* label = BdatGetPtrDirect(fp, str + 0x417, i);
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
                char* label = BdatGetPtrDirect(fp, str + 0x417, i);
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
                PaneSetTexPalette(paneB, (TPLPalette*)((u32)tex));
            }
        }
    }
}

void copyKizTag3(unsigned char* dst, const unsigned char* src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = 0;
}

extern "C" void __ct__CKizunaInfo(CKizunaInfo* self,
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

// us-8025d8ac (0x8025B670): build the kizuna line layout + two anim transforms, bind
// the font to the root pane, publish the 4 font-format values and 6 localized labels,
// then reset the +0x18 counter.
extern "C" void buildInfoLayout(UnkKizunaSelfB670* self) {
    buildLayout(&self->field8, self->field4, lbl_eu_8050CB20 + 0xb19);
    bindLayoutAnimTransform(self->field8, &self->field0C, self->field4, lbl_eu_8050CB20 + 0xb2c);
    bindLayoutAnimTransform(self->field8, &self->field10, self->field4, lbl_eu_8050CB20 + 0xb42);
    nw4r::lyt::Pane* rootPane = ((UnkKizunaLayoutView*)self->field8)->field10;
    void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->field8);
    u32 fontResult = static_cast<UnkKizunaFontView*>(fontObj)->vf7();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);
    u32 val = (u32)CUICfManager_getPackedFont9C();
    setLayoutTextBoxFont(self->field8, lbl_eu_8050CB20 + 0xb5d, val);
    setLayoutTextBoxFont(self->field8, lbl_eu_8050CB20 + 0xb6b, val);
    setLayoutTextBoxFont(self->field8, lbl_eu_8050CB20 + 0xb79, val);
    setLayoutTextBoxFont(self->field8, lbl_eu_8050CB20 + 0xb87, val);
    self->field8->UnbindAllAnimation();
    LayoutSetTextBoxFmtValue(self->field8, lbl_eu_8050CB20 + 0xb95,
                  BdatTouchStringCell(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xb), 0);
    LayoutSetTextBoxFmtValue(self->field8, lbl_eu_8050CB20 + 0xb9f,
                  BdatTouchStringCell(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xc), 0);
    LayoutSetTextBoxFmtValue(self->field8, lbl_eu_8050CB20 + 0xba9,
                  BdatTouchStringCell(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xd), 0);
    LayoutSetTextBoxFmtValue(self->field8, lbl_eu_8050CB20 + 0xbb3,
                  BdatTouchStringCell(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xe), 0);
    LayoutSetTextBoxFmtValue(self->field8, lbl_eu_8050CB20 + 0xbbd,
                  BdatTouchStringCell(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0xf), 0);
    LayoutSetTextBoxFmtValue(self->field8, lbl_eu_8050CB20 + 0xbc7,
                  BdatTouchStringCell(lbl_eu_8050CB20 + 0x316, lbl_eu_8050CB20 + 0x321, 0x10), 0);
    self->field18 = 0xFFFF;
    refreshKizLineDisplay((UnkKizunaSelf57D90*)self, 0);
}

// Per-frame display dispatch: while the current-line child (+0x08) is live,
// run the mode-specific handler for the +0x14 mode byte, then notify the
// child via vtable slot 14 (retail: cmpwi chain over modes 1/2/4/5).
extern "C" void tickInfoMode(UnkKizunaSelfC21C* self) {
    if (self->field8) {
        switch (self->field14) {
        case 1:
            infoAnimToMode2(self);
            break;
        case 2:
            infoAnimToMode3(self);
            break;
        case 4:
            infoAnimToMode5(self);
            break;
        case 5:
            infoAnimToMode0(self);
            break;
        }
        self->field8->target14(0);
    }
}

// Finalize then release/null a +0x08 child: if non-null, finalize it (vtable
// slot 2) with a 1 flag, then clear the pointer.
// The nested identical guards reproduce retail's duplicated test: both beqs
// share the single loaded pointer (MWCC CSEs the load).
extern "C" void releaseInfoChild(UnkKizunaSelf57D90* self) {
    if (self->field8 != 0) {
        if (self->field8 != 0) {
            self->field8->target2(1);
        }
        self->field8 = 0;
    }
}

// Set display mode 1 for the current-line child: publish the frame value via
// vtable slots 7/11 of the child at +0x08, and clear the animation flags.
extern "C" void infoEnterMode1(UnkKizunaSelfB958* self) {
    self->field14 = 1;
    self->field15 = 0;
    self->field16 = 0;
    self->field8->slot7(self->field0C);
    self->field8->slot11(self->field0C, 1);
}

// Same shape as infoEnterMode1 but display mode 4 and value from +0x10.
extern "C" void infoEnterMode4(UnkKizunaSelfB958* self) {
    self->field14 = 4;
    self->field15 = 0;
    self->field16 = 0;
    self->field8->slot7(self->field10);
    self->field8->slot11(self->field10, 1);
}

// Kizuna-line display refresh for a selected kizuna id. Guards on the cached
// id at +0x18, then either resets all text panes to blanks/zero (v == 0) or
// rebinds every pane from the BDAT progress tables (name/color/heart counts).
extern "C" void refreshKizLineDisplay(UnkKizunaSelf57D90* selfArg,
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
        LayoutSetTextBoxFmtValue(self->field8, "txt_info07", "", 0);
        LayoutSetTextBoxFmtValue(self->field8, "txt_info01", "", 0);
        LayoutSetTextBoxFmtValue(self->field8, "txt_info04", "", 0);
        LayoutSetTextBoxFmtValue(self->field8, "txt_info05", "", 0);
        LayoutSetTextBoxFmtValue(self->field8, "txt_info06_00", "", 0);
        LayoutSetTextBoxFmtValue(self->field8, "txt_info06_01", "", 0);
        LayoutSetTextBoxFmtValue(self->field8, "txt_info02", "", 0);
        LayoutSetTextBoxFmtValue(self->field8, "txt_info03_00", "", 0);
        LayoutSetTextBoxFmtValue(self->field8, "txt_info03_03", "", 0);
        LayoutSetTextBoxFmtValue(self->field8, "txt_info03_02", "", 0);
        // Hide the root pane fetched through slot 15 of the layout child.
        setPaneVisible(reinterpret_cast<nw4r::lyt::Pane*>(
                          reinterpret_cast<UnkKizunaLayoutSub57AFC*>(self->field8)
                              ->field10->slot15("pic_npcicon", 1)),
                      0);
        return;
    }

    UnkKizunaMgr* mgr = reinterpret_cast<UnkKizunaMgr*>(::cf::CfBdat::spFldNpcListFileData);

    // Progress-column color selector: look up the row value then map it to a
    // color index; the low byte picks which timg texture tag to bind.
    u16 progRow = BdatGetU16Direct(mgr, "rlt_lnd", v);
    u8 colIdx = BdatGetU8ByTableKey("landmarklist", "mapID", progRow);
    char* texName = NULL;
    switch (MapValueToRank6(colIdx)) {
    case 1:
        texName = ((UnkKizunaAccBA38*)CUICfManager_getArcResourceAccessor())
                      ->getTex(0x74696d67, "mf00_com12_face_area01.tpl", 0);
        break;
    case 2:
        texName = ((UnkKizunaAccBA38*)CUICfManager_getArcResourceAccessor())
                      ->getTex(0x74696d67, "mf00_com12_face_area02.tpl", 0);
        break;
    case 3:
        texName = ((UnkKizunaAccBA38*)CUICfManager_getArcResourceAccessor())
                      ->getTex(0x74696d67, "mf00_com12_face_area03.tpl", 0);
        break;
    case 4:
        texName = ((UnkKizunaAccBA38*)CUICfManager_getArcResourceAccessor())
                      ->getTex(0x74696d67, "mf00_com12_face_area04.tpl", 0);
        break;
    case 5:
        texName = ((UnkKizunaAccBA38*)CUICfManager_getArcResourceAccessor())
                      ->getTex(0x74696d67, "mf00_com12_face_area05.tpl", 0);
        break;
    }
    if (texName != NULL) {
        // Bind the color texture and show the tinted root pane.
        PaneSetTexPaletteByName(self->field8, "pic_npcicon", (TPLPalette*)texName);
        setPaneVisible(
            reinterpret_cast<nw4r::lyt::Pane*>(
                reinterpret_cast<UnkKizunaLayoutSub57AFC*>(self->field8)
                    ->field10->slot15("pic_npcicon", 1)),
            1);
    }

    // Progress color name for this save column and the kizuna's own color.
    LayoutSetTextBoxFmtValue(self->field8, "txt_info07",
                  BdatGetPtrDirect(::cf::CfBdat::spFldMapListFileData, "name", colIdx), 0);
    LayoutSetTextBoxFmtValue(self->field8, "txt_info01",
                  BdatGetPtrDirect(mgr, "name", v), 0);

    // Bond-stage label: stage indexes a per-language label table; stages above
    // 10 clamp to the first label.
    char* stageLabel;
    switch ((u8)BdatGetSexFlag(v)) {
    case 0:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x16);
        break;
    case 1:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x17);
        break;
    case 2:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x18);
        break;
    case 3:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x19);
        break;
    case 4:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x1a);
        break;
    case 5:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x1b);
        break;
    case 6:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x1c);
        break;
    case 7:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x1d);
        break;
    case 8:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x1e);
        break;
    case 9:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x18);
        break;
    case 10:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x19);
        break;
    default:
        stageLabel = BdatTouchStringCell("MNU_relate", "name", 0x16);
        break;
    }
    LayoutSetTextBoxFmtValue(self->field8, "txt_info04", stageLabel, 0);

    // Heart-level icon name from the heart-state table (0 hides the icon).
    char* heartLabel;
    switch ((u8)MapSexFlagGroup(v)) {
    case 0:
        heartLabel = NULL;
        break;
    case 1:
        heartLabel = BdatTouchStringCell("MNU_relate", "name", 0x20);
        break;
    case 2:
        heartLabel = BdatTouchStringCell("MNU_relate", "name", 0x21);
        break;
    case 3:
        heartLabel = BdatTouchStringCell("MNU_relate", "name", 0x22);
        break;
    case 4:
        heartLabel = BdatTouchStringCell("MNU_relate", "name", 0x23);
        break;
    case 5:
        heartLabel = BdatTouchStringCell("MNU_relate", "name", 0x24);
        break;
    }
    LayoutSetTextBoxFmtValue(self->field8, "txt_info05", heartLabel, 0);

    // Encounter counter, both names, formatted "current/max" heart counts.
    setLayoutTextBoxNumber(
        self->field8, "txt_info06_00",
        (u8)BdatGetU16Direct(mgr, "rlt_age", v));
    LayoutSetTextBoxFmtValue(self->field8, "txt_info06_01",
                  BdatTouchStringCell("MNU_relate", "name", 0x15), 0);
    LayoutSetTextBoxFmtValue(self->field8, "txt_info02",
                  BdatGetPtrDirect(mgr, "rlt_job", v), 0);
    u8 heartsCur = BdatGetU8Direct((const void*)mgr, "%02d:00", v);
    u8 heartsMax = BdatGetU8Direct((const void*)mgr, "range_e", v);
    char buf[0x14];
    sprintf(buf, "%02d:00", heartsCur);
    LayoutSetTextBoxText(self->field8, "txt_info03_00", buf, 0);
    sprintf(buf, "%02d:00", heartsMax);
    LayoutSetTextBoxText(self->field8, "txt_info03_03", buf, 0);
    LayoutSetTextBoxFmtValue(self->field8, "txt_info03_02",
                  BdatTouchStringCell("MNU_relate", "name", 0x14), 0);
}

// Mode-2 update: when the +0x0C anim transform reaches its last frame,
// switch the mode byte to 2, set the +0x16 flag, and publish state through
// the child at +0x08 (slots 14/8 with the transform id, slots 7/11 with
// the +0x10 transform and a 1 flag).
extern "C" void infoAnimToMode2(UnkKizunaSelfC21C* self) {
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->field0C, lbl_eu_80668834) != 0) {
        self->field14 = 2;
        self->field16 = 1;
        self->field8->target14(0);
        self->field8->target8(self->field0C);
        self->field8->slot7((u32)self->field10);
        self->field8->slot11((u32)self->field10, 1);
    }
}

extern "C" void infoAnimToMode3(UnkKizunaSelfC21C* self) {
    if (advanceAnimTransform(self->field10, lbl_eu_80668834) != 0) {
        // anim reached its last frame: switch to mode 3, publish via the child.
        self->field14 = 3;
        self->field15 = 1;
        self->field8->target14(0);
        self->field8->target8((u32)self->field10);
    }
}

// Mode-4 update: same shape as infoAnimToMode2 but mode byte 5 and the
// +0x10 transform published through slots 8/7/11.
extern "C" void infoAnimToMode5(UnkKizunaSelfC21C* self) {
    if (AnimRewindFrame(self->field10, lbl_eu_80668834) != 0) {
        // anim reached its last frame: switch to mode 5, publish via the child.
        self->field14 = 5;
        self->field16 = 1;
        self->field8->target14(0);
        self->field8->target8((u32)self->field10);
        self->field8->slot7(self->field0C);
        self->field8->slot11(self->field0C, 1);
    }
}

// infoAnimToMode0 (us-8025e494): kizuna-line update gate. Retail: reads the
// global float lbl_eu_80668834 (sdata), calls AnimRewindFrame([self+0xC], f1)
// (declared CArtsInfo.hpp, defined code_80135FDC.cpp -- the old comment's
// "no split symbols" blocker is stale); on non-zero return: field14=0,
// field15=1, then [self+8] virtual slot 56 (target14) twice with arg 0,
// then slot 32 (target8) with arg [self+0xC].
extern "C" void infoAnimToMode0(UnkKizunaSelfC21C* self) {
    if (AnimRewindFrame((nw4r::lyt::AnimTransform*)self->field0C, lbl_eu_80668834) != 0) {
        self->field14 = 0;
        self->field15 = 1;
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
void loadKizBindFiles(UnkKizunaSelfC510* self) {
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
void drawKizLayouts(UnkKizunaSelfC61C* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field74 != 0) {
        drawLayout(self->field74, drawInfo, 0, 1);
    }
    if (self->field38 != 0) {
        if (self->field39 == 3 || self->field39 == 6) {
            if (self->fieldDD != 0 && self->field8E != 0 && self->field54 != 0) {
                drawLayout(self->field54, drawInfo, 0, 1);
            }
            if (self->fieldB4 != 0) {
                drawLayout(self->fieldB4, drawInfo, 0, 1);
            }
            if (self->fieldC8 != 0) {
                drawLayout(self->fieldC8, drawInfo, 0, 1);
            }
        }
    }
}

// Destruction: release both file handles, clear the anim flag byte, then
// release each child object (0x4C / 0x68 / 0xAC / 0xC0) and the two resource
// accessors, and tear down the two memory regions at +0x08 / +0x18.
void teardownKizuna(UnkKizunaSelfC6F0* self) {
    closeFileHandle(&self->field28);
    closeFileHandle(&self->field2C);
    self->field38 = 0;
    releaseInfoChild(&self->sub4C);
    freeLinePanes((UnkKizunaSelf59B18*)&self->sub68);
    releaseCurChild(&self->subAC);
    releaseRadarChild(&self->subC0);
    releaseArcResourceAccessor(self->field30);
    releaseArcResourceAccessor(self->field34);
    self->mRegA.deleteRegion();
    self->mRegB.deleteRegion();
}

extern "C" unsigned char kizChartReady(u8* this_ptr) {
    KizChartState* st = (KizChartState*)this_ptr;
    if (st->f7E != 0) {
        return st->f3B;
    } else {
        return 0;
    }
}

extern "C" unsigned char kizChartOpen(u8* arg1)
{
    KizChartState* st = (KizChartState*)arg1;
    if (st->f61 == 0)
        return 0;
    if (st->f7D == 0)
        return 0;
    if (st->fBD != 0)
        return st->f3C;
    return 0;
}

extern "C" void kizStartChart(UnkKizunaSelfC7FC* self) {
    if (self->field39 != 0) return;
    self->field39 = 1;
    self->field3C = 0;
    ((u8*)self)[0x38] = 1;
    infoEnterMode1(&self->sub4C);
}

void kizCloseChart(UnkKizunaSelfC7FC* self, int arg4) {
    if (self->field39 == 3) {
        // switch to mode 4, reset the sub-flag, re-publish all children.
        self->field39 = 4;
        self->field3C = 0;
        infoEnterMode4(&self->sub4C);
        lineEnterMode3(&self->sub68);
        setCurPanePhase(&self->subAC, 0xff);
        if (arg4 != 0) {
            playUISound(6);
        }
    }
}


// Build the line layout from the shared arc string at +0x4C, then copy the
// animated position at +0x4C of the slot-15 result into the +0x14 Vec2.
// Each display reset: place a fixed color into the +0x68 sub-object, clear
// its state byte, reset the two line panes, and step the sub-anim.
extern "C" void kizCursorDir1(UnkKizunaSelfC874* self) {
    // Array member forces a block copy on assignment (retail's interleaved
    // lwz/stw shape); last-declared tmp sits at sp+8, v spans sp+0x14+.
    struct VecA { u32 a[3]; };
    VecA v;
    VecA tmp;
    VecA* src = reinterpret_cast<VecA*>(code80135FDC_setVec3(
        (nw4r::math::VEC3*)(reinterpret_cast<float*>(&tmp.a[0])), lbl_eu_80668828,
        lbl_eu_80668868, lbl_eu_80668828));
    v = *src;
    placeKizMarker(reinterpret_cast<UnkKizunaSelf59394*>(&self->sub68),
                  reinterpret_cast<UnkKizunaVec3*>(&v));
    tickKizMarker(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68));
    self->field3A = 0;
    setCurPanePhase(&self->subAC, 0);
    refreshKizLineDisplay(&self->sub4C, self->field8E);
    refreshKizEntryPanes(reinterpret_cast<UnkKizunaSelf9820*>(&self->sub68));
}

// Same display reset as kizCursorDir1 but with the second color constant.
extern "C" void kizCursorDir2(UnkKizunaSelfC874* self) {
    // Array member forces a block copy on assignment (retail's interleaved
    // lwz/stw shape); last-declared tmp sits at sp+8, v spans sp+0x14+.
    struct VecA { u32 a[3]; };
    VecA v;
    VecA tmp;
    VecA* src = reinterpret_cast<VecA*>(code80135FDC_setVec3(
        (nw4r::math::VEC3*)(reinterpret_cast<float*>(&tmp.a[0])), lbl_eu_80668828,
        lbl_eu_8066886C, lbl_eu_80668828));
    v = *src;
    placeKizMarker(reinterpret_cast<UnkKizunaSelf59394*>(&self->sub68),
                  reinterpret_cast<UnkKizunaVec3*>(&v));
    tickKizMarker(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68));
    self->field3A = 0;
    setCurPanePhase(&self->subAC, 0);
    refreshKizLineDisplay(&self->sub4C, self->field8E);
    refreshKizEntryPanes(reinterpret_cast<UnkKizunaSelf9820*>(&self->sub68));
}

// Same display reset as kizCursorDir1 but with the color constants swapped
// (retail loads f2 first then fmr f3, so the source order is x/y/z with the
// shared constant in y/z).
extern "C" void kizCursorDir3(UnkKizunaSelfC874* self) {
    // u32-array view forces retail's word-copy shape; declaring v first puts
    // it at sp+0x14 and the setVec3 temp at sp+8, matching retail's layout.
    struct VecW { u32 w[3]; };
    VecW v;
    VecW tmp;
    v = *reinterpret_cast<VecW*>(code80135FDC_setVec3(
        (nw4r::math::VEC3*)(reinterpret_cast<f32*>(&tmp.w[0])), lbl_eu_8066886C,
        lbl_eu_80668828, lbl_eu_80668828));
    placeKizMarker(reinterpret_cast<UnkKizunaSelf59394*>(&self->sub68),
                  reinterpret_cast<UnkKizunaVec3*>(&v));
    tickKizMarker(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68));
    self->field3A = 0;
    setCurPanePhase(&self->subAC, 0);
    refreshKizLineDisplay(&self->sub4C, self->field8E);
    refreshKizEntryPanes(reinterpret_cast<UnkKizunaSelf9820*>(&self->sub68));
}

// Dispatch on display-state byte at +0x3A; each case is a tail call.
// Same display reset as kizCursorDir1 but with the color constants swapped
// (retail loads f2 first then fmr f3, so the source order is x/y/z with the
// shared constant in y/z).
extern "C" void kizCursorDir4(UnkKizunaSelfC874* self) {
    // u32-array view forces retail's word-copy shape (lwz/stw, not lfs/stfs);
    // declaring v first puts it at sp+0x14 and the setVec3 temp at sp+8
    // (matches retail's slot assignment; see kizCursorDir3)
    struct VecW { u32 w[3]; };
    VecW v;
    VecW tmp;
    v = *reinterpret_cast<VecW*>(code80135FDC_setVec3(
        (nw4r::math::VEC3*)(reinterpret_cast<f32*>(&tmp.w[0])), lbl_eu_80668868,
        lbl_eu_80668828, lbl_eu_80668828));
    placeKizMarker(reinterpret_cast<UnkKizunaSelf59394*>(&self->sub68),
                  reinterpret_cast<UnkKizunaVec3*>(&v));
    tickKizMarker(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68));
    self->field3A = 0;
    setCurPanePhase(&self->subAC, 0);
    refreshKizLineDisplay(&self->sub4C, self->field8E);
    refreshKizEntryPanes(reinterpret_cast<UnkKizunaSelf9820*>(&self->sub68));
}

void tickKizMove(UnkKizunaDisp* self) {
    switch (self->field_0x3A) {
    case 0:
        return publishKizScaledVals((UnkKizunaSelfCE00*)self);
    case 1:
        return slideKizDown((UnkKizunaSelfCE78*)self);
    case 2:
        return tickCurByFlag(self);
    }
}

// Pulse the anim at +0x78 to frame 1.0, notify the child object (+0x74, via
// the embedded getLinePanePair sub at +0x68) on vtable slot 14, rebuild the
// two-word pair, and copy it into the +0xB4 object's child.
void kizCursorHoldA(UnkKizunaSelfCAE4* self) {
    advanceAnimTransform(self->field78, lbl_eu_80668834);
    ((UnkKizunaObjSlot14*)self->sub.field0C)->callSlot14(0);
    UnkKizunaPair p = getLinePanePair(&self->sub);
    CMenuArtsSet_setPaneScale(self->fieldB4->field10, &p);
}

// Same shape as kizCursorHoldA but the anim step uses AnimRewindFrame instead.
void kizCursorHoldB(UnkKizunaSelfCAE4* self) {
    AnimRewindFrame(self->field78, lbl_eu_80668834);
    ((UnkKizunaObjSlot14*)self->sub.field0C)->callSlot14(0);
    UnkKizunaPair p = getLinePanePair(&self->sub);
    CMenuArtsSet_setPaneScale(self->fieldB4->field10, &p);
}

extern "C" int kizChartBusy(void* self) { return 0; }

struct CKizunagramState {
    u8 _00[0x8C];
    u8 field8C;
};

extern "C" u8 kizHasSelFlag(CKizunagramState* self) {
    return self->field8C;
}


// retail: lfs f0,const; li r0,0; stb 0x34; sth 0x36; stfs 0x38
extern "C" void lineClearSelect(void* self) {
    UnkKizunaSelfAB* sel = (UnkKizunaSelfAB*)self;
    sel->field34 = 0;
    sel->field36 = 0;
    sel->field38 = lbl_eu_80668828;
}

// retail: lbz 0xdd; xori; subic; subfe; stb - toggle (x^1)!=0 under -O4,s
extern "C" void kizToggleHelp(void* self) {
    *(u8*)((char*)self + 0xDD) = (u8)((*(u8*)((char*)self + 0xDD) ^ 1) != 0);
}

// retail: if (field_62) { field_39 = 2; tail lineEnterMode1(self+0x68) }
// attribute MWCC folds their bodies into tickKizMain's switch arms.
extern "C" void tryKizMode2(void* self) {
    if (*(u8*)((char*)self + 0x62) != 0) {
        *(u8*)((char*)self + 0x39) = 2;
        lineEnterMode1((UnkKizunaSelf59228*)((char*)self + 0x68));
    }
}

// Gate: when both the +0x61 and +0x7D bytes are set, raise the +0x39/+0x3C
// state and tail-call the +0xAC sub-object with the +0x8C flag.
extern "C" void tryKizMode3(void* self) {
    if (*(u8*)((u8*)self + 0x61) == 0)
        return;
    if (*(u8*)((u8*)self + 0x7D) == 0)
        return;
    *((u8*)self + 0x39) = 3;
    *((u8*)self + 0x3C) = 1;
    if (*(u8*)((u8*)self + 0x8C) != 0)
        setCurPanePhase((UnkKizunaSelf57D90*)((u8*)self + 0xAC), 1);
    else
        setCurPanePhase((UnkKizunaSelf57D90*)((u8*)self + 0xAC), 0);
}

void tryKizMode5(u8* ptr) {
    if (ptr[0x62] != 0) {
        ptr[0x39] = 5;
    }
}

void tryKizIdle(u8* b) {
    if (!b[0x61]) return;
    if (!b[0x7d]) return;
    b[0x39] = 0;
    b[0x3c] = 1;
    b[0x38] = 0;
}

void tickKizLine(void* self) {}

// If the sub-state at +0x68 reports active, scale the stored int bit patterns
// at +0x80..0x88 down by the constant and publish the results. The u32 words
// are routed through a stack array so MWCC emits the retail lwz/stw/lfs
// bit-cast round-trips for +0x88 without consuming its result.
extern "C" void publishKizScaledVals(UnkKizunaSelfCE00* self) {
    if (tickKizMarker(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68)) != 0) {
        u32 raw[3];
        raw[0] = self->field80;
        raw[1] = self->field84;
        f32 denom = 3.0f;
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
extern "C" void slideKizDown(UnkKizunaSelfCE78* self) {
    // u32 words force retail's lwz/stw struct copy of the setVec3 result
    // (see kizCursorDir4); v lands at sp+0x14 and the setVec3 temp at sp+8.
    struct VecW { u32 w[3]; };
    VecW v;
    VecW tmp;
    v = *reinterpret_cast<VecW*>(code80135FDC_setVec3(
        (nw4r::math::VEC3*)(reinterpret_cast<f32*>(&tmp.w[0])), self->field40, self->field44,
        lbl_eu_80668828));
    placeKizMarker(reinterpret_cast<UnkKizunaSelf59394*>(&self->sub68),
                  reinterpret_cast<UnkKizunaVec3*>(&v));
    f32 f = self->field48 - lbl_eu_80668834;
    self->field48 = f;
    if (f <= lbl_eu_80668828) {
        self->field3A = 2;
        tickKizMarker(reinterpret_cast<UnkKizunaSelf5949C*>(&self->sub68));
        refreshKizLineDisplay(&self->sub4C, self->field8E);
        refreshKizEntryPanes(reinterpret_cast<UnkKizunaSelf9820*>(&self->sub68));
    }
}

// us-8025f08c (0x8025CF40): when both +0x30/+0x34 pointers are present, build a fresh
// CKizunaLine, copy it into the +0x68 line state (field-by-field, including the
// +0x18..+0x20 pad words the ctor leaves uninitialized), then rebuild the line display
// and re-fetch the two cur/line panes via slot 15.
extern "C" void rebuildKizLine(UnkKizunaSelfCF40* self) {
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
        buildKizLineLayout((UnkKizunaLineBuild*)&self->lineState);
        self->fieldCC =
            (u32)((UnkKizunaMid59344*)self->lineState.field0C)->field10->target(
                (int)(lbl_eu_8050CB20 + 0xa4), 1);
        self->fieldD0 =
            (u32)((UnkKizunaMid59344*)self->lineState.field0C)->field10->target(
                (int)(lbl_eu_8050CB20 + 0x8b), 1);
}

// File-load completion callback for the kizuna chart: whichever bind file finished
// (line-data handle at +0x28 or current-kizuna handle at +0x2C) gets its buffer
// detached and re-attached into a fresh ArcResourceAccessor built under a scratch MEM2
// region (RAII Class_8045F858 guard), then the matching sub-display is rebuilt.
// Sub-objects are constructed into stack temps and copied member-wise into place (the
// manual vtable word is not part of the copy).

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
        mtl::MemManager::setMemInitFlag(false);
        // Store the accessor straight into the field and re-load it from there
        // at each use - retail never keeps it live in a register across calls.
        field30 = (u32)CLibLayout::createArcResourceAccessor();
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
        buildInfoLayout((UnkKizunaSelfB670*)&mInfo);

        // Current-kizuna cursor: build temp, copy body, then init + publish.
        __ct__CKizunaCur((CKizunaCur*)curBuf,
                         (nw4r::lyt::ArcResourceAccessor*)field30);
        mCur.mArcResAcc = ((CKizunaCur*)curBuf)->mArcResAcc;
        mCur.mField08 = ((CKizunaCur*)curBuf)->mField08;
        mCur.mField0C = ((CKizunaCur*)curBuf)->mField0C;
        mCur.mField10 = ((CKizunaCur*)curBuf)->mField10;
        mCur.mField11 = ((CKizunaCur*)curBuf)->mField11;
        mCur.mField12 = ((CKizunaCur*)curBuf)->mField12;
        buildCurLayout((UnkKizunaSelf57E58*)&mCur);
        setCurPanePhase((UnkKizunaSelf57D90*)&mCur, 0xFF);

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
        buildRadarLayout((UnkKizunaSelfAFC*)&mRadar);

        if (CtrlObjectParam_GetWorkTailValue() == 0 || fieldDE != 0) {
            fieldDC = 4;
        }
        rebuildKizLine((UnkKizunaSelfCF40*)this);
        field28 = 0;
        mMemRegionA.validateHeap();
        return true;
    }
    if (field2C == (u32)pEventFile->mFileHandle) {
        mMemRegionB.createRegion((int)mtl::MemManager::getHandleMEM2(), 0x100,
                                 lbl_eu_8050CB20 + 0xd17, 0);
        // Retail quirk: the branch-2 guard is still constructed against
        // mMemRegionA (r31+8), not the region just created.
        Class_8045F858 host(&mMemRegionA);
        void* fileData = ((CFileHandle*)field2C)->getData();
        mtl::MemManager::setMemInitFlag(false);
        field34 = (u32)CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)field34)->Attach(fileData,
                                                           lbl_eu_8050CB20 + 0xd13);
        rebuildKizLine((UnkKizunaSelfCF40*)this);
        field2C = 0;
        mMemRegionB.validateHeap();
        return true;
    }
    return false;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// sinit_8025D304: init the 14-color kizuna line palette via setGXColorS10.
void sinit_8025D304() {
    setGXColorS10(&kizLineTevColorRank01First, 0x21, 0x1c, 0x15, 0x0);
    setGXColorS10(&kizLineTevColorRank01Second, 0xb5, 0xb1, 0xab, 0xff);
    setGXColorS10(&kizLineTevColorRank2First, 0x21, 0x1e, 0x15, 0x0);
    setGXColorS10(&kizLineTevColorRank2Second, 0xb5, 0xb1, 0xab, 0xff);
    setGXColorS10(&kizLineTevColorRank3First, 0x17, 0x21, 0x15, 0x0);
    setGXColorS10(&kizLineTevColorRank3Second, 0xb3, 0xaf, 0x97, 0xff);
    setGXColorS10(&kizLineTevColorRank4First, 0x1c, 0x24, 0x2d, 0x0);
    setGXColorS10(&kizLineTevColorRank4Second, 0xb5, 0xb1, 0xab, 0xff);
    setGXColorS10(&kizLineTevColorRank5First, 0x1c, 0x20, 0x31, 0x0);
    setGXColorS10(&kizLineTevColorRank5Second, 0xb5, 0xb1, 0xab, 0xff);
    setGXColorS10(&kizLineTevColorRegion19First, 0x1c, 0x2c, 0x1a, 0x0);
    setGXColorS10(&kizLineTevColorRegion19Second, 0xe5, 0xea, 0xde, 0xff);
    setGXColorS10(&kizLineTevColorRegion1AFirst, 0x21, 0x1e, 0x15, 0x0);
    setGXColorS10(&kizLineTevColorRegion1ASecond, 0xb5, 0xb1, 0xab, 0xff);
}

// Position the kizuna line panes: scale the +0x14 Vec2 by (const - the +0x10
// pane's scale) into the "line" pane's size, then scale the +0x0C pane's
// translate by the ratio of the two reference-pane size deltas into the line
// pane's translate. Finally pulse the layout anim.
void layoutKizLinePanes(UnkKizunaSelf57B6C* self) {
    if (self->field8 != 0) {
        if (self->field10 != 0) {
            nw4r::math::VEC2 sc = self->field10->GetScale();
            f32 sx = lbl_eu_8066882C - sc.x;
            f32 sy = lbl_eu_8066882C - sc.y;
            f32 tmp[2];
            TagCopyVec2f(tmp, self->field14);
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
            TagCopyVec2f(sa, const_cast<f32*>(&paneA->GetSize().width));
            nw4r::lyt::Pane* paneB = self->field8->GetRootPane()->FindPaneByName(
                strbase + 0x1d, 1);
            f32 sb[2];
            TagCopyVec2f(sb, const_cast<f32*>(&paneB->GetSize().width));
            f32 sc2[2];
            TagCopyVec2f(sc2, const_cast<f32*>(&c->GetSize().width));
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
// shared row cursor sKizRevealCursor. For each row, resolve its named pane and
// toggle its visibility per unlock state; unlocked rows get their texture tag
// rewritten from per-state string tables, then the texture is bound to the
// pane. Returns true once the cursor passes the table row count.
extern "C" bool revealKizEntries(UnkKizunaSelfA11C* self) {
    // declaration/statement order mirrors retail's register assignment
    // (self=r28, str=r26, fp=r31, count=r30, i=r29, tag-const=r27)
    char buf[0x24];

    Bdat_GetTable_AA34();
    const char* str = lbl_eu_8050CB20;
    void* fp = getFP("JNL_relatelist");
    u32 count = Bdat_GetMaxRow_B1EC(fp) & 0xffff;

    for (u32 i = 0; i < 0x10; i++) {
        u16 cnt = sKizRevealCursor;
        if ((u32)(u16)cnt >= count) {
            return true;
        }
        u32 id = ((u16)cnt + 1) & 0xffff;
        char* rowName = BdatGetPtrDirect(fp, "pain1", id);
        nw4r::lyt::Pane* pane =
            (nw4r::lyt::Pane*)self->field0C->field10->target((int)rowName, 1);
        u32 state = CtrlRemote_TouchBitByArg(id + 0x608) & 0xff;
        if (pane != NULL) {
            setPaneVisible(pane, state != 0);
        }
        if (state == 0) {
            // still locked: just advance the cursor
            sKizRevealCursor = sKizRevealCursor + 1;
            continue;
        }

        // Per-state tag: pick the formatted texture name from this row's tag.
        char* tag = BdatGetPtrDirect(fp, "pain_txt1", id);
        switch (state) {
        case 1:
            if (strcmp(tag, "txt_relate07") == 0) {
                sprintf(buf, "mf40_kiz_000line_e00.tpl");
            } else if (strcmp(tag, "txt_relate13") == 0) {
                sprintf(buf, "mf40_kiz_000line_e00.tpl");
            } else if (strcmp(tag, "txt_relate05") == 0) {
                sprintf(buf, "mf40_kiz_018line_e00.tpl");
            } else if (strcmp(tag, "txt_relate03") == 0) {
                sprintf(buf, "mf40_kiz_045line_e00.tpl");
            } else if (strcmp(tag, "txt_relate16") == 0) {
                sprintf(buf, "mf40_kiz_045line_e00.tpl");
            } else if (strcmp(tag, "txt_relate01") == 0) {
                sprintf(buf, "mf40_kiz_090line_e00.tpl");
            } else if (strcmp(tag, "txt_relate02") == 0) {
                sprintf(buf, "mf40_kiz_135line_e00.tpl");
            } else if (strcmp(tag, "txt_relate15") == 0) {
                sprintf(buf, "mf40_kiz_135line_e00.tpl");
            } else if (strcmp(tag, "txt_relate04") == 0) {
                sprintf(buf, "mf40_kiz_162line_e00.tpl");
            } else {
                sprintf(buf, "none");
            }
            break;
        case 2:
            if (strcmp(tag, "txt_relate07") == 0) {
                sprintf(buf, "mf40_kiz_000line_d00.tpl");
            } else if (strcmp(tag, "txt_relate13") == 0) {
                sprintf(buf, "mf40_kiz_000line_d00.tpl");
            } else if (strcmp(tag, "txt_relate05") == 0) {
                sprintf(buf, "mf40_kiz_018line_d00.tpl");
            } else if (strcmp(tag, "txt_relate03") == 0) {
                sprintf(buf, "mf40_kiz_045line_d00.tpl");
            } else if (strcmp(tag, "txt_relate16") == 0) {
                sprintf(buf, "mf40_kiz_045line_d00.tpl");
            } else if (strcmp(tag, "txt_relate01") == 0) {
                sprintf(buf, "mf40_kiz_090line_d00.tpl");
            } else if (strcmp(tag, "txt_relate02") == 0) {
                sprintf(buf, "mf40_kiz_135line_d00.tpl");
            } else if (strcmp(tag, "txt_relate15") == 0) {
                sprintf(buf, "mf40_kiz_135line_d00.tpl");
            } else if (strcmp(tag, "txt_relate04") == 0) {
                sprintf(buf, "mf40_kiz_162line_d00.tpl");
            } else {
                sprintf(buf, "none");
            }
            break;
        case 3:
            if (strcmp(tag, "txt_relate07") == 0) {
                sprintf(buf, "mf40_kiz_000line_c00.tpl");
            } else if (strcmp(tag, "txt_relate13") == 0) {
                sprintf(buf, "mf40_kiz_000line_c00.tpl");
            } else if (strcmp(tag, "txt_relate05") == 0) {
                sprintf(buf, "mf40_kiz_018line_c00.tpl");
            } else if (strcmp(tag, "txt_relate03") == 0) {
                sprintf(buf, "mf40_kiz_045line_c00.tpl");
            } else if (strcmp(tag, "txt_relate16") == 0) {
                sprintf(buf, "mf40_kiz_045line_c00.tpl");
            } else if (strcmp(tag, "txt_relate01") == 0) {
                sprintf(buf, "mf40_kiz_090line_c00.tpl");
            } else if (strcmp(tag, "txt_relate02") == 0) {
                sprintf(buf, "mf40_kiz_135line_c00.tpl");
            } else if (strcmp(tag, "txt_relate15") == 0) {
                sprintf(buf, "mf40_kiz_135line_c00.tpl");
            } else if (strcmp(tag, "txt_relate04") == 0) {
                sprintf(buf, "mf40_kiz_162line_c00.tpl");
            } else {
                sprintf(buf, "none");
            }
            break;
        case 4:
            if (strcmp(tag, "txt_relate07") == 0) {
                sprintf(buf, "mf40_kiz_000line_b00.tpl");
            } else if (strcmp(tag, "txt_relate13") == 0) {
                sprintf(buf, "mf40_kiz_000line_b00.tpl");
            } else if (strcmp(tag, "txt_relate05") == 0) {
                sprintf(buf, "mf40_kiz_018line_b00.tpl");
            } else if (strcmp(tag, "txt_relate03") == 0) {
                sprintf(buf, "mf40_kiz_045line_b00.tpl");
            } else if (strcmp(tag, "txt_relate16") == 0) {
                sprintf(buf, "mf40_kiz_045line_b00.tpl");
            } else if (strcmp(tag, "txt_relate01") == 0) {
                sprintf(buf, "mf40_kiz_090line_b00.tpl");
            } else if (strcmp(tag, "txt_relate02") == 0) {
                sprintf(buf, "mf40_kiz_135line_b00.tpl");
            } else if (strcmp(tag, "txt_relate15") == 0) {
                sprintf(buf, "mf40_kiz_135line_b00.tpl");
            } else if (strcmp(tag, "txt_relate04") == 0) {
                sprintf(buf, "mf40_kiz_162line_b00.tpl");
            } else {
                sprintf(buf, "none");
            }
            break;
        case 5:
            if (strcmp(tag, "txt_relate07") == 0) {
                sprintf(buf, "mf40_kiz_000line_a00.tpl");
            } else if (strcmp(tag, "txt_relate13") == 0) {
                sprintf(buf, "mf40_kiz_000line_a00.tpl");
            } else if (strcmp(tag, "txt_relate05") == 0) {
                sprintf(buf, "mf40_kiz_018line_a00.tpl");
            } else if (strcmp(tag, "txt_relate03") == 0) {
                sprintf(buf, "mf40_kiz_045line_a00.tpl");
            } else if (strcmp(tag, "txt_relate16") == 0) {
                sprintf(buf, "mf40_kiz_045line_a00.tpl");
            } else if (strcmp(tag, "txt_relate01") == 0) {
                sprintf(buf, "mf40_kiz_090line_a00.tpl");
            } else if (strcmp(tag, "txt_relate02") == 0) {
                sprintf(buf, "mf40_kiz_135line_a00.tpl");
            } else if (strcmp(tag, "txt_relate15") == 0) {
                sprintf(buf, "mf40_kiz_135line_a00.tpl");
            } else if (strcmp(tag, "txt_relate04") == 0) {
                sprintf(buf, "mf40_kiz_162line_a00.tpl");
            } else {
                sprintf(buf, "none");
            }
            break;
        }

        if (pane != NULL) {
            void* tex = self->texSrc->getTex(0x74696D67, (u32)buf, 0);
            if (tex != 0) {
                PaneSetTexPalette(pane, (TPLPalette*)((u32)tex));
            }
        }
        sKizRevealCursor = sKizRevealCursor + 1;
    }
    return false;
}

// Per-frame update: dispatch on the mode byte at +0x39, then step every child
// (line display, kizuna sub, cur layout, line panes).
void tickKizMain(UnkKizunaSelfC580* self) {
    switch (self->field39) {
    case 1:
        tryKizMode2(self);
        break;
    case 2:
        tryKizMode3(self);
        break;
    case 4:
        tryKizMode5((u8*)self);
        break;
    case 5:
        tryKizIdle((u8*)self);
        break;
    case 6:
        tickKizLine(self);
        break;
    }
    tickInfoMode(&self->sub4C);
    tickLineState(reinterpret_cast<UnkKizunaSelf58F9C*>(&self->sub68));
    tickCurAnim(&self->subAC);
    layoutKizLinePanes(&self->subC0);
}

// Countdown timer at +0x40: decrement; when it hits zero reset the state
// bytes, then publish the remaining scaled count into the slot-15 pane result.
extern "C" void lineCountDown(UnkKizunaSelf59D44* self) {
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
u8 kizChartStatus(UnkKizunaSelfCBCC* self) {
    // Sense: constant status when the anim sits AT frame 0; sub-anim query
    // runs while it is still moving.
    if (self->field8C != 0) {
        bool atZero = (lbl_eu_80668828 == self->field78->GetFrame());
        if (atZero) {
            return 0x54;
        }
        bool finished = kizAnimAtEnd((UnkKizunaSelf592D8*)&self->sub68);
        return finished ? 0x55 : 0x53;
    } else {
        bool atZero = (lbl_eu_80668828 == self->field78->GetFrame());
        if (atZero) {
            return 0x57;
        }
        bool finished = kizAnimAtEnd((UnkKizunaSelf592D8*)&self->sub68);
        return finished ? 0x58 : 0x56;
    }
}

#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/IUICf.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"

// Forward decl: full view is TU-local in CMenuEnemyState.cpp (only the
// pointer is needed for the memberized EneSt_ScanSlotHighlight declaration).
struct Actor2Layout;

extern "C" const f32 lbl_eu_80666FEC;
extern "C" const f32 lbl_eu_80667004;

// Panel entry in the 24-slot panel array at offset 0xA4, each 0x4C bytes.
// Initialised in the ctor do-while loop.  The entry-pointer trick
// (entry = this + idx*0x4C) means entry+0xA4 == &panels[idx]; named
// member access is used throughout.
struct MenuEnemyPanel {
    u32 actorId;                   // +0x00  (panelData+0x00, entry+0xA4)
    nw4r::lyt::Layout* layout1;    // +0x04  (entry+0xA8)
    u32 unk08;                     // +0x08
    nw4r::lyt::Layout* layout2;    // +0x0C  (entry+0xB0)
    u32 unk10;                     // +0x10
    u8 drawLayout0Flag;            // +0x14  (entry[0xB8], panelData[0x14])
    u8 visible;                    // +0x15  (entry[0xB9], panelData[0x15])
    f32 animMarker;                // +0x18  (entry+0xBC)
    u8 unk1C;                      // +0x1C  (entry[0xC0], panelData[0x1c])
    u8 unk1D;                      // +0x1D  (entry[0xC1], panelData[0x1d])
    u8 unk1E;                      // +0x1E  (entry[0xC2])
    u8 unk1F;                      // +0x1F  (entry[0xC3], panelData[0x1f])
    u8 unk20;                      // +0x20
    u8 unk21;                      // +0x21
    u8 unk22;                      // +0x22
    u32 unk24;                     // +0x24
    u8 panelType;                  // +0x28  (entry[0xCC], panelData[0x28])
    u8 unk29;                      // +0x29  (entry[0xCD])
    void* obj1;                    // +0x2C  (panelData+0x2C)
    void* obj2;                    // +0x30  (panelData+0x30)
    void* obj3;                    // +0x34  (panelData+0x34)
    u32 unk38;                     // +0x38
    u32 unk3C;                     // +0x3C
    u32 unk40;                     // +0x40
    u32 unk44;                     // +0x44
    u32 unk48;                     // +0x48

    MenuEnemyPanel()
        : actorId(0), layout1(NULL), unk08(0), layout2(NULL), unk10(0),
          drawLayout0Flag(0), visible(0), animMarker(lbl_eu_80666FEC), unk1C(0),
          unk1D(0), unk1E(0), unk1F(0), unk20(0), unk21(0), unk22(0), unk24(0),
          panelType(0), unk29(1), obj1(NULL), obj2(NULL), obj3(NULL), unk38(0),
          unk3C(0), unk40(0), unk44(0), unk48(0) {}
};

// 4 packed shorts returned in r3:r4 by PaneMatGetS16Quad.
struct FourShorts { s16 a, b, c, d; };

// Embedded target-select cursor (0x48 bytes at +0x7E4 of CMenuEnemyState).
// Virtual destructor only; the ctor is defined in CMenuEnemyState.cpp.
class CPcSelectCursor {
public:
    // Declared first so the compiler puts the vptr at +0x00.
    CPcSelectCursor();
    virtual ~CPcSelectCursor();

    u32 field04;                     // +0x04
    UnkClass_8045F564 mem08;         // +0x08 (16 B: field08..field14)
    u32 field18;                     // +0x18
    nw4r::lyt::Layout* layout1C;     // +0x1C
    nw4r::lyt::AnimTransform* anim20; // +0x20
    nw4r::lyt::AnimTransform* anim24; // +0x24
    u32 field28;                     // +0x28
    u32 field2C;                     // +0x2C
    u32 field30;                     // +0x30
    u32 field34;                     // +0x34
    u32 field38;                     // +0x38
    u32 field3C;                     // +0x3C
    u8 byte40;                       // +0x40
    u8 byte41;                       // +0x41
    u32 field44;                     // +0x44

    void EneSt_CursorBuild();
    void EneSt_CursorUpdate();
};

// IUICf task base (0x00-0x57), the IWorkEvent subobject at +0x58, the
// IScnRender subobject at +0x5C, then the menu fields.
class CMenuEnemyState : public IUICf, public IWorkEvent, public IScnRender {
public:
    // ---- fields ----
    // 0x060
    void* unk60;
    // 0x064-0x073  UnkClass_8045F564 (16 B)
    UnkClass_8045F564 mem64;
    // 0x074
    nw4r::lyt::Layout* unk74;
    // 0x078
    nw4r::lyt::AnimTransform* unk78;
    // 0x07C
    u32 field7C;
    // 0x080
    u32 field80;
    // 0x084
    u32 field84;
    // 0x088
    u32 field88;
    // 0x08C
    u32 field8C;
    // 0x090
    u32 field90;
    // 0x094
    u32 field94;
    // 0x098
    u32 field98;
    // 0x09C
    u32 field9C;
    // 0x0A0
    u32 fieldA0;
    // 0x0A4-0x7C3  24 panels x 0x4C each
    MenuEnemyPanel panels[24];
    // 0x7C4
    u8 field7C4;
    // 0x7C5-0x7C7
    u8 gap7C5[3];
    // 0x7C8
    f32 field7C8;
    // 0x7CC
    f32 field7CC;
    // 0x7D0
    f32 field7D0;
    // 0x7D4
    u32 field7D4;
    // 0x7D8
    f32 field7D8;
    // 0x7DC
    u32 field7DC;
    // 0x7E0
    u32 field7E0;
    // 0x7E4-0x82B  CPcSelectCursor (0x48 B)
    CPcSelectCursor selectCursor;
    // 0x82C
    u32 field82C;
    // 0x830
    u32 unk830;
    // 0x834
    u8 unk834;
    // 0x835-0x837
    u8 unk835[3];
    // 0x838
    f32 unk838;

    static CMenuEnemyState* sInstance;
    static CMenuEnemyState* create(CProcess* parent, CScn* scene, void* arg3);

    CMenuEnemyState(CScn* scn);
    virtual ~CMenuEnemyState();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();
    void EneSt_PositionPanel(u8* panelData, void* posA, void* posB);
    void EneSt_UpdatePanelHighlight(u8* panelData);
    void EneSt_HighlightPane(u8* panelData, int which);
    void EneSt_RefreshNameColor(u8* panelData);
    void EneSt_RefreshMenu();
    void EneSt_ScanSlotHighlight(Actor2Layout* actor2);
};

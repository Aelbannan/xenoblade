#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

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
    u8 gap16;                      // +0x16
    u8 gap17;                      // +0x17
    f32 animMarker;                // +0x18  (entry+0xBC)
    u8 unk1C;                      // +0x1C  (entry[0xC0], panelData[0x1c])
    u8 unk1D;                      // +0x1D  (entry[0xC1], panelData[0x1d])
    u8 unk1E;                      // +0x1E  (entry[0xC2])
    u8 unk1F;                      // +0x1F  (entry[0xC3], panelData[0x1f])
    u8 unk20;                      // +0x20
    u8 unk21;                      // +0x21
    u8 unk22;                      // +0x22
    u8 gap23;                      // +0x23
    u32 unk24;                     // +0x24
    u8 panelType;                  // +0x28  (entry[0xCC], panelData[0x28])
    u8 unk29;                      // +0x29  (entry[0xCD])
    u8 gap2A;                      // +0x2A
    u8 gap2B;                      // +0x2B
    void* obj1;                    // +0x2C  (panelData+0x2C)
    void* obj2;                    // +0x30  (panelData+0x30)
    void* obj3;                    // +0x34  (panelData+0x34)
    u32 unk38;                     // +0x38
    u32 unk3C;                     // +0x3C
    u32 unk40;                     // +0x40
    u32 unk44;                     // +0x44
    u32 unk48;                     // +0x48
};

// 4 packed shorts returned in r3:r4 by func_801397AC.
struct FourShorts { s16 a, b, c, d; };

// Abstract view of the object returned by CDeviceFont::func_80452C10; the
// 10th user virtual (vtable+0x24) yields the u32 bound by func_8013676C.
// All-pure, never constructed directly (no vtable is emitted).
class MenuFontView {
public:
    virtual void m00() = 0;   // 0x00
    virtual void m04() = 0;   // 0x04
    virtual void m08() = 0;   // 0x08
    virtual void m0C() = 0;   // 0x0C
    virtual void m10() = 0;   // 0x10
    virtual void m14() = 0;   // 0x14
    virtual void m18() = 0;   // 0x18
    virtual void m1C() = 0;   // 0x1C
    virtual void m20() = 0;   // 0x20
    virtual u32 m24() = 0;    // 0x24  (font-handle getter)
};

// CPcSelectCursor lives at offset 0x7E4 and is 0x48 bytes.
// Several fields alias into CMenuEnemyState (unk800..unk828).
// Declared here so the source can name the sub-fields.
struct CPcSelectCursorLayout {
    u8 field00[4];                         // +0x00 (vtable / reserved)
    u32 field04;                           // +0x04
    u32 field08;                           // +0x08
    u32 field0C;                           // +0x0C
    u32 field10;                           // +0x10
    u32 field14;                           // +0x14
    u32 field18;                           // +0x18
    nw4r::lyt::Layout* layout1C;          // +0x1C  (= unk800)
    nw4r::lyt::AnimTransform* anim20;      // +0x20  (= unk804)
    nw4r::lyt::AnimTransform* anim24;      // +0x24  (= unk808)
    u32 field28;                           // +0x28
    u32 field2C;                           // +0x2C
    u32 field30;                           // +0x30
    u32 field34;                           // +0x34
    u32 field38;                           // +0x38
    u32 field3C;                           // +0x3C
    u8 byte40;                             // +0x40  (= unk824)
    u8 byte41;                             // +0x41  (= unk825)
    u8 pad42[2];                           // +0x42-0x43
    u32 field44;                           // +0x44  (= unk828)
};

class CMenuEnemyState {
public:
    void cbRenderBefore();
    void Move();

    // ---- fields ----
    // 0x000-0x053: CProcess base data
    u8 unk00[0x54];
    // 0x054
    u8 unk54;
    // 0x055
    u8 unk55;
    // 0x056-0x057
    u8 gap56[2];
    // 0x058
    char* vtPtr1;
    // 0x05C
    char* vtPtr2;
    // 0x060
    void* unk60;
    // 0x064-0x073  UnkClass_8045F564Fv (16 B)
    u8 unk64[0x10];
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
    CPcSelectCursorLayout selectCursor;
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

    CMenuEnemyState(void* scn);
    // Deliberately non-virtual: the retail dtor stores no vtable (the ctor
    // writes the vtable constants manually), so declaring `virtual` here would
    // make MWCC emit an unwanted vptr store in the dtor.
    ~CMenuEnemyState();
    void Init();
    void Term();
};

// CPcSelectCursor lives at offset 0x7E4 in CMenuEnemyState and is 0x48 bytes.
// The retail dtor __dt__15CPcSelectCursorFv stores no vtable, so the class is
// deliberately non-polymorphic here (the +0x00 word is a plain reserved
// field). The ctor __ct__CPcSelectCursor is defined in another TU.
class CPcSelectCursor {
public:
    u32 field00;                     // +0x00 (vtable / reserved)
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
    u8 pad42[2];                     // +0x42-0x43
    u32 field44;                     // +0x44

    CPcSelectCursor();
    ~CPcSelectCursor();
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __ct__CPcSelectCursor(void* self);
extern "C" void* func_8016FE34(void* r3);
// Shared-arc text value (unmangled retail symbol); feeds func_801368C0.
extern "C" u32 func_801355D8();
// 4-short colour pairs read off a pane (unmangled retail symbol).
extern "C" FourShorts func_801397AC(void*, u32);
extern "C" void* __dt__15CMenuEnemyStateFv(CMenuEnemyState* self, int flags);
extern "C" void* __ct__CMenuEnemyState(void* self, void* scn);
extern "C" void* func_801355F4();
extern "C" void __dt__8CProcessFv(void* self, int flags);
extern "C" void func_80110A78(CMenuEnemyState* self, u32 actorId);
// Layout sound effect helper (retail symbol is unmangled).
extern "C" void func_80137B44(nw4r::lyt::Layout* layout, const char* name,
                              u32 value);
// Apply a loaded texture to a pane (retail symbol is unmangled).
extern "C" void func_80137F88(void* a, void* palette);
// Enemy-menu resource helpers (retail symbols unmangled). func_80138F78 is
// declared C++ in code_80135FDC.hpp; the C-linkage-first decl here makes the
// later plain redeclaration inherit it (same signature).
extern "C" u16 func_80136254(const void* a, const char* b, int c);
extern "C" char* func_80138F78(u32 id);
// Enemy-menu helpers used by the panel/cursor functions (retail unmangled).
// (func_8009ECB0 is declared in include/functions.hpp as `extern "C" int*`.)
extern "C" void* func_800B8B94(s32 a);
extern "C" void* func_800EA444(void);
extern "C" void func_8049B59C(void* out, void* pose, const void* in);
extern "C" void func_80137DB8(void* a, u32 b, u32 c);
extern "C" char* func_80138DA4(const char* s);
extern "C" void* func_801984F0(void* a, u32 idx);
extern "C" void func_80139AC8(void* a, void* b, void* c);
extern "C" void* func_800B8A64(void);
int sprintf(char*, const char*, ...);
// func_800AD860__FPv: C++ linkage (MWCC re-derives the retail mangled name).
void* func_800AD860(void* obj);

// Retail .sdata singleton (created by func_801109D8, cleared by Term).
extern u32 lbl_eu_80663F50;
// CPcSelectCursor vtable (.data; the retail ctor stores its address at +0x00).
extern char lbl_eu_8052C534[];
// String pool base (func_80111B08 pane names at +0xd3/+0xdd/+0x11d).
extern char lbl_eu_804FDBF8[];
// sdata2 constants used by the panel-highlight helpers.
extern const f32 lbl_eu_80667008;
extern const f32 lbl_eu_8066703C;
extern const f32 lbl_eu_80667040;
extern const f32 lbl_eu_80667044;
extern const f32 lbl_eu_80667048;
extern const f32 lbl_eu_8066704C;
extern const f32 lbl_eu_80667050;
extern const f32 lbl_eu_80667054;
extern const f32 lbl_eu_80667038;
// Enemy-data table pointer (sdata) used by the highlight-scan helpers.
extern u8* lbl_eu_806640E0;
// Texture-index table for the panel-highlight search (func_801132A8).
extern u32 lbl_eu_804FDBC8[];
// sdata colour/scale quads + s16 pairs used by func_80110A78's pane setup.
// Declared as 4-element s16 arrays so direct element access keeps the retail
// sda21 addressing (li lbl@sda21 + halfword ops).
extern s16 lbl_eu_80663F58[4];
extern u32 lbl_eu_80663F5C;
extern s16 lbl_eu_80663F60[4];
extern u32 lbl_eu_80663F64;
extern s16 lbl_eu_80663F68[4];
extern s16 lbl_eu_80663F70[4];
extern s16 lbl_eu_80663F78[4];
extern s16 lbl_eu_80663F80[4];
extern s16 lbl_eu_80663F88[4];
extern s16 lbl_eu_80663F90[4];
extern u32 lbl_eu_806640CC;
// sdata2 constants used by the position-marker helpers.
extern const f32 lbl_eu_80666FF0;
extern const f32 lbl_eu_80666FF4;
extern const f32 lbl_eu_80666FF8;
extern const f32 lbl_eu_80666FFC;
extern const f32 lbl_eu_80667000;
extern const f32 lbl_eu_80667018;
extern const f32 lbl_eu_8066701C;
extern const f32 lbl_eu_80667020;
extern const f32 lbl_eu_80667024;
extern const f32 lbl_eu_80667028;

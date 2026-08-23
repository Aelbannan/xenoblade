#pragma once

#include <types.h>
#include <revolution/GX.h>

// mpfsys render-state globals (SDA-relative linker symbols). Plain externs at
// global scope: MWCC does not mangle global-scope variable names, so these
// emit the exact retail lbl_eu_* symbols without extern "C".
extern u8* lbl_eu_80665844;
extern s32 lbl_eu_80665854;
extern s16 lbl_eu_80665858;
extern s16 lbl_eu_8066585A;
extern s16 lbl_eu_8066585C;
extern u32 lbl_eu_8066586C;
extern u32 lbl_eu_80665878;
extern u32 lbl_eu_8066587C;
extern GXColor lbl_eu_80663858;
extern f32 lbl_eu_8066A734;
extern f32 lbl_eu_8066A738;
extern f32 lbl_eu_8066A73C;
extern const f32 lbl_eu_8066A740;
extern u16* lbl_eu_80665850;
extern u32 lbl_eu_8066A7A0;
extern GXColor lbl_eu_8066A7D0;
extern GXColor lbl_eu_8066A7D4;
extern const f32 lbl_eu_8066A744;
extern const f32 lbl_eu_8066A74C;
extern const f32 lbl_eu_8066A758;
extern const f32 lbl_eu_8066A75C;
extern const f32 lbl_eu_8066A770;
extern const f32 lbl_eu_8066A7D8;

namespace mpfsys {

class UnkClass_80471EC8 {
public:
    void func_80471EC8();
    void func_80471FC8();
    void func_80471FCC();
    void func_80472064();
    void func_8047230C();
    void func_8047233C();
    void func_80472370();
    void func_804723A4();
    void func_804724DC();
    void func_80472864();
    void func_80472870();
    void func_804728E8();
    void func_80473394();
    void* func_804734F4(u8 layerIndex);
    void func_804737F0();
    void func_80473984();
    void func_80474064();
    void func_804742BC();
    void func_804743E0();
    void func_804744EC();
    void func_8047466C();
    void func_80474780();
    void func_8047491C();
    void func_80474A40();
    void func_80474AA0();
    void func_80474B00();
    void func_80474CC4();
    void func_80474CF4() const;
    void func_80474D50();
    void func_80474DAC();
    void func_80474DF8();
    void func_80474E24();
    void func_80474E68();
    void func_80474EB0();
    void func_80474F2C();
    void func_80474F54();

    // Layout recovered from field accesses (0x2D00 bit array in
    // func_8047233C/func_80472370, 0x2DFC/0x2E00 in func_8047230C). Total
    // object size 0x2E08 matches the embedded subobject modelled in
    // code_8047BB54.cpp. Public so the literal-name free-function forms of
    // the Fv-annotated members can access the fields.
    u8 field_0x0[0x2D00];
    u32 field_0x2D00[8]; // 256-bit flag array (bit set/clear by index)
    u8 field_0x2D20[0xC0]; // 0x2D20..0x2DE0
    f32 field_0x2DE0;      // 0x2DE0 - layer colour (set by func_80472870)
    f32 field_0x2DE4;
    f32 field_0x2DE8;
    u8 field_0x2DEC[0x10]; // 0x2DEC..0x2DFC
    f32 field_0x2DFC;
    f32 field_0x2E00;
    s32 field_0x2E04;
};
} // namespace mpfsys


#include "monolib/mpfsys/code_8047CA88.hpp"
#include "monolib/mpfsys/code_8047BB54.hpp"
#include <nw4r/math.h>

// .sbss 0x806658C0 (8B): initialized-flag byte + 7 pad bytes (the retail
// splitter sized the flag symbol 8B). The singleton object buffer (0x28B)
// and the three foreign float constants below are the unit's only other
// data: the floats live in other units' ranges and stay extern.
struct SFlag_806658C0 {
    s8 flag;      // 0x00 initialized flag
    u8 pad[7];    // 0x01..0x07
};

extern "C" {
    struct SFlag_806658C0 lbl_eu_806658C0;
    u8 lbl_eu_80658518[sizeof(UnkClass_8047CA88)];
    extern float lbl_eu_8066A870;
    extern float lbl_eu_8066A874;
    extern float lbl_eu_80665884;
}

UnkClass_8047CA88* UnkClass_8047CA88::getInstance(){
    if(!lbl_eu_806658C0.flag){
        lbl_eu_806658C0.flag = 1;
    }
    return (UnkClass_8047CA88*)&lbl_eu_80658518;
}

struct UnkParam_8047CA88 {
    u8 count;
    u8 type;
    u16 index;
    f32 x;
    f32 y;
    f32 z;
    f32 field_10;
};

extern "C" void func_8047CAA8__17UnkClass_8047CA88Fv(
    UnkClass_8047CA88* self, UnkParam_8047CA88* param)
{
    nw4r::math::VEC3 tmp;
    nw4r::math::VEC3Sub(&tmp, (nw4r::math::VEC3*)&param->x,
                        (nw4r::math::VEC3*)self->field_04);

    // Component dot through the nw4r PS inline kernel (ps_mul/ps_madd/ps_sum0).
    f32 dot = nw4r::math::VEC3Dot((nw4r::math::VEC3*)&self->field_08, &tmp);

    f32 clamped = self->field_14 * dot;
    if (lbl_eu_8066A870 > clamped) clamped = lbl_eu_8066A870;
    if (lbl_eu_8066A874 < clamped) clamped = lbl_eu_8066A874;

    nw4r::math::VEC3Scale(&tmp, (nw4r::math::VEC3*)&self->field_08, clamped);

    nw4r::math::VEC3Add(&tmp, (nw4r::math::VEC3*)self->field_04, &tmp);

    f32 scalar = self->field_18 * (clamped * lbl_eu_80665884) + param->field_10;

    nw4r::math::VEC3Sub(&tmp, (nw4r::math::VEC3*)&param->x, &tmp);

    f32 dist2 = nw4r::math::VEC3Dot(&tmp, &tmp);
    if (dist2 < scalar * scalar) {
        if (param->type != 0) {
            u16* idxList = (u16*)self->field_24 + param->index;
            u32 count = param->count;
            for (u32 i = 0; i < count; i++) {
                u16 idx = idxList[i];
                func_8047CAA8__17UnkClass_8047CA88Fv(
                    self, (UnkParam_8047CA88*)((u8*)self->field_1C + idx * 0x14));
            }
        } else {
            func_8047C040__17UnkClass_8047BB54Fv(
                func_8047C034__17UnkClass_8047BB54Fv(self),
                (u8*)self->field_20 + param->index * 8, param->count);
        }
    }
}

extern "C" void func_8047CC4C__17UnkClass_8047CA88Fv(
    UnkClass_8047CA88* self, void* desc, void* dataPtr, nw4r::math::VEC3* vec, f32 scale)
{
    f32 inv = lbl_eu_8066A874;
    u32 off = *(u32*)((u8*)desc + 0x24);
    self->field_04 = dataPtr;
    u8* base = (u8*)desc + off;

    struct UnkRaw3 { f32 x, y, z; };
    UnkRaw3 raw;
    nw4r::math::VEC3 v;
    nw4r::math::VEC3Scale(&v, vec, scale);

    raw.y = v.y;
    raw.z = v.z;
    raw.x = v.x;

    *(u32*)&self->field_08 = *(u32*)&raw.x;
    *(u32*)&self->field_0C = *(u32*)&raw.y;
    *(u32*)&self->field_10 = *(u32*)&raw.z;

    f32 dot = nw4r::math::VEC3Dot((nw4r::math::VEC3*)&self->field_08,
                                  (nw4r::math::VEC3*)&self->field_08);
    self->field_14 = inv / dot;

    self->field_20 = (u8*)desc + *(u32*)(base + 4);
    self->field_24 = (u8*)desc + *(u32*)(base + 8);
    self->field_18 = scale;
    self->field_1C = base + 0x20;

    func_8047CAA8__17UnkClass_8047CA88Fv(
        self, (UnkParam_8047CA88*)((u8*)base + 0x20));
}

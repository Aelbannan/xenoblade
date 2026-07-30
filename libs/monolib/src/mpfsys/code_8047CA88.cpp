#include "monolib/mpfsys/code_8047CA88.hpp"
#include "monolib/mpfsys/code_8047BB54.hpp"

extern "C" {
    s8 lbl_eu_806658C0;
    u8 lbl_eu_80658518[sizeof(UnkClass_8047CA88) + 64];
    float lbl_eu_8066A870;
    float lbl_eu_8066A874;
    float lbl_eu_80665884;
}

UnkClass_8047CA88* UnkClass_8047CA88::getInstance(){
    if(!lbl_eu_806658C0){
        lbl_eu_806658C0 = 1;
    }
    return (UnkClass_8047CA88*)&lbl_eu_80658518;
}

extern "C" void func_8047CAA8__17UnkClass_8047CA88Fv(void* self, void* param);

void func_8047CC4C__17UnkClass_8047CA88Fv(
    UnkClass_8047CA88* self, void* desc, void* ptr, float scale)
{
    float inv = lbl_eu_8066A874;
    u32 off = *(u32*)((u8*)desc + 0x24);
    self->field_04 = ptr;

    u8* base = (u8*)desc + off;
    void* target = base + 0x20;

    f32 v0 = ((f32*)ptr)[0];
    f32 v1 = ((f32*)ptr)[1];
    f32 v2 = ((f32*)ptr)[2];

    f32 s0 = v0 * scale;
    f32 s1 = v1 * scale;
    f32 s2 = v2 * scale;

    *(u32*)&self->field_08 = *(u32*)&s0;
    *(u32*)&self->field_0C = *(u32*)&s1;
    *(u32*)&self->field_10 = *(u32*)&s2;

    f32 len_sq = s0 * s0 + s1 * s1 + s2 * s2;
    f32 inv_len = inv / len_sq;
    self->field_14 = inv_len;

    u32 off1 = *(u32*)(base + 4);
    self->field_20 = (u8*)desc + off1;

    u32 off2 = *(u32*)(base + 8);
    self->field_24 = (u8*)desc + off2;

    self->field_18 = scale;
    self->field_1C = target;

    func_8047CAA8__17UnkClass_8047CA88Fv(self, target);
}

// Auto-scaffolded catalog TU for kyoshin/cf/code_800F42AC
// Replace stubs with high-level C/C++ during decomp.

#include <string.h>

#include "kyoshin/cf/code_800F42AC.hpp"

// Rounds a float to the nearest integer (as a float), matching MWCC's
// double-arithmetic + fctiwz idiom: (f32)(s32)((double)v + 0.5-ish).
static inline f32 roundf_to_f32(f32 v) {
    double round;
    if (v > lbl_eu_80666E90)
        round = lbl_eu_80666E98;
    else
        round = lbl_eu_80666EA0;
    return (f32)((s32)((double)v + round));
}

/// Computes an on-screen status ratio. When there is no sub-object, returns
/// 1.0f; otherwise prefers a combination from an entity-derived object, then
/// a game-manager flag driven constant, then the sub-object's own value.
float func_800F42AC(ScMain* self) {
    if (self->sub == NULL)
        return lbl_eu_80666E90;

    cf::Sc48778* node = (cf::Sc48778*)func_8016FE34((void*)func_800B708C__Fi((int)self->field_00));
    if (func_80148778(node->data_08, 0x10) != 0) {
        cf::Sc149154Ret* obj = (cf::Sc149154Ret*)func_80149154(node->data_08, 0x10);
        double round;
        f32 a = obj->field_20;
        f32 b = self->sub->field_7C;
        f32 sum = a + b;
        if (sum > lbl_eu_80666E90)
            round = lbl_eu_80666E98;
        else
            round = lbl_eu_80666EA0;
        return (f32)((s32)((double)sum + round));
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x04000000))
        return roundf_to_f32(self->field_830);
    return roundf_to_f32(self->sub->field_7C);
}

/// Returns a normalized ratio depending on status flags:
/// bit 20 of field_824 -> 1.0f; if mask 0x400 is active in the game
/// manager -> constant A; no sub-object -> 1.0f; else sub->field_7C / field_830.
float func_800F4424(ScMain* self) {
    if (self->flags_824 & 0x100000)
        return lbl_eu_80666E90;
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x04000000))
        return lbl_eu_80666EB0;
    if (self->sub == 0)
        return lbl_eu_80666E90;
    return self->sub->field_7C / self->field_830;
}

/// (Re)initialises a ScMain object: clears all slots, resets the per-sub
/// table (0xCC, 5 entries) and zeroes the status flags. When the bit-17
/// status flag is set, first tears down a register with id 0x375.
void func_800F449C(ScMain* self) {
    if (self->flags_824 & 0x20000) {
        if (func_80141270(0x375) == 0) {
            func_8009D018(0x30e3, 0);
            func_801412D0(0x375);
            func_8013F244();
        }
    }
    self->field_04 = 0;
    self->field_00 = 0;
    self->field_08 = 0;
    self->sub = 0;
    self->field_830 = lbl_eu_80666E90;
    memset(&self->zero_828, 0, 8);
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_58 = 0;
    self->field_5C = -1;
    self->field_60 = 0;
    self->f_64 = lbl_eu_80666E90;
    self->f_68 = lbl_eu_80666E90;
    self->f_6C = lbl_eu_80666E90;
    self->f_70 = lbl_eu_80666E90;
    self->f_74 = lbl_eu_80666E90;
    self->f_78 = lbl_eu_80666E90;
    self->f_7C = lbl_eu_80666E90;
    self->s_80 = 0;
    self->s_82 = 0;
    self->field_8C = 0;
    self->s_90 = 0;
    self->field_C8 = 0;
    memset(&self->data_18, 0, 0x40);
    memset(&self->data_94, 0, 0x34);
    self->flags_84 = 0;
    self->field_88 = 0;
    for (int i = 0; i < 5; i++) {
        ScMain::ScSub* s = &self->subs[i];
        s->field_00 = 0;
        s->field_04 = 0;
        s->field_48 = 0;
        s->field_4C = -1;
        s->field_50 = 0;
        s->f_54 = lbl_eu_80666E90;
        s->f_58 = lbl_eu_80666E90;
        s->f_5C = lbl_eu_80666E90;
        s->f_60 = lbl_eu_80666E90;
        s->f_64 = lbl_eu_80666E90;
        s->field_7C = 0;
        s->s_80 = 0;
        s->field_B8 = 0;
        s->f_68 = lbl_eu_80666E90;
        s->f_6C = lbl_eu_80666E90;
        s->s_70 = 0;
        s->s_72 = 0;
        memset(&s->data_08, 0, 0x40);
        s->field_74 = 0;
        s->field_78 = 0;
        memset(&s->data_84, 0, 0x34);
    }
    self->flags_824 = 0;
}

/// Applies status-flag priority to a combined size value, rounding to int.
/// bit 17 of field_824 -> 99999; sum(f_6C+f_70) < 1.0 -> 0; bit 7 of field_84
/// -> 0; otherwise round the sum to nearest integer (half-up / half-down).
int func_800F4648(ScMain* self) {
    if (self->flags_824 & 0x20000)
        return 99999;
    f32 sum = self->f_6C + self->f_70;
    if (sum < lbl_eu_80666E90)
        return 0;
    if (self->flags_84 & 0x80)
        return 0;
    double r;
    if (sum > lbl_eu_80666E90)
        r = lbl_eu_80666E98;
    else
        r = lbl_eu_80666EA0;
    return (int)((double)sum + r);
}

/// Returns whether `self`'s polymorphic query (vtable slot 19) and its
/// 0x3F10 pointer both differ from `other->field_04`.
int func_800F46C0(ScMain* other, Sc46C0Other* self) {
    void* f = other->field_04;
    int result = 0;
    if (self->mSub.v17() != f && self->field_3F10 != f)
        result = 1;
    return result;
}

/// Decodes low 5 bits of field_824: returns 5/4/3/2 for the top-priority bit
/// that is set, else the value of bit 4.
u32 func_800F4730(ScMain* self) {
    u32 flags = self->flags_824;
    if (flags & 1)
        return 5;
    if (flags & 2)
        return 4;
    if (flags & 4)
        return 3;
    if (flags & 8)
        return 2;
    return (flags & 0x10) != 0 ? 1 : 0;
}

/// Returns the sub-object pointer at offset 0x0C (field subObject).
cf::CfUnknownSub* func_800F477C(cf::CfCode800F42AC* self) {
    return self->subObject;
}

void* func_800F4784(void* self) {
    void* v = *(void**)((char*)self + 0xc);
    return v ? v : 0;
}

/// Returns items[idx]->v_14, negated when field_60C == 1.
float func_800F4798(Sc4798* self, int idx) {
    if (self->field_60C == 1)
        return -self->items[idx]->v_14;
    return self->items[idx]->v_14;
}

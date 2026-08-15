#pragma once

#include <types.h>
#include <revolution/MTX.h>

// Partial layouts for cross-TU data/helpers declared below (retail C-linkage
// pseudo-imports relocated from code_804CC2B8.cpp). Keep signatures/linkage
// identical - the names are the retail symbol names.
struct MemManGlob {
    u32 field_0x00;
    const void* field_0x04;
};

struct EffectNode {
    s16 field_0x00;
    s16 field_0x02;
    s16 field_0x04;
    s16 field_0x06;
    s16 field_0x08;
    s16 field_0x0a;
    s16 field_0x0c;
    u8 pad_0x0e[0x18 - 0x0e];
    u32 field_0x18;
};

struct U16Pair {
    u16 a;
    u16 b;
};

// C-linkage imports
extern "C" f64 lbl_eu_8066B0F0;
extern "C" f32 lbl_eu_8066B100;
extern "C" f32 lbl_eu_8066B0D8;
extern "C" f32 lbl_eu_8066B0E4;
extern "C" f64 lbl_eu_8066B0E8;
extern "C" f32 lbl_eu_8066B0E0;
extern "C" f32 lbl_eu_8066B140;
extern "C" f32 lbl_eu_8066B158;
extern "C" f32 lbl_eu_8066B15C;
extern "C" f32 lbl_eu_8066B160;
extern "C" f32 lbl_eu_8066B164;
extern "C" f32 lbl_eu_8066B168;
extern "C" f32 lbl_eu_8066B110;
extern "C" f32 lbl_eu_8066B144;
extern "C" f32 lbl_eu_8066B148;
extern "C" f32 lbl_eu_8066B14C;
extern "C" f32 lbl_eu_8066B150;
extern "C" u8 lbl_eu_80524500[];
extern "C" u8 lbl_eu_80524520[];
extern "C" void func_804D8AA4(int type, int flag);
extern "C" void* func_80496264(void* src, int index);
extern "C" u32 getMaxAllocSize__Q23mtl10MemManagerFUl(u32 mem);
extern "C" void* allocate_array__Q23mtl10MemManagerFUlUl(u32 count_bytes, u32 mem);
extern "C" u32 lbl_eu_8065FC18[];
extern "C" s32 lbl_eu_806659A8;
extern "C" s32 lbl_eu_806659AC;
extern "C" s32 lbl_eu_806659B0;
extern "C" s32 lbl_eu_806659B4;
extern "C" u32 lbl_eu_8066B124;
extern "C" void func_80495E84(void* p);
extern "C" void __dt__804D80F0(void* p, int flag);
extern "C" void __dl__FPv(void* p);
extern "C" EffectNode* func_804E0114(s32 index);
extern "C" void func_804E0098(s16 index);
extern "C" s32 func_804DFFA8(s32 index);
extern "C" u32 func_804E0104(void);
extern "C" void func_8006BEC0(Vec* p);
extern "C" void func_8004B0B0(Vec* p);
extern "C" void func_804EE60C(void* obj);
extern "C" void func_804EE658(void* obj, void* node);
extern "C" void func_804EE8FC(void* obj, void* node);
extern "C" void func_804EE558(void* obj, u32 a, u32 b, u32 c, u32 d);
extern "C" U16Pair* func_804DF2A8(const void* p);
extern "C" f32 lbl_eu_8066B104;
extern "C" void func_804DD4F8(Mtx* out);
extern "C" void func_804DB980(Mtx* out, const Vec* p, s32 field);
extern "C" void func_804DCA88(Mtx* out, const Vec* angles);
extern "C" f32 lbl_eu_8066B108;
extern "C" f32 lbl_eu_8066B10C;
extern "C" void func_80496120(const void* src, Mtx out, s32 flag);

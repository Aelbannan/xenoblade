#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
struct CfGimmickElvData;
class UnkClass_800817BC;

// Forward declarations for external functions used by CfGimmickElv
extern "C" {
void __ct__cf_CfGimmick(void* self);
void __dt__Q22cf9CfGimmickFv(void* self, int mode);
void* func_8003AA34();
u32 getBdatStringColumnValue(void* table, const char* colName, s32 rowId);
void func_8020938C(void* self, void* subobj, void* bdat, void* stack, int idx);
void func_802095D8(void* self, void* subobj, void* bdat, void* stack, int idx);
void func_80209488(void* self, void* subobj, void* bdat, void* stack, int idx);
void func_802089BC(void* dst, void* src1, void* src2);
// func_80208C48 plays a sound id at a position (first arg comes from a u16 field)
void func_80208C48(int id, void* pos);
// bdat-driven SFX lookup: (id, position)
int func_80208C60(u16 id, void* pos);
void func_80208EE4(void* self);
int func_8020971C(u16 id);
int func_80209754(u8 flag, void* a, void* b, void* c, u32 d);
// Retail evidence: called with no visible argument setup (r3 left as-is)
void func_80209F5C(void);
void* func_8020A35C(void* self, int a, void* b);
void func_8020A434(void* ptr);
void func_8020A484(u16 id);
int func_8020A5DC(void* self);
int func_8020A608(u16 id, int mode);
void func_8020A6B0(void* a, void* b, u16 c, f32 d, int e, int g);
int func_8020A87C(void* self, u32 ptr);
void func_80462FD8__8CTaskLODFv(u8 lod, f32 f);
void func_80462EF4__8CTaskLODFv(u8 lod, f32 f);
void func_80462F4C__8CTaskLODFv(u8 lod, int mode);
void func_80462ED0__8CTaskLODFv(u8 lod, int mode);
void func_80462E3C__8CTaskLODFv(u8 lod, f32 f);
void func_80462D04__8CTaskLODFv(u8 lod);
void func_80462F10__8CTaskLODFv(u8 lod);
void func_80462D5C__8CTaskLODFv(u8 lod);
f32 func_80462F2C__8CTaskLODFv(u8 lod);
f32 func_80462FF4__8CTaskLODFv(u8 lod);
void func_80462F70__8CTaskLODFv(u8 lod, int mode);
void func_80462F94__8CTaskLODFv(u8 lod, u16 id);
void func_80463014__8CTaskLODFv(u8 lod);
u16 func_80462FB8__8CTaskLODFv(u8 lod);
void* func_804BC9EC__Fv();
void func_804BCC30(void* ptr, u8 id);
void func_804BCC3C(void* ptr, u8 id);
void* func_804BCC54(void* ptr, u16 id);
u16 func_804BCC6C(void* ptr, u8 id);
f32 func_80496288(void* ptr);
UnkClass_800817BC* func_800817BC__Q22cf13CfGameManagerFv(u32 id, u32 mode);
void func_801BFED0(int a, u16 b, int c);
int func_801BFABC(int a);
void func_801BFF78(int a, u16 b, int c);
u16 func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(u32 a, u32 b, u32 c, u32 d, f32 f);
void func_801BFAE8(u16 id, void* pos);
void __dl__FPv(void* ptr);
void __ptmf_scall(void* obj, void* ptmf);
void func_8020B34C(CfGimmickElvData* self);
}

// CfGimmickElv field layout (derived from retail ASM analysis)
// Total size: ~0x1D8 bytes
struct CfVec3 {
    f32 x, y, z;
};

struct CfGimmickElvData {
    /* 0x00 */ void* vtable;
    /* 0x04 */ CfVec3 vec0;    // base position
    /* 0x10 */ CfVec3 vec1;    // base rotation
    /* 0x1C */ CfVec3 vec2;    // base scale
    /* 0x28 */ u8 gap28[0x38];
    /* 0x60 */ u32 unk60;
    /* 0x64 */ u16 bdatRowId;
    /* 0x66 */ u16 unk66;
    /* 0x68 */ u16 unk68;
    /* 0x6A */ u16 unk6A;
    /* 0x6C */ u8 gap6C[4];
    /* 0x70 */ u8 lod0;
    /* 0x71 */ u8 lod1;
    /* 0x72 */ u8 lod2;
    /* 0x73 */ u8 lod3;
    /* 0x74 */ u32 flags;
    /* 0x78 */ u32 unk78;
    /* 0x7C */ u32 unk7C;
    /* 0x80 */ u16 unk80;
    /* 0x82 */ u16 typeId;
    // --- derived class fields ---
    /* 0x84 */ CfVec3 elvVec0;  // A-type vec index 1
    /* 0x90 */ CfVec3 elvVec1;  // A-type vec index 2
    /* 0x9C */ CfVec3 elvVec2;  // A-type vec index 3
    /* 0xA8 */ CfVec3 elvVec3;  // B-type vec index 1
    /* 0xB4 */ CfVec3 elvVec4;  // B-type vec index 2
    /* 0xC0 */ CfVec3 elvVec5;  // B-type vec index 3
    /* 0xCC */ CfVec3 elvVec6;  // C-type vec index 1
    /* 0xD8 */ u8 gapD8[0x3C];
    /* 0x114 */ CfVec3 elvVec7; // C-type vec index 2
    /* 0x120 */ u8 gap120[0x38];
    /* 0x158 */ u32 unk158;
    /* 0x15C */ CfVec3 elvVec8; // C-type vec index 3
    /* 0x168 */ u8 gap168[0x38];
    /* 0x1A0 */ u32 unk1A0;
    /* 0x1A4 */ u32 unk1A4;
    /* 0x1A8 */ u32 unk1A8;
    /* 0x1AC */ u32 unk1AC;
    /* 0x1B0 */ u8 flag1B0;
    /* 0x1B1 */ u8 flag1B1;
    /* 0x1B2 */ u8 flag1B2;
    /* 0x1B3 */ u8 flag1B3;
    /* 0x1B4 */ u16 val1B4;
    /* 0x1B6 */ u16 val1B6;
    /* 0x1B8 */ u16 val1B8;
    /* 0x1BA */ u16 val1BA;
    /* 0x1BC */ u16 val1BC;
    /* 0x1BE */ u16 val1BE;
    /* 0x1C0 */ f32 val1C0;
    /* 0x1C4 */ f32 val1C4;
    /* 0x1C8 */ f32 val1C8;
    /* 0x1CC */ f32 val1CC;
    /* 0x1D0 */ u16 state;
    /* 0x1D2 */ u16 direction;
    /* 0x1D4 */ u16 val1D4;
    /* 0x1D6 */ u16 val1D6;
};

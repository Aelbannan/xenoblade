#pragma once

#include <types.h>
#include "monolib/math/CAttrTransform.hpp"
#include <nw4r/g3d/res/g3d_resfile.h>

// Scene item pool (defined in CScnItemPool.cpp; only used opaquely here).
class CScnItemPool;

class CScnItemModel;

class CScnItemModelNw4r;

// Overlay view of the model reference-slot run at CScnItemModel+0x08. The
// slots[4] array lands at absolute offsets 0x7B4..0x7C0. func_804859E8
// clears slots matching a node; retail folds the first two slots to direct
// offsets and keeps the self+8 base for the last two (0x7B4/0x7B8 offsets).
struct CScnItemModelRefs {
    u8 _00[0x7AC];               // 0x00..0x7AC
    CScnItemModel* slots[4];     // 0x7AC (abs 0x7B4) .. 0x7B8 (abs 0x7C0)
};

// Owner/scene object referenced from CScnItemModel::field_04. Only the pool
// pointer at 0x60 is known (read by func_804830AC).
struct CScnItemModelOwner {
    u8 _00[0x60];       // 0x00..0x60 opaque
    CScnItemPool* pool; // 0x60
};

// Animation-resource wrapper passed to func_80484164 / func_804972E8
// (retail C-ABI). The embedded nw4r ResFile handle lives at +0xC, matching
// the CScnItemAnim layout; func_80484164 queries its entry count.
struct CScnItemAnimResFile {
    u8 _00[0xC];                 // 0x00..0x0C
    nw4r::g3d::ResFile resFile;  // 0x0C
};

// Sub-object at CScnItemModel+0x824 (0x10 bytes): owns a MemManager-backed
// buffer (field_0) guarded by an ownership sentinel (field_C == 0xFFFFFFFF
// means the buffer is not owned). field_4 is the buffer's element count
// (walked by func_80484734); field_8 is cleared on destroy. Its destructor
// is the fragment symbol __dt__8048268C.
struct CScnItemModel824 {
    u8* field_0;   // 0x00 owned buffer (released when owned)
    u32 field_4;   // 0x04 element count of the buffer
    u8* field_8;   // 0x08
    u32 field_C;   // 0x0C ownership sentinel (0xFFFFFFFF = not owned)
};

// 4-byte buffer element owned by CScnItemModel824; func_80484734 toggles
// the u16 flag at offset 2.
struct CScnItemModel824Entry {
    u16 field_0;  // 0x00
    u16 flags;    // 0x02
};

class CScnItemModel {
public:
    // Constructed via the fragment function __ct__CScnItemModel (defined in
    // CScnItemModel.cpp): the retail ctor installs the vtable label
    // explicitly after two member stores, which a real member ctor cannot
    // reproduce (MWCC would emit the generated __vt__ symbol first).

    // 51 base virtuals (CScnItem pool item): slots 0x08..0xD0 (MWCC prepends
    // a 2-entry RTTI header to the vtable, so declared virtual N lands at
    // offset (N+1)*4). CScnItemModel's own virtuals follow: vfuncD4 (0xD4,
    // slot-list link notify called on the node with the parent as argument,
    // see func_804858C8), vfuncD8 (0xD8, model-link notify, see
    // func_80485994), and the destructor (0xDC) - 54 methods in total,
    // matching the retail vtable size of 0xE0.
    virtual void vfunc08();
    virtual void vfunc0C();
    virtual void vfunc10();
    virtual void vfunc14();
    virtual void vfunc18();
    virtual void vfunc1C();
    virtual void vfunc20();
    virtual void vfunc24();
    virtual void vfunc28(u32 a, u32 b);  // vtable 0x28 (2-arg notify)
    virtual int vfunc2C(u32 param);  // vtable 0x2C (1-arg query, non-zero = handled)
    virtual void vfunc30(u32 a, u32 b);  // vtable 0x30 (2-arg notify)
    virtual int vfunc34(u32 param);  // vtable 0x34 (1-arg query, non-zero = handled)
    virtual void vfunc38();
    virtual void vfunc3C();
    virtual void vfunc40();
    virtual void vfunc44();
    virtual void vfunc48(float f);  // vtable 0x48 (1-arg float notify, see func_80484838)
    virtual void vfunc4C();
    virtual void vfunc50();
    virtual void vfunc54();
    virtual void vfunc58();
    virtual void vfunc5C();
    virtual void vfunc60();
    virtual void vfunc64(u32 a);  // vtable 0x64 (1-arg notify, see func_804831C4)
    virtual void vfunc68();
    virtual void vfunc6C();
    virtual void vfunc70();
    virtual void vfunc74();
    virtual void vfunc78();
    virtual void vfunc7C();
    virtual void vfunc80();
    virtual void vfunc84();
    virtual void vfunc88();
    virtual void vfunc8C(u32 a);  // vtable 0x8C (1-arg notify, see func_804849E4)
    virtual int vfunc90(u32 a, u32 b);  // vtable 0x90 (2-arg query, non-zero = handled; see func_80484AB4)
    virtual void vfunc94(u32 a);  // vtable 0x94 (1-arg notify, see func_80484BB4)
    virtual void vfunc98();
    virtual void vfunc9C(u32 a, u32 b);  // vtable 0x9C (2-arg notify)
    virtual void vfuncA0();
    virtual void vfuncA4(u32 a);  // vtable 0xA4 (1-arg notify, see func_804838DC)
    virtual void vfuncA8();
    virtual void vfuncAC(CScnItemModel* node);  // vtable 0xAC (1-arg notify, see func_804831C4)
    virtual void vfuncB0(CScnItemModel* node);  // vtable 0xB0 (1-arg notify, see func_80483448)
    virtual void vfuncB4(u32 a);  // vtable 0xB4 (1-arg notify, see func_80484914)
    virtual void vfuncB8();
    virtual void vfuncBC();
    virtual void vfuncC0();
    virtual void vfuncC4();
    virtual void vfuncC8();
    virtual void vfuncCC();
    virtual void vfuncD0();  // vtable 0xD0
    virtual void vfuncD4(CScnItemModel* parent);  // vtable 0xD4 (slot-list link notify, called on the node)
    virtual void vfuncD8(CScnItemModel* node);  // vtable 0xD8 (model-link notify)
    virtual ~CScnItemModel();  // vtable 0xDC

    CScnItemModelOwner* field_04;  // 0x04 owner pointer
    u16 value08;                   // 0x08 (u16 type marker, set to 1 by the ctor)
    u8 _0A[0x2];                   // 0x0A..0x0C
    u8 field_0xC[0x1EC];           // 0x0C..0x1F8 (sub-object passed to func_80497724/90)
    u8 field_0x1F8[0x5AC];         // 0x1F8..0x7A4 (act-data base returned by func_8048315C)
    u32 flags7A4;                  // 0x7A4
    u32 flags7A8;                  // 0x7A8
    f32 value7AC;                  // 0x7AC
    f32 value7B0;                  // 0x7B0
    CScnItemModel* slots7B4[4];    // 0x7B4..0x7C4 (reference list, iterated by func_80485A48/98/84C84)
    CScnItemModel* field_0x7C4;    // 0x7C4
    CScnItemModel* field_0x7C8;    // 0x7C8
    f32 value7CC;                  // 0x7CC
    f32 value7D0;                  // 0x7D0
    f32 value7D4;                  // 0x7D4
    f32 value7D8;                  // 0x7D8
    f32 value7DC;                  // 0x7DC
    f32 value7E0;                  // 0x7E0
    u32 value7E4;                  // 0x7E4
    u32 value7E8;                  // 0x7E8
    u32 field_7EC;                 // 0x7EC
    u32 field_0x7F0[0xC];          // 0x7F0..0x820
    int count820;                  // 0x820 (signed: retail cmpw/cmpwi compares)
    CScnItemModel824 member824;    // 0x824..0x834 (buffer holder, dtor __dt__8048268C)
    CScnItemModel* slots834[4];    // 0x834..0x844 (second reference list, iterated by func_80484C84)
    u32 field_844;                 // 0x844
    u32 field_848;                 // 0x848
    u32 field_84C;                 // 0x84C
    u32 field_850;                 // 0x850
    u32 field_854;                 // 0x854
    f32 value858;                  // 0x858
    u32 field_85C;                 // 0x85C
    u32 field_860;                 // 0x860
};

// Opaque light-environment sub-object at CScnItemModel+0x31C (inside the
// act-data region). Its destructor (__dt__804BF6F4) is defined in
// code_804BF59C.cpp; this TU only needs a typed handle to destroy it.
struct CScnItemModelEnvLight {
    u8 data[0x420];
};

// Cross-TU imports (retail C-ABI free functions defined in
// CScn_80496B0C.cpp). extern "C" keeps the call reloc names verbatim
// (reloc-site gate; MWCC would otherwise mangle C++ linkage names).
extern "C" float func_80497724(u8* self);
extern "C" float func_80497790(u8* self);
extern "C" float func_804977C0(u8* self);
extern "C" f32 func_804977F0(u8* self);
extern "C" u32 func_8049715C(u8* self);
extern "C" void func_80497760(u8* self, float value);
extern "C" void func_8049782C(u8* self, u32 tag);
extern "C" void func_8049771C(u8* self, float val);
extern "C" void func_80497AA8(u8* self);
extern "C" u32 func_804972E8(u8* self, CScnItemAnimResFile* anim, int index,
                              u32 c, s32 f);
extern "C" u32 func_80497190(u8* self, CScnItemAnimResFile* anim, int index,
                              u32 c, s32 f, u32 g, u32 h);
extern "C" u32 func_804978B8(u8* self);

// Cross-TU query: returns the model owned by the given scene owner's pool
// (called by func_804831C4; defined in CScn_80496B0C.cpp).
extern "C" CScnItemModel* func_80495E94(CScnItemModelOwner* owner);

// Fragment-anchored destructors of sub-objects destroyed by the
// CScnItemModel dtor (defined in code_804BF59C.cpp / CScn_80496B0C.cpp).
// extern "C" keeps the call reloc names verbatim (reloc-site gate: MWCC
// would otherwise mangle them to __dt__804BF6F4__FPvi).
extern "C" void* __dt__804BF6F4(void* self, int deleting);
extern "C" void* __dt__80496BB4(void* self, int flags);

// Fragment-anchored constructor of the anim sub-object built by the
// CScnItemModel ctor (defined in CScn_80496B0C.cpp). extern "C" keeps the
// call reloc name verbatim. (The light-env ctor __ct__804BF5A0 is declared
// in code_804BF59C.hpp, already included by the TU.)
extern "C" void* __ct__80496B0C(void* self);

// View of the act-data region at CScnItemModel+0x1F8 (0x5AC bytes). The
// region starts with the embedded ml::CAttrTransform; the two floats touched
// by func_80485CE8 are named fields (abs offsets 0x2E8 / 0x304). Only cast
// into, never constructed.
struct CScnItemModelActData {
    ml::CAttrTransform transform;     // 0x1F8..0x2DC
    f32 f2DC[3];                     // 0x2DC..0x2E8
    f32 value2E8;                    // 0x2E8
    f32 f2EC[6];                     // 0x2EC..0x304
    f32 value304;                    // 0x304
    f32 f308[5];                     // 0x308..0x31C
    CScnItemModelEnvLight envLight;  // 0x31C..0x73C (destroyed by the model dtor)
    u8 _73C[0x68];                   // 0x73C..0x7A4
};

// EU TV-format float constants (.sdata2). const: MWCC hoists/schedules the
// sdata2 loads like retail (MWCC_CASES §174-184); a plain extern float
// forces reloads per use and can swap the assigned FPRs.
extern const float lbl_eu_8066A8C0;
extern const float lbl_eu_8066A8C4;
extern const float lbl_eu_8066A8C8;
extern const float lbl_eu_8066A8CC;

// Retail vtable data for CScnItemModel (.data at 0x8056DD70, 0xE0 bytes).
// Declared as a char array so the ctor can install the label explicitly.
extern u32 lbl_eu_8056DD70[];

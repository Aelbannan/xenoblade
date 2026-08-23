#pragma once

#include <types.h>
#include <revolution/MEM.h>  // MEMAllocator member at +0x16AC
#include <nw4r/math.h>
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include "libs/monolib/src/scn/CMdlAnmEye.hpp"
#include "monolib/scn/code_804BF59C.hpp"  // CScnEnvLgtData + func_804BFFB8 (env-light update)

class CScnVirtualLight;  // full definition in CScnVirtualLight.hpp (func_80493C30 arg)

// nw4r g3d types used as pointers only here (complete definitions are pulled
// in by the .cpp where members are dereferenced).
namespace nw4r {
namespace g3d {
class ScnObj;
class ScnGroup;
class ScnMdlExpand;
class ResTev;
struct ResMdlData;
}
} // namespace nw4r

// Forward declarations for cross-TU callees (defined in CScnRootNw4r.cpp;
// only the call-site types are needed here).
class CScnRootNw4r;
struct CScnCamLayout;
class CScn;
class CScnItemModelNw4r;

// Opaque sub-object at CScnItemModelNw4r+0x16C8 (the CMdlMaterial instance,
// 0x38 bytes; see CMdlMaterial.hpp). Kept opaque so the virtual-dtor member
// type does not drag dtor/vtable codegen into this TU.
struct CScnItemModelNw4rSub16C8 {
    u8 data[0x38];
};

// 12-byte block at CScnItemModelNw4r+0x76C: an 8-byte value pair plus a
// 4-byte tail. func_80488CF8 copies it as u64-pair + u32-tail (OSLaunch
// titleId pattern: loads w0,w1 then stores w1(+4),w0(+0)).
struct CScnItemModelNw4rBlock76C {
    u64 pair;   // 0x76C..0x774
    u32 tail;   // 0x774..0x778
};

// Frame-output slot written by func_8048917C: a u16 value at +6 (the
// current frame-table entry, fed to the CMdlLook handle). Only the +6
// member is known.
struct CScnItemModelNw4rFrameOut {
    u8 _00[0x6];
    u16 field_6;   // 0x06
};

// Owner/scene-object handle (prefix mirrors CScnItemModelOwner's layout).
// +0x5C is the virtual-light object fed by func_80489584 (func_80493C30
// arg); +0x7C is the light-env handle read by func_804899F4; +0x8C is the
// scene-root handle passed to the camera-hookup helpers func_8048F7A8 /
// func_8048F630 by func_8048BADC / func_8048BBF0.
struct CScnItemModelNw4rOwner {
    u8 _00[0x5C];                   // 0x00..0x5C
    CScnVirtualLight* field_0x5C;   // 0x5C (func_80493C30 arg, func_80489584)
    u8 _60[0x18];                   // 0x60..0x78
    u8* field_0x78;                 // 0x78 (fog manager, func_8049DE74 arg in the ctor)
    u8* field_7C;                   // 0x7C (light-env handle, func_804899F4)
    u8 _80[0x4];                    // 0x80..0x84
    // Fade-control sub-object at +0x84 (f32 fade value at +8, read by
    // func_80487EE0's fade state machine).
    struct {
        u8 _00[0x8];
        f32 value8;   // +0x08
    }* field_0x84;                  // 0x84
    u8 _88[0x4];                    // 0x88..0x8C
    CScnRootNw4r* field_0x8C;       // 0x8C (scene root, func_8048F7A8 arg)
    u8 _90[0x24];                   // 0x90..0xB4
    u8* field_0xB4;                 // 0xB4 (distance-check object, func_804885FC)
    u8 _B8[0x32C];                  // 0xB8..0x3E4
    u8 field_0x3E4;                 // 0x3E4 (busy flag read by func_8048A0B4)
};

// Opaque env-light sub-object at CScnItemModelNw4r+0x31C (0x420 bytes; the
// real type is CScnItemModelEnvLight from CScnItemModel.hpp). The busy byte
// at abs 0x3E4 is read by func_80489C94 before building a new model.
struct CScnItemModelNw4rEnvLight {
    u8 data[0xC8];              // 0x31C..0x3E4
    u8 field_0x3E4;             // abs 0x3E4 (busy flag, func_80489C94)
    u8 data2[0x420 - 0xC9];     // 0x3E5..0x73C
};

// Scale-source object at CScnItemModelNw4r+0x14AC: a 4x3 matrix view.
// +0x00 is a Vec (PSVECMag input in func_80489584); the translation column
// at +0xC / +0x1C / +0x2C is copied to the +0x310 triple by func_804884F8
// and to the +0x2DC bounding vec by func_80489584. The whole first 0x30
// bytes are copied into the act-data mMtx1 by func_80489584.
struct CScnItemModelNw4r14AC {
    nw4r::math::VEC3 vec0;  // +0x00 (PSVECMag input, func_80489584)
    f32 f0xC;    // 0x0C
    u8 _10[0xC];
    f32 f0x1C;   // 0x1C
    u8 _20[0xC];
    f32 f0x2C;   // 0x2C
};

// Opaque view of the g3d scene object's scale slot (ScnObj is 0xDC bytes;
// ScnLeaf::mScale lands at +0xDC). func_8048856C calls SetMtx through the
// real ScnObj type and writes the scale through this view.
struct CScnItemModelNw4rScnScale {
    u8 _00[0xDC];
    f32 mScale[3];   // 0xDC..0xE8
};

// Opaque view of the g3d scene model's per-node world-matrix array slot
// (ScnMdlSimple::mpWorldMtxArray lands at +0xEC). func_8048B728 /
// func_8048B68C index it by the node's mtxID (stride 0x30 = sizeof MTX34).
struct CScnItemModelNw4rScnMdlView {
    u8 _00[0xEC];
    nw4r::math::MTX34* mpWorldMtxArray;   // 0xEC
};

// Virtual-dispatch view for the vtable-0x74 slot of the model's primary
// vtable (declared virtual N lands at 8+4*N, so v27 -> 0x74). Used by
// func_80489014 to query the model's own "handled" state; the real virtual
// is CScnItemModel::vfunc74 (declared void in the shared header, so a local
// int-returning view is required for the call-site test).
struct CScnItemModelNw4rV74 {
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual int v27() = 0;   // vtable 0x74
};

// Virtual-dispatch views for the anim-object handles at +0x1484 / +0x1488 /
// +0x148C: vtable-0x24 (f32 arg) on the +0x1484 handle and vtable-0x28 on
// the +0x1488/+0x148C handles, used by func_80487EE0's fade push.
struct CScnItemModelNw4rAnimV24 {
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07(f32 f) = 0;   // vtable 0x24
};

struct CScnItemModelNw4rAnimV28 {
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08(f32 f) = 0;   // vtable 0x28
};

// Opaque view of the g3d scene object's +0x122 u16 slot (frame-table entry
// copy by func_80487EE0).
struct CScnItemModelNw4rScnObj122 {
    u8 _00[0x122];
    u16 field_0x122;   // +0x122
};

// Opaque views of the g3d scene object's callback-handle slots: +0xD4
// (ScnObj::mpFuncObjExec) and +0x11C (ScnMdlSimple::mpCalcWorldCallback), set
// by the ctor to the sub-object vtable anchors (+0x1468 / +0x1464).
struct CScnItemModelNw4rScnObjD4 {
    u8 _00[0xD4];
    void* field_0xD4;   // +0xD4 (mpFuncObjExec)
};

struct CScnItemModelNw4rScnMdl11C {
    u8 _00[0x11C];
    void* field_0x11C;  // +0x11C (mpCalcWorldCallback)
};

// Virtual-dispatch view for the scene-root handle returned by func_8048ECD0:
// the vtable-0x2C slot returns the MemManager handle used for the CMdlLook
// fallback allocation (declared virtual N lands at 8+4*N, so v09 -> 0x2C).
struct CScnItemModelNw4rRootHandle {
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual u32 v09() = 0;   // vtable 0x2C (MemManager handle)
};

// Node-id collection list passed to func_80491764 (mirrors
// CScnTexWorkManNodeIdList: array at +0x0, count at +0x4).
struct CScnItemModelNw4rNodeIdList {
    u32* array;   // +0x0
    u32 count;    // +0x4
};

// Minimal view of the anim sub-object at CScnItemModelNw4r+0xC (the retail
// CScn80496B0C from CScn_80496B0C.cpp; only the +0x0 owner pointer is
// checked by func_80489014 before passing the object to func_804979A4).
struct CScnItemModelNw4rAnimC {
    void* field_0x0;   // +0x00 owner (CScn*)
};

// Virtual-dispatch view for the vtable-0x20 slot of the func_80496264 result
// (declared virtual N lands at 8+4*N, so v06 -> 0x20). Used by func_804885FC
// to query the owner's node at the -1 index.
struct CScnItemModelNw4rV20 {
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual u32 v06(void* arg) = 0;   // vtable 0x20 (func_804885FC distance check)
    virtual void v07() = 0;
};

// Virtual-dispatch view for the vtable-0x10 slot of the owner's +0xB4
// distance-check object (v02 -> 0x10). func_804885FC passes the +0x2DC vec
// and the +0x2E8 radius; a non-zero return re-hides the model (bit 16).
struct CScnItemModelNw4rB4V10 {
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual u32 v02(void* arg, f32 f) = 0;   // vtable 0x10
    virtual void v03() = 0;
};

// View of the func_8048ECD8 scene-root result: only the +0x2888 slot that
// func_80487818 compares against the +0x1484 anm-scene object is known.
struct CScnItemModelNw4rRoot2888 {
    u8 _00[0x2888];     // 0x00..0x2888
    u32 field_0x2888;   // 0x2888
};

// 4-byte node-name handle returned by the func_8048B1F4 name-resolve helper
// (mirrors the nw4r ResName wrapper shape: a single const char*).
struct CScnItemModelNw4rName {
    const char* p;
};

// Virtual-dispatch view for the CScnItemModelNw4r primary vtable (declared
// virtual N lands at 8+4*N, MWCC RTTI header). Only the slots used by this
// TU's call sites are declared with their observed return types; the
// pure-virtual chain keeps the offsets exact.
struct CScnItemModelNw4rVtbl {
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual u32 v04() = 0;   // vtable 0x18 (func_80489C94: returns list id)
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual u32 v14(u32 arg) = 0;   // vtable 0x40 (func_80489E80: takes priority, returns link id)
    virtual void v15() = 0;
    virtual void v16() = 0;  // vtable 0x48 (func_80489E80)
    virtual void v17() = 0;  // vtable 0x4C (func_80489E80)
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0;
    virtual void v33() = 0;
    virtual void v34() = 0;
    virtual void v35() = 0;
    virtual void v36() = 0;
    virtual void v37() = 0;
    virtual void v38() = 0;
    virtual void v39() = 0;
    virtual void* v40() = 0;  // vtable 0xA8 (func_80489C94 tail call; retail re-uses its r3 result as the return value)
    virtual void v41() = 0;
    virtual void v42() = 0;
    virtual void v43() = 0;
    virtual void v44() = 0;  // vtable 0xB8 (func_80487818)
    virtual void v45() = 0;
    virtual u32 v46(u32 param, u32 nodeVal, u32 arg6) = 0;  // vtable 0xC0 (func_80489FDC tail)
};

// Virtual-dispatch view for the +0x7F0 hook-table entries (vtable-0xC slot
// called with the model as the sole arg by func_80489584).
struct CScnItemModelNw4rHookV0C {
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02(CScnItemModelNw4r* self) = 0;   // vtable 0xC
};

// Virtual-dispatch view for the CMdlLook handle at CScnItemModelNw4r+0x17C8.
// The vtable-0x8 slot is the first virtual (the dtor) called with the delete
// flag by func_80487818.
struct CScnItemModelNw4rLook {
    virtual void v00(int deleting) = 0;   // vtable 0x8 (dtor)
};

// 0x20-byte shadow-node entry at CScnItemModelNw4r+0x17DC (two entries;
// func_804873EC fills/clears one from a named resource node).
struct CScnItemModelNw4rShadowNode {
    ml::CVec3 vecA;     // +0x00 (zeroed when set)
    ml::CVec3 vecB;     // +0x0C (zeroed when set)
    u16 field_0x18;     // +0x18 (set to 0xA)
    u16 field_0x1A;     // +0x1A (set to 0x5A)
    s16 id;             // +0x1C (resource node id, -1 = free)
    u16 field_0x1E;     // +0x1E (cleared when set)
};

class CScnItemModelNw4r {
public:
    CScnItemModelNw4r();
    virtual ~CScnItemModelNw4r();

    // Layout recovered from retail accessors in this TU. The 0x04..0x864
    // prefix mirrors CScnItemModel's member offsets.
    /* 0x04 */ CScnItemModelNw4rOwner* field_04;     // owner (same slot as CScnItemModel::field_04)
    /* 0x08 */ u8 _pad_0x08[0x4];                    // to 0x0C
    /* 0x0C */ u8 field_0xC[0x54];                   // CScn80496B0C anim sub-object prefix (owner ptr at +0x0)
    /* 0x60 */ u8* field_0x60;                       // scene-item pool (func_8048C5B8/8C630 arg, func_80489C94)
    /* 0x64 */ u8 field_0x64[0x194];                 // rest of the anim sub-object (to 0x1F8)
    /* 0x1F8 */ u8 _pad_0x1F8[0x48];                 // to 0x240
    /* 0x240 */ nw4r::math::MTX34 field_0x240;       // matrix pushed to the g3d scene object (SetMtx)
    /* 0x270 */ u8 _pad_0x270[0x6C];                 // to 0x2DC
    /* 0x2DC */ f32 field_0x2DC[3];                  // transform output vec (PSMTXMultVec dst, func_804885FC)
    /* 0x2E8 */ f32 field_0x2E8;                     // distance/radius scaled by max scale (func_804885FC)
    /* 0x2EC */ f32 field_0x2EC[6];                  // two 3-float groups summed by func_804885FC
    /* 0x304 */ f32 field_0x304;                     // scale triple copied into the ScnObj
    /* 0x308 */ f32 field_0x308;
    /* 0x30C */ f32 field_0x30C;
    /* 0x310 */ f32 field_0x310;                     // scale/pos triple (bit-copied from mPrevPos)
    /* 0x314 */ f32 field_0x314;
    /* 0x318 */ f32 field_0x318;
    /* 0x31C */ CScnItemModelNw4rEnvLight field_0x31C;  // env-light sub-object (0x420 bytes)
    /* 0x73C */ u8 _pad_0x73C[0x30];                 // to 0x76C
    /* 0x76C */ u32 field_0x76C;                     // 12-byte block copied by func_80488CF8
    /* 0x770 */ u32 field_0x770;
    /* 0x774 */ u32 field_0x774;
    /* 0x778 */ u8 _pad_0x778[0x14];                 // to 0x78C
    /* 0x78C */ f32 field_0x78C;                     // color quadruple (func_80488D54 table copy)
    /* 0x790 */ f32 field_0x790;
    /* 0x794 */ f32 field_0x794;
    /* 0x798 */ f32 field_0x798;
    /* 0x79C */ u8 _pad_0x79C[0x4];                  // to 0x7A0
    /* 0x7A0 */ u32 field_0x7A0;                     // flag word (bits 0x8/0x10 set by func_80488D54)
    /* 0x7A4 */ u32 field_0x7A4;                     // flag word (bit 27 = 0x08000000 via func_8048BD04)
    /* 0x7A8 */ u32 field_0x7A8;                     // flag word (bit 8 synced by func_8048BBF0)
    /* 0x7AC */ f32 field_0x7AC;                     // radius scaled by max scale (func_804885FC)
    /* 0x7B0 */ u8 _pad_0x7B0[0x4];                  // to 0x7B4
    /* 0x7B4 */ CScnItemModelNw4r* slots7B4[4];      // reference list (same slot as CScnItemModel::slots7B4)
    /* 0x7C4 */ CScnItemModel* field_0x7C4;          // chain link (same slot as CScnItemModel::field_0x7C4)
    /* 0x7C8 */ void* field_0x7C8;                   // secondary attached model (func_80489200)
    /* 0x7CC */ nw4r::math::VEC3 vec7CC;             // translate VEC3 - func_80489014
    /* 0x7D8 */ nw4r::math::VEC3 vec7D8;             // second translate VEC3 (func_80489014)
    /* 0x7E4 */ u8 _pad_0x7E4[0xC];                  // to 0x7F0
    /* 0x7F0 */ u32 field_0x7F0[0xC];                // per-frame hook table (12 entries; iterated by func_80489584)
    /* 0x820 */ int field_0x820;                     // hook-table count (signed: retail cmpw)
    /* 0x824 */ CScnItemModel824 member824;          // 0x824..0x834 buffer holder (dtor __dt__804871B0)
    /* 0x834 */ CScnItemModelNw4r* slots834[4];     // second reference list (same slot as CScnItemModel::slots834)
    /* 0x844 */ u32 field_844[4];                    // per-slot link ids (same slot as CScnItemModel::field_844..850)
    /* 0x854 */ u32 field_854;                       // link id (same slot as CScnItemModel::field_854)
    /* 0x858 */ f32 field_0x858;                     // fade value compared by func_8048A588
    /* 0x85C */ u32 field_0x85C;                     // buffer flag word (bit 2/3 = node table carved from the static pool)
    /* 0x860 */ u32 field_0x860;                     // buffer used-bytes counter (0xC00 pool, see func_80488938)
    /* 0x864 */ u8 _pad_0x864[0xC08];                // to 0x146C
    /* 0x146C */ nw4r::g3d::ResMdlData* field_0x146C; // model resource data (name at +0x48)
    /* 0x1470 */ nw4r::g3d::ScnGroup* field_0x1470;  // parent scene group (Remove children by func_80487818)
    /* 0x1474 */ nw4r::g3d::ScnGroup* field_0x1474;  // scene group receiving inserted scene objects (Insert)
    /* 0x1478 */ nw4r::g3d::ScnGroup* field_0x1478;  // second scene group (Insert at Size by func_80488A28)
    /* 0x147C */ nw4r::g3d::ScnObj* field_0x147C;    // g3d scene object (SetMtx + scale slot)
    /* 0x1480 */ nw4r::g3d::ScnGroup* field_0x1480;  // scene group (Remove by func_8048A0B4)
    /* 0x1484 */ void* field_0x1484;                 // G3dObj (anm scene; released via ScnRoot::RemoveAnmScn)
    /* 0x1488 */ void* field_0x1488;                 // G3dObj (anm obj; removed from ScnMdl + Destroy)
    /* 0x148C */ void* field_0x148C;                 // G3dObj (anm obj; removed from ScnMdl + Destroy)
    /* 0x1490 */ void* field_0x1490;                 // G3dObj (Destroy by func_80487818)
    /* 0x1494 */ void* field_0x1494;                 // node table buffer (static pool or heap)
    /* 0x1498 */ void* field_0x1498;                 // second node table buffer (static pool or heap)
    /* 0x149C */ u16 field_0x149C;                    // cleared by the ctor
    /* 0x149E */ u16 field_0x149E;                    // u16 flag cleared by func_8048776C
    /* 0x14A0 */ u32 field_0x14A0;                    // node index compared by func_80489014
    /* 0x14A4 */ u8 _pad_0x14A4[0x4];                 // to 0x14A8
    /* 0x14A8 */ u32 field_0x14A8;                    // link id (-1 = unlinked, func_8048A0B4)
    /* 0x14AC */ CScnItemModelNw4r14AC* field_0x14AC; // scale-source object (floats at +0xC/+0x1C/+0x2C)
    /* 0x14B0 */ u32 field_0x14B0;                   // link id written by func_80489E80
    /* 0x14B4 */ nw4r::math::VEC3 field_0x14B4;      // translate result (GetTranslate by func_80489014)
    /* 0x14C0 */ u32 field_0x14C0;                  // self-pointer to the +0x14C4 effect sub-object (ctor)
    /* 0x14C4 */ u8 field_0x14C4[0x18C];             // CScnEffectActNw4r sub-object
    /* 0x1650 */ u8 field_0x1650[0x54];              // CScn::FvMaruShadowNw4r sub-object (to 0x16A4)
    /* 0x16A4 */ void* field_0x16A4;                 // allocator alloc fn (func_8048BD1C, ctor)
    /* 0x16A8 */ void* field_0x16A8;                 // allocator free fn (func_8048BD50, ctor)
    /* 0x16AC */ MEMAllocator allocator;             // nw4r MEMAllocator (funcs=&field_0x16A4, heap=0, heapParam1=this)
    /* 0x16BC */ f32 field_0x16BC;                   // fade latch (func_80487EE0 state machine)
    /* 0x16C0 */ s16 field_0x16C0;                   // param u16 (ctor; -1 default -> 7)
    /* 0x16C2 */ u16 field_0x16C2;                   // u16 flag cleared by func_804885FC
    /* 0x16C4 */ u16 field_0x16C4;                   // frame counter (u16, advanced by func_8048917C)
    /* 0x16C6 */ u8 _pad_0x16C6[0x2];                // to 0x16C8
    /* 0x16C8 */ CScnItemModelNw4rSub16C8 field_0x16C8;  // CMdlMaterial sub-object (0x38 bytes)
    /* 0x1700 */ u8 field_0x1700[0x30];              // CMdlMouth sub-object
    /* 0x1730 */ u8 field_0x1730[0x40];              // CMdlAnmUV sub-object
    /* 0x1770 */ CMdlAnmEye field_0x1770;            // eye-anim state machine (0x30 bytes)
    /* 0x17A0 */ u8 field_0x17A0[0x28];              // CMdlDynamics sub-object
    /* 0x17C8 */ s32 field_0x17C8;                   // handle null-checked by func_80488FEC / func_80489000
    /* 0x17CC */ u32* field_0x17CC;                  // frame table buffer (released by the dtor)
    /* 0x17D0 */ u32 field_0x17D0;                   // frame table count
    /* 0x17D4 */ u32 field_0x17D4;
    /* 0x17D8 */ u32 field_0x17D8;                   // frame-table id (-1 = none; dtor sentinel)
    /* 0x17DC */ CScnItemModelNw4rShadowNode shadowNodes[2];  // 0x17DC..0x181C (shadow nodes, func_804873EC)
};

// Cross-TU C-linkage placeholders (retail symbols are unmangled func_* names
// until symbol recovery renames them; keep the C linkage to match relocs).
extern "C" void func_804E8220(s32 v);
extern "C" void func_804E8284(s32 v);
extern "C" void func_8048310C(CScnItemModelNw4r* self, u32 enable);
extern "C" void func_80484C84(CScnItemModelNw4r* self);
extern "C" void func_804849E4(CScnItemModelNw4r* self, u32 param);
extern "C" void func_80484BB4(CScnItemModelNw4r* self);
extern "C" int func_804E6358(CScnItemModelNw4rSub16C8* sub);
extern "C" int func_804E5FD4(CScnItemModelNw4rSub16C8* sub);

// func_80488C78 / func_804888B4 cross-TU imports (defined in CMdlMaterial.cpp
// / CScnItemModel.cpp). extern "C" keeps the retail func_* names verbatim
// (MWCC mangles class-typed params). func_80488D54 passes a zero-color vec
// and a mode flag as the trailing args (defaulted so the 1-arg call site in
// func_80488C78 keeps its bytes).
extern "C" int func_804E6158(CScnItemModelNw4rSub16C8* sub, ml::CVec3* vec = 0,
                             u32 param = 0);
extern "C" int func_80484AB4(CScnItemModelNw4r* self, u32 a, u32 b);
extern "C" int func_804858C8(CScnItemModelNw4r* self, CScnItemModelNw4r* node);
// Model-link registration (defined in CScnItemModel.cpp; rejects an
// already-linked node). extern "C" keeps the retail func_* name verbatim.
extern "C" int func_80485994(CScnItemModelNw4r* self, CScnItemModelNw4r* node);
// Chain-wide 1-arg query (defined in CScnItemModel.cpp; vtable-0x34 on the
// reference-list models).
extern "C" int func_80485C28(CScnItemModelNw4r* self, u32 param);
extern "C" void func_80482918(CScnItemModelNw4r* self, u32 param);
// CMdlLook handle feed (defined in CMdlLook.cpp): (handle, model, frame,
// param).
extern "C" void func_804E8290(s32 handle, CScnItemModelNw4r* self, u32 frame,
                              u32 param);
extern "C" void func_804829E8(CScnItemModelNw4r* self, u32 param);

// Cross-TU imports used by func_8048776C / func_8048B54C / func_80488B50:
// model release (defined in CScn.cpp), chain-wide 2-arg notify (defined in
// CScnItemModel.cpp), material sub-object notify (defined in CMdlMaterial.cpp).
extern "C" bool func_80495E60(CScnItemModel* self);
extern "C" void func_80485B98(CScnItemModel* self, u32 a, u32 b);
extern "C" void func_804E64B0(CScnItemModelNw4rSub16C8* sub, u32 param,
                              CScnItemModelNw4r* obj);

// Act-data base getter (walks the chain-last node, returns node+0x1F8;
// defined in CScnItemModel.cpp).
extern "C" ml::CAttrTransform* func_8048315C(CScnItemModelNw4r* self);
// Env-light propagation (defined in CScnItemModel.cpp): with a null env,
// self's own +0x31C env data is pushed to every linked model.
extern "C" void func_80485804(CScnItemModel* self, CScnEnvLgtData* param);
// Per-frame virtual-light update (defined in CScnVirtualLight.cpp). Declared
// here with the 5-arg retail shape (like CScnVirtualLight.hpp re-declares
// func_804BFA70 over the code_804BF59C.hpp 4-arg stub; the two headers
// cannot both be included because of that conflict).
extern "C" void func_80493C30(CScnVirtualLight* self, CScnEnvLgtData* data,
                              const ml::CVec3* vec, int mode, f32 f1);
// Fade-distance refresh (defined in CScnItemModel.cpp).
extern "C" void func_80485D64(CScnItemModelNw4r* self);
// Eye-anim state setter (defined in CMdlAnmEye.cpp).
extern "C" void func_804E77BC(CMdlAnmEye* self, u32 val);
// Light-env init / clear (defined in CScnEnvLgtCtrl.cpp).
extern "C" void func_804C1720(u8* self, CScnItemModelNw4rEnvLight* lgt);
extern "C" void func_804C172C(u8* self);

// Retail destructor symbol is the unmangled member name; written as a
// fragment function (like __dt__13CScnItemModelFv) so the vtable label is
// installed explicitly and no compiler-generated __vt__ reloc appears.
extern "C" CScnItemModelNw4r* __dt__17CScnItemModelNw4rFv(CScnItemModelNw4r* self,
                                                           int deleting);

// Cross-TU sub-object hooks (defined in CMdlDynamics.cpp; retail names are
// unmangled func_* symbols).
extern "C" void func_804EB798(u8* self);
extern "C" void func_804EB7F8(u8* self);

// func_80487818 cross-TU imports: anim sub-object teardown (defined in
// CScn_80496B0C.cpp), pool deregistration (CScnItemModel.cpp), CMdlDynamics
// release (CMdlDynamics.cpp), the scene root anm-scene removal (g3d_scnroot)
// and the +0x16C8 sub-object destructor fragment.
extern "C" void func_80496D74(void* self);
extern "C" void func_804830AC(CScnItemModel* self);
extern "C" void func_804EB8A0(u8* self);
// CMdlDynamics world-matrix sync (defined in CMdlDynamics.cpp), called by
// func_80489200 after the shadow-matrix copy pass.
extern "C" void func_804EBBCC(void* self, nw4r::math::MTX34* mtxs);
// Shadow-matrix propagation tail of func_80489200 (defined later in this TU).
void func_8048AB2C(CScnItemModelNw4r* self, nw4r::math::MTX34* worldMtxBase);
extern "C" void* func_8048ECD8(void* self);
extern "C" void* RemoveAnmScn__Q34nw4r3g3d7ScnRootFv(void* self);
extern "C" void* __dt__804E5DE0(void* self);

// Retail vtable data for CScnItemModelNw4r (.data at 0x8056DE80, 0x608B).
// Defined in this TU; callers cast to u8* explicitly.
extern "C" u32 lbl_eu_8056DE80[];

// .sbss byte: one-shot allocator-path switch read/cleared by the ctor (the
// first constructed model uses the func_8048ED04 path, later ones
// func_8048ECE4). Owned by monolibdata2.o.
extern u8 lbl_eu_806658D8;

// .sdata2 constants used by the ctor: 30.0f (radius flag threshold) and the
// 2^52 u32->double conversion magic.
extern const f32 lbl_eu_8066A8E8;
extern const f64 lbl_eu_8066A8F0;

// PlayPolicy assert strings (.data): file / format for the
// AnmScnRes::SetPlayPolicy address-range check.
extern char lbl_eu_8056E03C[];
extern char lbl_eu_8056E008[];

// Node-name pointer (.sdata) the ctor resolves with ResMdl::GetResNode(PCc)
// for the +0x14A0 node index / frame-table setup.
extern const char* lbl_eu_806638D4;

// Cross-TU callees used by the flag-sync functions (func_8048BADC /
// func_8048BBF0): bit-21/bit-8 flag setters (CScnItemModel.cpp) and the
// camera-hookup helpers (CScnRootNw4r.cpp).
extern "C" void func_804830BC(CScnItemModelNw4r* self, u32 enable);
extern "C" void func_804830E4(CScnItemModelNw4r* self, u32 enable);
extern "C" bool func_8048F7A8(CScnRootNw4r* self, CScnCamLayout* cam);
extern "C" s32 func_8048F630(CScnRootNw4r* self, CScnCamLayout* cam);

// Reference-list notify (defined in CScnItemModel.cpp): vtable-0x28 on every
// live model in the 4-slot list and the +0x7C4 link.
extern "C" void func_80485A48(CScnItemModel* self, u32 a, u32 b);

// Anim sub-object helpers used by func_80489014 (defined in
// CScn_80496B0C.cpp / CMdlAnmUV.cpp / CMdlMouth.cpp).
extern "C" void func_804979A4(void* self, ml::CVec3* outA, ml::CVec3* outB,
                              ml::CVec3* outC, u32 nodeIdx);
extern "C" int func_804E72D0(void* self, u32 arg2, void* res);
extern "C" int func_804E68A0(void* self, u32 arg2, void* res);

// func_80487EE0 per-frame anim helpers (defined in CMdlAnmEye.cpp /
// CMdlMouth.cpp): eye-anim / mouth-anim refresh.
extern "C" void func_804E77C4(void* self);
extern "C" void func_804E6A28(void* self);
// Fade-distance walk (defined in CScnItemModel.cpp): last-chain fade value.
extern "C" float func_80484EB0(void* self);
// Fade refresh (defined in CScnItemModel.cpp).
extern "C" void func_80485CE8(CScnItemModelNw4r* self);
// Handled-state queries (defined in CScnItemModel.cpp).
extern "C" int func_804844D0(CScnItemModelNw4r* self);
extern "C" int func_804842B0(CScnItemModelNw4r* self);
// Flag-bit setter for func_80487EE0 (defined in CScnItemModel.cpp).
extern "C" void func_80483134(CScnItemModelNw4r* self, u32 enable);
// Material fade apply (defined in CScnItemModel.cpp), func_8048A588 tail.
extern "C" void func_80484838(CScnItemModelNw4r* self, f32 param);
// CMdlDynamics per-frame refresh (defined in CMdlDynamics.cpp), func_80487EE0
// +0x17A0 hook.
extern "C" void func_804EBAE8(void* self);

// Sub-object destructors invoked by the model dtor (retail fragment symbols;
// defined in their own TUs).
extern "C" void* __dt__12CMdlDynamicsFv(void* self, int deleting);
extern "C" void* __dt__10CMdlAnmEyeFv(void* self, int deleting);
extern "C" void* __dt__9CMdlAnmUVFv(void* self, int deleting);
extern "C" void* __dt__9CMdlMouthFv(void* self, int deleting);
extern "C" void* __dt__12CMdlMaterialFv(void* self, int deleting);
extern "C" void* __dt__4CScnFvMaruShadowNw4r(void* self, int deleting);
extern "C" void* __dt__17CScnEffectActNw4rFv(void* self, int deleting);
extern "C" CScnItemModel* __dt__13CScnItemModelFv(CScnItemModel* self,
                                                  int deleting);

// func_8048A588 material/tev assert strings (.data) + varargs (.sdata) and
// the 255.0f alpha constant (.sdata2).
extern char lbl_eu_8056E0D0[];   // tev null assert file
 extern char lbl_eu_8056E0B0[];
 extern const char lbl_eu_8056E068[0x10];  // mat null assert file
 extern const char lbl_eu_8056E04C[0x1A];
 extern char lbl_eu_8056E0A0[];  // pix misaligned assert file
 extern char lbl_eu_8056E078[];
extern char lbl_eu_806638F4;     // tev null vararg (.sdata)
 extern char lbl_eu_806638FC;
 extern const char lbl_eu_806638E8[7];
 extern char lbl_eu_806638F0;
extern const float lbl_eu_8066A8E4;   // 255.0f (func_8048A588 alpha scale)
// func_80487EE0 fade clamp minimum (.sdata2).
extern const float lbl_eu_8066A900;
extern char lbl_eu_8056E130[];   // panic file path (.data)
extern char lbl_eu_8056E110[];   // panic format string (.data)
extern char lbl_eu_8056E1C8[];   // node-id assert file path (.data)
extern char lbl_eu_8056E1A8[];   // node-id assert format string (.data)
extern char lbl_eu_80663900;     // panic vararg string (.sdata, @sda21)
extern char lbl_eu_8066390C;     // panic vararg string (.sdata, @sda21)
extern char lbl_eu_8056E194[];   // node-resolve panic file path (.data)
extern char lbl_eu_8056E178[];   // node-resolve panic format string (.data)
extern char lbl_eu_80663910;     // node-resolve panic vararg string (.sdata, @sda21)
extern char lbl_eu_80663918;     // node-resolve panic vararg string (.sdata, @sda21)
extern char lbl_eu_80529678[];   // node-buffer panic file path (.data)
extern char lbl_eu_80529658[];   // node-buffer panic format string (.data)

// func_8048A17C nw4r ResUserData accessor assert strings (.data) + varargs
// (.sdata, @sda21) + the searched name pointer (.sdata).
extern char lbl_eu_80530D18[];   // user-data null assert file
 extern char lbl_eu_80530CFC[];
 extern char lbl_eu_80530CF0[];
extern char lbl_eu_8066391C;     // user-data null assert vararg
 extern char lbl_eu_80530F08[];   // dic misaligned assert file
 extern char lbl_eu_80530EE0[];
extern char lbl_eu_8056E43C[];   // dic null assert file (bounds path)
 extern char lbl_eu_8056E420[];
extern char lbl_eu_80663928;     // dic null assert vararg
 extern char lbl_eu_80663930;
extern char lbl_eu_8056E3D0[];   // idx-out-of-range assert file
 extern char lbl_eu_8056E398[];
extern char lbl_eu_80530D54[];   // item misaligned assert file (0x26)
 extern char lbl_eu_80530D2C[];
extern char lbl_eu_80530DC4[];   // item null assert file #1
 extern char lbl_eu_80530DA8[];
 extern char lbl_eu_80530D68[];
extern char lbl_eu_80663924;     // item null assert vararg #1
 extern char lbl_eu_80530E1C[];   // valueType assert file (0x36)
 extern char lbl_eu_80530DD8[];
extern char lbl_eu_80530D94[];   // item null assert file #2
 extern char lbl_eu_80530D78[];
extern char lbl_eu_80663920;     // item null assert vararg #2
// func_8048A17C searched user-data item name (.sdata pointer; the string it
// points to is .data). Loaded with lwz @sda21, passed to strcmp.
extern const char* lbl_eu_806638D0;

// Node-buffer setup / owner-memory helpers (defined in CScnItemModel.cpp /
// CScn.cpp): set the model's working param, resolve the owner's memory
// handle.
extern "C" void func_804857DC(CScnItemModelNw4r* self, u32 param);
extern "C" void func_804857F0(CScnItemModelNw4r* self, u32 param);
extern "C" u32 func_80496018(CScnItemModelNw4rOwner* owner);
// Flag-bit setter for the ctor's effect-act registration (defined in
// CScnItemModel.cpp): syncs the +0x7A4 bit-30 flag to `param`.
extern "C" void func_804838DC(CScnItemModel* self, u32 param);
// Node lookup by name (defined in CScnTexWorkMan.cpp; returns the model
// resource's ResNode).
extern "C" nw4r::g3d::ResNode func_80490AF4(void* self, const char* name);

// func_80489E80 / func_80488D54 / func_80487C78 / func_80489C94 cross-TU
// imports (defined in CScnItemModel.cpp / CScn.cpp / CScnItemPool.cpp).
extern "C" void func_80484D3C(CScnItemModelNw4r* self, u32 a, u32 b);
extern "C" void func_80484914(CScnItemModel* self, u32 param);
extern "C" u32 func_80495FF0(CScnItemModelNw4r* self);
extern "C" u32 func_8048C5B8(u8* pool, s32 kind);
extern "C" u32 func_8048C630(u8* pool, CScnItemModelNw4r* model, u32 flag);

// g3d free-function accessors bound to the retail mangled symbols (defined
// in g3d_scnmdl.cpp / g3d_scnmdlexpand.cpp; the shared g3d_scnmdl.h does not
// declare them). extern "C" keeps the mangled identifiers verbatim.
extern "C" nw4r::g3d::ResTev
    GetResTevEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(void* self);
extern "C" nw4r::g3d::ResMatPix
    GetResMatIndMtxAndScaleEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(
        void* self);
extern "C" nw4r::g3d::ResMatTevColor
    GetResMatTexCoordGenEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(void* self);
extern "C" bool PushBack__Q34nw4r3g3d12ScnMdlExpandFPQ34nw4r3g3d6ScnObjUl(
    nw4r::g3d::ScnMdlExpand* group, nw4r::g3d::ScnObj* obj, u32 nodeID);

// Fragment-anchored constructor (retail symbol __ct__CScnItemModelNw4r is a
// bare fragment name, unlike the mangled dtor; extern "C" keeps it verbatim).
// The ResFile travels as a raw data pointer (the retail callee receives the
// value in a register - a by-value ResFile class would force MWCC's
// hidden-reference ABI and shift the call-site codegen).
extern "C" CScnItemModelNw4r* __ct__CScnItemModelNw4r(
    void* pMem, CScnItemModelNw4r* pSrc, nw4r::g3d::ResMdl* pResMdl,
    void* resFileData, u32 param, u32 flags);

// func_80488D54 mode/color data: 12-float .data table (4 groups of 3) and
// the .sdata2 zero-color constant the query vector is inited with.
extern const float lbl_eu_8056DE50[12];
extern const float lbl_eu_8066A8FC;   // 1.0f (func_80489584 scale denominator)
extern const float lbl_eu_8066A8E0;   // 0.0f (func_80489584 zero/threshold)

// func_804885FC half-scale constant (.sdata2, 0.5f) used to halve the summed
// bounding-box extents before scaling by the model scale.
extern const float lbl_eu_8066A904;

// nw4r FSqrt assert source-file + message (.data) used by func_80489584's
// column-normalize Warning (retail file/line of the inlined FSqrt helper).
extern char lbl_eu_80526324[];
extern char lbl_eu_80526300[];

// Owner memory/nodelist lookup (defined in CScn.cpp): resolve the owner's
// node at `idx` (-1 = default). func_804885FC calls vtable-0x20 on the
// result.
extern "C" void* func_80496264(void* rsrc, int idx);
// Owner scale getter (defined in CScn.cpp), func_8048AB2C fade scale input.
extern "C" f32 func_80496288(CScnItemModelNw4rOwner* owner);

// func_8048AB2C shadow-math constants (.sdata2).
extern const float lbl_eu_8066A908;  // short-delta length-squared threshold
 extern const float lbl_eu_8066A90C;  // normalize extra scale
 extern const float lbl_eu_8066A910;  // -1.0f (Y clamp step)

// func_80489C94 ResFile-alignment panic strings (.data).
extern char lbl_eu_8052637C[];
extern char lbl_eu_80526354[];

// ===========================================================================
// Constructor (__ct__CScnItemModelNw4r) cross-TU callees. All retail symbols
// here are unmangled fragment names; extern "C" keeps the call-site reloc
// names verbatim.
// ===========================================================================

// Base CScnItemModel constructor fragment (defined in CScnItemModel.cpp).
extern "C" CScnItemModel* __ct__CScnItemModel(CScnItemModel* self,
                                              CScnItemModelOwner* owner,
                                              u32 param);

// Sub-object constructors (defined in their own TUs; retail names are the
// bare fragment ctor names).
extern "C" void* __ct__CScnEffectActNw4r(void* self);
extern "C" void* __ct__CScnMaruShadowNw4r(void* self);
extern "C" void* __ct__CMdlMaterial(void* self);
extern "C" void* __ct__CMdlMouth(void* self);
extern "C" void* __ct__CMdlAnmUV(void* self);
extern "C" void* __ct__CMdlAnmEye(void* self);
extern "C" void* __ct__CMdlDynamics(void* self);
extern "C" void* __ct__CMdlLook(void* self);

// CScn allocator/root getters (defined in CScnMem.cpp): resolve the owner's
// MEMAllocator variants and the g3d scene root.
extern "C" void* func_8048ED04(CScn* self);
extern "C" void* func_8048ECE4(CScn* self);
extern "C" void* func_8048ECF4(CScn* self);
extern "C" u32 func_8048ECD0(CScn* self);

// Sub-object init hooks (retail unmangled fragment names): CMdlDynamics
// (func_804E95E0), CMdlMaterial (func_804E54B8 / func_804E5990), CMdlMouth
// (func_804E65CC), CMdlAnmUV (func_804E6C80), CMdlAnmEye (func_804E75B8),
// CMdlLook (func_804E7B38), CScnMaruShadowNw4r (func_8048E69C),
// CScnEffectActNw4r (func_8049B9EC), anim sub-object (func_80496F14).
extern "C" void func_804E95E0(void* self, void* model);
extern "C" void func_804E54B8(void* self, void* model);
extern "C" void func_804E5990(void* self, void* node, void* mtx);
extern "C" void func_804E65CC(void* self, void* model);
extern "C" void func_804E6C80(void* self, void* model);
extern "C" void func_804E75B8(void* self, void* model);
extern "C" void func_804E7B38(void* self, void* model);
extern "C" void func_8048E69C(void* self, u32* out, void* model);
extern "C" void func_8049B9EC(void* self, void* mgr);
extern "C" void func_80496F14(void* self, CScn* scn, void* model, u16 slot);

// Node-table helpers (defined in CScnTexWorkMan.cpp): count the non-degenerate
// nodes under a ResNode and collect their ids into a node-id list.
extern "C" int func_80491158(nw4r::g3d::ResNode* pNode);
extern "C" void func_80491764(nw4r::g3d::ResNode* pNode,
                              CScnItemModelNw4rNodeIdList* pList);

// Fog-manager param push (defined in CScnFogMan.cpp): (fogMan, fogType, color
// vec, near/far params).
extern "C" void func_8049DE74(void* self, u32 value, const f32* src, f32 p0,
                              f32 p1, f32 p2, f32 p3);

// nw4r g3d scene-root anm-scene attach (defined in g3d_scnroot.cpp; not
// declared in the shared g3d_scnroot.h).
extern "C" bool SetAnmScn__Q34nw4r3g3d7ScnRootFPQ34nw4r3g3d6AnmScn(
    nw4r::g3d::ScnRoot* self, nw4r::g3d::AnmScn* pAnmScn);

// ScnMdlExpand construction (defined in g3d_scnmdlexpand.cpp; the shared
// header only declares the class).
extern "C" nw4r::g3d::ScnMdlExpand*
    Construct__Q34nw4r3g3d12ScnMdlExpandFP12MEMAllocatorPUlUlPQ34nw4r3g3d12ScnMdlSimple(
        MEMAllocator* pAllocator, u32* pSize, u32 maxNumChildren,
        nw4r::g3d::ScnMdlSimple* pMdl);

// Runtime throw helper (NMWException.h is not included; noreturn elides the
// __end__catch epilogue of the ctor's catch-all handler).
extern "C" __declspec(noreturn) void __throw(char* throwtype, void* location,
                                              void* dtor);

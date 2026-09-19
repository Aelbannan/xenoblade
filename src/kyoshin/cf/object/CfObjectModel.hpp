#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObject.hpp"
#include "monolib/math/CVec3.hpp"

// Retail data labels referenced by this unit.
extern const float lbl_eu_80666A68;   // CfObject_UnkVirtualFunc20 constant / CfObject_UnkVirtualFunc56 fallback
extern float lbl_eu_80666A6C;   // CfModel_NotifyReady position constant (stack-vector fill)
extern const float lbl_eu_80666A70;   // UnkVirtualFunc25 first probe FP arg
extern const float lbl_eu_80666A74;   // UnkVirtualFunc25 probe second FP arg
extern const float lbl_eu_80666A78;   // UnkVirtualFunc25 lift-offset Y element
extern const float lbl_eu_80666A7C;   // UnkVirtualFunc25 lifted re-probe FP arg
extern const float lbl_eu_80666A80;   // CfObject_UnkVirtualFunc20 (model) stack-vector middle element
extern const float lbl_eu_80666A84;   // CfObject_UnkVirtualFunc20 (model) slot +0xB4 scale argument
extern const float lbl_eu_8066A210;   // CfObject_setScaledHeadAngle scale factor
extern const float lbl_eu_8066A20C;   // CfObject_UnkVirtualFunc34 scale factor
extern u8 lbl_eu_804FC548[];   // CfObjectModel_getModelName null placeholder (rodata, 8 bytes)
extern u8 lbl_eu_80529318[];   // cf::CfObject vtable (retail .data, stored by the model dtor)
extern u8 lbl_eu_805294E0[];   // cf::CfObjectModel vtable (retail .data)
extern const char lbl_eu_80529678[];   // Panic file (node-lookup assert, line 0x53)
extern const char lbl_eu_80529658[];   // Panic fmt (CfObject_UnkVirtualFunc52/53)

namespace nw4r {
namespace g3d {
struct ResMdlData;  // forward decl (full type in nw4r/g3d/res/g3d_resmdl.h)
}
}

// Retail ml static zero vector (unmangled symbol; ml::CVec3 comes from
// monolib/math/CVec3.hpp, already included transitively).
extern ml::CVec3 zero__Q22ml5CVec3;

// Effect-act owner for the +0xA8/+0x44 chain (CfObject_UnkVirtualFunc55):
// +0xA8 on the Nw4r scene model returns the embedded effect-act object,
// whose +0x44 slot is the id-table lookup. Mirror of
// libs/monolib/src/scn/CScnEffectActNw4r (same name, layout, and slot
// order; kept in sync by hand). The monolib header cannot be included
// from this TU: its extern "C" free decls (Scn_IsAnimActiveOrNull, simGetLeafActData,
// simSetFlag2OnTree) pin narrower arities that collide with this TU's
// retail-accurate FULL-matching decls (notably the 2-arg simSetFlag2OnTree
// and the CActParamAnim void*-shaped simGetLeafActData). Slot map dumped from
// the retail vtable lbl_eu_8056EAD8
// (build/us/asm/monolib/src/scn/CScnEffectActNw4r.s); every impl is
// decomp'd in CScnEffectActNw4r.cpp. The class is novtable (ctor/dtor are
// fragments installing the table manually), so this mirror emits nothing;
// virtual calls dispatch by slot, never by member symbol.
// PROMOTE: move these decls onto the monolib class and delete this mirror
// once the free-decl arity conflict is resolved wave-wide.
struct CScnEffectAct;  // defined in the monolib header; pointer use only
class __declspec(novtable) CScnEffectActNw4r {
public:
    CScnEffectActNw4r();
    virtual ~CScnEffectActNw4r();
    virtual void EffectActNw4rProbeAnimActive();                        // +0x0C release manager
    virtual void EffectActNw4rMgrCallV04();                        // +0x10 manager v4 notify
    virtual void EffectActNw4rSetLeafDist();                        // +0x14 manager detach
    virtual void EffectActNw4rSetTreeFlag2();                        // +0x18 manager visibility
    virtual void EffectActNw4rCallMgrV43();                        // +0x1C manager v43 notify
    virtual void EffectActNw4rCallMgrV16();                        // +0x20 manager v16 query
    virtual u8* EffectActNw4rGetLeafActData();                         // +0x24 manager act-data base
    virtual void EffectActNw4rStoreMgrScalePos(const void* other);       // +0x28 copy position triplet
    virtual u8* func_8049BF0C() const;                   // +0x2C act slot +0x48
    virtual u8* func_8049BF34() const;                   // +0x30 act slot +0x78
    virtual u8* func_8049BF5C() const;                   // +0x34 act slot +0xb8
    virtual u8* func_8049BF84() const;                   // +0x38 act slot +0xc4
    virtual CScnEffectAct* EffectActNw4rFindActGated(u32 idx) const; // +0x3C act lookup (flag-gated)
    // +0x40 pin: the slot holds free-function EffectActNw4rGetActPosOut whose retail
    // convention is (out, self, idx), not thiscall; this shape only
    // reserves the position (nothing dispatches +0x40 via it).
    virtual void EffectActNw4rGetActPosOut(ml::CVec3* out, u32 idx) const; // +0x40 act position triplet
    virtual CScnEffectAct* EffectActNw4rFindActById(u32 idx) const; // +0x44 id-table act lookup

    /* 0x04 */ void* mpMgr;
    /* 0x08 */ CScnEffectAct* mActs[64];
    /* 0x108 */ u16 mIds[64];
    /* 0x188 */ u32 mCount;
};

namespace cf {
    // Sub-object at CfObjectModel+0x98: flag words read by CfModel_GetFlag7A8
    // (bit 0 of field_7A8) and CfObject_UnkVirtualFunc69 (bit 1 of field_7A4).
    // Per-node matrix table reached through CfObjectModelSub98+0x147C: its
    // +0xEC word is the base of 0x30-byte matrix slots (CfObject_UnkVirtualFunc52/53).
    struct CfObjectModelActTable {
        u8 _padEC[0xEC];
        u8* field_EC;  // 0xEC
    };
    struct CfObjectModelSub98 {
        u8 _pad00[0x7A4];             // 0x00-0x7A3
        u32 field_7A4;                // 0x7A4
        u32 field_7A8;                // 0x7A8
        u8 _pad7AC[0x146C - 0x7AC];   // 0x7AC-0x146B
        nw4r::g3d::ResMdlData* field_146C;  // 0x146C (model resource data)
        u8 _pad1470[0x147C - 0x1470]; // 0x1470-0x147B
        CfObjectModelActTable* field_147C;  // 0x147C
        u8 _pad1480[0x14AC - 0x1480]; // 0x1480-0x14AB
        u32 field_14AC;               // 0x14AC (read by CfObject_UnkVirtualFunc54)
    };
    // Tiny iface for the +0xB0 resource-impl word: DELETED (Batch F). The
    // slot +0x08 table head is the owning impl class's deleting destructor
    // (CfResTboxImpl, vptr at +0x10 via its prefix); the dtor now uses
    // delete (CfResTboxImpl*)mSubObjB0 (cf. CfObjectTbox ctor, MWCC_PATTERNS
    // "Foreign sub-object dtor slot"), so no view is needed here.
    // (The +0xA8/+0x44 effect-act chain likewise goes through the real
    // owner ::CScnEffectActNw4r, mirrored at global scope above.)


    //min size: 0xbe
    class CfObjectModel : public CfObject {
    public:
        //vtable 1 (CfObject)
        virtual ~CfObjectModel();
        //vtable 1 (CfObjectModel)
        // Wave-64: UVF1/19 primaries flipped to behavior names (same as
        // Wave-62 for the rest of this block). Hand-built vtables / Fv
        // free-function bodies keep the Unk mangled linker names
        // (CREvtModel / CfCollSphereImpl / Map stubs). Unk* below are
        // non-virtual same-arity aliases. UVF1 stays an out-of-line
        // non-virtual member so Move's Class::UnkVirtualFunc1 call and
        // the Model hand-table Fv keep the retail Unk symbol (0x0 spare:
        // no extra trampoline). Locked CfObject-base Unk Fv names that
        // Model overrides (UVF24/28/34/35/36/52/54/55/67/68/69/70/72)
        // stay Unk on CfObject.hpp: Coll.hpp still redeclares those Unk
        // spellings (out of scope); Point's local mirror already flipped.
        virtual void CfObjectModel_releaseModelSub();  //0x178 (teardown: release mSubObj98)
        virtual void CfObjectModel_releaseModelList();  //0x17C (teardown: release model lists; Move overrides with real work)
        void CfObjectModel_UnkVirtualFunc2() { CfObjectModel_releaseModelList(); }  // alias for legacy callers (same arity)
        virtual void* CfObjectModel_getModelName();  //0x180 (retail returns a pointer)
        // Wave-62: promote behavior aliases to the virtual decls. Hand-built
        // vtables / Fv free-function bodies keep the Unk mangled linker names;
        // Unk* below are non-virtual same-arity aliases.
        virtual void* CfObjectModel_getAnimState();  //0x184
        virtual u32 CfObjectModel_getAnimFlags();  //0x188
        virtual void* CfObjectModel_checkTargetNode(void* arg);  //0x18C (retail returns a pointer)
        virtual void CfObjectModel_snapSubTarget(ml::CVec3* pos, float scale);  //0x190 (retail forwards r4/f1 to sub snapMoveTarget)
        virtual u32 CfObjectModel_getSubFlag04();   //0x194 (bit 4 of sub +0x7A4)
        virtual float CfObjectModel_getModelFloatA8();  //0x198 (read +0xA8)
        virtual void CfObjectModel_setModelFloatA8(float value); //0x19C (store +0xA8)
        virtual float CfObjectModel_getModelRate(); //0x1A0 (read +0xAC; pairs with setModelRate)
        virtual void CfObjectModel_setModelRate(float value); //0x1A4
        virtual void CfObjectModel_aimAtPosition(const ml::CVec3* pos); //0x1A8 (retail passes a stack vector)
        virtual void CfObjectModel_bindModelTo(cf::CfObject* other, const char* name); //0x1AC
        // Retail lookAt leaves (actor, name) live into the 0x1B0 bctr and a
        // booleanized int into 0x1B4 (ocUnit.cpp); impls are Fv-named free
        // functions with hidden args (snapToBone forwards to bindModelTo,
        // setBoneRotate stores field_BD). Decls carry the retail arity.
        virtual void CfObjectModel_snapToBone(cf::CfObject* other, const char* name); //0x1B0
        virtual void CfObjectModel_setBoneRotate(int flag); //0x1B4
        virtual u32 CfObjectModel_getBindSource(); //0x1B8 (field_B4 bind source pointer as u32)
        virtual void CfObjectModel_detachEffectSlot(void* arg); //0x1BC
        virtual void CfObjectModel_setModelVisible(int flag); //0x1C0 (mFlags68 bit 21; Move walks +0x6F8)
        virtual u32 CfObjectModel_getModelVisible(); //0x1C4 (+0x68 bit 21; pairs with setModelVisible)
        // Non-virtual same-arity aliases: legacy Unk spellings + densest
        // call-site names. MWCC inlines each into the identical virtual
        // dispatch. Fv linker bodies / hand-built vtables keep Unk mangled names.
        // UVF1: out-of-line non-virtual (def in .cpp) = retail Fv body + Move
        // Class:: call target; must NOT be an inline alias (would recolor Move).
        void CfObjectModel_UnkVirtualFunc1(); //0x178 Fv body (hand table / Move Class::)
        void* CfObjectModel_UnkVirtualFunc4() { return CfObjectModel_getAnimState(); } //0x184
        u32 CfObjectModel_UnkVirtualFunc5() { return CfObjectModel_getAnimFlags(); } //0x188
        void CfObjectModel_UnkVirtualFunc12(float value) { CfObjectModel_setModelRate(value); } //0x1A4
        void CfObjectModel_UnkVirtualFunc13(const ml::CVec3* pos) { CfObjectModel_aimAtPosition(pos); } //0x1A8
        void CfObjectModel_UnkVirtualFunc14(cf::CfObject* other, const char* name) { CfObjectModel_bindModelTo(other, name); } //0x1AC
        void CfObjectModel_lookAtBone(cf::CfObject* other, const char* name) { CfObjectModel_bindModelTo(other, name); } //0x1AC
        void CfObjectModel_UnkVirtualFunc15(cf::CfObject* other, const char* name) { CfObjectModel_snapToBone(other, name); } //0x1B0
        void CfObjectModel_UnkVirtualFunc16(int flag) { CfObjectModel_setBoneRotate(flag); } //0x1B4
        void CfObjectModel_UnkVirtualFunc18(void* eff) { CfObjectModel_detachEffectSlot(eff); } //0x1BC
        void CfObjectModel_UnkVirtualFunc19(int flag) { CfObjectModel_setModelVisible(flag); } //0x1C0
        void CfObjectModel_UnkVirtualFunc7(ml::CVec3* pos, float scale) { CfObjectModel_snapSubTarget(pos, scale); } //0x190
        u32 CfObjectModel_UnkVirtualFunc8() { return CfObjectModel_getSubFlag04(); } //0x194
        float CfObjectModel_UnkVirtualFunc9() { return CfObjectModel_getModelFloatA8(); } //0x198
        void CfObjectModel_UnkVirtualFunc10(float value) { CfObjectModel_setModelFloatA8(value); } //0x19C
        float CfObjectModel_UnkVirtualFunc11() { return CfObjectModel_getModelRate(); } //0x1A0
        u32 CfObjectModel_UnkVirtualFunc17() { return CfObjectModel_getBindSource(); } //0x1B8
        u32 CfObjectModel_UnkVirtualFunc20() { return CfObjectModel_getModelVisible(); } //0x1C4

        //0x0: vtable
        // CfObject ends at 0x70.
        void* mTarget70;              // 0x70 (first word of field_0x70)
        u8 field_0x74[0x1C - 4];      // 0x74-0x8F
        u16 unk8C_3;
        u16 field_0x8E;
        u8 field_0x90[0x8];          // 0x90-0x97
        CfObjectModelSub98* mSubObj98; // 0x98-0x9B
        u8 field_0x9C[0x4];           // 0x9C-0x9F
        float field_A0;               // 0xA0-0xA3
        u8 field_0xA4[0xC];           // 0xA4-0xAF
        void* mSubObjB0;      // 0xB0-0xB3
        cf::CfObjectModel* field_B4; // 0xB4 (source object stored by bindModelTo)
        const char* field_B8;        // 0xB8 (name/fallback label stored by bindModelTo)
        u8 field_BC;          // 0xBC
        u8 field_BD;          // 0xBD
    CfObjectModel();
    void CfObject_UnkVirtualFunc2();  // kept: CfObjectObj.cpp calls it qualified (links to the Fv free function)
    // (17 vestigial Unk member decls deleted: those slot bodies are verbatim
    // Fv free functions referenced straight from the manual vtables below;
    // the member decls were undefined and uncallable, so this is a no-op.)
    void CfObjectModel_teardownModel();
    void CfObject_notifyEventDone();
    virtual u32 CfObject_readKindFlagWord() override;
    void CfObject_setMoveTargetVec();
    ml::CVec3* CfObject_getPosVector();
    void CfObject_UnkVirtualFunc27(void* src);
    void CfObject_setMoveHeadAngle();
    void CfObject_setScaledHeadAngle();
    float CfObject_UnkVirtualFunc34();
    void* CObjectParam_UnkVirtualFunc2();
    void setPointEnabled(int flag); //0x158 override (base renamed; keeps CfObjectModel non-abstract)
    int CfObject_UnkVirtualFunc68(const ml::CVec3* vec = nullptr) override;
    float CfObject_getMoveHeadAngle();
    };
}

// Output vector pair filled by TexMan_GetDefaultXf_0A44 (monolib scene helper used by
// CfObject_UnkVirtualFunc56: the two positions' Y difference is returned).
struct CfObjectModelVec3 {
    float x;  // 0x00
    float y;  // 0x04
    float z;  // 0x08
};

// Imports (retail uses unmangled names; extern "C" so the linkage matches
// the CfObjectMap.hpp declarations of the same helpers).
// Releases a sub-object (used by CfObjectModel_UnkVirtualFunc1).
extern "C" void Scn_IsAnimActiveOrNull(void* ptr);
// Fills the two output vectors for the model sub-object (used by
// CfObject_UnkVirtualFunc56).
extern "C" void TexMan_GetDefaultXf_0A44(cf::CfObjectModelSub98* obj, CfObjectModelVec3* outA, CfObjectModelVec3* outB);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Function is defined in CfBdat.cpp with C linkage (retail uses unmangled name)
extern "C" void func_80142428();

// C-linkage import from libs/monolib/src/scn/CScnItemModel.cpp (retail uses
// the unmangled name); tail-called by CfModel_SyncVisFlag with the model sub-object.
extern "C" void simSetFlag2OnTree(cf::CfObjectModelSub98* model, int flag);

// operator delete (retail symbol __dl__FPv is the unmangled C name; declare
// extern "C" so the bl reloc references exactly __dl__FPv).
extern "C" void __dl__FPv(void* object);

// Imports for CfObjectModel_UnkVirtualFunc13 (sets sub-object +0x7A8 flag
// bits; defined in libs/monolib CScnItemModel.cpp) and CfModel_InstallSub
// (reattaches a detached effect target; defined in CfObjectEff.cpp).
// extern "C" so the bl relocs reference the unmangled retail names (a plain
// C++ header declaration makes MWCC emit a mangled __F<params> name).
extern "C" void simSetFlags7A8_12(void* self, u32 a, u32 b);
// Effect-chain helpers (defined in CfObjectEff.cpp). C linkage keeps
// the call-site reloc at the plain retail name (a C++ declaration makes
// MWCC emit the mangled __FPUc reference).
extern "C" void notifyDetach_(void* self);
extern "C" void reattachTrg__(void* a, void* b);
extern "C" void detachTrgPrt_(u8* arg);


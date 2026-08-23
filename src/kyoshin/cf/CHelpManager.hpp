#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"
#include "kyoshin/help/CHelp_LandMark.hpp"

namespace cf {

class CHelp_EtherMakeTIPS : public CHelp {
public:
    void func_80295D8C();
};

class CHelp_KizunaTIPS : public CHelp {
public:
    void func_80295D9C();
};

class CHelp_AlwaysTIPS : public CHelp {
public:
    void func_80295DAC();
};

class CHelp_TalkTIPS : public CHelp {
public:
    void func_80295D98();
};

class CHelp_EndEventTIPS : public CHelp {
public:
    void func_80295DA0();
};

// 8-byte list holder embedded at CHelpManager +0x0 and +0x8. Holds a
// NULL-terminated array of pointers to CHelp-derived objects; each element's
// interface-table pointer sits at +0x8 (see CHelp).
struct CHelpListHolder {
    void** mItems; // 0x0
    u8 mFlag;      // 0x4
    u8 pad_5[3];   // 0x5..0x7
};

// Zero a list holder (retail func_802968A8; defined in CHelpManager.cpp).

// Data-only base so the C++ vptr lands at +0x1C: MWCC places the vptr at the
// first virtual declaration, and retail's deleting-dtor `delete` path reads
// the vptr from +0x1C (lwz r12, 0x1c(r3) / lwz r12, 0x8(r12)).
struct CHelpManagerData {
    CHelpListHolder mListA; // 0x0
    CHelpListHolder mListB; // 0x8
    s32 mField10;           // 0x10 (signed: retail compares with cmpw/cmp)
    u8 mField14;            // 0x14
    u8 mField15;            // 0x15
    u8 mLearnArtsFlag;      // 0x16
    u8 mField17;            // 0x17
    u8 mField18;            // 0x18
    u8 pad_19[3];           // 0x19..0x1B
};

// novtable: the retail ctor stores the vtable label (lbl_eu_8053911C) into the
// SELF-ALLOCATED instance by hand, and the vtable itself lives in retail .data.
class __declspec(novtable) CHelpManager : public CHelpManagerData {
public:
    CHelpManager();
    virtual ~CHelpManager(); // vptr at 0x1C
};

// Layout mirror of CHelpManager used to install the retail vtable at +0x1C
// (the class is __declspec(novtable), so the ctor writes the vptr explicitly).
struct CHelpManagerVtblSlot : CHelpManagerData {
    u8* mVptr; // 0x1C
};

// Cast-only dispatch interface mirroring the CHelp manual interface table
// (vptr at +0x8; MWCC hides two typeinfo slots, so the Nth declared virtual
// lands at vtable +0x8 + 4*N == CHelpVtbl::mSlots[2+N]). Never constructed.
class CHelpDispatchIface {
public:
    u32 mOwner; // 0x0
    u32 mParam; // 0x4
    virtual void Slot2(); // vtable+0x8 (mSlots[2])
    virtual void Slot3(); // vtable+0xC (mSlots[3])
    virtual int Slot4();  // vtable+0x10 (mSlots[4])
};

// 8-byte entry of the scenario-help table (lbl_eu_80538E90): mKey is compared
// against CHelpManager::mField10; mItems is a NULL-terminated CHelp array.
struct CHelpTableEntry {
    s32 mKey;      // 0x0 (signed: retail compares with cmpw)
    void** mItems; // 0x4
};

// Static help-table entry view (lbl_eu_80576D08): CHelp-shaped object whose
// interface-table pointer sits at +0x8; dispatched as a virtual call through
// this polymorphic view so retail's r12 call sequence is emitted.
class CHelpTblView : public CHelpVtblPrefix {
public:
    virtual void f08(); // vtable 0x08
    virtual void f0C(); // vtable 0x0C - slot func_80295BF4 drives
    virtual void f10(); // vtable 0x10
};

// Never-instantiated derivative; its existence keeps MWCC from
// devirtualizing the CHelpTblView calls below (retail stays indirect).
struct CHelpTblViewDerived : CHelpTblView {};

// Static help-dispatch table (lbl_eu_80576D08) embedding the CHelp objects
// func_80295BF4 drives when mField10 is 3 / 5.
struct CHelpManagerTbl {
    u8 pad_00[0x20];
    cf::CHelpTblView mHelp1; // 0x20 (mField10 == 3)
    u8 pad_2C[0x24];
    cf::CHelpTblView mHelp2; // 0x50 (mField10 == 5)
    u8 pad_5C[0x4];
    cf::CHelpTblView mHelp3; // 0x60 (mField10 == 5)
};

// Battle-object argument for func_80295CC8 (flags word at +0x3F08).
struct CHelpBattleObjArg {
    u8 pad[0x3F08]; // 0x0..0x3F07
    u32 mField3F08; // 0x3F08
};

// Battle-manager actor-list view used by func_80295CC8: mirrors
// monolib's reslist/_reslist_base at CBattleManager+0x44 (mActorList3) so
// the inlined size() walk reproduces retail codegen.
struct CHelpBmListNode {
    CHelpBmListNode* mNext; // 0x0
    CHelpBmListNode* mPrev; // 0x4
};
struct CHelpBmResList {
    void* mVptr;                    // +0x00 (virtual dtor)
    CHelpBmListNode* mStartNodePtr; // +0x04
    CHelpBmListNode mStartNode;     // +0x08
    u32 size() const {
        CHelpBmListNode* curNode;
        CHelpBmListNode* endNode;

        u32 length;
        endNode = this->mStartNodePtr;
        length = 0;
        curNode = this->mStartNodePtr->mNext;

        while (curNode != endNode) {
            length++;
            curNode = curNode->mNext;
        }

        return length;
    }
};
struct CHelpBmView44 {
    u8 pad_00[0x44];       // 0x0..0x43
    CHelpBmResList mList3; // 0x44 (mActorList3)
};

// Minimal cf::CBattleManager stand-in so func_80295CC8 can call the real
// static member: its mangled name is exactly the retail import
// getInstance__Q22cf14CBattleManagerFv. Only the fields the helper touches
// are modeled (actor list at +0x44).
class CBattleManager {
public:
    static CBattleManager* getInstance();
    u8 pad_00[0x44];         // 0x0..0x43
    CHelpBmResList mActorList3; // 0x44
};

// Data-only shift base (mOwner@0, mParam@4) so the C++ vptr lands at +0x8
// (retail reads the interface table pointer from object+8) and the first
// virtual lands at vtable+0x8 - the slot func_802968B8 dispatches.
struct CHelpTIPSShift {
    void* mOwner; // 0x0
    u32 mParam;   // 0x4
};

class CHelpTIPS : public CHelpTIPSShift {
public:
    virtual void UnkVirtual1(); // vtable+0x8
};

// --- Static help-table entry shapes (lbl_eu_80576D08) ---
// The TU static initializer (sinit_80295DB0) builds every entry of the help
// table: base CHelp ctor call, then interface-table (vtable) override at +0x8
// and the extra payload fields. These mirrors only model the fields the sinit
// writes after the ctor (nothing is ever read back through them).

// 0x10-byte entry: extra word at +0xC.
struct CHelpWordC : public CHelp {
    u32 mFieldC; // 0xC
};

// 0x10-byte entry: extra float at +0xC.
struct CHelpFloatC : public CHelp {
    f32 mFieldC; // 0xC
};

// 0x10-byte entry: extra halfword at +0xC.
struct CHelpHalfC : public CHelp {
    u16 mFieldC; // 0xC
};

// 0x14-byte entry: extra word at +0x10.
struct CHelpWord10 : public CHelp {
    u8 mPadC[4];  // 0xC..0xF
    u32 mField10; // 0x10
};

// 0x14-byte entry: word at +0xC and byte at +0x10.
struct CHelpWordCByte10 : public CHelp {
    u32 mFieldC;  // 0xC
    u8 mField10;  // 0x10
    u8 mPad11[3]; // 0x11..0x13
};

// 0x18-byte entry: word at +0x10 and bytes at +0x14..+0x16.
struct CHelpWord10Bytes : public CHelp {
    u8 mPadC[4];  // 0xC..0xF
    u32 mField10; // 0x10
    u8 mField14;  // 0x14
    u8 mField15;  // 0x15
    u8 mField16;  // 0x16
    u8 mPad17;    // 0x17
};

// 0x18-byte entry: only the word at +0x10 is initialized.
struct CHelpWord10_18 : public CHelp {
    u8 mPadC[4];  // 0xC..0xF
    u32 mField10; // 0x10
    u8 mPad14[4]; // 0x14..0x17
};

// 0x1C-byte entry: word at +0xC; +0x10 doubles as the atexit-registration
// cookie slot for the following CHelp_LandMark entry (sinit passes its
// address to __register_global_object).
struct CHelpWordC_1C : public CHelp {
    u32 mFieldC;  // 0xC
    u32 mField10; // 0x10 (atexit cookie)
    u32 mField14; // 0x14
    u32 mField18; // 0x18
};

// Full static help table (retail lbl_eu_80576D08, ~0x5C0 bytes of .bss):
// one member per entry the sinit constructs, laid out at retail offsets.
// The sinit casts the CHelpManagerTbl extern to this type to reach the
// deep entries.
struct CHelpTableFull {
    CHelpWordC mObj00;        // 0x000
    CHelp mObj10;             // 0x010
    CHelpFloatC mObj20;       // 0x020
    CHelp mObj30;             // 0x030
    CHelp mObj40;             // 0x040
    CHelp mObj50;             // 0x050
    CHelpFloatC mObj60;       // 0x060
    CHelpWordC mObj70;        // 0x070
    CHelp mObj84;             // 0x084
    CHelp mObj90;             // 0x090
    CHelpWord10 mObjA0;       // 0x0A0
    CHelp mObjB4;             // 0x0B4
    CHelp mObjC0;             // 0x0C0
    CHelp mObjCC;             // 0x0CC
    CHelp mObjD8;             // 0x0D8
    CHelp mObjE4;             // 0x0E4
    CHelp mObjF0;             // 0x0F0
    CHelp mObjFC;             // 0x0FC
    CHelp mObj108;            // 0x108
    CHelp mObj118;            // 0x118
    CHelp mObj124;            // 0x124
    CHelp mObj130;            // 0x130
    CHelpWord10 mObj140;      // 0x140
    CHelp mObj154;            // 0x154
    CHelp mObj160;            // 0x160
    CHelp mObj170;            // 0x170
    CHelp mObj17C;            // 0x17C
    CHelp mObj188;            // 0x188
    CHelp mObj194;            // 0x194
    CHelp mObj1A0;            // 0x1A0
    CHelp mObj1B0;            // 0x1B0
    CHelp mObj1BC;            // 0x1BC
    CHelp mObj1C8;            // 0x1C8
    CHelp mObj1D8;            // 0x1D8
    CHelp mObj1E8;            // 0x1E8
    CHelpWord10 mObj1F4;      // 0x1F4
    CHelp mObj208;            // 0x208
    CHelp mObj214;            // 0x214
    CHelpWord10 mObj220;      // 0x220
    CHelpWord10 mObj234;      // 0x234
    CHelp mObj248;            // 0x248
    CHelp mObj258;            // 0x258
    CHelpWord10 mObj268;      // 0x268
    CHelp mObj27C;            // 0x27C
    CHelpWord10_18 mObj288;   // 0x288
    CHelp mObj2A0;            // 0x2A0
    CHelp mObj2B0;            // 0x2B0
    CHelpWord10_18 mObj2C0;   // 0x2C0
    CHelp mObj2D8;            // 0x2D8
    CHelp mObj2E8;            // 0x2E8
    CHelp mObj2F8;            // 0x2F8
    CHelp mObj308;            // 0x308
    CHelp mObj314;            // 0x314
    CHelpWord10 mObj320;      // 0x320
    CHelpWord10 mObj334;      // 0x334
    CHelp mObj348;            // 0x348
    CHelp mObj354;            // 0x354
    CHelpWord10 mObj360;      // 0x360
    CHelp mObj374;            // 0x374
    CHelpWord10_18 mObj380;   // 0x380
    CHelp mObj398;            // 0x398
    CHelp mObj3A8;            // 0x3A8
    CHelpWord10 mObj3B8;      // 0x3B8
    CHelp mObj3CC;            // 0x3CC
    CHelpWord10 mObj3D8;      // 0x3D8
    CHelpWord10 mObj3EC;      // 0x3EC
    CHelp mObj400;            // 0x400
    CHelpWord10 mObj410;      // 0x410
    CHelp mObj424;            // 0x424
    CHelp mObj430;            // 0x430
    CHelp mObj43C;            // 0x43C
    CHelp mObj448;            // 0x448
    CHelp mObj454;            // 0x454
    CHelpWord10Bytes mObj460; // 0x460
    CHelp mObj478;            // 0x478
    CHelp mObj488;            // 0x488
    CHelpWordC_1C mObj498;    // 0x498
    CHelp_LandMark mObj4B4;   // 0x4B4
    CHelp mObj4D0;            // 0x4D0
    CHelpHalfC mObj4E0;       // 0x4E0
    CHelpWord10Bytes mObj4F0; // 0x4F0
    CHelpWordCByte10 mObj508; // 0x508
    CHelpWordCByte10 mObj51C; // 0x51C
    CHelpWordC mObj530;       // 0x530
    CHelpWordC mObj540;       // 0x540
    CHelpWord10Bytes mObj550; // 0x550
    CHelpWord10Bytes mObj568; // 0x568
    CHelpWord10Bytes mObj580; // 0x580
    CHelp mObj598;            // 0x598
    CHelp mObj5A4;            // 0x5A4
    CHelpFloatC mObj5B0;      // 0x5B0
};

} // namespace cf

// TU-local helpers (retail symbols are unmangled free functions, so these
// keep C linkage; defined in CHelpManager.cpp). DECOMP_DONT_INLINE keeps
// retail's real `bl` calls (MWCC would otherwise inline the small bodies).
extern "C" DECOMP_DONT_INLINE void func_802968A8(cf::CHelpListHolder* self);
extern "C" DECOMP_DONT_INLINE void func_802968B8(cf::CHelpListHolder* self, void** items, u8 flag);
extern "C" DECOMP_DONT_INLINE void func_80296924(cf::CHelpListHolder* self);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern cf::CHelpManager* lbl_eu_80664A10;
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28; // menu-open flag (bit 24)
extern u8 lbl_eu_8053911C[]; // CHelpManager vtable (retail .data)
extern cf::CHelpManagerTbl lbl_eu_80576D08; // static CHelp dispatch objects

// Vtable labels stored into the help-table entries' interface slot (+0x8) by
// the TU static initializer (retail .data).
extern u8 lbl_eu_8050FF08[];
extern u8 lbl_eu_80538F98[];
extern u8 lbl_eu_80538FD4[];
extern u8 lbl_eu_80539028[];
extern u8 lbl_eu_80539064[];
extern u8 lbl_eu_805390B8[];
extern u8 lbl_eu_805390E8[];
extern u8 lbl_eu_8053B3C0[];
extern u8 lbl_eu_8053B378[];
extern u8 lbl_eu_8053B3F8[];
extern u8 lbl_eu_8053B420[];
extern u8 lbl_eu_8053B458[];
extern u8 lbl_eu_8053B490[];
extern u8 lbl_eu_8053B4C8[];
extern u8 lbl_eu_8053B4F0[];
extern u8 lbl_eu_8053B518[];
extern u8 lbl_eu_8053B540[];
extern u8 lbl_eu_8053B560[];
extern u8 lbl_eu_8053B588[];
extern u8 lbl_eu_8053B5B0[];
extern u8 lbl_eu_8053B5D8[];
extern u8 lbl_eu_8053B638[];
extern u8 lbl_eu_8053B660[];
extern u8 lbl_eu_8053B6C0[];
extern u8 lbl_eu_8053B6E8[];
extern u8 lbl_eu_8053B698[];
extern u8 lbl_eu_8053B720[];
extern u8 lbl_eu_8053B748[];
extern u8 lbl_eu_8053B770[];
extern u8 lbl_eu_8053B798[];
extern u8 lbl_eu_8053B7C0[];

// .sdata2 float payloads stored at +0xC of the float entries.
extern f32 lbl_eu_80668BC8; // 1.0f
extern f32 lbl_eu_80668BCC; // 300.0f

// C-linkage retail ctor of CHelp_LandMark (defined with C linkage in
// CHelp_LandMark.cpp; retail symbol is unmangled, so the call site must
// reference it under C linkage).
extern "C" cf::CHelp_LandMark* __ct__cf_CHelp_LandMark(cf::CHelp_LandMark* self,
                                                       void* owner, u32 param,
                                                       u32 field_10_val);
// C-linkage retail dtor of CHelp_LandMark (same TU); the sinit registers it
// for atexit. MWCC forbids taking &Class::~Class, hence the C-linkage form.
extern "C" cf::CHelp_LandMark* __dt__Q22cf14CHelp_LandMarkFv(
    cf::CHelp_LandMark* self, s32 deleteFlag);
// Compiler-runtime atexit registration (used by the sinit for the
// CHelp_LandMark entry's destructor).
extern void __register_global_object(void* object, void* dtor, void* cookie);
extern void* lbl_eu_80538E30[]; // default mListB item array
extern cf::CHelpTableEntry lbl_eu_80538E90[]; // mField10 -> item-array table
extern "C" int func_80085840__Q22cf13CfGameManagerFv();
extern "C" s32 func_800822F4__Q22cf13CfGameManagerFv(); // scenario sequence counter
extern "C" u32 func_802B7C64(); // help-active check (trampoline to func_8029A658)

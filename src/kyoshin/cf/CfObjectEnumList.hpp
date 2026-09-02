#pragma once

#include <types.h>

namespace cf {

class CfObjEnumList {
public:
    struct sObjInfo {
        u32 objectId;
        void* object;
        u32 field_08;
        u32 field_0C;
        u32 field_10;
        float field_14;
        u8 field_18;   // flag bits (0x70 filter mark) - u8, not bool: retail stores the raw byte
    };

    CfObjEnumList();
    ~CfObjEnumList();

    void* func_800F6E98(int index);
    void* func_800F6EAC(unsigned long idx);
    void* func_800F6EC0(int index);

    // reslist base: vtable pointer (0x00), sentinel, backing storage, and
    // ownership flag. The methods below are plain (direct-call) methods, but
    // the OBJECT carries a reslist vtable pointer at +0x00, so the array and
    // count sit at +0x20 / +0x620 (retail __ct__cf_CfObjEnumList stw r7,0x620).
    struct sResNode {              // reslist sentinel node (12 bytes)
        void* mNext;               // 0x00
        void* mPrev;               // 0x04
        void* mItem;               // 0x08
    };

    void* mVtable;                 // 0x00 (reslist base vtable)
    sResNode* mStartNode;          // 0x04 (pointer to the embedded sentinel)
    sResNode mStartNode_;          // 0x08 (sentinel node; mItem at +0x10)
    void* mList;                   // 0x14 (backing array)
    int mCapacity;                 // 0x18
    bool mOwnsList;                // 0x1C
    sObjInfo* mPtrArray[384];      // 0x0020
    u32 mPtrCount;             // 0x0620
    void* mSortVtableA;        // 0x0624
    void* mSortVtableB;        // 0x0628
    u32 field_062C;            // 0x062C
    sObjInfo mObjInfo[384];    // 0x0630
    u32 mObjInfoCount;         // 0x3030
    u32 field_3034;
    u32 field_3038;
    u32 field_303C;
};

} // namespace cf

// Sortable-list interface used by func_800F8890 (retail vtable slots +0x10
// get(), +0x14 value(); the retail vtable has two extra leading slots vs the
// Sortable-list interface used by func_800F8890.  The retail vtable is 8
// bytes past the declaration's slot 0 (MWCC emits two leading non-user
// vtable entries here, same +2 as CfEnumActorBase), so get()/value() land at
// retail +0x08/+0x0C.  get() returns a pointer to the element slot; the
// partition swaps elements by writing through those pointers.
class CfSortableList {
public:
    virtual void* get(int idx) = 0;     // decl idx 0 -> retail +0x08
    virtual float value(int idx) = 0;   // decl idx 1 -> retail +0x0C
};



// ---------------------------------------------------------------------------
// Actor views used by the list-rebuild ctors (CfObjectEnumList.cpp).
// Dummy-virtual padding reaches the retail vtable slots; with -RTTI on,
// declared index N sits at vtable offset (N+2)*4.  Never instantiated, so no
// vtables are emitted.
// ---------------------------------------------------------------------------
#define ENUM_VIEW_DUMMY(n_) virtual void v##n_()

// Word block returned by CfEnumSubObj4::b30() (vtable slot 0x30); its first
// word feeds the func_80174C98 arts-state gate.
struct CfEnumB30Word {
    u32 field_0;             // 0x00
};

// Actor's +0x4 sub-object: vtable slot 0x30 (index 10) returns the word
// block whose first word is queried by func_80174C98.
class CfEnumSubObj4 {
public:
    ENUM_VIEW_DUMMY(0);  ENUM_VIEW_DUMMY(1);  ENUM_VIEW_DUMMY(2);  ENUM_VIEW_DUMMY(3);
    ENUM_VIEW_DUMMY(4);  ENUM_VIEW_DUMMY(5);  ENUM_VIEW_DUMMY(6);  ENUM_VIEW_DUMMY(7);
    ENUM_VIEW_DUMMY(8);  ENUM_VIEW_DUMMY(9);
    virtual CfEnumB30Word* b30();   // index 10 -> vtable 0x30
};

// Battle-state block behind CfEnumActorView::vf298() (vtable 0x298); the
// byte at +0x42 is compared against the ctor's spot-id argument.
struct CfEnumBattleBlock {
    u8 _pad_00[0x3C];
    u16 field_0x3C;          // 0x3C (compared u16: 2/3/6 clear the rejection flag)
    u8 _pad_3E[0x42 - 0x3E];
    u8 field_0x42;           // 0x42 (compared byte)
};
struct CfEnumVf298 {
    u8 _pad_00[0x50];
    CfEnumBattleBlock* field_0x50;   // 0x50 (battle-state block)
};

// Embedded move sub-object at actor+0x3E9C: vtable 0x4C (index 17) returns
// the referenced spot id; vtable 0x84 (index 31) returns a state word.
class CfEnumSubObj_3E9C {
public:
    ENUM_VIEW_DUMMY(0);  ENUM_VIEW_DUMMY(1);  ENUM_VIEW_DUMMY(2);  ENUM_VIEW_DUMMY(3);
    ENUM_VIEW_DUMMY(4);  ENUM_VIEW_DUMMY(5);  ENUM_VIEW_DUMMY(6);  ENUM_VIEW_DUMMY(7);
    ENUM_VIEW_DUMMY(8);  ENUM_VIEW_DUMMY(9);  ENUM_VIEW_DUMMY(10); ENUM_VIEW_DUMMY(11);
    ENUM_VIEW_DUMMY(12); ENUM_VIEW_DUMMY(13); ENUM_VIEW_DUMMY(14); ENUM_VIEW_DUMMY(15);
    ENUM_VIEW_DUMMY(16);
    virtual u32 vfn13();     // index 17 -> vtable 0x4C (referenced spot id)
    ENUM_VIEW_DUMMY(18); ENUM_VIEW_DUMMY(19); ENUM_VIEW_DUMMY(20); ENUM_VIEW_DUMMY(21);
    ENUM_VIEW_DUMMY(22); ENUM_VIEW_DUMMY(23); ENUM_VIEW_DUMMY(24); ENUM_VIEW_DUMMY(25);
    ENUM_VIEW_DUMMY(26); ENUM_VIEW_DUMMY(27); ENUM_VIEW_DUMMY(28); ENUM_VIEW_DUMMY(29);
    ENUM_VIEW_DUMMY(30);
    virtual u32 f31();       // index 31 -> vtable 0x84 (state word)
};

// Actor view for the list-rebuild ctors: a CfEnumActor is a CfEnumActorBase
// (0x3E9C bytes) followed by its embedded CfEnumObject part, so the object's
// flags64 word sits at actor+0x3F00.
class CfEnumActorView {
public:
    ENUM_VIEW_DUMMY(0);   ENUM_VIEW_DUMMY(1);   ENUM_VIEW_DUMMY(2);   ENUM_VIEW_DUMMY(3);
    ENUM_VIEW_DUMMY(4);   ENUM_VIEW_DUMMY(5);   ENUM_VIEW_DUMMY(6);   ENUM_VIEW_DUMMY(7);
    ENUM_VIEW_DUMMY(8);   ENUM_VIEW_DUMMY(9);   ENUM_VIEW_DUMMY(10);  ENUM_VIEW_DUMMY(11);
    ENUM_VIEW_DUMMY(12);  ENUM_VIEW_DUMMY(13);  ENUM_VIEW_DUMMY(14);  ENUM_VIEW_DUMMY(15);
    ENUM_VIEW_DUMMY(16);  ENUM_VIEW_DUMMY(17);  ENUM_VIEW_DUMMY(18);  ENUM_VIEW_DUMMY(19);
    ENUM_VIEW_DUMMY(20);  ENUM_VIEW_DUMMY(21);  ENUM_VIEW_DUMMY(22);  ENUM_VIEW_DUMMY(23);
    ENUM_VIEW_DUMMY(24);  ENUM_VIEW_DUMMY(25);  ENUM_VIEW_DUMMY(26);  ENUM_VIEW_DUMMY(27);
    ENUM_VIEW_DUMMY(28);  ENUM_VIEW_DUMMY(29);  ENUM_VIEW_DUMMY(30);  ENUM_VIEW_DUMMY(31);
    ENUM_VIEW_DUMMY(32);  ENUM_VIEW_DUMMY(33);  ENUM_VIEW_DUMMY(34);  ENUM_VIEW_DUMMY(35);
    ENUM_VIEW_DUMMY(36);  ENUM_VIEW_DUMMY(37);  ENUM_VIEW_DUMMY(38);  ENUM_VIEW_DUMMY(39);
    ENUM_VIEW_DUMMY(40);  ENUM_VIEW_DUMMY(41);  ENUM_VIEW_DUMMY(42);  ENUM_VIEW_DUMMY(43);
    ENUM_VIEW_DUMMY(44);  ENUM_VIEW_DUMMY(45);  ENUM_VIEW_DUMMY(46);  ENUM_VIEW_DUMMY(47);
    ENUM_VIEW_DUMMY(48);  ENUM_VIEW_DUMMY(49);  ENUM_VIEW_DUMMY(50);  ENUM_VIEW_DUMMY(51);
    ENUM_VIEW_DUMMY(52);  ENUM_VIEW_DUMMY(53);  ENUM_VIEW_DUMMY(54);  ENUM_VIEW_DUMMY(55);
    ENUM_VIEW_DUMMY(56);  ENUM_VIEW_DUMMY(57);  ENUM_VIEW_DUMMY(58);  ENUM_VIEW_DUMMY(59);
    ENUM_VIEW_DUMMY(60);  ENUM_VIEW_DUMMY(61);  ENUM_VIEW_DUMMY(62);  ENUM_VIEW_DUMMY(63);
    ENUM_VIEW_DUMMY(64);  ENUM_VIEW_DUMMY(65);  ENUM_VIEW_DUMMY(66);  ENUM_VIEW_DUMMY(67);
    ENUM_VIEW_DUMMY(68);  ENUM_VIEW_DUMMY(69);  ENUM_VIEW_DUMMY(70);  ENUM_VIEW_DUMMY(71);
    ENUM_VIEW_DUMMY(72);  ENUM_VIEW_DUMMY(73);  ENUM_VIEW_DUMMY(74);  ENUM_VIEW_DUMMY(75);
    ENUM_VIEW_DUMMY(76);  ENUM_VIEW_DUMMY(77);  ENUM_VIEW_DUMMY(78);  ENUM_VIEW_DUMMY(79);
    ENUM_VIEW_DUMMY(80);  ENUM_VIEW_DUMMY(81);  ENUM_VIEW_DUMMY(82);  ENUM_VIEW_DUMMY(83);
    ENUM_VIEW_DUMMY(84);  ENUM_VIEW_DUMMY(85);  ENUM_VIEW_DUMMY(86);  ENUM_VIEW_DUMMY(87);
    ENUM_VIEW_DUMMY(88);  ENUM_VIEW_DUMMY(89);  ENUM_VIEW_DUMMY(90);  ENUM_VIEW_DUMMY(91);
    ENUM_VIEW_DUMMY(92);  ENUM_VIEW_DUMMY(93);  ENUM_VIEW_DUMMY(94);  ENUM_VIEW_DUMMY(95);
    ENUM_VIEW_DUMMY(96);  ENUM_VIEW_DUMMY(97);  ENUM_VIEW_DUMMY(98);  ENUM_VIEW_DUMMY(99);
    ENUM_VIEW_DUMMY(100); ENUM_VIEW_DUMMY(101); ENUM_VIEW_DUMMY(102); ENUM_VIEW_DUMMY(103);
    ENUM_VIEW_DUMMY(104); ENUM_VIEW_DUMMY(105); ENUM_VIEW_DUMMY(106); ENUM_VIEW_DUMMY(107);
    ENUM_VIEW_DUMMY(108); ENUM_VIEW_DUMMY(109); ENUM_VIEW_DUMMY(110); ENUM_VIEW_DUMMY(111);
    ENUM_VIEW_DUMMY(112); ENUM_VIEW_DUMMY(113); ENUM_VIEW_DUMMY(114); ENUM_VIEW_DUMMY(115);
    ENUM_VIEW_DUMMY(116); ENUM_VIEW_DUMMY(117); ENUM_VIEW_DUMMY(118); ENUM_VIEW_DUMMY(119);
    ENUM_VIEW_DUMMY(120); ENUM_VIEW_DUMMY(121); ENUM_VIEW_DUMMY(122); ENUM_VIEW_DUMMY(123);
    ENUM_VIEW_DUMMY(124); ENUM_VIEW_DUMMY(125); ENUM_VIEW_DUMMY(126); ENUM_VIEW_DUMMY(127);
    ENUM_VIEW_DUMMY(128); ENUM_VIEW_DUMMY(129); ENUM_VIEW_DUMMY(130); ENUM_VIEW_DUMMY(131);
    ENUM_VIEW_DUMMY(132); ENUM_VIEW_DUMMY(133); ENUM_VIEW_DUMMY(134); ENUM_VIEW_DUMMY(135);
    ENUM_VIEW_DUMMY(136); ENUM_VIEW_DUMMY(137); ENUM_VIEW_DUMMY(138); ENUM_VIEW_DUMMY(139);
    ENUM_VIEW_DUMMY(140); ENUM_VIEW_DUMMY(141); ENUM_VIEW_DUMMY(142); ENUM_VIEW_DUMMY(143);
    ENUM_VIEW_DUMMY(144); ENUM_VIEW_DUMMY(145); ENUM_VIEW_DUMMY(146); ENUM_VIEW_DUMMY(147);
    ENUM_VIEW_DUMMY(148); ENUM_VIEW_DUMMY(149); ENUM_VIEW_DUMMY(150); ENUM_VIEW_DUMMY(151);
    ENUM_VIEW_DUMMY(152); ENUM_VIEW_DUMMY(153); ENUM_VIEW_DUMMY(154); ENUM_VIEW_DUMMY(155);
    ENUM_VIEW_DUMMY(156); ENUM_VIEW_DUMMY(157); ENUM_VIEW_DUMMY(158); ENUM_VIEW_DUMMY(159);
    ENUM_VIEW_DUMMY(160); ENUM_VIEW_DUMMY(161); ENUM_VIEW_DUMMY(162); ENUM_VIEW_DUMMY(163);
    virtual CfEnumVf298* vf298();    // index 164 -> vtable 0x298

    CfEnumSubObj4* field_04;         // 0x04
    u8 _pad_08[0x60C - 0x08];        // 0x08-0x60B
    u8 field_60C[4];                 // 0x60C (probe block passed to func_804B1AD8)
    u8 _pad_610[0x63C - 0x610];      // 0x610-0x63B
    float field_63C;                 // 0x63C (radius/offset subtracted from distance)
    u8 _pad_640[0x3E9C - 0x640];     // 0x640-0x3E9B
    CfEnumSubObj_3E9C sub;           // 0x3E9C (embedded move sub-object)
    u8 _pad_3EA0[0x3F00 - 0x3EA0];   // 0x3EA0-0x3EFF
    u32 field_3F00;                  // 0x3F00 (embedded CfEnumObject flags64)
    u8 _pad_3F04[0x3F10 - 0x3F04];   // 0x3F04-0x3F0F
    u32 field_3F10;                  // 0x3F10 (embedded CfEnumObject objectId)
};

// Object-position block returned by CfEnumObjPosView::v128()/v12C(): the
// position floats sit at +0xC/+0x1C/+0x2C.
struct CfEnumPosBlock {
    u8 _pad_00[0xC];
    float x;                 // 0xC
    u8 _pad_10[0x1C - 0x10];
    float y;                 // 0x1C
    u8 _pad_20[0x2C - 0x20];
    float z;                 // 0x2C
};

// Object position/state view for the position-filtering list-rebuild ctors:
// vtable 0xAC returns a plain position vector; 0x128/0x12C return a
// CfEnumPosBlock (position floats at +0xC/+0x1C/+0x2C).
class CfEnumObjPosView {
public:
    ENUM_VIEW_DUMMY(0);   ENUM_VIEW_DUMMY(1);   ENUM_VIEW_DUMMY(2);   ENUM_VIEW_DUMMY(3);
    ENUM_VIEW_DUMMY(4);   ENUM_VIEW_DUMMY(5);   ENUM_VIEW_DUMMY(6);   ENUM_VIEW_DUMMY(7);
    ENUM_VIEW_DUMMY(8);   ENUM_VIEW_DUMMY(9);   ENUM_VIEW_DUMMY(10);  ENUM_VIEW_DUMMY(11);
    ENUM_VIEW_DUMMY(12);  ENUM_VIEW_DUMMY(13);  ENUM_VIEW_DUMMY(14);  ENUM_VIEW_DUMMY(15);
    ENUM_VIEW_DUMMY(16);  ENUM_VIEW_DUMMY(17);  ENUM_VIEW_DUMMY(18);  ENUM_VIEW_DUMMY(19);
    ENUM_VIEW_DUMMY(20);  ENUM_VIEW_DUMMY(21);  ENUM_VIEW_DUMMY(22);  ENUM_VIEW_DUMMY(23);
    ENUM_VIEW_DUMMY(24);  ENUM_VIEW_DUMMY(25);  ENUM_VIEW_DUMMY(26);  ENUM_VIEW_DUMMY(27);
    ENUM_VIEW_DUMMY(28);  ENUM_VIEW_DUMMY(29);  ENUM_VIEW_DUMMY(30);  ENUM_VIEW_DUMMY(31);
    ENUM_VIEW_DUMMY(32);  ENUM_VIEW_DUMMY(33);  ENUM_VIEW_DUMMY(34);  ENUM_VIEW_DUMMY(35);
    ENUM_VIEW_DUMMY(36);  ENUM_VIEW_DUMMY(37);  ENUM_VIEW_DUMMY(38);  ENUM_VIEW_DUMMY(39);
    ENUM_VIEW_DUMMY(40);
    virtual void* vAC();        // index 41 -> vtable 0xAC (position vector)
    ENUM_VIEW_DUMMY(42); ENUM_VIEW_DUMMY(43); ENUM_VIEW_DUMMY(44); ENUM_VIEW_DUMMY(45);
    ENUM_VIEW_DUMMY(46); ENUM_VIEW_DUMMY(47); ENUM_VIEW_DUMMY(48); ENUM_VIEW_DUMMY(49);
    ENUM_VIEW_DUMMY(50); ENUM_VIEW_DUMMY(51); ENUM_VIEW_DUMMY(52); ENUM_VIEW_DUMMY(53);
    ENUM_VIEW_DUMMY(54); ENUM_VIEW_DUMMY(55); ENUM_VIEW_DUMMY(56); ENUM_VIEW_DUMMY(57);
    ENUM_VIEW_DUMMY(58); ENUM_VIEW_DUMMY(59); ENUM_VIEW_DUMMY(60); ENUM_VIEW_DUMMY(61);
    ENUM_VIEW_DUMMY(62); ENUM_VIEW_DUMMY(63); ENUM_VIEW_DUMMY(64); ENUM_VIEW_DUMMY(65);
    ENUM_VIEW_DUMMY(66); ENUM_VIEW_DUMMY(67); ENUM_VIEW_DUMMY(68); ENUM_VIEW_DUMMY(69);
    ENUM_VIEW_DUMMY(70); ENUM_VIEW_DUMMY(71);
    virtual CfEnumPosBlock* v128();        // index 72 -> vtable 0x128
    virtual CfEnumPosBlock* v12C(int a);   // index 73 -> vtable 0x12C
};

// Object view used by func_800F89DC's projection pass: vtable 0xAC (index
// 41) returns the plain position vector; 0x120 (index 70) returns a
// CfEnumPosBlock given a context pointer.
class CfEnumProjView {
public:
    ENUM_VIEW_DUMMY(0);   ENUM_VIEW_DUMMY(1);   ENUM_VIEW_DUMMY(2);   ENUM_VIEW_DUMMY(3);
    ENUM_VIEW_DUMMY(4);   ENUM_VIEW_DUMMY(5);   ENUM_VIEW_DUMMY(6);   ENUM_VIEW_DUMMY(7);
    ENUM_VIEW_DUMMY(8);   ENUM_VIEW_DUMMY(9);   ENUM_VIEW_DUMMY(10);  ENUM_VIEW_DUMMY(11);
    ENUM_VIEW_DUMMY(12);  ENUM_VIEW_DUMMY(13);  ENUM_VIEW_DUMMY(14);  ENUM_VIEW_DUMMY(15);
    ENUM_VIEW_DUMMY(16);  ENUM_VIEW_DUMMY(17);  ENUM_VIEW_DUMMY(18);  ENUM_VIEW_DUMMY(19);
    ENUM_VIEW_DUMMY(20);  ENUM_VIEW_DUMMY(21);  ENUM_VIEW_DUMMY(22);  ENUM_VIEW_DUMMY(23);
    ENUM_VIEW_DUMMY(24);  ENUM_VIEW_DUMMY(25);  ENUM_VIEW_DUMMY(26);  ENUM_VIEW_DUMMY(27);
    ENUM_VIEW_DUMMY(28);  ENUM_VIEW_DUMMY(29);  ENUM_VIEW_DUMMY(30);  ENUM_VIEW_DUMMY(31);
    ENUM_VIEW_DUMMY(32);  ENUM_VIEW_DUMMY(33);  ENUM_VIEW_DUMMY(34);  ENUM_VIEW_DUMMY(35);
    ENUM_VIEW_DUMMY(36);  ENUM_VIEW_DUMMY(37);  ENUM_VIEW_DUMMY(38);  ENUM_VIEW_DUMMY(39);
    ENUM_VIEW_DUMMY(40);
    virtual void* vAC();                    // index 41 -> vtable 0xAC
    ENUM_VIEW_DUMMY(42);  ENUM_VIEW_DUMMY(43);  ENUM_VIEW_DUMMY(44);  ENUM_VIEW_DUMMY(45);
    ENUM_VIEW_DUMMY(46);  ENUM_VIEW_DUMMY(47);  ENUM_VIEW_DUMMY(48);  ENUM_VIEW_DUMMY(49);
    ENUM_VIEW_DUMMY(50);  ENUM_VIEW_DUMMY(51);  ENUM_VIEW_DUMMY(52);  ENUM_VIEW_DUMMY(53);
    ENUM_VIEW_DUMMY(54);  ENUM_VIEW_DUMMY(55);  ENUM_VIEW_DUMMY(56);  ENUM_VIEW_DUMMY(57);
    ENUM_VIEW_DUMMY(58);  ENUM_VIEW_DUMMY(59);  ENUM_VIEW_DUMMY(60);  ENUM_VIEW_DUMMY(61);
    ENUM_VIEW_DUMMY(62);  ENUM_VIEW_DUMMY(63);  ENUM_VIEW_DUMMY(64);  ENUM_VIEW_DUMMY(65);
    ENUM_VIEW_DUMMY(66);  ENUM_VIEW_DUMMY(67);  ENUM_VIEW_DUMMY(68);  ENUM_VIEW_DUMMY(69);
    virtual CfEnumPosBlock* vX120(u8* ctx); // index 70 -> vtable 0x120
};

// Actor view whose vtable slot 0x128 (index 72) returns a float sort value
// directly (used by func_800F9AEC's distance refresh).
class CfEnumActorValueView {
public:
    ENUM_VIEW_DUMMY(0);   ENUM_VIEW_DUMMY(1);   ENUM_VIEW_DUMMY(2);   ENUM_VIEW_DUMMY(3);
    ENUM_VIEW_DUMMY(4);   ENUM_VIEW_DUMMY(5);   ENUM_VIEW_DUMMY(6);   ENUM_VIEW_DUMMY(7);
    ENUM_VIEW_DUMMY(8);   ENUM_VIEW_DUMMY(9);   ENUM_VIEW_DUMMY(10);  ENUM_VIEW_DUMMY(11);
    ENUM_VIEW_DUMMY(12);  ENUM_VIEW_DUMMY(13);  ENUM_VIEW_DUMMY(14);  ENUM_VIEW_DUMMY(15);
    ENUM_VIEW_DUMMY(16);  ENUM_VIEW_DUMMY(17);  ENUM_VIEW_DUMMY(18);  ENUM_VIEW_DUMMY(19);
    ENUM_VIEW_DUMMY(20);  ENUM_VIEW_DUMMY(21);  ENUM_VIEW_DUMMY(22);  ENUM_VIEW_DUMMY(23);
    ENUM_VIEW_DUMMY(24);  ENUM_VIEW_DUMMY(25);  ENUM_VIEW_DUMMY(26);  ENUM_VIEW_DUMMY(27);
    ENUM_VIEW_DUMMY(28);  ENUM_VIEW_DUMMY(29);  ENUM_VIEW_DUMMY(30);  ENUM_VIEW_DUMMY(31);
    ENUM_VIEW_DUMMY(32);  ENUM_VIEW_DUMMY(33);  ENUM_VIEW_DUMMY(34);  ENUM_VIEW_DUMMY(35);
    ENUM_VIEW_DUMMY(36);  ENUM_VIEW_DUMMY(37);  ENUM_VIEW_DUMMY(38);  ENUM_VIEW_DUMMY(39);
    ENUM_VIEW_DUMMY(40);  ENUM_VIEW_DUMMY(41);  ENUM_VIEW_DUMMY(42);  ENUM_VIEW_DUMMY(43);
    ENUM_VIEW_DUMMY(44);  ENUM_VIEW_DUMMY(45);  ENUM_VIEW_DUMMY(46);  ENUM_VIEW_DUMMY(47);
    ENUM_VIEW_DUMMY(48);  ENUM_VIEW_DUMMY(49);  ENUM_VIEW_DUMMY(50);  ENUM_VIEW_DUMMY(51);
    ENUM_VIEW_DUMMY(52);  ENUM_VIEW_DUMMY(53);  ENUM_VIEW_DUMMY(54);  ENUM_VIEW_DUMMY(55);
    ENUM_VIEW_DUMMY(56);  ENUM_VIEW_DUMMY(57);  ENUM_VIEW_DUMMY(58);  ENUM_VIEW_DUMMY(59);
    ENUM_VIEW_DUMMY(60);  ENUM_VIEW_DUMMY(61);  ENUM_VIEW_DUMMY(62);  ENUM_VIEW_DUMMY(63);
    ENUM_VIEW_DUMMY(64);  ENUM_VIEW_DUMMY(65);  ENUM_VIEW_DUMMY(66);  ENUM_VIEW_DUMMY(67);
    ENUM_VIEW_DUMMY(68);  ENUM_VIEW_DUMMY(69);  ENUM_VIEW_DUMMY(70);  ENUM_VIEW_DUMMY(71);
    virtual float value128();        // index 72 -> vtable 0x128
};

#undef ENUM_VIEW_DUMMY

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_800F4798(void* self);
extern "C" int func_804B5088(void* self, void* a, void* b, int filter, int isFirst); // object enum probe
extern "C" int func_804BE348(void* a, void* b, int c, int d, int e);   // coli probe query
// cone probe on aux+0x60C (self, spot, range, cone half-angle, angle offset)
extern "C" int func_804B1C9C(void* self, void* spot, float a, float b, float c);
// func_804B5088 (object enum probe) is declared above; do not redeclare here.
extern "C" int func_804B1AD8(void* self, void* spot, float threshold); // distance probe on aux+0x60C block
// sdata: global coli probe object (accessed via sda21)
extern void* lbl_eu_80665958;
// Data import used by the list-rebuild ctor __ct__800FD250 (reslist-ish sort
// descriptor table; only its address is taken, at +0 and +8).
extern u8 lbl_eu_8052BDA0[0x20];

// Context blob passed to CfEnumProjView::vX120 by func_800F89DC.
extern u8 lbl_eu_804FCD24[];

// reslist<T> vtable stored by the CfObjectSelectorObj +4 list dtor (retail
// inlines the reslist-base dtor body: vtable store + sentinel walk + free).
extern u8 lbl_eu_8052585C[0xC];

// CfObjEnumList class vtable (stored at +0x00 by the ctor after the base
// vtable store; the retail linker keeps both vtable stores).
extern u8 lbl_eu_8052BD80[0xC];

// .sdata2 float constants (shared sdata2 pool, loaded via sda21).
// lbl_eu_80666EB8 / lbl_eu_80666EBC are declared in the cpp's extern "C"
// block: their declaration position keeps func_800F4B5C's reloc pairing.
extern const float lbl_eu_80666EC8;   // 0.2f
extern const float lbl_eu_80666ECC;   // 0.1f
extern const float lbl_eu_80666EF0;   // -1.0f (sdata2 word)
extern const float lbl_eu_8066A1FC;          // two*pi (angle wrap; pi is declared in CfCam.hpp)
extern "C" const float lbl_eu_8066A1F8;      // .sdata2 pi
extern const float lbl_eu_80666ED0;   // 0.024543693f (angle scale)
extern const float lbl_eu_80666ED4;   // 0.5f (half-angle scale)
extern const float lbl_eu_80666ED8;   // radians -> FIdx scale (SinFIdx/CosFIdx arg)
extern const double lbl_eu_80666EE0;  // sdata2 double
extern const double lbl_eu_80666EE8;  // sdata2 double

// 3x3 rotation matrix helper used by the segment-probe ctor __ct__800FBA18
// (Y-rotation rows: [c,0,s] / [0,1,0] / [-s,0,c]).
struct EnumRotMtx33 {
    float _00, _01, _02;
    float _10, _11, _12;
    float _20, _21, _22;
};

// nw4r db warning strings used by __ct__800FBA18's non-finite distance guard
// (file string, line 0x273, format string).
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// .bss probe-offset table written by func_800FD3FC (12 floats).
extern float lbl_eu_80573A30[12];

// .sbss flag byte guarding the probe-offset table init (signed: retail
// lbz + extsb. tests the raw byte).
extern s8 lbl_eu_80663F08;

// s32->f32 conversion magic double (2^52 + 2^31) referenced by MWCC's
// xoris/lfd/fsubs int->float idiom in __ct__800FA9B4.
extern const double lbl_eu_80666EC0;

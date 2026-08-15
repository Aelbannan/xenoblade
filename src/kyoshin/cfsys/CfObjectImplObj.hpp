#pragma once

#include <types.h>
#include "kyoshin/cfsys/CfObjectImplMove.hpp"
#include "kyoshin/cf/CPartsChange.hpp"

namespace cf {

// Driver sub-object at CfObjectImplObj::field_14: command dispatcher at
// vtable 0x204 (5-arg form used by func_800CA590), event id at +0x98
// (func_800CA540), driver id at +0xC4. Dummy slots pin vfn204 to vtable
// offset 0x204 (CfEmbeddedSubObj_3E9C scheme).
class CfObjectImplObjSub {
public:
    virtual void* vfn00(u32 a);   // index 0  -> vtable 0x08
    virtual void* f01();          // index 1
    virtual void* f02();          // index 2
    virtual void* f03();          // index 3
    virtual void* f04();          // index 4
    virtual void* f05();          // index 5
    virtual void* f06();          // index 6
    virtual void* f07();          // index 7
    virtual void* f08();          // index 8
    virtual void* f09();          // index 9
    virtual void* f10();          // index 10
    virtual void* f11();          // index 11
    virtual void* f12();          // index 12
    virtual void* f13();          // index 13
    virtual void* f14();          // index 14
    virtual void* f15();          // index 15
    virtual void* f16();          // index 16
    virtual void* vfn13();        // index 17 -> vtable 0x4c
    virtual void* vfn14(void* a); // index 18 -> vtable 0x50
    virtual void* f19();          // index 19
    virtual void* f20(); virtual void* f21(); virtual void* f22(); virtual void* f23();
    virtual void* f24(); virtual void* f25(); virtual void* f26(); virtual void* f27();
    virtual void* f28(); virtual void* f29(); virtual void* f30(); virtual void* f31();
    virtual void* f32(); virtual void* f33(); virtual void* f34(); virtual void* f35();
    virtual void* f36(); virtual void* f37(); virtual void* f38(); virtual void* f39();
    virtual void* f40(); virtual void* f41(); virtual void* f42(); virtual void* f43();
    virtual void* f44(); virtual void* f45(); virtual void* f46(); virtual void* f47();
    virtual void* f48(); virtual void* f49(); virtual void* f50(); virtual void* f51();
    virtual void* f52(); virtual void* f53(); virtual void* f54(); virtual void* f55();
    virtual void* f56(); virtual void* f57(); virtual void* f58(); virtual void* f59();
    virtual void* f60(); virtual void* f61(); virtual void* f62(); virtual void* f63();
    virtual void* f64(); virtual void* f65(); virtual void* f66(); virtual void* f67();
    virtual void* f68(); virtual void* f69(); virtual void* f70(); virtual void* f71();
    virtual void* f72(); virtual void* f73(); virtual void* f74(); virtual void* f75();
    virtual void* f76(); virtual void* f77(); virtual void* f78(); virtual void* f79();
    virtual void* f80(); virtual void* f81(); virtual void* f82(); virtual void* f83();
    virtual void* f84(); virtual void* f85(); virtual void* f86(); virtual void* f87();
    virtual void* f88(); virtual void* f89(); virtual void* f90(); virtual void* f91();
    virtual void* f92(); virtual void* f93(); virtual void* f94(); virtual void* f95();
    virtual void* f96(); virtual void* f97(); virtual void* f98(); virtual void* f99();
    virtual void* f100(); virtual void* f101(); virtual void* f102(); virtual void* f103();
    virtual void* f104(); virtual void* f105(); virtual void* f106(); virtual void* f107();
    virtual void* f108(); virtual void* f109(); virtual void* f110(); virtual void* f111();
    virtual void* f112(); virtual void* f113(); virtual void* f114(); virtual void* f115();
    virtual void* f116(); virtual void* f117(); virtual void* f118(); virtual void* f119();
    virtual void* f120(); virtual void* f121(); virtual void* f122(); virtual void* f123();
    virtual void* f124(); virtual void* f125(); virtual void* f126();
    virtual void vfn204(u32 a, int b, int c, int d, int e);  // index 127 -> vtable 0x204

    u8 _04_97[0x98 - 0x04];       // 0x04-0x97
    u32 field_0x98;               // 0x98 (event id)
    u8 _9C_C3[0xC4 - 0x9C];       // 0x9C-0xC3
    u32 field_0xC4;               // 0xC4 (driver id)
};

// Object handled by this TU's helpers (same family as CfObjectImplPc /
// CfObjectImplMove views of the "Obj" implementation object): vtable slots
// up to 0xE4 (the vfE4 query dispatched by func_800CA4EC), the driver
// sub-object at +0x14, the actor object at +0x18, and an embedded
// CPartsChange at +0x368 (0x30 bytes, released by the destructor).
// Never instantiated, so no vtable is emitted.
class CfObjectImplObj {
public:
    virtual ~CfObjectImplObj();                       // index 0 -> vtable offset 0x08
    virtual void vf04() = 0;                         // index 1
    virtual void vf08() = 0;                         // index 2
    virtual void vf0C() = 0;                         // index 3
    virtual void vf10() = 0;                         // index 4
    virtual void vf14() = 0;                         // index 5
    virtual void vf18() = 0;                         // index 6
    virtual void vf1C() = 0;                         // index 7
    virtual void vf20() = 0;                         // index 8
    virtual void vf24() = 0;                         // index 9
    virtual void vf28() = 0;                         // index 10
    virtual void vf2C() = 0;                         // index 11
    virtual void vf30() = 0;                         // index 12
    virtual void vf34() = 0;                         // index 13
    virtual void vf38() = 0;                         // index 14
    virtual void vf3C() = 0;                         // index 15
    virtual void vf40() = 0;                         // index 16
    virtual void vf44() = 0;                         // index 17
    virtual void vf48() = 0;                         // index 18
    virtual void vf4C() = 0;                         // index 19
    virtual void vf50() = 0;                         // index 20
    virtual void vf54() = 0;                         // index 21
    virtual void vf58() = 0;                         // index 22
    virtual void vf5C() = 0;                         // index 23
    virtual void vf60() = 0;                         // index 24
    virtual void vf64() = 0;                         // index 25
    virtual void vf68() = 0;                         // index 26
    virtual void vf6C() = 0;                         // index 27
    virtual void vf70() = 0;                         // index 28
    virtual void vf74() = 0;                         // index 29
    virtual void vf78() = 0;                         // index 30
    virtual void vf7C() = 0;                         // index 31
    virtual void vf80() = 0;                         // index 32
    virtual void vf84() = 0;                         // index 33
    virtual void vf88() = 0;                         // index 34
    virtual void vf94(u32 a) = 0;                    // index 35 -> vtable offset 0x94
    virtual void vf98() = 0;                         // index 36
    virtual void vf9C() = 0;                         // index 37
    virtual void vfA0() = 0;                         // index 38
    virtual void vfA4() = 0;                         // index 39
    virtual void vfA8() = 0;                         // index 40
    virtual void vfAC() = 0;                         // index 41
    virtual void vfB0() = 0;                         // index 42
    virtual void vfB4() = 0;                         // index 43
    virtual void vfB8() = 0;                         // index 44
    virtual void vfBC() = 0;                         // index 45
    virtual void vfC0() = 0;                         // index 46
    virtual void vfC4() = 0;                         // index 47
    virtual void vfC8() = 0;                         // index 48
    virtual void vfCC() = 0;                         // index 49
    virtual void vfD0() = 0;                         // index 50
    virtual void vfD4() = 0;                         // index 51
    virtual void vfD8() = 0;                         // index 52
    virtual void vfDC() = 0;                         // index 53
    virtual void vfE0() = 0;                         // index 54
    virtual u8* vfE4();                              // index 55 -> vtable offset 0xE4

    u8 _pad04[0x10];                 // 0x04-0x13
    CfObjectImplObjSub* field_14;    // 0x14 (driver sub-object)
    u8* field_18;                    // 0x18 (actor object)
    u8 _pad1C[0x368 - 0x1C];         // 0x1C-0x367
    CPartsChange mPartsChange;       // 0x368-0x397 (embedded parts-change)
};

// Command parameter consumed by func_800CA590: mode byte at +0xA (0x10 /
// 0x11) and a digit char at +0x1C parsed via atoi when it is '0'-'9'.
struct CfObjCmdParam {
    u8 _pad00[0xA];              // 0x00-0x09
    u8 field_0xA;                // 0x0A mode
    u8 _pad0B[0x1C - 0x0B];      // 0x0B-0x1B
    u8 field_0x1C;               // 0x1C
};

} // namespace cf

// C-ABI imports (retail symbols are unmangled - keep linkage/signatures
// verbatim; same scheme as CfObjectImplPc.hpp / CfObjectImplEne.hpp).
extern "C" {
void func_800CA948(void* self);
void func_800CAB00(CfObjectImplMoveObj* self);
void func_8015BB3C(void* a, void* b, void* c);
void func_80192E80(cf::CPartsChange* self, u8 r4, PartsChangeIf* obj);
int atoi(const char* str);
}

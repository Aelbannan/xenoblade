#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void* func_8016FE34(void* source);
extern "C" u32 func_8009CF8C(u32 resourceId);
extern "C" void* func_800BF324(void* objParam);

namespace cf {

// Phantom view of the embedded sub-object at +0x3E9C of the player object:
// its vtable slot 0x4C (index 17 with -RTTI leading entries) returns the
// parameter used for the talk-target lookup.
struct CParamSub4C {
    virtual void* f00();
    virtual void* f01();
    virtual void* f02();
    virtual void* f03();
    virtual void* f04();
    virtual void* f05();
    virtual void* f06();
    virtual void* f07();
    virtual void* f08();
    virtual void* f09();
    virtual void* f0A();
    virtual void* f0B();
    virtual void* f0C();
    virtual void* f0D();
    virtual void* f0E();
    virtual void* f0F();
    virtual void* f10();
    virtual void* vf4C(); // index 17 -> vtable 0x4C
};

// Actor object returned by func_800BF324(findObjectById(...)): state id at
// vtable 0x228 (index 136) and a u16 id at +0x8C.
struct CTalkActor : public CParamSub4C {
    // Filler slots 0x50-0x224 (indices 18-135) so vf228 sits at its retail offset.
    virtual void* g00(); virtual void* g01(); virtual void* g02(); virtual void* g03();
    virtual void* g04(); virtual void* g05(); virtual void* g06(); virtual void* g07();
    virtual void* g08(); virtual void* g09(); virtual void* g10(); virtual void* g11();
    virtual void* g12(); virtual void* g13(); virtual void* g14(); virtual void* g15();
    virtual void* g16(); virtual void* g17(); virtual void* g18(); virtual void* g19();
    virtual void* g20(); virtual void* g21(); virtual void* g22(); virtual void* g23();
    virtual void* g24(); virtual void* g25(); virtual void* g26(); virtual void* g27();
    virtual void* g28(); virtual void* g29(); virtual void* g30(); virtual void* g31();
    virtual void* g32(); virtual void* g33(); virtual void* g34(); virtual void* g35();
    virtual void* g36(); virtual void* g37(); virtual void* g38(); virtual void* g39();
    virtual void* g40(); virtual void* g41(); virtual void* g42(); virtual void* g43();
    virtual void* g44(); virtual void* g45(); virtual void* g46(); virtual void* g47();
    virtual void* g48(); virtual void* g49(); virtual void* g50(); virtual void* g51();
    virtual void* g52(); virtual void* g53(); virtual void* g54(); virtual void* g55();
    virtual void* g56(); virtual void* g57(); virtual void* g58(); virtual void* g59();
    virtual void* g60(); virtual void* g61(); virtual void* g62(); virtual void* g63();
    virtual void* g64(); virtual void* g65(); virtual void* g66(); virtual void* g67();
    virtual void* g68(); virtual void* g69(); virtual void* g70(); virtual void* g71();
    virtual void* g72(); virtual void* g73(); virtual void* g74(); virtual void* g75();
    virtual void* g76(); virtual void* g77(); virtual void* g78(); virtual void* g79();
    virtual void* g80(); virtual void* g81(); virtual void* g82(); virtual void* g83();
    virtual void* g84(); virtual void* g85(); virtual void* g86(); virtual void* g87();
    virtual void* g88(); virtual void* g89(); virtual void* g90(); virtual void* g91();
    virtual void* g92(); virtual void* g93(); virtual void* g94(); virtual void* g95();
    virtual void* g96(); virtual void* g97(); virtual void* g98(); virtual void* g99();
    virtual void* g100(); virtual void* g101(); virtual void* g102(); virtual void* g103();
    virtual void* g104(); virtual void* g105(); virtual void* g106(); virtual void* g107();
    virtual void* g108(); virtual void* g109(); virtual void* g110(); virtual void* g111();
    virtual void* g112(); virtual void* g113(); virtual void* g114(); virtual void* g115();
    virtual void* g116(); virtual void* g117();
    virtual u32 vf228();  // index 136 -> vtable 0x228 (state id)

    u8 _pad04[0x8C - 0x04];
    u16 field_8C; // 0x8C
};

// Result of func_8016FE34: sub-object pointer at +0x04 whose vtable slot
// 0x30 yields the word queried by func_80174C98.
struct CVoiceSub30 {
    virtual void* f00();
    virtual void* f01();
    virtual void* f02();
    virtual void* f03();
    virtual void* f04();
    virtual void* f05();
    virtual void* f06();
    virtual void* f07();
    virtual void* f08();
    virtual void* f09();
    virtual u32* vf30(); // index 10 -> vtable 0x30 (returns pointed-to query word)
};

struct CVoiceRec {
    u8 _pad00[0x4];
    CVoiceSub30* field_04; // 0x04
};

// Player record view with the parameter sub-object embedded at +0x3E9C
// (embedded members make MWCC emit the retail lwzu dispatch sequence).
struct CPlayerTalkRec {
    u8 _pad00[0x3E9C];
    CParamSub4C field_3E9C;
};

// CHelp_Talk inherits CHelpSwitch (mFlag at +0xC). Extra fields at +0x10..+0x16.
class __declspec(novtable) CHelp_Talk : public CHelpSwitch {
public:
    void resetTalkHelp();
    bool func_802B86F0();

    s32 field_0x10;  // 0x10 - compared (signed cmp) against a u16 from another object
    u8 field_0x14;   // 0x14 - flag checked before func_8009CF8C call
    u8 field_0x15;   // 0x15 - flag checked before final block
    u8 field_0x16;   // 0x16 - main state/result byte, read and written throughout
};

} // namespace cf

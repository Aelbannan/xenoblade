#pragma once

#include <types.h>
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
    int           func_802A3E88(CVS_THREAD* self);
    void          func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int           func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int           func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
    int           func_802A77E8(CVoiceHandle* handle);
    int           func_802A7850(int iter);
    int           func_802A7B90(CVoiceHandle* handle1, CVoiceHandle* handle2);
    CVoiceHandle* func_802A330C(int size, int align);
    u8*           func_802A34E4(int size);
    void          __ct__cf_CVS_THREAD();
}

// Init state triples and this subclass's vtable.
extern "C" u32 lbl_eu_80539C98[3];
extern "C" u32 lbl_eu_80539CB0[3];
extern "C" u32 lbl_eu_80539CA4[3];
extern "C" u32 lbl_eu_80539CBC[7];

// Phantom vtable view over CVoiceHandle so virtual dispatch is emitted as a
// true r12-chained indirect call (lwz r12,0(r3); lwz r12,0x2BC(r12); ...),
// matching retail. CVoiceHandle's runtime vtable is retail data that is never
// constructed from C++; we only cast + call a slot, so no vtable is emitted
// and no method needs a definition. isActive() sits at slot 0x2BC and
// state() at slot 0x308.
class CVoiceHandleVTV {
public:
    virtual void f0();   virtual void f1();   virtual void f2();   virtual void f3();
    virtual void f4();   virtual void f5();   virtual void f6();   virtual void f7();
    virtual void f8();   virtual void f9();   virtual void f10();  virtual void f11();
    virtual void f12();  virtual void f13();  virtual void f14();  virtual void f15();
    virtual void f16();  virtual void f17();  virtual void f18();  virtual void f19();
    virtual void f20();  virtual void f21();  virtual void f22();  virtual void f23();
    virtual void f24();  virtual void f25();  virtual void f26();  virtual void f27();
    virtual void f28();  virtual void f29();  virtual void f30();  virtual void f31();
    virtual void f32();  virtual void f33();  virtual void f34();  virtual void f35();
    virtual void f36();  virtual void f37();  virtual void f38();  virtual void f39();
    virtual void f40();  virtual void f41();  virtual void f42();  virtual void f43();
    virtual void f44();  virtual void f45();  virtual void f46();  virtual void f47();
    virtual void f48();  virtual void f49();  virtual void f50();  virtual void f51();
    virtual void f52();  virtual void f53();  virtual void f54();  virtual void f55();
    virtual void f56();  virtual void f57();  virtual void f58();  virtual void f59();
    virtual void f60();  virtual void f61();  virtual void f62();  virtual void f63();
    virtual void f64();  virtual void f65();  virtual void f66();  virtual void f67();
    virtual void f68();  virtual void f69();  virtual void f70();  virtual void f71();
    virtual void f72();  virtual void f73();  virtual void f74();  virtual void f75();
    virtual void f76();  virtual void f77();  virtual void f78();  virtual void f79();
    virtual void f80();  virtual void f81();  virtual void f82();  virtual void f83();
    virtual void f84();  virtual void f85();  virtual void f86();  virtual void f87();
    virtual void f88();  virtual void f89();  virtual void f90();  virtual void f91();
    virtual void f92();  virtual void f93();  virtual void f94();  virtual void f95();
    virtual void f96();  virtual void f97();  virtual void f98();  virtual void f99();
    virtual void f100(); virtual void f101(); virtual void f102(); virtual void f103();
    virtual void f104(); virtual void f105(); virtual void f106(); virtual void f107();
    virtual void f108(); virtual void f109(); virtual void f110(); virtual void f111();
    virtual void f112(); virtual void f113(); virtual void f114(); virtual void f115();
    virtual void f116(); virtual void f117(); virtual void f118(); virtual void f119();
    virtual void f120(); virtual void f121(); virtual void f122(); virtual void f123();
    virtual void f124(); virtual void f125(); virtual void f126(); virtual void f127();
    virtual void f128(); virtual void f129(); virtual void f130(); virtual void f131();
    virtual void f132(); virtual void f133(); virtual void f134(); virtual void f135();
    virtual void f136(); virtual void f137(); virtual void f138(); virtual void f139();
    virtual void f140(); virtual void f141(); virtual void f142(); virtual void f143();
    virtual void f144(); virtual void f145(); virtual void f146(); virtual void f147();
    virtual void f148(); virtual void f149(); virtual void f150(); virtual void f151();
    virtual void f152(); virtual void f153(); virtual void f154(); virtual void f155();
    virtual void f156(); virtual void f157(); virtual void f158(); virtual void f159();
    virtual void f160(); virtual void f161(); virtual void f162(); virtual void f163();
    virtual void f164(); virtual void f165(); virtual void f166(); virtual void f167();
    virtual void f168(); virtual void f169(); virtual void f170(); virtual void f171();
    virtual void f172();
    virtual int  isActive();   // slot 173 @ 0x2BC: handle is-active check
    virtual void f174(); virtual void f175(); virtual void f176(); virtual void f177();
    virtual void f178(); virtual void f179(); virtual void f180(); virtual void f181();
    virtual void f182(); virtual void f183(); virtual void f184(); virtual void f185();
    virtual void f186(); virtual void f187(); virtual void f188(); virtual void f189();
    virtual void f190(); virtual void f191();
    virtual int  state();      // slot 192 @ 0x308: handle state check
};

/**
 * CVS_THREAD_REVIVE - voice thread variant for revival/healing audio.
 *
 * Inherits from CVS_THREAD and returns a thread-local buffer of 230 bytes
 * (0xE6) for voice processing during character revival events.
 */
class CVS_THREAD_REVIVE : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0xE6;

    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)

    int blank1() override;
};
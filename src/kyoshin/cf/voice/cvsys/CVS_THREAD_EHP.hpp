#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

struct CVoiceHandle;

// CVS_THREAD_EHP: Voice thread for EHP (Emergency HP recovery) sequences.
// Object size 0x48 (72 bytes). The buffer-size virtual (func_802A6818, the
// CVS_THREAD::blank1 slot) returns 0xB4 (180). The EHP vtable
// (lbl_eu_80539B2C) is assigned manually by the factory __ct__802A5ED4.
//
// Field layout (base CVS_THREAD occupies 0x00-0x1F, vtable at 0x1C):
//   0x20  field_0x20   voice handle slot 1 (owner1)
//   0x24  field_0x24   voice handle slot 2 (owner2)
//   0x28  field_0x28   third constructor parameter
//   0x2C  field_0x2c   voice handle array (3 slots)
//   0x38  field_0x38   current rotating index
//   0x3C  field_0x3c   slot count / wrap bound (index range 0..field_0x3c)
//   0x40  field_0x40   stop/target index (triggers playback virtual)
//   0x44  field_0x44   direction flag (0 = forward, nonzero = backward)
class CVS_THREAD_EHP : public CVS_THREAD {
public:
    static const int BUFFER_SIZE = 0xB4;

    CVoiceHandle* field_0x20;    // 0x20: voice handle slot 1
    CVoiceHandle* field_0x24;    // 0x24: voice handle slot 2
    s32 field_0x28;              // 0x28: third constructor parameter
    CVoiceHandle* field_0x2c[3]; // 0x2C: voice handle slots
    s32 field_0x38;              // 0x38: current rotating index
    s32 field_0x3c;              // 0x3C: slot count / wrap bound
    s32 field_0x40;              // 0x40: stop/target index
    u8 field_0x44;               // 0x44: direction flag
};

// Manager-flag view over the voice-handle allocation: the flag word lives at
// +0x3F08 (bit 15 = EHP playback enabled).
struct CVoiceHandleFlags {
    u8 _00[0x3F08];
    u32 field_0x3F08;
};

// View of the {field_0, field_4, callback} slot-state triple installed at
// offset 0x00 of a CVS_THREAD (the ptmf + padding region).
struct CVSlotState {
    u32 field_0;
    u32 field_4;
    u32 callback;
};

// 4-byte-strided walk view over the rotating handle slots: each step lands
// one CVoiceHandle* later, with the slot always seen at offset 0x2C.
struct CVSlotWalk {
    u8 _00[0x2C];
    CVoiceHandle* slot;
};

// Phantom vtable view over a CVoiceHandle so the is-active check at vtable
// byte offset 0x2BC dispatches as a real r12-chained virtual call matching
// retail. Only cast + call a slot, so no vtable is emitted in this TU.
struct CVoiceChainVTV {
#define EHP_VTV_N(n) virtual void s##n();
    EHP_VTV_N(0) EHP_VTV_N(1) EHP_VTV_N(2) EHP_VTV_N(3) EHP_VTV_N(4) EHP_VTV_N(5) EHP_VTV_N(6) EHP_VTV_N(7) EHP_VTV_N(8) EHP_VTV_N(9)
    EHP_VTV_N(10) EHP_VTV_N(11) EHP_VTV_N(12) EHP_VTV_N(13) EHP_VTV_N(14) EHP_VTV_N(15) EHP_VTV_N(16) EHP_VTV_N(17) EHP_VTV_N(18) EHP_VTV_N(19)
    EHP_VTV_N(20) EHP_VTV_N(21) EHP_VTV_N(22) EHP_VTV_N(23) EHP_VTV_N(24) EHP_VTV_N(25) EHP_VTV_N(26) EHP_VTV_N(27) EHP_VTV_N(28) EHP_VTV_N(29)
    EHP_VTV_N(30) EHP_VTV_N(31) EHP_VTV_N(32) EHP_VTV_N(33) EHP_VTV_N(34) EHP_VTV_N(35) EHP_VTV_N(36) EHP_VTV_N(37) EHP_VTV_N(38) EHP_VTV_N(39)
    EHP_VTV_N(40) EHP_VTV_N(41) EHP_VTV_N(42) EHP_VTV_N(43) EHP_VTV_N(44) EHP_VTV_N(45) EHP_VTV_N(46) EHP_VTV_N(47) EHP_VTV_N(48) EHP_VTV_N(49)
    EHP_VTV_N(50) EHP_VTV_N(51) EHP_VTV_N(52) EHP_VTV_N(53) EHP_VTV_N(54) EHP_VTV_N(55) EHP_VTV_N(56) EHP_VTV_N(57) EHP_VTV_N(58) EHP_VTV_N(59)
    EHP_VTV_N(60) EHP_VTV_N(61) EHP_VTV_N(62) EHP_VTV_N(63) EHP_VTV_N(64) EHP_VTV_N(65) EHP_VTV_N(66) EHP_VTV_N(67) EHP_VTV_N(68) EHP_VTV_N(69)
    EHP_VTV_N(70) EHP_VTV_N(71)
    virtual float getMax();  // effective slot 74 @ 0x128: maximum gauge value
    virtual float getCur();  // effective slot 75 @ 0x12C: current gauge value
    EHP_VTV_N(74) EHP_VTV_N(75) EHP_VTV_N(76) EHP_VTV_N(77) EHP_VTV_N(78) EHP_VTV_N(79)
    EHP_VTV_N(80) EHP_VTV_N(81) EHP_VTV_N(82) EHP_VTV_N(83) EHP_VTV_N(84) EHP_VTV_N(85) EHP_VTV_N(86) EHP_VTV_N(87) EHP_VTV_N(88) EHP_VTV_N(89)
    EHP_VTV_N(90) EHP_VTV_N(91) EHP_VTV_N(92) EHP_VTV_N(93) EHP_VTV_N(94) EHP_VTV_N(95) EHP_VTV_N(96) EHP_VTV_N(97) EHP_VTV_N(98) EHP_VTV_N(99)
    EHP_VTV_N(100) EHP_VTV_N(101) EHP_VTV_N(102) EHP_VTV_N(103) EHP_VTV_N(104) EHP_VTV_N(105) EHP_VTV_N(106) EHP_VTV_N(107) EHP_VTV_N(108) EHP_VTV_N(109)
    EHP_VTV_N(110) EHP_VTV_N(111) EHP_VTV_N(112) EHP_VTV_N(113) EHP_VTV_N(114) EHP_VTV_N(115) EHP_VTV_N(116) EHP_VTV_N(117) EHP_VTV_N(118) EHP_VTV_N(119)
    EHP_VTV_N(120) EHP_VTV_N(121) EHP_VTV_N(122) EHP_VTV_N(123) EHP_VTV_N(124) EHP_VTV_N(125) EHP_VTV_N(126) EHP_VTV_N(127) EHP_VTV_N(128) EHP_VTV_N(129)
    EHP_VTV_N(130) EHP_VTV_N(131) EHP_VTV_N(132) EHP_VTV_N(133) EHP_VTV_N(134) EHP_VTV_N(135) EHP_VTV_N(136) EHP_VTV_N(137) EHP_VTV_N(138) EHP_VTV_N(139)
    EHP_VTV_N(140) EHP_VTV_N(141) EHP_VTV_N(142) EHP_VTV_N(143) EHP_VTV_N(144) EHP_VTV_N(145) EHP_VTV_N(146) EHP_VTV_N(147) EHP_VTV_N(148) EHP_VTV_N(149)
    EHP_VTV_N(150) EHP_VTV_N(151) EHP_VTV_N(152) EHP_VTV_N(153) EHP_VTV_N(154) EHP_VTV_N(155) EHP_VTV_N(156) EHP_VTV_N(157) EHP_VTV_N(158) EHP_VTV_N(159)
    EHP_VTV_N(160) EHP_VTV_N(161) EHP_VTV_N(162) EHP_VTV_N(163) EHP_VTV_N(164) EHP_VTV_N(165) EHP_VTV_N(166) EHP_VTV_N(167) EHP_VTV_N(168) EHP_VTV_N(169)
    EHP_VTV_N(170) EHP_VTV_N(171) EHP_VTV_N(172)
#undef EHP_VTV_N
    virtual int isActive();  // slot 173 @ 0x2BC: handle is-active check
};

// Phantom vtable view over the polymorphic sub-object reached through a
// voice handle (CVoiceHandle+0x04): slot 12 (@ 0x30) returns a pointer that
// the caller dereferences for the category check. Only cast + call a slot,
// so no vtable is emitted in this TU.
struct CVSubChainVTV {
#define EHP_SUB_N(n) virtual void s##n();
    EHP_SUB_N(0) EHP_SUB_N(1) EHP_SUB_N(2) EHP_SUB_N(3) EHP_SUB_N(4) EHP_SUB_N(5)
    EHP_SUB_N(6) EHP_SUB_N(7) EHP_SUB_N(8) EHP_SUB_N(9)
#undef EHP_SUB_N
    virtual u32* getRef();   // dispatches @ 0x30 with this layout
};

// Forward declaration of a polymorphic sub-object reached through a voice
// handle (CVoiceHandle+0x04). Only the vtable pointer at offset 0 is used.
struct CVSubObj {
    void** vtable;               // 0x00: vtable pointer
};

// Voice-handle type. The actual CCharVoice is embedded at offset 0x3E9C
// within the handle allocation (0x3E9C bytes of handle data + CCharVoice).
struct CVoiceHandleListNode {
    CVoiceHandleListNode* next;
    CVoiceHandleListNode* previous;
    CVoiceHandle* value;
};

struct CVoiceHandleList {
    CVoiceHandleListNode* head;
    CVoiceHandleListNode* end;
};

extern "C" {
// Sibling TU functions (unmangled global symbols).
int func_802A3E88(CVS_THREAD* self);
void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
int func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
CVoiceHandle* func_802A7998(CVoiceHandle* exclude);
CVoiceHandle* func_802A330C(int size, int align);
CVoiceHandleList* func_800B6BC8();
int func_802A7FE4(CVoiceHandle* handle);
int func_802A77E8(CVoiceHandle* handle);
int func_802A7B90(CVoiceHandle* handle1, CVoiceHandle* handle2);
// Collect free voice handles from `slots`; returns usable count.
int func_802A7870(CVoiceHandle** slots, int count, CVoiceHandle* exclude);
}

// Slot-state triples used by the EHP playback selector (func_802A617C).
extern u32 lbl_eu_80539AC4[3]; // {0, -1, func_802A8B3C} - low-gauge slot state
extern u32 lbl_eu_80539AD0[3]; // {0, -1, func_802A8D84} - high-gauge slot state

// Init-state triple copied by the factory __ct__802A5ED4.
extern u32 lbl_eu_80539AB8[3];

// CVS_THREAD_EHP vtable (stored at offset 0x1C by the factory).
extern u32 lbl_eu_80539B2C[];

// Scratch-buffer allocator and base constructor shared by the cvsys voice
// thread factories (same C-ABI imports the sibling thread TUs declare).
extern "C" u8* func_802A34E4(int size);
extern "C" CVS_THREAD* __ct__cf_CVS_THREAD(CVS_THREAD* self);

// Runtime rethrow (NMWException.h): declared noreturn so MWCC elides the
// __end__catch epilogue of a catch-all handler that ends with `bl __throw`
// (retail catch-all handlers end at the rethrow).
extern "C" __declspec(noreturn) void __throw(char* throwtype, void* location, void* dtor);

// sdata2 constants: float thresholds.
extern float lbl_eu_80668C90;  // 0.0
extern float lbl_eu_80668C94;  // 0.3

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 lbl_eu_80539ADC[3]; // {0, -1, func_802A6718} - EHP start-request slot state
extern "C" u32 lbl_eu_80539B14[3]; // {0, -1, func_802A6408}
extern "C" u32 lbl_eu_80539B20[3]; // {0, -1, func_802A6718}

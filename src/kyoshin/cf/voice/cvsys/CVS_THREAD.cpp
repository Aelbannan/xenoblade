#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"          // lbl_eu_80663E14
#include "libs/monolib/src/scn/CScn_8049603C.hpp"     // func_8049603C (single owner decl)
#include "monolib/math/Random.hpp"

// Phantom vtable view over a CVoiceHandle vtable (vptr at +0x00) so the
// idle-check slot lands at byte offset 0x2BC (declared slot 173; MWCC
// places declared slot P at vtable offset (P+2)*4). Cast+call only, so no
// vtable is emitted for this view.

// Result of the target-state virtual at vtable offset 0x2A4: +0x04 is an
// actor id fed to findObjectById, +0x50 is a work object carrying a u16
// state word at +0x48 checked by the battle-state helpers.
struct UnkVoiceWork {
    u8 _00[0x48];
    u16 field_48;
};
struct CVoiceVf167Res {
    u8 _00[4];
    u32 field_04;
    u8 _08[0x48];
    UnkVoiceWork* field_50;
};

// C++-mangled imports (MWCC emits findObjectById__Fi /
// func_800BFC68__FPQ22cf12CfObjectMove, matching the retail symbols).
namespace cf { class CfObjectMove; }
void* findObjectById(int id);
void* func_800BFC68(cf::CfObjectMove* objMove);
// Battle-state helpers (retail unmangled C-ABI symbols).
extern "C" int func_80146148(int value);
extern "C" int func_80146300(u32 id, u32 flag);

struct CVoiceVTV {
#define CVTV_N(n) virtual void s##n();
    CVTV_N(0) CVTV_N(1) CVTV_N(2) CVTV_N(3) CVTV_N(4) CVTV_N(5) CVTV_N(6) CVTV_N(7) CVTV_N(8) CVTV_N(9)
    CVTV_N(10) CVTV_N(11) CVTV_N(12) CVTV_N(13) CVTV_N(14) CVTV_N(15) CVTV_N(16) CVTV_N(17) CVTV_N(18) CVTV_N(19)
    CVTV_N(20) CVTV_N(21) CVTV_N(22) CVTV_N(23) CVTV_N(24) CVTV_N(25) CVTV_N(26) CVTV_N(27) CVTV_N(28) CVTV_N(29)
    CVTV_N(30) CVTV_N(31) CVTV_N(32) CVTV_N(33) CVTV_N(34) CVTV_N(35) CVTV_N(36) CVTV_N(37) CVTV_N(38) CVTV_N(39)
    CVTV_N(40) CVTV_N(41) CVTV_N(42) CVTV_N(43) CVTV_N(44) CVTV_N(45) CVTV_N(46) CVTV_N(47) CVTV_N(48) CVTV_N(49)
    CVTV_N(50) CVTV_N(51) CVTV_N(52) CVTV_N(53) CVTV_N(54) CVTV_N(55) CVTV_N(56) CVTV_N(57) CVTV_N(58) CVTV_N(59)
    CVTV_N(60) CVTV_N(61) CVTV_N(62) CVTV_N(63) CVTV_N(64) CVTV_N(65) CVTV_N(66) CVTV_N(67) CVTV_N(68) CVTV_N(69)
    CVTV_N(70) CVTV_N(71) CVTV_N(72) CVTV_N(73) CVTV_N(74) CVTV_N(75) CVTV_N(76) CVTV_N(77) CVTV_N(78) CVTV_N(79)
    CVTV_N(80) CVTV_N(81) CVTV_N(82) CVTV_N(83) CVTV_N(84) CVTV_N(85) CVTV_N(86) CVTV_N(87) CVTV_N(88) CVTV_N(89)
    CVTV_N(90) CVTV_N(91) CVTV_N(92) CVTV_N(93) CVTV_N(94) CVTV_N(95) CVTV_N(96) CVTV_N(97) CVTV_N(98) CVTV_N(99)
    CVTV_N(100) CVTV_N(101) CVTV_N(102) CVTV_N(103) CVTV_N(104) CVTV_N(105) CVTV_N(106) CVTV_N(107) CVTV_N(108) CVTV_N(109)
    CVTV_N(110) CVTV_N(111) CVTV_N(112) CVTV_N(113) CVTV_N(114) CVTV_N(115) CVTV_N(116) CVTV_N(117) CVTV_N(118) CVTV_N(119)
    CVTV_N(120) CVTV_N(121) CVTV_N(122) CVTV_N(123) CVTV_N(124) CVTV_N(125) CVTV_N(126) CVTV_N(127) CVTV_N(128) CVTV_N(129)
    CVTV_N(130) CVTV_N(131) CVTV_N(132) CVTV_N(133) CVTV_N(134) CVTV_N(135) CVTV_N(136) CVTV_N(137) CVTV_N(138) CVTV_N(139)
    CVTV_N(140) CVTV_N(141) CVTV_N(142) CVTV_N(143) CVTV_N(144) CVTV_N(145) CVTV_N(146) CVTV_N(147) CVTV_N(148) CVTV_N(149)
    CVTV_N(150) CVTV_N(151) CVTV_N(152) CVTV_N(153) CVTV_N(154) CVTV_N(155) CVTV_N(156) CVTV_N(157) CVTV_N(158) CVTV_N(159)
    CVTV_N(160) CVTV_N(161) CVTV_N(162) CVTV_N(163) CVTV_N(164) CVTV_N(165) CVTV_N(166)
    virtual CVoiceVf167Res* voiceTarget(); // slot 167 -> vtable offset 0x2A4
    CVTV_N(168) CVTV_N(169) CVTV_N(170) CVTV_N(171) CVTV_N(172)
#undef CVTV_N
    virtual int idle();               // slot 173 -> vtable offset 0x2BC
};

// ── Local phantom views for func_802A3D54 ─────────────────────────────────

// Voice object flag word at +0x64 (bit 2 selects the alternate sdata2
// constant for the sound request).
struct CVoiceSndFlagView {
    u8 _00[0x64];
    u32 flag64;
};

// Scene camera-view block returned by func_8049603C (float at +0xC is the
// remaining display-time fraction subtracted from the base volume).
struct CVoiceSndCamView {
    u8 _00[0xC];
    f32 field_C;
};

namespace cf {
// TU-local phantom view of cf::CfObject: emits the retail-mangled reloc
// requestVoice__Q22cf8CfObjectFiUlff. object/CfObject.hpp declares a
// void-returning form shared with other TUs; the mangling encodes neither
// the return type, and this call site passes the callee's r3 straight out.
class CfObject {
public:
    int requestVoice(int a, unsigned long b, float c, float d);
};
} // namespace cf

// ── Extern symbols ────────────────────────────────────────────────────────

extern "C" {
    extern u32 lbl_eu_80539910[];   // vtable for CVS_THREAD
    extern void func_800BE924(void* voice);
    extern int func_800BE8B4(u32 handle, int wait);
    extern u8* lbl_eu_80664A58;     // voice manager global pointer
    extern int func_802A77E8(CVoiceHandle* handle);
    extern int func_802A7850(int iter);
    extern int func_80148778(u32* obj, int arg);
}

// ── __ct__cf_CVS_THREAD (constructor) ─────────────────────────────────────
// Initialises a CVS_THREAD: stores the vtable at offset 0x1C, zeros unkC
// and unk10, calls func_802A35A0(this) to obtain an ID stored in unk18,
// and returns this.
// (Defined first: retail emits the ctor at the start of this TU's .text,
// with func_802A3E74 much later, after func_802A3E28.)
extern "C" CVS_THREAD* __ct__cf_CVS_THREAD(CVS_THREAD* self) {
    // Store the vtable pointer at offset 0x1C.
    *(u32*)((u8*)self + 0x1C) = (u32)&lbl_eu_80539910;
    self->unkC = 0;
    self->unk10 = 0;
    self->unk18 = func_802A35A0((unsigned int)self);
    return self;
}

// ── func_802A3ACC ─────────────────────────────────────────────────────────────────
// Each voice-event node stores a raw 12-byte ptmf at offset 0. Nodes whose
// handler differs from the reference ptmf at lbl_eu_805398F8 (and is set)
// get their handler invoked in place via the member-pointer runtime.
extern "C" void func_802A3ACC(void* node) {
    bool doCall = false;
    if (__ptmf_cmpr(node, lbl_eu_805398F8) != 0 && __ptmf_test(node) != 0)
        doCall = true;

    if (doCall) {
        if (__ptmf_test(node) != 0) {
            // Dispatch the installed member callback (retail __ptmf_scall).
            // The reinterpret deref keeps the ptmf in place at node+0, so
            // MWCC passes obj and &ptmf as the same register (retail r12=r3).
            (reinterpret_cast<CVoiceCbHost*>(node)->*
                (*reinterpret_cast<const VoiceCb*>(node)))();
        }
    }
}

// Installs the default voice-event handler ptmf, then polls the active
// voice: when the poll reports the handle still busy the handle state is
// torn down either way (both branches clear +0x10/+0x14).
void CVS_THREAD::func_802A3B50() {
    s32 poll;
    u32 handle = unk10;
    unk0 = lbl_eu_80539904;
    if (handle != 0 && unk14 >= 0) {
        poll = func_800BE8B4(handle, unk14);
        if (poll == 0) {
            unk10 = 0;
            unk14 = -1;
        }
    } else {
        poll = 0;
    }
    if (poll != 0) {
        unk10 = 0;
        unk14 = -1;
    }
}

// Compare the +0x10 field against the id; on a match dispatch func_800BE924
// and clear the +0x10/+0x14 pair.
void CVS_THREAD::func_802A3BEC(u32 id) {
    u32 v = unk10;
    if (v == 0)
        return;
    if (v == id) {
        if (v != 0)
            func_800BE924((void*)v);
        unk10 = 0;
        unk14 = -1;
    }
}

// ── func_802A3C44 ─────────────────────────────────────────────────────────
// Thread-driven variant of func_802A3D54: polls the thread's idle virtual
// (blank1) for the sound-request argument, applies the same game-manager
// gate and volume scaling, then records the started voice (+id) into the
// thread's +0x10/+0x14 pair when the request succeeded.
int func_802A3C44(CVS_THREAD* thread, CCharVoice* voice, s32 voiceId) {
    int result;
    // The two post-poll exits keep the shared store-check join (retail does
    // not fold them past the result test), so they assign-and-branch instead
    // of returning directly.
    if (voiceId < 0) {
        return 0;
    }
    int handle = thread->blank1();
    if (voiceId < 0) {
        result = 0;
        goto check;
    }
    if (cf::CfGameManager::isSceneLoading() != 0) {
        result = 0;
        goto check;
    }
    if (((CVoiceSndFlagView*)voice)->flag64 & 0x4) {
        // Flag set: alternate constant for the fourth argument.
        f32 vol = lbl_eu_80668C88 - ((CVoiceSndCamView*)func_8049603C(lbl_eu_80663E14))->field_C;
        result = ((cf::CfObject*)voice)->requestVoice(voiceId, handle,
                                                       lbl_eu_80662CB0 * vol,
                                                       lbl_eu_80662CB8);
    } else {
        f32 vol = lbl_eu_80668C88 - ((CVoiceSndCamView*)func_8049603C(lbl_eu_80663E14))->field_C;
        result = ((cf::CfObject*)voice)->requestVoice(voiceId, handle,
                                                       lbl_eu_80662CB0 * vol,
                                                       lbl_eu_80662CB4);
    }
check:
    if (result != 0) {
        thread->unk10 = (u32)voice;
        thread->unk14 = voiceId;
    }
    return result;
}

extern "C" int func_802A3D54(CCharVoice* voice, int voiceId, int arg);

// ── func_802A4798 ─────────────────────────────────────────────────────────
// Bit-1 gate on +0x3F00 plus the handle's idle-check virtual (vtable
// 0x2BC), validate the target's voice-thread state (1-6), reserve a
// scratch buffer for voice id 0x122, then play a random line from the
// 0xD49.. block through the embedded CCharVoice.
int func_802A4798(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 2))
        return 0;
    if (((CVoiceVTV*)self)->idle() != 0)
        return 0;
    // Target pointer chain must exist and carry a state in [1, 6].
    UnkTarget* target = self->unkTarget;
    if (target == NULL)
        return 0;
    UnkTargetInner* inner = target->field_0x08;
    int state = inner->field_0x18;
    // State must be in [1, 6]; the second guard skips over the shared
    // failure block when in range, matching retail cmpi/blt + cmpi/ble.
    if (state < 1)
        goto fail;
    if (6 < state)
    {
    fail:
        return 0;
    }
    if (func_802A330C(0x122, 1) == NULL)
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL)
        voicePtr = &self->voice;
    func_802A3D54(voicePtr, ml::math::mtRand(3) + 0xD49, 0x122);
    return 0;
}

// ── func_802A3D54 ─────────────────────────────────────────────────────────
// Voice playback request: gated on a valid voice id and the game-manager
// reset/menu state, then scales lbl_eu_80668C88 by the scene camera's
// remaining display fraction and forwards (voiceId, arg, volume, constant)
// to the object's sound request. Bit 2 of the +0x64 flag word picks the
// alternate fourth-argument constant.
// __declspec(noinline): retail keeps a plain bl from func_802A4798;
// without this MWCC -O4 inlines the body into the caller.
// extern "C": retail symbol is the unmangled func_802A3D54.
extern "C" __declspec(noinline) int func_802A3D54(CCharVoice* voice, int voiceId, int arg) {
    if (voiceId < 0) {
        return 0;
    }
    if (cf::CfGameManager::isSceneLoading() != 0) {
        return 0;
    }
    CVoiceSndFlagView* fv = (CVoiceSndFlagView*)voice;
    if (fv->flag64 & 0x4) {
        // Flag set: alternate constant for the fourth argument.
        f32 vol = lbl_eu_80668C88 - ((CVoiceSndCamView*)func_8049603C(lbl_eu_80663E14))->field_C;
        return ((cf::CfObject*)voice)->requestVoice(voiceId, arg,
                                                     lbl_eu_80662CB0 * vol,
                                                     lbl_eu_80662CB8);
    } else {
        f32 vol = lbl_eu_80668C88 - ((CVoiceSndCamView*)func_8049603C(lbl_eu_80663E14))->field_C;
        return ((cf::CfObject*)voice)->requestVoice(voiceId, arg,
                                                     lbl_eu_80662CB0 * vol,
                                                     lbl_eu_80662CB4);
    }
}

void func_802A3E28(CVS_THREAD* thread) {
    if (thread->unk10 != 0) {
        func_800BE924((void*)thread->unk10);
    }
    thread->unk10 = 0;
    thread->unk14 = -1;
}

// ── func_802A3E74 ─────────────────────────────────────────────────────────
// If the thread has a non-null voice handle at unk10, stop it via the sound
// system call func_800BE924.
void func_802A3E74(CVS_THREAD* thread) {
    if (thread->unk10 != 0) {
        func_800BE924((void*)thread->unk10);
    }
}

// Stops the active voice if its playback finished: queries the sound
// system via func_800BE8B4 and clears the +0x10/+0x14 pair once the voice
// is no longer playing.
int func_802A3E88(CVS_THREAD* thread) {
    // Voice still referenced: ask the sound system if it finished.
    if (thread->unk10 == 0)
        goto fail;
    if (thread->unk14 < 0)
        goto fail;
    int ret = func_800BE8B4(thread->unk10, thread->unk14);
    if (ret == 0) {
        thread->unk10 = 0;
        thread->unk14 = -1;
    }
    return ret;
fail:
    return 0;
}
// ── func_802A3EF0 ─────────────────────────────────────────────────────────
extern "C" int func_802A3FD4(CVoiceHandle* self);
extern "C" int func_802A4120(CVoiceHandle* self);
// Retail symbol for func_802A4430 is the unmangled func_802A4430.
extern "C" int func_802A4430(CVoiceHandle* self);
// Idle-triggered ambient/battle voice entry: gated on the manager flag bit
// 1 and the handle's idle-check virtual (vtable 0x2BC), picks a voice id by
// cascading three selectors, reserves the 0xA0 scratch buffer, then plays
// the line through the embedded CCharVoice.
int func_802A3EF0(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 2))
        return 0;
    if (((CVoiceVTV*)self)->idle() != 0)
        return 0;
    int voiceId = func_802A3FD4(self);
    if (voiceId == -1)
        voiceId = func_802A4120(self);
    if (voiceId == -1)
        voiceId = func_802A4430(self);
    if (voiceId <= 0)
        return 0;
    if (func_802A330C(0xA0, 1) == NULL)
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL)
        voicePtr = &self->voice;
    func_802A3D54(voicePtr, voiceId, 0xA0);
    return 0;
}

// ── func_802A3FD4 ─────────────────────────────────────────────────────────
// Idle ambient-line selector. Resolves the actor behind this handle via the
// target-state virtual (vtable 0x2A4): its +0x04 id goes through the
// actor-source lookup and CfObjectMove resolution. Gates on the +0x50 work
// object's state word (+0x48) passing either battle-state check, rolls a
// 25% play chance, then picks the line from the actor's voice-thread state.
extern "C" __declspec(noinline) int func_802A3FD4(CVoiceHandle* self) {
    // Declared up front in first-definition order so MWCC colors them
    // r29/r30/r31 like the retail body.
    cf::CfObjectMove* actor;
    UnkVoiceWork* work;
    CVoiceVf167Res* res;
    int state;

    // First dispatch of the target-state virtual: its +0x04 actor id goes
    // through the actor-source lookup and CfObjectMove resolution.
    res = ((CVoiceVTV*)self)->voiceTarget();
    actor = (cf::CfObjectMove*)func_800BFC68(
        (cf::CfObjectMove*)findObjectById(res->field_04));
    if (actor == NULL)
        return -1;

    // Second dispatch: the +0x50 work object's +0x48 word must pass either
    // the event-type filter or the always-on-id check.
    res = ((CVoiceVTV*)self)->voiceTarget();
    work = res->field_50;
    // `state` holds the gate flag, then the voice-thread state query.
    if (work != NULL) {
        state = func_80146148(work->field_48) ||
                func_80146300(work->field_48, 1);
    } else {
        state = 0;
    }
    if (state == 0)
        return -1;

    // 25% chance to actually speak.
    if (ml::math::mtRand(100) >= 25)
        return -1;

    int selfState;
    int actorState;
    selfState = func_802A77E8(self);
    actorState = func_802A77E8((CVoiceHandle*)actor);
    if (selfState == 1) {
        if (actorState == 3)
            return 0x8FD;
        if (actorState == 2)
            return 0x8FE;
        if (actorState == 4)
            return 0x8FF;
    }
    return -1;
}
// ── func_802A4120 ─────────────────────────────────────────────────────────
// Battle-context voice-line selector. Reads the character type from the
// target chain (or from the target-state virtual's work object when the
// thread state is 3-with-low-gauge or 10), applies the leader/player
// randomization, then picks a line from one of two base blocks depending
// on the leader flag, with equipment-state remaps layered on top.
extern "C" __declspec(noinline) int func_802A4120(CVoiceHandle* self) {
    // Character type via the target pointer chain.
    UnkTarget* target = self->unkTarget;
    int charType;
    if (target != NULL)
        charType = target->field_0x08->field_0x18;
    else
        charType = -1;

    // Character type must be in [0x10, 0x1F].
    if (charType < 0x10)
        goto invalid_char;
    if (0x1F >= charType)
        goto char_ok;
invalid_char:
    return -1;
char_ok:

    // Party-gauge override: when the thread is mid-chain-attack with a low
    // gauge, take the character type from the work object instead.
    {
        int gaugeActive = 0;
        if (func_802A77E8(self) == 3) {
            if ((u32)cf::CfGameManager::getQueuedFileEventCount() < 0x2A)
                gaugeActive = 1;
        }
        if (gaugeActive != 0) {
            CVoiceVf167Res* res = ((CVoiceVTV*)self)->voiceTarget();
            UnkEquipData* work = (UnkEquipData*)res->field_50;
            if (work == NULL) return -1;
            charType = work->field_0x77 + 0x10;
        }
    }
    if (func_802A77E8(self) == 0xA) {
        CVoiceVf167Res* res = ((CVoiceVTV*)self)->voiceTarget();
        UnkEquipData* work = (UnkEquipData*)res->field_50;
        if (work == NULL) return -1;
        charType = work->field_0x77 + 0x10;
    }

    // Leader check: bias handle to voice area at +0x3E9C, compare with
    // the player object. If this voice belongs to the player, there is
    // a 1-in-2 chance of treating it as a non-leader voice.
    CCharVoice* vp = (CCharVoice*)self;
    int isLeader = 1;
    if (self != NULL)
        vp = &self->voice;
    if (vp == (CCharVoice*)cf::CfGameManager::getPlayer(0)) {
        if (ml::math::mtRand(2) == 0)
            isLeader = 0;
    }

    // Thread states 0x11/0x1C/0x1D force the leader branch.
    if (func_802A77E8(self) == 1) {
        if (charType == 0x11) isLeader = 1;
        if (charType == 0x1C) isLeader = 1;
        if (charType == 0x1D) isLeader = 1;
    }

    int voiceId;
    if (isLeader != 0) {
        voiceId = charType + 0xDB1;
        // Equipment-category remap (category table entry 4 must exist).
        bool alt = false;
        if (func_802A77E8(self) == 4)
            alt = func_800A32BC(func_8009EC9C(4)) == 1;
        if (alt) {
            if (voiceId == 0xDC1) voiceId = 0xDD5;
            else if (voiceId == 0xDC2) voiceId = 0xDD6;
        }
        // Low party gauge during a chain attack shifts the line further.
        int gauge = 0;
        if (func_802A77E8(self) == 3 && (u32)cf::CfGameManager::getQueuedFileEventCount() < 0x2A)
            gauge = 1;
        if (gauge != 0) {
            if (voiceId == 0xDC1) voiceId = 0xDD5;
            else if (voiceId == 0xDC2) voiceId = 0xDD6;
            else if (voiceId == 0xDC3) voiceId = 0xDD7;
            else if (voiceId == 0xDC4) voiceId = 0xDD8;
        }
        return voiceId;
    } else {
        voiceId = charType + 0xD9D;
        bool alt = false;
        if (func_802A77E8(self) == 4)
            alt = func_800A32BC(func_8009EC9C(4)) == 1;
        if (alt) {
            if (voiceId == 0xDAD) voiceId = 0xDBC;
            else if (voiceId == 0xDAE) voiceId = 0xDB5;
        }
        int gauge = 0;
        if (func_802A77E8(self) == 3 && (u32)cf::CfGameManager::getQueuedFileEventCount() < 0x2A)
            gauge = 1;
        if (gauge != 0) {
            if (voiceId == 0xDAD) voiceId = 0xDB5;
            else if (voiceId == 0xDAE) voiceId = 0xDAF;
            else if (voiceId == 0xDAF) voiceId = 0xDB0;
            else if (voiceId == 0xDB0) voiceId = 0xDB2;
        }
        return voiceId;
    }
}
// ── func_802A4430 ─────────────────────────────────────────────────────────
// Voice-ID selector for battle-voice context.  Dispatches on
// voice-thread state (1-7) and character type to pick the appropriate
// voice line for tension, arts, chain-attack, revive, etc.

extern "C" int func_802A4430(CVoiceHandle* self) {
    int isLeader = 1;

    // Leader check: bias handle to voice area at +0x3E9C, compare with
    // the player object.  If this voice belongs to the player, there is
    // a 1-in-3 chance of treating it as a non-leader voice.
    {
        u8* vp = (u8*)self;
        if (self != NULL) {
            vp = (u8*)self + 0x3E9C;
        }
        if (vp == (u8*)cf::CfGameManager::getPlayer(0)) {
            if (ml::math::mtRand(2) == 0) {
                isLeader = 0;
            }
        }
    }

    // Extract character type from pointer chain.
    int charType;

    // Get voice-thread state.
    int state = func_802A77E8(self);

    {
        UnkTarget* target = self->unkTarget;
        if (target != NULL) {
            charType = target->field_0x08->field_0x18;
        } else {
            charType = -1;
        }
    }

    // Character type must be in [7, 14].
    if (charType < 7)
        goto invalid_char;
    if (charType - 15 < 0)
        goto char_ok;
invalid_char:
    return -1;
char_ok:

    // Validate iterator.
    if (func_802A7850(state) == 0) {
        return -1;
    }

    // Dispatch on voice-thread state.
    switch (state) {
    case 1:
        if (charType == 7) {
            // One-shot flag toggle on voice manager at offset 0x222.
            int flag;
            if (lbl_eu_80664A58[0x222] != 0) {
                lbl_eu_80664A58[0x222] = 0;
                flag = 1;
            } else {
                flag = 0;
            }
            if (flag != 0) {
                return 0xBBB;
            }
            return isLeader ? 0xDBD : 0xDD2;
        }
        if (charType == 8) {
            return isLeader ? 0xDD1 : 0xDB3;
        }
        goto fail;

    case 2:
        if (charType != 7) goto fail;
        // Check arts/skill availability via field at offset 0x8.
        if (func_80148778(&self->unk8, 0xED) != 0) {
            return isLeader ? 0xDD2 : 0xDBD;
        } else {
            return isLeader ? 0xDD1 : 0xDBD;
        }

    case 3: {
        // Party gauge check.  Reuses the charType register as the flag
        // (retail keeps both live ranges in r31).
        charType = 0;
        if (func_802A77E8(self) == 3) {
            if ((u32)cf::CfGameManager::getQueuedFileEventCount() < 42) {
                charType = 1;
            }
        }
        if (charType != 0) {
            return isLeader ? 0xDD9 : 0xDAD;
        }

        // No gauge: dispatch on sub-state from equipment data.
        {
            UnkWorkObj* workObj;
            UnkEquipData* equipData;
            u16 subState;

            workObj = ((UnkWorkObj*(*)(CVoiceHandle*))self->vtable[0xA9])(self);
            equipData = workObj->field_0x50;
            if (equipData == NULL) goto fail;
            subState = equipData->subState;

            // Jump-table dispatch (9 entries).  The unsigned bounds check
            // via cmplwi/bgt is generated by the switch itself.
            switch (subState - 0x77) {
            case 0: return isLeader ? 0xDD1 : 0xDBF;
            case 1: return isLeader ? 0xDD2 : 0xDBD;
            case 2: return isLeader ? 0xDD3 : 0xDAD;
            case 3: return isLeader ? 0xDD4 : 0xDBE;
            case 4: return isLeader ? 0xDD1 : 0xDBF;
            case 5: return isLeader ? 0xDD2 : 0xDBD;
            case 6: return isLeader ? 0xDD3 : 0xDAD;
            case 7: return isLeader ? 0xDD4 : 0xDBE;
            case 8: return isLeader ? 0xDD3 : 0xDAD;
            default: goto fail;
            }
        }
        goto fail;
    }

    case 4:
        // Character types 7-10 map linearly to voice IDs.
        if (charType < 7) goto fail;
        if (charType > 10) goto fail;
        return charType + 0xDCA;

    case 5:
        // Only valid for character types 7 and 8.
        if ((u32)(charType - 7) > 1) goto fail;
        return isLeader ? 0xDD1 : 0xDBD;

    case 6:
        if (charType != 7) goto fail;
        return 0xDD1;

    case 7:
        // Character types 8-13 map to dynamic voice IDs.
        if (charType < 8) goto fail;
        if (charType > 13) goto fail;
        return isLeader ? charType + 0xDCC : 0xDBE;
    }

fail:
    return -1;
}

#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_ORDER.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"
#include "monolib/math/Random.hpp"

// cf::CfGameManager / cf::CBattleManager have no shared standalone header;
// declare just the statics this TU uses.
namespace cf {
class CBattleManager {
public:
    static CBattleManager* getInstance();
};
}

// Local minimal layout for the gimmick list consumed by func_800B6BC8.
struct OrderGimmickNode {
    OrderGimmickNode* next; // 0x00
    u8 _04[4];              // 0x04..0x07
    u8* object;           // 0x08
};
struct OrderGimmickList {
    u8* field_00;         // 0x00
    OrderGimmickNode* head; // 0x04
};

// ── CVS_THREAD_ORDER extra state ─────────────────────────────────────────
// The retail object is one word larger than the header's CVS_THREAD base: a
// slot/owner field lives at +0x20.  The accessor below keeps that offset.
struct CVS_THREAD_ORDER_Data {
    CVS_THREAD base;          // 0x00-0x20 (vtable at 0x1C)
    CVoiceHandle* field_0x20; // 0x20 slot/owner handle
};

// Raw word view over the CVS_THREAD head (the installed ptmf is
// stored/restored by the retail code as three plain u32 words).
struct OrderThreadWords {
    u32 words[3]; // 0x00-0x08
};

// Raw layout of the CVS_THREAD_ORDER object exposing the implicit vtable
// pointer at 0x1C and the owner slot at 0x20.
struct CVS_THREAD_ORDER_raw {
    u32 state0;               // 0x00
    u32 state1;               // 0x04
    u32 state2;               // 0x08
    u32 field_0x0C;           // 0x0C
    u32 field_0x10;           // 0x10
    u32 field_0x14;           // 0x14
    u32 field_0x18;           // 0x18
    u32* vtable;              // 0x1C
    CVoiceHandle* field_0x20; // 0x20 owner/slot handle
};

// Battle-manager view for the two offsets this unit touches: the battle
// object-list sentinel at +0x48 and the active-battle s16 flag at +0x20C8
// (matching the retail lwz/lha loads).
struct BattleMgrView {
    u8 _00[0x48];
    u8* listHead;        // +0x48 battle object list sentinel
    u8 _4C[0x20C8 - 0x4C];
    s16 field_0x20C8;      // +0x20C8 active-battle flag
};

// Handle view produced by de-biasing a gimmick object pointer (-0x3E9C).
struct GimmickHandleData {
    u8 _00[0x4];
    u8* field_4;         // +0x04
    u8 _08[0x3F28 - 0x08];
    u16 field_3F28;        // +0x3F28 voice-id selector
};

// View for the player-voice selector probe in func_802B8D4C: virtual at
// index 17 lands on vtable offset 0x4c.
class CVoiceSelView {
public:
    virtual void v000() = 0;
    virtual void v001() = 0;
    virtual void v002() = 0;
    virtual void v003() = 0;
    virtual void v004() = 0;
    virtual void v005() = 0;
    virtual void v006() = 0;
    virtual void v007() = 0;
    virtual void v008() = 0;
    virtual void v009() = 0;
    virtual void v010() = 0;
    virtual void v011() = 0;
    virtual void v012() = 0;
    virtual void v013() = 0;
    virtual void v014() = 0;
    virtual void v015() = 0;
    virtual void v016() = 0;
    virtual int mAt4C() = 0; // index 17 -> vtable 0x4c
};

// View for the gimmick object name probe in func_802B8D4C: virtual at
// index 10 lands on vtable offset 0x30 and returns a pointer whose first
// word is the value compared by func_80174C98.
struct GimmickNameValue {
    u32 name; // +0x00 name word compared against the config key
};

class CGimmickNameView {
public:
    virtual void v000() = 0;
    virtual void v001() = 0;
    virtual void v002() = 0;
    virtual void v003() = 0;
    virtual void v004() = 0;
    virtual void v005() = 0;
    virtual void v006() = 0;
    virtual void v007() = 0;
    virtual void v008() = 0;
    virtual void v009() = 0;
    virtual GimmickNameValue* mAt30() = 0; // index 10 -> vtable 0x30
};

// Voice-config table entry used by func_802B8D4C: {u32 key, s16 idx, s16 id}.
struct OrderConfigEntry {
    s32 key;      // +0x00 key matched against handle->field_3F28
    s16 field_4;  // +0x04 index passed to func_802A7A54
    s16 field_6;  // +0x06 voice id passed to func_802A3D54
};

// ── Data imports (plain globals: MWCC does not mangle global data names) ──
extern u32 lbl_eu_8053B7E8[3];   // ORDER init-state triple
extern s32 lbl_eu_8053B7F4[];    // func_802B8B0C voice-ID table
extern u32 lbl_eu_8053B800[3];   // func_802B8B0C init-state triple
extern u32 lbl_eu_8053B80C[3];   // func_802B8C00 init-state triple
extern u32 lbl_eu_8053B818[];    // CVS_THREAD_ORDER vtable
extern OrderConfigEntry lbl_eu_8053B840[]; // func_802B8D4C config table

// C-ABI function imports (retail symbols are unmangled C -> extern "C").
extern "C" {
    int                 func_802A3E88(CVS_THREAD* self);
    int                 func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int                 func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
    int                 func_802A7870(void* arr, int capacity, void* unused);
    void*               func_802A7A54(int arg);
    CVoiceHandle*       func_802A330C(int size, int align);
    void*               func_802A34E4(int size);
    void*               func_8016FE34(void* r3);
    OrderGimmickList*   func_800B6BC8();
    void                __ct__cf_CVS_THREAD();

    // Runtime rethrow (NMWException.h): declared noreturn so MWCC elides the
    // __end__catch epilogue of a catch-all handler ending in `bl __throw`.
    // Pointer args kept as u32: the runtime ABI passes raw addresses and this
    // avoids opaque-pointer types in the TU.
    __declspec(noreturn) void __throw(char* throwtype, u32 location,
                                      u32 dtor);
}

// C++-mangled retail helper findObjectById__Fi (actor id -> action source).
extern void* findObjectById(int id);

// Vtable view on the voice handle: with -RTTI a virtual declared at index N
// sits at vtable offset (N+2)*4, so mAt2BC (index 173) lands on 0x2bc. The
// slab virtuals are padding to reach that index; calling through the view
// makes MWCC emit the retail r12-chain (lwz r12,0(rN); lwz r12,0x2bc(r12)).
class CVoiceDevView {
public:
    virtual void v000() = 0;
    virtual void v001() = 0;
    virtual void v002() = 0;
    virtual void v003() = 0;
    virtual void v004() = 0;
    virtual void v005() = 0;
    virtual void v006() = 0;
    virtual void v007() = 0;
    virtual void v008() = 0;
    virtual void v009() = 0;
    virtual void v010() = 0;
    virtual void v011() = 0;
    virtual void v012() = 0;
    virtual void v013() = 0;
    virtual void v014() = 0;
    virtual void v015() = 0;
    virtual void v016() = 0;
    virtual void v017() = 0;
    virtual void v018() = 0;
    virtual void v019() = 0;
    virtual void v020() = 0;
    virtual void v021() = 0;
    virtual void v022() = 0;
    virtual void v023() = 0;
    virtual void v024() = 0;
    virtual void v025() = 0;
    virtual void v026() = 0;
    virtual void v027() = 0;
    virtual void v028() = 0;
    virtual void v029() = 0;
    virtual void v030() = 0;
    virtual void v031() = 0;
    virtual void v032() = 0;
    virtual void v033() = 0;
    virtual void v034() = 0;
    virtual void v035() = 0;
    virtual void v036() = 0;
    virtual void v037() = 0;
    virtual void v038() = 0;
    virtual void v039() = 0;
    virtual void v040() = 0;
    virtual void v041() = 0;
    virtual void v042() = 0;
    virtual void v043() = 0;
    virtual void v044() = 0;
    virtual void v045() = 0;
    virtual void v046() = 0;
    virtual void v047() = 0;
    virtual void v048() = 0;
    virtual void v049() = 0;
    virtual void v050() = 0;
    virtual void v051() = 0;
    virtual void v052() = 0;
    virtual void v053() = 0;
    virtual void v054() = 0;
    virtual void v055() = 0;
    virtual void v056() = 0;
    virtual void v057() = 0;
    virtual void v058() = 0;
    virtual void v059() = 0;
    virtual void v060() = 0;
    virtual void v061() = 0;
    virtual void v062() = 0;
    virtual void v063() = 0;
    virtual void v064() = 0;
    virtual void v065() = 0;
    virtual void v066() = 0;
    virtual void v067() = 0;
    virtual void v068() = 0;
    virtual void v069() = 0;
    virtual void v070() = 0;
    virtual void v071() = 0;
    virtual void v072() = 0;
    virtual void v073() = 0;
    virtual void v074() = 0;
    virtual void v075() = 0;
    virtual void v076() = 0;
    virtual void v077() = 0;
    virtual void v078() = 0;
    virtual void v079() = 0;
    virtual void v080() = 0;
    virtual void v081() = 0;
    virtual void v082() = 0;
    virtual void v083() = 0;
    virtual void v084() = 0;
    virtual void v085() = 0;
    virtual void v086() = 0;
    virtual void v087() = 0;
    virtual void v088() = 0;
    virtual void v089() = 0;
    virtual void v090() = 0;
    virtual void v091() = 0;
    virtual void v092() = 0;
    virtual void v093() = 0;
    virtual void v094() = 0;
    virtual void v095() = 0;
    virtual void v096() = 0;
    virtual void v097() = 0;
    virtual void v098() = 0;
    virtual void v099() = 0;
    virtual void v100() = 0;
    virtual void v101() = 0;
    virtual void v102() = 0;
    virtual void v103() = 0;
    virtual void v104() = 0;
    virtual void v105() = 0;
    virtual void v106() = 0;
    virtual void v107() = 0;
    virtual void v108() = 0;
    virtual void v109() = 0;
    virtual void v110() = 0;
    virtual void v111() = 0;
    virtual void v112() = 0;
    virtual void v113() = 0;
    virtual void v114() = 0;
    virtual void v115() = 0;
    virtual void v116() = 0;
    virtual void v117() = 0;
    virtual void v118() = 0;
    virtual void v119() = 0;
    virtual void v120() = 0;
    virtual void v121() = 0;
    virtual void v122() = 0;
    virtual void v123() = 0;
    virtual void v124() = 0;
    virtual void v125() = 0;
    virtual void v126() = 0;
    virtual void v127() = 0;
    virtual void v128() = 0;
    virtual void v129() = 0;
    virtual void v130() = 0;
    virtual void v131() = 0;
    virtual void v132() = 0;
    virtual void v133() = 0;
    virtual void v134() = 0;
    virtual void v135() = 0;
    virtual void v136() = 0;
    virtual void v137() = 0;
    virtual void v138() = 0;
    virtual void v139() = 0;
    virtual void v140() = 0;
    virtual void v141() = 0;
    virtual void v142() = 0;
    virtual void v143() = 0;
    virtual void v144() = 0;
    virtual void v145() = 0;
    virtual void v146() = 0;
    virtual void v147() = 0;
    virtual void v148() = 0;
    virtual void v149() = 0;
    virtual void v150() = 0;
    virtual void v151() = 0;
    virtual void v152() = 0;
    virtual void v153() = 0;
    virtual void v154() = 0;
    virtual void v155() = 0;
    virtual void v156() = 0;
    virtual void v157() = 0;
    virtual void v158() = 0;
    virtual void v159() = 0;
    virtual void v160() = 0;
    virtual void v161() = 0;
    virtual void v162() = 0;
    virtual void v163() = 0;
    virtual void v164() = 0;
    virtual void v165() = 0;
    virtual void v166() = 0;
    virtual void v167() = 0;
    virtual void v168() = 0;
    virtual void v169() = 0;
    virtual void v170() = 0;
    virtual void v171() = 0;
    virtual void v172() = 0;
    virtual int mAt2BC() = 0; // index 173 -> vtable 0x2bc
};
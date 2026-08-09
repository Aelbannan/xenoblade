#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_ORDER.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"
#include "monolib/math/Random.hpp"

// cf::CfGameManager / cf::CBattleManager have no shared standalone header;
// declare just the statics this TU uses.
namespace cf {
class CfGameManager {
public:
    static void* getPlayer(int idx);
};
class CBattleManager {
public:
    static CBattleManager* getInstance();
};
}

// Local minimal layout for the gimmick list consumed by func_800B6BC8.
struct OrderGimmickNode {
    OrderGimmickNode* next; // 0x00
    u8 _04[4];              // 0x04..0x07
    void* object;           // 0x08
};
struct OrderGimmickList {
    void* field_00;         // 0x00
    OrderGimmickNode* head; // 0x04
};

// ── CVS_THREAD_ORDER extra state ─────────────────────────────────────────
// The retail object is one word larger than the header's CVS_THREAD base: a
// slot/owner field lives at +0x20.  The accessor below keeps that offset.
struct CVS_THREAD_ORDER_Data {
    CVS_THREAD base;          // 0x00-0x20 (vtable at 0x1C)
    CVoiceHandle* field_0x20; // 0x20 slot/owner handle
};

// Battle-manager view for the two offsets this unit touches: the battle
// object-list sentinel at +0x48 and the active-battle s16 flag at +0x20C8
// (matching the retail lwz/lha loads).
struct BattleMgrView {
    u8 _00[0x48];
    void* listHead;        // +0x48 battle object list sentinel
    u8 _4C[0x20C8 - 0x4C];
    s16 field_0x20C8;      // +0x20C8 active-battle flag
};

// Handle view produced by de-biasing a gimmick object pointer (-0x3E9C).
struct GimmickHandleData {
    u8 _00[0x4];
    void* field_4;         // +0x04
    u8 _08[0x3F28 - 0x08];
    u16 field_3F28;        // +0x3F28 voice-id selector
};

// Voice-config table entry used by func_802B8D4C: {u32 key, s16 idx, s16 id}.
struct OrderConfigEntry {
    u32 key;      // +0x00 key matched against handle->field_3F28
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
    int                 func_80174C98(void* actor, u32* val, int flags);
    OrderGimmickList*   func_800B6BC8();
    void                __ct__cf_CVS_THREAD();
}

// C++-mangled retail helper func_800B708C__Fi (actor id -> action source).
extern void* func_800B708C(int id);
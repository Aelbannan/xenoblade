#pragma once

#include <types.h>
#include "kyoshin/plugin/ocBdat.hpp"

extern u8* lbl_eu_806640E0;    // bdat file pointer (.sbss/.sdata)
extern char lbl_eu_805018A8[]; // bdat column-name string block (.rodata)

namespace cf {
    struct CBattleStateSrcEntry;

    // 0x34-byte slot layout used by CBattleState_addStagingEntry's incoming
    // arg (r4) and by the 8-entry array at CBattleState+0x1388. Same struct
    // shape reused for both (see MWCC_CASES sect.CBattleState_UnkVirtualFunc6).
    struct CBattleStateEntry {
        u32 unk00; // 0x00
        u32 unk04; // 0x04
        u32 unk08; // 0x08
        u16 unk0C; // 0x0C - id; also bit index into CBattleState::unk15AC
        s32 unk10; // 0x10 - clamped value
        s16 unk14; // 0x14
        s16 unk16; // 0x16
        s16 unk18; // 0x18 - lower clamp bound (0 == no lower clamp)
        s16 unk1A; // 0x1A
        f32 unk1C; // 0x1C
        f32 unk20; // 0x20
        f32 unk24; // 0x24
        f32 unk28; // 0x28
        u16 unk2C; // 0x2C
        u16 unk2E; // 0x2E
        u32 unk30; // 0x30
    };

    // View of the 0x68-slot status-entry array at object+0x8 (stride 0x34),
    // used by findBattleStatusEntry / lookupBattleStatusEntry (retail keeps the entry offsets
    // as load/return displacements instead of materializing a base pointer).
    struct CBattleStateEntryArray {
        u8 pad8[0x8]; // +0x0..+0x7
        CBattleStateEntry entries[0x68]; // +0x8
    };

    // size: 0x15D4 (retail; CActorParam members start at 0x8+0x15D4 = 0x15DC).
    // The pointer formerly declared here as field_0x15D8 is actually
    // CActorParam::unk15E0 (object+0x15E0), NOT a CBattleState member.
    class CBattleState {
    public:
        virtual void* CBattleState_UnkVirtualFunc1();  //0x8 (returns the owner object)
        virtual void CBattleState_UnkVirtualFunc2();  //0xC
        virtual int CBattleState_UnkVirtualFunc3();  //0x10
        virtual void CBattleState_setBattleParam(int val);  //0x14 - takes int 0x35 in CtrlObjectParam (retail li r4,0x35)
        virtual void CBattleState_enterStatusEntry(CBattleStateEntry* entry);  //0x18: full enter-status pipeline (state machine + slot scan + copy)
        virtual void CBattleState_addStagingEntry(CBattleStateEntry* entry);  //0x1C: bitfield set + 8-slot staging match/clamp/fill
        virtual void CBattleState_clearStatusId(u32 id);  //0x20 (retail fake-Fv ABI passes id in r4)
        virtual void CBattleState_applyEventEntry(CBattleStateEntry* entry);  //0x24
        virtual void CBattleState_reapplyStatusEntry(int index);  //0x28
        virtual void CBattleState_removeKeyedEntries(CBattleStateEntry* arg); //0x2C: remove all slots sharing the unk2E key
        virtual void CBattleState_clearEntriesByMask(u32 mask); //0x30: purge main-array entries whose unk30 matches the mask
        virtual void CBattleState_clearStagingEntry(u32 id); //0x34: clear the one staging slot whose id matches (retail Fv, id in r4)
        virtual void* CBattleState_getStatusSlot(int index); //0x38: base status array slot by int index (primitive behind 0x54)
        virtual void CBattleState_fetchStatusSlot(); //0x3C: base status array slot by ulong index in r4 (retail Fv; primitive behind 0x58)
        virtual void CBattleState_getEventSlot(); //0x40: event-window slot (entries[32..]) by ulong index in r4 (retail Fv; primitive behind 0x5C)
        virtual void CBattleState_readStatusSlot(); //0x44: upper status-window slot (entries[64..]) by int index in r4 (retail Fv; primitive behind 0x60)
        virtual void CBattleState_UnkVirtualFunc17(cf::CBattleStateEntry* entry); //0x48
virtual void* CBattleState_getLinkedActorId(CBattleStateEntry* entry = 0); //0x4C (retail Fv at CBattleManager sites; this TU passes the slot in r4)
        virtual void CBattleState_UnkVirtualFunc19(); //0x50
        virtual void* CBattleState_getEntryByIndex(int index); //0x54: indexed entry via slot 0x38, r3 passthrough
        virtual cf::CBattleStateEntry* CBattleState_fetchStatusEntry(int index); //0x58
        virtual cf::CBattleStateEntry* CBattleState_getEventEntry(int index); //0x5C
        virtual cf::CBattleStateEntry* CBattleState_getStatusEntry(int index); //0x60
        virtual void CBattleState_copyStatusEntries(const CBattleState* src); //0x64: copy the whole 0x1520 status array from src (retail Fv)
        virtual void* CBattleState_getEntryArray(); //0x68: base of the 0x68 status-entry array at +0x8 (retail Fv)
        virtual void CBattleState_applyArtsTable(const CBattleStateSrcEntry* src); //0x6C: stage 8 arts-table records + raw copy to unk152C
        virtual void* CBattleState_getStatusBlock(); //0x70
        virtual void* CBattleState_getStagingRecord(unsigned long index); //0x74: unk152C staging record by index (retail Fv, index in r4)
        virtual void CBattleState_clearStagingEntries(); //0x78: clear the 8 staging slots + dead-id status bits
        virtual void CBattleState_addStatusBits4(u32 flags); //0x7C: OR flags into the +0x4 status halfword (retail Fv)
        virtual int CBattleState_getEventMask(u32 id); //0x80: event-id to bit mask (0 = unknown)
        virtual void CBattleState_addStatusBits6(u32 flags); //0x84: OR flags into the +0x6 status halfword (retail Fv)
        virtual int CBattleState_getStatusMask(u32 id); //0x88: status-id to bit mask on unk6 (twin of 0x80 getEventMask)

        // Remaining same-slot forwarders (non-virtual, inline): the virtuals
        // keep legacy Unk names because slots 0x08/0x48 belong to the shared
        // base line overridden in CfObjectActor and CActorParam (out of scope).
        // MWCC inlines each to identical virtual-dispatch code at call sites.
        void* CBattleState_getOwner() { return CBattleState_UnkVirtualFunc1(); } //0x08: owner object (base returns null; overrides return the container)
        void CBattleState_notifyEntryUpdated(CBattleStateEntry* entry) { CBattleState_UnkVirtualFunc17(entry); } //0x48: post-update notify (base is an empty stub)

        CBattleState();

        u16 unk4;
        u16 unk6;
        u8 unk8[0x1520];
        u8 unk1528[4];
        u8 unk152C[0x80];
        u8 unk15AC[0x15D4 - 0x15AC];
    };
}

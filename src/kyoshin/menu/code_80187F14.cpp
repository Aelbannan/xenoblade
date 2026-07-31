// Auto-scaffolded catalog TU for kyoshin/menu/code_80187F14
// Sound effect management system for menu UI

#include <string.h>
#include "kyoshin/harness_catalog.hpp"
#include "monolib/lib/CLibCri.hpp"

// ===========================================================================
// Global data
// ===========================================================================

extern "C" {
    extern s32 lbl_eu_80575798[9];     // bss 0x24 bytes - slot 0 name buffer area
    extern s32 lbl_eu_805757BC[9];     // bss 0x24 bytes - slot 1 name buffer area
    extern float lbl_eu_80662490;      // base volume for slot 0
    extern float lbl_eu_80662494;      // base volume for slot 1
    extern float lbl_eu_80662498;      // base volume for slots 2-4
    extern float lbl_eu_8066249C;      // accumulated time
    extern float lbl_eu_806624A0;      // max time threshold
    extern u16 lbl_eu_806642E0;        // flags
    extern float lbl_eu_806642E4;      // target time
    extern s32* lbl_eu_80663E60;       // pointer to 5-slot SoundSlot array
    extern float lbl_eu_80667A08;      // 0.0
    extern float lbl_eu_80667A0C;      // 1.0
    extern float lbl_eu_80667A10;      // some float
    extern float lbl_eu_80667A14;      // some float
    extern float lbl_eu_80667A18;      // some float
}

// Format strings from rodata at 0x80503AB0
extern const char lbl_eu_80503AB0_str_dot;    // "."
extern const char lbl_eu_80503AB0_str_slash;  // "/"
extern const char lbl_eu_80503AB0_str_adx;    // ".adx"
extern const char lbl_eu_80503AB0_fmt;        // "%s%s%s"
extern const char lbl_eu_80503AB0_path;       // "snd/adx/"
extern const char lbl_eu_80503AB0_loop;       // "loop"

// ===========================================================================
// Sound slot structure (0xB8 bytes per slot, 5 slots)
// ===========================================================================

struct SoundSlot {
    s32 handle;          // 0x00: CRIA handle, -1 = inactive
    char name[0x40];     // 0x04: sound name (e.g. "SE_SYS_CURSOR")
    s32 nameLen;          // 0x44: strlen of name
    float field_0x48;     // 0x48: usually 0.0f
    float x;              // 0x4C: pan/volume x
    float y;              // 0x50: pan/volume y
    u16 field_0x54;       // 0x54
    u16 field_0x56;       // 0x56
    u16 status;           // 0x58: 0=inactive, 2=playing, 4=paused?, 5=stopped?
    s32 backupHandle;     // 0x5C: saved handle for swap
    char backupName[0x40]; // 0x60: saved name for swap
    s32 backupNameLen;    // 0xA0: strlen of backup name
    float backupF1;       // 0xA4: saved field_0x48
    float backupF2;       // 0xA8: saved x
    float backupF3;       // 0xAC: saved y
    u16 backupU16_1;      // 0xB0: saved field_0x54
    u16 backupU16_2;      // 0xB2: saved field_0x56
    u16 backupU16_3;      // 0xB4: saved status
};

// ===========================================================================
// Inline helpers
// ===========================================================================

static inline SoundSlot* getSlotPtr(s32 index) {
    s32* base = lbl_eu_80663E60;
    if (base == nullptr) return nullptr;
    return reinterpret_cast<SoundSlot*>(reinterpret_cast<u8*>(base) + index * 0xB8);
}

static inline bool handleIsValid(s32 handle) {
    return handle != -1;
}

static inline bool slotIsActive(SoundSlot* slot) {
    return slot != nullptr && handleIsValid(slot->handle);
}

// ===========================================================================
// Forward declarations
// ===========================================================================

void func_801882AC(SoundSlot* slot, u32 type);
void func_80188488(SoundSlot* slot, u32 type, float f1, float f2, float f3);
s32 func_801887C8(u32 wantId, u32 startIdx, u32 endIdx);
s32 func_80188B80(s32 index, const char* name, float f1, float f2, s32 flag);
void func_801889D0();
void func_8018986C(s32 index, float f1);
void func_80189A04(s32 index);

// ===========================================================================
// func_80189424 - Stop slot 1 (index 1)
// ===========================================================================

void func_80189424() {
    SoundSlot* slot = getSlotPtr(1);
    if (slot != nullptr) {
        func_801882AC(slot, 2);
    }
}

// ===========================================================================
// func_8018892C - Check if a slot is active (used/in use)
// ===========================================================================

s32 func_8018892C(s32 index) {
    SoundSlot* slot = getSlotPtr(index);
    if (slot == nullptr) return 0;
    return handleIsValid(slot->handle) ? 1 : 0;
}

// ===========================================================================
// func_80189BF4 - Check if a slot is active (returns 1 if inactive/null)
// ===========================================================================

s32 func_80189BF4(s32 index) {
    SoundSlot* slot = getSlotPtr(index);
    if (slot == nullptr) return 1;
    return (slot->handle == -1) ? 1 : 0;
}

// ===========================================================================
// func_80189C34 - Return pointer to second name buffer
// ===========================================================================

s32* func_80189C34() {
    return lbl_eu_805757BC;
}

// ===========================================================================
// func_80189C40 - Get a handle for a slot and pass to CLibCri
// ===========================================================================

s32 func_80189C40(s32 index) {
    SoundSlot* slot = getSlotPtr(index);
    if (slot == nullptr) return 0;
    return CLibCri::func_80459A9C(slot->handle);
}

// ===========================================================================
// func_80189C70 - Set target time to a specific value
// ===========================================================================

void func_80189C70() {
    lbl_eu_806642E4 = lbl_eu_80667A14;
}

// ===========================================================================
// func_80189C7C - Set target time to a different value
// ===========================================================================

void func_80189C7C() {
    lbl_eu_806642E4 = lbl_eu_80667A18;
}

// ===========================================================================
// func_801895EC - Get base volume for slot 0
// ===========================================================================

float func_801895EC() { return lbl_eu_80662490; }

// ===========================================================================
// func_801896A0 - Get base volume for slots 2-4
// ===========================================================================

float func_801896A0() { return lbl_eu_80662498; }

// ===========================================================================
// func_801896A8 - Set volume parameters
// ===========================================================================

void func_801896A8(s32 index, float f1, float f2) {
    lbl_eu_80662494 = f1;
    if (index != 0) return;
    
    for (s32 i = 0; i <= 1; i++) {
        float vol;
        if (i >= 0 && i <= 1) {
            vol = lbl_eu_80662490 * lbl_eu_80662494;
        } else {
            vol = lbl_eu_80662498;
        }
        SoundSlot* slot = getSlotPtr(i);
        if (slot != nullptr && handleIsValid(slot->handle)) {
            func_80188488(slot, 0, 0.0f, vol, f2);
        }
    }
}

// ===========================================================================
// func_80189510 - Set volume for slots 0-1
// ===========================================================================

void func_80189510(float f1) {
    lbl_eu_80662490 = f1;
    for (s32 i = 0; i <= 1; i++) {
        float vol;
        if (i >= 0 && i <= 1) {
            vol = lbl_eu_80662490 * lbl_eu_80662494;
        } else {
            vol = lbl_eu_80662498;
        }
        SoundSlot* slot = getSlotPtr(i);
        if (slot != nullptr && handleIsValid(slot->handle)) {
            func_80188488(slot, 0, 0.0f, vol, slot->x);
        }
    }
}

// ===========================================================================
// func_801895F4 - Set volume for slots 2-4
// ===========================================================================

void func_801895F4(float f1) {
    lbl_eu_80662498 = f1;
    for (s32 i = 2; i <= 4; i++) {
        float vol;
        if (i >= 0 && i <= 1) {
            vol = lbl_eu_80662490 * lbl_eu_80662494;
        } else {
            vol = lbl_eu_80662498;
        }
        SoundSlot* slot = getSlotPtr(i);
        if (slot != nullptr) {
            func_80188488(slot, 0, 1.0f, vol, 0.0f);
        }
    }
}

// ===========================================================================
// func_80188488 - Play/update sound on a slot
// ===========================================================================

void func_80188488(SoundSlot* slot, u32 type, float f1, float f2, float f3) {
    if (!slotIsActive(slot)) return;
    
    float f_prod = f1 * f2;
    CLibCri* cri = CLibCri::getInstance();
    
    if (slot->status == 4) return;
    
    if (f3 <= 0.0f || slot->status == 5) {
        CLibCri::func_80459A90(slot->handle);
        slot->x = f1;
        slot->y = f2;
        return;
    }
    
    CLibCri::func_80459A88(slot->handle, f_prod, f3, type);
    slot->x = f1;
    slot->y = f2;
    
    if (type == 2) {
        slot->handle = -1;
        slot->field_0x54 = 0;
        slot->field_0x56 = 0;
        slot->status = 0;
        slot->field_0x48 = 0.0f;
    }
}

// ===========================================================================
// func_801882AC - Stop/clear a sound slot
// ===========================================================================

void func_801882AC(SoundSlot* slot, u32 type) {
    CLibCri* cri = CLibCri::getInstance();
    
    // If the volume is 0, clear the backup slot
    if (lbl_eu_80667A08 == 0.0f) {
        // Actually the comparison is fcmpu against f31 (the passed float parameter)
        // Wait, let me re-read the assembly...
    }
    
    // Actually this function takes a float parameter (f1 = f31)
    // Let me re-read the assembly
}

// Let me re-read the assembly more carefully before writing the complex functions
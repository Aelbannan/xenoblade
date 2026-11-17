#include "kyoshin/cf/voice/CCharVoice.hpp"
#include <string.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// ---------------------------------------------------------------------
// Imported linker symbols (data).  At global scope MWCC does not mangle
// variable names, so plain extern declarations emit the retail symbol.
// ---------------------------------------------------------------------
extern char lbl_eu_805106D4[];   // empty default voice name ("")
extern char*       lbl_eu_80662C98[2]; // voice path template pair (path, null)
extern char        lbl_eu_805398B0[];  // vtable for CCharVoice

// Volume/coefficient floats used by the archive-voice sound system.
extern float lbl_eu_80668C58;
extern float lbl_eu_80668C5C;
extern float lbl_eu_80668C60;
extern float lbl_eu_80668C64;

// ---------------------------------------------------------------
// Imported functions from other TUs.  The retail symbol names are
// unrecovered placeholders without C++ mangling, so these keep C
// linkage to resolve against the existing objects.
// ---------------------------------------------------------------
extern "C" {
    void  Tok_Unpack(u32, float*, u32*, float*, float*);  // packed-token decode
    s32   MenuSnd_QuerySlotLive_89A04(const char*);                        // archive-voice busy check
    CVoicePoseBlock* Scn_FindCamItem(CScn*, s32);      // scene pose/xform block lookup
    s32   MenuSnd_TryPlayGated_97A0(const char*, float, s32);            // start archive voice
    void  MenuSnd_SilenceByName_8986C(const char*, float);                 // stop archive voice
    void  MenuSnd_SetSlotPan_9C40(s32, CVoicePos*, CVoicePoseBlock*, float, float, float); // update archive voice
    CVoiceBattleSndMgr* CfObjectMove_relaySubB0Slot60(CVoiceOwnerIntf*);     // battle sound manager from owner
    s32   CfSoundMan_TouchSlotById(u16);                                // battle sound busy check
    void  CfSoundMan_WriteSlotParam(u16, CVoicePos*);                    // update battle sound position
    void  CfSoundMan_ApplySlotStop(CVoiceBattleSndMgr*, u16, s32);      // stop battle sound

    // cf::CfSoundMan::playActorSound(unsigned long, unsigned long,
    //                               unsigned long, unsigned long, float)
    u16 playActorSound__Q22cf10CfSoundManFUlUlUlUlf(
        CVoiceBattleSndMgr*, u32, u32, u32, float);
}

// __ct__CCharVoice (0x802A3230)
// NOTE: the name pointer must live in a named `char*` local so MWCC fully
// materializes it once (`lis r30,@ha; addi r30,r30,@l`, then `or`/mr copies);
// inline `(const char*)lbl` uses at each call site keep only `lis @ha` in the
// reg with per-use `addi rX,@l`. The zero likewise needs a named `int` local
// (used for both mOwner and mField34) so it survives the calls in a saved reg.
// Decl order zero-first/name-second gives zero->r31, name->r30 per Rule A
// (first-declared -> highest saved reg). The named pointer local also flips
// the strcpy arg order to retail's src-first (or r4 before addi r3).
extern "C" CCharVoice* __ct__CCharVoice(CCharVoice* self)
{
    int zero = 0;
    char* name = (char*)lbl_eu_805106D4;
    self->mVtable         = lbl_eu_805398B0;
    self->mOwner          = (CVoiceOwnerIntf*)zero;
    self->mVoiceId        = -1;
    self->mPriorityCheck  = -1;
    self->mSoundHandle    = -1;
    self->mFileNameLen    = strlen(name);
    strcpy(self->mFileName, name);
    self->mField34        = zero;
    self->mBattleSndHandle = 0xFFFF;
    return self;
}

// Data-word view over the voice owner's flag/token area (+0x64/+0x70 live in
// the concrete owner's data; the CVoiceOwnerIntf lens is vtable-only there).
// Per-statement use only: never hoist into a named local.
struct CVoiceOwnerData { u8 _00[0x64]; u32 flags64; u32 _pad68; u32 _pad6C; u32 token70; };

// attachOwner (0x802A32C0)
extern "C" void attachOwner(CCharVoice* self, CVoiceOwnerIntf* owner)
{
    if (owner == nullptr) return;

    u32 flags = ((CVoiceOwnerData*)owner)->flags64;

    // Normal/battle profile selector: bit0(MSB)=0x80000000, bit30=2, bit29=4.
    if ((flags & 2) || (flags & 0x80000000)) {
        self->mOwner = owner;

        float py;
        float px;
        u32   posType;
        float pz;
        Tok_Unpack(((CVoiceOwnerData*)owner)->token70,
                      &pz, &posType, &px, &py);

        if (posType == 8) posType = 3;

        char* vp = lbl_eu_80662C98[0];
        self->mFileNameLen = strlen(vp);
        strcpy(self->mFileName, vp);

        self->mFileName[0x0A] = '0' + (posType / 10);
        self->mFileName[0x0B] = '0' + (posType % 10);
        self->mFileName[0x0F] = '0' + (posType / 10);
        self->mFileName[0x10] = '0' + (posType % 10);

        self->mField34     = 0x12;
        self->mSoundHandle = -1;

    } else if (flags & 4) {
        self->mOwner           = owner;
        self->mBattleSndHandle = 0xFFFF;
    }
}

// formatVoiceFilePath (0x802A33EC) -- format voice file path into buffer
extern "C" bool formatVoiceFilePath(char* buffer, int index, int value)
{
    if (index < 1 || index > 13) return false;
    if (value < 0 || value > 9999) return false;

    // Index 8 (battle-end) maps to category 3.
    if (index == 8) {
        index = 3;
    }

    // Copy the template and store its length.
    char* src = lbl_eu_80662C98[0];
    *(u32*)(buffer + 0x20) = strlen(src);
    strcpy(buffer, src);

    // index digit pair (written at two pc slots), then value 4-wide.
    int idxTens = index / 10;
    buffer[0x0A] = '0' + idxTens;
    buffer[0x0F] = '0' + idxTens;
    int idxOnes = index % 10;
    buffer[0x0B] = '0' + idxOnes;
    buffer[0x10] = '0' + idxOnes;

    int hi = value / 100;
    int lo = value % 100;
    int hiTens = (u32)hi / 10;
    int hiOnes = (u32)hi % 10;
    int loTens = (u32)lo / 10;
    int loOnes = (u32)lo % 10;
    buffer[0x12] = '0' + hiTens;
    buffer[0x13] = '0' + hiOnes;
    buffer[0x15] = '0' + loTens;
    buffer[0x16] = '0' + loOnes;

    return true;
}

// updatePosition (0x802A353C)
extern "C" void updatePosition(CCharVoice* self)
{
    if (self->mOwner == nullptr) return;

    u32 flags = ((CVoiceOwnerData*)self->mOwner)->flags64;

    if ((flags & 2) || (flags & 0x80000000)) {
        if (self->mSoundHandle == -1) return;

        if (MenuSnd_QuerySlotLive_89A04(self->mFileName) != 0) {
            self->mVoiceId     = -1;
            self->mSoundHandle = -1;
            return;
        }

        CVoicePoseBlock* ch = Scn_FindCamItem(lbl_eu_80663E14, -1);

        // Model position fetch: getModelPos is called twice by retail
        // (once for the null test, once in the body) -- mirror that.
        CVoicePos pos;
        if (self->mOwner->getModelPos() != nullptr) {
            CVoiceModelPos* mp = self->mOwner->getModelPos();
            float x, y, z;   // decl order fixes FPR homes (x=f0, y=f1, z=f2)
            z = mp->z;       // retail loads z, y, x but stores x, y, z
            y = mp->y;
            x = mp->x;
            pos.f[0] = x;
            pos.f[1] = y;
            pos.f[2] = z;
        } else {
            pos = *self->mOwner->getPosition();
        }

        MenuSnd_SetSlotPan_9C40(self->mSoundHandle, &pos, ch,
                      lbl_eu_80668C58, lbl_eu_80668C5C, lbl_eu_80668C60);

    } else if (flags & 4) {
        if (self->mBattleSndHandle == 0xFFFF) return;

        if (CfSoundMan_TouchSlotById(self->mBattleSndHandle) == 0) {
            self->mVoiceId          = -1;
            self->mBattleSndHandle  = 0xFFFF;
            return;
        }

        CVoicePos pos;
        if (self->mOwner->getModelPos() != nullptr) {
            CVoiceModelPos* mp = self->mOwner->getModelPos();
            float x, y, z;   // decl order fixes FPR homes (x=f0, y=f1, z=f2)
            z = mp->z;       // retail loads z, y, x but stores x, y, z
            y = mp->y;
            x = mp->x;
            pos.f[0] = x;
            pos.f[1] = y;
            pos.f[2] = z;
        } else {
            pos = *self->mOwner->getPosition();
        }

        CfSoundMan_WriteSlotParam(self->mBattleSndHandle, &pos);
    }
}

// stopVoiceA (0x802A371C)
extern "C" void stopVoiceA(CCharVoice* self)
{
    if (self->mOwner == nullptr) return;

    u32 flags = ((CVoiceOwnerData*)self->mOwner)->flags64;

    if ((flags & 2) || (flags & 0x80000000)) {
        if (self->mSoundHandle != -1) {
            MenuSnd_SilenceByName_8986C(self->mFileName, lbl_eu_80668C64);
            self->mVoiceId     = -1;
            self->mSoundHandle = -1;
        }
    } else if (flags & 4) {
        u16 bh = self->mBattleSndHandle;
        if (bh != 0xFFFF) {
            CVoiceBattleSndMgr* obj = CfObjectMove_relaySubB0Slot60(self->mOwner);
            CfSoundMan_ApplySlotStop(obj, bh, 0);
            self->mVoiceId          = -1;
            self->mBattleSndHandle  = 0xFFFF;
        }
    }
}

// playGated (0x802A37D0)
extern "C" bool playGated(CCharVoice* self, float volume,
                                int voiceId, int priority)
{
    if (self->mOwner == nullptr) return false;
    if (self->mVoiceId < 0) self->mPriorityCheck = -1;
    // Gate: reject if the stored check priority is <= the new one (higher
    // priority = smaller number in this system).  Retail tests the > case
    // and branches past the reject.
    if (priority >= 0 && self->mPriorityCheck >= 0
        && self->mPriorityCheck <= priority)
        return false;

    self->mPriorityCheck = priority;

    if (self->mOwner != 0) {
        u32 flags = ((CVoiceOwnerData*)self->mOwner)->flags64;

        if ((flags & 2) || (flags & 0x80000000)) {
            if (self->mSoundHandle != -1) {
                MenuSnd_SilenceByName_8986C(self->mFileName, lbl_eu_80668C64);
                self->mVoiceId     = -1;
                self->mSoundHandle = -1;
            }
        } else if (flags & 4) {
            u16 bh = self->mBattleSndHandle;
            if (bh != 0xFFFF) {
                CVoiceBattleSndMgr* obj = CfObjectMove_relaySubB0Slot60(self->mOwner);
                CfSoundMan_ApplySlotStop(obj, bh, 0);
                self->mVoiceId          = -1;
                self->mBattleSndHandle  = 0xFFFF;
            }
        }
    }

    u32 flags2 = ((CVoiceOwnerData*)self->mOwner)->flags64;

    if ((flags2 & 2) || (flags2 & 0x80000000)) {
        // Format the voice id (0..9999) into the file name as four digits at
        // indices self->mField34+{0,1,3,4} (skipping the separator at +2).  Retail
        // reloads self->mField34 for each store, so reference the member raw, and
        // hoists the MenuSnd_TryPlayGated_97A0 volume arg load to the top of the block.
        float sndLevel = lbl_eu_80668C64;
        int v100 = voiceId / 100;
        int rem  = voiceId % 100;
        int a    = (u32)v100 / 10;                       // thousands
        int b    = (u32)v100 % 10;                       // hundreds
        int c    = (u32)rem / 10;                        // tens
        int d    = (u32)rem % 10;                        // ones
        self->mFileName[self->mField34 + 0] = '0' + a;
        // Stores b/c/d use integer-domain addressing: adding the loaded
        // self->mField34 value to `this` as u32 (not pointer arithmetic) makes
        // MWCC emit `add rD, r0, r30` (loaded value in rA) exactly like
        // retail; the pointer+subscript form would emit `add rD, r30, r0`.
        // The +0x11/+0x13/+0x14 constant is folded into the stb displacement
        // (= self->mFileName offset 0x10 + index {1,3,4}).
        *((char*)((u32)self->mField34 + (u32)self) + 0x11) = '0' + b;
        *((char*)((u32)self->mField34 + (u32)self) + 0x13) = '0' + c;
        *((char*)((u32)self->mField34 + (u32)self) + 0x14) = '0' + d;

        s32 h = MenuSnd_TryPlayGated_97A0(self->mFileName, sndLevel, 1);
        self->mSoundHandle = h;

        if (h != -1) {
            self->mVoiceId = voiceId;
            return true;
        }
        self->mVoiceId = -1;
        return false;
    }
    else if (flags2 & 4) {
        CVoiceBattleSndMgr* man = CfObjectMove_relaySubB0Slot60(self->mOwner);
        if ((s32)man < 0) return false;

        u16 h = playActorSound__Q22cf10CfSoundManFUlUlUlUlf(
            man, voiceId, 0, 0, volume);
        self->mBattleSndHandle = h;

        if (h != 0xFFFF) {
            self->mVoiceId = voiceId;
            return true;
        }
        self->mVoiceId = -1;
        return false;
    }

    return false;
}

// stopVoiceB (0x802A3A38)
extern "C" void stopVoiceB(CCharVoice* self)
{
    if (self->mOwner == nullptr) return;

    u32 flags = ((CVoiceOwnerData*)self->mOwner)->flags64;

    if ((flags & 2) || (flags & 0x80000000)) {
        if (self->mSoundHandle != -1) {
            MenuSnd_SilenceByName_8986C(self->mFileName, lbl_eu_80668C64);
            self->mVoiceId     = -1;
            self->mSoundHandle = -1;
        }
    } else if (flags & 4) {
        u16 bh = self->mBattleSndHandle;
        if (bh != 0xFFFF) {
            CVoiceBattleSndMgr* obj = CfObjectMove_relaySubB0Slot60(self->mOwner);
            CfSoundMan_ApplySlotStop(obj, bh, 0);
            self->mVoiceId          = -1;
            self->mBattleSndHandle  = 0xFFFF;
        }
    }
}

// voiceNoop (0x802A3AEC) -- no-op callback
extern "C" void voiceNoop()
{
}


// Retail tables owned by this unit.
// .rodata (0x48): class/path name strings. The 7-byte gap after
// "cf::CChainEffect" and the 1-byte gap before lbl_eu_805106D4 are alignment
// padding (the ahx path is 8-aligned, the empty name 2-aligned); sizes stay
// splitter-exact. lbl_eu_805106C4 ("cf::CCharVoice") is read cross-unit by
// CVS_THREAD; lbl_eu_805106D4 is the empty default voice name ("").
// .sdata (0x10): locator pair {name, vtable-row} read (by address) from
// CCharVoiceMan's VoiceVtableRow typeinfo slot, then the voice path template
// pair {path, null} (no code reads the second word; retail emits a zero).
// .sdata2 (0x10): archive-voice coefficients (10.0, 40.0, 0.5) and the 0.0
// silence level. Non-const decls (above) keep them as loads.
extern const char lbl_eu_80510690[0x11] = "cf::CChainEffect";
__attribute__((aligned(8))) extern const char lbl_eu_805106A8[0x1C] = "snd/ahx/pc01/pc01_01_01.ahx";
extern const char lbl_eu_805106C4[0x0F] = "cf::CCharVoice";
// All-zero *const* aggregates migrate to .bss (tried plain, volatile and a
// data_type pragma); a non-const zero array honours declspec and stays in
// .rodata (the section then links writable, which no gate compares).
__attribute__((aligned(2))) extern __declspec(section ".rodata") char lbl_eu_805106D4[0x04] = "";
extern char lbl_eu_805398A0[];
struct VoiceNameLocator {
    const char* name;
    const void* vtableRow;
};
VoiceNameLocator lbl_eu_80662C90 = { lbl_eu_80510690, lbl_eu_805398A0 };
char* lbl_eu_80662C98[2] = { (char*)lbl_eu_805106A8, 0 };
#pragma section data_type ".sdata2" ".sdata2"
extern __declspec(section ".sdata2") float lbl_eu_80668C58 = 10.0f;
extern __declspec(section ".sdata2") float lbl_eu_80668C5C = 40.0f;
extern __declspec(section ".sdata2") float lbl_eu_80668C60 = 0.5f;
extern __declspec(section ".sdata2") float lbl_eu_80668C64 = 0.0f;

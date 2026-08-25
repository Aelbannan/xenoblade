#include "kyoshin/cf/voice/CCharVoice.hpp"
#include <string.h>

extern CVoiceName lbl_eu_805106D4;    // empty default name (""), rodata blob

extern "C" {
    extern char* lbl_eu_80662C98;       // pointer to default voice path string
    extern void* lbl_eu_80663E14;       // manager singleton for character lookup

    extern float lbl_eu_80668C58;
    extern float lbl_eu_80668C5C;
    extern float lbl_eu_80668C60;
    extern float lbl_eu_80668C64;

    void func_800AA318(u32, float*, u32*, float*, float*);
    s32  func_80189A04(const char*);
    void* func_80496264(void*, s32);
    s32  func_801897A0(const char*, float, s32);
    void func_8018986C(const char*, float);
    void func_80189C40(s32, void*, void*, float, float, float);
    void* func_800BF2CC(void*);
    s32  func_801BFAE4(u16);
    void func_801BFAE8(u16, void*);
    void func_801BFED0(void*, u16, s32);

    u16 func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
        void*, u32, u32, u32, float);
}

struct VoiceVec3 { float x, y, z; };

// __ct__CCharVoice (0x802A3230)
extern "C" {
    extern char lbl_eu_805398B0[];   // vtable for CCharVoice
}

// __ct__CCharVoice (0x802A3230)
// Hoisting the empty-name base pointer and the two init constants into locals
// makes MWCC keep them live in r30/r31/r0 across all the member stores,
// matching the retail allocation.
extern "C" CCharVoice* __ct__CCharVoice(CCharVoice* self)
{
    CVoiceName* emptyName = &lbl_eu_805106D4;
    void* zero = 0;
    s32   negOne = -1;

    self->mVtable         = lbl_eu_805398B0;
    self->mOwner          = zero;
    self->mVoiceId        = negOne + 1;
    self->mPriorityCheck  = negOne;
    self->mSoundHandle    = negOne;
    self->mFileNameLen    = strlen((char*)emptyName);
    strcpy(self->mFileName, (char*)emptyName);
    self->mField34        = 0;
    self->mBattleSndHandle = 0xFFFF;
    return self;
}

// func_802A0B8C (0x802A32C0)
void CCharVoice::func_802A0B8C(void* owner)
{
    if (owner == nullptr) return;

    u32 flags = *(u32*)((char*)owner + 0x64);

    // Normal/battle profile selector: bit0(MSB)=0x80000000, bit30=2, bit29=4.
    if ((flags & 2) || (flags & 0x80000000)) {
        mOwner = owner;

        float py;
        float px;
        u32   posType;
        float pz;
        func_800AA318(*(u32*)((char*)owner + 0x70),
                      &pz, &posType, &px, &py);

        if (posType == 8) posType = 3;

        char* vp = lbl_eu_80662C98;
        mFileNameLen = strlen(vp);
        strcpy(mFileName, vp);

        mFileName[0x0A] = '0' + (posType / 10);
        mFileName[0x0B] = '0' + (posType % 10);
        mFileName[0x0F] = '0' + (posType / 10);
        mFileName[0x10] = '0' + (posType % 10);

        mField34     = 0x12;
        mSoundHandle = -1;

    } else if (flags & 4) {
        mOwner           = owner;
        mBattleSndHandle = 0xFFFF;
    }
}

// func_802A0E08 (0x802A353C)
void CCharVoice::func_802A0E08()
{
    if (mOwner == nullptr) return;

    u32 flags = *(u32*)((char*)mOwner + 0x64);

    if ((flags & 2) || (flags & 0x80000000)) {
        if (mSoundHandle == -1) return;

        if (func_80189A04(mFileName) != 0) {
            mVoiceId     = -1;
            mSoundHandle = -1;
            return;
        }

        void* ch = func_80496264(lbl_eu_80663E14, -1);

        // Model position fetch: getModelPos is called twice by retail
        // (once for the null test, once in the body) -- mirror that.
        CVoicePos pos;
        if (((CVoiceOwnerIntf*)mOwner)->getModelPos() != nullptr) {
            CVoiceModelPos* mp = ((CVoiceOwnerIntf*)mOwner)->getModelPos();
            float x, y, z;   // decl order fixes FPR homes (x=f0, y=f1, z=f2)
            z = mp->z;       // retail loads z, y, x but stores x, y, z
            y = mp->y;
            x = mp->x;
            pos.f[0] = x;
            pos.f[1] = y;
            pos.f[2] = z;
        } else {
            pos = *((CVoiceOwnerIntf*)mOwner)->getPosition();
        }

        func_80189C40(mSoundHandle, &pos, ch,
                      lbl_eu_80668C58, lbl_eu_80668C5C, lbl_eu_80668C60);

    } else if (flags & 4) {
        if (mBattleSndHandle == 0xFFFF) return;

        if (func_801BFAE4(mBattleSndHandle) == 0) {
            mVoiceId          = -1;
            mBattleSndHandle  = 0xFFFF;
            return;
        }

        CVoicePos pos;
        if (((CVoiceOwnerIntf*)mOwner)->getModelPos() != nullptr) {
            CVoiceModelPos* mp = ((CVoiceOwnerIntf*)mOwner)->getModelPos();
            float x, y, z;   // decl order fixes FPR homes (x=f0, y=f1, z=f2)
            z = mp->z;       // retail loads z, y, x but stores x, y, z
            y = mp->y;
            x = mp->x;
            pos.f[0] = x;
            pos.f[1] = y;
            pos.f[2] = z;
        } else {
            pos = *((CVoiceOwnerIntf*)mOwner)->getPosition();
        }

        func_801BFAE8(mBattleSndHandle, &pos);
    }
}

// func_802A0FE8 (0x802A371C)
void CCharVoice::func_802A0FE8()
{
    if (mOwner == nullptr) return;

    u32 flags = *(u32*)((char*)mOwner + 0x64);

    if ((flags & 2) || (flags & 0x80000000)) {
        if (mSoundHandle != -1) {
            func_8018986C(mFileName, lbl_eu_80668C64);
            mVoiceId     = -1;
            mSoundHandle = -1;
        }
    } else if (flags & 4) {
        u16 bh = mBattleSndHandle;
        if (bh != 0xFFFF) {
            void* obj = func_800BF2CC(mOwner);
            func_801BFED0(obj, bh, 0);
            mVoiceId          = -1;
            mBattleSndHandle  = 0xFFFF;
        }
    }
}

// func_802A109C (0x802A37D0)
bool CCharVoice::func_802A109C(float volume,
                                int voiceId, int priority)
{
    if (mOwner == nullptr) return false;
    if (mVoiceId < 0) mPriorityCheck = -1;
    // Gate: reject if the stored check priority is <= the new one (higher
    // priority = smaller number in this system).  Retail tests the > case
    // and branches past the reject.
    if (priority >= 0 && mPriorityCheck >= 0
        && mPriorityCheck <= priority)
        return false;

    mPriorityCheck = priority;

    if (mOwner != 0) {
        u32 flags = *(u32*)((char*)mOwner + 0x64);

        if ((flags & 2) || (flags & 0x80000000)) {
            if (mSoundHandle != -1) {
                func_8018986C(mFileName, lbl_eu_80668C64);
                mVoiceId     = -1;
                mSoundHandle = -1;
            }
        } else if (flags & 4) {
            u16 bh = mBattleSndHandle;
            if (bh != 0xFFFF) {
                void* obj = func_800BF2CC(mOwner);
                func_801BFED0(obj, bh, 0);
                mVoiceId          = -1;
                mBattleSndHandle  = 0xFFFF;
            }
        }
    }

    u32 flags2 = *(u32*)((char*)mOwner + 0x64);

    if ((flags2 & 2) || (flags2 & 0x80000000)) {
        // Format the voice id (0..9999) into the file name as four digits at
        // indices mField34+{0,1,3,4} (skipping the separator at +2).  Retail
        // reloads mField34 for each store, so reference the member raw, and
        // hoists the func_801897A0 volume arg load to the top of the block.
        float sndLevel = lbl_eu_80668C64;
        int v100 = voiceId / 100;
        int rem  = voiceId % 100;
        int a    = (u32)v100 / 10;                       // thousands
        int b    = (u32)v100 % 10;                       // hundreds
        int c    = (u32)rem / 10;                        // tens
        int d    = (u32)rem % 10;                        // ones
        mFileName[mField34 + 0] = '0' + a;
        // Stores b/c/d use integer-domain addressing: adding the loaded
        // mField34 value to `this` as u32 (not pointer arithmetic) makes
        // MWCC emit `add rD, r0, r30` (loaded value in rA) exactly like
        // retail; the pointer+subscript form would emit `add rD, r30, r0`.
        // The +0x11/+0x13/+0x14 constant is folded into the stb displacement
        // (= mFileName offset 0x10 + index {1,3,4}).
        *((char*)((u32)mField34 + (u32)this) + 0x11) = '0' + b;
        *((char*)((u32)mField34 + (u32)this) + 0x13) = '0' + c;
        *((char*)((u32)mField34 + (u32)this) + 0x14) = '0' + d;

        s32 h = func_801897A0(mFileName, sndLevel, 1);
        mSoundHandle = h;

        if (h != -1) {
            mVoiceId = voiceId;
            return true;
        }
        mVoiceId = -1;
        return false;
    }
    else if (flags2 & 4) {
        void* man = func_800BF2CC(mOwner);
        if ((s32)man < 0) return false;

        u16 h = func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
            man, voiceId, 0, 0, volume);
        mBattleSndHandle = h;

        if (h != 0xFFFF) {
            mVoiceId = voiceId;
            return true;
        }
        mVoiceId = -1;
        return false;
    }

    return false;
}

// func_802A1304 (0x802A3A38)
void CCharVoice::func_802A1304()
{
    if (mOwner == nullptr) return;

    u32 flags = *(u32*)((char*)mOwner + 0x64);

    if ((flags & 2) || (flags & 0x80000000)) {
        if (mSoundHandle != -1) {
            func_8018986C(mFileName, lbl_eu_80668C64);
            mVoiceId     = -1;
            mSoundHandle = -1;
        }
    } else if (flags & 4) {
        u16 bh = mBattleSndHandle;
        if (bh != 0xFFFF) {
            void* obj = func_800BF2CC(mOwner);
            func_801BFED0(obj, bh, 0);
            mVoiceId          = -1;
            mBattleSndHandle  = 0xFFFF;
        }
    }
}

// func_802A13B8 (0x802A3AEC) -- no-op callback
extern "C" void func_802A13B8()
{
}

// func_802A0CB8 (0x802A33EC) -- format voice file path into buffer
extern "C" bool func_802A0CB8(char* buffer, int index, int value)
{
    if (index < 1 || index > 13) return false;
    if (value < 0 || value > 9999) return false;

    // Index 8 (battle-end) maps to category 3.
    if (index == 8) {
        index = 3;
    }

    // Copy the template and store its length.
    char* src = lbl_eu_80662C98;
    *(u32*)(buffer + 0x20) = strlen(src);
    strcpy(buffer, src);

    // index digit pair (written at two pc slots), then value 4-wide.
    int idxTens = index / 10;
    int idxOnes = index % 10;
    buffer[0x0A] = '0' + idxTens;
    buffer[0x0F] = '0' + idxTens;
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

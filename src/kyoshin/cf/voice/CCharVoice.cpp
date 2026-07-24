#include "kyoshin/cf/voice/CCharVoice.hpp"
#include <string.h>

extern "C" {
    extern char* lbl_eu_80662C98;       // pointer to default voice path string
    extern char  lbl_eu_805106D4[];     // empty default name ("")
    extern void* lbl_eu_80663E14;       // manager singleton for character lookup

    extern float lbl_eu_80668C58;
    extern float lbl_eu_80668C5C;
    extern float lbl_eu_80668C60;
    extern float lbl_eu_80668C64;

    void func_800AA318(u32, u32*, float*, float*, float*);
    s32  func_80189A04(const char*);
    void* func_80496264(void*, s32);
    s32  func_801897A0(const char*, float, s32);
    void func_8018986C(const char*, float);
    void func_80189C40(s32, void*, void*, float, float, float);
    void* func_800BF2CC(void*);
    u16  func_801BFAE4(u16);
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
extern "C" void __ct__CCharVoice(CCharVoice* self)
{
    self->mVtable        = lbl_eu_805398B0;
    self->mOwner         = nullptr;
    self->mVoiceId       = -1;
    self->mPriorityCheck = -1;
    self->mSoundHandle   = -1;

    char* defName = lbl_eu_805106D4;
    self->mFileNameLen = strlen(defName);
    strcpy(self->mFileName, defName);

    self->mField34         = 0;
    self->mBattleSndHandle = 0xFFFF;
}

// func_802A0B8C (0x802A32C0)
extern "C" void func_802A0B8C(CCharVoice* self, void* owner)
{
    if (owner == nullptr) return;

    u32 flags = *(u32*)((char*)owner + 0x64);

    if ((flags & 2) || (flags & 1)) {
        self->mOwner = owner;

        u32 posType;
        float px, py, pz;
        func_800AA318(*(u32*)((char*)owner + 0x70),
                      &posType, &px, &py, &pz);

        if (posType == 8) posType = 3;

        char* vp = lbl_eu_80662C98;
        self->mFileNameLen = strlen(vp);
        strcpy(self->mFileName, vp);

        self->mFileName[0x0A] = '0' + (posType / 10);
        self->mFileName[0x0B] = '0' + (posType % 10);
        self->mFileName[0x0F] = '0' + (posType / 10);
        self->mFileName[0x10] = '0' + (posType % 10);

        self->mSoundHandle = -1;
        self->mField34     = 0x12;

    } else if (flags & 4) {
        self->mOwner           = owner;
        self->mBattleSndHandle = 0xFFFF;
    }
}

// func_802A0E08 (0x802A353C)
extern "C" void func_802A0E08(CCharVoice* self)
{
    if (self->mOwner == nullptr) return;

    u32 flags = *(u32*)((char*)self->mOwner + 0x64);

    if ((flags & 2) || (flags & 1)) {
        if (self->mSoundHandle == -1) return;

        if (func_80189A04(self->mFileName) != 0) {
            self->mVoiceId     = -1;
            self->mSoundHandle = -1;
            return;
        }

        void* ch = func_80496264(lbl_eu_80663E14, -1);

        void** vtab = *(void***)self->mOwner;
        VoiceVec3 pos;
        typedef void* (*GetModelFn)(void*);

        void* model = ((GetModelFn)vtab[0x4A])(self->mOwner);
        if (model != nullptr) {
            float* f = (float*)model;
            pos.x = f[0x0C / 4];
            pos.y = f[0x1C / 4];
            pos.z = f[0x2C / 4];
        } else {
            void* alt = ((GetModelFn)vtab[0x2B])(self->mOwner);
            float* f  = (float*)alt;
            pos.x = f[0];
            pos.y = f[1];
            pos.z = f[2];
        }

        func_80189C40(self->mSoundHandle, &pos, ch,
                      lbl_eu_80668C58, lbl_eu_80668C5C, lbl_eu_80668C60);

    } else if (flags & 4) {
        if (self->mBattleSndHandle == 0xFFFF) return;

        if (func_801BFAE4(self->mBattleSndHandle) == 0) {
            self->mVoiceId          = -1;
            self->mBattleSndHandle  = 0xFFFF;
            return;
        }

        void** vtab = *(void***)self->mOwner;
        VoiceVec3 pos;
        typedef void* (*GetModelFn)(void*);

        void* model = ((GetModelFn)vtab[0x4A])(self->mOwner);
        if (model != nullptr) {
            float* f = (float*)model;
            pos.x = f[0x0C / 4];
            pos.y = f[0x1C / 4];
            pos.z = f[0x2C / 4];
        } else {
            void* alt = ((GetModelFn)vtab[0x2B])(self->mOwner);
            float* f  = (float*)alt;
            pos.x = f[0];
            pos.y = f[1];
            pos.z = f[2];
        }

        func_801BFAE8(self->mBattleSndHandle, &pos);
    }
}

// func_802A0FE8 (0x802A371C)
extern "C" void func_802A0FE8(CCharVoice* self)
{
    if (self->mOwner == nullptr) return;

    u32 flags = *(u32*)((char*)self->mOwner + 0x64);

    if ((flags & 2) || (flags & 1)) {
        if (self->mSoundHandle != -1) {
            func_8018986C(self->mFileName, lbl_eu_80668C64);
            self->mVoiceId     = -1;
            self->mSoundHandle = -1;
        }
    } else if (flags & 4) {
        if (self->mBattleSndHandle != 0xFFFF) {
            void* obj = func_800BF2CC(self->mOwner);
            func_801BFED0(obj, self->mBattleSndHandle, 0);
            self->mVoiceId          = -1;
            self->mBattleSndHandle  = 0xFFFF;
        }
    }
}

// func_802A109C (0x802A37D0)
extern "C" bool func_802A109C(CCharVoice* self, float volume,
                              int priority, int voiceId)
{
    if (self->mOwner == nullptr) return false;
    if (self->mVoiceId < 0) self->mPriorityCheck = -1;
    if (priority >= 0 && self->mPriorityCheck >= 0
        && self->mPriorityCheck > priority)
        return false;

    self->mPriorityCheck = priority;

    u32 flags = *(u32*)((char*)self->mOwner + 0x64);

    if ((flags & 2) || (flags & 1)) {
        if (self->mSoundHandle != -1) {
            func_8018986C(self->mFileName, lbl_eu_80668C64);
            self->mVoiceId     = -1;
            self->mSoundHandle = -1;
        }
    } else if (flags & 4) {
        if (self->mBattleSndHandle != 0xFFFF) {
            void* obj = func_800BF2CC(self->mOwner);
            func_801BFED0(obj, self->mBattleSndHandle, 0);
            self->mVoiceId          = -1;
            self->mBattleSndHandle  = 0xFFFF;
        }
    }

    flags = *(u32*)((char*)self->mOwner + 0x64);

    if ((flags & 2) || (flags & 1)) {
        // Format priority into the filename at positions relative to
        // (this + mField34).  mField34 is always 0x12 after initVoice,
        // so these writes go to mFileName[0x12..0x16].
        char* base = (char*)self + self->mField34;
        int v100 = priority / 100;
        int v10  = v100 / 10;
        int v1   = v100 % 10;
        int r100 = priority % 100;
        int r10  = r100 / 10;
        int r1   = r100 % 10;
        base[0x10] = '0' + v10;
        base[0x11] = '0' + v1;
        base[0x13] = '0' + r10;
        base[0x14] = '0' + r1;

        s32 handle = func_801897A0(self->mFileName, lbl_eu_80668C64, 1);
        self->mSoundHandle = handle;

        if (handle != -1) {
            self->mVoiceId = priority;
            return true;
        }
        self->mVoiceId = -1;
        return false;
    }

    if (flags & 4) {
        void* obj = func_800BF2CC(self->mOwner);
        if (obj == nullptr) return false;
        if (*(s32*)obj < 0) return false;

        u16 handle = func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
            self->mOwner, voiceId, 0, 0, volume);
        self->mBattleSndHandle = handle;

        if (handle != 0xFFFF) {
            self->mVoiceId = priority;
            return true;
        }
        self->mVoiceId = -1;
        return false;
    }

    return false;
}

// func_802A1304 (0x802A3A38)
extern "C" void func_802A1304(CCharVoice* self)
{
    if (self->mOwner == nullptr) return;

    u32 flags = *(u32*)((char*)self->mOwner + 0x64);

    if ((flags & 2) || (flags & 1)) {
        if (self->mSoundHandle != -1) {
            func_8018986C(self->mFileName, lbl_eu_80668C64);
            self->mVoiceId     = -1;
            self->mSoundHandle = -1;
        }
    } else if (flags & 4) {
        if (self->mBattleSndHandle != 0xFFFF) {
            void* obj = func_800BF2CC(self->mOwner);
            func_801BFED0(obj, self->mBattleSndHandle, 0);
            self->mVoiceId          = -1;
            self->mBattleSndHandle  = 0xFFFF;
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

    // Write index digits at the two "pcNN" slots.
    int idxTens = index / 10;
    int idxOnes = index % 10;
    buffer[0x0A] = '0' + idxTens;
    buffer[0x0B] = '0' + idxOnes;
    buffer[0x0F] = '0' + idxTens;
    buffer[0x10] = '0' + idxOnes;

    // Compute value/100 (hi) and value%100 (lo) using the same
    // sequence as the retail: /100 first, then split each into digits.
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

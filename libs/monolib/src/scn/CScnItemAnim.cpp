/**
 * CScnItemAnim - Scene animation item
 *
 * Manages a character animation resource (nw4r::g3d::ResFile) within
 * the scene graph. Provides access to the embedded ResFile handle,
 * animation name, and related metadata.
 */

#include "libs/monolib/src/scn/CScnItemAnim.hpp"
#include <cstring>
#include <nw4r/db/db_assert.h>

// ===========================================================================
// CScnItemAnim::~CScnItemAnim - removes the item from the owning host's pool
// (broadcast over the reslist) and frees the pooled slot when the delete flag
// is positive. MWCC emits the this!=0 guard, the vtable re-point and the
// flag>0 delete logic automatically for member dtors; the vptr is re-pointed
// manually (novtable) to the retail vtable label.
// ===========================================================================
CScnItemAnim::~CScnItemAnim() {
    *(void**)this = (void*)lbl_eu_8056EC90;
    func_8048CBC0(this->mParent->mPool, this);
}

// ===========================================================================
// func_8049E648 - find the index of the chr-anm entry whose embedded name
// matches `name`. Returns the index, or -1 when no entry matches. A null
// entry is a data assert (Panic, retail line 0x27).
// ===========================================================================
int func_8049E648(CScnItemAnim* self, const char* name) {
    int count = (int)self->mResFile.GetResAnmChrNumEntries();
    for (int i = 0; i < count; i++) {
        nw4r::g3d::ResAnmChr chr = self->mResFile.GetResAnmChr(i);
        if (chr.ptr() == 0) {
            nw4r::db::Panic(lbl_eu_8056E9D0, 0x27, lbl_eu_8056E9B4,
                            lbl_eu_8056E9A8, lbl_eu_80663A88);
        }
        const char* chrName = chr.ofs_to_ptr<char>(chr.ref().name);
        if (std::strcmp(chrName, name) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Get a character animation resource by index from the embedded ResFile.
 *
 * Computes `&this->mResFile` (offset 0x0C) and delegates to
 * nw4r::g3d::ResFile::GetResAnmChr(int).  Implemented as a tail
 * call in the original binary.
 */
extern "C" nw4r::g3d::ResAnmChr func_8049E708(CScnItemAnim* self, int index) {
    return self->mResFile.GetResAnmChr(index);
}

// ===========================================================================
// __ct__8049E710 - scene animation item factory (retail placeholder symbol).
// Allocates a CScnItemAnim (kind 2) from the host's item pool, binds the
// ResFile, seeds the item with the name of its first chr-anm entry, and
// registers the item back into the pool. Returns the item, or null when the
// file has no chr-anm data / the pool is full.
//
// The `resData` copy keeps the raw ResFileData pointer live in a saved
// register across the item-field stores; without it MWCC rematerialises the
// pointer from the resFile stack slot (extra lwz before each store).
//
// The try/catch is the retail exception frame: it drives the frame-pointer
// prologue and the CATCHBLOCK extab around the item-name init (the frame
// save `stw r1, 0x2c(r31)` lands after the vptr store only when the try
// starts at the mResFile store); the handler rethrows via the runtime
// __throw(0, 0, 0) (retail `li r3,0; li r4,0; li r5,0; bl __throw`).
// ===========================================================================
CScnItemAnim* __ct__8049E710(CScnItemAnimHost* self,
                             nw4r::g3d::ResFileData* data, u32 param) {
    nw4r::g3d::ResFileData* resData = data;
    bool aligned = ((u32)resData & 0x1F) == 0;
    nw4r::g3d::ResFile resFile(resData);
    if (!aligned) {
        nw4r::db::Panic(lbl_eu_8052637C, 0x3C, lbl_eu_80526354);
    }

    if (!resFile.HasResAnmChr()) {
        return 0;
    }
    resFile.Init();
    resFile.Bind(resFile);

    CScnItemAnim* item = func_8048C400(self->mPool);
    if (item != 0) {
        item->mParent = self;
        item->mType = 2;
        *(void**)item = (void*)lbl_eu_8056EC90;
        try {
            item->mResFile = nw4r::g3d::ResFile(resData);
            if (!aligned) {
                nw4r::db::Panic(lbl_eu_8052637C, 0x3C, lbl_eu_80526354);
            }
            item->mName[0] = 0;
            item->mNameLen = 0;
            item->mResFileCopy = nw4r::g3d::ResFile(resData);

            nw4r::g3d::ResAnmChr chr = item->mResFile.GetResAnmChr(0);
            if (chr.ptr() == 0) {
                nw4r::db::Panic(lbl_eu_8056E9D0, 0x27, lbl_eu_8056E9B4,
                                lbl_eu_8056E9A8, lbl_eu_80663A88);
            }
            char* name = chr.ofs_to_ptr<char>(chr.ref().name);
            item->mNameLen = std::strlen(name);
            std::strcpy(item->mName, name);
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    func_8048C630(self->mPool, item, param);
    return item;
}

/**
 * Return pointer to the animation name buffer at offset 0x10.
 *
 * The name is a C-string copied from the first animation entry
 * during construction.  The buffer is 0x40 bytes.
 */
extern "C" char* func_8049E8B4(CScnItemAnim* self) {
    return self->mName;
}

/**
 * Return the value at offset 0x54 of the object.
 *
 * This field is initialised to the same value as mResFile during
 * construction and may serve as a cached identifier.
 */
extern "C" nw4r::g3d::ResFile func_8049E8BC(CScnItemAnim* self) {
    return self->mResFileCopy;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace SIABlob {
extern "C" void __dt__12CScnItemAnimFv();
extern "C" void func_8048228C();
extern "C" void func_80482048();
extern "C" void func_80482288();
extern "C" void func_8049E8B4();
extern "C" void func_8049E8BC();
}
extern "C" u32 lbl_eu_80663A80;   // foreign .sdata
extern "C" u32 lbl_eu_806624D8;   // foreign .sdata

// [.data] 0x8056EC90-0x8056ECC0 (48B): CScnItemAnim vtable
extern "C" u32 lbl_eu_8056EC90[12] = {
    (u32)&lbl_eu_80663A80, 0x00000000,
    (u32)&SIABlob::__dt__12CScnItemAnimFv,
    (u32)&SIABlob::func_8048228C,
    (u32)&SIABlob::func_80482048,
    (u32)&SIABlob::func_80482288,
    (u32)&SIABlob::func_8049E8B4,
    (u32)&SIABlob::func_8049E8BC,
    (u32)&lbl_eu_806624D8, 0x00000000, 0x00000000, 0x00000000,
};

// [.rodata] 0x80524248-0x80524268 (32B)
extern "C" __declspec(align(4)) const char lbl_eu_80524248[0x20] = {
    0x43,0x53,0x63,0x6E,0x49,0x74,0x65,0x6D,0x41,0x6E,0x69,0x6D,0x00,0x00,0x00,0x00,
    0x43,0x61,0x6D,0x65,0x72,0x61,0x25,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

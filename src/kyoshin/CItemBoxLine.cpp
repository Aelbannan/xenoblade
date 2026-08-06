// Auto-scaffolded catalog TU for kyoshin/CItemBoxLine
// High-level C/C++ reconstruction from retail PPC.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CItemBoxLine.hpp"

// CIBLTabCur vtable (.data)
extern "C" void* lbl_eu_80534D80[];

// CBaseCur ctor/dtor + operator delete (defined in CCur unit).
extern "C" void __ct__8CBaseCurFv(CBaseCur* _this);
extern "C" void* __dt__8CBaseCurFv(CBaseCur* _this, int flags);
extern "C" void __dl__FPv(void*);

// Tab entry accessors (retail symbols are plain func_XXXX names).
u8 func_801EF034(const CIBLTabCur* self, unsigned int index);
u16 func_801EC3B0(const CIBLTab* self, unsigned int index);
void func_801EBB9C(CIBLTabEntry* dest, const CIBLTabEntry* src);
u8 func_801EC23C(const CIBLTab* self, unsigned int index);
u32 func_801EC260(const CIBLTab* self, unsigned int index);
u8 func_801EC8B4(const CIBLTab* self, unsigned int index);
void func_801EDA08(CItemBoxLine* self);

u8 func_801ED800(void* self) { return static_cast<CItemBoxLine*>(self)->unk59; }

// ============================================================================
// func_801EBB9C: copy a 12-byte tab entry
// ============================================================================
void func_801EBB9C(CIBLTabEntry* dest, const CIBLTabEntry* src) {
    dest->f0 = src->f0;
    dest->f4 = src->f4;
    dest->f8 = src->f8;
    dest->f9 = src->f9;
}

// ============================================================================
// Tab entry accessors (array of 12-byte CIBLTabEntry behind a halfword count)
// ============================================================================

// func_801EC23C: u8 at +0x8
u8 func_801EC23C(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f8;
    }
    return 0;
}

// func_801EC260: u32 at +0x4
u32 func_801EC260(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f4;
    }
    return 0;
}

// func_801EC3B0: u16 at +0x0
u16 func_801EC3B0(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f0;
    }
    return 0;
}

// func_801EC8B4: u8 at +0x9 (default 1)
u8 func_801EC8B4(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f9;
    }
    return 1;
}

// ============================================================================
// CIBLTabCur vtable plumbing (retail ctor/dtor symbols are plain names)
// ============================================================================

// func_801EE840 / __ct__CIBLTabCur
extern "C" CBaseCur* __ct__CIBLTabCur(CBaseCur* _this) {
    __ct__8CBaseCurFv(_this);
    _this->mVtable = (void*)lbl_eu_80534D80;
    return _this;
}

// func_801ED7BC / __dt__801EBBC0 (delete-only dtor)
extern "C" void* __dt__801EBBC0(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// func_801EE87C / __dt__801ECBC0
extern "C" void* __dt__801ECBC0(CBaseCur* _this, int flags) {
    if (_this != NULL) {
        __dt__8CBaseCurFv(_this, 0);
        if (flags > 0) {
            __dl__FPv(_this);
        }
    }
    return _this;
}

// ============================================================================
// func_801EF034: byte at this+0xA8, index bounded to a 12-entry table
// ============================================================================
u8 func_801EF034(const CIBLTabCur* self, unsigned int index) {
    if (index < 12u) {
        return self->unkA8[index];
    }
    return 0;
}

// ============================================================================
// func_801EDA08: reset tab byte entries + two counters
// ============================================================================
void func_801EDA08(CItemBoxLine* self) {
    memset(self->tabEntries, 0, 9);
    self->tabCount = 0;
    self->field6D = 0;
}

void func_801EC438(){}

void func_801EC808(){}

void func_801EC8D8(){}

void func_801EC9E0(){}

void func_801ECC10(){}

void __ct__CItemBoxLine(){}

CItemBoxLine::~CItemBoxLine() {}

void func_801ED31C(){}

void func_801ED3E8(){}

void func_801ED4FC(){}

void func_801ED618(){}

void func_801ED774(){}


void func_801ED808(){}

void func_801ED864(){}

void func_801ED97C(){}

void CItemBoxLine::func_801EDA4C(unsigned char val) {
    unsigned char n = reinterpret_cast<unsigned char*>(this)[0x63];
    if (n >= 9) {
        return;
    }
    reinterpret_cast<unsigned char*>(this)[0x5a + n] = val;
    reinterpret_cast<unsigned char*>(this)[0x63] = n + 1;
}

void func_801EDA6C(){}

void func_801EDB80(){}

void func_801EDC94(){}

void func_801EDF40(){}

void func_801EE228(){}

void func_801EE448(){}

void func_801EE684(){}

void func_801EE788(){}

u8 func_801EECC0(void* self) { return static_cast<CItemBoxLine*>(self)->unk39E; }

void CItemBoxLine::func_801EECC8() {
    func_801EC3B0(&unk3A4, (unsigned char)(unk38C + unk38E));
}

void func_801D2E4C(void* self);
void func_801EECE0(void* self) { ((void(*)(void*))func_801D2E4C)((char*)self + 0xb8); }

void func_801EECE8(){}

void func_801EED6C(){}

void func_801EEDF8(){}

void func_801EF050(){}

void func_801EF0EC(){}

void func_801EF1E4(){}

void func_801EF260(){}

void func_801EF2FC(){}

void func_801EF378(){}

void func_801EF3E8(){}

void func_801EF45C(){}

void func_801EF518(){}

void func_801EF734(){}

void func_801EF844(){}

void func_801EF954(){}

void func_801EFB24(){}

void func_801EFDF4(){}

void func_801EFE6C(){}

void func_801EFFC4(){}

void func_801F0030(){}

void func_801F0488(){}

void func_801F061C(){}

void func_801F071C(){}

void func_801F08B4(){}

void func_801F0A58(){}

void func_801F107C(){}

void func_801F183C(){}

void func_801F1E64(){}

void func_801F20F0(){}

void func_801F2298(){}

void func_801F2434(){}

void func_801F2880(){}

void CItemBoxLine::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_801F32EC(){}

extern "C" void func_801EBB88(void* self, u16 r4, u32 r5, u8 r6, u8 r7) {
    *(u16*)self = r4;
    *(u32*)((u8*)self + 4) = r5;
    *((u8*)self + 8) = r6;
    *((u8*)self + 9) = r7;
}
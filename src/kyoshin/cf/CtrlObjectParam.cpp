// Auto-scaffolded catalog TU for kyoshin/cf/CtrlObjectParam
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CtrlObjectParam.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"
#include "kyoshin/cf/object/CAIAction.hpp"   // func_800B8B94 (actor lookup)
#include "monolib/util/MemManager.hpp"       // mtl::MemManager (work-buffer alloc)

#include <new>
#include <cstring>

// Stub declared below (func_800A145C) — called by func_8009E054.
void func_800A145C(u8* self);

// Arts-list walker defined later in this TU; func_800A3594 aborts its row
// scan through it when it hits an unknown tag (noinline: retail calls it;
// extern "C": retail name is unmangled).
extern "C" __declspec(noinline) void func_800A3A6C(cf::CtrlObjectParamArtsList* list);

// Forward decls for the swap/type helpers defined later in this TU (callers
// in func_8009E168).
extern "C" int func_8009E20C(cf::CtrlObjectParamSwap* self, int firstType, int firstIndex,
                             int secondType, int secondIndex);
extern "C" int func_8009E344(const unsigned int* param_1, unsigned int param_2,
                             int* param_3, int* param_4);

// func_800A30E4 (init helper) — defined later in this TU, called by func_8009DFC8.
extern "C" __declspec(noinline) void func_800A30E4(cf::CtrlObjectParamActorOwner* self);

// func_800A30E4 (init helper) — defined later in this TU, called by func_8009DFC8.
extern "C" __declspec(noinline) void func_800A30E4(cf::CtrlObjectParamActorOwner* self);

// func_8009E974 (work-buffer reset) — defined later in this TU.
extern "C" void func_8009E974(cf::CtrlObjectParamWork* work);

void __ct__8009D604() {
    // Work-buffer (re)initialization: allocate the 0x3A38C-byte buffer once,
    // zero the per-slot arrays, then run the per-entry init (14 entries of
    // 0x3DD4 at +0x41F0). The global is stored even on alloc failure; the
    // buffer is always reset through func_8009E974 afterwards.
    if (lbl_eu_80663E88 == 0) {
        cf::CtrlObjectParamWork* work = static_cast<cf::CtrlObjectParamWork*>(
            mtl::MemManager::allocate(0x3A38C, mtl::MemManager::getHandleMEM1()));
        if (work != 0) {
            // 1000 x 8-byte item slots at +0x58.
            u8* p = reinterpret_cast<u8*>(work) + 0x58;
            u8* pEnd = reinterpret_cast<u8*>(work) + 0x1F98;
            do {
                memset(p, 0, 8);
                p += 8;
            } while (p < pEnd);
            work->field_1F98 = 0;
            func_8009E7C8(pEnd);
            // 14 character entries of 0x3DD4 at +0x41F0.
            cf::CtrlObjectParamEntry* e = reinterpret_cast<cf::CtrlObjectParamEntry*>(
                reinterpret_cast<u8*>(work) + 0x41F0);
            cf::CtrlObjectParamEntry* eEnd = reinterpret_cast<cf::CtrlObjectParamEntry*>(
                reinterpret_cast<u8*>(work) + 0x3A388);
            do {
                e->shortArr[0] = -1;
                e->shortArr[1] = -1;
                e->shortArr[2] = -1;
                e->shortArr[3] = -1;
                e->shortArr[4] = -1;
                e->shortArr[5] = -1;
                memset(e->blob_28, 0, sizeof(e->blob_28));
                memset(e->blob_E8, 0, sizeof(e->blob_E8));
                new (&e->param_17C[0]) cf::CActorParam(
                    reinterpret_cast<UNKTYPE*>(&e->field_34FC),
                    reinterpret_cast<UNKTYPE*>(&e->big[0]));
                e->field_3500 = 0;
                e->field_3504 = 0;
                e->field_3508 = 0;
                e->field_34FC = reinterpret_cast<u32>(lbl_eu_805280E8);
                e->field_350C = 0;
                e->field_352C = 0;
                e->field_3530 = 0;
                func_8025EDC8(reinterpret_cast<CPcKizunagramBig*>(&e->big));
                ++e;
            } while (e < eEnd);
            // Reset the freshly built buffer before publishing it (retail
            // calls func_8009E974 both here and again on the shared tail).
            func_8009E974(work);
        }
        lbl_eu_80663E88 = reinterpret_cast<u32>(work);
    }
    func_8009E974(reinterpret_cast<cf::CtrlObjectParamWork*>(lbl_eu_80663E88));
}

extern "C" void __dt__8009D72C() {
    // Global destructor for the work-buffer pointer. The duplicated guard is
    // the retail dead-branch artifact (MWCC_REFERENCE "goto-chain reproduces
    // duplicated-condition beq target"): two identical tests compile to one
    // cmpwi + two beq's, and the null-store stays inside the guarded path.
    if (lbl_eu_80663E88 == 0) goto done;
    if (lbl_eu_80663E88 == 0) goto done;   // dead duplicate, kept by retail
    ::operator delete(reinterpret_cast<void*>(lbl_eu_80663E88));
    lbl_eu_80663E88 = 0;
done:
    ;
}

extern "C" void* func_8009D764(cf::CtrlObjectParamInit* p) {
    p->field_00 = -1;
    p->field_02 = -1;
    p->field_04 = -1;
    p->field_06 = -1;
    p->field_08 = -1;
    p->field_0A = -1;
    extern void* memset(void*, int, unsigned long);
    return static_cast<char*>(memset(p->blob, 0, sizeof(p->blob)));
}

extern "C" u8* func_8009D790(s16* arr, u32 idx) {
    // Equip-table helper: when the s16 entry is valid (> -1), resolve the
    // item instance for the category and entry value; 0 otherwise.
    u8* result = 0;
    if (arr[idx] > -1) {
        s16* q = &arr[idx];
        u32 a = idx > 4 ? 2 : idx + 4;
        result = reinterpret_cast<u8*>(func_80157C4C(a, *q));
    }
    return result;
}

cf::CtrlObjectParamSubEntry* cf::CtrlObjectParamData::getSubStruct(unsigned long index) {
    return &entries[index];
}

extern "C" __declspec(noinline) void func_8009D7F4(s16* arr, u32 index, void* self) {}

void* cf::CActorParam::CActorParam_UnkVirtualFunc94() {
    return &reinterpret_cast<cf::CActorParamRetailView*>(this)->field_1650;
}

extern "C" void func_80155CD0(void* a, void* b);
extern "C" void func_8009DB1C(void* ignored, void* a, void* b) { func_80155CD0(a, b); }

extern "C" void func_8009DB28(cf::CtrlObjectParamEquipRow* self, u32 index) {
    // Remove the equip-slot entry: resolve the item instance, run the impl's
    // 0x10 hook, then clear the two s16 fields and resync the owner state.
    void* inst = 0;
    if (self->shortArr[index] > -1) {
        inst = func_80157C4C(index > 4 ? 2 : index + 4, self->shortArr[index]);
    }
    if (inst != 0) {
        reinterpret_cast<cf::CtrlObjectParamItemImplIf*>(
            CItem_initItemImplInstances(inst))->_v10(inst);
    }
    self->shortArr[index] = -1;
    self->field_0E[index] = 0;
    func_8009D7F4(&self->shortArr[0], index, self);
    func_800A30E4(reinterpret_cast<cf::CtrlObjectParamActorOwner*>(self));
}

void func_8009DBF4(){}

void cf::CActorParam::CActorParam_UnkVirtualFunc33(float val) {
    reinterpret_cast<float&>(unk17E4.unk0[4]) = val;
}

void func_8009DFC8(cf::CtrlObjectParamActorOwner* self) {
    // Activate the owner: init via func_800A30E4, then dispatch virtual
    // slots 0xA4 / 0xA8 through the CActorParam embedded at +0x17C.
    func_800A30E4(self);
    reinterpret_cast<cf::CtrlObjectParamVt0A8If*>(&self->mParam)->_v0A4(self);
    reinterpret_cast<cf::CtrlObjectParamVt0A8If*>(&self->mParam)->_v0A8(1);
}

extern "C" void func_8009DBF4(void* a, unsigned long b, void* c);
extern "C" void func_8009E024(void* a, void* c) { func_8009DBF4(a, 0, c); }

extern "C" void func_8009E030(void* a, void* c) { func_8009DBF4(a, 1, c); }

void cf::CtrlObjectParamData::setArgType2(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 2, arg);
}

void cf::CtrlObjectParamData::setArgType3(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 3, arg);
}

extern "C" void func_8009E054(cf::CtrlObjectParamData* self, u8* arg) {
    // Forward the arg as type 4, then run the type-8 init path when the
    // object's type tag is 8 and no actor exists for it.
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, u8*);
    func_8009DBF4(self, 4, arg);
    if (self->field_00 == 8) {
        if (func_800B8B94(self->field_00) == 0) {
            func_800A145C(reinterpret_cast<u8*>(self));
        }
    }
}

void cf::CtrlObjectParamData::setArgType5(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 5, arg);
}

long cf::CtrlObjectParamData::getShortAt1C(unsigned long index) {
    return entries[0].shortArr[index];
}

void func_8009E0C4(cf::CtrlObjectParamU16RowTable* table, u16 index, u16 value) {
    // Resolve the item family (outputs discarded), then store value into the
    // u16 row table at +2.
    u16 a;
    u16 b;
    func_80157F04(value, &a, &b);
    table->rows[index] = value;
}

extern "C" u32 func_8009E120(cf::CtrlObjectParamRowView* p, u32 value) {
    // Arts/row lookup: read the u16 table at +0x02 at index (u16)value; a
    // 0xFFFF table entry means "empty" and is folded to 0 before dispatch.
    u16 v = p->field_02[(u16)value];
    if (v == 0xFFFF) v = 0;
    if (v != 0)
        return func_80142074(p->field_00, (u16)v, 0);
    return func_80141E90(p->field_00, (s16)v, (u16)(value + 1), 0);
}

// Locate the (type, index) pair for value a and for value b in the swap
// object; when both are present, swap them via func_8009E20C.
int func_8009E168(cf::CtrlObjectParamSwap* self, unsigned int a, unsigned int b) {
    int t1 = 0;
    int i1 = 0;
    int t2 = 0;
    int i2 = 0;
    int r1 = func_8009E344(reinterpret_cast<const unsigned int*>(self), a, &t1, &i1);
    int r2 = func_8009E344(reinterpret_cast<const unsigned int*>(self), b, &t2, &i2);
    if (r1 != 0 && r2 != 0) {
        return func_8009E20C(self, t1, i1, t2, i2);
    }
    return 0;
}

extern "C" int func_8009E20C(cf::CtrlObjectParamSwap* self, int firstType, int firstIndex,
                             int secondType, int secondIndex) {
    int* first = 0;
    int* second = 0;
    if (firstType == 1)
        first = &self->intArr1[firstIndex];
    else if (firstType == 2)
        first = &self->intArr2[firstIndex];
    if (secondType == 1)
        second = &self->intArr1[secondIndex];
    else if (secondType == 2)
        second = &self->intArr2[secondIndex];
    int value = *first;
    *first = *second;
    *second = value;
    return 1;
}

// Membership test: 1 if value matches any of the 9 slot words (arr1[3] +
// arr2[6]) starting at +4, 0 otherwise. The tail reads go through a +8 base
// like the retail (p = &arr[2]; p[4..7]).
int func_8009E284(const int* arr, int value) {
    if (arr[1] == value) return 1;
    if (arr[2] == value) return 1;
    if (arr[3] == value) return 1;
    if (arr[4] == value) return 1;
    if (arr[5] == value) return 1;
    const int* p = arr + 2;
    if (p[4] == value) return 1;
    if (p[5] == value) return 1;
    if (p[6] == value) return 1;
    if (p[7] == value) return 1;
    return 0;
}

extern "C" int func_8009E344(const unsigned int* param_1, unsigned int param_2,
                              int* param_3, int* param_4) {
    for (int i = 0; i < 3; ++i) {
        if (param_1[i + 1] == param_2) {
            *param_3 = 1;
            *param_4 = i;
            return 1;
        }
    }
    for (int i = 0; i < 6; ++i) {
        if (param_1[i + 4] == param_2) {
            *param_3 = 2;
            *param_4 = i;
            return 1;
        }
    }
    return 0;
}

// Slot-validity check: all 9 slot words (arr1 + arr2) must be in [0, 0xD].
// The loop strides the dead counter by 2 and the word index by 3 per trip
// (3 trips of 3 checks = words 1..9), matching the retail mtctr/bdnz shape.
int func_8009E3C0(const int* arr) {
    int ok = 1;
    for (int i = 0, idx = 0; i < 6; i += 2, idx += 3) {
        if (arr[idx + 1] < 0 || arr[idx + 1] > 0xD) ok = 0;
        if (arr[idx + 2] < 0 || arr[idx + 2] > 0xD) ok = 0;
        if (arr[idx + 3] < 0 || arr[idx + 3] > 0xD) ok = 0;
    }
    return ok;
}

extern "C" int func_8009E474(cf::CtrlObjectParamSwap* self, unsigned int value) {
    // Move value to the head slot: probe the list, then swap the head entry
    // with value's current (type, index); when either lookup misses, just
    // store value at the head slot. The first probe's result is discarded.
    int t1 = 0, i1 = 0;
    u32 other = self->intArr1[0];
    func_8009E344(reinterpret_cast<const unsigned int*>(self), value, &t1, &i1);
    if (other != 0) {
        int i3, t3, i2, t2;
        t2 = 0;
        i2 = 0;
        t3 = 0;
        i3 = 0;
        int f2 = func_8009E344(reinterpret_cast<const unsigned int*>(self), value, &t2, &i2);
        int f3 = func_8009E344(reinterpret_cast<const unsigned int*>(self), other, &t3, &i3);
        int result;
        if (f2 != 0 && f3 != 0) {
            result = func_8009E20C(self, t2, i2, t3, i3);
        } else {
            result = 0;
        }
        if (result != 0) {
            return 1;
        }
        self->intArr1[0] = value;
        return 1;
    }
    self->intArr1[0] = value;
    return 1;
}

#pragma push
#pragma auto_inline off
extern "C" void func_8009E574(void* a, int b, int c, int d) {}
#pragma pop

extern "C" void func_8009E56C(void* a, int b, int c) { func_8009E574(a, b, c, -1); }

extern "C" int func_8009E740(cf::CtrlObjectParamSlots* self, int value) {
    // Clear the first occurrence of value in arr1 (3 entries), else in
    // arr2 (6 entries); return 1 on a hit, 0 otherwise.
    for (int i = 0; i < 3; ++i) {
        if (self->arr1[i] == value) {
            self->arr1[i] = 0;
            return 1;
        }
    }
    for (int i = 0; i < 6; ++i) {
        if (self->arr2[i] == value) {
            self->arr2[i] = 0;
            return 1;
        }
    }
    return 0;
}

int cf::CtrlObjectParamClear::clearStruct() {
    firstByte = 0;
    for (int i = 0; i < 24; ++i) words[i] = 0;
    return 1;
}

extern "C" void func_8009E838(u8* self) {
    for (int a = 0; a < 3; a++) {
        void* cur = (u8*)self + a * 4;
        if (*(u32*)((u8*)cur + 4) == 0) {
            int found = 0;
            for (int i = a + 1; i < 3; i++) {
                if (*(u32*)((u8*)self + (i * 4) + 4) != 0) {
                    *(u32*)((u8*)cur + 4) = *(u32*)((u8*)self + (i * 4) + 4);
                    *(u32*)((u8*)self + (i * 4) + 4) = 0;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                int done = 0;
                for (int j = 0; j < 6; j++) {
                    void* cur2 = (u8*)self + j * 4;
                    if (*(u32*)((u8*)cur2 + 0x10) == 0) {
                        for (int i = j + 1; i < 6; i++) {
                            if (*(u32*)((u8*)self + (i * 4) + 0x10) != 0) {
                                *(u32*)((u8*)cur2 + 0x10) = *(u32*)((u8*)self + (i * 4) + 0x10);
                                *(u32*)((u8*)self + (i * 4) + 0x10) = 0;
                                done = 1;
                                break;
                            }
                        }
                    }
                    if (done) break;
                }
                return;
            }
        }
    }
}

extern "C" void func_8009E974(cf::CtrlObjectParamWork* work) {
    // Reset the work buffer: zero the header/copy area, the two bitmaps and
    // the per-entry s16 fields, then re-run the per-slot and entry init and
    // refresh the event data table.
    memset(work, 0, 0x24);
    memset(reinterpret_cast<u8*>(work) + 0x24, 0, 0x24);
    memset(reinterpret_cast<u8*>(work) + 0x48, 0, 8);
    memset(work->bitmap_1FFC, 0, sizeof(work->bitmap_1FFC));
    memset(work->bitmap_3866, 0, sizeof(work->bitmap_3866));
    for (u32 i = 0; i < 14; ++i) {
        cf::CtrlObjectParamEntry* e = &work->entries[i];
        for (u32 j = 0; j < 7; ++j) e->field_00[j] = 0;
        for (u32 j = 0; j < 6; ++j) e->field_0E[j] = 0;
        for (u32 j = 0; j < 6; ++j) e->shortArr[j] = -1;
        memset(e->blob_28, 0, sizeof(e->blob_28));
    }
    for (u32 i = 0; i < 0x3E8; ++i) {
        memset(work->slots_58[i], 0, sizeof(work->slots_58[i]));
    }
    work->field_3A388 = 0;
    work->field_50 = 0;
    work->field_52 = 0;
    work->field_54 = 1;
    work->field_56 = 1;
    func_8016455C(reinterpret_cast<CEventDataTable*>(&work->eventTable));
}

extern "C" void func_8009EABC() {
    // Zero the 1000 8-byte entry slots at +0x58, then the 50000-bit
    // bitmap (0x186A bytes) at +0x1FFC.
    u8* work = reinterpret_cast<u8*>(lbl_eu_80663E88);
    for (u32 i = 0; i < 0x3E8; ++i) {
        memset(work + 0x58 + i * 8, 0, 8);
    }
    memset(work + 0x1FFC, 0, 0x186A);
}

extern "C" char* func_8009EB2C(u16 arg1, u16 arg2, const char* srcStr) {
    // Copy the 36-byte header block forward, write the two s16 ids, then
    // strncpy the caller's string into the (already copied) source area.
    cf::CtrlObjectParamCopyView* work =
        reinterpret_cast<cf::CtrlObjectParamCopyView*>(lbl_eu_80663E88);
    work->dst = work->src;
    work->src.field_02 = (s16)arg1;
    work->src.field_00 = (s16)arg2;
    return strncpy(work->src.str, srcStr, 0x1f);
}

void func_8009EB94(unsigned int idx, int flag) {
    // Set (flag != 0) or clear (flag == 0) bit (idx & 7) of the 50000-bit
    // bitmap at work-buffer +0x1FFC.
    if (idx >= 50000) return;
    int mask = 1 << (idx & 7);
    u8* p = reinterpret_cast<u8*>(lbl_eu_80663E88 + (idx >> 3));
    if (flag != 0) {
        p[0x1FFC] |= (u8)mask;
    } else {
        p[0x1FFC] &= (u8)~mask;
    }
}

extern "C" int func_8009EBE8(unsigned int idx) {
    extern u32 lbl_eu_80663E88;
    if (idx >= 50000) return 0;
    const u8* bitmap = reinterpret_cast<const u8*>(lbl_eu_80663E88 + 8188);
    return (bitmap[idx >> 3] >> (idx & 7)) & 1;
}

void func_8009EC18(unsigned int idx, int flag) {
    // Set (flag != 0) or clear (flag == 0) bit (idx & 7) of the 320-bit
    // bitmap at work-buffer +0x3866.
    if (idx >= 0x140) return;
    int mask = 1 << (idx & 7);
    u8* p = reinterpret_cast<u8*>(lbl_eu_80663E88 + (idx >> 3));
    if (flag != 0) {
        p[0x3866] |= (u8)mask;
    } else {
        p[0x3866] &= (u8)~mask;
    }
}

extern "C" int func_8009EC6C(unsigned int idx) {
    // Flag-bit lookup: bit idx of the 320-bit bitmap at work-buffer +0x3866.
    if (idx >= 0x140) return 0;
    const u8* bitmap = reinterpret_cast<const u8*>(lbl_eu_80663E88 + 0x3866);
    return (bitmap[idx >> 3] >> (idx & 7)) & 1;
}

extern "C" void* func_8009EC9C(int idx) {
    extern u32 lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + idx * 15828 + 16880);
}

extern "C" void* func_8009ECB0() {
    extern unsigned long lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + 0x1f98);
}

extern "C" void* func_8009ECBC(int idx) {
    extern u32 lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + (idx * 8) + 88);
}

extern "C" void func_8009ECD0(unsigned long val) {
    extern unsigned long lbl_eu_80663E88;
    unsigned long addr = lbl_eu_80663E88;
    *reinterpret_cast<unsigned long*>(addr + 0x3A388) = val;
}

extern "C" u32 func_8009ECE0() {
    extern u32 lbl_eu_80663E88;
    return *reinterpret_cast<u32*>(lbl_eu_80663E88 + 0x3A388);
}

extern "C" unsigned long func_8009ECF0() {
    extern unsigned long lbl_eu_80663E88;
    return *reinterpret_cast<unsigned short*>(lbl_eu_80663E88 + 0x50);
}

extern "C" void func_8009ECFC(unsigned short value) {
    extern unsigned long lbl_eu_80663E88;
    *reinterpret_cast<unsigned short*>(lbl_eu_80663E88 + 0x50) = value;
}

void __ct__8009ED08(){}

void func_8009EF9C(){}

void cf::CActorParam::CActorParam_UnkVirtualFunc16(float val) {
    unk15F8 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc55(u16 val) {
    unk160E = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc61(u16 val) {
    unk1616 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc65(float val) {
    unk1610 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc68(float val) {
    unk1618 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc74(float val) {
    unk17E4.unk4C = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc14(u8 val) {
    unk15F4[0] = val;
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc165() {
    return &reinterpret_cast<cf::CActorParamRetailView*>(this)->field_164C;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc116(float val) {
    unk1620 = val;
}

void func_8009F6D4(){}

void* cf::CActorParam::CActorParam_UnkVirtualFunc152() {
    return &reinterpret_cast<cf::CActorParamRetailView*>(this)->field_3358;
}

void __ct__8009F8B8(){}

void func_800A03F4(){}

void* cf::CActorParam::CActorParam_UnkVirtualFunc125() {
    return reinterpret_cast<cf::CActorParamRetailView*>(this)->field_2740;
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc76() {
    return &reinterpret_cast<cf::CActorParamRetailView*>(this)->field_1830;
}

extern "C" u32 func_800A082C(cf::CtrlObjectParamActorOwner* self) {
    // Virtual dispatch to CActorParam_UnkVirtualFunc94 (vtable slot 0x20C)
    // through the param embedded at +0x17C; the caller uses the low 16 bits
    // of the returned struct's first word.
    cf::CtrlObjectParamWordView* p =
        reinterpret_cast<cf::CtrlObjectParamWordView*>(self->mParam.CActorParam_UnkVirtualFunc94());
    return p->word0 & 0xFFFF;
}

void func_800A0860(){}

void* cf::CActorParam::CActorParam_UnkVirtualFunc100() {
    return &reinterpret_cast<cf::CActorParamRetailView*>(this)->field_17E4;
}

void func_800A0E64(){}

void func_800A11A4(){}

extern "C" void func_800A1370(cf::CtrlObjectParamArtsView* self) {
    // Resolve the actor for this arts type id, then hand the vtable-0x28C
    // result (a pointer) plus the arts-data write target at +0x17C to
    // func_80175A50 (retail arg order: value in r3, obj in r4).
    u8* obj = reinterpret_cast<u8*>(func_800B8B94(self->field_00));
    if (obj != 0) {
        u8* value = reinterpret_cast<cf::CtrlObjectParamVt028CIf*>(obj)->_v028C();
        func_80175A50(value, &self->field_17C);
    }
}

void CActorParam_UnkVirtualFunc126__Q22cf11CActorParamFv() {}

// Resolve the actor for this arts type id, then hand the vtable-0x28C result
// (a pointer) plus the arts-data write target at +0x17C to func_80175A50
// (retail arg order: value in r3, obj in r4). When no actor exists, activate
// the embedded CActorParam at +0x17C through its vtable-0xA4 slot instead.
void func_800A13C4(cf::CtrlObjectParamArtsView* self, u32 arg2) {
    void* actor = func_800B8B94(self->field_00);
    if (actor != 0) {
        func_801765A4(lbl_eu_806667A0, arg2);
        u8* value = reinterpret_cast<cf::CtrlObjectParamVt028CIf*>(actor)->_v028C();
        func_80175A50(value, &self->field_17C);
    } else {
        reinterpret_cast<cf::CtrlObjectParamVt0A8If*>(
            &reinterpret_cast<cf::CtrlObjectParamActorOwner*>(self)->mParam)
            ->_v0A4(reinterpret_cast<cf::CtrlObjectParamActorOwner*>(self));
    }
}

void func_800A145C(u8* self){}

u32 cf::CActorParam::CActorParam_UnkVirtualFunc26() {
    return *reinterpret_cast<u32*>(reinterpret_cast<cf::CActorParamRetailView*>(this)->field_1650);
}

void CActorParam_UnkVirtualFunc166__Q22cf11CActorParamFv() {}

void func_800A18A4(){}

void func_800A1B08(){}

void func_800A1CA0(){}

void func_800A1E3C(){}

void func_800A21F8(){}

void cf::CActorParam::CActorParam_UnkVirtualFunc82(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1604 + addend;
    unk1604 = val;
    if (val > cap) {
        unk1604 = cap;
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc90(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1608 + addend;
    unk1608 = val;
    if (val > cap) {
        unk1608 = cap;
    }
}

void func_800A26A4(){}

void func_800A282C(){}

void func_800A2974(){}

u32 cf::CActorParam::CActorParam_UnkVirtualFunc85() {
    return reinterpret_cast<cf::CActorParamRetailView*>(this)->field_1604;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc83(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1600 + addend;
    unk1600 = val;
    if (val > cap) {
        unk1600 = cap;
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc81(u32 val) {
    unk1604 = val;
}

void func_800A2AF0(){}

void func_800A2DE8(){}

extern "C" __declspec(noinline) void func_800A30E4(cf::CtrlObjectParamActorOwner* self){}

u8 cf::CtrlObjectParamByteE4::getByteE4() {
    return field_E4;
}

extern "C" u8 func_800A32C4(cf::CtrlObjectParamBdatRow* self) {
    // bdat column lookup: item/weapon table (lbl_eu_806640F4), column name
    // at lbl_eu_804FBCB0+0x58, row key at +0xC of the object; the returned
    // column value is truncated to a byte.
    return (u8)getBdatStringColumnValue(reinterpret_cast<void*>(lbl_eu_806640F4),
                                        &lbl_eu_804FBCB0[0x58], self->field_0C);
}

extern "C" void func_800A3304() {
    // Refresh every item instance in the equip rows for character rows
    // 1..13: resolve each valid slot entry and run the impl's 0x48 hook.
    int j;
    void* inst;
    cf::CtrlObjectParamEquipRow* r;
    for (u32 row = 1; row <= 13; ++row) {
        r = reinterpret_cast<cf::CtrlObjectParamEquipRow*>(
            reinterpret_cast<u8*>(lbl_eu_80663E88) + (u16)row * 0x3DD4 + 0x41F0);
        for (j = 0; j <= 5; ++j) {
            inst = 0;
            if (r->shortArr[j] > -1) {
                inst = func_80157C4C((u32)j > 4 ? 2 : j + 4, r->shortArr[j]);
            }
            if (inst != 0) {
                reinterpret_cast<cf::CtrlObjectParamItemImplIf*>(
                    CItem_initItemImplInstances(inst))->_v48(inst);
            }
        }
    }
}

extern "C" u32 func_800A33C8(void* arg) {
    // Look a character up in the party's equip slots: walk the 9-row slot
    // table; for each non-empty row, walk the 6 equip entries and compare
    // the resolved item instance against arg.
    u32 work = lbl_eu_80663E88;
    if (arg != 0) {
        u8* slots = reinterpret_cast<u8*>(work) + 0x1F9C;
        for (int i = 0; i < 9; ++i) {
            if (*(u32*)(slots + (i << 2)) != 0) {
                cf::CtrlObjectParamEquipRow* row =
                    reinterpret_cast<cf::CtrlObjectParamEquipRow*>(
                        reinterpret_cast<u8*>(lbl_eu_80663E88) +
                        (u16)*(u32*)(slots + (i << 2)) * 0x3DD4 + 0x41F0);
                for (int j = 0; j <= 5; ++j) {
                    void* inst = 0;
                    if (row->shortArr[j] > -1) {
                        inst = func_80157C4C((u32)j > 4 ? 2 : j + 4, row->shortArr[j]);
                    }
                    if (inst == arg) {
                        return *(u32*)(slots + (i << 2));
                    }
                }
            }
        }
    }
    return 0;
}

void cf::CObjectState::CObjectState_UnkVirtualFunc13() {
    unkC = unk8;
}

void* cf::CObjectState::CObjectState_UnkVirtualFunc12() {
    return &unkC;
}

void cf::CObjectState::CObjectState_UnkVirtualFunc6() {
    unk8 = 0;
}

void cf::CObjectState::CObjectState_UnkVirtualFunc4() {
    unk4 = 0;
}

int cf::CObjectParam::CObjectParam_UnkVirtualFunc3() {
    return reinterpret_cast<cf::CObjectParamRetailView*>(this)->field_30 != 0 ? 1 : 0;
}

void cf::CtrlObjectParamClear16::clear16Bytes() {
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[3] = 0;
}

extern "C" void* __dt__800A34E0(void* self, int flag) {
    // Delete-only destructor: free self when the delete flag is positive.
    if (self != 0 && flag > 0) {
        ::operator delete(self);
    }
    return self;
}

extern "C" void func_800A3520(cf::CtrlObjectParamArtsSlot* self,
                              cf::CtrlObjectParamArtsListEntry* buf,
                              u32 value, u32 other) {
    // Init an arts list entry: record the header, zero the 0x20-byte entry,
    // then stamp the 0x1111 tag and the (count-1) sentinel.
    if (buf != 0) {
        self->buf = buf;
        self->field_0C = value;
        self->field_04 = value >> 5;
        self->field_08 = other;
        memset(buf, 0, 0x20);
        self->buf->field_00 = 0x1111;
        self->buf->field_04 = self->field_04 - 1;
    }
}

extern "C" void* func_800A3594(cf::CtrlObjectParamArtsList* list, u32 value) {
    // Find the 0x1111 row whose count is the smallest fit for the (value+31)/32
    // slot need; an exact match stops the walk early. On success, re-tag the
    // row as 0xAAAA and split the surplus slots off as a new 0x1111 node.
    // best declared first: retail colors it r31 (register-mapping contract).
    cf::CtrlObjectParamArtsListEntry* best = 0;
    u32 need = (value + 31) >> 5;
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    u32 bestCount = list->field_04;
    while (e != 0) {
        u16 type = e->field_00;
        if (type == 0x1111) {
            u32 count = e->field_04;
            if (count >= need && count < bestCount) {
                best = e;
                bestCount = count;
                if (count == need) break;
            }
        } else if (type == 0xAAAA) {
            // pass-through row: skip
        } else {
            func_800A3A6C(list);
            break;
        }
        e = e->next;
    }
    if (best == 0) return 0;
    best->field_00 = 0xAAAA;
    memset(best->data, 0, 0xC);
    if (best->field_04 != need) {
        cf::CtrlObjectParamArtsNode* nd = reinterpret_cast<cf::CtrlObjectParamArtsNode*>(
            reinterpret_cast<u8*>(best) + (need << 5));
        nd->field_20 = 0x1111;
        nd->field_24 = best->field_04 - need - 1;
        nd->field_28 = reinterpret_cast<u32>(best);
        cf::CtrlObjectParamArtsListEntry* oldNext = best->next;
        nd->field_2C = reinterpret_cast<u32>(oldNext);
        cf::CtrlObjectParamArtsListEntry* n = reinterpret_cast<cf::CtrlObjectParamArtsListEntry*>(
            reinterpret_cast<u8*>(nd) + 0x20);
        if (oldNext != 0) {
            oldNext->prev = reinterpret_cast<u32>(n);
        }
        best->field_04 = need;
        best->next = n;
    }
    return reinterpret_cast<u8*>(best) + 0x20;
}

extern "C" void* func_800A36A4(cf::CtrlObjectParamArtsList* list, u32 value) {
    // Arts-list allocator: find the 0x1111 row with count >= need, starting
    // from the TAIL of the list and walking backward through prev; the first
    // hit wins. An exact match re-tags the whole row 0xAAAA; a larger row has
    // its surplus tail split off as a new 0xAAAA node. Returns the data area
    // (+0x20) of the winning node, or 0 when no row fits.
    u32 need = (value + 31) >> 5;
    cf::CtrlObjectParamArtsListEntry* best = 0;
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    while (e->next != 0) e = e->next;
    while (e != 0) {
        if (e->field_00 == 0x1111) {
            if (e->field_04 >= need) {
                best = e;
                break;
            }
        }
        e = reinterpret_cast<cf::CtrlObjectParamArtsListEntry*>(e->prev);
    }
    if (best == 0) return 0;
    u32 count = best->field_04;
    cf::CtrlObjectParamArtsListEntry* node = best;
    if (count == need) {
        best->field_00 = 0xAAAA;
        memset(&best->data[0], 0, 0xC);
    } else {
        // Split the surplus (count - need) slots off the tail as a new node.
        node = reinterpret_cast<cf::CtrlObjectParamArtsListEntry*>(
            reinterpret_cast<u8*>(best) + ((count - need) << 5));
        node->field_00 = 0xAAAA;
        memset(&node->data[0], 0, 0xC);
        node->field_04 = need;
        node->prev = reinterpret_cast<u32>(best);
        node->next = best->next;
        if (node->next != 0) {
            node->next->prev = reinterpret_cast<u32>(node);
        }
        best->field_04 = count - need - 1;
        best->next = node;
    }
    return reinterpret_cast<u8*>(node) + 0x20;
}

void func_800A37CC(){}

u32 func_800A3940(cf::CtrlObjectParamArtsList* list) {
    // Walk the arts list keeping the maximum (count << 5) across 0x1111 rows;
    // 0xAAAA rows are skipped, anything else aborts with 0.
    u32 max = 0;
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    while (e != 0) {
        int type = e->field_00;
        switch (type) {
        case 0x1111: {
            u32 count = e->field_04 << 5;
            if (count > max) max = count;
            break;
        }
        case 0xAAAA:
            break;
        default:
            return 0;
        }
        e = e->next;
    }
    return max;
}

extern "C" int func_800A3998(cf::CtrlObjectParamArtsList* list) {
    // Walk the arts list; 0x1111 rows contribute (count << 5), 0xAAAA rows
    // are skipped, anything else aborts with 0.
    int total = 0;
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    while (e != 0) {
        int type = e->field_00;
        if (type == 0x1111) {
            total += (int)(e->field_04 << 5);
        } else if (type != 0xAAAA) {
            return 0;
        }
        e = e->next;
    }
    return total;
}

extern "C" int func_800A39E8(cf::CtrlObjectParamArtsList* list, u8* target,
                             u32 index, u32 value) {
    // Walk the arts list for an 0xAAAA row whose data area (+0x20) is the
    // target; write value into row->data[index] and re-verify the row tag.
    int found = 0;
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    while (e != 0) {
        u16 type = e->field_00;
        if (type == 0x1111) {
            e = e->next;
        } else if (type == 0xAAAA) {
            if ((u8*)&e->data[4] == target) {
                found = 1;
                break;
            }
            e = e->next;
        } else {
            break;
        }
    }
    if (found == 0) return 0;
    cf::CtrlObjectParamArtsListEntry* entry =
        reinterpret_cast<cf::CtrlObjectParamArtsListEntry*>(target - 0x20);
    if (entry->field_00 != 0xAAAA) return 0;
    entry->data[index] = value;
    return 1;
}

// Walk the arts list: 0x1111 rows get their data[0] slot cleared, 0xAAAA rows
// sprintf their data[1] name into a local buffer; any other tag aborts the
// walk. Rows with a non-zero data[0] after handling get it formatted via
// func_800AA33C (goto-chain reproduces the retail bodies-after-tests layout).
extern "C" void func_800A3A6C(cf::CtrlObjectParamArtsList* list) {
    char sbuf[0x40];
    cf::CtrlObjectParamArtsListEntry* e = list->head;
    while (e != 0) {
        u16 type = e->field_00;
        if (type == 0x1111) goto case1111;
        if (type == 0xAAAA) goto caseAAAA;
        goto loop_done;
    case1111:
        e->data[0] = 0;
        goto common;
    caseAAAA:
        sprintf(sbuf, lbl_eu_804FBF58, e->data[1]);
        goto common;
    common:
        if (e->data[0] != 0) {
            ml::FixStr<64> str(true);
            func_800AA33C(str, e->data[0], 0, 0);
        }
        e = e->next;
    }
loop_done: ;
}

extern "C" void func_8009E0A8(void* a, void* c) { func_8009DBF4(a, 5, c); }

// Auto-scaffolded catalog TU for kyoshin/CItemBoxInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "monolib/util.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include <new>

// Retail is 2-arg (category index + s16 item id, see CEquipChange.hpp).
extern "C" void* func_80157C4C(u32 index, s16 value);
// Legacy 1-arg form (category 0) used by not-yet-matched reconstructions.
static inline void* func_80157C4C_1(u32 id) { return func_80157C4C(0, (s16)id); }
// --- Forward declarations ---
namespace nw4r { namespace lyt { class Layout; class DrawInfo; class AnimTransform; } }
void func_80136910(nw4r::lyt::Layout*, char*, u8);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
int sprintf(char*, const char*, ...);
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void func_80127BD8(void*, float*);
char* func_80136190(char*, char*, u32);
extern "C" char* func_8013639C(void*, char*);
u32 func_801392C0();
u8 func_8013600C(const void*, const void*, u32);
u32 func_800A32BC();
u32 func_800A082C(void*);
extern "C" f32 func_8013B380(u32);
extern void* lbl_eu_806645A8;
extern void* lbl_eu_806645B0;
extern void* lbl_eu_80664598;
extern void* lbl_eu_806645A0;
void func_801D885C(CItemBoxInfo*);
extern "C" void func_801D5564(void*, void*, void*, void*);
void func_801D8318(CItemBoxInfo*);
extern "C" void func_801D4E2C(void*, void*, void*);
extern "C" void func_801D69FC(CItemBoxInfo*, u32, void*);
void func_801D8A88(CItemBoxInfo*);
void func_801D77A4(void*, u32, u16);
extern "C" void func_801D8E34(CItemBoxInfo*, u32, void*, u32);
extern "C" void func_801E197C(void*, void*, void*);
void func_801E1E0C(CItemBoxSlotFlags*, void*, void*);
extern "C" void* func_8009ECB0();
extern "C" void* func_800B8B94(u32);
extern "C" void func_800A13C4(void*, u32);
extern "C" void func_80136C98(void*, u32);
extern "C" void func_80136D74(void*, const char*, u32);
extern "C" void func_8009D7E4(void*, u32);
extern "C" s32 func_801C6158(float);
extern "C" void func_80139AC8(void*, void*, void*);
extern "C" u32 func_801E9774(void*, u16, void*);
extern "C" u32 func_801E9690(void*, u32, u32);
extern "C" u32 func_801E9310(void*, void*, u32, void*);
extern "C" s32 func_801E9190(void*, void*, s32, void*);
extern "C" s32 func_801E9224(void*, void*, s32, void*);
extern "C" u32 func_801E96F0(void*, u32, u32);
// .sbss GXColorS10 palette globals (8-byte entries; declared as void* per
// the hard-symbol convention, read through casts in the renderers).
extern void* lbl_eu_80664518;
extern void* lbl_eu_80664520;
extern void* lbl_eu_80664528;
extern void* lbl_eu_80664530;
extern void* lbl_eu_80664538;
extern void* lbl_eu_80664540;
extern void* lbl_eu_80664548;
extern void* lbl_eu_80664550;
extern void* lbl_eu_80664558;
extern void* lbl_eu_80664560;
extern void* lbl_eu_80664568;
extern void* lbl_eu_80664570;
extern void* lbl_eu_80664578;
extern void* lbl_eu_80664580;
extern void* lbl_eu_80664588;
extern void* lbl_eu_80664590;
extern void* lbl_eu_806645A8;
extern void* lbl_eu_806645B0;
extern void* lbl_eu_806645C8;
extern void* lbl_eu_806645D0;
extern void* lbl_eu_806645B8;
extern void* lbl_eu_806645C0;
char* func_801394D4(u32);
u32 func_801E9774(void*, u16, void*);
bool func_801E98E4(void*, u16, void*);
u32 func_801DFD60(void*, void*, u32);
u32 func_801DFDC0(void*, u32, void*);
u32 func_801DF610(void*, void*, u32, void*);
u32 func_801DFE48(void*, u16, void*);
u32 func_801DFFB8(void*, u16, void*, void*);
s32 func_801DF4E0(void*, void*, s32, void*);
s32 func_801DF578(void*, void*, s32, void*);
u32 func_801E92B8(void*, void*);
u32 func_801E9310(void*, void*, u32, void*);
void func_801DF4B4(void*, void*);
void func_801DF4D0(void*, u16, u32, u8);
void func_801E9164(void*, void*);
void func_801E9180(void*, u16, u32, u8);
void func_801D4260(CItemBoxInfo*, u16, void*, u16);
extern void* lbl_eu_80664104;
extern void* lbl_eu_806640A8;
extern void* lbl_eu_806640F4;
extern void* lbl_eu_806640EC;
extern void* func_801571FC();
extern void* lbl_eu_806640F8;
extern void* lbl_eu_806640D8;
extern u32 lbl_eu_80506330[8];
extern float lbl_eu_80668040;
extern float lbl_eu_8066800C;
extern const float lbl_eu_80668010;
extern const float lbl_eu_80668014;
extern const float lbl_eu_80668018;
extern const float lbl_eu_8066801C;
extern const double lbl_eu_80668020;
// 6-byte item-box slot tables (pair of u32+u16 .sdata2 constants).
extern const u32 lbl_eu_8066804C;
extern const u16 lbl_eu_80668050;
extern const u32 lbl_eu_80668054;
extern const u16 lbl_eu_80668058;
extern const u32 lbl_eu_8066805C;
extern const u16 lbl_eu_80668060;
extern const u32 lbl_eu_80668074;
extern const u16 lbl_eu_80668078;
extern const u32 lbl_eu_8066807C;
extern const u16 lbl_eu_80668080;
extern "C" u8 getLanguage__9CDeviceSCFv();
extern void* lbl_eu_80664110;
extern void* lbl_eu_80664090;

void __declspec(noinline) resetCItemBox() {}

// --- CItemBoxInfo methods ---

u8 getItemBoxState(CItemBoxInfo* self) {
    return self->state.current;
}

u8 getItemBox2State(CItemBoxInfo2* self) {
    return self->state.current;
}

void advanceItemBoxState(CItemBoxInfo* info) {
    if (info->state.state == 3) {
        info->state.state = 4;
        info->state.visible = 0;
    }
}

void tryActivateItemBox(CItemBoxInfo* info) {
    if (info->state.layout == 0) return;
    if (info->state.resource != 0) {
        info->state.current = 1;
        info->state.active = 1;
    } else {
        return;
    }
}

u32 func_801D4AB0(void* arg) {
    u32 v = *(u32*)arg;
    u32 result = 0;
    if (((v >> 16) & 0xF) == 9) {
        u32 flags = ((u8*)arg)[7];
        if ((flags & 3) == 3u) {
            result = 1;
        }
    }
    return result;
}

#pragma push
#pragma optimize_for_size on
void func_801D4AE0(CItemBoxInfo* info, char* arg1, char* arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x151], arg1);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, arg2, 0);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
void func_801D4BDC(CItemBoxInfo* info, u8 arg2, u8 arg3) {
    if (info->state.layout != 0) {
        func_80136910((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x17d], arg2);
        func_80136910((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x188], arg3);
    }
}
#pragma pop

// --- Remaining CItemBoxInfo stubs ---

extern "C" void func_801D8C0C(CItemBoxInfo*);
extern "C" void func_801E4194(CItemBoxInfo2*);

void func_801D421C(CItemBoxInfo* info) {
    if (info->state.state != 0) return;
    info->state.state = 1;
    info->state.visible = 0;
    // Retail tail-calls func_801D8C0C (b, not bl) - dont_inline pragma keeps it a call.
    func_801D8C0C(info);
}

void func_801D4154(CItemBoxInfo* info, nw4r::lyt::DrawInfo* drawInfo) {
    if (info->state.active != 0) {
        func_80137038((nw4r::lyt::Layout*)info->state.layout, drawInfo, 0, 1);
    }
}

void func_80127BD8(void*, float*);

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801D4A2C(void* sub) {
    float f = lbl_eu_8066800C;
    for (u8 i = 0; i < 12; i++) {
        ((s16*)sub)[i] = 0;
        float temp[3] = {f, f, f};
        copyVEC3((u8*)sub + i * 0xC + 0x18, temp);
    }
}
#pragma pop


// Retail func_801D4054 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge). The setHandleFlag1 call reuses the readFile result in r3.
#pragma push
#pragma optimize_for_size on
void func_801D4054(CItemBoxInfo* info) {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    CFileHandle* fh1 = CDeviceFile::readFile(handle, &lbl_eu_805063BC[0x8e], reinterpret_cast<IWorkEvent*>(info), 0, 0);
    info->state.fileHandle1 = fh1;
    CDeviceFile::setHandleFlag1(fh1);
    handle = mtl::MemManager::getHandleMEM2();
    CFileHandle* fh2 = CDeviceFile::readFile(handle, &lbl_eu_805063BC[0xa6], reinterpret_cast<IWorkEvent*>(info), 0, 0);
    info->state.fileHandle2 = fh2;
    CDeviceFile::setHandleFlag1(fh2);
}
#pragma pop
// Retail func_801D4174 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge): r30 holds `this` and r31 the zero constant for the four
// post-call pointer clears, so both survive the func_801390E0/80139124 calls.
#pragma push
#pragma optimize_for_size on
void func_801D4174(CItemBoxInfo* info) {
    func_801390E0(&info->state.fileHandle1);
    func_801390E0(&info->state.fileHandle2);
    void* layout = info->state.layout;
    info->state.active = 0;
    if (layout != 0) {
        if (layout != 0) {
            // Retail calls the deleting destructor through vtable slot 2;
            // real virtual dispatch reproduces the r12/CTR load pattern.
            reinterpret_cast<CItemBoxLayoutDtorVt*>(layout)->destroy(1);
        }
        info->state.layout = 0;
    }
    info->state.animTransform1 = 0;
    info->state.animTransform2 = 0;
    func_80139124(info->state.arcResourceAccessor);
    func_80139124(info->state.resource);
    info->state.arcResourceAccessor = 0;
    info->state.resource = 0;
    info->state.memRegion1.func_8045F778();
    info->state.memRegion2.func_8045F778();
}
#pragma pop



// Retail func_801D4B3C uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) for arg0/child/first-result in r29/r30/r31.
#pragma push
#pragma optimize_for_size on
void func_801D4B3C(u8* arg0, CItemBoxInfo* info, u32 arg2) {
    char buf[0x20];
    // Declare `first` before `child`: low-degree webs color in reverse birth
    // order, so child (declared later) colors first into info's dead r30 and
    // first reuses the pool base's r31 (matches retail r29/r30/r31 layout).
    nw4r::lyt::Pane* first;
    nw4r::lyt::Pane* second;
    nw4r::lyt::Pane* child;
    char* base = lbl_eu_805063BC;
    sprintf(buf, base + 0x161, arg2 + 1);
    child = (nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10);
    first = child->FindPaneByName(base + 0x16e, true);
    second = child->FindPaneByName(buf, true);
    func_80137924((nw4r::math::VEC3*)arg0, second, first, child);
}
#pragma pop

void func_801D4C3C(CItemBoxInfo* info, void* arg2) {
    if (info->state.layout == 0) return;
    nw4r::lyt::Pane* child = (nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10);
    nw4r::lyt::Pane* result = child->FindPaneByName((char*)&lbl_eu_805063BC[0x193], true);
    func_80124270(result, (u32)arg2);
}

#pragma push
#pragma auto_inline off
extern "C" void func_801D4C9C(CItemBoxInfo* info) {
    float arg = lbl_eu_80668010;
    if (func_80137444((nw4r::lyt::AnimTransform*)info->state.animTransform1, arg) != 0) {
        ((nw4r::lyt::Layout*)info->state.layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)info->state.animTransform1, false);
        ((nw4r::lyt::Layout*)info->state.layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)info->state.animTransform2, true);
        info->state.state = 2;
    }
}

extern "C" void func_801D4D18(CItemBoxInfo* info) {
    if (func_80137444((nw4r::lyt::AnimTransform*)info->state.animTransform2, lbl_eu_80668010) != 0) {
        info->state.state = 3;
        info->state.visible = 1;
    }
}

extern "C" void func_801D4D64(CItemBoxInfo* info) {
    if (func_80137510((nw4r::lyt::AnimTransform*)info->state.animTransform2, lbl_eu_80668010) != 0) {
        ((nw4r::lyt::Layout*)info->state.layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)info->state.animTransform2, false);
        ((nw4r::lyt::Layout*)info->state.layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)info->state.animTransform1, true);
        info->state.state = 5;
    }
}

extern "C" void func_801D4DE0(CItemBoxInfo* info) {
    if (func_80137510((nw4r::lyt::AnimTransform*)info->state.animTransform1, -0.0f) != 0) {
        info->state.visible = 1;
        info->state.state = 0;
    }
}
#pragma pop

// Retail func_801D4E2C uses the _savegpr_20 + f29/f30/f31 frame (MWCC
// optimize_for_size prologue merge) and keeps the mtctr copy loop for the
// 0x34-byte record copy (li r0,6 + 4-byte tail). Per-slot banks: +0x16 holds
// a name-resolved flag (forced for slot 0xB when cat == 0x4C), +0x1D/+0x24/
// +0x2B hold {1,2,0} comparisons of the looked-up item's table values against
// the candidate item's.
#pragma push
#pragma optimize_for_size on
extern "C" void func_801D4E2C(void* out, void* member, void* arg3) {
    // Scratch doubles for MWCC's int->float magic (2^52 unsigned repair,
    // 2^63 sign-repair); retail materializes both high words up front.
    CItemBoxCompRecord rec;
    void* global = lbl_eu_806640F4;
    func_801392E4((u32)arg3);
    // v2 is a narrow-typed local: MWCC keeps it zero-extended and re-applies
    // the u16->u32 promotion per call site (retail clrlwi r5,r29,16 each
    // time, no shared masked temp).
    u16 v2 = func_80139358((u32)arg3);
    char* base = (char*)&lbl_eu_805063BC;
    rec.s2C =(u16)func_80136254((const void*)global, (const void*)(base + 0x19c), v2);
    rec.s2E = (u16)func_80136254((const void*)global, (const void*)(base + 0x1a4), v2);
    rec.s30 = (u8)func_801361E8((u32)global, base + 0x1ab, v2);
    rec.s32 = (u8)func_801361E8((u32)global, base + 0x1b3, v2);
    rec.s34 = (u8)func_801361E8((u32)global, base + 0x1bb, v2);
    rec.f38 = (f32)((u8)func_801361E8((u32)global, base + 0x1c3, v2) / lbl_eu_80668014);
    rec.s3C = (u8)func_801361E8((u32)global, base + 0x1c9, v2);
    if (func_801361E8((u32)global, base + 0x1d2, v2) & 4) {
        void* lookup = func_8009EC9C(1);
        u32 r = func_800A082C(lookup);
        rec.s2C = (s16)(lbl_eu_80668018 * (f32)(rec.s2C * (u16)r));
        u32 r2 = func_800A082C(lookup);
        rec.s2E = (s16)(lbl_eu_8066801C * (f32)(rec.s2E * (u16)r2));
        if (rec.s2C >= 999) rec.s2C = 999;
        if (rec.s2E >= 999) rec.s2E = 999;
    }
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        // slot stays raw and re-masks per use, matching retail r21.
        u32 slot = func_801392B4(i);
        char buf[0x20];
        sprintf(buf, base + 0x1d7, (u8)slot);
        u16 flag = func_801361E8((u32)global, buf, v2);
        // name-flag bank (+0x16): set when the slot tag resolves; slot 0xB
        // is forced on for candidate category 0x4C.
        rec._16[i] = (flag != 0);
        if ((u8)slot == 0xb && v2 == 0x4c) rec._16[i] = 1;
        if (rec._16[i] == 0) continue;
        rec.f38 += func_8013B380((u8)slot);
        void* lookup = func_8009EC9C((u8)slot);
        void* item = func_80157C4C(2, *(s16*)((u8*)lookup + 0x26));
        if (item == NULL || *(u32*)item == 0) continue;
        // cat is likewise narrow-typed for per-site promotion.
        u16 cat = func_80139358(*(u32*)item >> 20);
        u16 a = (u16)func_80136254((const void*)global, (const void*)(base + 0x19c), cat);
        u16 b = (u16)func_80136254((const void*)global, (const void*)(base + 0x1a4), cat);
        f32 ratio = (f32)((u8)func_801361E8((u32)global, base + 0x1c3, cat) / lbl_eu_80668014);
        rec.f28 = func_80139C98((u32)a, (u32)b, 0, ratio);
        u8 v1ab = (u8)func_801361E8((u32)global, base + 0x1ab, cat);
        u8 v1b3 = (u8)func_801361E8((u32)global, base + 0x1b3, cat);
        if (rec.f28 > ratio) rec.flags1[i] = 1;
        else if (rec.f28 < ratio) rec.flags1[i] = 2;
        else rec.flags1[i] = 0;
        if (rec.s30 > v1ab) rec.flags2[i] = 1;
        else if (rec.s30 < v1ab) rec.flags2[i] = 2;
        else rec.flags2[i] = 0;
        if (rec.s32 > v1b3) rec.flags3[i] = 1;
        else if (rec.s32 < v1b3) rec.flags3[i] = 2;
        else rec.flags3[i] = 0;
    }
    // 0x34-byte copy: pair-copy with s[1]/s[2] accesses reproduces the retail
    // mtctr lwzu/stwu 8-byte-pair loop (li r0,6 + 4-byte tail).
    {
        u32* s = (u32*)&rec - 1;
        u32* d = (u32*)out - 1;
        for (u32 k = 0; k < 6; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
// Retail keeps the stmw/lmw frame (optimize_for_size prologue merge) and the
// mtctr pair-copy loop for the 0x1C-byte record (li r0,3 + 4-byte tail). The
// null path memsets the record, copies, and returns early; the filled path
// falls through to its own copy of the same loop. Per-slot flag banks are
// addressed through one walked pointer (&flags[i], +0/+7/+14).
extern "C" void func_801D5274(void* out, void* arg2, void* arg3) {
    CItemBoxSlotFlags rec;
    if (arg3 == NULL) {
        memset(&rec, 0, 0x1C);
        // Pair-copy: reproduces the retail inlined mtctr lwzu/stwu
        // 8-byte-pair loop (li r0,3 + 4-byte tail).
        {
            u32* d = (u32*)out - 1;
            u32* s = (u32*)&rec - 1;
            for (u32 k = 0; k < 3; k++) {
                d[1] = s[1];
                d[2] = s[2];
                s += 2;
                d += 2;
            }
            d[1] = s[1];
        }
        return;
    }
    void* global = lbl_eu_806640F8;
    u32 v1 = func_801392E4((u32)arg3);
    u32 v2 = func_80139358((u32)arg3);
    char* base = (char*)&lbl_eu_805063BC;
    rec.v[0] = (u8)func_801361E8((u32)global, base + 0x1ab, (u16)v2);
    rec.v[1] = (u8)func_801361E8((u32)global, base + 0x1b3, (u16)v2);
    rec.v[2] = (u8)func_801361E8((u32)global, base + 0x1e2, (u16)v2);
    u8 v3 = (u8)func_801361E8((u32)global, base + 0x1eb, v2);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 slot = (u8)func_801392B4((u8)i);
        char buf[0x20];
        sprintf(buf, base + 0x1f4, (u8)slot);
        u8 flag = (u8)(func_801361E8((u32)global, buf, (u16)v2) != 0);
        u8* banks = &rec.flags[(u8)i];
        banks[0] = flag;
        if (flag == 0) continue;
        void* lookup = func_8009EC9C(slot);
        if (v3 == 3) {
            if (func_8026178C((u8*)lookup + 0x3534, 0x85) == 0) banks[0] = 0;
        } else if (v3 == 2) {
            if (func_8026178C((u8*)lookup + 0x3534, 0x84) == 0) banks[0] = 0;
        } else {
            s16 value = -1;
            switch ((u16)v1) {
                case 4: value = *(s16*)((u8*)lookup + 0x1C); break;
                case 5: value = *(s16*)((u8*)lookup + 0x1E); break;
                case 6: value = *(s16*)((u8*)lookup + 0x20); break;
                case 7: value = *(s16*)((u8*)lookup + 0x22); break;
                case 8: value = *(s16*)((u8*)lookup + 0x24); break;
            }
            if (value >= 0) {
                void* item = func_80157C4C((u16)v1, value);
                if (item != NULL && *(u32*)item != 0) {
                    u16 cat = (u16)func_80139358(*(u32*)item >> 20);
                    u8 n1 = (u8)func_801361E8((u32)global, base + 0x1ab, cat);
                    u8 n2 = (u8)func_801361E8((u32)global, base + 0x1b3, cat);
                    if (rec.v[0] > n1) banks[7] = 1;
                    else if (rec.v[0] < n1) banks[7] = 2;
                    else banks[7] = 0;
                    if (rec.v[1] > n2) banks[14] = 1;
                    else if (rec.v[1] < n2) banks[14] = 2;
                    else banks[14] = 0;
                }
            }
        }
    }
    // Same pair-copy at the end of the filled path.
    {
        u32* d = (u32*)out - 1;
        u32* s = (u32*)&rec - 1;
        for (u32 k = 0; k < 3; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
}
#pragma pop
// Retail func_801D5564 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) and keeps the mtctr copy loop for the 0xA4-byte record
// copy (li r0,0x14 + 4-byte tail). The name/category tables are resolved
// through the item vtable (arg3) or the raw data pointers (arg3 == NULL);
// the text buffer is then scanned for '$' markers ('$1' switches on the
// equip-state byte, '$2' copies the static label-pointer table at
// lbl_eu_8050634C and formats the count-indexed label); other bytes advance
// Shift-JIS-aware.
#pragma push
#pragma optimize_for_size on
void __declspec(noinline) func_801D5564(void* out, void* unused, void* data, void* arg3) {
    u32 cat;
    u32 count;
    void* item = arg3 != NULL ? arg3 : NULL;
    // Retail hoists this global into a register (r26) across all calls.
    void* gd8 = lbl_eu_806640D8;
    if (arg3 == NULL) {
        func_801392E4((u32)data);
    }
    if (arg3 != NULL) {
        CItemImplVt54* inst = (CItemImplVt54*)CItem_initItemImplInstances(item);
        cat = inst->_v54(item);
    } else {
        // Retail keeps the raw result live in r28 (no early narrowing).
        cat = ((u32 (*)(u32))func_80139358)((u32)data);
    }
    if (arg3 != NULL) {
        CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
        count = (u16)inst->_v08(item);
    } else {
        count = func_801361E8((u32)lbl_eu_806640EC, &lbl_eu_805063BC[0x1f9], (u32)data);
    }
    char* base = (char*)&lbl_eu_805063BC;
    CItemBoxNameRecord rec;
    rec.count = (u8)count;
    rec.str = (u32)func_80136190(base + 0x130, base + 0x139, 0x1e - ((u8)count - 1));
    char* s2 = func_80136190(base + 0x202, base + 0x139, 0xf);
    if (arg3 != NULL) {
        CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
        sprintf(rec.name, base, inst->_v90(item));
    } else {
        sprintf(rec.name, base + 0x18, s2);
    }
    rec.e0 = (u8)func_801361E8((u32)gd8, base + 0x3, (u16)cat);
    rec.e1 = (u8)func_801361E8((u32)gd8, base + 0x20b, (u16)cat);
    rec.color = 0xFFFFFFFF;
    switch (rec.e1) {
        case 4: rec.color = 0xFF00FFFF; break;
        case 5: rec.color = 0x0000FFFF; break;
        case 6: rec.color = 0xFFFFFFFF; break;
        case 7: rec.color = 0x0000FFFF; break;
        case 8: rec.color = 0x00FFFFFF; break;
        case 9: rec.color = 0x775544FF; break;
    }
    sprintf(rec.text, base + 0x18,
            ((char* (*)(void*, const char*, u16))&func_8013639C)(gd8, base + 0xc, (u16)cat));
    u32 wide = 1;
    if (getLanguage__9CDeviceSCFv() != 3) {
        if (getLanguage__9CDeviceSCFv() != 2) {
            wide = 0;
        }
    }
    CItemBoxLabelTable tbl;
    char buf28[0x20];
    char buf48[0x20];
    char* cur = rec.text;
    while (*cur != 0) {
        if (*cur == '$') {
            memset(buf28, 0, 0x20);
            cur++;
            if (*cur == '1') {
                cur++;
                switch (rec.e0) {
                    case 1:
                        sprintf(buf28, base + 0x1b, rec.name);
                        break;
                    case 2:
                        sprintf(buf28, base + 0x1b, rec.name);
                        break;
                    case 3:
                        if (wide != 0) {
                            sprintf(buf28, base + 0x30, rec.name);
                        } else {
                            sprintf(buf28, base + 0x48, rec.name);
                        }
                        break;
                    case 4:
                        if (wide != 0) {
                            sprintf(buf28, base + 0x30, rec.name);
                        } else {
                            sprintf(buf28, base + 0x48, rec.name);
                        }
                        break;
                    default:
                        break;
                }
            } else if (*cur == '2') {
                // 0x1C-byte move of the static label-pointer table into the
                // stack copy (retail mtctr 3-pair loop + 4-byte tail).
                {
                    u32* s = (u32*)&lbl_eu_8050634C - 1;
                    u32* d = (u32*)&tbl - 1;
                    for (u32 k = 0; k < 3; k++) {
                        d[1] = s[1];
                        d[2] = s[2];
                        s += 2;
                        d += 2;
                    }
                    d[1] = s[1];
                }
                cur++;
                u32 name = func_801361E8((u32)gd8, tbl.labels[(u8)count], (u16)cat);
                if (wide != 0) {
                    sprintf(buf28, base + 0x5f, (u8)name);
                } else {
                    sprintf(buf28, base + 0x77, (u8)name);
                }
            }
            // strlen of buf28: retail's counter runs one past the terminator
            // (len+1), folded back out by the len-1 splice offset below.
            u32 len = 1;
            while (buf28[len - 1] != 0) {
                len++;
            }
            sprintf(buf48, base + 0x18, cur);
            cur -= 2;
            sprintf(cur, base + 0x18, buf28);
            cur = cur + len - 1;
            sprintf(cur, base + 0x18, buf48);
        } else {
            s8 sc = (s8)*cur;
            if ((sc >= 0x81 && sc <= 0x9f) || (sc >= 0xe0 && sc <= 0xef)) {
                cur += 2;
            } else {
                cur += 1;
            }
        }
    }
    // 0xA4-byte copy: pair-copy with s[1]/s[2] accesses reproduces the retail
    // mtctr lwzu/stwu 8-byte-pair loop (li r0,0x14 + 4-byte tail).
    {
        u32* s = (u32*)&rec - 1;
        u32* d = (u32*)out - 1;
        for (u32 k = 0; k < 0x14; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// Retail keeps the lookup-table pointer live in r29 across all calls and
// spills the four results to stack slots, reloading them for the final four
// output-word stores. dont_inline keeps IPA from folding this body into
// func_801D8058/func_801E3730, which lets them keep both stack arrays.
void func_801D59C0(u32* out, void* unused, void* arg2) {
    CItemBoxLabelRec r;
    void* tbl = lbl_eu_80664104;
    func_801392E4((u32)arg2);
    // cat stays a raw u16 in a register; each call site re-applies the
    // u16->u32 argument conversion (retail clrlwi per site, no shared temp).
    u16 cat = func_80139358((u32)arg2);
    // base is anchored at the first lookup so its definition schedules after
    // the cat move (retail order: or r30,r3 then lis/addi).
    r.a = (u8)func_801361E8((u32)tbl, (char*)&lbl_eu_805063BC + 0x214, cat);
    char* base = (char*)&lbl_eu_805063BC;
    r.b = func_80136190(base + 0x219, base + 0x139, r.a);
    // Residual: retail narrows this result into r5, we pick r0 - a
    // scheduler-driven compare-temp allocation choice insensitive to every
    // source shape tried (named local, raw+cast, assign-in-cond, & 0xFF).
    r.c = (u8)func_801361E8((u32)tbl, base + 0x225, cat);
    if (r.c == 0x1A) {
        r.d = func_80136190(base + 0x219, base + 0x139, 0x14);
    } else {
        r.d = func_8013639C(lbl_eu_806640A8, base + 0x139);
    }
    // Word-wise copy: retail expands the record copy as four lwz/stw pairs.
    u32* src = (u32*)&r;
    out[0] = src[0];
    out[1] = src[1];
    out[2] = src[2];
    out[3] = src[3];
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801D5AA0(CItemBoxInfo* out, void* unused, void* data) {
    void* global = lbl_eu_80664110;
    u16 v1 = func_801392E4((u32)data);
    u16 v2 = func_80139358((u32)data);
    u8 arr[8];
    arr[0] = (u8)func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x22b], v2);
    // count stays u32: retail keeps the raw call result (r28) and masks at
    // each (u8)count use (loop mask hoisted to r29).
    u32 count = func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x237], v2);
    for (u32 i = 0; i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 a = (u8)func_801392B4((u8)i);
        arr[(u8)i + 1] = (a == (u8)count) ? 1 : 0;
    }
    u32 key = func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x23f], v2);
    u32 val = func_8013600C((char*)&lbl_eu_805063BC[0x248], (char*)&lbl_eu_805063BC[0x250], (u8)key);
    void* lookup = func_8009EC9C((u8)count);
    u8 cat = (u8)func_800A32BC();
    u8* entry = (u8*)lookup + cat * 0x49 + (u8)val * 2;
    u8 flag2 = 0;
    switch (arr[0]) {
        case 1: if (entry[0xE8] != 0) flag2 = 1; break;
        case 2: if ((entry[0xE9] >> 0) & 1) flag2 = 1; break;
        case 3: if ((entry[0xE9] >> 1) & 1) flag2 = 1; break;
    }
    *(u32*)((u8*)out + 0) = *(u32*)arr;
    *(u32*)((u8*)out + 4) = *(u32*)(arr + 4);
    ((u8*)out)[8] = flag2;
}
#pragma pop
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801D5C38(void* out, void* unused, void* data, void* arg3) {
    void* p;
    if (arg3 != NULL)
        p = arg3;
    else
        p = NULL;
    func_801392E4((u32)data);
    func_80139358((u32)data);
    CItemBoxSlotRecord1 rec;
    // Chained call + virtual dispatch: no named intermediate keeps the result
    // web dead after each use, matching retail's direct r3 consumption.
    u32 v08 = ((CItemImplVt*)CItem_initItemImplInstances(p))->_v08(p);
    rec.count = (u8)v08;
    char* base = lbl_eu_805063BC;
    rec.str = (u32)func_80136190(&base[0x130], &base[0x139], 0x1e - ((u8)v08 - 1));
    rec.counter = 0;
    for (u32 i = 0; i < 4; i++) {
        u32 n = ((CItemImplVt*)CItem_initItemImplInstances(p))->_v4C(p, (u8)i);
        // Signed >0 test: folds into the record-form clrlwi as retail's ble.
        if ((s32)n > 0) {
            // Retail passes the pair count n as a third arg to func_8013639C.
            rec.text[rec.counter] =
                (u32)((char*(*)(void*, char*, u32))&func_8013639C)(lbl_eu_806640D8, &base[0x139], n);
            rec.vals[rec.counter] = ((CItemImplVt*)CItem_initItemImplInstances(p))->_v64(p, (u8)i);
            rec.counts[rec.counter] = n;
            rec.counter++;
        }
    }
    // 0x2C-byte copy: retail mtctr 8-byte-pair loop (li r0,5 + 4-byte tail).
    {
        u32* s = (u32*)&rec - 1;
        u32* d = (u32*)out - 1;
        for (u32 k = 0; k < 5; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
}
#pragma pop
// Retail func_801D5DA4: item-box renderer for selection kind 2. Builds the
// 0x34 comparison record (func_801D4E2C), formats three stat panes from it
// through an ml::FixStr<32>, resets the highlight panes, then per visible
// slot colours the row pane and stamps the candidate-item texture.
#pragma push
#pragma optimize_for_size on
void func_801D5DA4(CItemBoxInfo* info, u16 arg2, void* arg3, u32 arg4) {
    func_801D8318(info);
    // Declaration order follows the retail frame layout (high -> low):
    // staged colour temps > copied record > FixStr storage > source record >
    // pane-name buffers.
    CItemBoxColorBlock selB;
    CItemBoxColorBlock selA;
    CItemBoxCompRecord local;
    // Layout-compatible POD stand-in for ml::FixStr<32>: the format() calls
    // cast its address inline so no FixStr pointer web stays live across the
    // lookup calls (retail touches the buffer purely through sp offsets).
    struct {
        char mString[0x20];
        u32 mLength;
    } text;
    CItemBoxCompRecord record;
    char paneName[0x20];
    char label[0x20];
    func_801D4E2C(&record, info, (void*)(u32)arg2);
    // 0x34-byte copy: load both words of each pair before storing so MWCC
    // keeps the retail lwz/lwzu + stw/stwu pair-loop shape (li r0,6 + tail).
    {
        u32* d = (u32*)&local - 1;
        u32* s = (u32*)&record - 1;
        for (u32 k = 0; k < 6; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
    char* base = (char*)&lbl_eu_805063BC;
    // Raw-storage binding: retail inlines the bool-ctor clear as bare stb/stw
    // stores; format() is invoked through a cast so no reference web occupies
    // a register.
    text.mString[0] = 0;
    text.mLength = 0;
    // NOTE (open item): spelling the label-pool base inline at every use
    // re-anchors it MORE than retail (which uses r19/r19/r28) and regressed
    // the diff; one cached base pointer is the closest shape tried so far.
    char* lbl = func_80136190(&base[0x130], &base[0x139], 0xb);
    ((ml::FixStr<32>*)&text)->format(&base[0x254], local.s2C, lbl, local.s2E);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &base[0x25b], text.mString, 0);
    func_80136910((nw4r::lyt::Layout*)info->state.layout, &base[0x267], (u8)local.s30);
    func_80136910((nw4r::lyt::Layout*)info->state.layout, &base[0x273], (u8)local.s32);
    char* str7f = func_80136190(&base[0x130], &base[0x139], 0x7f);
    char* str80 = func_80136190(&base[0x130], &base[0x139], 0x80);
    if (local.s34 != 0) {
        ((ml::FixStr<32>*)&text)->format(&base[0x27f], str7f, str80, local.s34);
    } else {
        ((ml::FixStr<32>*)&text)->format(&base[0x13e], local.s34, str80);
    }
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &base[0x286], text.mString, 0);
    str7f = func_80136190(&base[0x130], &base[0x139], 0x7f);
    str80 = func_80136190(&base[0x130], &base[0x139], 0x80);
    if (local.s3C != 0) {
        ((ml::FixStr<32>*)&text)->format(&base[0x27f], str7f, str80, local.s3C);
    } else {
        ((ml::FixStr<32>*)&text)->format(&base[0x13e], local.s3C, str80);
    }
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &base[0x292], text.mString, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &base[0x29e], &base[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &base[0x2ab], &base[0x2aa], 0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &base[0x2b6], &lbl_eu_80664598, &lbl_eu_806645A0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &base[0x2c1], &lbl_eu_80664598, &lbl_eu_806645A0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &base[0x2cc], &lbl_eu_80664598, &lbl_eu_806645A0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &base[0x2d7], &lbl_eu_80664598, &lbl_eu_806645A0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &base[0x2e2], &lbl_eu_80664598, &lbl_eu_806645A0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &base[0x2ed], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &base[0x2f8], &lbl_eu_806645A8, &lbl_eu_806645B0);
    u32 playerIdx = (arg4 >> 16) & 0xFF;
    // Hoisted halves: MWCC materializes these once (lis) and rebuilds each
    // constant at its use site with a single addi.
    u32 darkBase = 0x77770000;
    u32 tagHi = 0x74690000;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = local._16[(u8)i];
        u32 color = darkBase + 0x77ff;  // 0x777777ff
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(paneName, base + 0x303, (u8)i + 1);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, paneName, color);
        // Default vertex colours carry data only in the alpha channel
        // (info+0xA2 / info+0xAA); the selected-slot pair comes from
        // info+0x9C/0xA4 when the slot is active.
        s16 vA2 = *(s16*)((u8*)info + 0xA2);
        s16 vAA = *(s16*)((u8*)info + 0xAA);
        // Loop-scope slots descend: args (sp+0x18) > defA2 (sp+0x10) >
        // defAA (sp+0x08); POD types keep the builds as bare halfword stores.
        u32 args[4];
        CItemBoxColorRaw defA2;
        CItemBoxColorRaw defAA;
        defA2.b = 0;
        defA2.g = 0;
        defA2.r = 0;
        defA2.a = vA2;
        defAA.b = 0;
        defAA.g = 0;
        defAA.r = 0;
        defAA.a = vAA;
        args[0] = 0;
        args[1] = 0;
        args[2] = 0;
        args[3] = 0;
        // Selected colour is staged through the named temp then reloaded into
        // the sprintf argument block (retail stores to sp+0xf8/sp+0x100 first).
        if (flag != 0) {
            selA.w[0] = *(u32*)((u8*)info + 0x9C);
            selA.w[1] = *(u32*)((u8*)info + 0xA0);
        } else {
            selA.w[0] = *(u32*)&defA2.r;
            selA.w[1] = *(u32*)&defA2.a;
        }
        args[0] = selA.w[0];
        args[1] = selA.w[1];
        if (flag != 0) {
            selB.w[0] = *(u32*)((u8*)info + 0xA4);
            selB.w[1] = *(u32*)((u8*)info + 0xA8);
        } else {
            selB.w[0] = *(u32*)&defAA.r;
            selB.w[1] = *(u32*)&defAA.a;
        }
        args[2] = selB.w[0];
        args[3] = selB.w[1];
        sprintf(paneName, base + 0x161, (u8)i + 1, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane =
            ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(paneName, true);
        if (pane != NULL) {
            void* obj = ((CItemBoxPaneVt*)pane)->getColorObject();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(obj, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        if (flag != 0 && arg3 != NULL) {
            u8 slot = (u8)func_801392B4((u8)i);
            if (slot != 0) {
                void* member = func_8009EC9C(slot);
                // Fixed equip category 2: candidate value read at member+0x26.
                void* item = func_80157C4C(2, *(s16*)((u8*)member + 0x26));
                if (item != NULL && *(u32*)item != 0 && arg3 == item) {
                    sprintf(label, base + 0x30e, (u8)i + 1);
                    u32 tex;
                    if (slot == (u8)func_801392B4(playerIdx)) {
                        tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)
                                  ->findName(tagHi + 0x6d67, (u32)(base + 0x319), 0);
                    } else {
                        tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)
                                  ->findName(tagHi + 0x6d67, (u32)(base + 0x32d), 0);
                    }
                    if (tex == 0) {
                        tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)
                                  ->findName(tagHi + 0x6d67, (u32)(base + 0x341), 0);
                    }
                    if (tex != 0) {
                        func_80137E7C((nw4r::lyt::Layout*)info->state.layout, label, tex);
                    }
                }
            }
        }
    }
    func_801D69FC(info, arg2, arg3);
}
#pragma pop

void __declspec(noinline) func_801D62F8(void* arr, u32 index, const void* color) {
    if (index >= 3) {
        Panic__Q24nw4r2dbFPCciPCce(lbl_eu_8052E590, 0x8f, lbl_eu_8052E568);
    }
    __as__11_GXColorS10FRC11_GXColorS10((u8*)arr + index * 8 + 0x10, color);
}
void func_801D3FF0(void*);

void func_801D40C4(CItemBoxInfo* info) {
    if (info->state.active != 0) {
        switch (info->state.state) {
            case 1: func_801D4C9C(info); break;
            case 2: func_801D4D18(info); break;
            case 4: func_801D4D64(info); break;
            case 5: func_801D4DE0(info); break;
        }
        ((nw4r::lyt::Layout*)info->state.layout)->Animate(0);
    }
}

void func_801E1348(CItemBoxInfo2* info) {
    if (info->state.active != 0) {
        switch (info->state.state) {
            case 1: func_801E17EC(info); break;
            case 2: func_801E1868(info); break;
            case 4: func_801E18B4(info); break;
            case 5: func_801E1930(info); break;
        }
        ((nw4r::lyt::Layout*)info->state.layout)->Animate(0);
    }
}

// Retail func_801D6394: item-box detail renderer. Builds a per-slot comparison
// record for the candidate item (func_801D5274), lays out the label/stat
// panes, then per-slot colours the row panes and stamps the item name onto
// the slot whose record matches the candidate.
void func_801D6394(CItemBoxInfo* info, u32 itemId, void* record, u32 arg4) {
    // Locals are declared in retail stack-slot order (low -> high addresses):
    // colour pair, vertex-colour pair, comparison record, pane-name buffers,
    // record copy, per-colour word temporaries.
    CItemBoxColorPOD cA;
    CItemBoxColorPOD cB;
    CItemBoxColorPOD quadCol[2];
    CItemBoxSlotFlags rec;
    char buf2[0x20];
    char buf[0x20];
    CItemBoxSlotFlagsAny recAny;
    // per-slot comparison record for the candidate item, built by
    // func_801D5274 and block-copied to a second local.
    func_801D85D8(info);
    func_801D5274(&rec, info, (void*)itemId);
    {
        u32* s = (u32*)&rec - 1;
        u32* d = (u32*)&recAny.rec - 1;
        for (u32 k = 0; k < 3; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
    char* base = lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x25b, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x286, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x292, base + 0x2aa, 0);
    func_80136910((nw4r::lyt::Layout*)info->state.layout, base + 0x267, recAny.rec.v[0]);
    func_80136910((nw4r::lyt::Layout*)info->state.layout, base + 0x273, recAny.rec.v[1]);
    func_80136910((nw4r::lyt::Layout*)info->state.layout, base + 0x29e, recAny.rec.v[2]);
    u32 v1 = func_801361E8((u32)lbl_eu_806640F8, base + 0x1eb, (u16)func_80139358(itemId));
    char* s1;
    switch ((u8)v1) {
        case 3: s1 = func_80136190(base + 0x130, base + 0x139, 0x32); break;
        case 2: s1 = func_80136190(base + 0x130, base + 0x139, 0x31); break;
        case 1: s1 = func_80136190(base + 0x130, base + 0x139, 0x30); break;
        case 4: case 5: case 6: case 7: case 8:
        case 9: case 10: case 11: case 12: case 13:
            s1 = func_80136190(base + 0x130, base + 0x139, 0x2e); break;
        default: s1 = 0; break;
    }
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x354, s1, 0);
    char* s2;
    switch ((u8)v1) {
        case 4: s2 = func_80136190(base + 0x248, base + 0x139, 0x77); break;
        case 5: s2 = func_80136190(base + 0x248, base + 0x139, 0x78); break;
        case 6: s2 = func_80136190(base + 0x248, base + 0x139, 0x79); break;
        case 7: s2 = func_80136190(base + 0x248, base + 0x139, 0x7a); break;
        case 8: s2 = func_80136190(base + 0x248, base + 0x139, 0x7b); break;
        case 9: s2 = func_80136190(base + 0x248, base + 0x139, 0x7c); break;
        case 10: s2 = func_80136190(base + 0x248, base + 0x139, 0x7d); break;
        case 11: s2 = func_80136190(base + 0x248, base + 0x139, 0x7e); break;
        case 12: s2 = func_80136190(base + 0x248, base + 0x139, 0x7f); break;
    }
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x2ab, s2, 0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2c1, &lbl_eu_80664598, &lbl_eu_806645A0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2cc, &lbl_eu_80664598, &lbl_eu_806645A0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2d7, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2e2, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2ed, &lbl_eu_80664598, &lbl_eu_806645A0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2f8, &lbl_eu_80664598, &lbl_eu_806645A0);
    u32 cur = (arg4 >> 16) & 0xFF;
    u32 tag = 0x74696D67;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = recAny.rec.flags[(u8)i];
        u32 color = 0x777777FF;
        if (flag != 0) color = 0xFFFFFFFF;
        u32 idx = (u8)i + 1;
        sprintf(buf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, color);
        // two vertex colours: filled slots use the info stat words, empty ones
        // fall back to transparent-black quads built member-wise here.
        cB.r = 0; cB.g = 0; cB.b = 0; cB.a = *(s16*)((u8*)info + 0xA2);
        cA.r = 0; cA.g = 0; cA.b = 0; cA.a = *(s16*)((u8*)info + 0xAA);
        quadCol[0].r = 0; quadCol[0].g = 0; quadCol[0].b = 0; quadCol[0].a = 0;
        quadCol[1].r = 0; quadCol[1].g = 0; quadCol[1].b = 0; quadCol[1].a = 0;
        // filled slots take the raw stat word pair from info; empty slots fall
        // back to the (zero) RGB halves of the member-wise colour locals.
        u32 w0, w1, w2, w3;
        if (flag != 0) {
            w0 = *(u32*)((u8*)info + 0x9C);
            w1 = *(u32*)((u8*)info + 0xA0);
        } else {
            w0 = *(u32*)&cB.r;
            w1 = *(u32*)&cB.b;
        }
        *(u32*)&quadCol[0].r = w0;
        *(u32*)&quadCol[0].b = w1;
        if (flag != 0) {
            w2 = *(u32*)((u8*)info + 0xA4);
            w3 = *(u32*)((u8*)info + 0xA8);
        } else {
            w2 = *(u32*)&cA.r;
            w3 = *(u32*)&cA.b;
        }
        *(u32*)&quadCol[1].r = w2;
        *(u32*)&quadCol[1].b = w3;
        sprintf(buf2, base + 0x161, idx);
        nw4r::lyt::Pane* pane =
            ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* mat = ((CItemBoxPaneVt*)pane)->getColorObject();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(mat, (u8)j, (u8*)quadCol + (u8)j * 8);
            }
        }
        if (flag == 0 || record == NULL) continue;
        int slot = func_801392B4((u8)i);
        if ((u8)slot == 0) continue;
        void* lookup = func_8009EC9C(slot);
        u32 cat = func_801392E4((u32)itemId);
        s16 value = -1;
        switch (cat & 0xFFFF) {
            case 4: value = *(s16*)((u8*)lookup + 0x1C); break;
            case 5: value = *(s16*)((u8*)lookup + 0x1E); break;
            case 6: value = *(s16*)((u8*)lookup + 0x20); break;
            case 7: value = *(s16*)((u8*)lookup + 0x22); break;
            case 8: value = *(s16*)((u8*)lookup + 0x24); break;
        }
        void* r = func_80157C4C(cat & 0xFFFF, value);
        if (r == NULL || *(u32*)r == 0 || r != record) continue;
        sprintf(buf2, base + 0x30e, idx);
        int curSlot = func_801392B4(cur);
        u32 tex;
        if ((u8)slot == (u8)curSlot) {
            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(tag, (u32)(base + 0x319), 0);
        } else {
            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(tag, (u32)(base + 0x32d), 0);
        }
        if (tex == 0) {
            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(tag, (u32)(base + 0x341), 0);
        }
        if (tex != 0) {
            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, buf2, tex);
        }
    }
    func_801D69FC(info, itemId, record);
}
// Retail func_801D69FC: item-box slot renderer. With a candidate record it
// walks the three slot panes showing per-slot item info (regular items via
// the item table, equipped gear via the sub-record); with no record it shows
// the plain item-name list. Ends by stamping the per-slot data into info.
// Retail uses the stmw/lmw frame (MWCC optimize_for_size prologue merge) with
// same-TU helpers kept as real bl calls.
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801D69FC(CItemBoxInfo* info, u32 itemId, void* record) {
    ml::FixStr<32> text(true);
    func_801D885C(info);
    char* base = lbl_eu_805063BC;
    char buf[0x20];
    char buf2[0x20];
    record = record != NULL ? record : NULL;
    if (record != NULL && *(u32*)record != 0) {
        // Retail re-fetches the item interface at each use; it is never held
        // live across calls, so keep it scoped to avoid burning a
        // callee-saved register and shifting the whole allocation window.
        u16 count = (u16)((CItemImplVt*)CItem_initItemImplInstances(record))->_v30(record);
        u32 tag = 0x74696D67;
        for (u32 i = 0; i < 3; i++) {
            u32 idx = (u8)i + 1;
            sprintf(buf, base + 0x35f, idx);
            nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
            if ((u8)i < count) {
                func_80124270(pane, 1);
                s16 itemVal = 0;
                u8 itemCount = 0;
                s16 value = 0;
                u32 tex = 0;
                CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(record);
                s16 v40 = (s16)inst->_v40(record, (u8)i);
                if (v40 != -1) {
                    // regular item path: look the item up and format its name.
                    void* rec = func_80157C4C(3, v40);
                    u32 cat = *(u32*)rec >> 20;
                    CItemBoxNameRecord2 rec2;
                    func_801D5564(&rec2, info, (void*)cat, rec);
                    // Retail copies the 0xA4-byte record to a second local via
                    // the memcpy mtctr 8-byte-pair loop (li r0,0x14 + tail).
                    // Whole-struct assign: MWCC expands the POD copy to the
                    // retail mtctr 8-byte-pair loop.
                    CItemBoxNameRecord2 rec2b = rec2;
                    switch (rec2b.e1) {
                        case 0: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x36e), 0); break;
                        case 4: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x384), 0); break;
                        case 5: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x39a), 0); break;
                        case 6: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3b0), 0); break;
                        case 7: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3c6), 0); break;
                        case 8: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3dc), 0); break;
                        case 9: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3f2), 0); break;
                    }
                    if (rec2b.name[0] == '0') {
                        sprintf(buf, base + 0x408, idx);
                        func_80136B4C(info->state.layout, buf, base + 0x2aa, 0);
                    } else {
                        if (rec2b.e0 == 0) {
                            text.format(base + 0x41e, rec2b.name);
                        } else if ((u8)(rec2b.e0 - 3) <= 1) {
                            text.format(base + 0x419, rec2b.name, func_80136190(base + 0x130, base + 0x139, 0x21));
                        } else {
                            text.format(base + 0x41e, rec2b.name);
                        }
                        sprintf(buf, base + 0x408, idx);
                        func_80136B4C(info->state.layout, buf, text.c_str(), 0);
                    }
                    inst = (CItemImplVt*)CItem_initItemImplInstances(rec);
                    itemVal = inst->_v54(rec);
                    itemCount = (u8)inst->_v08(rec);
                    value = (s16)inst->_v90(rec);
                } else {
                    // equipped-gear path via the sub-record; no-item fallback.
                    CItemBoxSubRecord* sub = inst->_v2C(record, (u8)i);
                    if (sub == NULL || (sub->field_04 & 1) == 0) {
                        tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x36e), 0);
                        text.format(base + 0x18, func_80136190(base + 0x130, base + 0x139, 0x2a));
                    } else {
                        u16 equip = (u16)((sub->field_04 >> 16) & 0xFFF);
                        u32 w = sub->_00;
                        char* label = func_80136190(base + 0x130, base + 0x139, 0x1e - ((w >> 22 & 7) - 1));
                        u32 st = func_801361E8((u32)lbl_eu_806640D8, base + 0x20b, equip);
                        switch (st & 0xFF) {
                            case 0: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x36e), 0); break;
                            case 4: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x384), 0); break;
                            case 5: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x39a), 0); break;
                            case 6: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3b0), 0); break;
                            case 7: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3c6), 0); break;
                            case 8: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3dc), 0); break;
                            case 9: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3f2), 0); break;
                        }
                        s16 val = (s16)((w >> 11) & 0x7FF);
                        if (val != 0) {
                            u32 v2 = func_801361E8((u32)lbl_eu_806640D8, base + 0x3, equip);
                            if (v2 != 0 && (u8)(v2 - 3) <= 1) {
                                text.format(base + 0x13e, val, func_80136190(base + 0x130, base + 0x139, 0x21));
                            } else {
                                text.format(base + 0x422, val);
                            }
                            sprintf(buf, base + 0x408, idx);
                            func_80136B4C(info->state.layout, buf, text.c_str(), 0);
                        } else {
                            sprintf(buf, base + 0x408, idx);
                            func_80136B4C(info->state.layout, buf, base + 0x2aa, 0);
                        }
                        char* s = ((char*(*)(void*, char*, u16))func_8013639C)(lbl_eu_806640D8, base + 0x139, equip);
                        text.format(base + 0x419, s, label);
                        itemCount = (u8)((w >> 22) & 7);
                        value = (s16)((w >> 11) & 0x7FF);
                    }
                }
                sprintf(buf, base + 0x426, idx);
                func_80136B4C(info->state.layout, buf, text.c_str(), 0);
                if (tex != 0) func_80137F88(pane, tex);
                if (((u8*)info)[0x9A] != 4) {
                    nw4r::lyt::Pane* pane2 = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(base + 0x16e, true);
                    // POD vectors: default/copy ctors would emit extra bl calls
                    // absent from retail; field-wise staging reproduces the
                    // three lwz/stw word pairs before copyVEC3.
                    nw4r::math::_VEC3 pos;
                    func_80137924(&pos, pane, pane2, (nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10));
                    nw4r::math::_VEC3 tmp;
                    if ((u8)i < 12) *(s16*)((u8*)info + 0xB0 + (u8)i * 2) = itemVal;
                    if ((u8)i < 12) ((u8*)info)[0x158 + (u8)i] = 3;
                    if ((u8)i < 12) ((u8*)info)[0x164 + (u8)i] = itemCount;
                    if ((u8)i < 12) *(s16*)((u8*)info + 0x170 + (u8)i * 2) = value;
                    tmp.x = pos.x;
                    tmp.y = pos.y;
                    tmp.z = pos.z;
                    if ((u8)i < 12) copyVEC3((u8*)info + 0xC8 + (u8)i * 12, &tmp);
                }
            } else {
                func_80124270(pane, 0);
            }
        }
    } else {
        // No candidate record: plain item-name list driven by the name table.
        void* obj = (void*)((char* (*)(u32))func_801393CC)(itemId);
        u32 cat = func_80139358(itemId);
        u8 sel = (u8)func_801361E8((u32)obj, base + 0x432, (u16)cat);
        u32 tag = 0x74696D67;
        for (u32 i = 0; i < 3; i++) {
            u32 idx = (u8)i + 1;
            sprintf(buf2, base + 0x35f, idx);
            nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf2, true);
            if ((u8)i < sel) {
                func_80124270(pane, 1);
                u32 itemVal = 0;
                u32 itemCount = 0;
                u8 val = 0;
                u32 tex = 0;
                text.format(base + 0x43b, idx);
                u16 nameId = (u16)func_80136254((void*)obj, text.c_str(), (u16)cat);
                if (nameId == 0) {
                    tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x36e), 0);
                    text.format(base + 0x18, func_80136190(base + 0x130, base + 0x139, 0x2a));
                } else {
                    CItemBoxNameRecord2 rec3;
                    func_801D5564(&rec3, info, (void*)nameId, 0);
                    CItemBoxNameRecord2 rec3b = rec3;
                    switch (rec3b.e1) {
                        case 0: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x36e), 0); break;
                        case 4: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x384), 0); break;
                        case 5: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x39a), 0); break;
                        case 6: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3b0), 0); break;
                        case 7: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3c6), 0); break;
                        case 8: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3dc), 0); break;
                        case 9: tex = ((CItemNameLookupVt*)info->state.resource)->findName(tag, (u32)(base + 0x3f2), 0); break;
                    }
                    if (rec3b.name[0] == '0') {
                        sprintf(buf2, base + 0x408, idx);
                        func_80136B4C(info->state.layout, buf2, base + 0x2aa, 0);
                    } else {
                        if (rec3b.e0 == 0) {
                            text.format(base + 0x41e, rec3b.name);
                        } else if ((u8)(rec3b.e0 - 3) <= 1) {
                            text.format(base + 0x419, rec3b.name, func_80136190(base + 0x130, base + 0x139, 0x21));
                        } else {
                            text.format(base + 0x41e, rec3b.name);
                        }
                        sprintf(buf2, base + 0x408, idx);
                        func_80136B4C(info->state.layout, buf2, text.c_str(), 0);
                    }
                    char* s = func_801394D4(nameId);
                    text.format(base + 0x419, rec3b.str, s);
                    itemVal = func_80139358(nameId);
                    itemCount = func_801361E8((u32)lbl_eu_806640EC, base + 0x1f9, nameId);
                    val = (u8)func_801361E8((u32)lbl_eu_806640EC, base + 0x447, nameId);
                }
                sprintf(buf2, base + 0x426, idx);
                func_80136B4C(info->state.layout, buf2, text.c_str(), 0);
                if (tex != 0) func_80137F88(pane, tex);
                if (((u8*)info)[0x9A] != 4) {
                    nw4r::lyt::Pane* pane2 = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(base + 0x16e, true);
                    nw4r::math::_VEC3 pos;
                    func_80137924(&pos, pane, pane2, (nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10));
                    nw4r::math::_VEC3 tmp;
                    if ((u8)i < 12) *(s16*)((u8*)info + 0xB0 + (u8)i * 2) = (s16)itemVal;
                    if ((u8)i < 12) ((u8*)info)[0x158 + (u8)i] = 3;
                    if ((u8)i < 12) ((u8*)info)[0x164 + (u8)i] = (u8)itemCount;
                    if ((u8)i < 12) *(s16*)((u8*)info + 0x170 + (u8)i * 2) = (s16)val;
                    tmp.x = pos.x;
                    tmp.y = pos.y;
                    tmp.z = pos.z;
                    if ((u8)i < 12) copyVEC3((u8*)info + 0xC8 + (u8)i * 12, &tmp);
                }
            } else {
                func_80124270(pane, 0);
            }
        }
    }
#pragma pop
}

void setItemBoxIndex(CItemBoxInfo* info, unsigned char index, short value) {
    if (index >= 12) return;
    info->state.values[index] = value;
}

// Retail uses the stmw/lmw frame (MWCC optimize_for_size prologue merge) and
// keeps the same-TU helpers as real bl calls (dont_inline blocks IPA from
// folding func_801D8930/func_801D85D8/func_801D5AA0 into this body).
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801D77BC(CItemBoxInfo* info, u16 arg2) {
    func_801D8930(info);
    func_801D85D8(info);
    // Selection-colour staging: each if/else pair assigns its colour words to
    // u32 temporaries declared first (highest frame slots, retail
    // sp+0x68..0x74); the join reloads them into out[].
    u32 stage1Lo;
    u32 stage1Hi;
    u32 stage2Lo;
    u32 stage2Hi;
    char buf[0x20];
    CItemBoxSlotFlagRecord local;
    u32 out[4];
    CItemBoxSlotFlagRecord record;
    // 9-byte per-slot flag record: bytes [1..8] are per-slot flags, byte [8]
    // also selects the row-height label size. Only the three live members are
    // copied (retail copies u32+u32+u8).
    func_801D5AA0(reinterpret_cast<CItemBoxInfo*>(&record), info, (void*)arg2);
    local.a = record.a;
    local.b = record.b;
    local.c = record.c;
    char* base = (char*)&lbl_eu_805063BC;
    u32 rowHeight = 0x2c;
    if (local.c != 0) rowHeight = 0x2b;
    char* label = func_80136190(base + 0x130, base + 0x139, rowHeight);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x44f, label, 0);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 color = 0x777777ff;
        if (((u8*)&local)[(u8)i + 1] != 0) color = 0xFFFFFFFF;
        sprintf(buf, base + 0x303, (u8)i + 1);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, color);
        s16 vA2 = *(s16*)((u8*)info + 0xA2);
        s16 vAA = *(s16*)((u8*)info + 0xAA);
        // zero via a variable: MWCC materializes it in a register and cannot
        // fold the {zero, zero} s16-pair arrays to a constant, keeping the
        // retail stack build of the four u32 colour args.
        s16 zero = 0;
        s16 c0hi[2] = {zero, vA2};
        s16 c0lo[2] = {zero, zero};
        s16 c1hi[2] = {zero, vAA};
        s16 c1lo[2] = {zero, zero};
        u8 flag = ((u8*)&local)[(u8)i + 1];
        out[0] = 0;
        out[1] = 0;
        out[2] = 0;
        out[3] = 0;
        if (flag != 0) {
            stage1Lo = *(u32*)((u8*)info + 0x9C);
            stage1Hi = *(u32*)((u8*)info + 0xA0);
        } else {
            stage1Lo = *(u32*)c0lo;
            stage1Hi = *(u32*)c0hi;
        }
        out[0] = stage1Lo;
        out[1] = stage1Hi;
        if (flag != 0) {
            stage2Lo = *(u32*)((u8*)info + 0xA4);
            stage2Hi = *(u32*)((u8*)info + 0xA8);
        } else {
            stage2Lo = *(u32*)c1lo;
            stage2Hi = *(u32*)c1hi;
        }
        out[2] = stage2Lo;
        out[3] = stage2Hi;
        sprintf(buf, base + 0x161, (u8)i + 1, out[0], out[1], out[2], out[3]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(material, (u8)j, (u8*)out + (u8)j * 8);
            }
        }
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
// Retail func_801D79F8 (kind-3 detail renderer): rebuild the detail record
// (func_801D5564), print the description/caption text, position the cursor
// row from the layout panes, stash the equip summary at info+0xB0.., then per
// player slot recolor the row panes and stamp the owning player's name onto
// equipment entries that resolve back to the selected item.
void func_801D79F8(CItemBoxInfo* info, u16 arg2, void* arg3, u32 arg4) {
    // Frame order mirrors retail (descending slots): record > body > caption
    // storage > position > colours > slot values > cursor temp > staged pair.
    // body must be copy-INITIALIZED (not default-decl + assign): an assignment
    // compiles to an out-of-line __as__ call, while copy-init emits the
    // retail mtctr 8-byte-pair copy loop.
    CItemBoxDetailRec rec;
    char textStorage[sizeof(ml::FixStr<32>)];
    ml::FixStr<32>& caption = *reinterpret_cast<ml::FixStr<32>*>(textStorage);
    nw4r::math::_VEC3 pos;
    CItemBoxVertexColors cols;
    CItemBoxSlotVals vals;
    nw4r::math::_VEC3 tmp;
    CItemBoxTagStage cur;
    // volatile: retail retains these never-read default-handle stores (the
    // table handles come straight off .sdata2); a plain local gets them DCE'd.
    volatile CItemBoxTagStage stage;

    func_801D8A88(info);
    func_801D85D8(info);
    func_801D5564(&rec, info, (void*)(u32)arg2, arg3);
    CItemBoxDetailBody body = rec.body;

    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x45a, body.text, 0);

    caption.mString[0] = 0;
    caption.mLength = 0;
    void* item = NULL;
    if (arg3 != NULL) {
        item = arg3;
    }
    if (body.kind == 0x30) {
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x466, base + 0x2aa, 0);
    } else {
        if (body.flag18 != 0) {
            // Flag values 3/4 pick a count-formatted caption label.
            if ((u32)(body.flag18 - 3) <= 1) {
                char* label = func_80136190(base + 0x130, base + 0x139, 0x21);
                caption.format(base + 0x419, &body.kind, label);
            } else {
                caption.format(base + 0x41e, &body.kind);
            }
        } else {
            caption.format(base + 0x41e, &body.kind);
        }
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x466, caption.mString, 0);
    }

    // Ternaries kept inline at their use sites so the results stay in the
    // ABI return register instead of living across the whole state!=4 block.
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x472,
                  (arg3 != NULL)
                      ? (const char*)((CItemImplVt*)CItem_initItemImplInstances(item))->_v20(item)
                      : func_801394D4(arg2),
                  0);

    if (*(u8*)((u8*)info + 0x9A) != 4) {
        nw4r::lyt::Pane* pane =
            ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(base + 0x472, true);
        nw4r::lyt::Pane* pane2 =
            ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(base + 0x16e, true);
        func_80137924((nw4r::math::_VEC3*)&pos, pane, pane2,
                      (nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10));

        *(s16*)((u8*)info + 0xB0) = (arg3 != NULL)
            ? ((CItemImplVt*)CItem_initItemImplInstances(item))->_v54(item)
            : func_80139358(arg2);
        *(u8*)((u8*)info + 0x158) = (arg3 != NULL) ? 3 : 9;
        *(u8*)((u8*)info + 0x164) = (u8)((arg3 != NULL)
            ? (u16)((CItemImplVt*)CItem_initItemImplInstances(item))->_v08(item)
            : 0);
        setItemBoxIndex((CItemBoxInfo*)((u8*)info + 0xB0), 0,
                        (s16)((arg3 != NULL)
                            ? (s16)((CItemImplVt*)CItem_initItemImplInstances(item))->_v90(item)
                            : 0));
        // Field-wise copy: retail expands this as three lwz/stw word pairs
        // (a whole-struct assign emits an out-of-line __as__ call).
        tmp.x = pos.x;
        tmp.y = pos.y;
        tmp.z = pos.z;
        copyVEC3((u8*)info + 0xC8, &tmp);
    }

    u16 cat2 = (arg3 != NULL)
        ? ((CItemImplVt*)CItem_initItemImplInstances(item))->_v54(item)
        : func_80139358(arg2);
    u8 rows = (u8)func_801361E8((u32)(&lbl_eu_806640D8)[cat2], base + 0x47d, cat2);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x484,
                  func_80136190(base + 0x130, base + 0x139, rows + 0x15), 0);

    stage.tbl = lbl_eu_80668030;
    stage.h = lbl_eu_80668034;
    u8 playerByte = (u8)(arg4 >> 16);

    for (u32 i = 0; (u32)(u8)i < (u32)(u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(caption.mString, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, caption.mString, -1);
        cols.w[0] = 0;
        cols.w[1] = 0;
        cols.w[2] = 0;
        cols.w[3] = 0;
        cols.w[0] = *(u32*)((u8*)info + 0x9C);
        cols.w[1] = *(u32*)((u8*)info + 0xA0);
        cols.w[2] = *(u32*)((u8*)info + 0xA4);
        cols.w[3] = *(u32*)((u8*)info + 0xA8);
        sprintf(caption.mString, base + 0x161, idx);
        nw4r::lyt::Pane* pane =
            ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(textStorage, true);
        if (pane != NULL) {
            void* colorObj = ((CItemBoxPaneVt*)pane)->getColorObject();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(colorObj, j, &cols.w[j * 2]);
            }
        }
        if (arg3 == NULL) continue;
        u8 flag = func_801392B4(i);
        if (flag == 0) continue;
        void* lookup = func_8009EC9C(flag);
        cur.tbl = *(u32*)((u8*)lookup + 8);
        cur.h = *(u16*)((u8*)lookup + 0xc);
        vals.v26 = *(s16*)((u8*)lookup + 0x26);
        vals.v1C = *(s16*)((u8*)lookup + 0x1c);
        vals.v1E = *(s16*)((u8*)lookup + 0x1e);
        vals.v20 = *(s16*)((u8*)lookup + 0x20);
        vals.v22 = *(s16*)((u8*)lookup + 0x22);
        vals.v24 = *(s16*)((u8*)lookup + 0x24);
        for (u32 k = 0; k < 6; k++) {
            void* hit = func_80157C4C(((u8*)&cur)[k], vals.s[k]);
            if (hit == NULL || *(u32*)hit == 0) continue;
            u8 n = ((CItemImplVt*)CItem_initItemImplInstances(hit))->_v30(hit);
            for (u32 m = 0; (u32)(u8)m < (u32)n; m++) {
                s16 v40 = ((CItemImplVt*)CItem_initItemImplInstances(hit))->_v40(hit, m);
                void* rec3 = func_80157C4C(3, v40);
                if (rec3 == NULL || *(u32*)rec3 == 0 || rec3 != arg3) continue;
                sprintf(caption.mString, base + 0x30e, idx);
                u8 owner = func_801392B4((u32)playerByte);
                u32 name;
                if (flag == owner) {
                    name = ((CItemNameLookupVt*)info->state.arcResourceAccessor)
                               ->findName(0x74696d67, (u32)(base + 0x319), 0);
                } else {
                    name = ((CItemNameLookupVt*)info->state.arcResourceAccessor)
                               ->findName(0x74696d67, (u32)(base + 0x32d), 0);
                }
                if (name == 0) {
                    name = ((CItemNameLookupVt*)info->state.arcResourceAccessor)
                               ->findName(0x74696d67, (u32)(base + 0x341), 0);
                }
                if (name != 0) {
                    func_80137E7C((nw4r::lyt::Layout*)info->state.layout, caption.mString, name);
                }
            }
        }
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
// Four-word label-argument record copied by func_801D8058/func_801E3730.
struct CItemBoxLabelArgs { u32 v[4]; };

void func_801D8058(CItemBoxInfo* info, u32 arg2) {
    func_801D8B08(info);
    func_801D85D8(info);
    // Out-buffer copy: retail reloads all four words from the escaped buffer
    // and re-homes them via a plain struct-style copy before the calls.
    // vals declared first so it lands at the higher frame slot (retail
    // places the D59C0 out-buffer below the staged copy).
    CItemBoxLabelArgs vals;
    CItemBoxLabelArgs out;
    func_801D59C0(out.v, info, (void*)arg2);
    vals = out;
    char* base = lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x48f, (char*)vals.v[1], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x49b, (char*)vals.v[3], 0);
}
#pragma pop
void func_801D5C38(void*, void*, void*, void*);
void __declspec(noinline) func_801D8B60(CItemBoxInfo*);
// Retail func_801D80EC: item-box slot-list renderer. Rebuilds the per-slot
// record (func_801D5C38), stamps the header pane with the formatted item
// string, then per filled pair sets the row-name pane text and (unless mode
// 0x9A==4) measures the row position and caches value/count/pos per slot.
#pragma push
#pragma optimize_for_size on
void func_801D80EC(CItemBoxInfo* info, u16 arg2, void* arg3) {
    // Frame-slot order matters: state > cur > text > out > buf > pos > tmp
    // (descending). The 9 constant claims r31 first (Rule A).
    u8 slotState = 9;
    CItemBoxSlotRecAny1 cur;
    ml::FixStr<32> text(false);
    CItemBoxSlotRecAny1 out;
    char buf[0x20];
    nw4r::math::_VEC3 pos;
    nw4r::math::_VEC3 tmp;
    func_801D8B60(info);
    func_801D85D8(info);
    func_801D5C38(&out.rec, info, (void*)(u32)arg2, arg3);
    // 0x2C-byte copy: retail mtctr 8-byte-pair loop (li r0,5 + 4-byte tail).
    cur = out;
    char* base = lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x4a7, (char*)cur.rec.str, 0);
    u8 count = cur.rec.counter;
    for (u8 i = 0; (u8)i < count; i++) {
        sprintf(buf, base + 0x4b3, (u32)((u8)i * 2) + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, (char*)cur.rec.text[(u8)i], 0);
        if (((u8*)info)[0x9A] != 4) {
            nw4r::lyt::Pane* pane =
                ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
            nw4r::lyt::Pane* pane2 =
                ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(base + 0x16e, true);
            func_80137924((nw4r::math::VEC3*)&pos, pane, pane2,
                          (nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10));
            // POD copy: retail expands this as three lwz/stw pairs.
            tmp = *(nw4r::math::_VEC3*)&pos;
            if ((u8)i < 12) *(s16*)((u8*)info + 0xB0 + (u8)i * 2) = (s16)cur.rec.counts[(u8)i];
            if ((u8)i < 12) ((u8*)info)[0x158 + (u8)i] = slotState;
            if ((u8)i < 12) copyVEC3((u8*)info + 0xC8 + (u8)i * 12, &tmp);
        }
        text.clear();
        sprintf(buf, base + 0x4c0, (u32)(u8)i + 0x1f);
        text.format(base + 0x13e, cur.rec.vals[(u8)i],
                    func_80136190(base + 0x130, base + 0x139, 0x21));
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, text.c_str(), 0);
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801D8318(CItemBoxInfo* info) {
    // No local base pointer: MWCC CSEs the repeated symbol offsets into one
    // register, splitting the address into a pre-stmw volatile lis plus a
    // post-stmw callee-saved addi exactly like retail.
    char buf[0x20];
    u32 i;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x25b], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x267], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x273], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x286], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x292], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x29e], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x354], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2ab], &lbl_eu_805063BC[0x2aa], 0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2b6], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2c1], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2cc], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2d7], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2e2], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2ed], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2f8], &lbl_eu_806645A8, &lbl_eu_806645B0);
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(buf, &lbl_eu_805063BC[0x303], idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, 0x777777ff);
        u32 args[4];
        // Zero via a variable: MWCC materializes it in a register and cannot
        // fold the {zero, zero} s16-pair arrays to a constant, keeping the
        // retail stack build of the four u32 colour args.
        s16 zero = 0;
        s16 c0hi[2] = {zero, *(s16*)((u8*)info + 0xA2)};
        s16 c0lo[2] = {zero, zero};
        s16 c1hi[2] = {zero, *(s16*)((u8*)info + 0xAA)};
        s16 c1lo[2] = {zero, zero};
        args[1] = *(u32*)c0hi;
        args[0] = *(u32*)c0lo;
        args[3] = *(u32*)c1hi;
        args[2] = *(u32*)c1lo;
        sprintf(buf, &lbl_eu_805063BC[0x161], idx, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(material, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(buf, &lbl_eu_805063BC[0x30e], idx);
        u32 tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)(&lbl_eu_805063BC[0x341]), 0);
        if (tex != 0) {
            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, buf, tex);
        }
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801D85D8(CItemBoxInfo* info) {
    u32 i;
    char* base = (char*)&lbl_eu_805063BC;
    u32 idx;
    u32 tag;
    u32 white;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x267, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x273, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x29e, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x354, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x2ab, base + 0x2aa, 0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2c1, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2cc, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2d7, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2e2, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2ed, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2f8, &lbl_eu_806645A8, &lbl_eu_806645B0);
    char buf[0x20];
    tag = 0x74696D67u;
    white = 0x777777FFu;
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        idx = (u32)((u8)i + 1);
        sprintf(buf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, white);
        u32 args[4];
        // zero via a variable: MWCC materializes it in a register and cannot
        // fold the {zero, zero} s16-pair arrays to a constant, keeping the
        // retail stack build of the four u32 colour args.
        s16 zero = 0;
        s16 c0hi[2] = {0, *(s16*)((u8*)info + 0xA2)};
        s16 c0lo[2] = {0, 0};
        s16 c1hi[2] = {0, *(s16*)((u8*)info + 0xAA)};
        s16 c1lo[2] = {0, 0};
        args[1] = *(u32*)c0hi;
        args[0] = *(u32*)c0lo;
        args[3] = *(u32*)c1hi;
        args[2] = *(u32*)c1lo;
        sprintf(buf, base + 0x161, idx, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(material, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(buf, base + 0x30e, idx);
        u32 tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(tag, (u32)(base + 0x341), 0);
        if (tex != 0) {
            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, buf, tex);
        }
    }
}
#pragma pop
// Retail func_801D885C uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge): info in r28, loop counter in r29, index in r30,
// string-pool base in r31.
#pragma push
#pragma optimize_for_size on
void func_801D885C(CItemBoxInfo* info) {
    char buf[0x20];
    // Declaration order fixes the r29/r30 coloring: retail claims index (r30)
    // before the loop counter i (r29) - saved regs go high->low by birth order.
    u32 index;
    u32 i;
    for (i = 0; i < 3; i++) {
        index = (u8)i + 1;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x35f], index);
        void* r = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        func_80124270(r, 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x408], index);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x426], index);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
    }
}
#pragma pop
// Retail func_801D8930: label every slot pane, then build two 8-byte
// selection colours (from the s16s at +0xA2/+0xAA) read as four u32 varargs
// for the pane-name sprintf, look the pane up, and push both colours into its
// vertex buffer via func_801D62F8. layout is reloaded per use (lwz 0x34).
#pragma push
#pragma optimize_for_size on
void func_801D8930(CItemBoxInfo* info) {
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x44f, base + 0x2aa, 0);
    char buf[0x20];
    // Retail re-evaluates the slot count on every iteration (bl in the
    // loop condition), so do not hoist the call.
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u32)((u8)i + 1);
        sprintf(buf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, 0x777777ff);
        // Selection colours: two 8-byte pairs built from the s16s at +0xA2
        // and +0xAA; retail stages the four arrays at sp+0x08..0x17 and the
        // u32 varargs copy at sp+0x18..0x27.
        s16 zero = 0;
        s16 c0hi[2] = {zero, *(s16*)((u8*)info + 0xA2)};
        s16 c0lo[2] = {zero, zero};
        s16 c1hi[2] = {zero, *(s16*)((u8*)info + 0xAA)};
        s16 c1lo[2] = {zero, zero};
        u32 args[4];
        args[0] = *(u32*)c0lo;
        args[1] = *(u32*)c0hi;
        args[2] = *(u32*)c1lo;
        args[3] = *(u32*)c1hi;
        sprintf(buf, base + 0x161, idx, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* obj = ((CItemBoxPaneVt*)pane)->getColorObject();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(obj, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
    }
}
#pragma pop
// Retail func_801D8A88 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge); the first call's arg4 zero is hoisted into the prologue.
#pragma push
#pragma optimize_for_size on
void func_801D8A88(CItemBoxInfo* info) {
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x45a],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x466],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x484],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x472],
                  &lbl_eu_805063BC[0x2aa], 0);
}
#pragma pop

// Retail func_801D8B60 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge): info in r29, loop counter in r30, string-pool base in r31.
#pragma push
#pragma optimize_for_size on
void func_801D8B60(CItemBoxInfo* info) {
    char buf[0x20];
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x4a7],
                  &lbl_eu_805063BC[0x2aa], 0);
    for (u32 i = 0; i < 4; i++) {
        sprintf(buf, &lbl_eu_805063BC[0x4b3], (u8)i * 2 + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, &lbl_eu_805063BC[0x4c0], (u8)i + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
    }
}
#pragma pop

#pragma push
#pragma dont_inline on
#pragma optimize_for_size on
extern "C" void func_801D8B08(CItemBoxInfo* info) {
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x48f],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x49b],
                  &lbl_eu_805063BC[0x2aa], 0);
}
#pragma pop

// Retail func_801D8C0C: per-slot item-box1 renderer. Loops 7 slots; for slots
// below the active count it resolves the item name via func_801355F4/vtable[3]
// and colors the pane; slots at/above the count get a plain label pane. Uses
// the stmw/lmw frame (optimize_for_size prologue merge). The two sprintf
// buffers stay separate locals (retail sp+0x48 / sp+0x28).
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
extern "C" void func_801D8C0C(CItemBoxInfo* info) {
    // Shape mirrors the proven func_801E4194 (ItemBox2 twin): if/else instead
    // of continue-guards, hoisted i/idx, direct lbl refs, typed name lookup.
    u32 i;
    char bufElse[0x20];
    char buf[0x20];
    u8 count = code80135FDC_getByte_64077();
    u32 idx;
    for (i = 0; i < 7; i++) {
        if ((u8)i >= count) {
            sprintf(bufElse, (char*)&lbl_eu_805063BC[0x161], (u8)i + 1);
            nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(bufElse, true);
            func_80124270(pane, 0);
        } else {
            u8 slot = (u8)func_801392B4((u8)i);
            u32 tag = 0x74696D67u;
            u32 nameId = (u16)func_80136254(lbl_eu_80664090, (char*)&lbl_eu_805063BC[0x4ce], slot);
            u32 itemId = (u32)func_80138F78(nameId);
            u32 found = ((CItemNameLookupVt*)func_801355F4())->findName(tag, itemId, 0);
            if (found != 0) {
                idx = (u32)((u8)i + 1);
                sprintf(buf, (char*)&lbl_eu_805063BC[0x303], idx);
                nw4r::lyt::Pane* pane2 = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
                func_80137F88(pane2, found);
                func_80137C1C(pane2, 0x777777ffu);
                s16 zero = 0;
                s16 c0hi[2] = {zero, *(s16*)((u8*)info + 0xA2)};
                s16 c0lo[2] = {zero, zero};
                s16 c1hi[2] = {zero, *(s16*)((u8*)info + 0xAA)};
                s16 c1lo[2] = {zero, zero};
                u32 args[4];
                args[0] = *(u32*)c0lo;
                args[1] = *(u32*)c0hi;
                args[2] = *(u32*)c1lo;
                args[3] = *(u32*)c1hi;
                sprintf(buf, (char*)&lbl_eu_805063BC[0x161], idx, args[0], args[1], args[2], args[3]);
                nw4r::lyt::Pane* pane3 = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
                void* vertexObj = pane3->GetMaterial();
                for (u32 j = 0; j < 2; j++) {
                    func_801D62F8(vertexObj, (u8)j, (u8*)args + (u8)j * 8);
                }
            }
        }
    }
}
#pragma pop
// Local data shapes used by the ItemBox1 comparison renderer.
struct D8EQuad {
    s16 r, g, b, a;
};
struct D8ERenderQuad {
    f32 values[8];
};
struct D8EEntry {
    union {
        u32 words[13];
        s16 halfWords[26];
    };
};
struct D8EArmorEntry {
    union {
        u32 words[7];
        s16 halfWords[14];
        struct {
            u16 _00;
            u16 _02;
            s16 etherDefense;
            s16 physicalDefense;
            u8 _08[0x14];
        };
    };
};
union D8EComparisonStorage {
    D8EEntry weapon[4];
    D8EArmorEntry armor[4];
};

static inline void scaleArmorDefense(D8EArmorEntry& entry, s32 effect) {
    f32 scale = 0.01f * (100.0f + (f32)effect);
    entry.physicalDefense = (s16)(s32)((f32)entry.physicalDefense * scale);
    entry.etherDefense = (s16)(s32)((f32)entry.etherDefense * scale);
}

static inline void applyArmorEffects(void* info, u16 member,
                                     D8EArmorEntry& entry, u8 armorType) {
    switch (armorType) {
        case 3: {
            s32 effect = func_801DFD60(info, (void*)(u32)member, 0x0D);
            if (effect != 0) scaleArmorDefense(entry, effect);
            effect = func_801DFD60(info, (void*)(u32)member, 0x24);
            if (effect != 0) {
                entry.etherDefense = (s16)(entry.etherDefense + (s16)effect);
            }
            break;
        }
        case 2: {
            s32 effect = func_801DFD60(info, (void*)(u32)member, 0x0C);
            if (effect != 0) scaleArmorDefense(entry, effect);
            break;
        }
        case 1: {
            s32 effect = func_801DFD60(info, (void*)(u32)member, 0x0B);
            if (effect != 0) scaleArmorDefense(entry, effect);
            break;
        }
    }
}
struct D8EStatC {
    u8 _00[6];
    s16 s06;
    u8 _08[4];
    s16 s0C, s0E, s10;
    u8 _12[6];
    s16 s18;
    u8 _1A[2];
    s16 s1C;
    u8 _1E[4];
    s16 s22;
};
struct D8EStatA {
    u8 _00[0x10];
    f32 f10;
    u8 _14[8];
    s16 s1C, s1E, s20;
    u8 _22[0xC];
    s16 s2E;
    u8 _30[2];
    s16 s32;
    u8 _34[4];
    s16 s38;
    u8 _3A[0x1B];
    u8 b55;
};
struct D8EStatB {
    u8 _00[0x10];
    f32 f10;
};
typedef void (*D8EVF)(void*, u32);

static inline void applyItemBoxDeltaColor(void* valuePane, void* labelPane, s16 delta) {
    D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
    D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
    D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
    D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
    if (delta < 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
    } else if (delta > 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
    }
    func_80139AC8(valuePane, &valueTop, &valueBottom);
    func_80139AC8(labelPane, &labelTop, &labelBottom);
}

static inline void applyItemBoxDeltaColorAt(CItemBoxInfo* info, u32 valueIndex,
                                             u32 labelIndex, s16 delta) {
    D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
    D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
    D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
    D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
    if (delta < 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
    } else if (delta > 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
    }
    nw4r::lyt::Pane** panes = (nw4r::lyt::Pane**)((u8*)info + 0x40);
    func_80139AC8(panes[valueIndex], &valueTop, &valueBottom);
    func_80139AC8(panes[labelIndex], &labelTop, &labelBottom);
}

static inline void applyItemBoxDeltaColorPair(void* firstValuePane, void* firstLabelPane,
                                               void* secondValuePane, void* secondLabelPane,
                                               s16 delta) {
    D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
    D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
    D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
    D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
    if (delta < 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
    } else if (delta > 0) {
        __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
        __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
        __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
        __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
    }
    func_80139AC8(firstValuePane, &valueTop, &valueBottom);
    func_80139AC8(firstLabelPane, &labelTop, &labelBottom);
    func_80139AC8(secondValuePane, &valueTop, &valueBottom);
    func_80139AC8(secondLabelPane, &labelTop, &labelBottom);
}

void copyItemBoxEntry(CItemBoxInfoEntry*, const CItemBoxInfoEntry*);
void setItemBoxEntry(CItemBoxInfoEntry*, u16, u32, u8);
extern const u32 lbl_eu_8066806C;
extern const u8 lbl_eu_80668070;
extern const u32 lbl_eu_80506368[6];
u32 func_801DF988(void*, void*, u32, void*, s32);

// Named views keep the item-stat calculations readable while leaving the
// backing entry as a plain 13-word record, which is how MWCC copies it.
#define w04 words[1]
#define _0C words[3]
#define _10 words[4]
#define w14 halfWords[10]

#pragma push
#pragma optimize_for_size on
extern "C" void func_801D8E34(CItemBoxInfo* info, u32 arg2, void* arg3, u32 arg4) {
    // Raw-storage reference binding: retail emits NO FixStr ctor anywhere in
    // this function (first use is format(), which fully initializes), so the
    // out-of-line __ct__Q22ml10FixStr<32>Fb/Fb calls must be suppressed.
    char textBufferStorage[sizeof(ml::FixStr<32>)];
    ml::FixStr<32>& textBuffer = *reinterpret_cast<ml::FixStr<32>*>(textBufferStorage);
    // Packed selection (retail rlwinm-decoded): low nibble is the equipment
    // category, bits 4..7 are unused-by-extraction type?? -> retail extracts
    // member=(arg2>>8)&0xFF and type=(arg2>>4)&0xF. Slot zero takes its
    // category from the candidate item itself ((item>>16)&0xF).
    u8 slot = (u8)(arg2 & 0xF);
    u8 type = (u8)((arg2 >> 4) & 0xF);
    // member nibble-pair is extracted in the prologue region: it stays live
    // across the party-ping calls (retail rlwinm r18 right after type).
    u8 memberRaw = (u8)((arg2 >> 8) & 0xFF);
    if (slot == 0) {
        void* selectedItem = arg3 != NULL ? arg3 : NULL;
        if (selectedItem != NULL) {
            type = (u8)((*(u32*)selectedItem >> 12) & 0xF);
        }
    }

    // ---- party-slot ping: 12-word copy of party struct + 2x3 vtable[0xA4] ----
    struct PartyData { u32 w[12]; };
    void* party = func_8009ECB0();
    PartyData partyData = *(PartyData*)((u8*)party + 4);
    for (u32 row = 0; row < 2; row++) {
        for (u32 col = 0; col < 3; col++) {
            u8 id = (u8)partyData.w[col];
            if (id != 0) {
                void* actor = func_800B8B94(id);
                if (actor != NULL) {
                    ((D8EVF*)(*(void***)actor))[0x29](actor, 0);
                }
            }
        }
    }

    // ---- character setup ----
    u8 member = (u8)func_801392B4(memberRaw);
    void* charObj = func_8009EC9C(member);
    if (func_800B8B94(member) == NULL) {
        func_800A13C4(charObj, 1);
    }
    void* stats = (u8*)charObj + 0x17C;
    void** vt = *(void***)stats;

    // ---- HP values (clamped to 9999) ----
    s32 hp1 = (s32)((float(*)(void*))vt[0x4A])(stats);
    if (hp1 > 9999) hp1 = 9999;
    s32 hp2 = (s32)((float(*)(void*))vt[0x4B])(stats);
    if (hp2 > 9999) hp2 = 9999;

    // ---- name / pane text ----
    func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), &lbl_eu_805063BC[0x4D7], func_8013639C(lbl_eu_806640D8, &lbl_eu_805063BC[0x139]), 0);
    func_80136910((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), &lbl_eu_805063BC[0x4E3], ((u8(*)(void*))vt[0x42])(stats));
    func_80136910((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), &lbl_eu_805063BC[0x4F0], ((u8(*)(void*))vt[0x79])(stats));
    func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], hp1);
    func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], hp2);
    func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[3], func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x82), 0);

    // ---- stat sub-objects ----
    D8EStatA* stA = (D8EStatA*)((void*(*)(void*))vt[0x8A])(stats);
    D8EStatB* stB = (D8EStatB*)((void*(*)(void*))vt[0x83])(stats);
    D8EStatC* stC = (D8EStatC*)((void*(*)(void*))vt[0x8B])(stats);
    func_8009D7E4((u8*)charObj + 0x1C, 5);

    // ---- arts sum ----
    s32 artsSum = (s32)func_801DFE48(info, member, NULL);
    u32 w = func_801DFD60(info, (void*)(u32)member, 0x97);
    if (w != 0) {
        artsSum = artsSum - (s32)w;
        if (artsSum < 0) artsSum = 0;
    }

    // ---- stat bars (display formula) ----
    s32 effect = func_801DF610(info, (void*)(u32)member, 0x21, NULL);
    // Expanded inline: optimize_for_size out-lines the helper, but retail
    // has no separate calls here (formula folded into each bar's sequence).
    volatile s16 bar1 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10) * (f32)(stA->s20 + effect)));
    effect = func_801DF610(info, (void*)(u32)member, 0x1, NULL);
    volatile s16 bar2 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C) * (f32)(stA->s1C + effect)));
    effect = func_801DF610(info, (void*)(u32)member, 0x31, NULL);
    s16 bar3 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)(stA->s32 + effect)));
    effect = func_801DF610(info, (void*)(u32)member, 0x41, NULL);
    volatile s16 bar4 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E) * (f32)(stA->s1E - (s16)artsSum + effect)));
    effect = func_801DF610(info, (void*)(u32)member, 0x51, NULL);
    s16 bar5 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + effect)));
    effect = func_801DF610(info, (void*)(u32)member, 0x54, NULL);
    s16 bar6 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)(stA->s38 + effect)));
    effect = func_801DF610(info, (void*)(u32)member, 0x11, NULL);
    volatile s16 hpStat = (s16)(s32)(stA->f10 + (f32)func_801C6158(
        0.01f * stB->f10 * (f32)(stC->s06 + effect)));
    if (hpStat > 9999) hpStat = 9999;

    // ---- equip-mode gate: recompute bars with the equipped stat (0x30) ----
    if (func_801DFFB8(info, member, NULL, NULL)) {
        s32 eq = func_801DFD60(info, (void*)(u32)member, 0x30);
        if (eq != 0) {
            s16 barA = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)eq) * (f32)(stA->s20 + func_801DF610(info, (void*)(u32)member, 0x21, NULL))));
            s16 barB = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)barA) * (f32)(stA->s1C + func_801DF610(info, (void*)(u32)member, 0x1, NULL))));
            s16 barC = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)eq) * (f32)(stA->s1E + func_801DF610(info, (void*)(u32)member, 0x41, NULL) - artsSum)));
            hpStat = (s16)(s32)(stA->f10 + (f32)func_801C6158(
                0.01f * stB->f10 *
                (f32)(stC->s06 + eq + func_801DF610(info, (void*)(u32)member, 0x11, NULL))));

            // ---- current weapon block ----
            void* item2 = func_80157C4C(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                D8EEntry e1;
                func_801D4E2C(&e1, (void*)(u32)member, (void*)(u32)((u16)((*(u32*)item2 >> 20) & 0xFFFF)));
                D8EEntry e2 = e1;
                s32 s1 = func_801DF4E0(info, (void*)(u32)member,
                                       (u16)e2.w04, NULL);
                s32 s2 = func_801DF578(info, (void*)(u32)member,
                                       (u16)(e2.w04 >> 16), NULL);
                s32 atk1 = (s32)barB + s1;
                s32 atk2 = (s32)barB + s2;
                if (atk1 > atk2) atk1 = atk2;
                char* rangeSeparator = func_80136190(
                    &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB);
                textBuffer.format(&lbl_eu_805063BC[0x254], atk1, rangeSeparator, atk2);
                func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], textBuffer.mString, 0);
            }

            // ---- numbers ----
            if (barC <= 0) barC = 1;
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], (s16)barB);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], (s16)barC);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], (s16)barA);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], (s16)bar5);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], (s16)bar3);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], hpStat);
            char* percentSuffix = func_80136190(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
            textBuffer.format(&lbl_eu_805063BC[0x13E], (s16)bar6, percentSuffix);
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], textBuffer.mString, 0);
            textBuffer.format(&lbl_eu_805063BC[0x13E], (s16)(stA->b55), percentSuffix);
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], textBuffer.mString, 0);

            // ---- 20x color application ----
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[3], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[1], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[5], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[7], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[9], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[11], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[13], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[15], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[17], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[19], &q1, &q2);
        }
    }

    // ---- stat panel (arg4 != 0, type in {2,4,5,6,7,8}) ----
    if (arg4 != 0 && (type == 2 || (type >= 4 && type <= 8))) {
        s32 e1 = 0;
        if (func_801DFFB8(info, member, NULL, NULL)) e1 = func_801DFD60(info, (void*)(u32)member, 0x30);
        s32 e2 = 0;
        if (func_801DFFB8(info, member, arg3, NULL)) e2 = func_801DFD60(info, (void*)(u32)member, 0x30);

        // row A: 0x21 -> panes 0x60/0x64
        s16 dB = 0;
        {
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)e1) * (f32)(stA->s20 + func_801DF610(info, (void*)(u32)member, 0x21, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)e2) * (f32)(stA->s20 + func_801DF610(info, (void*)(u32)member, 0x21, arg3))));
            s16 d = (s16)(nb - pb);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], nb);
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            if (d < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (d > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[9], &q1, &q2);
        }

        // row B: 0x1 -> panes 0x50/0x54
        {
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)e1) * (f32)(stA->s1C + func_801DF610(info, (void*)(u32)member, 0x1, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)e2) * (f32)(stA->s1C + func_801DF610(info, (void*)(u32)member, 0x1, arg3))));
            dB = (s16)(nb - pb);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], nb);
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            if (dB < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dB > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[5], &q1, &q2);
        }

        s32 comparisonArtsSum = (s32)func_801DFE48(info, member, NULL);
        u32 comparisonWeight = func_801DFD60(info, (void*)(u32)member, 0x97);
        if (comparisonWeight != 0) {
            comparisonArtsSum -= (s32)comparisonWeight;
            if (comparisonArtsSum < 0) comparisonArtsSum = 0;
        }

        // row C: 0x41 -> panes 0x58/0x5C
        {
            s16 oldAgilityBase = (s16)(stA->s1E - (s16)comparisonArtsSum);
            if (oldAgilityBase <= 0) oldAgilityBase = 1;
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)e1) *
                (f32)(oldAgilityBase + func_801DF610(
                    info, (void*)(u32)member, 0x41, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)e2) *
                (f32)(oldAgilityBase + func_801DF610(
                    info, (void*)(u32)member, 0x41, arg3))));
            s16 dC = (s16)(nb - pb);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], nb);
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            if (dC < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dC > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[7], &q1, &q2);
        }

        // row D: 0x11 + func_801C6158 -> panes 0x40/0x48
        {
            s16 pb = (s16)(s32)(stA->f10 + (f32)func_801C6158(0.01f * stB->f10 * (f32)(stC->s06 + e1 + func_801DF610(info, (void*)(u32)member, 0x11, NULL))));
            s16 nb = (s16)(s32)(stA->f10 + (f32)func_801C6158(0.01f * stB->f10 * (f32)(stC->s06 + e2 + func_801DF610(info, (void*)(u32)member, 0x11, arg3))));
            if (pb > 9999) pb = 9999;
            if (nb > 9999) nb = 9999;
            s16 dD = (s16)(nb - pb);
            if (((float(*)(void*))vt[0x4A])(stats) > (f32)nb) {
                func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], nb);
            }
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], nb);
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            if (dD < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dD > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[3], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[1], &q1, &q2);
        }
        // ---- type switch: slot id + item ----
        s16 slotId = 0;
        switch (type) {
            case 2: slotId = *(s16*)((u8*)charObj + 0x26); break;
            case 4: slotId = *(s16*)((u8*)charObj + 0x1C); break;
            case 5: slotId = *(s16*)((u8*)charObj + 0x1E); break;
            case 6: slotId = *(s16*)((u8*)charObj + 0x20); break;
            case 7: slotId = *(s16*)((u8*)charObj + 0x22); break;
            case 8: slotId = *(s16*)((u8*)charObj + 0x24); break;
        }
        D8EComparisonStorage comparisonStorage;
        void* item = func_80157C4C(type, slotId);
        if (type == 2) {
            // ---- weapon block (0x801E7300) ----
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            D8EEntry& e_cur = comparisonStorage.weapon[0];
            func_801D4E2C(&e_cur, (void*)(u32)member, (void*)(u32)w0);
            D8EEntry& c_cur = comparisonStorage.weapon[1];
for (u32 w_ = 0; w_ < 13; w_++) {
                c_cur.words[w_] = e_cur.words[w_];
            }
            D8EEntry& e_new = comparisonStorage.weapon[2];
            func_801D4E2C(&e_new, (void*)(u32)member, arg3);
            D8EEntry& c_new = comparisonStorage.weapon[3];
for (u32 w_ = 0; w_ < 13; w_++) {
                c_new.words[w_] = e_new.words[w_];
            }
            s32 eq1 = 0;
            if (func_801DFFB8(info, member, NULL, NULL)) eq1 = func_801DFD60(info, (void*)(u32)member, 0x30);
            s32 eq2 = 0;
            if (func_801DFFB8(info, member, arg3, NULL)) eq2 = func_801DFD60(info, (void*)(u32)member, 0x30);
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq1) * (f32)(stA->s1C + func_801DF610(info, (void*)(u32)member, 0x1, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + func_801DF610(info, (void*)(u32)member, 0x1, arg3))));
            s32 atkA = (s32)pb + func_801DF4E0(info, (void*)(u32)member, (u16)c_cur.w04, NULL);
            s32 atkC = (s32)pb + func_801DF578(info, (void*)(u32)member, (u16)(c_cur.w04 >> 16), NULL);
            s32 atkB = (s32)nb + func_801DF4E0(info, (void*)(u32)member, (u16)c_new.w04, arg3);
            s32 atkD = (s32)nb + func_801DF578(info, (void*)(u32)member,
                (u16)(c_new.w04 >> 16), arg3);
            if (atkB < atkD) atkB = atkD;
            s16 dW = (s16)(atkC - atkA);
            s32 c1 = func_801DF610(info, (void*)(u32)member, 0x31, arg3);
            s16 v612 = (s16)c_new._0C;
            s16 v646 = (s16)c_cur._0C;
            v612 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)((stA->s32 - v646) + (c1 + v612))));
            c1 = func_801DF610(info, (void*)(u32)member, 0x31, NULL);
            v646 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)(stA->s32 + c1)));
            c1 = func_801DF610(info, (void*)(u32)member, 0x51, arg3);
            s16 v610 = (s16)c_new._10;
            s16 v644 = (s16)c_cur._10;
            v610 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)((stA->s2E - v644) + (c1 + v610))));
            c1 = func_801DF610(info, (void*)(u32)member, 0x51, NULL);
            v644 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            c1 = func_801DF610(info, (void*)(u32)member, 0x54, arg3);
            s16 v614 = c_new.w14;
            s16 v648 = c_cur.w14;
            v614 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)((stA->s38 - v648) + (c1 + v614))));
            c1 = func_801DF610(info, (void*)(u32)member, 0x54, NULL);
            v648 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)(stA->s38 + c1)));
            s16 snap644 = v644;
            s16 snap646 = v646;
            s16 d1 = (s16)(v610 - snap644);
            s16 d2 = (s16)(v612 - snap646);
            s16 d3 = (s16)(c_new.w14 - c_cur.w14);
            s16 d4 = (s16)(v614 - v648);
            s16 disp1 = v610;
            s16 disp2 = v612;
            s16 disp3 = v614;
            char* rangeSeparator = func_80136190(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB);
            textBuffer.format(&lbl_eu_805063BC[0x254], atkD, rangeSeparator, atkC);
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], textBuffer.mString, 0);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], (s16)disp1);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], (s16)disp2);
            char* percentSuffix2 = func_80136190(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
            textBuffer.format(&lbl_eu_805063BC[0x13E], (s16)disp3, percentSuffix2);
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], textBuffer.mString, 0);
            char* percentSuffix3 = func_80136190(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
            textBuffer.format(&lbl_eu_805063BC[0x13E],
                           (s32)stA->b55 + (s32)d3, percentSuffix3);
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], textBuffer.mString, 0);
            // delta colors (5 rows)
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[11], &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[13], &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[15], &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d4 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d4 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[17], &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d3 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d3 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[19], &q1, &q2);
            }
        } else {
            // ---- armor block (0x801E7DA0) ----
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            D8EArmorEntry& e_cur = comparisonStorage.armor[0];
            func_801D5274(&e_cur, (void*)(u32)member, (void*)(u32)w0);
            D8EArmorEntry& c_cur = comparisonStorage.armor[1];
            c_cur = e_cur;
            D8EArmorEntry& e_new = comparisonStorage.armor[2];
            func_801D5274(&e_new, (void*)(u32)member, arg3);
            D8EArmorEntry& c_new = comparisonStorage.armor[3];
            c_new = e_new;
            volatile s16 v484 = (s16)func_801DFE48(
                info, member,
                (item != NULL && *(u32*)item != 0)
                    ? (void*)(u32)(u16)(*(u32*)item >> 20)
                    : NULL);
            volatile s16 v468 = (s16)func_801DFE48(info, member, arg3);
            s32 eq1 = 0;
            if (func_801DFFB8(info, member, NULL, NULL)) eq1 = func_801DFD60(info, (void*)(u32)member, 0x30);
            s32 eq2 = 0;
            if (func_801DFFB8(info, member, arg3, NULL)) eq2 = func_801DFD60(info, (void*)(u32)member, 0x30);
            // arm-type scaling of the entry value pairs
            u8 arm1 = (u8)func_801361E8(
                (u32)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB],
                func_80139358((u32)((item != NULL && *(u32*)item != 0)
                    ? (u16)(*(u32*)item >> 20)
                    : 0)));
            {
                    switch (arm1) {
                        case 3: {
                            s32 effect_3 = func_801DFD60(info, (void*)(u32)member, 0x0D);
                            if (effect_3 != 0) {
                                f32 scale_3 = 0.01f * (100.0f + (f32)effect_3);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_3);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_3);
                            }
                            s32 effect_24 = func_801DFD60(info, (void*)(u32)member, 0x24);
                            if (effect_24 != 0) {
                                c_cur.etherDefense = (s16)(c_cur.etherDefense + (s16)effect_24);
                            }
                            break;
                        }
                        case 2: {
                            s32 effect_2 = func_801DFD60(info, (void*)(u32)member, 0x0C);
                            if (effect_2 != 0) {
                                f32 scale_2 = 0.01f * (100.0f + (f32)effect_2);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_2);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_2);
                            }
                            break;
                        }
                        case 1: {
                            s32 effect_1 = func_801DFD60(info, (void*)(u32)member, 0x0B);
                            if (effect_1 != 0) {
                                f32 scale_1 = 0.01f * (100.0f + (f32)effect_1);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_1);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_1);
                            }
                            break;
                        }
                    }
                }
            u8 arm2 = (u8)func_801361E8((u32)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], func_80139358((u32)(*(u32*)arg3 >> 20)));
            {
                    switch (arm2) {
                        case 3: {
                            s32 effect_3 = func_801DFD60(info, (void*)(u32)member, 0x0D);
                            if (effect_3 != 0) {
                                f32 scale_3 = 0.01f * (100.0f + (f32)effect_3);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_3);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_3);
                            }
                            s32 effect_24 = func_801DFD60(info, (void*)(u32)member, 0x24);
                            if (effect_24 != 0) {
                                c_new.etherDefense = (s16)(c_new.etherDefense + (s16)effect_24);
                            }
                            break;
                        }
                        case 2: {
                            s32 effect_2 = func_801DFD60(info, (void*)(u32)member, 0x0C);
                            if (effect_2 != 0) {
                                f32 scale_2 = 0.01f * (100.0f + (f32)effect_2);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_2);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_2);
                            }
                            break;
                        }
                        case 1: {
                            s32 effect_1 = func_801DFD60(info, (void*)(u32)member, 0x0B);
                            if (effect_1 != 0) {
                                f32 scale_1 = 0.01f * (100.0f + (f32)effect_1);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_1);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_1);
                            }
                            break;
                        }
                    }
                }
            // weight
            s32 wv = func_801DFD60(info, (void*)(u32)member, 0x97);
            if (wv != 0) {
                v484 = (s16)(v484 - (s16)wv);
                v468 = (s16)(v468 - (s16)wv);
                if (v484 < 0) v484 = 0;
                if (v468 < 0) v468 = 0;
            }
            // slot sum
            s32 ssum = (s32)*(s16*)((u8*)charObj + 0x22) + *(s16*)((u8*)charObj + 0x20) + *(s16*)((u8*)charObj + 0x24) + *(s16*)((u8*)charObj + 0x1C) + *(s16*)((u8*)charObj + 0x1E);
            if (ssum <= -5) {
                func_801DFD60(info, (void*)(u32)member, 0x1A);
            }
            // Recalculate the two armor values in candidate/current pairs.  Keeping
            // each result adjacent to its query mirrors the original UI pipeline
            // and makes the current-value subtraction explicit.
            s32 c1 = func_801DF610(info, (void*)(u32)member, 0x31, arg3);
            c_new.etherDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) *
                (f32)((stA->s32 - c_cur.etherDefense) + (c1 + c_new.etherDefense))));
            c1 = func_801DF610(info, (void*)(u32)member, 0x31, NULL);
            c_cur.etherDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) *
                (f32)(stA->s32 + c1)));
            c1 = func_801DF610(info, (void*)(u32)member, 0x51, arg3);
            c_new.physicalDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) *
                (f32)((stA->s2E - c_cur.physicalDefense) +
                      (c1 + c_new.physicalDefense))));
            c1 = func_801DF610(info, (void*)(u32)member, 0x51, NULL);
            c_cur.physicalDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) *
                (f32)(stA->s2E + c1)));
            s16 d1;
            s16 d2;
            s16 attackDelta = 0;
            s16 percentDelta;
            s16 agilityDelta;
            s16 b14;
            // embedded weapon block
            void* item2 = func_80157C4C(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                D8EEntry previewWeapon;
                func_801D4E2C(&previewWeapon, (void*)(u32)member,
                    (void*)(u32)(u16)(*(u32*)item2 >> 20));
                s16 newPercent = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) *
                    (f32)(stA->s38 + func_801DF610(
                        info, (void*)(u32)member, 0x54, arg3))));
                s16 oldPercent = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) *
                    (f32)(stA->s38 + func_801DF610(
                        info, (void*)(u32)member, 0x54, NULL))));
                percentDelta = (s16)(newPercent - oldPercent);
                char* percentSuffix = func_80136190(
                    &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
                textBuffer.format(&lbl_eu_805063BC[0x13E], newPercent,
                                   percentSuffix);
                func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16],
                              textBuffer.mString, 0);

                d1 = (s16)(c_new.etherDefense - c_cur.etherDefense);
                d2 = (s16)(c_new.physicalDefense - c_cur.physicalDefense);

                s16 oldAgility = (s16)(s32)(0.01f *
                    ((100.0f + (f32)stC->s0E + (f32)eq1) *
                     (f32)((stA->s1E - (s16)v484) + func_801DF610(
                         info, (void*)(u32)member, 0x41, NULL))));
                s16 newAgility = (s16)(s32)(0.01f *
                    ((100.0f + (f32)stC->s0E + (f32)eq2) *
                     (f32)((stA->s1E - (s16)v468) + func_801DF610(
                         info, (void*)(u32)member, 0x41, arg3))));
                if (oldAgility <= 0) oldAgility = 1;
                if (newAgility <= 0) newAgility = 1;
                agilityDelta = (s16)(newAgility - oldAgility);
                b14 = newAgility;

                D8EEntry ew;
                func_801D4E2C(&ew, (void*)(u32)member, (void*)(u32)(u16)(*(u32*)item2 >> 20));
                D8EEntry cw = ew;
                s32 c3 = func_801DF610(info, (void*)(u32)member, 0x1, NULL);
                s16 wpb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq1) * (f32)(stA->s1C + c3)));
                c3 = func_801DF610(info, (void*)(u32)member, 0x1, arg3);
                s16 wnb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + c3)));
                s32 wa = (s32)wpb + func_801DF4E0(info, (void*)(u32)member, (u16)cw.w04, NULL);
                s32 wb = (s32)wnb + func_801DF578(info, (void*)(u32)member, (u16)(cw.w04 >> 16), NULL);
                s32 wc = (s32)wnb + func_801DF4E0(info, (void*)(u32)member, (u16)cw.w04, arg3);
                s32 wd = (s32)wnb + func_801DF578(info, (void*)(u32)member, (u16)(cw.w04 >> 16), arg3);
                s32 wmin = wc < wd ? wc : wd;
                attackDelta = (s16)(wd - wa);
                char* rangeSeparator = func_80136190(
                    &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB);
                textBuffer.format(&lbl_eu_805063BC[0x254], wmin, rangeSeparator, wd);
                func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], textBuffer.mString, 0);

            }
            // numbers
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], c_new.physicalDefense);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], c_new.etherDefense);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], (s16)b14);
            // delta colors (4 rows)
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (attackDelta < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (attackDelta > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[11], &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[13], &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[15], &q1, &q2);
            }
            {
                D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
                D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
                D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
                D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
                if (percentDelta < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (percentDelta > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[7], &q1, &q2);
            }
            {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((agilityDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((agilityDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = (nw4r::lyt::Pane**)((u8*)info + 0x40);
        func_80139AC8(panes[(8)], &valueTop, &valueBottom);
        func_80139AC8(panes[(9)], &labelTop, &labelBottom);
}
        }

        // ItemBox1 also previews effects for the selected inventory slot. The
        // slot-aware helper mirrors func_801DF610 but includes the candidate
        // item and selected slot in its lookup.
        {
        s32 currentEquipBonus = 0;
        if (func_801DFFB8(info, member, NULL, NULL)) {
            currentEquipBonus = func_801DFD60(info, (void*)(u32)member, 0x30);
        }
        s16 oldStrength = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s10 + currentEquipBonus)) * (f32)((stA->s20) + (func_801DF610(info, (void*)(u32)member, 0x21, NULL)))));
        s16 newStrength = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s10 + currentEquipBonus)) * (f32)((stA->s20) + (func_801DF988(info, (void*)(u32)member, 0x21, arg3, slot)))));
        s16 strengthDelta = (s16)(newStrength - oldStrength);
        func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], newStrength);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((strengthDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((strengthDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = (nw4r::lyt::Pane**)((u8*)info + 0x40);
        func_80139AC8(panes[(8)], &valueTop, &valueBottom);
        func_80139AC8(panes[(9)], &labelTop, &labelBottom);
}

        s16 oldPhysical = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0C + currentEquipBonus)) * (f32)((stA->s1C) + (func_801DF610(info, (void*)(u32)member, 0x01, NULL)))));
        s16 newPhysical = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0C + currentEquipBonus)) * (f32)((stA->s1C) + (func_801DF988(info, (void*)(u32)member, 0x01, arg3, slot)))));
        s16 physicalDelta = (s16)(newPhysical - oldPhysical);
        func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], newPhysical);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((physicalDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((physicalDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = (nw4r::lyt::Pane**)((u8*)info + 0x40);
        func_80139AC8(panes[(4)], &valueTop, &valueBottom);
        func_80139AC8(panes[(5)], &labelTop, &labelBottom);
}

        s16 oldHp = (s16)(s32)(stA->f10 + (f32)func_801C6158(
            0.01f * stB->f10 * (f32)(stC->s06 + currentEquipBonus +
                func_801DF610(info, (void*)(u32)member, 0x11, NULL))));
        s16 newHp = (s16)(s32)(stA->f10 + (f32)func_801C6158(
            0.01f * stB->f10 * (f32)(stC->s06 + currentEquipBonus +
                func_801DF988(info, (void*)(u32)member, 0x11, arg3, slot))));
        if (oldHp > 9999) oldHp = 9999;
        if (newHp > 9999) newHp = 9999;
        s16 hpDelta = (s16)(newHp - oldHp);
        if (((float(*)(void*))vt[0x4A])(stats) > (f32)newHp) {
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], newHp);
        }
        func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], newHp);
        {
            D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
            if (hpDelta < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
            } else if (hpDelta > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
            }
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0],
                          &valueTop, &valueBottom);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[1],
                          &labelTop, &labelBottom);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2],
                          &valueTop, &valueBottom);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[3],
                          &labelTop, &labelBottom);
        }

        s16 oldEther = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s1C)) * (f32)((stA->s32) + (func_801DF610(info, (void*)(u32)member, 0x31, NULL)))));
        s16 newEther = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s1C)) * (f32)((stA->s32) + (func_801DF988(info, (void*)(u32)member, 0x31, arg3, slot)))));
        s16 etherDelta = (s16)(newEther - oldEther);
        func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], newEther);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((etherDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((etherDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = (nw4r::lyt::Pane**)((u8*)info + 0x40);
        func_80139AC8(panes[(12)], &valueTop, &valueBottom);
        func_80139AC8(panes[(13)], &labelTop, &labelBottom);
}

        s32 oldArtsTotal = (s32)func_801DFE48(info, member, NULL);
        s32 weightPenalty = (s32)func_801DFD60(info, (void*)(u32)member, 0x97);
        if (weightPenalty != 0) {
            oldArtsTotal -= weightPenalty;
            if (oldArtsTotal < 0) oldArtsTotal = 0;
        }
        s16 oldAgilityBase = (s16)(stA->s1E - (s16)oldArtsTotal);
        if (oldAgilityBase <= 0) oldAgilityBase = 1;
        s16 oldAgilityEffect = (s16)func_801DF610(
            info, (void*)(u32)member, 0x41, NULL);
        s16 oldAgility = (s16)(s32)(0.01f *
            ((100.0f + (f32)stC->s0E + (f32)currentEquipBonus) *
             (f32)(oldAgilityBase + oldAgilityEffect)));

        s16 newAgility = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0E)) * (f32)((oldAgilityBase) + (func_801DF988(info, (void*)(u32)member, 0x41, arg3, slot)))));
        s16 agilityDelta = (s16)(newAgility - oldAgility);
        func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], newAgility);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((agilityDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((agilityDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = (nw4r::lyt::Pane**)((u8*)info + 0x40);
        func_80139AC8(panes[(6)], &valueTop, &valueBottom);
        func_80139AC8(panes[(7)], &labelTop, &labelBottom);
}

        s16 oldDefense = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s18)) * (f32)((stA->s2E) + (func_801DF610(info, (void*)(u32)member, 0x51, NULL)))));
        s16 newDefense = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s18)) * (f32)((stA->s2E) + (func_801DF988(info, (void*)(u32)member, 0x51, arg3, slot)))));
        s16 defenseDelta = (s16)(newDefense - oldDefense);
        func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], newDefense);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((defenseDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((defenseDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = (nw4r::lyt::Pane**)((u8*)info + 0x40);
        func_80139AC8(panes[(14)], &valueTop, &valueBottom);
        func_80139AC8(panes[(15)], &labelTop, &labelBottom);
}

        s16 oldResistance = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s22)) * (f32)((stA->s38) + (func_801DF610(info, (void*)(u32)member, 0x54, NULL)))));
        s16 newResistance = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s22)) * (f32)((stA->s38) + (func_801DF988(info, (void*)(u32)member, 0x54, arg3, slot)))));
        char* percentSuffix = func_80136190(
            &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
        textBuffer.format(&lbl_eu_805063BC[0x13E], newResistance,
                              percentSuffix);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], textBuffer.mString, 0);
        s16 resistanceDelta = (s16)(newResistance - oldResistance);
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((resistanceDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((resistanceDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = (nw4r::lyt::Pane**)((u8*)info + 0x40);
        func_80139AC8(panes[(16)], &valueTop, &valueBottom);
        func_80139AC8(panes[(17)], &labelTop, &labelBottom);
}

        void* currentWeaponItem = func_80157C4C(2, *(s16*)((u8*)charObj + 0x26));
        s16 attackDelta = 0;
        if (currentWeaponItem != NULL && *(u32*)currentWeaponItem != 0) {
            D8EEntry currentWeapon;
            func_801D4E2C(&currentWeapon, (void*)(u32)member,
                (void*)(u32)(u16)(*(u32*)currentWeaponItem >> 20));
            D8EEntry weaponStats = currentWeapon;
            s32 oldBaseEffect = func_801DF610(
                info, (void*)(u32)member, 0x01, NULL);
            s16 oldAttackBase = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0C + currentEquipBonus)) * (f32)((stA->s1C) + (oldBaseEffect))));
            s32 newBaseEffect = func_801DF988(
                info, (void*)(u32)member, 0x01, arg3, slot);
            s16 newAttackBase = (s16)(s32)(0.01f * ((100.0f + (f32)(stC->s0C)) * (f32)((stA->s1C) + (newBaseEffect))));

            // The low-bound lookup is retained for its item-effect traversal;
            // the comparison display uses the high bound as its old endpoint.
            func_801DF4E0(info, (void*)(u32)member,
                          (u16)weaponStats.w04, NULL);
            s32 oldAttackHigh = oldAttackBase + func_801DF578(
                info, (void*)(u32)member,
                (u16)(weaponStats.w04 >> 16), NULL);

            s32 lowScale = (s32)func_801DFD60(
                info, (void*)(u32)member, 0x2D);
            lowScale += (s32)func_801DF988(
                info, (void*)(u32)member, 0x52, arg3, slot);
            s32 newAttackLow = newAttackBase + (s32)(0.01f *
                (f32)((u16)weaponStats.w04 * (100 + lowScale)));

            s32 highScale = (s32)func_801DFD60(
                info, (void*)(u32)member, 0x2D);
            highScale += (s32)func_801DF988(
                info, (void*)(u32)member, 0x53, arg3, slot);
            s32 newAttackHigh = newAttackBase + (s32)(0.01f *
                (f32)((u16)(weaponStats.w04 >> 16) * (100 + highScale)));
            s32 displayLow = newAttackLow < newAttackHigh
                ? newAttackLow : newAttackHigh;
            attackDelta = (s16)(newAttackHigh - oldAttackHigh);

            char* rangeSeparator = func_80136190(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB);
            textBuffer.format(&lbl_eu_805063BC[0x254], displayLow,
                              rangeSeparator, newAttackHigh);
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10],
                          textBuffer.mString, 0);
        }
        {
        D8EQuad labelTop = *(D8EQuad*)&lbl_eu_80664518;
        D8EQuad labelBottom = *(D8EQuad*)&lbl_eu_80664520;
        D8EQuad valueTop = *(D8EQuad*)&lbl_eu_80664558;
        D8EQuad valueBottom = *(D8EQuad*)&lbl_eu_80664560;
        if ((attackDelta) < 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664538);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664540);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664578);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664580);
        } else if ((attackDelta) > 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&labelTop, &lbl_eu_80664528);
            __as__11_GXColorS10FRC11_GXColorS10(&labelBottom, &lbl_eu_80664530);
            __as__11_GXColorS10FRC11_GXColorS10(&valueTop, &lbl_eu_80664568);
            __as__11_GXColorS10FRC11_GXColorS10(&valueBottom, &lbl_eu_80664570);
        }
        nw4r::lyt::Pane** panes = (nw4r::lyt::Pane**)((u8*)info + 0x40);
        func_80139AC8(panes[(10)], &valueTop, &valueBottom);
        func_80139AC8(panes[(11)], &labelTop, &labelBottom);
}
        }

    }

    // ---- equip-detail / simple path (arg4 == 0, type in {2,4,5,6,7,8}) ----
    if (arg4 == 0 && (type == 2 || (type >= 4 && type <= 8))) {
        if (type != 2 && func_801DFDC0(info, member, arg3)) {
            // ---- equip detail (0x801E8C88) ----
            s16 slotId = 0;
            switch (type) {
                case 2: slotId = *(s16*)((u8*)charObj + 0x26); break;
                case 4: slotId = *(s16*)((u8*)charObj + 0x1C); break;
                case 5: slotId = *(s16*)((u8*)charObj + 0x1E); break;
                case 6: slotId = *(s16*)((u8*)charObj + 0x20); break;
                case 7: slotId = *(s16*)((u8*)charObj + 0x22); break;
                case 8: slotId = *(s16*)((u8*)charObj + 0x24); break;
            }
            void* item = func_80157C4C(type, slotId);
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            D8EArmorEntry e_cur;
            func_801D5274(&e_cur, (void*)(u32)member, (void*)(u32)w0);
            D8EArmorEntry c_cur;
for (u32 w_ = 0; w_ < 7; w_++) {
                c_cur.words[w_] = e_cur.words[w_];
            }
            D8EArmorEntry e_new;
            func_801D5274(&e_new, (void*)(u32)member, arg3);
            D8EArmorEntry c_new;
for (u32 w_ = 0; w_ < 7; w_++) {
                c_new.words[w_] = e_new.words[w_];
            }
            volatile s16 v44C = (s16)func_801DFE48(
                info, member,
                (item != NULL && *(u32*)item != 0)
                    ? (void*)(u32)(u16)(*(u32*)item >> 20)
                    : NULL);
            volatile s16 v430 = (s16)func_801DFE48(info, member, arg3);
            s32 eq1 = 0;
            if (func_801DFFB8(info, member, NULL, NULL)) eq1 = func_801DFD60(info, (void*)(u32)member, 0x30);
            s32 eq2 = 0;
            if (func_801DFFB8(info, member, arg3, NULL)) eq2 = func_801DFD60(info, (void*)(u32)member, 0x30);
            func_801DF610(info, (void*)(u32)member, 0x21, NULL);
            s16 bA = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)eq2) * (f32)(stA->s20 + func_801DF610(info, (void*)(u32)member, 0x21, arg3))));
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], (s16)bA);
            func_801DF610(info, (void*)(u32)member, 0x1, NULL);
            s16 bB = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + func_801DF610(info, (void*)(u32)member, 0x1, arg3))));
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], (s16)bB);
            func_801C6158(0.01f * stB->f10 * (f32)(stC->s06 + eq1 + func_801DF610(info, (void*)(u32)member, 0x11, NULL)));
            s32 r6158 = func_801C6158(0.01f * stB->f10 * (f32)(stC->s06 + eq2 + func_801DF610(info, (void*)(u32)member, 0x11, arg3)));
            s16 nb = (s16)(s32)(stA->f10 + (f32)r6158);
            if (nb > 9999) nb = 9999;
            if (((float(*)(void*))vt[0x4A])(stats) > (f32)nb) {
                func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], nb);
            }
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], nb);
            // arm-type scaling of entry value pairs
            u8 arm1 = (u8)func_801361E8(
                (u32)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB],
                func_80139358((u32)((item != NULL && *(u32*)item != 0)
                    ? (u16)(*(u32*)item >> 20)
                    : 0)));
            {
                    switch (arm1) {
                        case 3: {
                            s32 effect_3 = func_801DFD60(info, (void*)(u32)member, 0x0D);
                            if (effect_3 != 0) {
                                f32 scale_3 = 0.01f * (100.0f + (f32)effect_3);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_3);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_3);
                            }
                            s32 effect_24 = func_801DFD60(info, (void*)(u32)member, 0x24);
                            if (effect_24 != 0) {
                                c_cur.etherDefense = (s16)(c_cur.etherDefense + (s16)effect_24);
                            }
                            break;
                        }
                        case 2: {
                            s32 effect_2 = func_801DFD60(info, (void*)(u32)member, 0x0C);
                            if (effect_2 != 0) {
                                f32 scale_2 = 0.01f * (100.0f + (f32)effect_2);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_2);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_2);
                            }
                            break;
                        }
                        case 1: {
                            s32 effect_1 = func_801DFD60(info, (void*)(u32)member, 0x0B);
                            if (effect_1 != 0) {
                                f32 scale_1 = 0.01f * (100.0f + (f32)effect_1);
                                c_cur.physicalDefense = (s16)(s32)((f32)c_cur.physicalDefense * scale_1);
                                c_cur.etherDefense = (s16)(s32)((f32)c_cur.etherDefense * scale_1);
                            }
                            break;
                        }
                    }
                }
            u8 arm2 = (u8)func_801361E8((u32)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], func_80139358((u32)(*(u32*)arg3 >> 20)));
            {
                    switch (arm2) {
                        case 3: {
                            s32 effect_3 = func_801DFD60(info, (void*)(u32)member, 0x0D);
                            if (effect_3 != 0) {
                                f32 scale_3 = 0.01f * (100.0f + (f32)effect_3);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_3);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_3);
                            }
                            s32 effect_24 = func_801DFD60(info, (void*)(u32)member, 0x24);
                            if (effect_24 != 0) {
                                c_new.etherDefense = (s16)(c_new.etherDefense + (s16)effect_24);
                            }
                            break;
                        }
                        case 2: {
                            s32 effect_2 = func_801DFD60(info, (void*)(u32)member, 0x0C);
                            if (effect_2 != 0) {
                                f32 scale_2 = 0.01f * (100.0f + (f32)effect_2);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_2);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_2);
                            }
                            break;
                        }
                        case 1: {
                            s32 effect_1 = func_801DFD60(info, (void*)(u32)member, 0x0B);
                            if (effect_1 != 0) {
                                f32 scale_1 = 0.01f * (100.0f + (f32)effect_1);
                                c_new.physicalDefense = (s16)(s32)((f32)c_new.physicalDefense * scale_1);
                                c_new.etherDefense = (s16)(s32)((f32)c_new.etherDefense * scale_1);
                            }
                            break;
                        }
                    }
                }
            // weight
            s32 wv = func_801DFD60(info, (void*)(u32)member, 0x97);
            if (wv != 0) {
                v44C = (s16)(v44C - (s16)wv);
                v430 = (s16)(v430 - (s16)wv);
                if (v44C < 0) v44C = 0;
                if (v430 < 0) v430 = 0;
            }
            // slot sum
            s32 ssum = (s32)*(s16*)((u8*)charObj + 0x22) + *(s16*)((u8*)charObj + 0x20) + *(s16*)((u8*)charObj + 0x24) + *(s16*)((u8*)charObj + 0x1C) + *(s16*)((u8*)charObj + 0x1E);
            if (ssum <= -5) {
                func_801DFD60(info, (void*)(u32)member, 0x1A);
            }
            // Recalculate candidate and current armor values in matched pairs.
            s32 c1 = func_801DF610(info, (void*)(u32)member, 0x31, arg3);
            c_new.etherDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) *
                (f32)((stA->s32 - c_cur.etherDefense) + (c1 + c_new.etherDefense))));
            c1 = func_801DF610(info, (void*)(u32)member, 0x31, NULL);
            c_cur.etherDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) *
                (f32)(stA->s32 + c1)));
            c1 = func_801DF610(info, (void*)(u32)member, 0x51, arg3);
            c_new.physicalDefense = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) *
                (f32)((stA->s2E - c_cur.physicalDefense) +
                      (c1 + c_new.physicalDefense))));
            c1 = func_801DF610(info, (void*)(u32)member, 0x51, NULL);
            volatile s16 currentPhysicalDefense = (s16)(s32)(0.01f *
                ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            func_801DF610(info, (void*)(u32)member, 0x41, NULL);
            s32 c2 = func_801DF610(info, (void*)(u32)member, 0x41, arg3);
            s16 b14 = (s16)(s32)(0.01f * ((100.0f + (f32)eq2) *
                (f32)((stA->s1E - (s16)v430) + c2)));
            if (b14 <= 0) b14 = 1;
            // embedded weapon block
            void* item2 = func_80157C4C(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                D8EEntry ew;
                func_801D4E2C(&ew, (void*)(u32)member, (void*)(u32)(u16)(*(u32*)item2 >> 20));
                D8EEntry cw = ew;
                func_801DF610(info, (void*)(u32)member, 0x1, NULL);
                s16 wpb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C) *
                    (f32)(stA->s1C + func_801DF610(
                        info, (void*)(u32)member, 0x1, arg3))));
                s32 wa = (s32)wpb + func_801DF4E0(info, (void*)(u32)member, (u16)cw.w04, NULL);
                s32 wb = (s32)wpb + func_801DF578(info, (void*)(u32)member, (u16)(cw.w04 >> 16), NULL);
                s32 wc = (s32)wpb + func_801DF4E0(info, (void*)(u32)member, (u16)cw.w04, arg3);
                s32 wd = (s32)wpb + func_801DF578(info, (void*)(u32)member, (u16)(cw.w04 >> 16), arg3);
                s32 wmin = wc < wd ? wc : wd;
                char* rangeSeparator = func_80136190(
                    &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB);
                textBuffer.format(&lbl_eu_805063BC[0x254], wmin, rangeSeparator, wd);
                func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], textBuffer.mString, 0);
            }
            // numbers
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], c_new.physicalDefense);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], c_new.etherDefense);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], (s16)b14);
            // 20x color application (selected palette A8/B0/C8/D0)
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_806645A8;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_806645B0;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_806645C8;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_806645D0;
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[3], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[1], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[5], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[7], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[9], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[11], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[13], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[15], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[17], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[19], &q1, &q2);

        }
        // ---- simple path ----
        D8EQuad q1 = *(D8EQuad*)&lbl_eu_806645A8;
        D8EQuad q2 = *(D8EQuad*)&lbl_eu_806645B0;
        D8EQuad q3 = *(D8EQuad*)&lbl_eu_806645C8;
        D8EQuad q4 = *(D8EQuad*)&lbl_eu_806645D0;
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], &lbl_eu_805063BC[0x2AA], 0);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], &lbl_eu_805063BC[0x2AA], 0);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[3], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[1], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[5], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[7], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[9], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[11], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[13], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[15], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[17], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[19], &q1, &q2);
    }

    // ---- common tail: two 8-entry item lists ----
    CItemBoxInfoEntry listA[8];
    CItemBoxInfoEntry listB[8];
    listA[0].itemId = 0;
    listA[0].value = 0;
    listA[0].state = 0;
    setItemBoxEntry(&listA[1], 0, 0, 0);
    setItemBoxEntry(&listA[2], 0, 0, 0);
    setItemBoxEntry(&listA[3], 0, 0, 0);
    setItemBoxEntry(&listA[4], 0, 0, 0);
    setItemBoxEntry(&listA[5], 0, 0, 0);
    setItemBoxEntry(&listA[6], 0, 0, 0);
    setItemBoxEntry(&listA[7], 0, 0, 0);
    listB[0].itemId = 0;
    listB[0].value = 0;
    listB[0].state = 0;
    setItemBoxEntry(&listB[1], 0, 0, 0);
    setItemBoxEntry(&listB[2], 0, 0, 0);
    setItemBoxEntry(&listB[3], 0, 0, 0);
    setItemBoxEntry(&listB[4], 0, 0, 0);
    setItemBoxEntry(&listB[5], 0, 0, 0);
    setItemBoxEntry(&listB[6], 0, 0, 0);
    setItemBoxEntry(&listB[7], 0, 0, 0);

    u8 listIdxA = 0;
    u8 listIdxB = 0;

    // weapon slot
    {
        s16 weaponId = *(s16*)((u8*)charObj + 0x26);
        s16 headId = *(s16*)((u8*)charObj + 0x1C);
        s16 torsoId = *(s16*)((u8*)charObj + 0x1E);
        s16 armId = *(s16*)((u8*)charObj + 0x20);
        s16 legId = *(s16*)((u8*)charObj + 0x22);
        s16 footId = *(s16*)((u8*)charObj + 0x24);
        void* itemW = func_80157C4C(2, weaponId);
        void* items[5];
        items[0] = func_80157C4C(4, headId);
        items[1] = func_80157C4C(5, torsoId);
        items[2] = func_80157C4C(6, armId);
        items[3] = func_80157C4C(7, legId);
        items[4] = func_80157C4C(8, footId);
        if (itemW != NULL) {
            void* itemImpl = CItem_initItemImplInstances(itemW);
            u8 cnt = ((u8(*)(void*, void*))(*(void***)itemImpl)[12])(itemImpl, itemW);
            for (u8 i = 0; i < 3; i++) {
                CItemBoxInfoEntry tmp;
                tmp.itemId = 0;
                tmp.value = 0;
                tmp.state = 0;
                if (i < cnt) {
                    void* recordImpl = CItem_initItemImplInstances(itemW);
                    void* e = ((void*(*)(void*, void*, u32))(*(void***)recordImpl)[11])(
                        recordImpl, itemW, i);
                    if (e != NULL) {
                        u32 w0 = *(u32*)e;
                        u16 w1 = *(u16*)((u8*)e + 4);
                        CItemBoxInfoEntry tmp2;
                        setItemBoxEntry(&tmp2, (s16)((w0 >> 11) & 0x7FF),
                                        (w0 >> 22) & 7, (w1 >> 4) & 0xFFF);
                        copyItemBoxEntry(&tmp, &tmp2);
                    }
                }
                copyItemBoxEntry(&listA[listIdxA++], &tmp);
                if (type != 2) {
                    copyItemBoxEntry(&listB[listIdxB++], &tmp);
                }
            }
        }
        // Candidate weapon gems use two retail representations. Kind 3 stores
        // one synthesized record at the selected slot; other weapon kinds
        // expose up to three ordinary sub-records through vf30/vf2C.
        if (type == 2) {
            void* candidateWeapon = arg3 != NULL ? arg3 : NULL;
            if (candidateWeapon != NULL && *(u32*)candidateWeapon != 0) {
            bool synthesized = ((*(u32*)arg3 >> 16) & 0xF) == 3;
            if (synthesized) {
                if (arg4 != 0) {
                    for (u8 i = 0; i < 3; i++) {
                        CItemBoxInfoEntry tmp = {0, 0, 0};
                        if (i == slot - 1) {
                            CItemBoxInfoEntry built;
                            void* idImpl = CItem_initItemImplInstances(arg3);
                            u16 itemId = (u16)((u32(*)(void*, void*))(*(void***)idImpl)[2])(
                                idImpl, arg3);
                            void* valueImpl = CItem_initItemImplInstances(arg3);
                            u32 value = ((u32(*)(void*, void*))(*(void***)valueImpl)[36])(
                                valueImpl, arg3);
                            void* stateImpl = CItem_initItemImplInstances(arg3);
                            u8 state = (u8)((u32(*)(void*, void*))(*(void***)stateImpl)[21])(
                                stateImpl, arg3);
                            setItemBoxEntry(&built, itemId, value, state);
                            copyItemBoxEntry(&tmp, &built);
                        }
                        copyItemBoxEntry(&listB[listIdxB++], &tmp);
                    }
                } else {
                    { CItemBoxInfoEntry blank = {0, 0, 0};
                      copyItemBoxEntry(&listB[listIdxB++], &blank); }
                    { CItemBoxInfoEntry blank = {0, 0, 0};
                      copyItemBoxEntry(&listB[listIdxB++], &blank); }
                    { CItemBoxInfoEntry blank = {0, 0, 0};
                      copyItemBoxEntry(&listB[listIdxB++], &blank); }
                }
            } else if (arg4 != 0) {
                void* countImpl = CItem_initItemImplInstances(arg3);
                u8 count = ((u8(*)(void*, void*))(*(void***)countImpl)[12])(countImpl, arg3);
                for (u8 i = 0; i < 3; i++) {
                    CItemBoxInfoEntry tmp = {0, 0, 0};
                    if (i < count) {
                        void* recordImpl = CItem_initItemImplInstances(arg3);
                        void* record = ((void*(*)(void*, void*, u32))(*(void***)recordImpl)[11])(
                            recordImpl, arg3, i);
                        if (record != NULL) {
                            u32 packed = *(u32*)record;
                            u16 tail = *(u16*)((u8*)record + 4);
                            CItemBoxInfoEntry built;
                            setItemBoxEntry(&built, (packed >> 11) & 0x7FF,
                                (packed >> 22) & 7, (tail >> 4) & 0xFFF);
                            copyItemBoxEntry(&tmp, &built);
                        }
                    }
                    copyItemBoxEntry(&listB[listIdxB++], &tmp);
                }
            } else {
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
            }
            } else {
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
                { CItemBoxInfoEntry blank = {0, 0, 0};
                  copyItemBoxEntry(&listB[listIdxB++], &blank); }
            }
        }
        // armor slots
        {
            u8 slotTypes[5];
            *(u32*)&slotTypes[0] = lbl_eu_8066806C;
            slotTypes[4] = lbl_eu_80668070;
            for (u8 i = 0; i < 5; i++) {
                CItemBoxInfoEntry tmp;
                tmp.itemId = 0;
                tmp.value = 0;
                tmp.state = 0;
                void* item = items[i];
                if (item != NULL) {
                    void* itemImpl = CItem_initItemImplInstances(item);
                    if (((u8(*)(void*, void*))(*(void***)itemImpl)[12])(itemImpl, item) != 0) {
                        void* recordImpl = CItem_initItemImplInstances(item);
                        void* e = ((void*(*)(void*, void*, u32))(*(void***)recordImpl)[11])(
                            recordImpl, item, 0);
                        if (e != NULL) {
                            u32 w0 = *(u32*)e;
                            u16 w1 = *(u16*)((u8*)e + 4);
                            CItemBoxInfoEntry tmp2;
                            setItemBoxEntry(&tmp2, (s16)((w0 >> 11) & 0x7FF),
                                            (w0 >> 22) & 7, (w1 >> 4) & 0xFFF);
                            copyItemBoxEntry(&tmp, &tmp2);
                        }
                    }
                }
                copyItemBoxEntry(&listA[listIdxA++], &tmp);
                if (type != slotTypes[i]) {
                    copyItemBoxEntry(&listB[listIdxB++], &tmp);
                }
                if (type == slotTypes[i]) {
                    CItemBoxInfoEntry tmp3;
                    tmp3.itemId = 0;
                    tmp3.value = 0;
                    tmp3.state = 0;
                    if (arg3 != NULL && *(u32*)arg3 != 0 &&
                        ((*(u32*)arg3 >> 16) & 0xF) == 3) {
                            CItemBoxInfoEntry built = {0, 0, 0};
                            void* idImpl = CItem_initItemImplInstances(arg3);
                            u16 itemId = (u16)((u32(*)(void*, void*))(*(void***)idImpl)[2])(
                                idImpl, arg3);
                            void* valueImpl = CItem_initItemImplInstances(arg3);
                            u32 value = ((u32(*)(void*, void*))(*(void***)valueImpl)[36])(
                                valueImpl, arg3);
                            void* stateImpl = CItem_initItemImplInstances(arg3);
                            u8 state = (u8)((u32(*)(void*, void*))(*(void***)stateImpl)[21])(
                                stateImpl, arg3);
                            setItemBoxEntry(&built, itemId, value, state);
                            copyItemBoxEntry(&tmp3, &built);
                    } else if (arg4 != 0 || arg3 != NULL) {
                            void* countImpl = CItem_initItemImplInstances(arg3);
                            u8 count = ((u8(*)(void*, void*))(*(void***)countImpl)[12])(
                                countImpl, arg3);
                            if (count != 0) {
                                void* recordImpl = CItem_initItemImplInstances(arg3);
                                void* record = ((void*(*)(void*, void*, u32))(*(void***)recordImpl)[11])(
                                    recordImpl, arg3, 0);
                                if (record != NULL) {
                                    u32 packed = *(u32*)record;
                                    u16 tail = *(u16*)((u8*)record + 4);
                                    CItemBoxInfoEntry built;
                                    setItemBoxEntry(&built, (packed >> 11) & 0x7FF,
                                        (packed >> 22) & 7, (tail >> 4) & 0xFFF);
                                    copyItemBoxEntry(&tmp3, &built);
                                }
                            }
                    }
                    copyItemBoxEntry(&listB[listIdxB++], &tmp3);
                }
            }
        }

        // Type 3 replaces a crystal rather than a whole equipment slot. Find
        // the matching equipped sub-record and synthesize the replacement at
        // the resolved list position.
        if (type == 3) {
            CItemBoxInfoEntry candidate = {0, 0, 0};
            void* replacement = arg3 != NULL ? arg3 : NULL;
            if (replacement != NULL) {
            struct EquippedTypes {
                u32 values[6];
            };
            EquippedTypes equippedTypes =
                *(const EquippedTypes*)lbl_eu_80506368;
            s16 equippedIds[6] = {
                *(s16*)((u8*)charObj + 0x26), *(s16*)((u8*)charObj + 0x1C),
                *(s16*)((u8*)charObj + 0x1E), *(s16*)((u8*)charObj + 0x20),
                *(s16*)((u8*)charObj + 0x22), *(s16*)((u8*)charObj + 0x24)
            };
            u8 destination = 0;
            for (u8 equipmentIndex = 0; equipmentIndex < 6 && destination == 0;
                 equipmentIndex++) {
                void* equipped = func_80157C4C(
                                                (u8)equippedTypes.values[equipmentIndex],
                                                equippedIds[equipmentIndex]);
                if (equipped == NULL || *(u32*)equipped == 0) continue;

                void* countImpl = CItem_initItemImplInstances(equipped);
                u16 count = (u16)((u32(*)(void*, void*))(*(void***)countImpl)[12])(
                    countImpl, equipped);
                for (u8 itemIndex = 0; itemIndex < count; itemIndex++) {
                    void* indexImpl = CItem_initItemImplInstances(equipped);
                    s16 crystalId = (s16)((s32(*)(void*, void*, u32))(*(void***)indexImpl)[16])(
                        indexImpl, equipped, itemIndex);
                    if (crystalId != -1) {
                        void* crystal = func_80157C4C(3, crystalId);
                        if (crystal != NULL && *(u32*)crystal != 0 &&
                            crystal == replacement) {
                            destination = equipmentIndex == 0 ? itemIndex + 1
                                                              : equipmentIndex + 3;
                            break;
                        }
                    } else if (arg4 == 0) {
                        void* recordImpl = CItem_initItemImplInstances(equipped);
                        void* record = ((void*(*)(void*, void*, u32))(*(void***)recordImpl)[11])(
                            recordImpl, equipped, itemIndex);
                        if (record == NULL || (*(u16*)((u8*)record + 4) & 1) == 0) continue;

                        void* candidateStateImpl = CItem_initItemImplInstances(arg3);
                        u16 candidateState = (u16)((u32(*)(void*, void*))
                            (*(void***)candidateStateImpl)[21])(candidateStateImpl, arg3);
                        s16 recordValue = (s16)((*(u32*)record >> 11) & 0x7FF);
                        void* candidateValueImpl = CItem_initItemImplInstances(arg3);
                        u32 candidateValue = ((u32(*)(void*, void*))
                            (*(void***)candidateValueImpl)[36])(candidateValueImpl, arg3);
                        if (((*(u16*)((u8*)record + 4) >> 4) & 0xFFF) == candidateState &&
                            recordValue == candidateValue) {
                            destination = equipmentIndex == 0 ? itemIndex + 1
                                                              : equipmentIndex + 3;
                            break;
                        }
                    }
                }
            }

            if (destination == 0) destination = slot;
            void* idImpl = CItem_initItemImplInstances(arg3);
            u16 itemId = (u16)((u32(*)(void*, void*))(*(void***)idImpl)[2])(idImpl, arg3);
            void* valueImpl = CItem_initItemImplInstances(arg3);
            u32 value = ((u32(*)(void*, void*))(*(void***)valueImpl)[36])(valueImpl, arg3);
            void* stateImpl = CItem_initItemImplInstances(arg3);
            u8 state = (u8)((u32(*)(void*, void*))(*(void***)stateImpl)[21])(stateImpl, arg3);
            CItemBoxInfoEntry built;
            setItemBoxEntry(&built, itemId, value, state);
            copyItemBoxEntry(&candidate, &built);
            copyItemBoxEntry(&listB[destination - 1], &candidate);
            }
        }
    }

    // ---- compact listA (dedupe by stackable flag, sum values, cap) ----
    {
        u16 ids[8] = {0};
        u32 vals[8] = {0};
        u8 flags[8] = {0};
        u16 ids2[8] = {0};
        u32 vals2[8] = {0};
        u8 flags2[8] = {0};
        u8 n = 0;
        for (u8 i = 0; i < 8; i++) {
            u16 id = listA[i].itemId;
            u32 val = listA[i].value;
            u8 flag = listA[i].state;
            if (id == 0) {
                ids[n] = id;
                vals[n] = val;
                flags[n] = flag;
                n++;
            } else if (func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], id) != 0) {
                u8 found = 0;
                for (u8 j = 0; j < n; j++) {
                    if (id == ids[j]) {
                        vals[j] += val;
                        for (u8 k = j + 1; k < n; k++) {
                            if (id == ids[k]) vals[k] = vals[j];
                        }
                        found = 1;
                        ids[n] = id;
                        vals[n] = vals[j];
                        flags[n] = flag;
                        n++;
                        break;
                    }
                }
                if (!found) {
                    ids[n] = id;
                    vals[n] = val;
                    flags[n] = flag;
                    n++;
                }
            }
        }
        // same for listB
        u8 n2 = 0;
        for (u8 i = 0; i < 8; i++) {
            u16 id = listB[i].itemId;
            u32 val = listB[i].value;
            u8 flag = listB[i].state;
            if (id == 0) {
                ids2[n2] = id;
                vals2[n2] = val;
                flags2[n2] = flag;
                n2++;
            } else if (func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], id) != 0) {
                u8 found = 0;
                for (u8 j = 0; j < n2; j++) {
                    if (id == ids2[j]) {
                        vals2[j] += val;
                        for (u8 k = j + 1; k < n2; k++) {
                            if (id == ids2[k]) vals2[k] = vals2[j];
                        }
                        found = 1;
                        ids2[n2] = id;
                        vals2[n2] = vals2[j];
                        flags2[n2] = flag;
                        n2++;
                        break;
                    }
                }
                if (!found) {
                    ids2[n2] = id;
                    vals2[n2] = val;
                    flags2[n2] = flag;
                    n2++;
                }
            }
        }
        for (u8 i = 0; i < 8; i++) {
            if (ids[i] != 0) {
                u32 cap = func_80136254(lbl_eu_806640D8,
                                       &lbl_eu_805063BC[0x503], ids[i]);
                if (vals[i] > cap) vals[i] = cap;
            }
            if (ids2[i] != 0) {
                u32 cap = func_80136254(lbl_eu_806640D8, &lbl_eu_805063BC[0x503], ids2[i]);
                if (vals2[i] > cap) vals2[i] = cap;
            }
        }

        // better/worse flags comparing listB vs listA
        u8 cmp[8] = {0};
        for (u8 i = 0; i < 8; i++) {
            u16 compareId = ids2[i];
            s32 compareValue = (s32)vals2[i];
            if (compareId != 0 && func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], compareId) != 0) {
                u8 done = 0;
                for (u8 j = 0; j < 8; j++) {
                    if (compareId == ids[j]) {
                        if (compareValue > (s32)vals[j]) cmp[i] = 1;
                        else if (compareValue < (s32)vals[j]) cmp[i] = 2;
                        done = 1;
                        break;
                    }
                }
                if (!done) cmp[i] = 1;
            } else {
                u8 done = 0;
                if (compareId == ids[i]) {
                    if (compareValue > (s32)vals[i]) cmp[i] = 1;
                    else if (compareValue < (s32)vals[i]) cmp[i] = 2;
                    done = 1;
                }
                if (!done) cmp[i] = 1;
            }
        }
        // clear duplicate ids in listB
        for (u8 i = 0; i < 8; i++) {
            if (ids2[i] != 0 && func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], ids2[i]) != 0) {
                for (u8 j = i + 1; j < 8; j++) {
                    if (ids2[i] == ids2[j]) ids2[j] = 0;
                }
            }
        }

        // pane name setup
        nw4r::lyt::Pane* root = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(&lbl_eu_805063BC[0x16E], true);
        for (u8 i = 0; i < 8; i++) {
            char buf1[0x20];
            char buf2[0x20];
            sprintf(buf1, &lbl_eu_805063BC[0x507], i * 2 + 0x13);
            sprintf(buf2, &lbl_eu_805063BC[0x515], i + 0x10);
            func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf1, &lbl_eu_805063BC[0x2AA], 0);
            func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf2, &lbl_eu_805063BC[0x2AA], 0);
        }

        // per-slot render
        D8ERenderQuad quad;
        for (u8 i = 0; i < 8; i++) {
            u16 id = ids2[i];
            u32 val = vals2[i];
            u8 flag = flags2[i];
            u8 cf = cmp[i];
            if (id == 0) continue;
            char buf1[0x20];
            char buf2[0x20];
            sprintf(buf1, &lbl_eu_805063BC[0x507], i * 2 + 0x13);
            sprintf(buf2, &lbl_eu_805063BC[0x515], i + 0x10);
            nw4r::lyt::Pane* pane1 = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(buf1, true);
            func_80137924((nw4r::math::VEC3*)&quad, pane1, root, ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10)));
            // Transpose alternating UI rows into the two four-slot columns.
            s32 idx = (s32)(0.5f * (f32)i);
            if (((s8)i % 2) != 0) idx += 4;
            if ((u8)idx < 12) {
                *(s16*)((u8*)info + 0xB0 + (u8)idx * 2) = id;
            }
            if ((u8)idx < 12) {
                *(u8*)((u8*)info + 0x158 + (u8)idx) = 3;
            }
            if ((u8)idx < 12) {
                *(u8*)((u8*)info + 0x164 + (u8)idx) = flag;
            }
            if ((u8)idx < 12) {
                *(s16*)((u8*)info + 0x170 + (u8)idx * 2) = (s16)val;
            }
            u32 dstWords[3] = {
                ((u32*)&quad)[0], ((u32*)&quad)[1], ((u32*)&quad)[2]
            };
            if ((u8)idx < 12) {
                copyVEC3((void*)((u8*)info + 0xC8 + (u8)idx * 12),
                         (void*)dstWords);
            }
            func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf1, func_8013639C(lbl_eu_806640D8, &lbl_eu_805063BC[0x139]), 0);
            if (val == 0) {
                func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34),
                              buf2, &lbl_eu_805063BC[0x2AA], 0);
            }
            u8 rvs = (u8)func_801361E8((u32)lbl_eu_806640D8,
                                       &lbl_eu_805063BC[0x3], id);
            if (rvs != 0) {
                if (rvs == 0xFF || rvs == 0xFE) {
                    textBuffer.format(&lbl_eu_805063BC[0x13E], (s32)val,
                        func_80136190(&lbl_eu_805063BC[0x130],
                                      &lbl_eu_805063BC[0x139], 0x21));
                } else {
                    textBuffer.format(&lbl_eu_805063BC[0x422], (s32)val);
                }
            } else {
                textBuffer.format(&lbl_eu_805063BC[0x422], (s32)val);
            }
            func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34),
                          buf2, textBuffer.mString, 0);
            if (arg3 != NULL && arg4 == 0 &&
                (cf != 0 || type == 2 || !func_801DFDC0(info, member, arg3))) {
                cf = 0;
                func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34),
                              buf2, &lbl_eu_805063BC[0x2AA], 0);
            }
            D8EQuad q1 = *(D8EQuad*)&lbl_eu_80664518;
            D8EQuad q2 = *(D8EQuad*)&lbl_eu_80664520;
            D8EQuad q3 = *(D8EQuad*)&lbl_eu_80664558;
            D8EQuad q4 = *(D8EQuad*)&lbl_eu_80664560;
            u32 cap = func_80136254(lbl_eu_806640D8, &lbl_eu_805063BC[0x503], id);
            if ((u32)val >= cap) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664548);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664550);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664588);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664590);
            } else if (cf == 1) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            } else if (cf == 2) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            }
            if (arg4 == 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_806645A8);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_806645B0);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_806645C8);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_806645D0);
            }
            func_80139A18((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf1, &q1, &q2);
            func_80139A18((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf2, &q3, &q4);
        }
    }
}
#pragma pop

#undef w04
#undef _0C
#undef _10
#undef w14

#pragma push
#pragma auto_inline off
void copyItemBoxEntry(CItemBoxInfoEntry* dst, const CItemBoxInfoEntry* src) {
    dst->itemId = src->itemId;
    dst->value = src->value;
    dst->state = src->state;
}

void setItemBoxEntry(CItemBoxInfoEntry* self, u16 a, u32 b, u8 c) {
    *(u16*)self = a;
    *(u32*)((u8*)self + 4) = b;
    *(u8*)((u8*)self + 8) = c;
}

s32 func_801DF4E0(void* a, void* b, s32 arg2, void* d) {
    // MWCC emits the 0x4330/xoris int->float trick natively for the plain
    // (f32) cast on Broadway (no fcfid); keep it as a simple cast.
    // Same shape as func_801DF578: statement-ordered calls, builtin
    // s32->f32 cast emits MWCC's 0x4330/xoris double-trick.
    u32 t1 = func_801DFD60(a, b, 0x2d);
    u32 t2 = func_801DF610(a, b, 0x52, d);
    return (s32)(lbl_eu_80668040 * (f32)(arg2 * (s32)(t2 + t1 + 0x64)));
}

s32 func_801DF578(void* a, void* b, s32 arg2, void* d) {
    // Statement-ordered calls keep the retail bl order; single-expression sum
    // keeps the values in registers like retail. The builtin s32->f32 cast
    // emits MWCC's 0x4330/xoris double-trick.
    u32 t1 = func_801DFD60(a, b, 0x2d);
    u32 t2 = func_801DF610(a, b, 0x53, d);
    return (s32)(lbl_eu_80668040 * (float)(arg2 * (s32)(t2 + t1 + 0x64)));
}
// Retail func_801DF610: sum the equipped-effect values for one stat category.
// Six slots are scanned via the lookup table (s16 ids at +0x26/+0x1C..0x24
// and a 6-byte slot table from .sdata2). For each slot, resolve the equipped
// item (or the candidate arg3 when its category nibble matches), walk its
// sub-items, and add the category-matching values (vtable+0x54 category,
// +0x90 value for equipped, or the +0x2C sub-record for passives). The id==-1
// slot accepts only the candidate item. Result is clamped by the table cap.
u32 func_801DF610(void* unused, u16 lookup_key, u32 category, void* arg3) {
    void* lookup = func_8009EC9C(lookup_key);
    s16 slotIds[6];
    slotIds[0] = *(s16*)((u8*)lookup + 0x26);
    slotIds[1] = *(s16*)((u8*)lookup + 0x1C);
    slotIds[2] = *(s16*)((u8*)lookup + 0x1E);
    slotIds[3] = *(s16*)((u8*)lookup + 0x20);
    slotIds[4] = *(s16*)((u8*)lookup + 0x22);
    slotIds[5] = *(s16*)((u8*)lookup + 0x24);
    CItemBoxSlotBytes bytes;
    bytes.ab.a = lbl_eu_8066804C;
    bytes.ab.b = lbl_eu_80668050;
    u32 result = 0;
    for (u32 slot = 0; slot < 6; slot++) {
        s16 id = slotIds[(u8)slot];
        u8 j;
        s16 v;
        if (id != -1) {
            void* item = func_80157C4C(bytes.bytes[(u8)slot], id);
            if (arg3 != NULL &&
                (s32)bytes.bytes[(u8)slot] == (s32)((*(u32*)arg3 >> 16) & 0xF))
                item = arg3;
            if (item != NULL && *(u32*)item != 0) {
                CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
                u8 count = inst->_v30(item);
                for (j = 0; (u8)j < count; j++) {
                    CItemImplVt* inst2 = (CItemImplVt*)CItem_initItemImplInstances(item);
                    v = inst2->_v40(item, (u8)j);
                    if (v != -1) {
                        void* r = func_80157C4C(3, v);
                        if (r != NULL && *(u32*)r != 0) {
                            CItemImplVt* inst3 = (CItemImplVt*)CItem_initItemImplInstances(r);
                            u16 cat = inst3->_v54(r);
                            if (category == cat) {
                                CItemImplVt* inst4 = (CItemImplVt*)CItem_initItemImplInstances(r);
                                result += inst4->_v90(r);
                            }
                        }
                    } else {
                        CItemImplVt* inst5 = (CItemImplVt*)CItem_initItemImplInstances(item);
                        CItemBoxSubRecord* sub = inst5->_v2C(item, (u8)j);
                        if (sub != NULL) {
                            u16 cat2 = (sub->field_04 >> 4) & 0xFFF;
                            if (category == cat2) {
                                s32 val2 = (sub->_00 >> 10) & 0x7FF;
                                result += (s16)val2;
                            }
                        }
                    }
                }
            }
        } else {
            if (arg3 != NULL) {
                void* itemB = NULL;
                if ((s32)bytes.bytes[(u8)slot] ==
                    (s32)((*(u32*)arg3 >> 16) & 0xF))
                    itemB = arg3;
                if (itemB != NULL && *(u32*)itemB != 0) {
                    CItemImplVt* instB = (CItemImplVt*)CItem_initItemImplInstances(itemB);
                    // j and v are shared with the branch above: retail
                    // colors them identically in both arms.
                    u8 countB = instB->_v30(itemB);
                    for (j = 0; (u8)j < countB; j++) {
                        CItemImplVt* inst2B = (CItemImplVt*)CItem_initItemImplInstances(itemB);
                        v = inst2B->_v40(itemB, (u8)j);
                        if (v != -1) {
                            void* rB = func_80157C4C(3, v);
                            if (rB != NULL && *(u32*)rB != 0) {
                                CItemImplVt* inst3B = (CItemImplVt*)CItem_initItemImplInstances(rB);
                                u16 catB = inst3B->_v54(rB);
                                if (category == catB) {
                                    CItemImplVt* inst4B = (CItemImplVt*)CItem_initItemImplInstances(rB);
                                    result += inst4B->_v90(rB);
                                }
                            }
                        } else {
                            CItemImplVt* inst5B = (CItemImplVt*)CItem_initItemImplInstances(itemB);
                            CItemBoxSubRecord* subB = inst5B->_v2C(itemB, (u8)j);
                            if (subB != NULL) {
                                u16 cat2B = (subB->field_04 >> 4) & 0xFFF;
                                if (category == cat2B) {
                                    s32 val2B = (subB->_00 >> 10) & 0x7FF;
                                    result += (s16)val2B;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // cap is u16: retail clrlwi's the call result and compares signed.
    u16 cap = (u16)func_80136254((char*)lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x503], category);
    if ((s32)cap < (s32)result) result = cap;
    return result;
}

// Retail func_801DF988 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge). The slot argument maps 1..3 to type 2 with a sub-index,
// and 4..8 to direct types; the candidate pointer is nulled out by the first
// pass when one of its gems matches, so the sum pass treats it as unequipped.
#pragma push
#pragma optimize_for_size on
u32 func_801DF988(void* info, void* member, u32 category, void* candidate, s32 slot) {
    (void)info;
    u32 type = 0;
    u32 index = 0;
    // Consecutive cases 1..3 lower to the retail range check
    // (subi r0,slot,1; cmplwi r0,2; ble) with the shared body after the
    // 4..8 dispatch chain, matching the retail block layout.
    switch (slot) {
        case 1:
        case 2:
        case 3:
            index = (u8)(slot - 1);
            type = 2;
            break;
        case 4: type = 4; index = 0; break;
        case 5: type = 5; index = 0; break;
        case 6: type = 6; index = 0; break;
        case 7: type = 7; index = 0; break;
        case 8: type = 8; index = 0; break;
        default: break;
    }
    void* lookup = func_8009EC9C((u32)member);
    s16 ids[6];
    // ids[0] is the halfword at lookup+0x26; the remaining five are the packed
    // block at 0x1C..0x24 (retail load order).
    ids[0] = *(s16*)((u8*)lookup + 0x26);
    ids[1] = *(s16*)((u8*)lookup + 0x1C);
    ids[2] = *(s16*)((u8*)lookup + 0x1E);
    ids[3] = *(s16*)((u8*)lookup + 0x20);
    ids[4] = *(s16*)((u8*)lookup + 0x22);
    ids[5] = *(s16*)((u8*)lookup + 0x24);
    // 6-byte slot table copied from two .sdata2 constants.
    CItemBoxSlotBytes bytes;
    bytes.ab.a = lbl_eu_80668054;
    bytes.ab.b = lbl_eu_80668058;
    s32 sum = 0;
    // First pass: if the candidate item's gem chain contains the candidate
    // pointer itself, null the candidate out (treated as unequipped below).
    u32 found = 0;
    for (u32 i = 0; i < 6; i++) {
        s16 id = ids[(u8)i];
        if (id == -1) continue;
        void* item = func_80157C4C(bytes.bytes[(u8)i], id);
        if (item == NULL || *(u32*)item == 0) continue;
        CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
        u32 count = inst->_v30(item);
        for (u32 j = 0; (u8)j < count; j++) {
            CItemImplVt* inst2 = (CItemImplVt*)CItem_initItemImplInstances(item);
            s16 v = inst2->_v40(item, (u8)j);
            if (v == -1) continue;
            void* r = func_80157C4C(3, v);
            if (r == NULL || *(u32*)r == 0) continue;
            if (r == candidate) {
                candidate = NULL;
                found = 1;
                break;
            }
        }
        if (found) break;
    }
    // Second pass: sum the gem values whose equip category matches, with the
    // candidate slot/index pair handled through its own vtable accessors.
    for (u32 i = 0; i < 6; i++) {
        s16 id = ids[(u8)i];
        if (id == -1) continue;
        u8 slotB = bytes.bytes[(u8)i];
        void* item = func_80157C4C(slotB, id);
        if (item == NULL || *(u32*)item == 0) continue;
        CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
        u32 count = inst->_v30(item);
        for (u32 j = 0; (u8)j < count; j++) {
            if (candidate != NULL && slotB == type && (u8)j == index) {
                CItemImplVt* instA = (CItemImplVt*)CItem_initItemImplInstances(candidate);
                u16 cat = instA->_v54(candidate);
                if (category == (u32)cat) {
                    CItemImplVt* instB = (CItemImplVt*)CItem_initItemImplInstances(candidate);
                    sum += (s32)instB->_v90(candidate);
                }
            } else {
                CItemImplVt* instC = (CItemImplVt*)CItem_initItemImplInstances(item);
                s16 v = instC->_v40(item, (u8)j);
                if (v != -1) {
                    void* r = func_80157C4C(3, v);
                    if (r != NULL && *(u32*)r != 0) {
                        CItemImplVt* instD = (CItemImplVt*)CItem_initItemImplInstances(r);
                        u16 cat2 = instD->_v54(r);
                        if (category == (u32)cat2) {
                            CItemImplVt* instE = (CItemImplVt*)CItem_initItemImplInstances(r);
                            sum += (s32)instE->_v90(r);
                        }
                    }
                } else {
                    CItemImplVt* instF = (CItemImplVt*)CItem_initItemImplInstances(item);
                    CItemBoxSubRecord* sub = instF->_v2C(item, (u8)j);
                    if (sub != NULL && category == (u32)((sub->field_04 >> 4) & 0xFFF)) {
                        sum += (s16)((sub->_00 >> 10) & 0x7FF);
                    }
                }
            }
        }
    }
    u16 max = (u16)func_80136254(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x503], category);
    if (max < sum) sum = max;
    return (u32)sum;
}
#pragma pop
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
u32 func_801DFD60(void* a, void* b, u32 arg2) {
    // Retail keeps obj in r31 and result in r30 (obj declared first gets the
    // highest saved register); stmw/lmw frame from optimize_for_size.
    void* obj;
    u32 result = 0;
    obj = (u8*)func_8009EC9C((u32)b) + 0x3534;
    if (func_8026178C(obj, arg2) != 0) {
        result = func_8025FB10(obj, arg2);
    }
    return result;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
u32 func_801E9690(void* dummy, u32 arg1, u32 arg2) {
    // Retail keeps obj in r31 and result in r30 (obj declared first gets the
    // highest saved register); the result=0 init lands before the call.
    void* obj;
    u32 result = 0;
    obj = (u8*)func_8009EC9C(arg1) + 0x3534;
    if (func_8026178C(obj, arg2) != 0) {
        result = func_8025FB10(obj, arg2);
    }
    return result;
}
#pragma pop

u32 func_801E92B8(void* dummy, void* arg1) {
    void* obj = (u8*)func_8009EC9C((u8)(u32)arg1) + 0x3534;
    if (func_8026178C(obj, 0x2d) != 0) {
        return func_8025FB10(obj, 0x2d);
    }
    return 0;
}


#pragma push
#pragma optimize_for_size on
u32 func_801DFDC0(void* dummy, u32 arg1, void* arg2) {
    if (arg1 == 0 || arg2 == 0) return 0;
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x1f4], arg1);
    u32 g = (u32)lbl_eu_806640F8;
    u8 result = func_801361E8(g, buf, (u16)func_80139358(*(u32*)arg2 >> 20));
    return result != 0;
}
#pragma pop
u32 func_801DFE48(void* global, u16 arg2, void* arg3) {
    if (arg2 == 0) return 0;
    void* g = lbl_eu_806640F8;
    u16 v1 = arg3 ? (u16)func_801392E4((u32)arg3) : 0;
    u16 v2 = arg3 ? (u16)func_80139358((u32)arg3) : 0;
    void* lookup = func_8009EC9C(arg2);
    u32 result = 0;
    for (u32 i = 4; i <= 8; i++) {
        u8 ii = (u8)i;
        if (ii == v1) continue;
        s16 val = -1;
        switch (ii) {
            case 4: val = *(s16*)((u8*)lookup + 0x1C); break;
            case 5: val = *(s16*)((u8*)lookup + 0x1E); break;
            case 6: val = *(s16*)((u8*)lookup + 0x20); break;
            case 7: val = *(s16*)((u8*)lookup + 0x22); break;
            case 8: val = *(s16*)((u8*)lookup + 0x24); break;
        }
        if (val == -1) continue;
        // Retail passes the slot id (u8)i and the slot value val (2-arg form).
        void* r = func_80157C4C((u8)i, val);
        if (r == NULL) continue;
        u32 v = *(u32*)r;
        if (v == 0) continue;
        result += (u8)func_801361E8((u32)g, (char*)&lbl_eu_805063BC[0x1e2], func_80139358(v >> 20));
    }
    if (arg3 != NULL) {
        result += (u8)func_801361E8((u32)g, (char*)&lbl_eu_805063BC[0x1e2], v2);
    }
    return result;
}
u32 func_801DFFB8(void* unused, u16 lookup_key, void* arg3, void* unused2) {
    void* lookup = func_8009EC9C(lookup_key);
    CItemBoxSlotBytes bytes;
    s16 ids[6];
    // ids[0] is the halfword at lookup+0x26; the remaining five are the packed
    // block at 0x1C..0x24 (retail load order).
    ids[0] = *(s16*)((u8*)lookup + 0x26);
    ids[1] = *(s16*)((u8*)lookup + 0x1C);
    ids[2] = *(s16*)((u8*)lookup + 0x1E);
    ids[3] = *(s16*)((u8*)lookup + 0x20);
    ids[4] = *(s16*)((u8*)lookup + 0x22);
    ids[5] = *(s16*)((u8*)lookup + 0x24);
    // 6-byte slot table copied from two .sdata2 constants.
    bytes.ab.a = lbl_eu_8066805C;
    bytes.ab.b = lbl_eu_80668060;
    for (u8 i = 0; i < 6; i++) {
        s16 id = ids[i];
        if (id != -1) {
            u8 slot = bytes.bytes[i];
            // Retail passes (slot, id): id already sits in r4 from the lhax,
            // so MWCC reuses it as the second argument (2-arg form).
            void* item = func_80157C4C(slot, id);
            if (arg3 != NULL && slot == (s32)((*(u32*)arg3 >> 16) & 0xF)) item = arg3;
            if (item != NULL && *(u32*)item != 0) {
                CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
                u8 count = inst->_v30(item);
                for (u8 j = 0; j < count; j++) {
                    CItemImplVt* inst2 = (CItemImplVt*)CItem_initItemImplInstances(item);
                    s16 v = inst2->_v40(item, j);
                    if (v != -1) {
                        void* r = func_80157C4C(3, v);
                        if (r != NULL && *(u32*)r != 0) return 0;
                    } else {
                        CItemImplVt* inst3 = (CItemImplVt*)CItem_initItemImplInstances(item);
                        CItemBoxSubRecord* sub = inst3->_v2C(item, j);
                        if (sub != NULL && ((sub->field_04 >> 4) & 0xFFF) != 0) return 0;
                    }
                }
            }
        } else {
            if (arg3 != NULL) {
                // item defaults to NULL; retail places the init after the
                // arg3 guard and only overwrites it on a category match.
                void* item = NULL;
                if (bytes.bytes[i] == (s32)((*(u32*)arg3 >> 16) & 0xF)) item = arg3;
                if (item != NULL && *(u32*)item != 0) {
                    CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
                    u32 count = inst->_v30(item);
                    for (u8 j = 0; j < (u8)count; j++) {
                        CItemImplVt* inst2 = (CItemImplVt*)CItem_initItemImplInstances(item);
                        s16 v = inst2->_v40(item, j);
                        if (v != -1) {
                            void* r = func_80157C4C(3, v);
                            if (r != NULL && *(u32*)r != 0) return 0;
                        } else {
                            CItemImplVt* inst3 = (CItemImplVt*)CItem_initItemImplInstances(item);
                            CItemBoxSubRecord* sub = inst3->_v2C(item, j);
                            if (sub != NULL && ((sub->field_04 >> 4) & 0xFFF) != 0) return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
// Retail CItemBoxInfo::OnFileEvent: two file-load branches. The line-arc
// branch builds the whole layout (0x14000 scratch region), seeds every label
// textbox with the shared text object, then runs the item-box1 reset pass:
// pane clears gated on the mode byte (+0x9A), fixed label texts, per-mode
// caption, and the four vertex-colour blocks whose results are written both
// into this (+0x9C/+0xA4) and into the .sbss colour globals (518/520/558/
// 560/598/5A0/5B8/5C0) with their alpha halves propagated to the paired
// globals. Finally caches the 20 slot panes at +0x40..+0x8C.
bool CItemBoxInfo::OnFileEvent(CEventFile* file) {
    char* base = lbl_eu_805063BC;
    if (state.fileHandle1 == file->mFileHandle) {
        state.memRegion1.createRegion(mtl::MemManager::getHandleMEM2(), 0x14000,
                                      &lbl_eu_805063BC[0x524], 1);
        Class_8045F858 host(&state.memRegion1);
        u8* data = (u8*)state.fileHandle1->getData();
        mtl::MemManager::func_80434A4C(false);
        nw4r::lyt::ArcResourceAccessor* acc = createArcResourceAccessor__10CLibLayoutFv();
        state.arcResourceAccessor = acc;
        acc->Attach(data, &lbl_eu_805063BC[0x531]);
        func_80136E84(&state.layout, acc, &lbl_eu_805063BC[0x535]);
        func_80136F08(state.layout, &state.animTransform1, acc, &lbl_eu_805063BC[0x54a]);
        func_80136F08(state.layout, &state.animTransform2, acc, &lbl_eu_805063BC[0x562]);
        nw4r::lyt::Pane* root = (nw4r::lyt::Pane*)*(void**)((u8*)state.layout + 0x10);
        void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, state.layout);
        func_8013676C(root, reinterpret_cast<CItemBoxFontInfoVt*>(fontObj)->fontData());

        // Seed the label textboxes with the shared text object.
        char* text = func_801355BC();
        func_801368C0(state.layout, &lbl_eu_805063BC[0x25b], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x267], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x273], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x286], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x292], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x29e], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x45a], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x466], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x4a7], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x57f], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x58b], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x597], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5a3], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5af], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5be], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5cd], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x143], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x4e3], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x4f0], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5dc], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5e9], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5f6], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x603], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x610], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x61d], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x62a], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x637], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x644], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x651], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x65e], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x66b], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x678], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x685], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x692], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x69f], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x6ac], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x6b9], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x6c6], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x17d], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x6d2], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x188], (u32)text);

        state.layout->SetAnimationEnable(state.animTransform2, false);
        state.layout->SetAnimationEnable(state.animTransform1, true);
        state.layout->Animate(0);

        u8 mode = *(u8*)((u8*)this + 0x9A);
        if (mode != 1 && mode != 2) {
            func_80124270(root->FindPaneByName(&lbl_eu_805063BC[0x6dd], true), 0);
        }
        if (mode != 3) {
            func_80124270(root->FindPaneByName(&lbl_eu_805063BC[0x6e6], true), 0);
        }

        // Fixed label texts for the closed item-box view.
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x2b6],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xa), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x2d7],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xd), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x2e2],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x11), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x2c1],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x12), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x2cc],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xc), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x2ed],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x13), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x6f2],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x18), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x6fd],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x18), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x708],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x24), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x713],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x25), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x71e],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x70), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x72a],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x72), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x736],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x73), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x742],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x74), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x74e],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x79), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x75a],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x75), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x766],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x7b), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x772],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x76), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x77e],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x77), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x78a],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x78), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x796],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x7e), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x2f8],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x2f), 0);

        // Per-mode caption: modes 0/1 show the owned-item count; mode 3 shows
        // the category name resolved from the selected item id.
        if ((s8)mode <= 1) {
            ml::FixStr<32> caption(true);
            caption.format(&lbl_eu_805063BC[0x13e], func_801571FC(),
                           func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 3));
            func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x143],
                          caption.c_str(), 0);
        } else if (mode == 3) {
            u16 itemId = *(u16*)((u8*)this + 0xAC);
            u8 kind = (u8)func_80138E90(itemId);
            char* s;
            if (kind == 0) {
                s = func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x24);
            } else {
                s = func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], kind + 0x1F);
            }
            func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x7ad], s, 0);
            char* name = ((char* (*)(void*, const char*, u16))&func_8013639C)(
                &lbl_eu_80664098, &lbl_eu_805063BC[0x139], itemId);
            func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x7b9],
                          func_80138DA4(name), 0);
            func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x7c5],
                          func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x2d), 0);
            func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x7d2],
                          &lbl_eu_805063BC[0x2aa], 0);
            func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x7e0],
                          &lbl_eu_805063BC[0x2aa], 0);
        }

        // Vertex colours: the highlight pair goes onto this (+0x9C/+0xA4); the
        // three named panes' pairs go into the .sbss colour globals with their
        // alpha halves propagated to the paired globals.
        nw4r::lyt::Pane* hl = root->FindPaneByName(&lbl_eu_805063BC[0x7ee], true);
        CItemBoxGXColor c = func_801397AC(hl, 0);
        __as__11_GXColorS10FRC11_GXColorS10((u8*)this + 0x9C, &c);
        c = func_801397AC(hl, 1);
        __as__11_GXColorS10FRC11_GXColorS10((u8*)this + 0xA4, &c);

        nw4r::lyt::Pane* p71e = root->FindPaneByName(&lbl_eu_805063BC[0x71e], true);
        c = func_801397AC(p71e, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664518, &c);
        c = func_801397AC(p71e, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664520, &c);
        {
            s16 a0 = *(s16*)((char*)&lbl_eu_80664518 + 6);
            s16 a1 = *(s16*)((char*)&lbl_eu_80664520 + 6);
            *(s16*)((char*)&lbl_eu_80664528 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664530 + 6) = a1;
            *(s16*)((char*)&lbl_eu_80664538 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664540 + 6) = a1;
            *(s16*)((char*)&lbl_eu_80664548 + 6) = a0;
            *(s16*)((char*)&lbl_eu_80664550 + 6) = a1;
        }

        nw4r::lyt::Pane* p4e3 = root->FindPaneByName(&lbl_eu_805063BC[0x4e3], true);
        c = func_801397AC(p4e3, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664558, &c);
        c = func_801397AC(p4e3, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664560, &c);

        nw4r::lyt::Pane* p2b6 = root->FindPaneByName(&lbl_eu_805063BC[0x2b6], true);
        c = func_801397AC(p2b6, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_80664598, &c);
        c = func_801397AC(p2b6, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_806645A0, &c);
        *(s16*)((char*)&lbl_eu_806645A8 + 6) = *(s16*)((char*)&lbl_eu_80664598 + 6);
        *(s16*)((char*)&lbl_eu_806645B0 + 6) = *(s16*)((char*)&lbl_eu_806645A0 + 6);

        nw4r::lyt::Pane* p25b = root->FindPaneByName(&lbl_eu_805063BC[0x25b], true);
        c = func_801397AC(p25b, 0);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_806645B8, &c);
        c = func_801397AC(p25b, 1);
        __as__11_GXColorS10FRC11_GXColorS10(&lbl_eu_806645C0, &c);
        *(s16*)((char*)&lbl_eu_806645C8 + 6) = *(s16*)((char*)&lbl_eu_806645B8 + 6);
        *(s16*)((char*)&lbl_eu_806645D0 + 6) = *(s16*)((char*)&lbl_eu_806645C0 + 6);

        func_801D8C0C(this);

        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x7f9],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 6), 0);
        func_80136B4C((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x6d2],
                      func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 9), 0);
        func_80136910((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x17d], 0);
        func_80136910((nw4r::lyt::Layout*)state.layout, &lbl_eu_805063BC[0x188], 0);
        func_80124270(root->FindPaneByName(&lbl_eu_805063BC[0x193], true), 1);

        // Cache the slot panes at +0x40..+0x8C for the renderer (retail keeps
        // the layout's root pane live in r28 across all 20 lookups).
        nw4r::lyt::Pane* cacheRoot = (nw4r::lyt::Pane*)*(void**)((u8*)state.layout + 0x10);
        *(nw4r::lyt::Pane**)((u8*)this + 0x40) = cacheRoot->FindPaneByName(&base[0x5dc], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x44) = cacheRoot->FindPaneByName(&base[0x736], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x48) = cacheRoot->FindPaneByName(&base[0x5e9], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x4c) = cacheRoot->FindPaneByName(&base[0x6c6], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x50) = cacheRoot->FindPaneByName(&base[0x5f6], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x54) = cacheRoot->FindPaneByName(&base[0x742], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x58) = cacheRoot->FindPaneByName(&base[0x603], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x5c) = cacheRoot->FindPaneByName(&base[0x75a], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x60) = cacheRoot->FindPaneByName(&base[0x610], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x64) = cacheRoot->FindPaneByName(&base[0x772], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x68) = cacheRoot->FindPaneByName(&base[0x61d], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x6c) = cacheRoot->FindPaneByName(&base[0x74e], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x70) = cacheRoot->FindPaneByName(&base[0x637], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x74) = cacheRoot->FindPaneByName(&base[0x77e], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x78) = cacheRoot->FindPaneByName(&base[0x644], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x7c) = cacheRoot->FindPaneByName(&base[0x78a], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x80) = cacheRoot->FindPaneByName(&base[0x62a], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x84) = cacheRoot->FindPaneByName(&base[0x766], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x88) = cacheRoot->FindPaneByName(&base[0x651], true);
        *(nw4r::lyt::Pane**)((u8*)this + 0x8c) = cacheRoot->FindPaneByName(&base[0x796], true);

        tryActivateItemBox(this);
        state.fileHandle1 = 0;
        state.memRegion1.func_8045F810();
        return true;
    }
    if (state.fileHandle2 == file->mFileHandle) {
        state.memRegion2.createRegion(mtl::MemManager::getHandleMEM2(), 0x100,
                                      &lbl_eu_805063BC[0x804], 1);
        Class_8045F858 host(&state.memRegion2);
        u8* data = (u8*)state.fileHandle2->getData();
        mtl::MemManager::func_80434A4C(false);
        nw4r::lyt::ArcResourceAccessor* acc2 = createArcResourceAccessor__10CLibLayoutFv();
        state.resource = acc2;
        acc2->Attach(data, &lbl_eu_805063BC[0x531]);
        tryActivateItemBox(this);
        state.fileHandle2 = 0;
        state.memRegion2.func_8045F810();
        return true;
    }
    return false;
}
#pragma pop

// Retail __ct__CItemBoxInfo uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) with arg2/arg3 saved in r30/r31; plain -O4,p splits the
// saves into separate stw's. The trailing resetCItemBox__Fv call is a real
// bl (the callee is __declspec(noinline)).
#pragma push
#pragma optimize_for_size on
CItemBoxInfo* __ct__CItemBoxInfo(CItemBoxInfo* info, u8 arg2, u16 arg3) {
    *(void**)info = (void*)lbl_eu_80534B28;
    __ct__17UnkClass_8045F564Fv((u8*)info + 0x4);
    __ct__17UnkClass_8045F564Fv((u8*)info + 0x14);
    *(u32*)((u8*)info + 0x24) = 0;
    *(u32*)((u8*)info + 0x28) = 0;
    *(u32*)((u8*)info + 0x2C) = 0;
    *(u32*)((u8*)info + 0x30) = 0;
    *(u32*)((u8*)info + 0x34) = 0;
    *(u32*)((u8*)info + 0x38) = 0;
    *(u32*)((u8*)info + 0x3C) = 0;
    *(u32*)((u8*)info + 0x40) = 0;
    *(u32*)((u8*)info + 0x44) = 0;
    *(u32*)((u8*)info + 0x48) = 0;
    *(u32*)((u8*)info + 0x4C) = 0;
    *(u32*)((u8*)info + 0x50) = 0;
    *(u32*)((u8*)info + 0x54) = 0;
    *(u32*)((u8*)info + 0x58) = 0;
    *(u32*)((u8*)info + 0x5C) = 0;
    *(u32*)((u8*)info + 0x60) = 0;
    *(u32*)((u8*)info + 0x64) = 0;
    *(u32*)((u8*)info + 0x68) = 0;
    *(u32*)((u8*)info + 0x6C) = 0;
    *(u32*)((u8*)info + 0x70) = 0;
    *(u32*)((u8*)info + 0x74) = 0;
    *(u32*)((u8*)info + 0x78) = 0;
    *(u32*)((u8*)info + 0x7C) = 0;
    *(u32*)((u8*)info + 0x80) = 0;
    *(u32*)((u8*)info + 0x84) = 0;
    *(u32*)((u8*)info + 0x88) = 0;
    *(u32*)((u8*)info + 0x8C) = 0;
    *(u8*)((u8*)info + 0x90) = 0;
    *(u32*)((u8*)info + 0x94) = 0;
    *(u8*)((u8*)info + 0x98) = 0;
    *(u8*)((u8*)info + 0x99) = 1;
    *(u8*)((u8*)info + 0x9A) = arg2;
    *(u16*)((u8*)info + 0xAC) = arg3;
    *(u8*)((u8*)info + 0xAE) = 0;
    // Retail keeps a dead addi r3,r29,0xb0 (scheduled before the zero-stores)
    // plus the trailing bl resetCItemBox__Fv; the cast call makes MWCC emit
    // the extra-arg setup and then fold to the direct bl of the Fv symbol.
    ((void(*)(void*))resetCItemBox)((u8*)info + 0xB0);
    return info;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
extern "C" CItemBoxInfo* __dt__12CItemBoxInfoFv(CItemBoxInfo* info, s32 flags) {
    if (info != NULL) {
        __dt__17UnkClass_8045F564Fv(&info->state.memRegion2, -1);
        __dt__17UnkClass_8045F564Fv(&info->state.memRegion1, -1);
        if (flags > 0) ::operator delete(info);
    }
    return info;
}
#pragma pop

void func_801D3FF0(void*) {}

// Retail __ct__CItemBoxInfo2 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) with arg2/arg3 saved in r30/r31; plain -O4,p splits the
// saves into separate stw's. The trailing resetCItemBox__Fv call is a real
// bl (the callee is __declspec(noinline)).
#pragma push
#pragma optimize_for_size on
CItemBoxInfo2* __ct__CItemBoxInfo2(CItemBoxInfo2* info, u8 arg2, u16 arg3) {
    *(void**)info = (void*)lbl_eu_80534A90;
    __ct__17UnkClass_8045F564Fv((u8*)info + 0x4);
    __ct__17UnkClass_8045F564Fv((u8*)info + 0x14);
    *(u32*)((u8*)info + 0x24) = 0;
    *(u32*)((u8*)info + 0x28) = 0;
    *(u32*)((u8*)info + 0x2C) = 0;
    *(u32*)((u8*)info + 0x30) = 0;
    *(u32*)((u8*)info + 0x34) = 0;
    *(u32*)((u8*)info + 0x38) = 0;
    *(u32*)((u8*)info + 0x3C) = 0;
    *(u32*)((u8*)info + 0x40) = 0;
    *(u32*)((u8*)info + 0x44) = 0;
    *(u32*)((u8*)info + 0x48) = 0;
    *(u32*)((u8*)info + 0x4C) = 0;
    *(u32*)((u8*)info + 0x50) = 0;
    *(u32*)((u8*)info + 0x54) = 0;
    *(u32*)((u8*)info + 0x58) = 0;
    *(u32*)((u8*)info + 0x5C) = 0;
    *(u32*)((u8*)info + 0x60) = 0;
    *(u32*)((u8*)info + 0x64) = 0;
    *(u32*)((u8*)info + 0x68) = 0;
    *(u32*)((u8*)info + 0x6C) = 0;
    *(u32*)((u8*)info + 0x70) = 0;
    *(u32*)((u8*)info + 0x74) = 0;
    *(u32*)((u8*)info + 0x78) = 0;
    *(u32*)((u8*)info + 0x7C) = 0;
    *(u32*)((u8*)info + 0x80) = 0;
    *(u32*)((u8*)info + 0x84) = 0;
    *(u32*)((u8*)info + 0x88) = 0;
    *(u32*)((u8*)info + 0x8C) = 0;
    *(u8*)((u8*)info + 0x90) = 0;
    *(u32*)((u8*)info + 0x94) = 0;
    *(u8*)((u8*)info + 0x98) = 0;
    *(u8*)((u8*)info + 0x99) = 1;
    *(u8*)((u8*)info + 0x9A) = arg2;
    *(u16*)((u8*)info + 0xAC) = arg3;
    // Retail keeps a dead addi r3,r29,0xb0 (scheduled before the zero-stores)
    // plus the trailing bl resetCItemBox__Fv; the cast call makes MWCC emit
    // the extra-arg setup and then fold to the direct bl of the Fv symbol.
    ((void(*)(void*))resetCItemBox)((u8*)info + 0xB0);
    return info;
}
#pragma pop

// Retail __dt__13CItemBoxInfo2Fv uses the stmw/lmw frame (MWCC
// optimize_for_size prologue merge); plain -O4,p splits the r30/r31 saves
// into separate stw's. Same pattern as the matched kyoshin dtors.
#pragma push
#pragma optimize_for_size on
CItemBoxInfo2* __dt__13CItemBoxInfo2Fv(CItemBoxInfo2* info, s32 flags) {
    if (info != NULL) {
        __dt__17UnkClass_8045F564Fv(&info->state.memRegion2, -1);
        __dt__17UnkClass_8045F564Fv(&info->state.memRegion1, -1);
        if (flags > 0) ::operator delete(info);
    }
    return info;
}
#pragma pop

// Retail func_801E12E0 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) like the sibling ItemBox1/2 dtors.
#pragma push
#pragma optimize_for_size on
void func_801E12E0(CItemBoxInfo2* info) {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    info->state.fileHandle1 = CDeviceFile::readFile(handle, &lbl_eu_805063BC[0x8e], reinterpret_cast<IWorkEvent*>(info), 0, 0);
    handle = mtl::MemManager::getHandleMEM2();
    info->state.fileHandle2 = CDeviceFile::readFile(handle, &lbl_eu_805063BC[0xa6], reinterpret_cast<IWorkEvent*>(info), 0, 0);
}
#pragma pop


void drawItemBox2Layout(CItemBoxInfo2* info, nw4r::lyt::DrawInfo* drawInfo) {
    if (info->state.active != 0) {
        func_80137038((nw4r::lyt::Layout*)info->state.layout, drawInfo, 0, 1);
    }
}

// Retail func_801E13F8 (CItemBoxInfo2 teardown) keeps a plain single-save
// frame (only r31 = `this` survives the calls; zeros are re-loaded per store),
// so unlike func_801D4174 it must NOT use optimize_for_size, and it does not
// clear the 0x2C/0x30 accessor/resource pointers after releasing them.
void func_801E13F8(CItemBoxInfo2* info) {
    func_801390E0(&info->state.fileHandle1);
    func_801390E0(&info->state.fileHandle2);
    void* layout = info->state.layout;
    info->state.active = 0;
    if (layout != 0) {
        if (layout != 0) {
            // Deleting destructor via vtable slot 2 (see func_801D4174).
            reinterpret_cast<CItemBoxLayoutDtorVt*>(layout)->destroy(1);
        }
        info->state.layout = 0;
    }
    info->state.animTransform1 = 0;
    info->state.animTransform2 = 0;
    func_80139124(info->state.arcResourceAccessor);
    func_80139124(info->state.resource);
    info->state.memRegion1.func_8045F778();
    info->state.memRegion2.func_8045F778();
}


#pragma push
#pragma auto_inline off
void func_801E1498(CItemBoxInfo2* info) {
    if (info->state.state != 0) return;
    info->state.state = 1;
    info->state.visible = 0;
    func_801E4194(info);
}
#pragma pop

void advanceItemBox2State(CItemBoxInfo2* info) {
    if (info->state.state == 3) {
        info->state.state = 4;
        info->state.visible = 0;
    }
}

// Retail func_801E14DC: ItemBox2 info dispatch. Zeroes the slot VEC3 block,
// renders the base panel, derives the item id (or the candidate's own), then
// dispatches on the item category to the per-category renderers. Ends by
// formatting a header string into a FixStr and pushing it to the layout.
// stmw/lmw frame (optimize_for_size prologue merge).
#pragma push
#pragma optimize_for_size on
#pragma auto_inline off
extern "C" void func_801E14DC(CItemBoxInfo2* info, u16 arg2, void* arg3, u16 arg4, u32 arg5) {
    func_801D4A2C((u8*)info + 0xB0);
    func_801E43BC(info, arg2, arg3, arg4, arg5);
    u16 id = arg3 ? (u16)(*(u32*)arg3 >> 20) : 0;
    u16 idFinal = arg2;
    if (*(u8*)((u8*)info + 0x9A) == 4) idFinal = id;
    u32 r = (u8)(u32)func_801392E4((u32)idFinal);
    if (arg3 != NULL && (func_801C6E90(arg3) != 0 || func_801D4AB0(arg3) != 0)) r = 9;
    if (r - 4 <= 4) {
        func_801E2C5C(info, idFinal, arg3, arg4);
    } else if (r == 2) {
        func_801E2928(info, idFinal, arg3, arg4);
    } else if (r == 3) {
        func_801E3228(info, idFinal, arg3, arg4);
    } else if (r == 9) {
        func_801E37C4(info, (void*)(u32)idFinal, arg3);
    } else if (r == 10) {
        func_801E3730(info, idFinal);
    } else if (r == 13) {
        func_801E2FEC(info, idFinal);
    } else {
        func_801E3918(info);
        func_801E3B9C(info);
        func_801E3DE4(info);
        func_801E3EB8(info);
        func_801E4010(info);
        func_801E4090(info);
        func_801E40E8(info);
    }
    char* s = func_80136190((char*)&lbl_eu_805063BC[0x130], (char*)&lbl_eu_805063BC[0x139], 3);
    // Not named textBuffer: the textBuffer raw-storage macro from
    // func_801D8E34 is still in scope until its #undef below.
    ml::FixStr<32> caption;
    caption.format(&lbl_eu_805063BC[0x13e], func_801571FC(), s);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x143], caption.c_str(), 0);
}
#pragma pop
#pragma push
#pragma optimize_for_size on
void func_801E16F0(CItemBoxInfo2* info, char* arg1, char* arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x151], arg1);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, arg2, 0);
}
#pragma pop

// Retail func_801E174C (CItemBoxInfo2 variant) uses the same stmw/lmw frame.
#pragma push
#pragma optimize_for_size on
void func_801E174C(u8* arg0, CItemBoxInfo2* info, u32 arg2) {
    char buf[0x20];
    // Same declaration-order trick as func_801D4B3C: `first` before `child`
    // so child colors into dead info's r30 and first reuses base's r31.
    nw4r::lyt::Pane* first;
    nw4r::lyt::Pane* second;
    nw4r::lyt::Pane* child;
    char* base = lbl_eu_805063BC;
    sprintf(buf, base + 0x161, arg2 + 1);
    child = (nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10);
    first = child->FindPaneByName(base + 0x16e, true);
    second = child->FindPaneByName(buf, true);
    func_80137924((nw4r::math::VEC3*)arg0, second, first, child);
}
#pragma pop
#pragma push
#pragma auto_inline off
extern "C" void func_801E17EC(CItemBoxInfo2* info) {
    if (func_80137444((nw4r::lyt::AnimTransform*)*(void**)((u8*)info + 0x38), lbl_eu_80668010) != 0) {
        ((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34))->SetAnimationEnable((nw4r::lyt::AnimTransform*)*(void**)((u8*)info + 0x38), false);
        ((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34))->SetAnimationEnable((nw4r::lyt::AnimTransform*)*(void**)((u8*)info + 0x3C), true);
        *(u32*)((u8*)info + 0x94) = 2;
    }
}

extern "C" void func_801E1868(CItemBoxInfo2* info) {
    if (func_80137444((nw4r::lyt::AnimTransform*)info->state.animTransform2, -0.0f) != 0) {
        info->state.state = 3;
        info->state.visible = 1;
    }
}

extern "C" void func_801E18B4(CItemBoxInfo2* info) {
    if (func_80137510((nw4r::lyt::AnimTransform*)info->state.animTransform2, lbl_eu_80668010) != 0) {
        ((nw4r::lyt::Layout*)info->state.layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)info->state.animTransform2, false);
        ((nw4r::lyt::Layout*)info->state.layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)info->state.animTransform1, true);
        info->state.state = 5;
    }
}

extern "C" void func_801E1930(CItemBoxInfo2* info) {
    if (func_80137510((nw4r::lyt::AnimTransform*)info->state.animTransform1, -0.0f) != 0) {
        info->state.visible = 1;
        info->state.state = 0;
    }
}
#pragma pop

#pragma push
#pragma auto_inline off
// Retail func_801E197C uses the _savegpr_20 + f29/f30/f31 frame (MWCC
// optimize_for_size prologue merge). On a null item it zeroes the 0x34-byte
// record and copies it out; otherwise it builds the record like func_801D4E2C
// (same lookup chain, clamp and per-slot flag banks) and copies it out.
#pragma push
#pragma optimize_for_size on
#pragma auto_inline off
void func_801E197C(void* out, void* arg2, void* arg3) {
    CItemBoxCompRecord rec;
    // 0x43300000 integer->double conversion scratch; retail stores both high
    // words before the null-item branch.
    union { double d; u32 w[2]; } u1;
    union { double d; u32 w[2]; } u2;
    u1.w[0] = 0x43300000;
    u2.w[0] = 0x43300000;
    // Retail emits the 0x34-byte pair-copy once per branch (null path copies
    // immediately and returns; the build path has its own copy at the end).
    if (arg3 == NULL) {
        memset(&rec, 0, 0x34);
        {
            u32* d = (u32*)out - 1;
            u32* s = (u32*)&rec - 1;
            for (u32 k = 0; k < 6; k++) {
                d[1] = s[1];
                d[2] = s[2];
                s += 2;
                d += 2;
            }
            d[1] = s[1];
        }
        return;
    }
    {
        void* global = lbl_eu_806640F4;
        func_801392E4((u32)arg3);
        u32 v2 = func_80139358((u32)arg3);
        char* base = (char*)&lbl_eu_805063BC;
        rec.s2C = (u16)func_80136254((const void*)global, (const void*)(base + 0x19c), (int)(u16)v2);
        rec.s2E = (u16)func_80136254((const void*)global, (const void*)(base + 0x1a4), (int)(u16)v2);
        rec.s30 = (u8)func_801361E8((u32)global, base + 0x1ab, (u16)v2);
        rec.s32 = (u8)func_801361E8((u32)global, base + 0x1b3, (u16)v2);
        rec.s34 = (u8)func_801361E8((u32)global, base + 0x1bb, (u16)v2);
        u1.w[1] = (u8)func_801361E8((u32)global, base + 0x1c3, (u16)v2);
        rec.f38 = (f32)(u1.d - lbl_eu_80668020) / lbl_eu_80668014;
        rec.s3C = (u8)func_801361E8((u32)global, base + 0x1c9, (u16)v2);
        if (func_801361E8((u32)global, base + 0x1d2, (u16)v2) & 4) {
            void* lookup = func_8009EC9C(1);
            u32 r = func_800A082C(lookup);
            u2.w[1] = (u32)((s32)rec.s2C * (s32)r) ^ 0x80000000;
            rec.s2C = (s16)(s32)(lbl_eu_80668018 * (f32)(u2.d - lbl_eu_80668028));
            u32 r2 = func_800A082C(lookup);
            u1.w[1] = (u32)((s32)rec.s2E * (s32)r2) ^ 0x80000000;
            rec.s2E = (s16)(s32)(lbl_eu_8066801C * (f32)(u1.d - lbl_eu_80668028));
            if (rec.s2C > 999) rec.s2C = 999;
            if (rec.s2E > 999) rec.s2E = 999;
        }
        // MWCC hoists these loop-invariant .sdata2 loads into callee-saved
        // f29/f30 when the globals are referenced directly.
        for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
            u8 slot = (u8)func_801392B4((u8)i);
            char buf[0x20];
            sprintf(buf, base + 0x1d7, slot);
            u8 v = (u8)func_801361E8((u32)global, buf, (u16)v2);
            // +0x16 flag bank: slot has a resolvable entry at all.
            rec._16[(u8)i] = (v != 0);
            if (rec._16[(u8)i] == 0) continue;
            // Accumulate the slot's stat contribution into the ratio total.
            rec.f38 += func_8013B380(slot);
            void* lookup = func_8009EC9C(slot);
            void* item = func_80157C4C(2, *(s16*)((u8*)lookup + 0x26));
            if (item == NULL || *(u32*)item == 0) continue;
            u16 cat = (u16)func_80139358(*(u32*)item >> 20);
            s16 n1 = (u16)func_80136254((const void*)global, (const void*)(base + 0x19c), (int)cat);
            s16 n2 = (u16)func_80136254((const void*)global, (const void*)(base + 0x1a4), (int)cat);
            u2.w[1] = (u8)func_801361E8((u32)global, base + 0x1c3, cat);
            f32 slotRatio =
                func_80139C98(n1, n2, 0, (f32)((u2.d - lbl_eu_80668020) / lbl_eu_80668014));
            u8 v1ab = (u8)func_801361E8((u32)global, base + 0x1ab, cat);
            u8 v1b3 = (u8)func_801361E8((u32)global, base + 0x1b3, cat);
            f32 curRatio = rec.f28;
            if (curRatio > slotRatio) rec.flags1[(u8)i] = 1;
            else if (curRatio < slotRatio) rec.flags1[(u8)i] = 2;
            else rec.flags1[(u8)i] = 0;
            if ((s32)rec.s30 > (s32)v1ab) rec.flags2[(u8)i] = 1;
            else if ((s32)rec.s30 < (s32)v1ab) rec.flags2[(u8)i] = 2;
            else rec.flags2[(u8)i] = 0;
            if ((s32)rec.s32 > (s32)v1b3) rec.flags3[(u8)i] = 1;
            else if ((s32)rec.s32 < (s32)v1b3) rec.flags3[(u8)i] = 2;
            else rec.flags3[(u8)i] = 0;
        }
        rec.f28 = func_80139C98((u32)rec.s2C, (u32)rec.s2E, 0, rec.f38);
    }
    // 0x34-byte copy: pair-copy with s[1]/s[2] accesses reproduces the retail
    // mtctr lwzu/stwu 8-byte-pair loop (li r0,6 + 4-byte tail).
    {
        u32* s = (u32*)&rec - 1;
        u32* d = (u32*)out - 1;
        for (u32 k = 0; k < 6; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
}
#pragma pop
// Retail func_801E1E0C builds the 0x1C-byte comparison record like
// func_801D5274, with two differences: the per-slot flag is the boolean
// (name resolved ? 1 : 0) via subic/subfe, and when the category check
// (0x85/0x84) SUCCEEDS the value-switch path still runs (the switch body is
// shared through the else-block fall-through, so it is emitted once).
// func_801D5274, with two differences: the per-slot flag is the boolean
// (name resolved ? 1 : 0) via subic/subfe, and when the category check
// (0x85/0x84) SUCCEEDS the value-switch path still runs (the switch body is
// shared through the else-block fall-through, so it is emitted once).
// Retail uses the stmw/lmw frame and keeps the mtctr copy loops (the whole
// retail unit is size-optimized), so the function is wrapped in
// optimize_for_size like the other matched stmw-frame functions here.
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801E1E0C(CItemBoxSlotFlags* out, void* arg2, void* arg3) {
    CItemBoxSlotFlags local;
    if (arg3 == NULL) {
        memset(&local, 0, 0x1C);
        // 0x1C-byte copy: 2-word-per-iteration loop under optimize_for_size
        // reproduces the retail mtctr 8-byte-pair loop (li r0,3 + 4-byte
        // tail). The memset+copy is NOT folded here (retail keeps both).
        {
            u32* d = (u32*)out;
            u32* s = (u32*)&local;
            for (u32 k = 0; k < 3; k++) {
                *d++ = *s++;
                *d++ = *s++;
            }
            *d = *s;
        }
        return;
    }
    u32 v1 = func_801392E4((u32)arg3);
    u32 v2 = func_80139358((u32)arg3);
    local.v[0] = (u8)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], (u16)v2);
    local.v[1] = (u8)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], (u16)v2);
    local.v[2] = (u8)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], (u16)v2);
    u32 v3 = func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1eb], (u16)v2);
    for (u8 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 v = func_801392B4((u8)i);
        char buf[0x10];
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], (u8)v);
        u8 f = (u8)func_801361E8((u32)lbl_eu_806640F8, buf, (u16)v2);
        local.flags[i] = (u8)(f != 0);
        if (f != 0) {
            void* lookup = func_8009EC9C((u8)v);
            if ((u8)v3 == 3) {
                if (func_8026178C((u8*)lookup + 0x3534, 0x85) == 0) {
                    local.flags[i] = 0;
                } else {
                    goto valuePath;
                }
            } else if ((u8)v3 == 2) {
                if (func_8026178C((u8*)lookup + 0x3534, 0x84) == 0) {
                    local.flags[i] = 0;
                } else {
                    goto valuePath;
                }
            } else {
            valuePath:
                s16 value = -1;
                switch (v1 & 0xFFFF) {
                    case 4: value = *(s16*)((u8*)lookup + 0x1C); break;
                    case 5: value = *(s16*)((u8*)lookup + 0x1E); break;
                    case 6: value = *(s16*)((u8*)lookup + 0x20); break;
                    case 7: value = *(s16*)((u8*)lookup + 0x22); break;
                    case 8: value = *(s16*)((u8*)lookup + 0x24); break;
                }
                if (value >= 0) {
                    void* r = func_80157C4C((u16)v1, value);
                    if (r != NULL && *(u32*)r != 0) {
                        u16 cat = (u16)func_80139358(*(u32*)r >> 20);
                        u32 n1 = func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], cat);
                        u32 n2 = func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], cat);
                        if (local.v[0] > (u8)n1) local.flags[i + 7] = 1;
                        else if (local.v[0] < (u8)n1) local.flags[i + 7] = 2;
                        else local.flags[i + 7] = 0;
                        if (local.v[1] > (u8)n2) local.flags[i + 14] = 1;
                        else if (local.v[1] < (u8)n2) local.flags[i + 14] = 2;
                        else local.flags[i + 14] = 0;
                    }
                }
            }
        }
    }
    // 0x1C-byte copy: 2-word-per-iteration loop under optimize_for_size
    // reproduces the retail mtctr 8-byte-pair loop (li r0,3 + 4-byte tail).
    {
        u32* d = (u32*)out;
        u32* s = (u32*)&local;
        for (u32 k = 0; k < 3; k++) {
            *d++ = *s++;
            *d++ = *s++;
        }
        *d = *s;
    }
}
#pragma pop
// Retail func_801E20FC (ItemBox2 twin of func_801D5564): builds the 0xA4-byte
// item-name record (count byte at +0, label string pointer, name buffer, two
// state bytes, equip colour, text buffer) then copies it out. Uses the
// stmw/lmw frame (optimize_for_size prologue merge). The label table lives
// in a 0x34-byte stack region that is later copied into the record tail, so
// the 6-pair mtctr copy loop reads it directly.
#pragma push
#pragma optimize_for_size on
void func_801E20FC(void* out, void* unused, void* data, void* arg3) {
    void* item = arg3 != NULL ? arg3 : NULL;
    // Retail hoists this global into a register (r26) across all the calls;
    // a local copy reproduces the single sda21 load in the prologue.
    void* gd8 = lbl_eu_806640D8;
    u32 cat;
    u16 count;
    if (arg3 == NULL) {
        func_801392E4((u32)data);
    }
    if (arg3 != NULL) {
        CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
        cat = inst->_v54(item);
    } else {
        cat = func_80139358((u32)data);
    }
    if (arg3 != NULL) {
        CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
        count = (u16)inst->_v08(item);
    } else {
        count = (u8)func_801361E8((u32)lbl_eu_806640EC, &lbl_eu_805063BC[0x1f9], (u32)data);
    }
    // base is only materialized after the cat/count joins (retail lis/addi at
    // the join point, not in the prologue).
    char* base = (char*)&lbl_eu_805063BC;
    CItemBoxNameRecord2 rec;
    rec.count = (u8)count;
    rec.str = (u32)func_80136190(base + 0x130, base + 0x139, 0x1e - ((u8)count - 1));
    char* s2 = func_80136190(base + 0x202, base + 0x139, 0xf);
    if (arg3 != NULL) {
        CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
        sprintf(rec.name, base, inst->_v90(item));
    } else {
        sprintf(rec.name, base + 0x18, s2);
    }
    rec.e0 = (u8)func_801361E8((u32)gd8, base + 0x3, (u16)cat);
    rec.e1 = (u8)func_801361E8((u32)gd8, base + 0x20b, (u16)cat);
    rec.color = 0xFFFFFFFF;
    switch (rec.e1) {
        case 4: rec.color = 0xFF00FFFF; break;
        case 5: rec.color = 0x0000FFFF; break;
        case 6: rec.color = 0xFFFFFFFF; break;
        case 7: rec.color = 0x0000FFFF; break;
        case 8: rec.color = 0x00FFFFFF; break;
        case 9: rec.color = 0x775544FF; break;
    }
    sprintf(rec.text, base + 0x18, ((char*(*)(void*, const char*, u16))&func_8013639C)(gd8, base + 0xc, (u16)cat));
    u32 wide = 1;
    if (getLanguage__9CDeviceSCFv() != 3) {
        if (getLanguage__9CDeviceSCFv() != 2) {
            wide = 0;
        }
    }
    // Marker-replacement scan over rec.text: a '$' followed by '1'/'2' is
    // replaced by a localized string (the '1' marker switches on the equip
    // state byte, the '2' marker copies the static label-pointer table into
    // the local array and formats the count-indexed label); every other char
    // advances the scan Shift-JIS-aware (lead-byte ranges advance 2). After
    // a marker, the remaining text is spliced after the replacement.
    char* labels = (char*)lbl_eu_80506380;
    u32 labels2[4];
    char buf28[0x20];
    char buf48[0x20];
    char* cur = rec.text;
    while (*cur != 0) {
        if (*cur == '$') {
            memset(buf28, 0, 0x20);
            cur++;
            if (*cur == '1') {
                cur++;
                switch (rec.e0) {
                    case 1:
                        sprintf(buf28, base + 0x1b, rec.name);
                        break;
                    case 2:
                        sprintf(buf28, base + 0x1b, rec.name);
                        break;
                    case 3:
                        if (wide != 0) {
                            sprintf(buf28, base + 0x30, rec.name);
                        } else {
                            sprintf(buf28, base + 0x48, rec.name);
                        }
                        break;
                    case 4:
                        if (wide != 0) {
                            sprintf(buf28, base + 0x30, rec.name);
                        } else {
                            sprintf(buf28, base + 0x48, rec.name);
                        }
                        break;
                    default:
                        break;
                }
            } else if (*cur == '2') {
                // 0x24-byte copy of the static label-pointer table into the
                // local array (retail mtctr 4-pair loop + 4-byte tail).
                {
                    u32* s = (u32*)labels - 1;
                    u32* d = (u32*)labels2 - 1;
                    for (u32 k = 0; k < 4; k++) {
                        d[1] = s[1];
                        d[2] = s[2];
                        s += 2;
                        d += 2;
                    }
                    d[1] = s[1];
                }
                cur++;
                u32 name = func_801361E8((u32)gd8, (char*)labels2[rec.count], (u16)cat);
                if (wide != 0) {
                    sprintf(buf28, base + 0x5f, (u8)name);
                } else {
                    sprintf(buf28, base + 0x77, (u8)name);
                }
            }
            // strlen of buf28 (post-increment counter: retail counts len+1).
            char* p = buf28;
            u32 len = 0;
            while (*p != 0) {
                len++;
                p++;
            }
            sprintf(buf48, base + 0x18, cur);
            cur = cur - 2;
            sprintf(cur, base + 0x18, buf28);
            cur = cur + len - 1;
            sprintf(cur, base + 0x18, buf48);
        } else {
            s8 sc = (s8)*cur;
            if ((sc >= 0x81 && sc <= 0x9f) || (sc >= 0xe0 && sc <= 0xef)) {
                cur += 2;
            } else {
                cur += 1;
            }
        }
    }
    // 0xA4-byte copy: pair-copy with s[1]/s[2] accesses reproduces the retail
    // mtctr lwzu/stwu 8-byte-pair loop (li r0,0x14 + 4-byte tail).
    {
        u32* s = (u32*)&rec - 1;
        u32* d = (u32*)out - 1;
        for (u32 k = 0; k < 0x14; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// Twin of func_801D59C0 (see comment there). dont_inline keeps IPA from
// folding this body into func_801E3730.
void func_801E2558(u32* out, void* info, void* arg2) {
    CItemBoxLabelRec r;
    void* tbl = lbl_eu_80664104;
    func_801392E4((u32)arg2);
    // cat stays a raw u16 in a register; each call site re-applies the
    // u16->u32 argument conversion (retail clrlwi per site, no shared temp).
    u16 cat = func_80139358((u32)arg2);
    // base is anchored at the first lookup so its definition schedules after
    // the cat move (retail order: or r30,r3 then lis/addi).
    r.a = (u8)func_801361E8((u32)tbl, (char*)&lbl_eu_805063BC + 0x214, cat);
    char* base = (char*)&lbl_eu_805063BC;
    r.b = func_80136190(base + 0x219, base + 0x139, r.a);
    // Residual: retail narrows this result into r5, we pick r0 - a
    // scheduler-driven compare-temp allocation choice insensitive to every
    // source shape tried (named local, raw+cast, assign-in-cond, & 0xFF).
    r.c = (u8)func_801361E8((u32)tbl, base + 0x225, cat);
    if (r.c == 0x1A) {
        r.d = func_80136190(base + 0x219, base + 0x139, 0x14);
    } else {
        r.d = func_8013639C(lbl_eu_806640A8, base + 0x139);
    }
    // Word-wise copy: retail expands the record copy as four lwz/stw pairs.
    u32* src = (u32*)&r;
    out[0] = src[0];
    out[1] = src[1];
    out[2] = src[2];
    out[3] = src[3];
}
#pragma pop

// Retail func_801E2638 (ItemBox2 variant of func_801D5AA0): fills the 8-byte
// slot table (flag byte + per-slot selected flags) and a flag2 result from the
// item-table lookup, then writes all three to the info block. Retail uses the
// stmw/lmw frame (optimize_for_size prologue merge).
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801E2638(CItemBoxSlotSelTable* out, CItemBoxInfo2* unused, void* data) {
    // Table handle is anchored before the first call so its sdata2 load
    // schedules above the func_801392E4 call (retail lwz r26 first).
    void* global = lbl_eu_80664110;
    u8 arr[8];
    u8 flag2;
    func_801392E4((u32)data);
    u16 cat = func_80139358((u32)data);
    // base is anchored after the cat move (retail: mr r27,r3 then lis/addi).
    char* base = (char*)&lbl_eu_805063BC;
    arr[0] = (u8)func_801361E8((u32)global, base + 0x22b, cat);
    u32 count = func_801361E8((u32)global, base + 0x237, cat);
    u8 count8 = (u8)count;
    for (u8 i = 0; i < code80135FDC_getByte_64077(); i++) {
        u8 a = (u8)func_801392B4(i);
        // plain bool store: MWCC lowers == to subf/cntlzw/srwi here
        arr[i + 1] = a == count8;
    }
    u32 key = func_801361E8((u32)global, base + 0x23f, cat);
    u32 val = func_8013600C(base + 0x248, base + 0x250, (u8)key);
    void* lookup = func_8009EC9C((u8)count);
    u8 category = (u8)func_800A32BC();
    // Per-category item row: stride 0x49, pairs of bytes per entry.
    u8* entry = (u8*)lookup + category * 0x49 + (u8)val * 2;
    flag2 = 0;
    switch (arr[0]) {
        case 1: if (entry[0xE8] != 0) flag2 = 1; break;
        case 2: if ((entry[0xE9] >> 0) & 1) flag2 = 1; break;
        case 3: if ((entry[0xE9] >> 1) & 1) flag2 = 1; break;
    }
    // Word-wise copy of the 8-byte slot table plus the trailing flag byte.
    out->selWords[0] = *(u32*)arr;
    out->selWords[1] = *(u32*)(arr + 4);
    out->flag2 = flag2;
}
#pragma pop
// Retail func_801E27D0 fills a 0x24-byte slot record (count byte, string ptr,
// per-item text ptrs at +8, per-item values at +0x1C, counter at +0x21) then
// copies it out. The func_801392E4/80139358 results are discarded (dead calls
// kept because MWCC never elides calls). Param 2 (info) is unused in retail.
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801E27D0(u8* out, void* unused, void* item, void* arg4) {
    u8* dst = out;
    CItemBoxSlotRecord rec;
    void* p = arg4 != 0 ? arg4 : 0;
    func_801392E4((u32)item);
    func_80139358((u32)item);
    void* inst = CItem_initItemImplInstances(p);
    u8 r = ((u8(*)(void*, void*))(*(void***)inst)[2])(inst, p);
    rec.count = r;
    char* base = (char*)&lbl_eu_805063BC;
    rec.str = (u32)func_80136190(&base[0x130], &base[0x139], 0x1e - (r - 1));
    rec.tail[1] = 0;
    for (u32 i = 0; i < 4; i++) {
        void* inst2 = CItem_initItemImplInstances(p);
        u16 n = ((u16(*)(void*, void*, u32))(*(void***)inst2)[19])(inst2, p, (u8)i);
        if (n > 0) {
            rec.text[rec.tail[1]] = (u32)func_8013639C(lbl_eu_806640D8, &base[0x139]);
            void* inst3 = CItem_initItemImplInstances(p);
            u8 val = ((u8(*)(void*, void*, u32))(*(void***)inst3)[25])(inst3, p, (u8)i);
            rec.vals[rec.tail[1]] = val;
            rec.tail[1]++;
        }
    }
    // 4x 8-byte pair loop + 4-byte tail == retail mtctr copy shape; the inner
    // constant-trip loop is what keeps -O4,p from unrolling the outer loop.
    u32* s = (u32*)&rec - 1;
    u32* d = (u32*)dst - 1;
    for (u32 k = 0; k < 4; k++) {
        u32 t1 = s[1];
        u32 t2 = s[2];
        d[1] = t1;
        d[2] = t2;
        s += 2;
        d += 2;
    }
    d[1] = s[1];
}
#pragma pop
// Render the item-box slot info (equip-category 2 items): for each slot
// (count from the slot-table byte getter) set up a labelled layout pane,
// per-slot vertex colours, and (when a selection applies) a texture/name tag.
// Retail keeps the stmw/lmw frame (optimize_for_size prologue merge) and the
// mtctr copy loop for the 0x34-byte comparison record.
#pragma push
#pragma optimize_for_size on
void func_801E2928(CItemBoxInfo2* info, u16 arg1, void* arg2, u16 arg3) {
    func_801E3918(info);
    // Declaration order follows the retail frame layout (high -> low):
    // copied record, pane-name/label buffers, source record, colour defaults.
    CItemBoxCompRecord local;
    char paneName[0x20];
    char label[0x20];
    CItemBoxCompRecord record;
    func_801E197C(&record, info, (void*)arg1);
    // 0x34-byte copy: pair-copy with s[1]/s[2] accesses reproduces the retail
    // mtctr lwzu/stwu 8-byte-pair loop (li r0,6 + 4-byte tail).
    {
        // dst declared first: low-degree webs color in reverse birth order,
        // putting the source pointer in the lower retail register.
        u32* d = (u32*)&local - 1;
        u32* s = (u32*)&record - 1;
        for (u32 k = 0; k < 6; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
    char* base = (char*)&lbl_eu_805063BC;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = local._16[(u8)i];
        u32 color = 0x777777FF;
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(paneName, base + 0x303, (u8)i + 1);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, paneName, color);
        // Default vertex colours carry data only in the alpha channel
        // (info+0xA2 / info+0xAA); the selected-slot pair comes from
        // info+0x9C/0xA4 when the slot is active.
        s16 vA2 = *(s16*)((u8*)info + 0xA2);
        s16 vAA = *(s16*)((u8*)info + 0xAA);
        // defAA declared first: retail keeps the vAA colour in the lower
        // frame slot pair (sp+0x08) and the vA2 colour above it (sp+0x10).
        CItemBoxQuadColor defAA;
        defAA.b = 0;
        defAA.g = 0;
        defAA.r = 0;
        defAA.a = vAA;
        CItemBoxQuadColor defA2;
        defA2.b = 0;
        defA2.g = 0;
        defA2.r = 0;
        defA2.a = vA2;
        u32 args[4];
        args[0] = 0;
        args[1] = 0;
        args[2] = 0;
        args[3] = 0;
        if (flag != 0) {
            args[0] = *(u32*)((u8*)info + 0x9C);
            args[1] = *(u32*)((u8*)info + 0xA0);
        } else {
            args[0] = *(u32*)&defA2.r;
            args[1] = *(u32*)&defA2.a;
        }
        if (flag != 0) {
            args[2] = *(u32*)((u8*)info + 0xA4);
            args[3] = *(u32*)((u8*)info + 0xA8);
        } else {
            args[2] = *(u32*)&defAA.r;
            args[3] = *(u32*)&defAA.a;
        }
        sprintf(paneName, base + 0x161, (u8)i + 1, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(paneName, true);
        if (pane != NULL) {
            void* obj = ((CItemBoxPaneVt*)pane)->getColorObject();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(obj, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        if (flag != 0 && (arg2 != NULL || arg1 != 0)) {
            u8 slot = (u8)func_801392B4((u8)i);
            if (slot != 0) {
                void* member = func_8009EC9C(slot);
                // Fixed equip category 2: candidate value read at member+0x26.
                void* item = func_80157C4C(2, *(s16*)((u8*)member + 0x26));
                if (item != NULL && *(u32*)item != 0) {
                    if (arg2 == item || arg1 == (u32)(*(u32*)item >> 20)) {
                        sprintf(label, base + 0x30e, (u8)i + 1);
                        u8 otherSlot = (u8)func_801392B4(arg3);
                        u32 tex;
                        if (slot == otherSlot) {
                            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)(base + 0x319), 0);
                        } else {
                            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)(base + 0x32d), 0);
                        }
                        if (tex == 0) {
                            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)(base + 0x341), 0);
                        }
                        if (tex != 0) {
                            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, label, tex);
                        }
                    }
                }
            }
        }
    }
}
#pragma pop
// Retail func_801E2C5C uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) and keeps the mtctr copy loop for the 0x1C-byte record
// copy, so it is wrapped in optimize_for_size like the other stmw-frame
// functions in this unit.
// dont_inline: retail keeps this out-of-line; MWCC would otherwise inline it
// into func_801E14DC (forward declaration makes the body visible).
// dont_inline is NOT used here: retail emits the record copy and colour
// staging as inline sequences, and func_801E14DC's auto_inline-off region
// already keeps this body out of its caller.
#pragma push
#pragma optimize_for_size on
// dont_inline: retail keeps this out-of-line; MWCC would otherwise inline it
// into func_801E14DC (forward declaration makes the body visible).
#pragma dont_inline on
void func_801E2C5C(CItemBoxInfo2* info, u16 arg2, void* arg3, u16 arg4) {
    func_801E3B9C(info);
    // Frame-slot order: record > buf > bufName > local > args > cols >
    // selB/selA (the staged colour temps; retail keeps its copies above the
    // record, ours sit lowest - offsets inside the staging block differ).
    CItemBoxSlotFlagsAny record;
    char buf[0x20];
    char bufName[0x20];
    CItemBoxSlotFlagsAny local;
    u32 args[4];
    CItemBoxColorRaw cols[2];
    CItemBoxColorBlock selB;
    CItemBoxColorBlock selA;
    func_801E1E0C(&local.rec, info, (void*)(u32)arg2);
    // 0x1C-byte record copy: load both words of each pair before storing so
    // MWCC keeps the retail lwz/lwzu + stw/stwu pair-loop shape.
    {
        u32* s = (u32*)&local.rec - 1;
        u32* d = (u32*)&record.rec - 1;
        for (u32 k = 0; k < 3; k++) {
            u32 a = s[1];
            u32 b = s[2];
            d[1] = a;
            d[2] = b;
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
    char* base = (char*)&lbl_eu_805063BC;
    // Hoisted halves: MWCC materializes these once (lis) and rebuilds each
    // constant at its use site with a single addi.
    u32 darkBase = 0x77770000;
    u32 tagHi = 0x74690000;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = record.rec.flags[(u8)i];
        u32 color = darkBase + 0x77ff;  // 0x777777ff
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(buf, base + 0x303, (u8)i + 1);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, color);
        s16 vA2 = *(s16*)((u8*)info + 0xA2);
        s16 vAA = *(s16*)((u8*)info + 0xAA);
        // Retail builds cols[1] first, then cols[0], each as three zero
        // halfwords plus the alpha from the info block.
        cols[1].r = 0; cols[1].g = 0; cols[1].b = 0; cols[1].a = vA2;
        cols[0].r = 0; cols[0].g = 0; cols[0].b = 0; cols[0].a = vAA;
        args[0] = 0;
        args[1] = 0;
        args[2] = 0;
        args[3] = 0;
        // Selected colour is staged through the named temp then reloaded into
        // the sprintf argument block (retail stores to sp+0xa8/sp+0xb0 first).
        if (flag != 0) {
            selA.w[0] = *(u32*)((u8*)info + 0x9C);
            selA.w[1] = *(u32*)((u8*)info + 0xA0);
        } else {
            selA = *(CItemBoxColorBlock*)&cols[1];
        }
        args[0] = selA.w[0];
        args[1] = selA.w[1];
        if (flag != 0) {
            selB.w[0] = *(u32*)((u8*)info + 0xA4);
            selB.w[1] = *(u32*)((u8*)info + 0xA8);
        } else {
            selB = *(CItemBoxColorBlock*)&cols[0];
        }
        args[2] = selB.w[0];
        args[3] = selB.w[1];
        sprintf(buf, base + 0x161, (u8)i + 1, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* obj = ((CItemBoxPaneVt*)pane)->getColorObject();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(obj, (u8)j, (u8*)args + (u32)j * 8);
            }
        }
        if (flag != 0 && (arg3 != NULL || arg2 != 0)) {
            u8 slot = (u8)func_801392B4((u8)i);
            if (slot != 0) {
                void* member = func_8009EC9C(slot);
                u16 category = (u16)func_801392E4((u32)arg2);
                s16 value = -1;
                switch (category) {
                    case 4: value = *(s16*)((u8*)member + 0x1C); break;
                    case 5: value = *(s16*)((u8*)member + 0x1E); break;
                    case 6: value = *(s16*)((u8*)member + 0x20); break;
                    case 7: value = *(s16*)((u8*)member + 0x22); break;
                    case 8: value = *(s16*)((u8*)member + 0x24); break;
                }
                void* item = func_80157C4C(category, value);
                if (item != NULL && *(u32*)item != 0) {
                    if (arg3 == item || arg2 == (*(u32*)item >> 20)) {
                        sprintf(bufName, base + 0x30e, (u8)i + 1);
                        u32 tex;
                        if (slot == (u8)func_801392B4(arg4)) {
                            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(tagHi + 0x6d67, (u32)(base + 0x319), 0);
                        } else {
                            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(tagHi + 0x6d67, (u32)(base + 0x32d), 0);
                        }
                        if (tex == 0) {
                            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(tagHi + 0x6d67, (u32)(base + 0x341), 0);
                        }
                        if (tex != 0) {
                            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, bufName, tex);
                        }
                    }
                }
            }
        }
    }
}
#pragma pop
// end func_801E2C5C
#pragma push
#pragma optimize_for_size on
// Retail func_801E2FEC: refresh the item-box slot rows. Fetches the per-slot
// selection table via func_801E2638 into a stack record, sets the row label to
// the category name (length 0x2c vs 0x2b depending on flag2), then for each
// visible slot colours the pane (white when selected) and pushes two vertex
// colours - the item-specific pair from info+0x9c when selected, otherwise
// alpha-only defaults built from info+0xa2/0xaa - through func_801D62F8.
void func_801E2FEC(CItemBoxInfo2* info, u16 arg2) {
    func_801E3EB8(info);
    func_801E3B9C(info);
    // Selection-colour staging: the aggregate ternaries below force MWCC to
    // materialize each selected 8-byte colour into a stack temp (retail
    // sp+0x68..0x74) before copying it into out[].
    char buf[0x20];
    CItemBoxSlotSelTable local;
    CItemBoxColorBlock out[2];
    CItemBoxSlotSelTable record;
    CItemBoxColorRaw cols[2];
    // 9-byte per-slot selection record: bytes [1..8] are the per-slot flags,
    // byte [8] also selects the row-height label size.
    func_801E2638(&record, info, (void*)(u32)arg2);
    local.selWords[0] = record.selWords[0];
    local.selWords[1] = record.selWords[1];
    local.flag2 = record.flag2;
    char* base = (char*)&lbl_eu_805063BC;
    u32 rowHeight = 0x2c;
    if (local.flag2 != 0) rowHeight = 0x2b;
    char* label = func_80136190(base + 0x130, base + 0x139, rowHeight);
    func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), base + 0x44f, label, 0);
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = ((u8*)&local)[(u8)i + 1];
        u32 color = 0x777777ff;
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(buf, base + 0x303, (u8)i + 1);
        func_80137B44((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf, color);
        s16 vA2 = *(s16*)((u8*)info + 0xA2);
        s16 vAA = *(s16*)((u8*)info + 0xAA);
        // Retail builds cols[1] first, then cols[0], each as three zero
        // halfwords plus the alpha from the info block.
        cols[1].r = 0; cols[1].g = 0; cols[1].b = 0; cols[1].a = vA2;
        cols[0].r = 0; cols[0].g = 0; cols[0].b = 0; cols[0].a = vAA;
        out[0].w[0] = 0;
        out[0].w[1] = 0;
        out[1].w[0] = 0;
        out[1].w[1] = 0;
        // Aggregate ternaries: selected 8-byte colour is staged through a
        // stack temp then copied as one block (retail lwz/stw pairs).
        out[0] = (flag != 0) ? *(CItemBoxColorBlock*)((u8*)info + 0x9C)
                             : *(CItemBoxColorBlock*)&cols[1];
        out[1] = (flag != 0) ? *(CItemBoxColorBlock*)((u8*)info + 0xA4)
                             : *(CItemBoxColorBlock*)&cols[0];
        sprintf(buf, base + 0x161, (u8)i + 1, cols[0]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* obj = ((CItemBoxPaneVt*)pane)->getColorObject();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(obj, (u8)j, &out[j]);
            }
        }
    }
}
#pragma pop
// Retail func_801E3228 uses the stmw/lmw 0x200 frame (optimize_for_size
// prologue merge). Pipeline: two layout preps, then func_801E20FC fills a
// 0xA0-byte label record that is struct-copied to a local (20-pair mtctr
// loop) and stamped onto the fixed panes; the candidate item's name/category
// labels follow. The per-slot loop colours each row (red when unselected,
// white + the info-block vertex colours when selected) and, for slots whose
// six equip values resolve to the candidate item, stamps its texture.
#pragma push
#pragma optimize_for_size on
void func_801E3228(CItemBoxInfo2* info, u16 arg2, void* arg3, u16 arg4) {
    // Frame-slot order matters: vals > out > textBuf > buf2 > cols > entry
    // > stage (descending first-declared = highest slot).
    CItemBoxInfo2LabelRec vals;
    CItemBoxInfo2LabelRec out;
    char textBuf[0x20];
    char buf2[0x20];
    CItemBoxVertexColors cols;
    CItemBoxSlotEntry entry;
    CItemBoxTagStage stage;

    func_801E4010(info);
    func_801E3B9C(info);
    func_801E20FC(&out, info, (void*)(u32)arg2, arg3);
    // 0xA0-byte record copy: pair-copy reproduces the retail mtctr lwzu/stwu
    // 8-byte-pair loop (li r0,0x14, no tail). NOTE: struct assign is not
    // usable here - MWCC emits an out-of-line __as__ call for any 0xA0-byte
    // record (scalar-only or not), so the copy must stay a manual loop.
    {
        // volatile forces retail's conservative load-load/store-store order;
        // non-volatile locals let the scheduler interleave the pairs.
        volatile u32* d = (volatile u32*)&vals - 1;
        volatile u32* s = (volatile u32*)&out - 1;
        for (u32 k = 0; k < 20; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
    }
    // Retail materializes the pool base only after the record copy.
    char* base = (char*)&lbl_eu_805063BC;

    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x45a, (char*)vals.str, 0);
    if ((s8)vals.name[0] == 0x30) {
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x466, base + 0x2aa, 0);
    } else {
        if (vals.e0 != 0) {
            if ((u8)(vals.e0 - 3) <= 1) {
                sprintf(textBuf, base + 0x419, vals.name,
                        func_80136190(base + 0x130, base + 0x139, 0x21));
            } else {
                sprintf(textBuf, base + 0x41e, vals.name);
            }
        } else {
            sprintf(textBuf, base + 0x41e, vals.name);
        }
        func_80136A1C((nw4r::lyt::Layout*)info->state.layout, base + 0x466, textBuf, 0);
    }

    // Candidate item name (via the item vtable) or the raw item-name lookup.
    void* item;
    char* txt;
    if (arg3 != NULL) {
        item = arg3;
        txt = (char*)((CItemImplVt*)CItem_initItemImplInstances(item))->_v20(item);
    } else {
        item = NULL;
        txt = ((char* (*)(u32))func_801394D4)(arg2);
    }
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x472, txt, 0);

    u16 cat;
    if (arg3 != NULL) {
        cat = (u16)((CItemImplVt54*)CItem_initItemImplInstances(item))->_v54(item);
    } else {
        cat = func_80139358(arg2);
    }
    u32 st = func_801361E8((u32)lbl_eu_806640D8, base + 0x47d, cat);
    char* label = func_80136190(base + 0x130, base + 0x139, (u8)st + 0x15);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x484, label, 0);

    // Table handle pair is loop-invariant: staged once before the loop.
    stage.tbl = lbl_eu_80668064;
    stage.h = lbl_eu_80668068;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(textBuf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, textBuf, -1);
        // Row colour pair: zeroed memberwise, then filled from the info
        // selection block; passed to sprintf as one 0x10-byte vararg.
        cols.w[0] = 0; cols.w[1] = 0; cols.w[2] = 0; cols.w[3] = 0;
        cols.w[0] = *(u32*)((u8*)info + 0x9C);
        cols.w[1] = *(u32*)((u8*)info + 0xA0);
        cols.w[2] = *(u32*)((u8*)info + 0xA4);
        cols.w[3] = *(u32*)((u8*)info + 0xA8);
        sprintf(textBuf, base + 0x161, idx, cols);
        nw4r::lyt::Pane* pane =
            ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(textBuf, true);
        if (pane != NULL) {
            void* obj = ((CItemBoxPaneVt*)pane)->getColorObject();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(obj, (u8)j, (u8*)&cols + (u8)j * 8);
            }
        }
        if (arg3 == NULL) continue;
        // Slot tag must resolve; then stage the slot's equip-value record.
        u8 slot = (u8)func_801392B4((u8)i);
        if (slot == 0) continue;
        void* lookup = func_8009EC9C(slot);
        entry.tbl = stage.tbl;
        entry.h = stage.h;
        entry.vals[0] = *(s16*)((u8*)lookup + 0x26);
        entry.vals[1] = *(s16*)((u8*)lookup + 0x1C);
        entry.vals[2] = *(s16*)((u8*)lookup + 0x1E);
        entry.vals[3] = *(s16*)((u8*)lookup + 0x20);
        entry.vals[4] = *(s16*)((u8*)lookup + 0x22);
        entry.vals[5] = *(s16*)((u8*)lookup + 0x24);
        for (u32 j = 0; (u8)j < 6; j++) {
            void* cand = func_80157C4C((u8)j, entry.vals[(u8)j]);
            if (cand == NULL || *(u32*)cand == 0) continue;
            u8 cnt = ((CItemImplVt*)CItem_initItemImplInstances(cand))->_v30(cand);
            for (u32 k = 0; (u8)k < cnt; k++) {
                s16 itemVal =
                    ((CItemImplVt*)CItem_initItemImplInstances(cand))->_v40(cand, (u8)k);
                void* hit = func_80157C4C(3, itemVal);
                if (hit == NULL || *(u32*)hit == 0 || hit != arg3) continue;
                sprintf(buf2, base + 0x30e, idx);
                u32 tex;
                if (slot == (u8)func_801392B4(arg4)) {
                    tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)
                              ->findName(0x74696d67, (u32)(base + 0x319), 0);
                } else {
                    tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)
                              ->findName(0x74696d67, (u32)(base + 0x32d), 0);
                }
                if (tex == 0) {
                    tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)
                              ->findName(0x74696d67, (u32)(base + 0x341), 0);
                }
                if (tex != 0) {
                    func_80137E7C((nw4r::lyt::Layout*)info->state.layout, buf2, tex);
                }
            }
        }
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
void func_801E3730(CItemBoxInfo2* info, u32 arg2) {
    func_801E4090(info);
    func_801E3B9C(info);
    // Out-buffer copy: retail reloads all four words from the escaped buffer
    // and re-homes them via a plain struct-style copy before the calls.
    // vals declared first lands at the higher frame slot (retail places the
    // staged copy above the E2558 out-buffer).
    CItemBoxLabelArgs vals;
    CItemBoxLabelArgs out;
    func_801E2558(out.v, info, (void*)arg2);
    vals = out;
    char* base = lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x48f, (char*)vals.v[1], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x49b, (char*)vals.v[3], 0);
}
#pragma pop
// Retail func_801E37C4: run the two layout preps, build a 0x24-byte slot
// record via func_801E27D0, struct-copy it to a local (retail mtctr 8-byte
// pair loop), then label every slot: a count-driven loop of pane-name
// sprintf + per-item text/value colour labels (inlined FixStr<32> ctor).
#pragma push
#pragma optimize_for_size on
#pragma auto_inline off
#pragma dont_inline on
// Word-array view: assigning the union copies the whole object as one block
// (reproduces the retail mtctr lwzu/stwu pair loop) instead of expanding
// field-by-field.
union CItemBoxSlotRecAny {
    CItemBoxSlotRecord rec;
    u32 w[9];
};
void func_801E37C4(CItemBoxInfo2* info, void* arg1, void* arg2) {
    // Frame-slot order matters: cur > text > out > paneName (descending).
    CItemBoxSlotRecAny cur;
    ml::FixStr<32> text(false);
    CItemBoxSlotRecAny out;
    char paneName[0x20];
    func_801E40E8(info);
    func_801E3B9C(info);
    func_801E27D0((u8*)&out.rec, info, arg1, arg2);
    cur = out;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x4a7], (char*)cur.rec.str, 0);
    u8 count = cur.rec.tail[1];
    for (u8 i = 0; i < count; i++) {
        sprintf(paneName, &lbl_eu_805063BC[0x4b3], (u8)i * 2 + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, paneName, (char*)cur.rec.text[i], 0);
        text.clear();
        sprintf(paneName, &lbl_eu_805063BC[0x4c0], (u8)i + 0x1f);
        text.format(&lbl_eu_805063BC[0x13e], cur.rec.vals[i],
                    func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x21));
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, paneName, text.c_str(), 0);
    }
}
#pragma auto_inline reset
#pragma pop
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// dont_inline keeps IPA from folding this body into its callers
// (func_801E14DC etc.) - retail always calls it.
void func_801E3918(CItemBoxInfo2* info) {
    // i declared first: retail claims r31 for the loop counter before any
    // other saved local (register_mapping.md Rule A).
    // No named base local: retail holds &lbl_eu_805063BC purely as a compiler
    // CSE temporary (lis into a scratch register during prologue argument
    // prep), LICM-promoted to a saved register for the loop.
    char buf[0x20];
    u32 i;
    u32 idx;
    s16 zero = 0;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x25b], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x267], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x273], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x286], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x292], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x29e], &lbl_eu_805063BC[0x2aa], 0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2b6], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2c1], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2cc], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2d7], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2e2], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2ed], &lbl_eu_806645A8, &lbl_eu_806645B0);
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        idx = (u8)i + 1;
        sprintf(buf, &lbl_eu_805063BC[0x303], idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, 0x777777ff);
        u32 args[4];
        s16 c0hi[2] = {zero, *(s16*)((u8*)info + 0xA2)};
        s16 c0lo[2] = {zero, zero};
        s16 c1hi[2] = {zero, *(s16*)((u8*)info + 0xAA)};
        s16 c1lo[2] = {zero, zero};
        args[1] = *(u32*)c0hi;
        args[0] = *(u32*)c0lo;
        args[3] = *(u32*)c1hi;
        args[2] = *(u32*)c1lo;
        sprintf(buf, &lbl_eu_805063BC[0x161], idx, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(material, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(buf, &lbl_eu_805063BC[0x30e], idx);
        u32 tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)&lbl_eu_805063BC[0x341], 0);
        if (tex != 0) {
            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, buf, tex);
        }
    }
}
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// dont_inline keeps IPA from folding this body into its callers
// (func_801E3730 etc.) - retail always calls it.
void func_801E3B9C(CItemBoxInfo2* info) {
    // No named base local: retail holds &lbl_eu_805063BC purely as a
    // compiler CSE temporary (lis into a scratch register during prologue
    // argument prep), which fixes both its schedule and its register.
    char text[0x20];
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x267], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x273], &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x29e], &lbl_eu_805063BC[0x2aa], 0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2b6], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2c1], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2cc], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2d7], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2e2], &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x2ed], &lbl_eu_806645A8, &lbl_eu_806645B0);
    // Zero lives outside the loop: retail materializes it once in the
    // preheader (a loop-invariant CSE), not per-iteration.
    s16 shade = 0;
    for (u32 index = 0; (u8)index < (u8)code80135FDC_getByte_64077(); index++) {
        u32 slot = (u8)index + 1;
        sprintf(text, &lbl_eu_805063BC[0x303], slot);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, text, 0x777777ff);
        u32 args[4];
        s16 hiA[2] = {shade, *(s16*)((u8*)info + 0xA2)};
        s16 loA[2] = {shade, shade};
        s16 hiB[2] = {shade, *(s16*)((u8*)info + 0xAA)};
        s16 loB[2] = {shade, shade};
        args[1] = *(u32*)hiA;
        args[0] = *(u32*)loA;
        args[3] = *(u32*)hiB;
        args[2] = *(u32*)loB;
        sprintf(text, &lbl_eu_805063BC[0x161], slot, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pPane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(text, true);
        if (pPane != NULL) {
            nw4r::lyt::Material* pMaterial = pPane->GetMaterial();
            for (u32 k = 0; k < 2; k++) {
                func_801D62F8(pMaterial, (u8)k, (u8*)args + (u8)k * 8);
            }
        }
        // Item-name lookup on the shared arc resource accessor (vtable+0x0C),
        // then push the texture name onto the slot pane.
        sprintf(text, &lbl_eu_805063BC[0x30e], slot);
        u32 texId = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)&lbl_eu_805063BC[0x341], 0);
        if (texId != 0) {
            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, text, texId);
        }
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
void func_801E3DE4(CItemBoxInfo2* info) {
    char buf[0x20];
    // Declaration order fixes r29/r30: retail claims index (r30) before i (r29).
    u32 index;
    u32 i;
    for (i = 0; i < 3; i++) {
        index = (u8)i + 1;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x35f], index);
        void* r = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(buf, true);
        func_80124270(r, 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x408], index);
        func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x426], index);
        func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801E3EB8(CItemBoxInfo2* info) {
    // Retail uses the stmw/lmw frame (optimize_for_size prologue merge).
    // dont_inline keeps IPA from restructuring this leaf renderer.
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x44f], &lbl_eu_805063BC[0x2aa], 0);
    char buf[0x20];
    u32 i;
    u32 idx;
    s16 zero = 0;
    for (i = 0; i < (u8)code80135FDC_getByte_64077(); i++) {
        idx = (u8)i + 1;
        sprintf(buf, &lbl_eu_805063BC[0x303], idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, 0x777777ff);
        u32 args[4];
        // zero via a variable: MWCC materializes it in a register and cannot
        // fold the {zero, zero} s16-pair arrays to a constant,
        // keeping the retail stack build of the four u32 colour args.
        s16 c0hi[2] = {0, *(s16*)((u8*)info + 0xA2)};
        s16 c0lo[2] = {0, 0};
        s16 c1hi[2] = {0, *(s16*)((u8*)info + 0xAA)};
        s16 c1lo[2] = {0, 0};
        args[0] = *(u32*)c0lo;
        args[1] = *(u32*)c0hi;
        args[2] = *(u32*)c1lo;
        args[3] = *(u32*)c1hi;
        sprintf(buf, &lbl_eu_805063BC[0x161], idx, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            // Real virtual dispatch through CItemBoxPaneVt keeps the ABI r12
            // vtable load (a manual (*(void***)pane)[26] cast colors r5).
            void* obj = ((CItemBoxPaneVt*)pane)->getColorObject();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(obj, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
    }
}
#pragma pop
// Retail uses the stmw/lmw frame (MWCC optimize_for_size prologue merge);
// the first call's arg4 zero and the string base are hoisted into the prologue.
#pragma push
#pragma optimize_for_size on
void func_801E4010(CItemBoxInfo2* info) {
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x45a],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x466],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x484],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x472],
                  &lbl_eu_805063BC[0x2aa], 0);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma push
#pragma dont_inline on
void func_801E4090(CItemBoxInfo2* info) {
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x48f],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x49b],
                  &lbl_eu_805063BC[0x2aa], 0);
}
#pragma pop
#pragma pop
// Retail func_801E40E8 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge): info in r29, loop counter in r30, string-pool base in r31.
// dont_inline keeps IPA from folding this body into its callers
// (func_801E37C4 etc.) - retail always calls it.
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
void func_801E40E8(CItemBoxInfo2* info) {
    char buf[0x20];
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x4a7],
                  &lbl_eu_805063BC[0x2aa], 0);
    for (u32 i = 0; i < 4; i++) {
        sprintf(buf, &lbl_eu_805063BC[0x4b3], (u8)i * 2 + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, &lbl_eu_805063BC[0x4c0], (u8)i + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
    }
}
#pragma pop

// Retail func_801E4194: ItemBox2 variant of func_801D8C0C (same body, same
// stmw r22 frame). Loops 7 slots; below the active count it resolves the item
// name via func_801355F4/vtable+0x0C and colors the pane; otherwise a plain
// label pane. The two alpha-only vertex colours are packed into a 4-word block
// shared by sprintf and the per-vertex func_801D62F8 loop.
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
extern "C" void func_801E4194(CItemBoxInfo2* info) {
    u32 i;
    char bufElse[0x20];
    char buf[0x20];
    u8 count = code80135FDC_getByte_64077();
    u32 idx;
    for (i = 0; i < 7; i++) {
        if ((u8)i >= count) {
            sprintf(bufElse, (char*)&lbl_eu_805063BC[0x161], (u8)i + 1);
            nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(bufElse, true);
            func_80124270(pane, 0);
        } else {
            u8 slot = (u8)func_801392B4((u8)i);
            u32 tag = 0x74696D67u;
            u32 nameId = (u16)func_80136254(lbl_eu_80664090, (char*)&lbl_eu_805063BC[0x4ce], slot);
            u32 itemId = (u32)func_80138F78(nameId);
            // Item-name lookup through the shared name system (vtable+0x0C).
            u32 found = ((CItemNameLookupVt*)func_801355F4())->findName(tag, itemId, 0);
            if (found != 0) {
                idx = (u32)((u8)i + 1);
                sprintf(buf, (char*)&lbl_eu_805063BC[0x303], idx);
                nw4r::lyt::Pane* pane2 = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
                func_80137F88(pane2, found);
                func_80137C1C(pane2, 0x777777ffu);
                // Selection colours: two 8-byte pairs built from the s16s at +0xA2
                // and +0xAA.
                s16 zero = 0;
                s16 c0hi[2] = {zero, *(s16*)((u8*)info + 0xA2)};
                s16 c0lo[2] = {zero, zero};
                s16 c1hi[2] = {zero, *(s16*)((u8*)info + 0xAA)};
                s16 c1lo[2] = {zero, zero};
                u32 args[4];
                args[0] = *(u32*)c0lo;
                args[1] = *(u32*)c0hi;
                args[2] = *(u32*)c1lo;
                args[3] = *(u32*)c1hi;
                sprintf(buf, (char*)&lbl_eu_805063BC[0x161], idx, args[0], args[1], args[2], args[3]);
                nw4r::lyt::Pane* pane3 = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
                void* vertexObj = pane3->GetMaterial();
                for (u32 j = 0; j < 2; j++) {
                    func_801D62F8(vertexObj, (u8)j, (u8*)args + (u8)j * 8);
                }
            }
        }
    }
}
#pragma pop
// Activates the panel: once both the layout (+0x34) and its resource
// accessor (+0x30) are present, set the current/active bytes. Goto-gate
// layout matches retail's branch-over-branch shape (bne over a blr with
// the body placed after the return - see CTitle func_802B64AC family).
void func_801E4390(CItemBoxInfo2* info) {
    if (info->state.layout != 0 && info->state.resource != 0) {
        goto body;
    }
    return;
body:
    info->state.current = 1;
    info->state.active = 1;
}
// ============================================================================
// func_801E43BC - ItemBox2 info-panel render (retail 0x801E5FB8, 0x4DA8 bytes)
// Reconstructed from retail disassembly (build/us/asm/kyoshin/CItemBoxInfo.s).
// NOTE: arg3 (r5) is dead in retail - clobbered by the prologue (lis r5, 0x4330).
// ============================================================================
// TU-local stat-object shapes (retail lha/lfs/lbz offsets from func_801E43BC).
struct E43Quad {           // POD quad color (copy-init must stay memberwise)
    s16 r, g, b, a;
};
struct E43Entry {          // 0x34-byte item-stat entry (func_801E197C/801E1E0C output)
    u16 w00;
    u16 _02;
    u32 w04;                       // +0x04 value (u16 pairs read via lhz at +4/+6)
    u8 _08[4];
    u32 _0C;
    u32 _10;
    s16 w14;                       // +0x14 (delta-compared)
    s16 _16;
    u8 _18[0x34 - 0x18];
};
struct E43StatC {          // vtable[0x22C] result - s16 stat block
    u8 _00[6];
    s16 s06;
    u8 _08[4];
    s16 s0C;
    s16 s0E;
    s16 s10;
    u8 _12[6];
    s16 s18;
    u8 _1A[2];
    s16 s1C;
    u8 _1E[4];
    s16 s22;
};
struct E43StatA {          // vtable[0x228] result - s16 stat block + f32
    u8 _00[0x10];
    f32 f10;
    u8 _14[8];
    s16 s1C;
    s16 s1E;
    s16 s20;
    u8 _22[0xC];
    s16 s2E;
    u8 _30[2];
    s16 s32;
    u8 _34[4];
    s16 s38;
    u8 _3A[0x1B];
    u8 b55;
};
struct E43StatB {          // vtable[0x20C] result - f32 block
    u8 _00[0x10];
    f32 f10;
};

typedef void (*E43VF)(void*, u32);
struct ItemBoxInfoCopy {
    unsigned short itemId;
    unsigned short _02;
    unsigned int value;
    unsigned char state;
};
void copyItemBoxCopy(ItemBoxInfoCopy* dst, const ItemBoxInfoCopy* src) {
    dst->itemId = src->itemId;
    dst->value = src->value;
    dst->state = src->state;
}

void setItemBoxCopy(ItemBoxInfoCopy* self, unsigned short a, unsigned int b, unsigned char c) {
    *(unsigned short*)self = a;
    *(unsigned int*)((u8*)self + 4) = b;
    *(unsigned char*)((u8*)self + 8) = c;
}
extern const u32 lbl_eu_8066806C;
extern const u8 lbl_eu_80668070;

extern "C" void func_801E43BC(CItemBoxInfo2* info, u16 arg2, void* arg3, u16 arg4, u32 arg5) {
    // ---- type (main switch key) ----
    u8 type = (u8)func_801392E4((u32)arg2);

    // ---- party-slot ping: 12-word copy of party struct + 2x3 vtable[0xA4] ----
    struct PartyData { u32 w[12]; };
    PartyData partyData = *(PartyData*)((u8*)func_8009ECB0() + 4);
    for (u32 row = 0; row < 2; row++) {
        for (u32 col = 0; col < 3; col++) {
            u8 id = (u8)partyData.w[col];
            if (id != 0) {
                void* actor = func_800B8B94(id);
                if (actor != NULL) {
                    ((E43VF*)(*(void***)actor))[0x29](actor, 0);
                }
            }
        }
    }

    // ---- character setup ----
    u8 member = (u8)func_801392B4(arg4);
    void* charObj = func_8009EC9C(member);
    if (func_800B8B94(member) == NULL) {
        func_800A13C4(charObj, 1);
    }
    void* stats = (u8*)charObj + 0x17C;
    void** vt = *(void***)stats;

    // ---- HP values (clamped to 9999) ----
    s32 hp1 = (s32)((float(*)(void*))vt[0x4A])(stats);
    if (hp1 > 9999) hp1 = 9999;
    s32 hp2 = (s32)((float(*)(void*))vt[0x4B])(stats);
    if (hp2 > 9999) hp2 = 9999;

    // ---- name / pane text ----
    func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), &lbl_eu_805063BC[0x4D7], func_8013639C(lbl_eu_806640D8, &lbl_eu_805063BC[0x139]), 0);
    func_80136910((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), &lbl_eu_805063BC[0x4E3], ((u8(*)(void*))vt[0x42])(stats));
    func_80136910((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), &lbl_eu_805063BC[0x4F0], ((u8(*)(void*))vt[0x79])(stats));
    func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], hp1);
    func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], hp2);
    func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[3], func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x82), 0);

    // ---- stat sub-objects ----
    E43StatA* stA = (E43StatA*)((void*(*)(void*))vt[0x8A])(stats);
    E43StatB* stB = (E43StatB*)((void*(*)(void*))vt[0x83])(stats);
    E43StatC* stC = (E43StatC*)((void*(*)(void*))vt[0x8B])(stats);
    func_8009D7E4((u8*)charObj + 0x1C, 5);

    // ---- arts sum ----
    s32 artsSum = (s32)func_801E9774(info, member, NULL);
    u32 w = func_801E9690(info, member, 0x97);
    if (w != 0) {
        artsSum = artsSum - (s32)w;
        if (artsSum < 0) artsSum = 0;
    }

    // ---- stat bars (display formula) ----
    s16 bar1 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10) * (f32)(stA->s20 + func_801E9310(info, (void*)(u32)member, 0x21, NULL))));
    s16 bar2 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C) * (f32)(stA->s1C + func_801E9310(info, (void*)(u32)member, 0x31, NULL))));
    s16 bar3 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)(stA->s32 + func_801E9310(info, (void*)(u32)member, 0x41, NULL))));
    s16 bar4 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E) * (f32)(stA->s1E + func_801E9310(info, (void*)(u32)member, 0x51, NULL) - artsSum)));
    s16 bar5 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + func_801E9310(info, (void*)(u32)member, 0x54, NULL))));
    s16 bar6 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)(stA->s38 + func_801E9310(info, (void*)(u32)member, 0x11, NULL))));
    func_801C6158(0.01f * stB->f10 * (f32)(stC->s06 + func_801E9310(info, (void*)(u32)member, 0x11, NULL)));

    // ---- equip-mode gate: recompute bars with the equipped stat (0x30) ----
    if (func_801E98E4(info, member, NULL)) {
        s32 eq = func_801E9690(info, member, 0x30);
        if (eq != 0) {
            s16 barA = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)eq) * (f32)(stA->s20 + func_801E9310(info, (void*)(u32)member, 0x21, NULL))));
            s16 barB = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)barA) * (f32)(stA->s1C + func_801E9310(info, (void*)(u32)member, 0x1, NULL))));
            s16 barC = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)eq) * (f32)(stA->s1E + func_801E9310(info, (void*)(u32)member, 0x41, NULL) - artsSum)));
            func_801C6158(0.01f * stB->f10 * (f32)(stC->s06 + eq + func_801E9310(info, (void*)(u32)member, 0x11, NULL)));

            // ---- current weapon block ----
            void* item2 = func_80157C4C(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                ItemBoxInfoCopy e1;
                func_801E197C(&e1, info, (void*)(u32)((u16)((*(u32*)item2 >> 20) & 0xFFFF)));
                ItemBoxInfoCopy e2 = e1;  // 7-word copy
                s32 s1 = func_801E9190(info, (void*)(u32)member, e2.value & 0xFFFF, NULL);
                s32 s2 = func_801E9224(info, (void*)(u32)member, *(u16*)((u8*)&e2 + 6), NULL);
                s32 atk1 = (s32)barB + s1;
                s32 atk2 = (s32)barB + s2;
                if (atk1 > atk2) atk1 = atk2;
                ml::FixStr<32> fmtBuf;
                fmtBuf.format(&lbl_eu_805063BC[0x254], atk1, func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB), atk2);
                func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], fmtBuf.c_str(), 0);
            }

            // ---- numbers ----
            if (barC <= 0) barC = 1;
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], (s16)barB);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], (s16)barC);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], (s16)barA);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], (s16)bar5);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], (s16)bar3);
            ml::FixStr<32> fmtBuf2;
            fmtBuf2.format(&lbl_eu_805063BC[0x13E], (s16)bar6, func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80));
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], fmtBuf2.c_str(), 0);
            ml::FixStr<32> fmtBuf3;
            fmtBuf3.format(&lbl_eu_805063BC[0x13E], (s16)(stA->b55), func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80));
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], fmtBuf3.c_str(), 0);

            // ---- 20x color application ----
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[3], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[1], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[5], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[7], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[9], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[11], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[13], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[15], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[17], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[19], &q1, &q2);
        }
    }

    // ---- stat panel (arg5 != 0, type in {2,4,5,6,7,8}) ----
    if (arg5 != 0 && (type == 2 || (type >= 4 && type <= 8))) {
        s32 e1 = 0;
        if (func_801E98E4(info, member, NULL)) e1 = func_801E9690(info, member, 0x30);
        s32 e2 = 0;
        if (func_801E98E4(info, member, arg3)) e2 = func_801E9690(info, member, 0x30);

        // row A: 0x21 -> panes 0x60/0x64
        s16 dB = 0;
        {
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)e1) * (f32)(stA->s20 + func_801E9310(info, (void*)(u32)member, 0x21, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)e2) * (f32)(stA->s20 + func_801E9310(info, (void*)(u32)member, 0x21, arg3))));
            s16 d = (s16)(nb - pb);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], nb);
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            if (d < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (d > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[9], &q1, &q2);
        }

        // row B: 0x1 -> panes 0x50/0x54
        {
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)e1) * (f32)(stA->s1C + func_801E9310(info, (void*)(u32)member, 0x1, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)e2) * (f32)(stA->s1C + func_801E9310(info, (void*)(u32)member, 0x1, arg3))));
            dB = (s16)(nb - pb);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], nb);
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            if (dB < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dB > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[5], &q1, &q2);
        }

        // row C: 0x41 -> panes 0x58/0x5C
        {
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)e1) * (f32)(dB + func_801E9310(info, (void*)(u32)member, 0x41, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)e2) * (f32)(dB + func_801E9310(info, (void*)(u32)member, 0x41, arg3))));
            s16 dC = (s16)(nb - pb);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], nb);
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            if (dC < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dC > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[7], &q1, &q2);
        }

        // row D: 0x11 + func_801C6158 -> panes 0x40/0x48
        {
            s16 pb = (s16)(s32)(stA->f10 + (f32)func_801C6158(0.01f * stB->f10 * (f32)(stC->s06 + e1 + func_801E9310(info, (void*)(u32)member, 0x11, NULL))));
            s16 nb = (s16)(s32)(stA->f10 + (f32)func_801C6158(0.01f * stB->f10 * (f32)(stC->s06 + e2 + func_801E9310(info, (void*)(u32)member, 0x11, arg3))));
            if (pb > 9999) pb = 9999;
            if (nb > 9999) nb = 9999;
            s16 dD = (s16)(nb - pb);
            if (((float(*)(void*))vt[0x4A])(stats) > (f32)nb) {
                func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], nb);
            }
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], nb);
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            if (dD < 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            } else if (dD > 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            }
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[3], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[1], &q1, &q2);
        }
        // ---- type switch: slot id + item ----
        s16 slotId = 0;
        switch (type) {
            case 2: slotId = *(s16*)((u8*)charObj + 0x26); break;
            case 4: slotId = *(s16*)((u8*)charObj + 0x1C); break;
            case 5: slotId = *(s16*)((u8*)charObj + 0x1E); break;
            case 6: slotId = *(s16*)((u8*)charObj + 0x20); break;
            case 7: slotId = *(s16*)((u8*)charObj + 0x22); break;
            case 8: slotId = *(s16*)((u8*)charObj + 0x24); break;
        }
        void* item = func_80157C4C(type, slotId);
        if (type == 2) {
            // ---- weapon block (0x801E7300) ----
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            E43Entry e_cur;
            func_801E197C(&e_cur, info, (void*)(u32)w0);
            E43Entry c_cur = e_cur;
            E43Entry e_new;
            func_801E197C(&e_new, info, arg3);
            E43Entry c_new = e_new;
            s32 eq1 = 0;
            if (func_801E98E4(info, member, NULL)) eq1 = func_801E9690(info, member, 0x30);
            s32 eq2 = 0;
            if (func_801E98E4(info, member, arg3)) eq2 = func_801E9690(info, member, 0x30);
            s16 pb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq1) * (f32)(stA->s1C + func_801E9310(info, (void*)(u32)member, 0x1, NULL))));
            s16 nb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + func_801E9310(info, (void*)(u32)member, 0x1, NULL))));
            s32 atkA = (s32)pb + func_801E9190(info, (void*)(u32)member, (u16)c_cur.w04, NULL);
            s32 atkB = (s32)nb + func_801E9190(info, (void*)(u32)member, (u16)c_new.w04, arg3);
            s32 atkC = (s32)atkA + func_801E9224(info, (void*)(u32)member, (u16)(c_cur.w04 >> 16), NULL);
            s32 atkD = atkB < atkC ? atkB : atkC;
            s16 dW = (s16)(atkC - atkA);
            s16 v610, v612, v614, v644, v646, v648;
            s32 c1 = func_801E9310(info, (void*)(u32)member, 0x31, arg3);
            v612 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)((stA->s32 - v646) + (c1 + v612))));
            c1 = func_801E9310(info, (void*)(u32)member, 0x51, arg3);
            v646 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)(stA->s32 + c1)));
            c1 = func_801E9310(info, (void*)(u32)member, 0x51, NULL);
            v610 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)((stA->s2E - v644) + (c1 + v610))));
            c1 = func_801E9310(info, (void*)(u32)member, 0x54, arg3);
            v644 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            c1 = func_801E9310(info, (void*)(u32)member, 0x54, NULL);
            v614 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)((stA->s38 - v648) + (c1 + v614))));
            c1 = func_801E9310(info, (void*)(u32)member, 0x54, NULL);
            v648 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s22) * (f32)(stA->s38 + c1)));
            s16 snap644 = v644;
            s16 snap646 = v646;
            s16 d1 = (s16)(v610 - snap644);
            s16 d2 = (s16)(v612 - snap646);
            s16 d3 = (s16)(c_new.w14 - c_cur.w14);
            s16 d4 = (s16)(v614 - v648);
            if (arg5 == 0) {
                dW = 0;
                d1 = 0;
                d2 = 0;
                d3 = 0;
                d4 = 0;
            }
            ml::FixStr<32> fmtBuf;
            fmtBuf.format(&lbl_eu_805063BC[0x254], atkD, func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB), atkC);
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], fmtBuf.c_str(), 0);
            s16 disp1 = v610;
            s16 disp2 = v612;
            s16 disp3 = v614;
            if (arg5 == 0) {
                disp1 = snap644;
                disp2 = snap646;
                disp3 = v648;
            }
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], (s16)disp1);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], (s16)disp2);
            ml::FixStr<32> fmtBuf2;
            fmtBuf2.format(&lbl_eu_805063BC[0x13E], (s16)disp3, func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80));
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], fmtBuf2.c_str(), 0);
            ml::FixStr<32> fmtBuf3;
            fmtBuf3.format(&lbl_eu_805063BC[0x13E], (s32)stA->b55 + (s32)d3, func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80));
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], fmtBuf3.c_str(), 0);
            // delta colors (5 rows)
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[11], &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[13], &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[15], &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d4 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d4 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[17], &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d3 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d3 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[19], &q1, &q2);
            }
        } else {
            // ---- armor block (0x801E7DA0) ----
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            E43Entry e_cur;
            func_801E1E0C((CItemBoxSlotFlags*)&e_cur, info, (void*)(u32)w0);
            E43Entry c_cur = e_cur;
            E43Entry e_new;
            func_801E1E0C((CItemBoxSlotFlags*)&e_new, info, arg3);
            E43Entry c_new = e_new;
            s32 eq1 = 0;
            if (func_801E98E4(info, member, NULL)) eq1 = func_801E9690(info, member, 0x30);
            s32 eq2 = 0;
            if (func_801E98E4(info, member, arg3)) eq2 = func_801E9690(info, member, 0x30);
            u16 v484 = (u16)func_801E9774(info, member, (void*)(u32)w0);
            u16 v468 = (u16)func_801E9774(info, member, arg3);
            // arm-type scaling of the entry value pairs
            u8 arm1 = (u8)func_801361E8((u32)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], func_80139358((u32)w0));
            if (arm1 == 3) {
                s32 v = func_801E9690(info, member, 0xD);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
                v = func_801E9690(info, member, 0x24);
                if (v != 0) {
                    c_cur.w14 = (s16)((s32)c_cur.w14 + (s16)v);
                }
            } else if (arm1 == 2) {
                s32 v = func_801E9690(info, member, 0xC);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
            } else if (arm1 == 1) {
                s32 v = func_801E9690(info, member, 0xB);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
            }
            u8 arm2 = (u8)func_801361E8((u32)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], func_80139358((u32)(*(u32*)arg3 >> 20)));
            if (arm2 == 3) {
                s32 v = func_801E9690(info, member, 0xD);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
                v = func_801E9690(info, member, 0x24);
                if (v != 0) {
                    c_new.w14 = (s16)((s32)c_new.w14 + (s16)v);
                }
            } else if (arm2 == 2) {
                s32 v = func_801E9690(info, member, 0xC);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
            } else if (arm2 == 1) {
                s32 v = func_801E9690(info, member, 0xB);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
            }
            // weight
            s32 wv = func_801E9690(info, member, 0x97);
            if (wv != 0) {
                s32 t1 = (s32)v484 - wv;
                if (t1 < 0) t1 = 0;
                v484 = (u16)t1;
                s32 t2 = (s32)v468 - wv;
                if (t2 < 0) t2 = 0;
                v468 = (u16)t2;
            }
            // slot sum
            s32 ssum = (s32)*(s16*)((u8*)charObj + 0x22) + *(s16*)((u8*)charObj + 0x20) + *(s16*)((u8*)charObj + 0x24) + *(s16*)((u8*)charObj + 0x1C) + *(s16*)((u8*)charObj + 0x1E);
            if (ssum <= -5) {
                func_801E9690(info, member, 0x1A);
            }
            // bars (armor stat pair updates)
            s32 c1 = func_801E9310(info, (void*)(u32)member, 0x31, arg3);
            c_new.w14 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)((stA->s32 - (s16)(u16)c_cur.w04) + (c1 + c_new.w14))));
            c1 = func_801E9310(info, (void*)(u32)member, 0x51, NULL);
            c_new.w04 = (u32)(s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)((stA->s2E - (s16)(u16)c_cur.w04) + (c1 + (s16)(u16)c_new.w04))));
            c1 = func_801E9310(info, (void*)(u32)member, 0x41, NULL);
            c_cur.w04 = (u32)(s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            s16 d1 = (s16)(c_new.w14 - (s16)(u16)c_cur.w14);
            s16 d2 = (s16)((s16)(u16)c_new.w04 - (s16)(u16)c_cur.w04);
            s32 c2 = func_801E9310(info, (void*)(u32)member, 0x41, arg3);
            s16 b27 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)eq1) * (f32)((stA->s1E - (s16)v484) + c2)));
            c2 = func_801E9310(info, (void*)(u32)member, 0x41, NULL);
            s16 b14 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)eq2) * (f32)((stA->s1E - (s16)v468) + c2)));
            if (b27 <= 0) b27 = 1;
            if (b14 <= 0) b14 = 1;
            s16 dW = (s16)(b14 - b27);
            // embedded weapon block
            void* item2 = func_80157C4C(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                E43Entry ew;
                func_801E197C(&ew, info, (void*)(u32)(u16)(*(u32*)item2 >> 20));
                E43Entry cw = ew;
                s32 c3 = func_801E9310(info, (void*)(u32)member, 0x1, NULL);
                s16 wpb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq1) * (f32)(stA->s1C + c3)));
                c3 = func_801E9310(info, (void*)(u32)member, 0x1, NULL);
                s16 wnb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + c3)));
                s32 wa = (s32)wpb + func_801E9190(info, (void*)(u32)member, (u16)cw.w04, NULL);
                s32 wb = (s32)wnb + func_801E9224(info, (void*)(u32)member, (u16)(cw.w04 >> 16), NULL);
                s32 wc = (s32)wnb + func_801E9190(info, (void*)(u32)member, (u16)cw.w04, arg3);
                s32 wd = (s32)wnb + func_801E9224(info, (void*)(u32)member, (u16)(cw.w04 >> 16), arg3);
                s32 wmin = wc < wd ? wc : wd;
                dW = (s16)(wd - wa);
                ml::FixStr<32> fmtBuf;
                fmtBuf.format(&lbl_eu_805063BC[0x254], wmin, func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB), wd);
                func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], fmtBuf.c_str(), 0);
            }
            // numbers
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], (s16)(u16)c_new.w04);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], (s16)c_new.w14);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], (s16)b14);
            // delta colors (4 rows)
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[11], &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d2 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d2 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[13], &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (d1 < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (d1 > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[15], &q1, &q2);
            }
            {
                E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
                E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
                E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
                E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
                if (dW < 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
                } else if (dW > 0) {
                    __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                    __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                    __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                    __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
                }
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &q3, &q4);
                func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[7], &q1, &q2);
            }
        }

    }

    // ---- equip-detail / simple path (arg5 == 0, type in {2,4,5,6,7,8}) ----
    if (arg5 == 0 && (type == 2 || (type >= 4 && type <= 8))) {
        if (type != 2 && func_801E96F0(info, member, (u32)arg3)) {
            // ---- equip detail (0x801E8C88) ----
            s16 slotId = 0;
            switch (type) {
                case 4: slotId = *(s16*)((u8*)charObj + 0x1C); break;
                case 5: slotId = *(s16*)((u8*)charObj + 0x1E); break;
                case 6: slotId = *(s16*)((u8*)charObj + 0x20); break;
                case 7: slotId = *(s16*)((u8*)charObj + 0x22); break;
                case 8: slotId = *(s16*)((u8*)charObj + 0x24); break;
            }
            void* item = func_80157C4C(type, slotId);
            u16 w0 = (item != NULL && *(u32*)item != 0) ? (u16)(*(u32*)item >> 20) : 0;
            E43Entry e_cur;
            func_801E1E0C((CItemBoxSlotFlags*)&e_cur, info, (void*)(u32)w0);
            E43Entry c_cur = e_cur;
            E43Entry e_new;
            func_801E1E0C((CItemBoxSlotFlags*)&e_new, info, arg3);
            E43Entry c_new = e_new;
            u16 v44C = (u16)func_801E9774(info, member, (void*)(u32)w0);
            u16 v430 = (u16)func_801E9774(info, member, arg3);
            s32 eq1 = 0;
            if (func_801E98E4(info, member, NULL)) eq1 = func_801E9690(info, member, 0x30);
            s32 eq2 = 0;
            if (func_801E98E4(info, member, arg3)) eq2 = func_801E9690(info, member, 0x30);
            func_801E9310(info, (void*)(u32)member, 0x21, NULL);
            s16 bA = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s10 + (f32)eq2) * (f32)(stA->s20 + func_801E9310(info, (void*)(u32)member, 0x21, arg3))));
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], (s16)bA);
            func_801E9310(info, (void*)(u32)member, 0x1, NULL);
            s16 bB = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + func_801E9310(info, (void*)(u32)member, 0x1, arg3))));
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], (s16)bB);
            func_801C6158(0.01f * stB->f10 * (f32)(stC->s06 + eq1 + func_801E9310(info, (void*)(u32)member, 0x11, NULL)));
            s32 r6158 = func_801C6158(0.01f * stB->f10 * (f32)(stC->s06 + eq2 + func_801E9310(info, (void*)(u32)member, 0x11, arg3)));
            s16 nb = (s16)(s32)(stA->f10 + (f32)r6158);
            if (nb > 9999) nb = 9999;
            if (((float(*)(void*))vt[0x4A])(stats) > (f32)nb) {
                func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], nb);
            }
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], nb);
            // arm-type scaling of entry value pairs
            u8 arm1 = (u8)func_801361E8((u32)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], func_80139358((u32)w0));
            if (arm1 == 3) {
                s32 v = func_801E9690(info, member, 0xD);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
                v = func_801E9690(info, member, 0x24);
                if (v != 0) {
                    c_cur.w14 = (s16)((s32)c_cur.w14 + (s16)v);
                }
            } else if (arm1 == 2) {
                s32 v = func_801E9690(info, member, 0xC);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
            } else if (arm1 == 1) {
                s32 v = func_801E9690(info, member, 0xB);
                if (v != 0) {
                    c_cur.w04 = (u32)(s32)((f32)(s32)c_cur.w04 * (0.01f * (100.0f + (f32)v)));
                    c_cur.w14 = (s16)(s32)((f32)c_cur.w14 * (0.01f * (100.0f + (f32)v)));
                }
            }
            u8 arm2 = (u8)func_801361E8((u32)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], func_80139358((u32)(*(u32*)arg3 >> 20)));
            if (arm2 == 3) {
                s32 v = func_801E9690(info, member, 0xD);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
                v = func_801E9690(info, member, 0x24);
                if (v != 0) {
                    c_new.w14 = (s16)((s32)c_new.w14 + (s16)v);
                }
            } else if (arm2 == 2) {
                s32 v = func_801E9690(info, member, 0xC);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
            } else if (arm2 == 1) {
                s32 v = func_801E9690(info, member, 0xB);
                if (v != 0) {
                    c_new.w04 = (u32)(s32)((f32)(s32)c_new.w04 * (0.01f * (100.0f + (f32)v)));
                    c_new.w14 = (s16)(s32)((f32)c_new.w14 * (0.01f * (100.0f + (f32)v)));
                }
            }
            // weight
            s32 wv = func_801E9690(info, member, 0x97);
            if (wv != 0) {
                s32 t1 = (s32)v44C - wv;
                if (t1 < 0) t1 = 0;
                v44C = (u16)t1;
                s32 t2 = (s32)v430 - wv;
                if (t2 < 0) t2 = 0;
                v430 = (u16)t2;
            }
            // slot sum
            s32 ssum = (s32)*(s16*)((u8*)charObj + 0x22) + *(s16*)((u8*)charObj + 0x20) + *(s16*)((u8*)charObj + 0x24) + *(s16*)((u8*)charObj + 0x1C) + *(s16*)((u8*)charObj + 0x1E);
            if (ssum <= -5) {
                func_801E9690(info, member, 0x1A);
            }
            // bars (armor stat pair updates)
            s32 c1 = func_801E9310(info, (void*)(u32)member, 0x31, arg3);
            c_new.w14 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s1C) * (f32)((stA->s32 - (s16)(u16)c_cur.w04) + (c1 + c_new.w14))));
            c1 = func_801E9310(info, (void*)(u32)member, 0x51, arg3);
            c_new.w04 = (u32)(s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            c1 = func_801E9310(info, (void*)(u32)member, 0x51, NULL);
            c_new.w04 = (u32)(s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)((stA->s2E - (s16)(u16)c_cur.w04) + (c1 + (s16)(u16)c_new.w04))));
            c1 = func_801E9310(info, (void*)(u32)member, 0x41, NULL);
            c_cur.w04 = (u32)(s16)(s32)(0.01f * ((100.0f + (f32)stC->s18) * (f32)(stA->s2E + c1)));
            s32 c2 = func_801E9310(info, (void*)(u32)member, 0x41, arg3);
            s16 b14 = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0E + (f32)eq2) * (f32)((stA->s1E - (s16)v430) + c2)));
            if (b14 <= 0) b14 = 1;
            // embedded weapon block
            void* item2 = func_80157C4C(2, *(s16*)((u8*)charObj + 0x26));
            if (item2 != NULL && *(u32*)item2 != 0) {
                E43Entry ew;
                func_801E197C(&ew, info, (void*)(u32)(u16)(*(u32*)item2 >> 20));
                E43Entry cw = ew;
                func_801E9310(info, (void*)(u32)member, 0x1, NULL);
                s16 wpb = (s16)(s32)(0.01f * ((100.0f + (f32)stC->s0C + (f32)eq2) * (f32)(stA->s1C + func_801E9310(info, (void*)(u32)member, 0x1, arg3))));
                s32 wa = (s32)wpb + func_801E9190(info, (void*)(u32)member, (u16)cw.w04, NULL);
                s32 wb = (s32)wpb + func_801E9224(info, (void*)(u32)member, (u16)(cw.w04 >> 16), NULL);
                s32 wc = (s32)wpb + func_801E9190(info, (void*)(u32)member, (u16)cw.w04, arg3);
                s32 wd = (s32)wpb + func_801E9224(info, (void*)(u32)member, (u16)(cw.w04 >> 16), arg3);
                s32 wmin = wc < wd ? wc : wd;
                ml::FixStr<32> fmtBuf;
                fmtBuf.format(&lbl_eu_805063BC[0x254], wmin, func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0xB), wd);
                func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], fmtBuf.c_str(), 0);
            }
            // numbers
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], (s16)(u16)c_new.w04);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], (s16)c_new.w14);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], (s16)b14);
            // 20x color application (selected palette A8/B0/C8/D0)
            E43Quad q1 = *(E43Quad*)&lbl_eu_806645A8;
            E43Quad q2 = *(E43Quad*)&lbl_eu_806645B0;
            E43Quad q3 = *(E43Quad*)&lbl_eu_806645C8;
            E43Quad q4 = *(E43Quad*)&lbl_eu_806645D0;
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[3], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[1], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[5], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[7], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[9], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[11], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[13], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[15], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[17], &q1, &q2);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], &q3, &q4);
            func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[19], &q1, &q2);

        }
        // ---- simple path ----
        E43Quad q1 = *(E43Quad*)&lbl_eu_806645A8;
        E43Quad q2 = *(E43Quad*)&lbl_eu_806645B0;
        E43Quad q3 = *(E43Quad*)&lbl_eu_806645C8;
        E43Quad q4 = *(E43Quad*)&lbl_eu_806645D0;
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[0], &lbl_eu_805063BC[0x2AA], 0);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[2], &lbl_eu_805063BC[0x2AA], 0);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[3], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[1], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[5], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[7], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[9], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[11], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[13], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[15], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[17], &q1, &q2);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], &lbl_eu_805063BC[0x2AA], 0);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], &q3, &q4);
        func_80139AC8(((nw4r::lyt::Pane**)((u8*)info + 0x40))[19], &q1, &q2);
    }

    // ---- common tail: two 8-entry item lists ----
    ItemBoxInfoCopy listA[8];
    ItemBoxInfoCopy listB[8];
    setItemBoxCopy(&listA[0], 0, 0, 0);
    setItemBoxCopy(&listA[1], 0, 0, 0);
    setItemBoxCopy(&listA[2], 0, 0, 0);
    setItemBoxCopy(&listA[3], 0, 0, 0);
    setItemBoxCopy(&listA[4], 0, 0, 0);
    setItemBoxCopy(&listA[5], 0, 0, 0);
    setItemBoxCopy(&listA[6], 0, 0, 0);
    setItemBoxCopy(&listA[7], 0, 0, 0);
    setItemBoxCopy(&listB[0], 0, 0, 0);
    setItemBoxCopy(&listB[1], 0, 0, 0);
    setItemBoxCopy(&listB[2], 0, 0, 0);
    setItemBoxCopy(&listB[3], 0, 0, 0);
    setItemBoxCopy(&listB[4], 0, 0, 0);
    setItemBoxCopy(&listB[5], 0, 0, 0);
    setItemBoxCopy(&listB[6], 0, 0, 0);
    setItemBoxCopy(&listB[7], 0, 0, 0);

    u8 listIdxA = 0;
    u8 listIdxB = 0;

    // weapon slot
    {
        void* itemW = func_80157C4C(2, *(s16*)((u8*)charObj + 0x26));
        void* items[5];
        items[0] = func_80157C4C(4, *(s16*)((u8*)charObj + 0x1C));
        items[1] = func_80157C4C(5, *(s16*)((u8*)charObj + 0x1E));
        items[2] = func_80157C4C(6, *(s16*)((u8*)charObj + 0x20));
        items[3] = func_80157C4C(7, *(s16*)((u8*)charObj + 0x22));
        items[4] = func_80157C4C(8, *(s16*)((u8*)charObj + 0x24));
        if (itemW != NULL) {
            u8 cnt = (u8)((u32(*)(void*))((void**)itemW)[0x0C])(itemW);
            for (u8 i = 0; i < 3; i++) {
                ItemBoxInfoCopy tmp;
                tmp.itemId = 0;
                tmp.value = 0;
                tmp.state = 0;
                if (i < cnt) {
                    void* e = ((void*(*)(void*, u32))((void**)itemW)[0x0B])(itemW, i);
                    if (e != NULL) {
                        u32 w0 = *(u32*)e;
                        u16 w1 = *(u16*)((u8*)e + 4);
                        ItemBoxInfoCopy tmp2;
                        setItemBoxCopy(&tmp2, (s16)((w0 >> 11) & 0x7FF), (w0 >> 22) & 7, (w1 >> 16) & 0xFFF);
                        copyItemBoxCopy(&tmp, &tmp2);
                    }
                }
                copyItemBoxCopy(&listA[listIdxA++], &tmp);
                if (type != 2) {
                    copyItemBoxCopy(&listB[listIdxB++], &tmp);
                }
            }
        }
        // crystal list (type == 2 && arg5 != 0)
        if (type == 2 && arg5 != 0) {
            u8 cnt = (u8)func_801361E8((u32)lbl_eu_806640F4, &lbl_eu_805063BC[0x432], func_80139358(*(u32*)arg3 >> 20));
            for (u8 i = 0; i < 3; i++) {
                ItemBoxInfoCopy tmp;
                tmp.itemId = 0;
                tmp.value = 0;
                tmp.state = 0;
                if (i < cnt) {
                    char buf[0x20];
                    sprintf(buf, &lbl_eu_805063BC[0x43B], i + 1);
                    u32 id = func_80136254(lbl_eu_806640F4, buf, func_80139358(*(u32*)arg3 >> 20));
                    u16 nameId = (u16)func_80136254(lbl_eu_806640EC, &lbl_eu_805063BC[0x814], id);
                    u8 icon = (u8)func_801361E8((u32)lbl_eu_806640EC, &lbl_eu_805063BC[0x447], id);
                    u8 val = (u8)func_801361E8((u32)lbl_eu_806640EC, &lbl_eu_805063BC[0x1F9], id);
                    if (id != 0) {
                        ItemBoxInfoCopy tmp2;
                        tmp2.itemId = nameId;
                        tmp2.value = icon;
                        tmp2.state = val;
                        copyItemBoxCopy(&tmp, &tmp2);
                    }
                }
                copyItemBoxCopy(&listB[listIdxB++], &tmp);
            }
        }
        // blank entries (type == 2 && arg5 == 0)
        if (type == 2 && arg5 == 0) {
            ItemBoxInfoCopy tmp;
            tmp.itemId = 0;
            tmp.value = 0;
            tmp.state = 0;
            copyItemBoxCopy(&listB[listIdxB++], &tmp);
            copyItemBoxCopy(&listB[listIdxB++], &tmp);
            copyItemBoxCopy(&listB[listIdxB++], &tmp);
        }
        // armor slots
        {
            u8 slotTypes[5];
            *(u32*)&slotTypes[0] = lbl_eu_8066806C;
            slotTypes[4] = lbl_eu_80668070;
            for (u8 i = 0; i < 5; i++) {
                ItemBoxInfoCopy tmp;
                tmp.itemId = 0;
                tmp.value = 0;
                tmp.state = 0;
                void* item = items[i];
                if (item != NULL) {
                    if ((u8)((u32(*)(void*))((void**)item)[0x0C])(item) != 0) {
                        void* e = ((void*(*)(void*, u32))((void**)item)[0x0B])(item, 0);
                        if (e != NULL) {
                            u32 w0 = *(u32*)e;
                            u16 w1 = *(u16*)((u8*)e + 4);
                            ItemBoxInfoCopy tmp2;
                            setItemBoxCopy(&tmp2, (s16)((w0 >> 11) & 0x7FF), (w0 >> 22) & 7, (w1 >> 16) & 0xFFF);
                            copyItemBoxCopy(&tmp, &tmp2);
                        }
                    }
                }
                copyItemBoxCopy(&listA[listIdxA++], &tmp);
                if (type != slotTypes[i]) {
                    copyItemBoxCopy(&listB[listIdxB++], &tmp);
                }
                if (type == slotTypes[i]) {
                    ItemBoxInfoCopy tmp3;
                    tmp3.itemId = 0;
                    tmp3.value = 0;
                    tmp3.state = 0;
                    if (arg5 != 0) {
                        if (func_801361E8((u32)lbl_eu_806640F8, &lbl_eu_805063BC[0x432], func_80139358(*(u32*)arg3 >> 20)) != 0) {
                            u16 nameId = (u16)func_80136254(lbl_eu_806640F8, &lbl_eu_805063BC[0x81B], func_80139358(*(u32*)arg3 >> 20));
                            u16 capId = (u16)func_80136254(lbl_eu_806640EC, &lbl_eu_805063BC[0x814], nameId);
                            u8 icon = (u8)func_801361E8((u32)lbl_eu_806640EC, &lbl_eu_805063BC[0x447], nameId);
                            u8 val = (u8)func_801361E8((u32)lbl_eu_806640EC, &lbl_eu_805063BC[0x1F9], nameId);
                            if (nameId != 0) {
                                tmp3.itemId = capId;
                                tmp3.value = icon;
                                tmp3.state = val;
                            }
                        }
                    }
                    copyItemBoxCopy(&listB[listIdxB++], &tmp3);
                }
            }
        }
    }

    // ---- compact listA (dedupe by stackable flag, sum values, cap) ----
    {
        u16 ids[8];
        u32 vals[8];
        u8 flags[8];
        u8 n = 0;
        for (u8 i = 0; i < 8; i++) {
            u16 id = listA[i].itemId;
            u32 val = listA[i].value;
            u8 flag = listA[i].state;
            if (id == 0) {
                ids[n] = id;
                vals[n] = val;
                flags[n] = flag;
                n++;
            } else if (func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], id) != 0) {
                u8 found = 0;
                for (u8 j = 0; j < n; j++) {
                    if (id == ids[j]) {
                        vals[j] += val;
                        for (u8 k = j + 1; k < n; k++) {
                            if (id == ids[k]) vals[k] = vals[j];
                        }
                        found = 1;
                        ids[n] = id;
                        vals[n] = vals[j];
                        flags[n] = flag;
                        n++;
                        break;
                    }
                }
                if (!found) {
                    ids[n] = id;
                    vals[n] = val;
                    flags[n] = flag;
                    n++;
                }
            }
        }
        for (u8 i = 0; i < 8; i++) {
            if (ids[i] != 0) {
                u32 cap = func_80136254(lbl_eu_806640D8, &lbl_eu_805063BC[0x503], ids[i]);
                if (vals[i] > cap) vals[i] = cap;
            }
        }
        // same for listB
        u16 ids2[8];
        u32 vals2[8];
        u8 flags2[8];
        u8 n2 = 0;
        for (u8 i = 0; i < 8; i++) {
            u16 id = listB[i].itemId;
            u32 val = listB[i].value;
            u8 flag = listB[i].state;
            if (id == 0) {
                ids2[n2] = id;
                vals2[n2] = val;
                flags2[n2] = flag;
                n2++;
            } else if (func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], id) != 0) {
                u8 found = 0;
                for (u8 j = 0; j < n2; j++) {
                    if (id == ids2[j]) {
                        vals2[j] += val;
                        for (u8 k = j + 1; k < n2; k++) {
                            if (id == ids2[k]) vals2[k] = vals2[j];
                        }
                        found = 1;
                        ids2[n2] = id;
                        vals2[n2] = vals2[j];
                        flags2[n2] = flag;
                        n2++;
                        break;
                    }
                }
                if (!found) {
                    ids2[n2] = id;
                    vals2[n2] = val;
                    flags2[n2] = flag;
                    n2++;
                }
            }
        }
        for (u8 i = 0; i < 8; i++) {
            if (ids2[i] != 0) {
                u32 cap = func_80136254(lbl_eu_806640D8, &lbl_eu_805063BC[0x503], ids2[i]);
                if (vals2[i] > cap) vals2[i] = cap;
            }
        }

        // better/worse flags comparing listB vs listA
        u8 cmp[8];
        for (u8 i = 0; i < 8; i++) {
            if (ids2[i] != 0 && func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], ids2[i]) != 0) {
                u8 done = 0;
                for (u8 j = 0; j < 8; j++) {
                    if (ids2[i] == ids[j]) {
                        if (vals2[i] > vals[j]) cmp[i] = 1;
                        else if (vals2[i] < vals[j]) cmp[i] = 2;
                        done = 1;
                        break;
                    }
                }
                if (!done) cmp[i] = 1;
            } else if (ids2[i] != 0) {
                u8 done = 0;
                for (u8 j = 0; j < 8; j++) {
                    if (ids2[i] == ids[j]) {
                        if (vals2[i] > vals[j]) cmp[i] = 1;
                        else if (vals2[i] < vals[j]) cmp[i] = 2;
                        done = 1;
                        break;
                    }
                }
                if (!done) cmp[i] = 1;
            }
        }
        // clear duplicate ids in listB
        for (u8 i = 0; i < 8; i++) {
            if (ids2[i] != 0 && func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_805063BC[0x4FD], ids2[i]) != 0) {
                for (u8 j = i + 1; j < 8; j++) {
                    if (ids2[i] == ids2[j]) ids2[j] = 0;
                }
            }
        }

        // pane name setup
        nw4r::lyt::Pane* root = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(&lbl_eu_805063BC[0x16E], true);
        for (u8 i = 0; i < 8; i++) {
            char buf1[0x20];
            char buf2[0x20];
            sprintf(buf1, &lbl_eu_805063BC[0x507], i * 2 + 0x13);
            sprintf(buf2, &lbl_eu_805063BC[0x515], i + 0x10);
            func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf1, &lbl_eu_805063BC[0x2AA], 0);
            func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf2, &lbl_eu_805063BC[0x2AA], 0);
        }

        // per-slot render
        for (u8 i = 0; i < 8; i++) {
            u16 id = ids2[i];
            u32 val = vals2[i];
            u8 flag = flags2[i];
            u8 cf = cmp[i];
            if (id == 0) continue;
            char buf1[0x20];
            char buf2[0x20];
            sprintf(buf1, &lbl_eu_805063BC[0x507], i * 2 + 0x13);
            sprintf(buf2, &lbl_eu_805063BC[0x515], i + 0x10);
            nw4r::lyt::Pane* pane1 = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(buf1, true);
            nw4r::lyt::Pane* pane2 = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(buf2, true);
            CItemBoxQuad quad;
            func_80137924((nw4r::math::VEC3*)&quad, pane1, root, ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10)));
            // slot index transposition: idx = (s32)(0.5f * i) + ((i & 1) ? 4 : 0)
            s32 idx = (s32)(0.5f * (f32)i) + ((i & 1) ? 4 : 0);
            if (idx < 12) {
                *(s16*)((u8*)info + 0xB0 + idx * 2) = id;
            }
            if (idx < 12) {
                *(u8*)((u8*)info + 0x158 + idx) = 3;
            }
            if (idx < 12) {
                *(u8*)((u8*)info + 0x164 + idx) = flag;
            }
            if (idx < 12) {
                *(s16*)((u8*)info + 0x170 + idx * 2) = (s16)val;
            }
            if (idx < 12) {
                f32 v0 = ((f32*)&quad)[0];
                f32 v1 = ((f32*)&quad)[1];
                f32 v2 = ((f32*)&quad)[2];
                f32 dst[3] = {v0, v1, v2};
                copyVEC3((void*)((u8*)info + 0xC8 + idx * 12), (void*)dst);
            }
            func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf1, func_8013639C(lbl_eu_806640D8, &lbl_eu_805063BC[0x139]), 0);
            if (val == 0) {
                func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf2, &lbl_eu_805063BC[0x2AA], 0);
            } else {
                u8 rvs = (u8)func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_805063BC[0x3], id);
                if (rvs != 0 && (rvs == 0xFF || rvs == 0xFE)) {
                    ml::FixStr<32> fmtBuf;
                    fmtBuf.format(&lbl_eu_805063BC[0x13E], (s32)val, func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x21));
                    func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf2, fmtBuf.c_str(), 0);
                } else {
                    ml::FixStr<32> fmtBuf;
                    fmtBuf.format(&lbl_eu_805063BC[0x422], (s32)val);
                    func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf2, fmtBuf.c_str(), 0);
                }
            }
            if (arg5 == 0) {
                func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34), buf2, &lbl_eu_805063BC[0x2AA], 0);
            }
            E43Quad q1 = *(E43Quad*)&lbl_eu_80664518;
            E43Quad q2 = *(E43Quad*)&lbl_eu_80664520;
            E43Quad q3 = *(E43Quad*)&lbl_eu_80664558;
            E43Quad q4 = *(E43Quad*)&lbl_eu_80664560;
            u32 cap = func_80136254(lbl_eu_806640D8, &lbl_eu_805063BC[0x503], id);
            if ((u32)val >= cap) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664548);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664550);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664588);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664590);
            } else if (cf == 1) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664528);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664530);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664568);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664570);
            } else if (cf == 2) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_80664538);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_80664540);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_80664578);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_80664580);
            }
            if (arg5 == 0) {
                __as__11_GXColorS10FRC11_GXColorS10(&q1, &lbl_eu_806645A8);
                __as__11_GXColorS10FRC11_GXColorS10(&q2, &lbl_eu_806645B0);
                __as__11_GXColorS10FRC11_GXColorS10(&q3, &lbl_eu_806645C8);
                __as__11_GXColorS10FRC11_GXColorS10(&q4, &lbl_eu_806645D0);
            }
            func_80139AC8(pane1, &q1, &q2);
            func_80139AC8(pane2, &q3, &q4);
        }
    }
}




#pragma push
#pragma auto_inline off
s32 func_801E9190(void* a, void* b, s32 arg2, void* d) {
    // Single-expression sum; builtin s32->f32 cast emits MWCC's
    // 0x4330/xoris double-trick.
    // NOTE: param-save copy pair order residual (see attempts.jsonl).
    s32 prod = arg2 * (s32)(func_801E9310(a, b, 0x52, d) + func_801E92B8(a, b) + 0x64);
    return (s32)(lbl_eu_80668040 * (f32)prod);
}
#pragma pop

#pragma push
#pragma auto_inline off
s32 func_801E9224(void* a, void* b, s32 arg2, void* d) {
    // Single-expression sum; builtin
    // 0x4330/xoris double-trick.
    s32 prod = (s32)(func_801E9310(a, b, 0x53, d) + func_801E92B8(a, b) + 0x64) * arg2;
    return (s32)(lbl_eu_80668040 * (f32)prod);
}
#pragma pop
#pragma push
#pragma auto_inline off
// Retail func_801E9310: the ItemBox2 variant of func_801DF610. Six slots are
// scanned via the lookup table (s16 ids at +0x26/+0x1C..0x24, 6-byte slot
// table from .sdata2). When the slot's byte equals the candidate's equip id
// ((u8)func_801392E4((u32)arg3)), a name-based lookup runs instead of the item
// walk: count = func_801361E8(v1, base+0x432, v2), then for each index the
// name v = func_80136254(v1, buf, v2) resolves a category (func_80139358)
// matching the requested item id, and the name-count byte is added. The
// id==-1 slot only runs the name lookup. Result is clamped by the table cap.
u32 func_801E9310(void* a, void* b, u32 c, void* d) {
    void* lookup = func_8009EC9C((u32)b);
    u32 nameObj = (u32)((char* (*)(void*))func_801393CC)(d);
    u32 nameId = func_80139358((u32)d);
    u8 equipSlot = (u8)func_801392E4((u32)d);
    char* base = lbl_eu_805063BC;
    s16 slotIds[6];
    slotIds[0] = *(s16*)((u8*)lookup + 0x26);
    slotIds[1] = *(s16*)((u8*)lookup + 0x1C);
    slotIds[2] = *(s16*)((u8*)lookup + 0x1E);
    slotIds[3] = *(s16*)((u8*)lookup + 0x20);
    slotIds[4] = *(s16*)((u8*)lookup + 0x22);
    slotIds[5] = *(s16*)((u8*)lookup + 0x24);
    CItemBoxSlotBytes bytes;
    bytes.ab.a = lbl_eu_80668074;
    bytes.ab.b = lbl_eu_80668078;
    char buf[0x20];
    u32 result = 0;
    for (u32 slot = 0; slot < 6; slot++) {
        s16 id = slotIds[(u8)slot];
        if (id != -1) {
            u8 slotByte = bytes.bytes[(u8)slot];
            void* item = func_80157C4C(slotByte, id);
            if (d != NULL && slotByte == equipSlot) {
                // candidate slot: name-based lookup over the item names
                u8 count = (u8)func_801361E8(nameObj, base + 0x432, (u16)nameId);
                for (u8 j = 0; (u8)j < count; j++) {
                    sprintf(buf, base + 0x43b, (u8)j + 1);
                    u32 v = func_80136254((char*)nameObj, buf, (u16)nameId);
                    if ((u16)v != 0) {
                        u16 cat = (u16)func_80139358(v);
                        if (c == cat) {
                            result += (u8)func_801361E8((u32)lbl_eu_806640EC, base + 0x447, (u16)v);
                        }
                    }
                }
            } else if (item != NULL && *(u32*)item != 0) {
                CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
                u8 count = inst->_v30(item);
                for (u8 j = 0; (u8)j < count; j++) {
                    CItemImplVt* inst2 = (CItemImplVt*)CItem_initItemImplInstances(item);
                    s16 v = inst2->_v40(item, (u8)j);
                    if (v != -1) {
                        void* r = func_80157C4C(3, v);
                        if (r != NULL && *(u32*)r != 0) {
                            CItemImplVt* inst3 = (CItemImplVt*)CItem_initItemImplInstances(r);
                            u16 cat = (u16)inst3->_v54(r);
                            if (c == cat) {
                                CItemImplVt* inst4 = (CItemImplVt*)CItem_initItemImplInstances(r);
                                result += inst4->_v90(r);
                            }
                        }
                    } else {
                        CItemImplVt* inst5 = (CItemImplVt*)CItem_initItemImplInstances(item);
                        CItemBoxSubRecord* sub = inst5->_v2C(item, (u8)j);
                        if (sub != NULL) {
                            u16 cat2 = (sub->field_04 >> 4) & 0xFFF;
                            if (c == cat2) {
                                // retail sign-extends the 11-bit field (rlwinm+extsh)
                                s32 val2 = (sub->_00 >> 10) & 0x7FF;
                                result += (s16)val2;
                            }
                        }
                    }
                }
            }
        } else {
            if (d != NULL && bytes.bytes[(u8)slot] == equipSlot) {
                // candidate slot: name-based lookup
                u8 count = (u8)func_801361E8(nameObj, base + 0x432, (u16)nameId);
                for (u8 j = 0; (u8)j < count; j++) {
                    sprintf(buf, base + 0x43b, (u8)j + 1);
                    u32 v = func_80136254((char*)nameObj, buf, (u16)nameId);
                    if ((u16)v != 0) {
                        u16 cat = (u16)func_80139358(v);
                        if (c == cat) {
                            result += (u8)func_801361E8((u32)lbl_eu_806640EC, base + 0x447, (u16)v);
                        }
                    }
                }
            }
        }
    }
    u16 cap = (u16)func_80136254(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x503], c);
    if ((s16)cap < (s32)result) result = cap;
    return result;
}
#pragma pop
#pragma push
#pragma auto_inline off
#pragma optimize_for_size on
u32 func_801E96F0(void* dummy, u32 arg1, u32 arg2) {
    if (arg1 == 0 || arg2 == 0) return 0;
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x1f4], arg1);
    u32 g = (u32)lbl_eu_806640F8;
    return (u8)func_801361E8(g, buf, (u16)func_80139358(arg2)) != 0;
}
#pragma pop
#pragma push
#pragma auto_inline off
u32 func_801E9774(void* global, u16 arg2, void* arg3) {
    if (arg2 == 0) return 0;
    void* g = lbl_eu_806640F8;
    u16 v1 = arg3 ? (u16)func_801392E4((u32)arg3) : (u16)0;
    u16 v2 = arg3 ? (u16)func_80139358((u32)arg3) : (u16)0;
    void* lookup = func_8009EC9C(arg2);
    u32 result = 0;
    for (u32 i = 4; i <= 8; i++) {
        u8 ii = (u8)i;
        if (ii == v1) continue;
        s16 val = -1;
        switch (ii) {
            case 4: val = *(s16*)((u8*)lookup + 0x1C); break;
            case 5: val = *(s16*)((u8*)lookup + 0x1E); break;
            case 6: val = *(s16*)((u8*)lookup + 0x20); break;
            case 7: val = *(s16*)((u8*)lookup + 0x22); break;
            case 8: val = *(s16*)((u8*)lookup + 0x24); break;
        }
        if (val == -1) continue;
        // Retail passes the slot id (u8)i and the slot value val (2-arg form).
        void* r = func_80157C4C((u8)i, val);
        if (r == NULL) continue;
        u32 v = *(u32*)r;
        if (v == 0) continue;
        result += (u8)func_801361E8((u32)g, (char*)&lbl_eu_805063BC[0x1e2], func_80139358(v >> 20));
    }
    if (arg3 != NULL) {
        result += (u8)func_801361E8((u32)g, (char*)&lbl_eu_805063BC[0x1e2], v2);
    }
    return result;
}
#pragma pop
#pragma push
#pragma auto_inline off
bool func_801E98E4(void*, u16 b, void* c) {
    // Validates every equip slot against candidate item c before an exchange:
    // for each of the six slots either the indexed item names must still
    // resolve (when the slot holds the equipped item) or no indexed entry may
    // be occupied/in-use.
    void* lookup = func_8009EC9C(b);
    u32 nameObj = (u32)((char* (*)(void*))func_801393CC)(c);
    u32 nameId = func_80139358((u32)c);
    u8 slotId = (u8)func_801392E4((u32)c);
    // 6-byte slot table from two .sdata2 constants (different pair from
    // func_801DFFB8).
    CItemBoxSlotBytes tbl;
    s16 ids[6];
    // ids[0] is the halfword at lookup+0x26; the remaining five are the
    // packed block at 0x1C..0x24 (retail load order).
    ids[0] = *(s16*)((u8*)lookup + 0x26);
    ids[1] = *(s16*)((u8*)lookup + 0x1C);
    ids[2] = *(s16*)((u8*)lookup + 0x1E);
    ids[3] = *(s16*)((u8*)lookup + 0x20);
    ids[4] = *(s16*)((u8*)lookup + 0x22);
    ids[5] = *(s16*)((u8*)lookup + 0x24);
    tbl.ab.a = lbl_eu_8066807C;
    tbl.ab.b = lbl_eu_80668080;
    char buf[0x20];
    for (u8 i = 0; i < 6; i++) {
        if (ids[i] != -1) {
            u8 slot = tbl.bytes[i];
            void* item = func_80157C4C(slot, ids[i]);
            if (c != NULL && slot == slotId) {
                // candidate matches the equipped slot: verify every indexed
                // item name still resolves.
                u8 count = (u8)func_801361E8(nameObj, &lbl_eu_805063BC[0x432], (u16)nameId);
                for (u8 j = 0; j < count; j++) {
                    sprintf(buf, &lbl_eu_805063BC[0x43b], j + 1);
                    if ((u16)func_80136254((void*)nameObj, buf, (int)(u16)nameId) != 0) return false;
                }
            } else if (item != NULL && *(u32*)item != 0) {
                CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
                u8 count = inst->_v30(item);
                for (u8 j = 0; j < count; j++) {
                    CItemImplVt* inst2 = (CItemImplVt*)CItem_initItemImplInstances(item);
                    s16 v = inst2->_v40(item, j);
                    if (v != -1) {
                        void* r = func_80157C4C(3, v);
                        if (r != NULL && *(u32*)r != 0) return false;
                    } else {
                        CItemImplVt* inst3 = (CItemImplVt*)CItem_initItemImplInstances(item);
                        CItemBoxSubRecord* sub = inst3->_v2C(item, j);
                        if (sub != NULL && ((sub->field_04 >> 4) & 0xFFF) != 0) return false;
                    }
                }
            }
        } else {
            if (c != NULL && tbl.bytes[i] == slotId) {
                u8 count = (u8)func_801361E8(nameObj, &lbl_eu_805063BC[0x432], (u16)nameId);
                for (u8 j = 0; j < count; j++) {
                    sprintf(buf, &lbl_eu_805063BC[0x43b], j + 1);
                    if ((u16)func_80136254((void*)nameObj, buf, (int)(u16)nameId) != 0) return false;
                }
            }
        }
    }
    return true;
}
#pragma pop

// Retail CItemBoxInfo2::OnFileEvent: two file-load branches. The line-arc
// branch (fileHandle1) builds the whole layout in a 0x18000 scratch region;
// the common-arc branch (fileHandle2) only re-attaches its accessor into a
// 0x100 scratch region and refreshes via func_801E4390.
#pragma push
#pragma optimize_for_size on
bool CItemBoxInfo2::OnFileEvent(CEventFile* file) {
    if (state.fileHandle1 == file->mFileHandle) {
        state.memRegion1.createRegion(mtl::MemManager::getHandleMEM2(), 0x18000,
                                      &lbl_eu_805063BC[0x826], 1);
        Class_8045F858 host(&state.memRegion1);
        u8* data = (u8*)state.fileHandle1->getData();
        mtl::MemManager::func_80434A4C(false);
        nw4r::lyt::ArcResourceAccessor* acc = createArcResourceAccessor__10CLibLayoutFv();
        state.arcResourceAccessor = acc;
        acc->Attach(data, &lbl_eu_805063BC[0x531]);
        func_80136E84(&state.layout, acc, &lbl_eu_805063BC[0x535]);
        func_80136F08(state.layout, &state.animTransform1, acc, &lbl_eu_805063BC[0x54a]);
        func_80136F08(state.layout, &state.animTransform2, acc, &lbl_eu_805063BC[0x562]);
        nw4r::lyt::Pane* root = (nw4r::lyt::Pane*)*(void**)((u8*)state.layout + 0x10);
        void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, state.layout);
        func_8013676C(root, reinterpret_cast<CItemBoxFontInfoVt*>(fontObj)->fontData());

        // Seed the label textboxes with the shared text object.
        char* text = func_801355BC();
        func_801368C0(state.layout, &lbl_eu_805063BC[0x25b], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x267], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x273], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x286], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x292], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x29e], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x45a], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x466], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x4a7], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x57f], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x58b], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x597], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5a3], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5af], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5be], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5cd], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x143], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x4e3], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x4f0], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5dc], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5e9], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x5f6], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x603], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x610], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x61d], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x62a], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x637], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x644], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x651], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x65e], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x66b], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x678], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x685], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x692], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x69f], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x6ac], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x6b9], (u32)text);
        func_801368C0(state.layout, &lbl_eu_805063BC[0x6c6], (u32)text);

        state.layout->SetAnimationEnable(state.animTransform2, false);
        state.layout->SetAnimationEnable(state.animTransform1, true);
        state.layout->Animate(0);

        char* base = lbl_eu_805063BC;
        u8 mode = *(u8*)((u8*)this + 0x9A);
        (void)mode;
        // Cache the selection-row panes for the renderer.
        state.slotPanes[0] = root->FindPaneByName(base + 0x77e, true);
        state.slotPanes[1] = root->FindPaneByName(base + 0x644, true);
        state.slotPanes[2] = root->FindPaneByName(base + 0x78a, true);
        state.slotPanes[3] = root->FindPaneByName(base + 0x62a, true);
        state.slotPanes[4] = root->FindPaneByName(base + 0x766, true);
        state.slotPanes[5] = root->FindPaneByName(base + 0x651, true);
        state.slotPanes[6] = root->FindPaneByName(base + 0x796, true);
        state.fileHandle1 = 0;
        state.memRegion1.func_8045F810();
        return true;
    }
    if (state.fileHandle2 == file->mFileHandle) {
        state.memRegion2.createRegion(mtl::MemManager::getHandleMEM2(), 0x100,
                                      &lbl_eu_805063BC[0x834], 1);
        Class_8045F858 host(&state.memRegion2);
        u8* data = (u8*)state.fileHandle2->getData();
        mtl::MemManager::func_80434A4C(false);
        nw4r::lyt::ArcResourceAccessor* acc2 = createArcResourceAccessor__10CLibLayoutFv();
        state.resource = acc2;
        acc2->Attach(data, &lbl_eu_805063BC[0x531]);
        func_801E4390(this);
        state.fileHandle2 = 0;
        state.memRegion2.func_8045F810();
        return true;
    }
    return false;
}
#pragma pop

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern void* lbl_eu_80664518;
extern void* lbl_eu_80664520;
extern void* lbl_eu_80664528;
extern void* lbl_eu_80664530;
extern void* lbl_eu_80664538;
extern void* lbl_eu_80664540;
extern void* lbl_eu_80664548;
extern void* lbl_eu_80664550;
extern void* lbl_eu_80664558;
extern void* lbl_eu_80664560;
extern void* lbl_eu_80664568;
extern void* lbl_eu_80664570;
extern void* lbl_eu_80664578;
extern void* lbl_eu_80664580;
extern void* lbl_eu_80664588;
extern void* lbl_eu_80664590;
extern void* lbl_eu_80664598;
extern void* lbl_eu_806645A0;
extern void* lbl_eu_806645B8;
extern void* lbl_eu_806645C0;
extern void* lbl_eu_806645C8;
extern void* lbl_eu_806645D0;

void sinit_801EABC4() {
    func_801D1F9C(&lbl_eu_80664518, 0);
    func_801D1F9C(&lbl_eu_80664520, 0);
    func_801C4B60(&lbl_eu_80664528, 0x25, 0x8a, 0xce, 0);
    func_801C4B60(&lbl_eu_80664530, 0x25, 0x8a, 0xce, 0);
    func_801C4B60(&lbl_eu_80664538, 0xd2, 0x28, 0x14, 0);
    func_801C4B60(&lbl_eu_80664540, 0xd2, 0x28, 0x14, 0);
    func_801C4B60(&lbl_eu_80664548, 0x1f, 0xb0, 0x1a, 0);
    func_801C4B60(&lbl_eu_80664550, 0x1f, 0xb0, 0x1a, 0);
    func_801D1F9C(&lbl_eu_80664558, 0);
    func_801D1F9C(&lbl_eu_80664560, 0);
    func_801C4B60(&lbl_eu_80664568, 0xff, 0xff, 0xfa, 0);
    func_801C4B60(&lbl_eu_80664570, 0x25, 0x8a, 0xce, 0);
    func_801C4B60(&lbl_eu_80664578, 0xff, 0xff, 0xfa, 0);
    func_801C4B60(&lbl_eu_80664580, 0xd2, 0x28, 0x14, 0);
    func_801C4B60(&lbl_eu_80664588, 0xff, 0xff, 0xfa, 0);
    func_801C4B60(&lbl_eu_80664590, 0x1f, 0xa6, 0x1a, 0);
    func_801D1F9C(&lbl_eu_80664598, 0);
    func_801D1F9C(&lbl_eu_806645A0, 0);
    func_801C4B60(&lbl_eu_806645A8, 0x80, 0x80, 0x80, 0);
    func_801C4B60(&lbl_eu_806645B0, 0x80, 0x80, 0x80, 0);
    func_801D1F9C(&lbl_eu_806645B8, 0);
    func_801D1F9C(&lbl_eu_806645C0, 0);
    func_801C4B60(&lbl_eu_806645C8, 0xff, 0xff, 0xfa, 0);
    func_801C4B60(&lbl_eu_806645D0, 0x80, 0x80, 0x80, 0);
}

// Retail func_801D3C74: fills the item-name buffer at item_data+0xD9 from
// the item table, expanding the $1/$2 format tags in place: $1 is replaced
// by the item name (format select from the func_801361E8 byte value), $2 by
// the category entry text. Both insertions re-splice the tail via the
// three-sprintf sequence (tail backup, insert, tail restore). Retail uses
// the stmw frame and keeps the mtctr table-copy loop (size-optimized).
#pragma push
#pragma optimize_for_size on
char* func_801D3C74(void* item_data, u8 index) {
    if (index >= 12) return NULL;
    u16 item_id = *(u16*)((u8*)item_data + index * 2);
    if (item_id == 0) return NULL;
    u8 category = *(u8*)((u8*)item_data + index + 0xB4);
    s16 val = *(s16*)((u8*)item_data + index * 2 + 0xC0);
    char buf[0x20];
    char tmp[0x20];
    char tailBuf[0x20];
    char* base = lbl_eu_805063BC;
    sprintf(buf, base, val);
    u8 fmt = (u8)func_801361E8((u32)lbl_eu_806640D8, base + 3, item_id);
    char* name = ((char*(*)(void*, const char*, u32))&func_8013639C)(lbl_eu_806640D8, base + 0xC, item_id);
    char* out = (char*)item_data + 0xD9;
    sprintf(out, base + 0x18, name);
    u8 is_jp = 1;
    u8 lang = getLanguage__9CDeviceSCFv();
    if (lang != 3 && getLanguage__9CDeviceSCFv() != 2) is_jp = 0;
    char* scan = out;
    while (*scan) {
        if (*scan == '$') {
            char c = *++scan;
            memset(tmp, 0, 0x20);
            if (c == '1') {
                scan++;
                switch (fmt) {
                    case 1: sprintf(tmp, base + 0x1B, buf); break;
                    case 2: sprintf(tmp, base + 0x1B, buf); break;
                    case 3:
                        if (is_jp) sprintf(tmp, base + 0x30, buf);
                        else sprintf(tmp, base + 0x48, buf);
                        break;
                    case 4:
                        if (is_jp) sprintf(tmp, base + 0x30, buf);
                        else sprintf(tmp, base + 0x48, buf);
                        break;
                }
            } else if (c == '2') {
                scan++;
                // 28-byte table copy (retail mtctr 3-pair loop + tail) from
                // the category string table; entry [1+category] is the text id.
                u32 tbl[7];
                u32* d = tbl;
                u32* s = (u32*)lbl_eu_80506330;
                for (u32 k = 0; k < 3; k++) {
                    *d++ = *s++;
                    *d++ = *s++;
                }
                *d = *s;
                u8 r = (u8)func_801361E8((u32)lbl_eu_806640D8, (const char*)tbl[1 + category], item_id);
                if (is_jp) sprintf(tmp, base + 0x5F, r);
                else sprintf(tmp, base + 0x77, r);
            }
            // re-splice: back up the tail, insert the formatted text over
            // the 2-char tag, then restore the tail after it. The length
            // scan counts the NUL (retail lbz/extsb. loop) so the final
            // tail position is scan-2+len-1.
            u32 len = 0;
            const char* p = tmp;
            s8 b;
            do {
                b = (s8)*p;
                len++;
                p++;
            } while (b != 0);
            sprintf(tailBuf, base + 0x18, scan);
            scan -= 2;
            sprintf(scan, base + 0x18, tmp);
            scan += len;
            scan -= 1;
            sprintf(scan, base + 0x18, tailBuf);
        } else {
            s8 c = (s8)*scan;
            if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xEF)) scan += 2;
            else scan += 1;
        }
    }
    return out;
}
#pragma pop

void func_801DF4B4(void* dst, void* src) {
    *(u16*)((u8*)dst + 0) = *(u16*)((u8*)src + 0);
    *(u32*)((u8*)dst + 4) = *(u32*)((u8*)src + 4);
    *(u8*)((u8*)dst + 8) = *(u8*)((u8*)src + 8);
}

void func_801DF4D0(void* dst, u16 a, u32 b, u8 c) {
    *(u16*)((u8*)dst + 0) = a;
    *(u32*)((u8*)dst + 4) = b;
    *(u8*)((u8*)dst + 8) = c;
}

void func_801E9164(void* dst, void* src) {
    *(u16*)((u8*)dst + 0) = *(u16*)((u8*)src + 0);
    *(u32*)((u8*)dst + 4) = *(u32*)((u8*)src + 4);
    *(u8*)((u8*)dst + 8) = *(u8*)((u8*)src + 8);
}

void func_801E9180(void* dst, u16 a, u32 b, u8 c) {
    *(u16*)((u8*)dst + 0) = a;
    *(u32*)((u8*)dst + 4) = b;
    *(u8*)((u8*)dst + 8) = c;
}

void func_801D77A4(void* arr, u32 index, u16 value) {
    if (index < 12) {
        *(u16*)((u8*)arr + index * 2 + 0xC0) = value;
    }
}

// Retail func_801D4260: finds each slot pane (vtable+0x3C FindPaneByName on
// the layout child) and clears it, then re-labels panes according to the
// item-box state byte (info+0x9A) and the equip byte (info+0xAE) and the
// category index arg2: 2/4-8 -> two panes, 3 -> one, 9/0xA/0xD -> one each.
// The switch is emitted twice (both the state==4/byte!=0 and the state!=4
// paths run the identical category dispatch). Uses the stmw/lmw frame
// (optimize_for_size prologue merge).
#pragma push
#pragma optimize_for_size on
void func_801D4260(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4) {
    char* base = (char*)&lbl_eu_805063BC;
#define SET_PANE(_off, _val) \
    func_80124270(((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(base + _off, true), (u32)(_val))
    SET_PANE(0xc2, 0x0);
    SET_PANE(0xce, 0x0);
    SET_PANE(0xda, 0x0);
    SET_PANE(0xe6, 0x0);
    SET_PANE(0xf2, 0x0);
    SET_PANE(0xfe, 0x0);
    SET_PANE(0x10b, 0x0);
    SET_PANE(0x113, 0x0);
    SET_PANE(0x121, 0x0);
    if (*(u8*)((u8*)info + 0x9A) == 4) {
        if (*(u8*)((u8*)info + 0xAE) == 0) {
            SET_PANE(0xfe, 0x1);
            SET_PANE(0x121, 0x1);
        } else {
            SET_PANE(0x113, 0x1);
            switch (arg2) {
                case 2:
                    SET_PANE(0xc2, 0x1);
                    SET_PANE(0x10b, 0x1);
                    break;
                case 3:
                    SET_PANE(0xda, 0x1);
                    break;
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                    SET_PANE(0xc2, 0x1);
                    SET_PANE(0x10b, 0x1);
                    break;
                case 9:
                    SET_PANE(0xce, 0x1);
                    break;
                case 0xa:
                    SET_PANE(0xe6, 0x1);
                    break;
                case 0xd:
                    SET_PANE(0xf2, 0x1);
                    break;
            }
        }
    } else {
        SET_PANE(0x113, 0x1);
        switch (arg2) {
            case 2:
                SET_PANE(0xc2, 0x1);
                SET_PANE(0x10b, 0x1);
                break;
            case 3:
                SET_PANE(0xda, 0x1);
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                SET_PANE(0xc2, 0x1);
                SET_PANE(0x10b, 0x1);
                break;
            case 9:
                SET_PANE(0xce, 0x1);
                break;
            case 0xa:
                SET_PANE(0xe6, 0x1);
                break;
            case 0xd:
                SET_PANE(0xf2, 0x1);
                break;
        }
    }
#undef SET_PANE
}
#pragma pop

void func_801D47D4(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4) {
    func_801D4A2C((u8*)info + 0xB0);
    if (*(u8*)((u8*)info + 0x9A) == 4) {
        func_801D8E34(info, arg2, arg3, arg4);
    }
    void* item2 = arg3 != 0 ? arg3 : 0;
    u16 category = item2 != 0 ? (u16)(*(u32*)item2 >> 20) : 0;
    u16 slot = arg2;
    if (*(u8*)((u8*)info + 0x9A) == 4) {
        slot = category;
    }
    u8 kind = (u8)func_801392E4((u32)slot);
    if (item2 != 0 && (func_801C6E90(item2) != 0 || func_801D4AB0(item2) == 0)) {
        kind = 9;
    }
    // Dispatch on the selection kind; the 4..8 range opens the item-box
    // detail view and each of the 2/3/9/0xA/0xD cases picks a renderer.
    if ((u32)kind - 4 <= 4) {
        func_801D6394(info, slot, arg3,
                      (*(u8*)((u8*)info + 0x9A) > 2) ? arg2 : (u16)-1);
    } else if (kind == 2) {
        func_801D5DA4(info, slot, arg3,
                      (*(u8*)((u8*)info + 0x9A) > 2) ? (u32)arg2 : 0xFFFF);
    } else if (kind == 3) {
        func_801D79F8(info, slot, arg3,
                      (*(u8*)((u8*)info + 0x9A) > 2) ? arg2 : (u16)-1);
    } else if (kind == 9) {
        func_801D80EC(info, slot, arg3);
    } else if (kind == 0xA) {
        func_801D8058(info, slot);
    } else if (kind == 0xD) {
        func_801D77BC(info, slot);
    } else {
        func_801D8318(info);
        func_801D85D8(info);
        func_801D885C(info);
        func_801D8930(info);
        func_801D8A88(info);
        func_801D8B08(info);
        func_801D8B60(info);
    }
    char* label = func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 3);
    // The bool ctor clears inline (mString[0]=0 / mLength=0) exactly like the
    // retail stb/stw pair; the default ctor would emit a __ct__ call instead.
    ml::FixStr<32> text(true);
    text.format(&lbl_eu_805063BC[0x13e], func_801571FC(), label);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x143],
                  text.c_str(), 0);
}

u8 func_801D4214(CItemBoxInfo* info) {
    return info->state.current;
}

u32 func_801D4240(CItemBoxInfo* info) {
    return info->state.state == 3;
}

u32 func_801D8E08(CItemBoxInfo* info) {
    return info->state.layout != NULL;
}

u8 func_801E13D8(CItemBoxInfo2* info) {
    return *(u8*)((u8*)info + 0x90);
}

u8 func_801E1490(CItemBoxInfo2* info) {
    return *(u8*)((u8*)info + 0x98);
}

u32 func_801E14BC(CItemBoxInfo2* info) {
    return *(u32*)((u8*)info + 0x94) == 3;
}

// GXColorS10::operator= (retail __as__11_GXColorS10FRC11_GXColorS10): the
// implicit 4x s16 struct copy emits the batched lha/sth sequence (all loads
// first, then all stores) that matches the retail body exactly.
struct GXColorS10Copy {
    s16 r, g, b, a;
};
extern "C" void __as__11_GXColorS10FRC11_GXColorS10(void* dst, const void* src) {
    s16* d = (s16*)dst;
    const s16* s = (const s16*)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

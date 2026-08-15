// Auto-scaffolded catalog TU for kyoshin/CItemBoxInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "monolib/util.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
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
char* func_8013639C(void*, char*);
u32 func_801392B4(u32);
u32 func_801392C0();
u32 func_8013600C(void*, const char*, u32);
u32 func_800A32BC();
u32 func_80136254(void*, const char*, u32);
u32 func_800A082C(void*);
void func_8013B380(u32);
extern void* lbl_eu_806645A8;
extern void* lbl_eu_806645B0;
extern void* lbl_eu_80664598;
extern void* lbl_eu_806645A0;
void func_801D885C(CItemBoxInfo*);
extern "C" void func_801D5564(void*, void*, void*, void*);
void func_801D8318(CItemBoxInfo*);
void func_801D4E2C(void*, u16, void*);
extern "C" void func_801D69FC(CItemBoxInfo*, u32, void*);
void func_801D8A88(CItemBoxInfo*);
void func_801D77A4(void*, u32, u16);
extern "C" void func_801D8E34(CItemBoxInfo*, u32, void*, u32);
void func_801E197C(void*, void*, void*);
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
char* func_801394D4(void*, u16);
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

void CItemBoxInfo::advanceItemBoxState() {
    if (state.state == 3) {
        state.state = 4;
        state.visible = 0;
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

void func_801D4AE0(CItemBoxInfo* info, char* arg1, char* arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x151], arg1);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, arg2, 0);
}

void func_801D4BDC(CItemBoxInfo* info, u8 arg2, u8 arg3) {
    if (info->state.layout != 0) {
        func_80136910((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x17d], arg2);
        func_80136910((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x188], arg3);
    }
}

// --- Remaining CItemBoxInfo stubs ---

extern "C" void func_801D8C0C(CItemBoxInfo*);
extern "C" void func_801E4194(CItemBoxInfo2*);

void func_801D421C(CItemBoxInfo* info) {
    if (info->state.state != 0) return;
    info->state.state = 1;
    info->state.visible = 0;
    func_801D8C0C(info);
    return;
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
// 0x34-byte record copy (li r0,6 + 4-byte tail). The record's flag banks at
// +0x1D/+0x24/+0x2B hold per-slot {1,2,0} comparison results against the
// current item's values; bank1 compares the candidate name ratio as a float.
#pragma push
#pragma optimize_for_size on
void func_801D4E2C(void* out, u16 arg2, void* arg3) {
    void* global = lbl_eu_806640F4;
    func_801392E4(arg3);
    u32 v2 = func_80139358((u32)arg3);
    char* base = (char*)&lbl_eu_805063BC;
    CItemBoxCompRecord rec;
    union { double d; u32 w[2]; } u1;
    union { double d; u32 w[2]; } u2;
    u1.w[0] = 0x43300000;
    u2.w[0] = 0x43300000;
    rec.s2C = (u16)func_80136254(global, base + 0x19c, (u16)v2);
    rec.s2E = (u16)func_80136254(global, base + 0x1a4, (u16)v2);
    rec.s30 = (u8)func_801361E8((u32)global, base + 0x1ab, (u16)v2);
    rec.s32 = (u8)func_801361E8((u32)global, base + 0x1b3, (u16)v2);
    rec.s34 = (u8)func_801361E8((u32)global, base + 0x1bb, (u16)v2);
    u1.w[1] = (u8)func_801361E8((u32)global, base + 0x1c3, (u16)v2);
    rec.f38 = (f32)(u1.d - lbl_eu_80668020) / lbl_eu_80668014;
    rec.s3C = (u8)func_801361E8((u32)global, base + 0x1c9, (u16)v2);
    if (func_801361E8((u32)global, base + 0x1d2, (u16)v2) & 4) {
        void* lookup = func_8009EC9C(1);
        u32 r = func_800A082C(lookup);
        u2.w[1] = (u32)(rec.s2C * (u16)r) ^ 0x80000000;
        rec.s2C = (s16)(s32)(lbl_eu_80668018 * (f32)(u2.d - lbl_eu_80668028));
        u32 r2 = func_800A082C(lookup);
        u1.w[1] = (u32)(rec.s2E * (u16)r2) ^ 0x80000000;
        rec.s2E = (s16)(s32)(lbl_eu_8066801C * (f32)(u1.d - lbl_eu_80668028));
        if (rec.s2C > 999) rec.s2C = 999;
        if (rec.s2E > 999) rec.s2E = 999;
    }
    f32 fixedRatio;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 slot = (u8)func_801392B4((u8)i);
        char buf[0x10];
        sprintf(buf, base + 0x1d7, slot);
        u8 v = (u8)func_801361E8((u32)global, buf, (u16)v2);
        // bank1 float compare: candidate name ratio vs the fixed count ratio.
        f32 slotRatio = (f32)((double)v / lbl_eu_80668014);
        fixedRatio = (f32)((double)(u8)rec.s3C / lbl_eu_80668014);
        u8 v1ab = (u8)func_801361E8((u32)global, base + 0x1ab, (u16)v2);
        u8 v1b3 = (u8)func_801361E8((u32)global, base + 0x1b3, (u16)v2);
        if (slotRatio > fixedRatio) rec.flags1[(u8)i] = 1;
        else if (slotRatio < fixedRatio) rec.flags1[(u8)i] = 2;
        else rec.flags1[(u8)i] = 0;
        if ((s32)rec.s30 > (s32)v1ab) rec.flags2[(u8)i] = 1;
        else if ((s32)rec.s30 < (s32)v1ab) rec.flags2[(u8)i] = 2;
        else rec.flags2[(u8)i] = 0;
        if ((s32)rec.s32 > (s32)v1b3) rec.flags3[(u8)i] = 1;
        else if ((s32)rec.s32 < (s32)v1b3) rec.flags3[(u8)i] = 2;
        else rec.flags3[(u8)i] = 0;
    }
    rec.f28 = func_80139C98((u32)rec.s2C, (u32)rec.s2E, 0, rec.f38);
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
extern "C" void func_801D5274(void* out, void* arg2, void* arg3) {
    // Retail builds the 0x1C-byte comparison record in a local (memset on the
    // null path, per-slot fills otherwise) and copies it out at the end.
    CItemBoxSlotFlags local;
    if (arg3 == NULL) {
        memset(&local, 0, 0x1C);
    } else {
        u32 v1 = func_801392E4(arg3);
        u32 v2 = func_80139358((u32)arg3);
        local.v[0] = (u8)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], (u16)v2);
        local.v[1] = (u8)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], (u16)v2);
        local.v[2] = (u8)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], (u16)v2);
        u32 v3 = func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1eb], (u16)v2);
        for (u8 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
            u32 v = func_801392B4((u8)i);
            char buf[0x10];
            sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], (u8)v);
            u8 f = (u8)((u8)func_801361E8((u32)lbl_eu_806640F8, buf, (u16)v2) ? 2 : 1);
            local.flags[i] = f;
            if (f != 0) {
                void* lookup = func_8009EC9C((u8)v);
                if ((u8)v3 == 3) {
                    if (func_8026178C((u8*)lookup + 0x3534, 0x85) == 0) local.flags[i] = 0;
                } else if ((u8)v3 == 2) {
                    if (func_8026178C((u8*)lookup + 0x3534, 0x84) == 0) local.flags[i] = 0;
                } else {
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
    }
    // Whole-struct copy: MWCC inlines constant-size struct copies as the
    // retail mtctr 8-byte-pair loop (3 pairs + 4-byte tail).
    *(CItemBoxSlotFlags*)out = local;
}
// Retail func_801D5564 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) and keeps the mtctr copy loop for the 0xA4-byte record
// copy (li r0,0x14 + 4-byte tail). The name/category tables are resolved
// through the item vtable (arg3) or the raw data pointers (arg3 == NULL);
// the language-dependent text width is scanned Shift-JIS-aware.
#pragma push
#pragma optimize_for_size on
void __declspec(noinline) func_801D5564(void* out, void* unused, void* data, void* arg3) {
    void* item = arg3 != NULL ? arg3 : NULL;
    u32 cat;
    u8 count;
    char* base = (char*)&lbl_eu_805063BC;
    if (arg3 == NULL) {
        func_801392E4(data);
        cat = func_80139358((u32)data);
    } else {
        CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(arg3);
        cat = inst->_v54(arg3);
    }
    if (arg3 == NULL) {
        count = (u8)func_801361E8((u32)lbl_eu_806640EC, base + 0x1f9, (u32)data);
    } else {
        CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(arg3);
        count = (u8)inst->_v08(arg3);
    }
    CItemBoxNameRecord rec;
    rec.count = count;
    rec.str = (u32)func_80136190(base + 0x130, base + 0x139, 0x1e - (count - 1));
    char* s2 = func_80136190(base + 0x202, base + 0x139, 0xf);
    if (arg3 != NULL) {
        CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(arg3);
        sprintf(rec.name, base, inst->_v90(arg3));
    } else {
        sprintf(rec.name, base + 0x18, s2);
    }
    rec.e0 = (u8)func_801361E8((u32)lbl_eu_806640D8, base + 0x3, (u16)cat);
    rec.e1 = (u8)func_801361E8((u32)lbl_eu_806640D8, base + 0x20b, (u16)cat);
    rec.color = 0xFFFFFFFF;
    switch (rec.e1) {
        case 4: rec.color = 0xFF00FFFF; break;
        case 5: rec.color = 0x0000FFFF; break;
        case 6: rec.color = 0xFFFFFFFF; break;
        case 7: rec.color = 0x0000FFFF; break;
        case 8: rec.color = 0x00FFFFFF; break;
        case 9: rec.color = 0x775544FF; break;
    }
    sprintf(rec.text, base + 0x18, func_8013639C(lbl_eu_806640D8, base + 0xc));
    u32 wide = 1;
    if (getLanguage__9CDeviceSCFv() != 3) {
        if (getLanguage__9CDeviceSCFv() != 2) {
            wide = 0;
        }
    }
    // label table: four format-result strings indexed by the count byte.
    char* labels[4];
    for (u32 j = 0; j <= 3; j++) {
        char buf[0x10];
        sprintf(buf, base + 0x4b3, (u8)j * 2 + 0x1f);
        labels[j] = func_80136190(buf, base + 0x130, 0);
    }
    // 0x34-byte chunk copy: pair-copy with s[1]/s[2] accesses reproduces the
    // retail mtctr lwzu/stwu 8-byte-pair loop (li r0,6 + 4-byte tail).
    {
        u8 tmp[0x34];
        memcpy(tmp, labels, 0x10);
        u32* s = (u32*)tmp - 1;
        u32* d = (u32*)&rec._54 - 1;
        for (u32 k = 0; k < 6; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
    u32 name = func_801361E8((u32)lbl_eu_806640D8, labels[count], (u16)cat);
    char buf28[0x20];
    char buf48[0x20];
    if (wide != 0) {
        sprintf(buf28, base + 0x5f, (u8)name);
    } else {
        sprintf(buf28, base + 0x77, (u8)name);
    }
    // strlen of buf28.
    u32 len = 0;
    while (buf28[len] != 0) len++;
    sprintf(buf48, base + 0x18, rec.text);
    char* cur = rec.text - 2;
    sprintf(cur, base + 0x18, buf28);
    cur = cur + len - 1;
    sprintf(cur, base + 0x18, buf48);
    // Shift-JIS-aware width scan (lead bytes 0x81-0x9F / 0xE0-0xEF advance 2).
    char* p = rec.text;
    while (*p != 0) {
        u8 c = (u8)*p;
        if ((c >= 0x81 && c <= 0x9f) || ((s8)c >= 0xe0 && (s8)c <= 0xef)) {
            p += 2;
        } else {
            p += 1;
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
#pragma auto_inline off
extern "C" void func_801D59C0(u32* out, void* unused, void* arg2) {
    func_801392E4(arg2);
    u16 vcat = func_80139358((u32)arg2);
    u8 v0 = (u8)func_801361E8((u32)lbl_eu_80664104, &lbl_eu_805063BC[0x214], vcat);
    char* v1 = (char*)func_80136190(&lbl_eu_805063BC[0x219], &lbl_eu_805063BC[0x139], v0);
    u8 v2 = (u8)func_801361E8((u32)lbl_eu_80664104, &lbl_eu_805063BC[0x225], vcat);
    char* v3;
    if (v2 == 0x1A) {
        v3 = (char*)func_80136190(&lbl_eu_805063BC[0x219], &lbl_eu_805063BC[0x139], 0x14);
    } else {
        v3 = func_8013639C(lbl_eu_806640A8, &lbl_eu_805063BC[0x139]);
    }
    out[0] = v0;
    out[1] = (u32)v1;
    out[2] = v2;
    out[3] = (u32)v3;
}
#pragma pop

void func_801D5AA0(CItemBoxInfo* out, void* unused, void* data) {
    void* global = lbl_eu_80664110;
    u16 v1 = func_801392E4(data);
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
void func_801D5C38(void* out, void* unused, void* data, void* arg3) {
    void* p = arg3 != 0 ? arg3 : 0;
    func_801392E4(data);
    func_80139358((u32)data);
    CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(p);
    u32 r = inst->_v08(p);
    CItemBoxSlotRecord1 rec;
    rec.count = (u8)r;
    char* base = (char*)&lbl_eu_805063BC;
    rec.str = (u32)func_80136190(&base[0x130], &base[0x139], 0x1e - ((u8)r - 1));
    rec.counter = 0;
    for (u32 i = 0; i < 4; i++) {
        CItemImplVt* inst2 = (CItemImplVt*)CItem_initItemImplInstances(p);
        u16 n = inst2->_v4C(p, (u8)i);
        if (n > 0) {
            // Retail passes the pair count n as a third arg to func_8013639C.
            rec.text[rec.counter] =
                (u32)((char*(*)(void*, const char*, u16))&func_8013639C)(lbl_eu_806640D8, &base[0x139], n);
            CItemImplVt* inst3 = (CItemImplVt*)CItem_initItemImplInstances(p);
            rec.vals[rec.counter] = inst3->_v64(p, (u8)i);
            rec.counts[rec.counter] = n;
            rec.counter++;
        }
    }
    // 0x2C-byte copy: MWCC inlines constant-size memcpy as the retail mtctr
    // 8-byte-pair loop (5 pairs + 4-byte tail).
    memcpy(out, &rec, 0x2C);
}
void func_801D5DA4(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4) {
    func_801D8318(info);
    func_801D4E2C(info, arg2, arg3);
    func_801D69FC(info, (u32)arg2, arg3);
    void* layout = info->state.layout;
    char* base = (char*)&lbl_eu_805063BC;
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2c1, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2cc, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2d7, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x267, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x273, base + 0x2aa, 0);
    func_80136190(base + 0x130, base + 0x139, 0);
    char buf[0x40];
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 v = func_801392B4(i);
        func_80136910((nw4r::lyt::Layout*)layout, base + 0x1f4, 0);
        func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x303, base + 0x2aa, 0);
    }
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x135, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x165, base + 0x2aa, 0);
}

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
    func_801D85D8(info);
    CItemBoxSlotFlags flags;
    func_801D5274(&flags, info, (void*)itemId);
    CItemBoxSlotFlags flagsCopy = flags;
    nw4r::lyt::Layout* layout = info->state.layout;
    char* base = lbl_eu_805063BC;
    func_80136B4C(layout, base + 0x25b, base + 0x2aa, 0);
    func_80136B4C(layout, base + 0x286, base + 0x2aa, 0);
    func_80136B4C(layout, base + 0x292, base + 0x2aa, 0);
    func_80136910(layout, base + 0x267, (u8)flagsCopy.v[0]);
    func_80136910(layout, base + 0x273, (u8)flagsCopy.v[1]);
    func_80136910(layout, base + 0x29e, (u8)flagsCopy.v[2]);
    u32 v3 = func_801361E8((u32)lbl_eu_806640F8, base + 0x1eb, (u16)func_80139358(itemId));
    char* s1;
    switch ((u8)v3) {
        case 3: s1 = func_80136190(base + 0x130, base + 0x139, 0x32); break;
        case 2: s1 = func_80136190(base + 0x130, base + 0x139, 0x31); break;
        case 1: s1 = func_80136190(base + 0x130, base + 0x139, 0x30); break;
        case 4: case 5: case 6: case 7: case 8:
        case 9: case 10: case 11: case 12: case 13:
            s1 = func_80136190(base + 0x130, base + 0x139, 0x2e); break;
        default: s1 = 0; break;
    }
    func_80136B4C(layout, base + 0x354, s1, 0);
    char* s2 = 0;
    switch ((u8)v3) {
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
    func_80136B4C(layout, base + 0x2ab, s2, 0);
    func_80139A18(layout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18(layout, base + 0x2c1, &lbl_eu_80664598, &lbl_eu_806645A0);
    func_80139A18(layout, base + 0x2cc, &lbl_eu_80664598, &lbl_eu_806645A0);
    func_80139A18(layout, base + 0x2d7, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18(layout, base + 0x2e2, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18(layout, base + 0x2ed, &lbl_eu_80664598, &lbl_eu_806645A0);
    func_80139A18(layout, base + 0x2f8, &lbl_eu_80664598, &lbl_eu_806645A0);
    u32 cur = (arg4 >> 16) & 0xFF;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 flag = flagsCopy.flags[(u8)i];
        u32 color = 0x777777ff;
        if (flag != 0) color = 0xFFFFFFFF;
        u32 idx = (u8)i + 1;
        char buf[0x20];
        sprintf(buf, base + 0x303, idx);
        func_80137B44(layout, buf, color);
        u32 args[4] = {0, 0, 0, 0};
        s16 zero = 0;
        s16 c0hi[2] = {zero, *(s16*)((u8*)info + 0xA2)};
        s16 c0lo[2] = {zero, zero};
        s16 c1hi[2] = {zero, *(s16*)((u8*)info + 0xAA)};
        s16 c1lo[2] = {zero, zero};
        if (flag != 0) {
            args[0] = *(u32*)((u8*)info + 0x9C);
            args[1] = *(u32*)((u8*)info + 0xA0);
        } else {
            args[0] = *(u32*)c0lo;
            args[1] = *(u32*)c0hi;
        }
        if (flag != 0) {
            args[2] = *(u32*)((u8*)info + 0xA4);
            args[3] = *(u32*)((u8*)info + 0xA8);
        } else {
            args[2] = *(u32*)c1lo;
            args[3] = *(u32*)c1hi;
        }
        sprintf(buf, base + 0x161, idx, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)layout + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* mat = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(mat, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        if (flag == 0 || record == 0) continue;
        u8 slot = (u8)func_801392B4((u8)i);
        if (slot == 0) continue;
        void* lookup = func_8009EC9C(slot);
        u32 v1 = func_801392E4((void*)itemId);
        s16 value = -1;
        switch (v1 & 0xFFFF) {
            case 4: value = *(s16*)((u8*)lookup + 0x1C); break;
            case 5: value = *(s16*)((u8*)lookup + 0x1E); break;
            case 6: value = *(s16*)((u8*)lookup + 0x20); break;
            case 7: value = *(s16*)((u8*)lookup + 0x22); break;
            case 8: value = *(s16*)((u8*)lookup + 0x24); break;
        }
        void* r = func_80157C4C((u16)v1, value);
        if (r == NULL || *(u32*)r == 0 || r != record) continue;
        char buf2[0x20];
        sprintf(buf2, base + 0x30e, idx);
        u8 curSlot = (u8)func_801392B4(cur);
        u32 tex;
        if (slot == curSlot) {
            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)(base + 0x319), 0);
        } else {
            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)(base + 0x32d), 0);
        }
        if (tex == 0) {
            tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)(base + 0x341), 0);
        }
        if (tex != 0) {
            func_80137E7C(layout, buf2, tex);
        }
    }
    func_801D69FC(info, itemId, record);
}
// Retail func_801D69FC: item-box slot renderer. With a candidate record it
// walks the three slot panes showing per-slot item info (regular items via
// the item table, equipped gear via the sub-record); with no record it shows
// the plain item-name list. Ends by stamping the per-slot data into info.
void func_801D69FC(CItemBoxInfo* info, u32 itemId, void* record) {
    ml::FixStr<32> text(true);
    func_801D885C(info);
    char* base = lbl_eu_805063BC;
    char buf[0x20];
    char buf2[0x20];
    record = record != NULL ? record : NULL;
    if (record != NULL && *(u32*)record != 0) {
        CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(record);
        u16 count = (u16)inst->_v30(record);
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
                inst = (CItemImplVt*)CItem_initItemImplInstances(record);
                s16 v40 = (s16)inst->_v40(record, (u8)i);
                if (v40 != -1) {
                    // regular item path: look the item up and format its name.
                    void* rec = func_80157C4C(3, v40);
                    u32 cat = *(u32*)rec >> 20;
                    CItemBoxNameRecord2 rec2;
                    func_801D5564(&rec2, info, (void*)cat, rec);
                    // Retail copies the 0xA4-byte record to a second local via
                    // the memcpy mtctr 8-byte-pair loop (li r0,0x14 + tail).
                    CItemBoxNameRecord2 rec2b;
                    {
                        u32* s = (u32*)&rec2 - 1;
                        u32* d = (u32*)&rec2b - 1;
                        for (u32 k = 0; k < 0x14; k++) {
                            d[1] = s[1];
                            d[2] = s[2];
                            s += 2;
                            d += 2;
                        }
                        d[1] = s[1];
                    }
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
                    nw4r::math::VEC3 pos;
                    func_80137924(&pos, pane, pane2, (nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10));
                    nw4r::math::VEC3 tmp = pos;
                    if ((u8)i < 12) *(s16*)((u8*)info + 0xB0 + (u8)i * 2) = itemVal;
                    if ((u8)i < 12) ((u8*)info)[0x158 + (u8)i] = 3;
                    if ((u8)i < 12) ((u8*)info)[0x164 + (u8)i] = itemCount;
                    if ((u8)i < 12) *(s16*)((u8*)info + 0x170 + (u8)i * 2) = value;
                    if ((u8)i < 12) copyVEC3((u8*)info + 0xC8 + (u8)i * 12, &tmp);
                }
            } else {
                func_80124270(pane, 0);
            }
        }
    } else {
        // No candidate record: plain item-name list driven by the name table.
        void* obj = (void*)func_801393CC((void*)itemId);
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
                    CItemBoxNameRecord2 rec3b;
                    {
                        u32* s = (u32*)&rec3 - 1;
                        u32* d = (u32*)&rec3b - 1;
                        for (u32 k = 0; k < 0x14; k++) {
                            d[1] = s[1];
                            d[2] = s[2];
                            s += 2;
                            d += 2;
                        }
                        d[1] = s[1];
                    }
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
                    char* s = ((char*(*)(void*))func_801394D4)((void*)(u32)nameId);
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
                    nw4r::math::VEC3 pos;
                    func_80137924(&pos, pane, pane2, (nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10));
                    nw4r::math::VEC3 tmp = pos;
                    if ((u8)i < 12) *(s16*)((u8*)info + 0xB0 + (u8)i * 2) = (s16)itemVal;
                    if ((u8)i < 12) ((u8*)info)[0x158 + (u8)i] = 3;
                    if ((u8)i < 12) ((u8*)info)[0x164 + (u8)i] = (u8)itemCount;
                    if ((u8)i < 12) *(s16*)((u8*)info + 0x170 + (u8)i * 2) = (s16)val;
                    if ((u8)i < 12) copyVEC3((u8*)info + 0xC8 + (u8)i * 12, &tmp);
                }
            } else {
                func_80124270(pane, 0);
            }
        }
    }
}

void CItemBoxInfo::setItemBoxIndex(unsigned char index, short value) {
    if (index >= 12) return;
    state.values[index] = value;
}

void func_801D77BC(CItemBoxInfo* info, u16 arg2) {
    func_801D8930(info);
    func_801D85D8(info);
    // Selection-colour staging: each if/else pair assigns two u32 colour
    // values (retail stages them through sp+0x68..0x74 at the top of the
    // frame before the join copies them into out[]).
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
        out[0] = 0;
        out[1] = 0;
        out[2] = 0;
        out[3] = 0;
        if (((u8*)&local)[(u8)i + 1] != 0) {
            out[0] = *(u32*)((u8*)info + 0x9C);
            out[1] = *(u32*)((u8*)info + 0xA0);
        } else {
            out[0] = *(u32*)c0lo;
            out[1] = *(u32*)c0hi;
        }
        if (((u8*)&local)[(u8)i + 1] != 0) {
            out[2] = *(u32*)((u8*)info + 0xA4);
            out[3] = *(u32*)((u8*)info + 0xA8);
        } else {
            out[2] = *(u32*)c1lo;
            out[3] = *(u32*)c1hi;
        }
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
void func_801D79F8(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4) {
    func_801D85D8(info);
    void* layout = info->state.layout;
    char* base = (char*)&lbl_eu_805063BC;
    func_801D8A88(info);
    void* r = func_80157C4C_1(0);
    if (r != NULL && *(u32*)r != 0) {
        func_801D5564(info, 0, r, info);
    }
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 v = func_801392B4(i);
        void* inst = CItem_initItemImplInstances(info);
        u16 cat = func_80139358(v);
        func_80136190(base + 0x130, base + 0x139, cat);
        func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x267, base + 0x2aa, 0);
        func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x273, base + 0x2aa, 0);
        func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x303, base + 0x2aa, 0);
        func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x303, base + 0x2aa, 0);
        func_801D77A4(info, v, arg4);
    }
}
void func_801D8058(CItemBoxInfo* info, u32 arg2) {
    func_801D8B08(info);
    func_801D85D8(info);
    u32 buf[4];
    func_801D59C0(buf, info, (void*)arg2);
    u32 buf2[4];
    buf2[0] = buf[0];
    buf2[1] = buf[1];
    buf2[2] = buf[2];
    buf2[3] = buf[3];
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, (char*)&lbl_eu_805063BC[0x48f], (char*)buf2[1], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, (char*)&lbl_eu_805063BC[0x49b], (char*)buf2[3], 0);
}
void func_801D80EC(CItemBoxInfo* info, u16 arg2, void* arg3) {
    void* layout = info->state.layout;
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x44f, base + 0x2aa, 0);
}
void func_801D8318(CItemBoxInfo* info) {
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x25b, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x267, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x273, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x286, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x292, base + 0x2aa, 0);
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
    u32 i;
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(buf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, 0x777777ff);
        u32 args[4];
        // zero via a variable: MWCC materializes it in a register and cannot
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
        u32 tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)(base + 0x341), 0);
        if (tex != 0) {
            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, buf, tex);
        }
    }
}
extern "C" void func_801D85D8(CItemBoxInfo* info) {
    char* base = (char*)&lbl_eu_805063BC;
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
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(buf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, 0x777777ff);
        u32 args[4];
        // zero via a variable: MWCC materializes it in a register and cannot
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
        u32 tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)(base + 0x341), 0);
        if (tex != 0) {
            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, buf, tex);
        }
    }
}
// Retail func_801D885C uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge): info in r28, loop counter in r29, index in r30,
// string-pool base in r31.
#pragma push
#pragma optimize_for_size on
void func_801D885C(CItemBoxInfo* info) {
    char buf[0x20];
    for (u32 i = 0; i < 3; i++) {
        u32 index = (u8)i + 1;
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
void func_801D8930(CItemBoxInfo* info) {
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x44f, base + 0x2aa, 0);
    char buf[0x20];
    // Retail re-evaluates the slot count on every iteration (bl in the
    // loop condition), so do not hoist the func_801392C0 call.
    for (u32 i = 0; i < (u8)func_801392C0(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(buf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, 0x777777ff);
        u32 args[4];
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
            void* obj = ((void*(*)(void*))(*(void***)pane)[26])(pane);
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(obj, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
    }
}
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
#pragma auto_inline off
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
extern "C" void func_801D8C0C(CItemBoxInfo* info) {
    u8 count = (u8)code80135FDC_getByte_64077();
    char* base = (char*)&lbl_eu_805063BC;
    char bufElse[0x20];
    char buf[0x20];
    u32 args[4];
    for (u32 i = 0; i < 7; i++) {
        if ((u8)i >= count) {
            sprintf(bufElse, base + 0x161, (u8)i + 1);
            nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(bufElse, true);
            func_80124270(pane, 0);
            continue;
        }
        u8 slot = (u8)func_801392B4((u8)i);
        u16 nameId = (u16)func_80136254(lbl_eu_80664090, base + 0x4ce, slot);
        u32 itemName = func_80138F78(nameId);
        void* obj = func_801355F4();
        u32 result = ((u32(*)(void*, u32, u32, u32))(*(void***)obj)[3])(obj, 0x74696D67u, itemName, 0);
        if (result == 0) continue;
        sprintf(buf, base + 0x303, (u8)i + 1);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        func_80137F88(pane, result);
        func_80137C1C(pane, 0x777777FFu);
        s16 c0hi[2] = {0, *(s16*)((u8*)info + 0xA2)};
        s16 c0lo[2] = {0, 0};
        s16 c1hi[2] = {0, *(s16*)((u8*)info + 0xAA)};
        s16 c1lo[2] = {0, 0};
        args[1] = *(u32*)c0hi;
        args[0] = *(u32*)c0lo;
        args[3] = *(u32*)c1hi;
        args[2] = *(u32*)c1lo;
        sprintf(buf, base + 0x161, (u8)i + 1, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane2 = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        void* vertexObj = pane2->GetMaterial();
        for (u32 j = 0; j < 2; j++) {
            func_801D62F8(vertexObj, (u8)j, (u8*)args + (u8)j * 8);
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

static inline s16 calculateItemBoxStat(s16 modifier, s16 base, s32 effect) {
    return (s16)(s32)(0.01f * ((100.0f + (f32)modifier) * (f32)(base + effect)));
}

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
extern "C" void copyVEC3(void*, void*);
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
    ml::FixStr<32> textBuffer;
    // Packed selection: low nibble is slot, bits 16..23 select the party member,
    // and bits 24..27 identify the equipment category. Slot zero takes its
    // category from the candidate item itself.
    u8 slot = (u8)(arg2 & 0xF);
    u8 type = (u8)((arg2 >> 24) & 0xF);
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
    u8 member = (u8)func_801392B4((arg2 >> 16) & 0xFF);
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
    volatile s16 bar1 = calculateItemBoxStat(stC->s10, stA->s20, effect);
    effect = func_801DF610(info, (void*)(u32)member, 0x1, NULL);
    volatile s16 bar2 = calculateItemBoxStat(stC->s0C, stA->s1C, effect);
    effect = func_801DF610(info, (void*)(u32)member, 0x31, NULL);
    s16 bar3 = calculateItemBoxStat(stC->s1C, stA->s32, effect);
    effect = func_801DF610(info, (void*)(u32)member, 0x41, NULL);
    volatile s16 bar4 = calculateItemBoxStat(stC->s0E, stA->s1E - (s16)artsSum, effect);
    effect = func_801DF610(info, (void*)(u32)member, 0x51, NULL);
    s16 bar5 = calculateItemBoxStat(stC->s18, stA->s2E, effect);
    effect = func_801DF610(info, (void*)(u32)member, 0x54, NULL);
    s16 bar6 = calculateItemBoxStat(stC->s22, stA->s38, effect);
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
                func_801D4E2C(&e1, member, (void*)(u32)((u16)((*(u32*)item2 >> 20) & 0xFFFF)));
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
                func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], textBuffer.c_str(), 0);
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
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], textBuffer.c_str(), 0);
            textBuffer.format(&lbl_eu_805063BC[0x13E], (s16)(stA->b55), percentSuffix);
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], textBuffer.c_str(), 0);

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
            func_801D4E2C(&e_cur, member, (void*)(u32)w0);
            D8EEntry& c_cur = comparisonStorage.weapon[1];
            c_cur = e_cur;
            D8EEntry& e_new = comparisonStorage.weapon[2];
            func_801D4E2C(&e_new, member, arg3);
            D8EEntry& c_new = comparisonStorage.weapon[3];
            c_new = e_new;
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
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], textBuffer.c_str(), 0);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], (s16)disp1);
            func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], (s16)disp2);
            char* percentSuffix2 = func_80136190(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
            textBuffer.format(&lbl_eu_805063BC[0x13E], (s16)disp3, percentSuffix2);
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], textBuffer.c_str(), 0);
            char* percentSuffix3 = func_80136190(
                &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
            textBuffer.format(&lbl_eu_805063BC[0x13E],
                           (s32)stA->b55 + (s32)d3, percentSuffix3);
            func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[18], textBuffer.c_str(), 0);
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
            applyArmorEffects(info, member, c_cur, arm1);
            u8 arm2 = (u8)func_801361E8((u32)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], func_80139358((u32)(*(u32*)arg3 >> 20)));
            applyArmorEffects(info, member, c_new, arm2);
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
                func_801D4E2C(&previewWeapon, member,
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
                              textBuffer.c_str(), 0);

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
                func_801D4E2C(&ew, member, (void*)(u32)(u16)(*(u32*)item2 >> 20));
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
                func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], textBuffer.c_str(), 0);

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
            applyItemBoxDeltaColorAt(info, 8, 9, agilityDelta);
        }

        // ItemBox1 also previews effects for the selected inventory slot. The
        // slot-aware helper mirrors func_801DF610 but includes the candidate
        // item and selected slot in its lookup.
        {
        s32 currentEquipBonus = 0;
        if (func_801DFFB8(info, member, NULL, NULL)) {
            currentEquipBonus = func_801DFD60(info, (void*)(u32)member, 0x30);
        }
        s16 oldStrength = calculateItemBoxStat(stC->s10 + currentEquipBonus, stA->s20,
            func_801DF610(info, (void*)(u32)member, 0x21, NULL));
        s16 newStrength = calculateItemBoxStat(stC->s10 + currentEquipBonus, stA->s20,
            func_801DF988(info, (void*)(u32)member, 0x21, arg3, slot));
        s16 strengthDelta = (s16)(newStrength - oldStrength);
        func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[8], newStrength);
        applyItemBoxDeltaColorAt(info, 8, 9, strengthDelta);

        s16 oldPhysical = calculateItemBoxStat(stC->s0C + currentEquipBonus, stA->s1C,
            func_801DF610(info, (void*)(u32)member, 0x01, NULL));
        s16 newPhysical = calculateItemBoxStat(stC->s0C + currentEquipBonus, stA->s1C,
            func_801DF988(info, (void*)(u32)member, 0x01, arg3, slot));
        s16 physicalDelta = (s16)(newPhysical - oldPhysical);
        func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[4], newPhysical);
        applyItemBoxDeltaColorAt(info, 4, 5, physicalDelta);

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

        s16 oldEther = calculateItemBoxStat(stC->s1C, stA->s32,
            func_801DF610(info, (void*)(u32)member, 0x31, NULL));
        s16 newEther = calculateItemBoxStat(stC->s1C, stA->s32,
            func_801DF988(info, (void*)(u32)member, 0x31, arg3, slot));
        s16 etherDelta = (s16)(newEther - oldEther);
        func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[12], newEther);
        applyItemBoxDeltaColorAt(info, 12, 13, etherDelta);

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

        s16 newAgility = calculateItemBoxStat(stC->s0E, oldAgilityBase,
            func_801DF988(info, (void*)(u32)member, 0x41, arg3, slot));
        s16 agilityDelta = (s16)(newAgility - oldAgility);
        func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[6], newAgility);
        applyItemBoxDeltaColorAt(info, 6, 7, agilityDelta);

        s16 oldDefense = calculateItemBoxStat(stC->s18, stA->s2E,
            func_801DF610(info, (void*)(u32)member, 0x51, NULL));
        s16 newDefense = calculateItemBoxStat(stC->s18, stA->s2E,
            func_801DF988(info, (void*)(u32)member, 0x51, arg3, slot));
        s16 defenseDelta = (s16)(newDefense - oldDefense);
        func_80136C98(((nw4r::lyt::Pane**)((u8*)info + 0x40))[14], newDefense);
        applyItemBoxDeltaColorAt(info, 14, 15, defenseDelta);

        s16 oldResistance = calculateItemBoxStat(stC->s22, stA->s38,
            func_801DF610(info, (void*)(u32)member, 0x54, NULL));
        s16 newResistance = calculateItemBoxStat(stC->s22, stA->s38,
            func_801DF988(info, (void*)(u32)member, 0x54, arg3, slot));
        char* percentSuffix = func_80136190(
            &lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 0x80);
        textBuffer.format(&lbl_eu_805063BC[0x13E], newResistance,
                              percentSuffix);
        func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[16], textBuffer.c_str(), 0);
        s16 resistanceDelta = (s16)(newResistance - oldResistance);
        applyItemBoxDeltaColorAt(info, 16, 17, resistanceDelta);

        void* currentWeaponItem = func_80157C4C(2, *(s16*)((u8*)charObj + 0x26));
        s16 attackDelta = 0;
        if (currentWeaponItem != NULL && *(u32*)currentWeaponItem != 0) {
            D8EEntry currentWeapon;
            func_801D4E2C(&currentWeapon, member,
                (void*)(u32)(u16)(*(u32*)currentWeaponItem >> 20));
            D8EEntry weaponStats = currentWeapon;
            s32 oldBaseEffect = func_801DF610(
                info, (void*)(u32)member, 0x01, NULL);
            s16 oldAttackBase = calculateItemBoxStat(
                stC->s0C + currentEquipBonus, stA->s1C, oldBaseEffect);
            s32 newBaseEffect = func_801DF988(
                info, (void*)(u32)member, 0x01, arg3, slot);
            s16 newAttackBase = calculateItemBoxStat(
                stC->s0C, stA->s1C, newBaseEffect);

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
                          textBuffer.c_str(), 0);
        }
        applyItemBoxDeltaColorAt(info, 10, 11, attackDelta);
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
            D8EArmorEntry c_cur = e_cur;
            D8EArmorEntry e_new;
            func_801D5274(&e_new, (void*)(u32)member, arg3);
            D8EArmorEntry c_new = e_new;
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
            applyArmorEffects(info, member, c_cur, arm1);
            u8 arm2 = (u8)func_801361E8((u32)lbl_eu_806640F8, &lbl_eu_805063BC[0x1EB], func_80139358((u32)(*(u32*)arg3 >> 20)));
            applyArmorEffects(info, member, c_new, arm2);
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
                func_801D4E2C(&ew, member, (void*)(u32)(u16)(*(u32*)item2 >> 20));
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
                func_80136D74(((nw4r::lyt::Pane**)((u8*)info + 0x40))[10], textBuffer.c_str(), 0);
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
                          buf2, textBuffer.c_str(), 0);
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
    u32 t1 = func_801DFD60(a, b, 0x2d);
    u32 t2 = func_801DF610(a, b, 0x52, d);
    s32 sum = (s32)(t2 + t1 + 0x64);
    union { double d; u32 w[2]; } u;
    u.w[1] = (u32)(arg2 * sum) ^ 0x80000000;
    u.w[0] = 0x43300000;
    return (s32)(lbl_eu_80668040 * (float)(u.d - lbl_eu_80668028));
}

s32 func_801DF578(void* a, void* b, s32 arg2, void* d) {
    u32 t1 = func_801DFD60(a, b, 0x2d);
    u32 t2 = func_801DF610(a, b, 0x53, d);
    s32 sum = (s32)(t2 + t1 + 0x64);
    union { double d; u32 w[2]; } u;
    u.w[1] = (u32)(arg2 * sum) ^ 0x80000000;
    u.w[0] = 0x43300000;
    return (s32)(lbl_eu_80668040 * (float)(u.d - lbl_eu_80668028));
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
        if (id != -1) {
            u8 slotByte = bytes.bytes[(u8)slot];
            void* item = func_80157C4C(slotByte, id);
            if (arg3 != NULL && (s32)slotByte == (s32)((*(u32*)arg3 >> 16) & 0xF)) item = arg3;
            if (item != NULL && *(u32*)item != 0) {
                CItemImplVt* inst = (CItemImplVt*)CItem_initItemImplInstances(item);
                u8 count = inst->_v30(item);
                for (u8 j = 0; (u8)j < count; j++) {
                    CItemImplVt* inst2 = (CItemImplVt*)CItem_initItemImplInstances(item);
                    s16 v = inst2->_v40(item, (u8)j);
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
                u8 slotByteB = bytes.bytes[(u8)slot];
                void* itemB = NULL;
                if ((s32)slotByteB == (s32)((*(u32*)arg3 >> 16) & 0xF)) itemB = arg3;
                if (itemB != NULL && *(u32*)itemB != 0) {
                    CItemImplVt* instB = (CItemImplVt*)CItem_initItemImplInstances(itemB);
                    u8 countB = instB->_v30(itemB);
                    for (u8 jB = 0; (u8)jB < countB; jB++) {
                        CItemImplVt* inst2B = (CItemImplVt*)CItem_initItemImplInstances(itemB);
                        s16 vB = inst2B->_v40(itemB, (u8)jB);
                        if (vB != -1) {
                            void* rB = func_80157C4C(3, vB);
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
                            CItemBoxSubRecord* subB = inst5B->_v2C(itemB, (u8)jB);
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
u32 func_801DFD60(void* a, void* b, u32 arg2) {
    u32 result = 0;
    void* obj = (u8*)func_8009EC9C((u32)b) + 0x3534;
    if (func_8026178C(obj, arg2) != 0) {
        result = func_8025FB10(obj, arg2);
    }
    return result;
}

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


u32 func_801DFDC0(void* dummy, u32 arg1, void* arg2) {
    if (arg1 == 0 || arg2 == 0) return 0;
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x1f4], arg1);
    u16 r5 = func_80139358(*(u32*)arg2 >> 20);
    u32 result = func_801361E8((u32)lbl_eu_806640F8, buf, r5);
    return (u8)result != 0;
}
u32 func_801DFE48(void* global, u16 arg2, void* arg3) {
    if (arg2 == 0) return 0;
    void* g = lbl_eu_806640F8;
    u16 v1 = arg3 ? (u16)func_801392E4(arg3) : 0;
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

bool CItemBoxInfo::OnFileEvent(CEventFile* file) {
    return false;
}

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

extern "C" CItemBoxInfo* __dt__12CItemBoxInfoFv(CItemBoxInfo* info, s32 flags) {
    if (info != NULL) {
        __dt__17UnkClass_8045F564Fv(&info->state.memRegion2, -1);
        __dt__17UnkClass_8045F564Fv(&info->state.memRegion1, -1);
        if (flags > 0) ::operator delete(info);
    }
    return info;
}

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


void CItemBoxInfo2::drawItemBox2Layout(nw4r::lyt::DrawInfo* drawInfo) {
    if (state.active != 0) {
        func_80137038((nw4r::lyt::Layout*)state.layout, drawInfo, 0, 1);
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

void CItemBoxInfo2::advanceItemBox2State() {
    if (state.state == 3) {
        state.state = 4;
        state.visible = 0;
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
    u32 r = (u8)(u32)func_801392E4((void*)(u32)idFinal);
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
    ml::FixStr<32> textBuffer;
    textBuffer.format(&lbl_eu_805063BC[0x13e], func_801571FC(), s);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x143], textBuffer.c_str(), 0);
}
#pragma pop
void func_801E16F0(CItemBoxInfo2* info, char* arg1, char* arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x151], arg1);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, arg2, 0);
}

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
    if (arg3 == NULL) {
        memset(&rec, 0, 0x34);
    } else {
        void* global = lbl_eu_806640F4;
        func_801392E4(arg3);
        u32 v2 = func_80139358((u32)arg3);
        char* base = (char*)&lbl_eu_805063BC;
        union { double d; u32 w[2]; } u1;
        union { double d; u32 w[2]; } u2;
        u1.w[0] = 0x43300000;
        u2.w[0] = 0x43300000;
        rec.s2C = (u16)func_80136254(global, base + 0x19c, (u16)v2);
        rec.s2E = (u16)func_80136254(global, base + 0x1a4, (u16)v2);
        rec.s30 = (u8)func_801361E8((u32)global, base + 0x1ab, (u16)v2);
        rec.s32 = (u8)func_801361E8((u32)global, base + 0x1b3, (u16)v2);
        rec.s34 = (u8)func_801361E8((u32)global, base + 0x1bb, (u16)v2);
        u1.w[1] = (u8)func_801361E8((u32)global, base + 0x1c3, (u16)v2);
        rec.f38 = (f32)(u1.d - lbl_eu_80668020) / lbl_eu_80668014;
        rec.s3C = (u8)func_801361E8((u32)global, base + 0x1c9, (u16)v2);
        if (func_801361E8((u32)global, base + 0x1d2, (u16)v2) & 4) {
            void* lookup = func_8009EC9C(1);
            u32 r = func_800A082C(lookup);
            u2.w[1] = (u32)(rec.s2C * (u16)r) ^ 0x80000000;
            rec.s2C = (s16)(s32)(lbl_eu_80668018 * (f32)(u2.d - lbl_eu_80668028));
            u32 r2 = func_800A082C(lookup);
            u1.w[1] = (u32)(rec.s2E * (u16)r2) ^ 0x80000000;
            rec.s2E = (s16)(s32)(lbl_eu_8066801C * (f32)(u1.d - lbl_eu_80668028));
            if (rec.s2C > 999) rec.s2C = 999;
            if (rec.s2E > 999) rec.s2E = 999;
        }
        f32 fixedRatio;
        for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
            u8 slot = (u8)func_801392B4((u8)i);
            char buf[0x10];
            sprintf(buf, base + 0x1d7, slot);
            u8 v = (u8)func_801361E8((u32)global, buf, (u16)v2);
            // bank1 float compare: candidate name ratio vs the fixed ratio.
            f32 slotRatio = (f32)((double)v / lbl_eu_80668014);
            fixedRatio = (f32)((double)(u8)rec.s3C / lbl_eu_80668014);
            u8 v1ab = (u8)func_801361E8((u32)global, base + 0x1ab, (u16)v2);
            u8 v1b3 = (u8)func_801361E8((u32)global, base + 0x1b3, (u16)v2);
            if (slotRatio > fixedRatio) rec.flags1[(u8)i] = 1;
            else if (slotRatio < fixedRatio) rec.flags1[(u8)i] = 2;
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
    u32 v1 = func_801392E4(arg3);
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
        func_801392E4(data);
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

void func_801E2558(u32* out, void* info, void* arg2) {
    u32 v0 = func_801392E4(arg2);
    u16 v2 = func_80139358((u32)arg2);
    u8 r1 = func_801361E8((u32)lbl_eu_80664104, (char*)&lbl_eu_805063BC[0x214], v2);
    char* r2 = (char*)func_80136190((char*)&lbl_eu_805063BC[0x219], (char*)&lbl_eu_805063BC[0x139], r1);
    u8 r3 = func_801361E8((u32)lbl_eu_80664104, (char*)&lbl_eu_805063BC[0x225], v2);
    char* r4;
    if (r3 == 0x1A) {
        r4 = (char*)func_80136190((char*)&lbl_eu_805063BC[0x219], (char*)&lbl_eu_805063BC[0x139], 0x14);
    } else {
        r4 = func_8013639C(lbl_eu_806640A8, (char*)&lbl_eu_805063BC[0x139]);
    }
    out[0] = r1;
    out[1] = (u32)r2;
    out[2] = r3;
    out[3] = (u32)r4;
}

// Retail func_801E2638 (ItemBox2 variant of func_801D5AA0): fills the 8-byte
// slot table (flag byte + per-slot selected flags) and a flag2 result from the
// item-table lookup, then writes all three to the info block. Retail uses the
// stmw/lmw frame (optimize_for_size prologue merge).
#pragma push
#pragma optimize_for_size on
void func_801E2638(CItemBoxInfo2* info, u16 arg2, void* data) {
    void* global = lbl_eu_80664110;
    u16 v1 = func_801392E4(data);
    u16 v2 = func_80139358((u32)data);
    u8 arr[8];
    arr[0] = (u8)func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x22b], v2);
    u32 count = func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x237], v2);
    u8 count8 = (u8)count;
    u8* arrp = arr;
    for (u32 i = 0; i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 a = (u8)func_801392B4((u8)i);
        arrp[(u8)i + 1] = (a == count8) ? 1 : 0;
    }
    u32 key = func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x23f], v2);
    u32 val = func_8013600C((char*)&lbl_eu_805063BC[0x248], (char*)&lbl_eu_805063BC[0x250], (u8)key);
    void* lookup = func_8009EC9C((u8)count);
    u8 cat = (u8)func_800A32BC();
    u8* entry = (u8*)lookup + cat * 0x49 + (u8)val * 2;
    u8 flag2 = 0;
    u8* flag2p = &flag2;
    switch (arr[0]) {
        case 1: if (entry[0xE8] != 0) *flag2p = 1; break;
        case 2: if ((entry[0xE9] >> 0) & 1) *flag2p = 1; break;
        case 3: if ((entry[0xE9] >> 1) & 1) *flag2p = 1; break;
    }
    *(u32*)((u8*)info + 0) = *(u32*)arr;
    *(u32*)((u8*)info + 4) = *(u32*)(arr + 4);
    ((u8*)info)[8] = *flag2p;
}
#pragma pop
// Retail func_801E27D0 fills a 0x24-byte slot record (count byte, string ptr,
// per-item text ptrs at +8, per-item values at +0x1C, counter at +0x21) then
// copies it out. The func_801392E4/80139358 results are discarded (dead calls
// kept because MWCC never elides calls). Param 2 (info) is unused in retail.
void func_801E27D0(u8* out, void* unused, void* item, void* arg4) {
    u8* dst = out;
    CItemBoxSlotRecord rec;
    void* p = arg4 != 0 ? arg4 : 0;
    func_801392E4(item);
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
    u32* d = (u32*)dst;
    u32* s = (u32*)&rec;
    for (u32 k = 0; k < 4; k++) {
        for (u32 j = 0; j < 2; j++) {
            *d++ = *s++;
        }
    }
    *(u32*)((u8*)dst + 0x20) = *(u32*)((u8*)&rec + 0x20);
}
// Render the item-box slot info: for each slot (count from the slot-table
// byte getter) set up a labelled layout pane, per-slot vertex colours, and
// (when a selection applies) a texture/name tag.
void func_801E2928(CItemBoxInfo2* info, u16 arg1, void* arg2, u16 arg3) {
    func_801E3918(info);
    CItemBoxQuad quad;
    char label[0x20];
    char paneName[0x20];
    u8 slotData[0x30];
    u8 slotCopy[0x30];
    void* layout = *(void**)((u8*)info + 0x34);
    func_801E197C(slotData, info, arg2);
    memcpy(slotCopy, slotData, 0x30);

    char* base = (char*)&lbl_eu_805063BC;
    u32 max = code80135FDC_getByte_64077();
    for (u8 i = 0; i < (u8)max; i++) {
        u8 flag = slotCopy[i + 0x1A];
        int colorVal = flag ? -1 : 0x777777ff;

        sprintf(paneName, &base[0x303], i + 1);
        func_80137B44((nw4r::lyt::Layout*)layout, paneName, colorVal);

        // Build the two per-slot colour pairs; the selection colour comes from
        // the info block when the slot is active, otherwise from the defaults.
        s16 va2 = *(s16*)((u8*)info + 0xA2);
        s16 vaa = *(s16*)((u8*)info + 0xAA);
        quad.col[1] = CItemBoxQuadColor(0, 0, 0, va2);
        quad.col[0] = CItemBoxQuadColor(0, 0, 0, vaa);
        quad.col[2] = CItemBoxQuadColor();
        quad.col[3] = CItemBoxQuadColor();
        quad.col[2] = flag ? *(CItemBoxQuadColor*)((u8*)info + 0x9C) : quad.col[1];
        quad.col[3] = flag ? *(CItemBoxQuadColor*)((u8*)info + 0xA4) : quad.col[0];

        sprintf(paneName, &base[0x161], i + 1);
        void* child = *(void**)((u8*)layout + 0x10);
        void* pane = ((void*(*)(void*, void*, int))(*(void***)child)[15])(child, paneName, 1);
        if (pane != 0) {
            void* obj = ((void*(*)(void*))(*(void***)pane)[26])(pane);
            for (int j = 0; j < 2; j++) {
                func_801D62F8(obj, j, &quad.col[2 + j]);
            }
        }

        if (flag != 0) {
            if (arg2 != 0 || arg1 != 0) {
                u8 idx = (u8)func_801392B4(i);
                if (idx != 0) {
                    void* lookup = func_8009EC9C(idx);
                    void* item = ((void*(*)(u32, u32))func_80157C4C)(2, *(s16*)((u8*)lookup + 0x26));
                    if (item != 0) {
                        u32 val = *(u32*)item;
                        if (val != 0 && (arg2 == item || arg1 == (u32)(val >> 20))) {
                            sprintf(label, &base[0x30e], i + 1);
                            void* arc = *(void**)((u8*)info + 0x2C);
                            void* tex;
                            if (idx == (u8)func_801392B4(arg3)) {
                                tex = ((void*(*)(void*, u32, const char*, int))(*(void***)arc)[3])(arc, 0x74696d67, &base[0x319], 0);
                            } else {
                                tex = ((void*(*)(void*, u32, const char*, int))(*(void***)arc)[3])(arc, 0x74696d67, &base[0x32d], 0);
                            }
                            if (tex == 0) {
                                tex = ((void*(*)(void*, u32, const char*, int))(*(void***)arc)[3])(arc, 0x74696d67, &base[0x341], 0);
                            }
                            if (tex != 0) {
                                func_80137E7C((nw4r::lyt::Layout*)layout, label, (u32)tex);
                            }
                        }
                    }
                }
            }
        }
    }
}
// Retail func_801E2C5C uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge) and keeps the mtctr copy loop for the 0x1C-byte record
// copy, so it is wrapped in optimize_for_size like the other stmw-frame
// functions in this unit.
#pragma push
#pragma optimize_for_size on
void func_801E2C5C(CItemBoxInfo2* info, u16 arg2, void* arg3, u16 arg4) {
    func_801E3B9C(info);
    // Declaration order follows the retail frame layout (high -> low): record,
    // buf, bufName, local, args, colour pairs.
    CItemBoxSlotFlags record;
    char buf[0x20];
    char bufName[0x20];
    CItemBoxSlotFlags local;
    u32 args[4];
    s16 c0hi[2];
    s16 c0lo[2];
    s16 c1hi[2];
    s16 c1lo[2];
    s16 zero = 0;
    func_801E1E0C(&local, info, (void*)arg2);
    // 0x1C-byte copy: pair-copy with s[1]/s[2] accesses reproduces the retail
    // mtctr lwzu/stwu 8-byte-pair loop (li r0,3 + 4-byte tail).
    {
        u32* s = (u32*)&local - 1;
        u32* d = (u32*)&record - 1;
        for (u32 k = 0; k < 3; k++) {
            d[1] = s[1];
            d[2] = s[2];
            s += 2;
            d += 2;
        }
        d[1] = s[1];
    }
    char* base = (char*)&lbl_eu_805063BC;
    for (u32 i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u8 flag = record.flags[(u8)i];
        u32 color = 0x777777FF;
        if (flag != 0) color = 0xFFFFFFFF;
        sprintf(buf, base + 0x303, (u8)i + 1);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, color);
        // zero via a variable: MWCC materializes it in a register and cannot
        // fold the {zero, zero} s16-pair arrays to a constant, keeping the
        // retail stack build of the four u32 colour args.
        c0hi[0] = zero;
        c0lo[1] = zero;
        c0lo[0] = zero;
        c0hi[1] = *(s16*)((u8*)info + 0xA2);
        c1hi[0] = zero;
        c1lo[1] = zero;
        c1lo[0] = zero;
        c1hi[1] = *(s16*)((u8*)info + 0xAA);
        args[0] = 0;
        args[1] = 0;
        args[2] = 0;
        args[3] = 0;
        if (flag != 0) {
            args[0] = *(u32*)((u8*)info + 0x9C);
            args[1] = *(u32*)((u8*)info + 0xA0);
        } else {
            args[0] = *(u32*)c0lo;
            args[1] = *(u32*)c0hi;
        }
        if (flag != 0) {
            args[2] = *(u32*)((u8*)info + 0xA4);
            args[3] = *(u32*)((u8*)info + 0xA8);
        } else {
            args[2] = *(u32*)c1lo;
            args[3] = *(u32*)c1hi;
        }
        sprintf(buf, base + 0x161, (u8)i + 1, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            nw4r::lyt::Material* material = pane->GetMaterial();
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(material, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
        if (flag != 0 && (arg3 != NULL || arg2 != 0)) {
            u8 slot = (u8)func_801392B4((u8)i);
            if (slot != 0) {
                void* member = func_8009EC9C(slot);
                u16 category = (u16)func_801392E4((void*)arg2);
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
                    if (arg3 == item || arg2 == (u16)(*(u32*)item >> 20)) {
                        sprintf(bufName, base + 0x30e, (u8)i + 1);
                        u8 otherSlot = (u8)func_801392B4(arg4);
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
                            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, bufName, tex);
                        }
                    }
                }
            }
        }
    }
}
#pragma pop
void func_801E2FEC(CItemBoxInfo2* info, u16 arg2) {
    func_801E3EB8(info);
    func_801E3B9C(info);
    u32 buf[2];
    func_801E2638(info, 0, (void*)0);
    void* layout = *(void**)((u8*)info + 0x34);
    char* base = (char*)&lbl_eu_805063BC;
    char tmp[0x20];
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        sprintf(tmp, base + 0x303, i + 1);
        func_80136B4C((nw4r::lyt::Layout*)layout, tmp, base + 0x2aa, 0);
        func_80137B44((nw4r::lyt::Layout*)layout, tmp, 0x777777ff);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(tmp, true);
        if (pane != NULL) {
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8((u8*)pane + 0x10, j, tmp);
            }
        }
    }
}
void func_801E3228(CItemBoxInfo2* info, u16 arg2, void* arg3, u16 arg4) {
    void* layout = *(void**)((u8*)info + 0x34);
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x44f, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x267, base + 0x2aa, 0);
    u32 max = func_801392C0();
    char buf[0x40];
    for (u32 i = 0; i < max; i++) {
        sprintf(buf, base + 0x303, i + 1);
        func_80137B44((nw4r::lyt::Layout*)layout, buf, 0x777777ff);
        void* child = *(void**)((u8*)layout + 0x10);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)child)->FindPaneByName(buf, true);
        if (pane != NULL) {
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8((u8*)pane + 0x10, j, buf);
            }
        }
    }
}
void func_801E3730(CItemBoxInfo2* info, u32 arg2) {
    func_801E4090(info);
    func_801E3B9C(info);
    u32 buf[4];
    func_801E2558(buf, info, (void*)arg2);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, (char*)&lbl_eu_805063BC[0x48f], (char*)buf[1], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, (char*)&lbl_eu_805063BC[0x49b], (char*)buf[3], 0);
}
// Retail func_801E37C4: run the two layout preps, build a 0x24-byte slot
// record via func_801E27D0, copy it to a local, then label every slot: a
// count-driven loop of 4-byte pane names + per-item text/val colour labels
// (FixStr<32> buffer sits between the record and the copy in the frame).
void func_801E37C4(CItemBoxInfo2* info, void* arg1, void* arg2) {
    char paneName[0x20];
    u8 out[0x24];
    func_801E40E8(info);
    func_801E3B9C(info);
    func_801E27D0(out, info, arg1, arg2);
    u8 dst[0x24];
    memcpy(dst, out, 0x24);
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)*(void**)((u8*)info + 0x34);
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C(layout, &base[0x4a7], *(const char**)(dst + 4), 0);
    u8 count = dst[0x21];
    for (u8 i = 0; i < count; i++) {
        sprintf(paneName, &base[0x4b3], (u8)i * 2 + 0x1f);
        func_80136B4C(layout, paneName, *(const char**)(dst + 8 + (u8)i * 4), 0);
        ml::FixStr<32> text;
        sprintf(paneName, &base[0x4c0], (u8)i + 0x1f);
        text.format(&base[0x13e], dst[0x1c + i], func_80136190(&base[0x130], &base[0x139], 0x21));
        func_80136B4C(layout, paneName, text.c_str(), 0);
    }
}
void func_801E3918(CItemBoxInfo2* info) {
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x25b, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x267, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x273, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x286, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x292, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x29e, base + 0x2aa, 0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2c1, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2cc, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2d7, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2e2, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2ed, &lbl_eu_806645A8, &lbl_eu_806645B0);
    char buf[0x20];
    u32 i;
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(buf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, 0x777777ff);
        u32 args[4];
        // zero via a variable: MWCC materializes it in a register and cannot
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
        u32 tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)(base + 0x341), 0);
        if (tex != 0) {
            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, buf, tex);
        }
    }
}
void func_801E3B9C(CItemBoxInfo2* info) {
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x267, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x273, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x29e, base + 0x2aa, 0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2c1, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2cc, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2d7, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2e2, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)info->state.layout, base + 0x2ed, &lbl_eu_806645A8, &lbl_eu_806645B0);
    char buf[0x20];
    u32 i;
    for (i = 0; (u8)i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(buf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, 0x777777ff);
        u32 args[4];
        // zero via a variable: MWCC materializes it in a register and cannot
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
        u32 tex = ((CItemNameLookupVt*)info->state.arcResourceAccessor)->findName(0x74696D67, (u32)(base + 0x341), 0);
        if (tex != 0) {
            func_80137E7C((nw4r::lyt::Layout*)info->state.layout, buf, tex);
        }
    }
}
void func_801E3DE4(CItemBoxInfo2* info) {
    char buf[0x20];
    for (int i = 0; i < 3; i++) {
        int index = (u8)i + 1;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x35f], index);
        void* layout = *(void**)((u8*)info + 0x34);
        void* r = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(buf, true);
        func_80124270(r, 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x408], index);
        func_80136B4C((nw4r::lyt::Layout*)layout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x426], index);
        func_80136B4C((nw4r::lyt::Layout*)layout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
    }
}
void func_801E3EB8(CItemBoxInfo2* info) {
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, base + 0x44f, base + 0x2aa, 0);
    char buf[0x20];
    for (u32 i = 0; i < (u8)code80135FDC_getByte_64077(); i++) {
        u32 idx = (u8)i + 1;
        sprintf(buf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)info->state.layout, buf, 0x777777ff);
        u32 args[4];
        // zero via a variable: MWCC materializes it in a register (r30-like)
        // and cannot fold the {zero, zero} s16-pair arrays to a constant,
        // keeping the retail stack build of the four u32 colour args.
        s16 zero = 0;
        s16 c0hi[2] = {zero, *(s16*)((u8*)info + 0xA2)};
        s16 c0lo[2] = {zero, zero};
        s16 c1hi[2] = {zero, *(s16*)((u8*)info + 0xAA)};
        s16 c1lo[2] = {zero, zero};
        args[1] = *(u32*)c0hi;
        args[0] = *(u32*)c0lo;
        args[3] = *(u32*)c1hi;
        args[2] = *(u32*)c1lo;
        sprintf(buf, base + 0x161, idx, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            void* obj = ((void*(*)(void*))(*(void***)pane)[26])(pane);
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8(obj, (u8)j, (u8*)args + (u8)j * 8);
            }
        }
    }
}
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

void func_801E4090(CItemBoxInfo2* info) {
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x48f],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x49b],
                  &lbl_eu_805063BC[0x2aa], 0);
}
// Retail func_801E40E8 uses the stmw/lmw frame (MWCC optimize_for_size
// prologue merge): info in r29, loop counter in r30, string-pool base in r31.
#pragma push
#pragma optimize_for_size on
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
// name via func_801355F4/vtable[3] and colors the pane; otherwise a plain
// label pane.
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
extern "C" void func_801E4194(CItemBoxInfo2* info) {
    u8 count = (u8)code80135FDC_getByte_64077();
    char* base = (char*)&lbl_eu_805063BC;
    char bufElse[0x20];
    char buf[0x20];
    u32 args[4];
    for (u32 i = 0; i < 7; i++) {
        if ((u8)i >= count) {
            sprintf(bufElse, base + 0x161, (u8)i + 1);
            nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(bufElse, true);
            func_80124270(pane, 0);
            continue;
        }
        u8 slot = (u8)func_801392B4((u8)i);
        u16 nameId = (u16)func_80136254(lbl_eu_80664090, base + 0x4ce, slot);
        u32 itemName = func_80138F78(nameId);
        void* obj = func_801355F4();
        u32 result = ((u32(*)(void*, u32, u32, u32))(*(void***)obj)[3])(obj, 0x74696D67u, itemName, 0);
        if (result == 0) continue;
        sprintf(buf, base + 0x303, (u8)i + 1);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        func_80137F88(pane, result);
        func_80137C1C(pane, 0x777777FFu);
        s16 c0hi[2] = {0, *(s16*)((u8*)info + 0xA2)};
        s16 c0lo[2] = {0, 0};
        s16 c1hi[2] = {0, *(s16*)((u8*)info + 0xAA)};
        s16 c1lo[2] = {0, 0};
        args[1] = *(u32*)c0hi;
        args[0] = *(u32*)c0lo;
        args[3] = *(u32*)c1hi;
        args[2] = *(u32*)c1lo;
        sprintf(buf, base + 0x161, (u8)i + 1, args[0], args[1], args[2], args[3]);
        nw4r::lyt::Pane* pane2 = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        void* vertexObj = pane2->GetMaterial();
        for (u32 j = 0; j < 2; j++) {
            func_801D62F8(vertexObj, (u8)j, (u8*)args + (u8)j * 8);
        }
    }
}
#pragma pop
// Activates the panel: once both the layout (+0x34) and its resource
// accessor (+0x30) are present, set the current/active bytes. Goto-gate
// layout matches retail's branch-over-branch shape (bne over a blr with
// the body placed after the return — see CTitle func_802B64AC family).
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
// func_801E43BC — ItemBox2 info-panel render (retail 0x801E5FB8, 0x4DA8 bytes)
// Reconstructed from retail disassembly (build/us/asm/kyoshin/CItemBoxInfo.s).
// NOTE: arg3 (r5) is dead in retail — clobbered by the prologue (lis r5, 0x4330).
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
struct E43StatC {          // vtable[0x22C] result — s16 stat block
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
struct E43StatA {          // vtable[0x228] result — s16 stat block + f32
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
struct E43StatB {          // vtable[0x20C] result — f32 block
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
extern "C" void copyVEC3(void*, void*);

extern "C" void func_801E43BC(CItemBoxInfo2* info, u16 arg2, void* arg3, u16 arg4, u32 arg5) {
    // ---- type (main switch key) ----
    u8 type = (u8)func_801392E4((void*)(u32)arg2);

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
    u32 t1 = func_801E92B8(a, b);
    u32 t2 = func_801E9310(a, b, 0x52, d);
    s32 sum = (s32)(t2 + t1 + 0x64);
    union { double d; u32 w[2]; } u;
    u.w[1] = (u32)(arg2 * sum) ^ 0x80000000;
    u.w[0] = 0x43300000;
    return (s32)(lbl_eu_80668040 * (float)(u.d - lbl_eu_80668028));
}
#pragma pop

#pragma push
#pragma auto_inline off
s32 func_801E9224(void* a, void* b, s32 arg2, void* d) {
    u32 t1 = func_801E92B8(a, b);
    u32 t2 = func_801E9310(a, b, 0x53, d);
    s32 sum = (s32)(t2 + t1 + 0x64);
    union { double d; u32 w[2]; } u;
    u.w[1] = (u32)(arg2 * sum) ^ 0x80000000;
    u.w[0] = 0x43300000;
    return (s32)(lbl_eu_80668040 * (float)(u.d - lbl_eu_80668028));
}
#pragma pop
#pragma push
#pragma auto_inline off
// Retail func_801E9310: the ItemBox2 variant of func_801DF610. Six slots are
// scanned via the lookup table (s16 ids at +0x26/+0x1C..0x24, 6-byte slot
// table from .sdata2). When the slot's byte equals the candidate's equip id
// ((u8)func_801392E4(arg3)), a name-based lookup runs instead of the item
// walk: count = func_801361E8(v1, base+0x432, v2), then for each index the
// name v = func_80136254(v1, buf, v2) resolves a category (func_80139358)
// matching the requested item id, and the name-count byte is added. The
// id==-1 slot only runs the name lookup. Result is clamped by the table cap.
u32 func_801E9310(void* a, void* b, u32 c, void* d) {
    void* lookup = func_8009EC9C((u32)b);
    u32 v1 = func_801393CC(d);
    u32 v2 = func_80139358((u32)d);
    u8 v3 = (u8)func_801392E4(d);
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
            if (d != NULL && (s32)slotByte == (s32)v3) {
                // candidate slot: name-based lookup over the item names
                u8 count = (u8)func_801361E8(v1, base + 0x432, (u16)v2);
                for (u8 j = 0; (u8)j < count; j++) {
                    sprintf(buf, base + 0x43b, (u8)j + 1);
                    u32 v = func_80136254((char*)v1, buf, (u16)v2);
                    if ((u16)v != 0) {
                        u16 cat = func_80139358((u16)v);
                        if ((u32)c == (u32)cat) {
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
                            u16 cat = inst3->_v54(r);
                            if ((u32)c == (u32)cat) {
                                CItemImplVt* inst4 = (CItemImplVt*)CItem_initItemImplInstances(r);
                                result += inst4->_v90(r);
                            }
                        }
                    } else {
                        CItemImplVt* inst5 = (CItemImplVt*)CItem_initItemImplInstances(item);
                        CItemBoxSubRecord* sub = inst5->_v2C(item, (u8)j);
                        if (sub != NULL) {
                            u16 cat2 = (sub->field_04 >> 4) & 0xFFF;
                            if ((u32)c == (u32)cat2) {
                                s32 val2 = (sub->_00 >> 10) & 0x7FF;
                                result += (s16)val2;
                            }
                        }
                    }
                }
            }
        } else {
            if (d != NULL && (s32)bytes.bytes[(u8)slot] == (s32)v3) {
                // candidate slot: name-based lookup
                u8 count = (u8)func_801361E8(v1, base + 0x432, (u16)v2);
                for (u8 j = 0; (u8)j < count; j++) {
                    sprintf(buf, base + 0x43b, (u8)j + 1);
                    u32 v = func_80136254((char*)v1, buf, (u16)v2);
                    if ((u16)v != 0) {
                        u16 cat = func_80139358((u16)v);
                        if ((u32)c == (u32)cat) {
                            result += (u8)func_801361E8((u32)lbl_eu_806640EC, base + 0x447, (u16)v);
                        }
                    }
                }
            }
        }
    }
    u16 cap = (u16)func_80136254((char*)lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x503], c);
    if ((s32)cap < (s32)result) result = cap;
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
    u16 cat = func_80139358(arg2);
    u8 result = func_801361E8(g, buf, cat);
    return result != 0;
}
#pragma pop
#pragma push
#pragma auto_inline off
u32 func_801E9774(void* global, u16 arg2, void* arg3) {
    if (arg2 == 0) return 0;
    void* g = lbl_eu_806640F8;
    u16 v1 = arg3 ? (u16)func_801392E4(arg3) : (u16)0;
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
bool func_801E98E4(void* a, u16 b, void* c) {
    void* lookup = func_8009EC9C(b);
    u32 v1 = func_801393CC(c);
    u32 v2 = func_80139358((u32)c);
    u8 v3 = (u8)func_801392E4(c);
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
    // 6-byte slot table copied from two .sdata2 constants (different pair
    // from func_801DFFB8).
    bytes.ab.a = lbl_eu_8066807C;
    bytes.ab.b = lbl_eu_80668080;
    char* base = (char*)&lbl_eu_805063BC;
    char buf[0x20];
    for (u8 i = 0; i < 6; i++) {
        s16 id = ids[i];
        if (id != -1) {
            u8 slot = bytes.bytes[i];
            void* item = func_80157C4C(slot, id);
            if (c != NULL && slot == v3) {
                // candidate matches the equipped slot: verify every indexed
                // item name still resolves, else the comparison is invalid.
                u8 count = (u8)func_801361E8(v1, base + 0x432, (u16)v2);
                for (u8 j = 0; j < count; j++) {
                    sprintf(buf, base + 0x43b, (u8)j + 1);
                    if ((func_80136254((void*)v1, buf, (u16)v2) & 0xFFFF) != 0) return 0;
                }
            } else if (item != NULL && *(u32*)item != 0) {
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
            if (c != NULL && bytes.bytes[i] == v3) {
                u8 count = (u8)func_801361E8(v1, base + 0x432, (u16)v2);
                for (u8 j = 0; j < count; j++) {
                    sprintf(buf, base + 0x43b, (u8)j + 1);
                    if ((func_80136254((void*)v1, buf, (u16)v2) & 0xFFFF) != 0) return 0;
                }
            }
        }
    }
    return 1;
}
#pragma pop

bool CItemBoxInfo2::OnFileEvent(CEventFile* file) {
    return false;
}

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
    u8 kind = (u8)func_801392E4((void*)(u32)slot);
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
                      (*(u8*)((u8*)info + 0x9A) > 2) ? arg2 : (u16)-1);
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
    *(GXColorS10Copy*)dst = *(const GXColorS10Copy*)src;
}

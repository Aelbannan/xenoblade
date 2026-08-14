// Auto-scaffolded catalog TU for kyoshin/CItemBoxInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "monolib/util.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>

// Retail is 2-arg (category index + s16 item id, see CEquipChange.hpp).
extern "C" void* func_80157C4C(u32 index, s16 value);
// Legacy 1-arg form (category 0) used by not-yet-matched reconstructions.
static inline void* func_80157C4C_1(u32 id) { return func_80157C4C(0, (s16)id); }

// --- Forward declarations ---
namespace nw4r { namespace lyt { class Layout; class DrawInfo; class AnimTransform; } }
void func_80136B4C(nw4r::lyt::Layout*, const char*, const char*, u32);
void func_80136910(nw4r::lyt::Layout*, const char*, u8);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
int sprintf(char*, const char*, ...);
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
u32 func_8026178C(void*, u32);
u32 func_8025FB10(void*, u32);
void func_80127BD8(void*, float*);
void func_801390E0__FPP11CFileHandle(void*);
void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
void func_8045F778__17UnkClass_8045F564Fv(void*);
char* func_80136190(char*, char*, u32);
char* func_8013639C(void*, char*);
u32 func_801392B4(u32);
u32 func_801392C0();
u8 func_8013600C(void*, void*, u32);
u32 func_800A32BC();
u32 func_80136254(void*, const char*, u16);
void func_800A082C(void*);
void func_8013B380(u32);
void func_80139C98(u32);
extern void* lbl_eu_806645A8;
extern void* lbl_eu_806645B0;
void func_80139A18(nw4r::lyt::Layout*, char*, void*, void*);
void func_801D8930(CItemBoxInfo*);
void func_801D885C(CItemBoxInfo*);
void func_801D5564(void*, void*, void*, void*);
void func_801D8318(CItemBoxInfo*);
void func_801D4E2C(void*, u16, void*);
void func_801D69FC(CItemBoxInfo*);
void func_801D8A88(CItemBoxInfo*);
void func_801D77A4(void*, u32, u16);
void func_801D80EC(CItemBoxInfo*, u16, void*);
void func_801D4A2C(void*);
extern "C" void func_801D8E34(CItemBoxInfo*, u32, void*, u32);
u32 func_801C6E90(void*);
void func_801D5AA0(CItemBoxInfo*, u16, void*);
void func_801E40E8(CItemBoxInfo2*);
void func_801E3B9C(CItemBoxInfo2*);
void func_801E3EB8(CItemBoxInfo2*);
void func_801E2638(CItemBoxInfo2*, u16, void*);
void func_801E2928(CItemBoxInfo2*, u16, void*, u16);
void func_801E2C5C(CItemBoxInfo2*, u16, void*, u16);
void func_801E37C4(CItemBoxInfo2*);
extern "C" void func_801E43BC(CItemBoxInfo2*, u16, void*, u16, u32);
void func_801E3228(CItemBoxInfo2*, u16, void*, u16);
void func_801E3730(CItemBoxInfo2*, u32);
void func_801E27D0(void*, void*);
void func_801E3918(CItemBoxInfo2*);
void func_801E197C(void*, void*, void*);
void func_801E1E0C(void*, void*, void*);
void func_80137B44(nw4r::lyt::Layout*, const char*, u32);
void func_80137E7C(nw4r::lyt::Layout*, const char*);
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
void func_80137F88(void*, u16);
void func_801394D4(void*, u16);
u32 func_801E9774(void*, u16, void*);
bool func_801E98E4(void*, u16, void*);
void func_801D62F8(void*, u32, const void*);
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
void func_801E2FEC(CItemBoxInfo2*, u16);
void func_801D4260(CItemBoxInfo*, u16, void*, u16);
void func_801E14DC(CItemBoxInfo2*, u16, void*, u16, u32);
extern void* lbl_eu_80664104;
extern void* lbl_eu_806640A8;
extern void* lbl_eu_806640F4;
extern void* lbl_eu_806640EC;
extern void* func_801571FC();
extern void* lbl_eu_806640F8;
extern void* lbl_eu_806640D8;
extern void* lbl_eu_80506330;
extern float lbl_eu_80668040;
extern float lbl_eu_8066800C;
extern const float lbl_eu_80668010;
// 6-byte item-box slot tables (pair of u32+u16 .sdata2 constants).
extern const u32 lbl_eu_8066804C;
extern const u16 lbl_eu_80668050;
extern const u32 lbl_eu_8066805C;
extern const u16 lbl_eu_80668060;
extern const u32 lbl_eu_80668074;
extern const u16 lbl_eu_80668078;
extern const u32 lbl_eu_8066807C;
extern const u16 lbl_eu_80668080;
u32 getLanguage__9CDeviceSCFv();
extern "C" void* CItem_initItemImplInstances(void*);
u32 func_801393CC(void*);
extern void* lbl_eu_80664110;
u32 getHandleMEM2__Q23mtl10MemManagerFv();
void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
void setHandleFlag1__11CDeviceFileFP11CFileHandle(void*);

void resetCItemBox() {}

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

void func_801D8C0C(CItemBoxInfo*);
void func_801E4194(CItemBoxInfo2*);

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

void func_801D4A2C(void* sub) {
    float f = lbl_eu_8066800C;
    for (int i = 0; i < 12; i++) {
        ((s16*)sub)[i] = 0;
        float temp[3] = {f, f, f};
        func_80127BD8((u8*)sub + 0x18 + i * 0xC, temp);
    }
}


void func_801D4054(CItemBoxInfo* info) {
    u32 handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
    info->state.fileHandle1 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, &lbl_eu_805063BC[0x8e], info, 0, 0);
    setHandleFlag1__11CDeviceFileFP11CFileHandle(info->state.fileHandle1);
    handle = getHandleMEM2__Q23mtl10MemManagerFv();
    info->state.fileHandle2 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, &lbl_eu_805063BC[0xa6], info, 0, 0);
    setHandleFlag1__11CDeviceFileFP11CFileHandle(info->state.fileHandle2);
}
void func_801D4174(CItemBoxInfo* info) {
    func_801390E0__FPP11CFileHandle(&info->state.fileHandle1);
    func_801390E0__FPP11CFileHandle(&info->state.fileHandle2);
    void* layout = info->state.layout;
    info->state.active = 0;
    if (layout != 0) {
        if (layout != 0) {
            (*(void(**)(void*, u32))(*(void***)layout + 2))(layout, 1);
        }
        info->state.layout = 0;
    }
    info->state.animTransform1 = 0;
    info->state.animTransform2 = 0;
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(info->state.arcResourceAccessor);
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(info->state.resource);
    info->state.arcResourceAccessor = 0;
    info->state.resource = 0;
    func_8045F778__17UnkClass_8045F564Fv(&info->state.memRegion1);
    func_8045F778__17UnkClass_8045F564Fv(&info->state.memRegion2);
}



void func_80137924(void*, void*, void*, void*);

void func_801D4B3C(void* arg0, CItemBoxInfo* info, u32 arg2) {
    char buf[0x20];
    sprintf(buf, (char*)&lbl_eu_805063BC[0x161], arg2 + 1);
    nw4r::lyt::Pane* child = (nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10);
    void* r1 = child->FindPaneByName((char*)&lbl_eu_805063BC[0x16e], true);
    void* r2 = child->FindPaneByName(buf, true);
    func_80137924(arg0, r2, r1, child);
}

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
    if (func_80137444((nw4r::lyt::AnimTransform*)info->state.animTransform2, -0.0f) != 0) {
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

void func_801D4E2C(void* out, u16 arg2, void* arg3) {
    if (arg3 == NULL) return;
    u16 v1 = func_801392E4(arg3);
    u16 v2 = func_80139358((u32)arg3);
    char buf[0x20];
    func_80136254(buf, (char*)&lbl_eu_805063BC[0x1ab], v2);
    func_80136254(buf, (char*)&lbl_eu_805063BC[0x1b3], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x193], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1eb], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], v2);
    void* lookup = func_8009EC9C(v2);
    func_800A082C(lookup);
    func_800A082C(lookup);
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 v = func_801392B4(i);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], v);
        func_801361E8((u32)lbl_eu_806640F8, buf, v2);
        func_8013B380(v);
        void* lookup2 = func_8009EC9C(v);
        void* r = func_80157C4C_1(v + 4);
        if (r != NULL && *(u32*)r != 0) {
            u16 cat = func_80139358(*(u32*)r >> 20);
            func_80136254(buf, (char*)&lbl_eu_805063BC[0x1f4], cat);
            func_801361E8((u32)lbl_eu_806640F8, buf, v2);
        }
        func_80139C98(v);
        func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], v2);
        func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
        func_80139C98(v + 1);
    }
}
void func_801D5274(void* out, u16 arg2, void* arg3) {
    if (arg3 == NULL) {
        memset(out, 0, 0x1C);
        return;
    }
    u16 v1 = func_801392E4(arg3);
    u16 v2 = func_80139358((u32)arg3);
    ((u16*)out)[0] = (u16)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
    ((u16*)out)[1] = (u16)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], v2);
    ((u16*)out)[2] = (u16)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1eb], v2);
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 v = func_801392B4(i);
        char buf[0x10];
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], v);
        ((u8*)out)[i] = func_801361E8((u32)lbl_eu_806640F8, buf, v2);
        void* lookup = func_8009EC9C(v);
        u32 t1 = func_8026178C(lookup, v2);
        u32 t2 = func_8026178C(lookup, v2);
        ((u32*)((u8*)out + 4))[i] = t1;
        ((u32*)((u8*)out + 8))[i] = t2;
        void* r = func_80157C4C_1(v);
        if (r != NULL && *(u32*)r != 0) {
            u16 cat = func_80139358(*(u32*)r >> 20);
            ((u8*)out)[i + 12] = func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], cat);
        }
    }
}
void func_801D5564(void* out, void* unused, void* data, void* arg3) {
    if (arg3 == NULL) return;
    u16 v1 = func_801392E4(data);
    u16 v2 = func_80139358((u32)data);
    void* inst = CItem_initItemImplInstances(arg3);
    u8 r = ((u8(*)(void*, void*))(*(void***)inst)[2])(inst, arg3);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    char* s1 = (char*)func_80136190((char*)&lbl_eu_805063BC[0x130], (char*)&lbl_eu_805063BC[0x139], 0x1e - (r - 1));
    char* s2 = (char*)func_80136190((char*)&lbl_eu_805063BC[0x130], (char*)&lbl_eu_805063BC[0x139], 0);
    void* inst2 = CItem_initItemImplInstances(arg3);
    for (u32 i = 0; i < 3; i++) {
        char buf[0x20];
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], i);
        func_801361E8((u32)lbl_eu_806640F8, buf, v2);
        char* t = func_8013639C(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x139]);
        ((u32*)((u8*)out + 0xC))[i] = (u32)t;
    }
}

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

void func_801D5AA0(CItemBoxInfo* info, u16 arg2, void* arg3) {
    void* global = lbl_eu_80664110;
    u16 v1 = func_801392E4(arg3);
    u16 v2 = func_80139358((u32)arg3);
    u8 flag1 = func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x22b], v2);
    u8 count = func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x237], v2);
    u8 arr[4] = {flag1, 0, 0, 0};
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 a = func_801392B4(i);
        arr[i + 1] = (a != count ? 1 : 0);
    }
    u8 r5 = func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x23f], v2);
    u8 val = func_8013600C((char*)&lbl_eu_805063BC[0x248], (char*)&lbl_eu_805063BC[0x250], r5);
    void* lookup = func_8009EC9C(count);
    u8 cat = func_800A32BC();
    u8* entry = (u8*)lookup + cat * 0x49 + val * 2;
    u8 flag2 = 0;
    switch (arr[0]) {
        case 1: if (entry[0xE8] != 0) flag2 = 1; break;
        case 2: if ((entry[0xE9] >> 0) & 1) flag2 = 1; break;
        case 3: if ((entry[0xE9] >> 1) & 1) flag2 = 1; break;
    }
    ((u32*)info)[0] = (arr[0]) | (arr[1] << 8) | (arr[2] << 16) | (arr[3] << 24);
    ((u32*)info)[1] = 0;
    ((u8*)info)[8] = flag2;
}
void func_801D5C38(void* out, void* unused, void* data, void* arg3) {
    void* item = arg3 ? arg3 : NULL;
    u16 v1 = func_801392E4(data);
    u16 v2 = func_80139358((u32)data);
    void* inst = CItem_initItemImplInstances(item);
    u8 r = ((u8(*)(void*, void*))(*(void***)inst)[2])(inst, item);
    u8 buf[0x30];
    buf[0] = r;
    char* s = (char*)func_80136190((char*)&lbl_eu_805063BC[0x130], (char*)&lbl_eu_805063BC[0x139], 0x1e - (r - 1));
    ((u32*)(buf + 4))[0] = (u32)s;
    u8 count = 0;
    buf[0x21] = 0;
    for (u32 i = 0; i < 4; i++) {
        void* inst2 = CItem_initItemImplInstances(item);
        u16 n = ((u16(*)(void*, void*, u32))(*(void***)inst2)[19])(inst2, item, i);
        if (n > 0) {
            char* t = func_8013639C(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x139]);
            ((u32*)(buf + 8))[count] = (u32)t;
            void* inst3 = CItem_initItemImplInstances(item);
            u8 val = ((u8(*)(void*, void*, u32))(*(void***)inst3)[25])(inst3, item, i);
            buf[0x1C + count] = val;
            ((u16*)(buf + 0x22))[count] = n;
            count++;
        }
    }
    u32* dst = (u32*)((u8*)out - 4);
    u32* src = (u32*)(buf + 4);
    for (int i = 0; i < 5; i++) {
        *++dst = *++src;
    }
    *++dst = *++src;
}
void func_801D5DA4(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4) {
    func_801D8318(info);
    func_801D4E2C(info, arg2, arg3);
    func_801D69FC(info);
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

void func_801D62F8(void* arr, u32 index, const void* color) {
    if (index >= 3) {
        Panic__Q24nw4r2dbFPCciPCce(lbl_eu_8052E590, 0x8f, lbl_eu_8052E568);
    }
    __as__11_GXColorS10FRC11_GXColorS10((u8*)arr + index * 8 + 0x10, color);
}
void func_80137924(void*, void*, void*, void*);
void func_801D3FF0(void*);
void __dl__FPv(void*);

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

void func_801D6394(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4) {
    func_801D85D8(info);
    func_801D69FC(info);
    void* layout = info->state.layout;
    char* base = (char*)&lbl_eu_805063BC;
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2c1, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2cc, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2d7, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2e2, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x267, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x273, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x29e, base + 0x2aa, 0);
    func_80136190(base + 0x130, base + 0x139, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x303, base + 0x2aa, 0);
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 v = func_801392B4(i);
        func_80136910((nw4r::lyt::Layout*)layout, base + 0x1f4, 0);
        func_80136190(base + 0x130, base + 0x139, v);
    }
    func_801D5274(layout, arg2, arg3);
}
void func_801D69FC(CItemBoxInfo* info) {
    func_801D885C(info);
    char buf[0x80];
    void* layout = info->state.layout;
    nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName((char*)&lbl_eu_805063BC[0x1f4], true);
    if (pane != NULL) {
        func_80124270(pane, 0);
    }
    void* r = func_80157C4C_1(0);
    if (r != NULL && *(u32*)r != 0) {
        func_801D5564(info, 0, r, info);
    }
    for (int i = 0; i < 3; i++) {
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], i);
        func_80136B4C((nw4r::lyt::Layout*)layout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
        char* s = (char*)func_80136190(buf, (char*)&lbl_eu_805063BC[0x130], 0);
        func_80136B4C((nw4r::lyt::Layout*)layout, s, (char*)&lbl_eu_805063BC[0x2aa], 0);
        void* inst = CItem_initItemImplInstances(info);
        u16 v = func_801393CC(inst);
        func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v);
        func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v);
        func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], v);
        func_801394D4(layout, v);
        func_801D5564(info, 0, (void*)(u32)v, info);
        char* t = func_8013639C(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x139]);
        func_80136B4C((nw4r::lyt::Layout*)layout, t, (char*)&lbl_eu_805063BC[0x2aa], 0);
        func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_805063BC[0x2ab], (char*)&lbl_eu_805063BC[0x2aa], 0);
        func_80137F88(layout, v);
        func_80127BD8(layout, (float*)&lbl_eu_80668010);
        func_80124270(pane, (u32)i);
    }
}

void CItemBoxInfo::setItemBoxIndex(unsigned char index, short value) {
    if (index >= 12) return;
    state.values[index] = value;
}

void func_801D77BC(CItemBoxInfo* info, u16 arg2) {
    func_801D8930(info);
    func_801D85D8(info);
    u32 buf[2];
    func_801D5AA0(info, 0, (void*)0);
    void* layout = info->state.layout;
    char* base = (char*)&lbl_eu_805063BC;
    char tmp[0x20];
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        sprintf(tmp, base + 0x303, i + 1);
        func_80136B4C((nw4r::lyt::Layout*)layout, tmp, base + 0x2aa, 0);
        func_80137B44((nw4r::lyt::Layout*)layout, tmp, 0x777777ff);
        void* child = *(void**)((u8*)layout + 0x10);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)child)->FindPaneByName(tmp, true);
        if (pane != NULL) {
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8((u8*)pane + 0x10, j, tmp);
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
    void* layout = info->state.layout;
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x44f, base + 0x2aa, 0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2c1, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2cc, &lbl_eu_806645A8, &lbl_eu_806645B0);
    char buf[0x20];
    u32 max = func_801392C0();
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
extern "C" void func_801D85D8(CItemBoxInfo* info) {
    void* layout = info->state.layout;
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x267, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x273, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x29e, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x354, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x2ab, base + 0x2aa, 0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2c1, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2cc, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2d7, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2e2, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2ed, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2f8, &lbl_eu_806645A8, &lbl_eu_806645B0);
    char buf[0x20];
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        sprintf(buf, base + 0x303, i + 1);
        func_80137B44((nw4r::lyt::Layout*)layout, buf, 0x777777ff);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8((u8*)pane + 0x10, j, buf);
            }
        }
    }
}
void func_801D885C(CItemBoxInfo* info) {
    char buf[0x20];
    for (int i = 0; i < 3; i++) {
        int index = (u8)i + 1;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x35f], index);
        void* r = ((nw4r::lyt::Pane*)*(void**)((u8*)info->state.layout + 0x10))->FindPaneByName(buf, true);
        func_80124270(r, 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x408], index);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x426], index);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
    }
}
void func_801D8930(CItemBoxInfo* info) {
    char* base = (char*)&lbl_eu_805063BC;
    void* layout = info->state.layout;
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x44f, base + 0x2aa, 0);
    char buf[0x20];
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u32 idx = i + 1;
        sprintf(buf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)layout, buf, 0x777777ff);
        sprintf(buf, base + 0x161, idx);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8((u8*)pane + 0x10, j, buf);
            }
        }
    }
}
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

void func_801D8B60(CItemBoxInfo* info) {
    char buf[0x20];
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x4a7],
                  &lbl_eu_805063BC[0x2aa], 0);
    for (int i = 0; i < 4; i++) {
        sprintf(buf, &lbl_eu_805063BC[0x4b3], (u8)i * 2 + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, &lbl_eu_805063BC[0x4c0], (u8)i + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
    }
}

#pragma push
#pragma auto_inline off
extern "C" void func_801D8B08(CItemBoxInfo* info) {
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x48f],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x49b],
                  &lbl_eu_805063BC[0x2aa], 0);
}
#pragma pop

void func_801D8C0C(CItemBoxInfo* info) {
    void* layout = info->state.layout;
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x44f, base + 0x2aa, 0);
    char buf[0x20];
    u32 max = func_801392C0();
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
u32 func_801DF988(void*, void*, u32, void*, u32);

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
            func_801D5274(&e_cur, member, (void*)(u32)w0);
            D8EArmorEntry& c_cur = comparisonStorage.armor[1];
            c_cur = e_cur;
            D8EArmorEntry& e_new = comparisonStorage.armor[2];
            func_801D5274(&e_new, member, arg3);
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
            func_801D5274(&e_cur, member, (void*)(u32)w0);
            D8EArmorEntry c_cur = e_cur;
            D8EArmorEntry e_new;
            func_801D5274(&e_new, member, arg3);
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
            func_80137924(&quad, pane1, root, ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10)));
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
    void* r27 = a;
    void* r28 = b;
    s32 r29 = arg2;
    void* r30 = d;
    u32 r31 = func_801DFD60(r27, r28, 0x2d);
    u32 r3 = func_801DF610(r27, r28, 0x52, r30);
    s32 sum = (s32)(r3 + r31 + 0x64);
    return (s32)(lbl_eu_80668040 * (float)(r29 * sum));
}

s32 func_801DF578(void* a, void* b, s32 arg2, void* d) {
    void* r27 = a;
    void* r28 = b;
    s32 r29 = arg2;
    void* r30 = d;
    u32 r31 = func_801DFD60(r27, r28, 0x2d);
    u32 r3 = func_801DF610(r27, r28, 0x53, r30);
    s32 sum = (s32)(r3 + r31 + 0x64);
    return (s32)(lbl_eu_80668040 * (float)(r29 * sum));
}
u32 func_801DF610(void* unused, u16 lookup_key, u32 category, void* arg3) {
    void* lookup = func_8009EC9C(lookup_key);
    s16 ids[6];
    ids[0] = *(s16*)((u8*)lookup + 0x1C);
    ids[1] = *(s16*)((u8*)lookup + 0x1E);
    ids[2] = *(s16*)((u8*)lookup + 0x20);
    ids[3] = *(s16*)((u8*)lookup + 0x22);
    ids[4] = *(s16*)((u8*)lookup + 0x24);
    ids[5] = *(s16*)((u8*)lookup + 0x26);
    u32 result = 0;
    for (int i = 0; i < 6; i++) {
        s16 id = ids[i];
        if (id != -1) {
            void* item = func_80157C4C_1(id);
            if (arg3 != NULL && (*(u32*)arg3 >> 20) == id) item = arg3;
            if (item != NULL && *(u32*)item != 0) {
                void* inst = CItem_initItemImplInstances(item);
                u8 count = ((u8(*)(void*, void*))(*(void***)inst)[12])(inst, item);
                for (u32 j = 0; j < count; j++) {
                    void* inst2 = CItem_initItemImplInstances(item);
                    s16 v = ((s16(*)(void*, void*, u32))(*(void***)inst2)[16])(inst2, item, j);
                    if (v != -1) {
                        void* r = func_80157C4C_1(3);
                        if (r != NULL && *(u32*)r != 0) {
                            void* inst3 = CItem_initItemImplInstances(r);
                            u16 cat = ((u16(*)(void*, void*))(*(void***)inst3)[21])(inst3, r);
                            if (cat == category) {
                                void* inst4 = CItem_initItemImplInstances(r);
                                u32 val = ((u32(*)(void*, void*))(*(void***)inst4)[36])(inst4, r);
                                result += val;
                            }
                        }
                    } else {
                        void* inst5 = CItem_initItemImplInstances(item);
                        void* sub = ((void*(*)(void*, void*, u32))(*(void***)inst5)[11])(inst5, item, j);
                        if (sub != NULL) {
                            u16 cat2 = (*(u16*)((u8*)sub + 4) >> 4) & 0xFFF;
                            if (cat2 == category) {
                                s32 val2 = (*(u32*)sub >> 11) & 0x7FF;
                                result += (s16)val2;
                            }
                        }
                    }
                }
            }
        } else {
            if (arg3 != NULL) {
                u8 idx = *(u8*)((u8*)lookup + 0x8 + i);
                if ((*(u32*)arg3 >> 20) == idx) {
                    void* item = arg3;
                    if (item != NULL && *(u32*)item != 0) {
                        void* inst = CItem_initItemImplInstances(item);
                        u8 count = ((u8(*)(void*, void*))(*(void***)inst)[12])(inst, item);
                        for (u32 j = 0; j < count; j++) {
                            void* inst2 = CItem_initItemImplInstances(item);
                            s16 v = ((s16(*)(void*, void*, u32))(*(void***)inst2)[16])(inst2, item, j);
                            if (v != -1) {
                                void* r = func_80157C4C_1(3);
                                if (r != NULL && *(u32*)r != 0) {
                                    void* inst3 = CItem_initItemImplInstances(r);
                                    u16 cat = ((u16(*)(void*, void*))(*(void***)inst3)[21])(inst3, r);
                                    if (cat == category) {
                                        void* inst4 = CItem_initItemImplInstances(r);
                                        u32 val = ((u32(*)(void*, void*))(*(void***)inst4)[36])(inst4, r);
                                        result += val;
                                    }
                                }
                            } else {
                                void* inst5 = CItem_initItemImplInstances(item);
                                void* sub = ((void*(*)(void*, void*, u32))(*(void***)inst5)[11])(inst5, item, j);
                                if (sub != NULL) {
                                    u16 cat2 = (*(u16*)((u8*)sub + 4) >> 4) & 0xFFF;
                                    if (cat2 == category) {
                                        s32 val2 = (*(u32*)sub >> 11) & 0x7FF;
                                        result += (s16)val2;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    u32 r = func_80136254((char*)lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x503], category);
    if (result > (u16)r) result = (u16)r;
    return result;
}

u32 func_801DF988(void* info, void* member, u32 category, void* candidate, u32 slot) {
    (void)info;
    (void)member;
    (void)category;
    (void)candidate;
    (void)slot;
    void* lookup = func_8009EC9C(0);
    for (int i = 0; i < 12; i++) {
        void* r = func_80157C4C_1(i);
        if (r != NULL && *(u32*)r != 0) {
            void* inst = CItem_initItemImplInstances(r);
        }
    }
    return 0;
}
u32 func_801DFD60(void* a, void* b, u32 arg2) {
    u32 result = 0;
    void* obj = (u8*)func_8009EC9C((u32)b) + 0x3534;
    if (func_8026178C(obj, arg2) != 0) {
        result = func_8025FB10(obj, arg2);
    }
    return result;
}

#pragma push
#pragma auto_inline off
u32 func_801E9690(void* dummy, u32 arg1, u32 arg2) {
    u32 result = 0;
    void* obj = (u8*)func_8009EC9C(arg1) + 0x3534;
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
        void* r = func_80157C4C_1(ii);
        if (r == NULL) continue;
        u32 v = *(u32*)r;
        if (v == 0) continue;
        u16 cat = func_80139358(v >> 20);
        result += (u8)func_801361E8((u32)g, (char*)&lbl_eu_805063BC[0x1e2], cat);
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
            void* item = func_80157C4C_1(bytes.bytes[i]);
            if (arg3 != NULL && (*(u32*)arg3 >> 16 & 0xF) == bytes.bytes[i]) item = arg3;
            if (item != NULL && *(u32*)item != 0) {
                void* inst = CItem_initItemImplInstances(item);
                u8 count = ((u8(*)(void*, void*))(*(void***)inst)[12])(inst, item);
                for (u8 j = 0; j < count; j++) {
                    void* inst2 = CItem_initItemImplInstances(item);
                    s16 v = ((s16(*)(void*, void*, u32))(*(void***)inst2)[16])(inst2, item, j);
                    if (v != -1) {
                        void* r = func_80157C4C_1(3);
                        if (r != NULL && *(u32*)r != 0) return 0;
                    } else {
                        void* inst3 = CItem_initItemImplInstances(item);
                        void* sub = ((void*(*)(void*, void*, u32))(*(void***)inst3)[11])(inst3, item, j);
                        if (sub != NULL && (*(u16*)((u8*)sub + 4) >> 4) != 0) return 0;
                    }
                }
            }
        } else {
            if (arg3 != NULL) {
                u8 b = bytes.bytes[i];
                if ((*(u32*)arg3 >> 16 & 0xF) == b) {
                    void* item = arg3;
                    if (item != NULL && *(u32*)item != 0) {
                        void* inst = CItem_initItemImplInstances(item);
                        u8 count = ((u8(*)(void*, void*))(*(void***)inst)[12])(inst, item);
                        for (u8 j = 0; j < count; j++) {
                            void* inst2 = CItem_initItemImplInstances(item);
                            s16 v = ((s16(*)(void*, void*, u32))(*(void***)inst2)[16])(inst2, item, j);
                            if (v != -1) {
                                void* r = func_80157C4C_1(3);
                                if (r != NULL && *(u32*)r != 0) return 0;
                            } else {
                                void* inst3 = CItem_initItemImplInstances(item);
                                void* sub = ((void*(*)(void*, void*, u32))(*(void***)inst3)[11])(inst3, item, j);
                                if (sub != NULL && (*(u16*)((u8*)sub + 4) >> 4) != 0) return 0;
                            }
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
    func_801D3FF0((u8*)info + 0xB0);
    return info;
}

extern "C" CItemBoxInfo* __dt__12CItemBoxInfoFv(CItemBoxInfo* info, s32 flags) {
    if (info != NULL) {
        __dt__17UnkClass_8045F564Fv((u8*)info + 0x14, -1);
        __dt__17UnkClass_8045F564Fv((u8*)info + 0x4, -1);
        if (flags > 0) __dl__FPv(info);
    }
    return info;
}

void func_801D3FF0(void*) {}

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
    func_801D3FF0((u8*)info + 0xB0);
    return info;
}

CItemBoxInfo2* __dt__13CItemBoxInfo2Fv(CItemBoxInfo2* info, s32 flags) {
    if (info != NULL) {
        __dt__17UnkClass_8045F564Fv((u8*)info + 0x14, -1);
        __dt__17UnkClass_8045F564Fv((u8*)info + 0x4, -1);
        if (flags > 0) __dl__FPv(info);
    }
    return info;
}

void func_801E12E0(CItemBoxInfo2* info) {
    u32 handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
    info->state.fileHandle1 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, &lbl_eu_805063BC[0x8e], info, 0, 0);
    handle = getHandleMEM2__Q23mtl10MemManagerFv();
    info->state.fileHandle2 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, &lbl_eu_805063BC[0xa6], info, 0, 0);
}


void CItemBoxInfo2::drawItemBox2Layout(nw4r::lyt::DrawInfo* drawInfo) {
    if (state.active != 0) {
        func_80137038((nw4r::lyt::Layout*)state.layout, drawInfo, 0, 1);
    }
}

void func_801E13F8(CItemBoxInfo2* info) {
    func_801390E0__FPP11CFileHandle(&info->state.fileHandle1);
    func_801390E0__FPP11CFileHandle(&info->state.fileHandle2);
    void* layout = info->state.layout;
    info->state.active = 0;
    if (layout != 0) {
        if (layout != 0) {
            (*(void(**)(void*, u32))(*(void***)layout + 2))(layout, 1);
        }
        info->state.layout = 0;
    }
    info->state.animTransform1 = 0;
    info->state.animTransform2 = 0;
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(info->state.arcResourceAccessor);
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(info->state.resource);
    info->state.arcResourceAccessor = 0;
    info->state.resource = 0;
    func_8045F778__17UnkClass_8045F564Fv(&info->state.memRegion1);
    func_8045F778__17UnkClass_8045F564Fv(&info->state.memRegion2);
}

void func_801E1498(CItemBoxInfo2* info) {
    if (info->state.state != 0) return;
    info->state.state = 1;
    info->state.visible = 0;
    func_801E4194(info);
}

void CItemBoxInfo2::advanceItemBox2State() {
    if (state.state == 3) {
        state.state = 4;
        state.visible = 0;
    }
}

void func_801E14DC(CItemBoxInfo2* info, u16 arg2, void* arg3, u16 arg4, u32 arg5) {
    func_801D4A2C((u8*)info + 0xB0);
    u8 val = *(u8*)((u8*)info + 0x9A);
    if (val == 4) {
        func_801E43BC(info, arg2, arg3, arg4, arg5);
    }
    void* item = arg3 ? arg3 : NULL;
    u16 id = item ? (*(u32*)item >> 20) : 0;
    u16 id_final = arg2;
    if (val != 4) id_final = id;
    u8 r = (u8)(u32)func_801392E4((void*)(u32)id_final);
    if (item != NULL) {
        if (func_801C6E90(item) != 0 || func_801D4AB0(item) == 0) {
            r = 9;
        }
    }
    if (r - 4 <= 4) {
        func_801E3228(info, id_final, arg3, arg4);
    } else if (r == 2) {
        if (val <= 2) arg4 = -1;
        func_801E2928(info, id_final, arg3, arg4);
    } else if (r == 3) {
        if (val <= 2) arg4 = -1;
        func_801E2C5C(info, id_final, arg3, arg4);
    } else if (r == 9 || r == 10) {
        func_801E37C4(info);
    } else if (r == 13) {
        func_801E3730(info, arg2);
    }
}
void func_801E16F0(CItemBoxInfo2* info, char* arg1, char* arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x151], arg1);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, arg2, 0);
}

void func_801E174C(void* arg0, CItemBoxInfo2* info, u32 arg2) {
    char buf[0x20];
    sprintf(buf, (char*)&lbl_eu_805063BC[0x161], arg2 + 1);
    void* layout = info->state.layout;
    nw4r::lyt::Pane* child = (nw4r::lyt::Pane*)*(void**)((u8*)layout + 0x10);
    void* r1 = child->FindPaneByName((char*)&lbl_eu_805063BC[0x16e], true);
    void* r2 = child->FindPaneByName(buf, true);
    func_80137924(arg0, r2, r1, child);
}

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
void func_801E197C(void* out, void* arg2, void* arg3) {
    if (arg3 == 0) return;
    u16 v1 = func_801392E4((void*)(u32)arg3);
    u16 v2 = func_80139358((u32)arg3);
    char buf[0x20];
    func_80136254(buf, (char*)&lbl_eu_805063BC[0x1ab], v2);
    func_80136254(buf, (char*)&lbl_eu_805063BC[0x1b3], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x193], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1eb], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], v2);
    void* lookup = func_8009EC9C(v2);
    func_800A082C(lookup);
    func_800A082C(lookup);
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 v = func_801392B4(i);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], v);
        func_801361E8((u32)lbl_eu_806640F8, buf, v2);
        func_8013B380(v);
        void* lookup2 = func_8009EC9C(v);
        void* r = func_80157C4C_1(v + 4);
        if (r != NULL && *(u32*)r != 0) {
            u16 cat = func_80139358(*(u32*)r >> 20);
            func_80136254(buf, (char*)&lbl_eu_805063BC[0x1f4], cat);
            func_801361E8((u32)lbl_eu_806640F8, buf, v2);
        }
        func_80139C98(v);
        func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], v2);
        func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
        func_80139C98(v + 1);
    }
}
#pragma pop
#pragma push
#pragma auto_inline off
void func_801E1E0C(void* out, void* arg2, void* arg3) {
    if (arg3 == 0) {
        memset(out, 0, 0x1C);
        return;
    }
    u16 v1 = func_801392E4((void*)(u32)arg3);
    u16 v2 = func_80139358((u32)arg3);
    ((u16*)out)[0] = (u16)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
    ((u16*)out)[1] = (u16)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], v2);
    ((u16*)out)[2] = (u16)func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1eb], v2);
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 v = func_801392B4(i);
        char buf[0x10];
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], v);
        ((u8*)out)[i] = func_801361E8((u32)lbl_eu_806640F8, buf, v2);
        void* lookup = func_8009EC9C(v);
        u32 t1 = func_8026178C(lookup, v2);
        u32 t2 = func_8026178C(lookup, v2);
        ((u32*)((u8*)out + 4))[i] = t1;
        ((u32*)((u8*)out + 8))[i] = t2;
        void* r = func_80157C4C_1(v);
        if (r != NULL && *(u32*)r != 0) {
            u16 cat = func_80139358(*(u32*)r >> 20);
            ((u8*)out)[i + 12] = func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], cat);
        }
    }
}
#pragma pop
void func_801E20FC(void* out, void* unused, void* data, void* arg3) {
    if (arg3 == NULL) return;
    u16 v1 = func_801392E4(data);
    u16 v2 = func_80139358((u32)data);
    void* inst = CItem_initItemImplInstances(arg3);
    u8 r = ((u8(*)(void*, void*))(*(void***)inst)[2])(inst, arg3);
    func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    char* s1 = (char*)func_80136190((char*)&lbl_eu_805063BC[0x130], (char*)&lbl_eu_805063BC[0x139], 0x1e - (r - 1));
    char* s2 = (char*)func_80136190((char*)&lbl_eu_805063BC[0x130], (char*)&lbl_eu_805063BC[0x139], 0);
    void* inst2 = CItem_initItemImplInstances(arg3);
    for (u32 i = 0; i < 3; i++) {
        char buf[0x20];
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], i);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], i + 1);
        func_801361E8((u32)lbl_eu_806640F8, buf, v2);
        func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
        char* t = func_8013639C(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x139]);
        ((u32*)((u8*)out + 0xC))[i] = (u32)t;
        u8 lang = getLanguage__9CDeviceSCFv();
    }
}

void func_801E2558(u32* out, void* arg2) {
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

void func_801E2638(CItemBoxInfo2* info, u16 arg2, void* arg3) {
    void* global = lbl_eu_80664110;
    u16 v1 = func_801392E4(arg3);
    u16 v2 = func_80139358((u32)arg3);
    u8 flag1 = func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x22b], v2);
    u8 count = func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x237], v2);
    u8 arr[4] = {flag1, 0, 0, 0};
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 a = func_801392B4(i);
        arr[i + 1] = (a != count ? 1 : 0);
    }
    u8 r5 = func_801361E8((u32)global, (char*)&lbl_eu_805063BC[0x23f], v2);
    u8 val = func_8013600C((char*)&lbl_eu_805063BC[0x248], (char*)&lbl_eu_805063BC[0x250], r5);
    void* lookup = func_8009EC9C(count);
    u8 cat = func_800A32BC();
    u8* entry = (u8*)lookup + cat * 0x49 + val * 2;
    u8 flag2 = 0;
    switch (arr[0]) {
        case 1: if (entry[0xE8] != 0) flag2 = 1; break;
        case 2: if ((entry[0xE9] >> 0) & 1) flag2 = 1; break;
        case 3: if ((entry[0xE9] >> 1) & 1) flag2 = 1; break;
    }
    ((u32*)info)[0] = (arr[0]) | (arr[1] << 8) | (arr[2] << 16) | (arr[3] << 24);
    ((u32*)info)[1] = 0;
    ((u8*)info)[8] = flag2;
}
void func_801E27D0(void* out, void* arg2) {
    u16 v1 = func_801392E4(arg2);
    u16 v2 = func_80139358((u32)arg2);
    void* inst = CItem_initItemImplInstances(arg2);
    u8 r = ((u8(*)(void*, void*))(*(void***)inst)[2])(inst, arg2);
    char* s = (char*)func_80136190((char*)&lbl_eu_805063BC[0x130], (char*)&lbl_eu_805063BC[0x139], 0x1e - (r - 1));
    ((u32*)out)[0] = 0;
    ((u8*)out)[4] = 0;
    for (u32 i = 0; i < 3; i++) {
        void* inst2 = CItem_initItemImplInstances(arg2);
        u16 count = ((u16(*)(void*, void*, u32))(*(void***)inst2)[19])(inst2, arg2, i);
        if (count == 0) continue;
        char* t = func_8013639C(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x139]);
        ((u32*)((u8*)out + 8))[i * 4] = (u32)t;
        void* inst3 = CItem_initItemImplInstances(arg2);
        u8 val = ((u8(*)(void*, void*, u32))(*(void***)inst3)[25])(inst3, arg2, i);
        ((u8*)out)[i + 0x1C] = val;
    }
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
                                func_80137E7C((nw4r::lyt::Layout*)layout, label);
                            }
                        }
                    }
                }
            }
        }
    }
}
void func_801E2C5C(CItemBoxInfo2* info) {
    void* layout = *(void**)((u8*)info + 0x34);
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x44f, base + 0x2aa, 0);
    char buf[0x20];
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        sprintf(buf, base + 0x303, i + 1);
        func_80137B44((nw4r::lyt::Layout*)layout, buf, 0x777777ff);
        void* child = *(void**)((u8*)layout + 0x10);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)child)->FindPaneByName(buf, true);
        if (pane != NULL) {
            for (u32 j = 0; j < 2; j++) func_801D62F8((u8*)pane + 0x10, j, buf);
        }
    }
}
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
    func_801D8B08((CItemBoxInfo*)info);
    func_801E3918(info);
    void* layout = *(void**)((u8*)info + 0x34);
    u32 buf[4];
    func_801D59C0(buf, info, (void*)arg2);
    func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_805063BC[0x48f], (char*)buf[1], 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_805063BC[0x49b], (char*)buf[3], 0);
}
void func_801E37C4(CItemBoxInfo2* info) {
    func_801E40E8(info);
    func_801E3B9C(info);
    func_801E27D0(info, 0);
    void* layout = *(void**)((u8*)info + 0x34);
    char* base = (char*)&lbl_eu_805063BC;
    char buf[0x20];
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x1e2, base + 0x2aa, 0);
    func_80136190(buf, base + 0x130, 0);
}
void func_801E3918(CItemBoxInfo2* info) {
    void* layout = *(void**)((u8*)info + 0x34);
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x267, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x273, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x29e, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x354, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x2ab, base + 0x2aa, 0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2c1, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2cc, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2d7, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2e2, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2ed, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2f8, &lbl_eu_806645A8, &lbl_eu_806645B0);
    char buf[0x20];
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        sprintf(buf, base + 0x303, i + 1);
        func_80137B44((nw4r::lyt::Layout*)layout, buf, 0x777777ff);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10))->FindPaneByName(buf, true);
        if (pane != NULL) {
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8((u8*)pane + 0x10, j, buf);
            }
        }
    }
}
void func_801E3B9C(CItemBoxInfo2* info) {
    void* layout = *(void**)((u8*)info + 0x34);
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x44f, base + 0x2aa, 0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2b6, &lbl_eu_806645A8, &lbl_eu_806645B0);
    func_80139A18((nw4r::lyt::Layout*)layout, base + 0x2c1, &lbl_eu_806645A8, &lbl_eu_806645B0);
    char buf[0x20];
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        sprintf(buf, base + 0x303, i + 1);
        func_80137B44((nw4r::lyt::Layout*)layout, buf, 0x777777ff);
        sprintf(buf, base + 0x161, i + 1);
        void* child = *(void**)((u8*)layout + 0x10);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)child)->FindPaneByName(buf, true);
        if (pane != NULL) {
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8((u8*)pane + 0x10, j, buf);
            }
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
    void* layout = *(void**)((u8*)info + 0x34);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x44f, base + 0x2aa, 0);
    char buf[0x20];
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u32 idx = i + 1;
        sprintf(buf, base + 0x303, idx);
        func_80137B44((nw4r::lyt::Layout*)layout, buf, 0x777777ff);
        sprintf(buf, base + 0x161, idx);
        void* child = *(void**)((u8*)layout + 0x10);
        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)child)->FindPaneByName(buf, true);
        if (pane != NULL) {
            void** pvt = *(void***)pane;
            u16 count = ((u16(*)(void*))pvt[26])(pane);
            for (u32 j = 0; j < 2; j++) {
                func_801D62F8((u8*)pane + 0x10, j, buf);
            }
        }
    }
}
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

void func_801E4090(CItemBoxInfo2* info) {
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x48f],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x49b],
                  &lbl_eu_805063BC[0x2aa], 0);
}
void func_801E40E8(CItemBoxInfo2* info) {
    char buf[0x20];
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x4a7],
                  &lbl_eu_805063BC[0x2aa], 0);
    for (int i = 0; i < 4; i++) {
        sprintf(buf, &lbl_eu_805063BC[0x4b3], (u8)i * 2 + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, &lbl_eu_805063BC[0x4c0], (u8)i + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
    }
}

void func_801E4194(CItemBoxInfo2* info) {
    void* layout = *(void**)((u8*)info + 0x34);
    char* base = (char*)&lbl_eu_805063BC;
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x44f, base + 0x2aa, 0);
    char buf[0x20];
    u32 max = func_801392C0();
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
void func_801E4390(CItemBoxInfo2* info) {
    if (info->state.layout == 0) return;
    if (info->state.resource != 0) {
        info->state.current = 1;
        info->state.active = 1;
    } else {
        return;
    }
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
            func_801E1E0C(&e_cur, info, (void*)(u32)w0);
            E43Entry c_cur = e_cur;
            E43Entry e_new;
            func_801E1E0C(&e_new, info, arg3);
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
            func_801E1E0C(&e_cur, info, (void*)(u32)w0);
            E43Entry c_cur = e_cur;
            E43Entry e_new;
            func_801E1E0C(&e_new, info, arg3);
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
            func_80137924(&quad, pane1, root, ((nw4r::lyt::Pane*)*(void**)((u8*)*(void**)((u8*)info + 0x34) + 0x10)));
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
    u32 r31 = func_801E92B8(a, b);
    void* r27 = a;
    void* r28 = b;
    s32 r29 = arg2;
    void* r30 = d;
    u32 r3 = func_801E9310(r27, r28, 0x52, r30);
    s32 sum = (s32)(r3 + r31 + 0x64);
    return (s32)(lbl_eu_80668040 * (float)(r29 * sum));
}
#pragma pop

#pragma push
#pragma auto_inline off
s32 func_801E9224(void* a, void* b, s32 arg2, void* d) {
    u32 r31 = func_801E92B8(a, b);
    void* r27 = a;
    void* r28 = b;
    s32 r29 = arg2;
    void* r30 = d;
    u32 r3 = func_801E9310(r27, r28, 0x53, r30);
    s32 sum = (s32)(r3 + r31 + 0x64);
    return (s32)(lbl_eu_80668040 * (float)(r29 * sum));
}
#pragma pop
#pragma push
#pragma auto_inline off
u32 func_801E9310(void* a, void* b, u32 c, void* d) {
    void* lookup = func_8009EC9C(c);
    u32 max = func_801393CC(lookup);
    for (u32 i = 0; i < max; i++) {
        void* r = func_80157C4C_1(i);
        if (r != NULL && *(u32*)r != 0) {
            u16 cat = func_80139358(*(u32*)r >> 20);
            func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], cat);
        }
    }
    return 0;
}
#pragma pop
#pragma push
#pragma auto_inline off
u32 func_801E96F0(void* dummy, u32 arg1, u32 arg2) {
    if (arg1 == 0 || arg2 == 0) return 0;
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x1f4], arg1);
    u16 r5 = func_80139358(arg2);
    u32 result = func_801361E8((u32)lbl_eu_806640F8, buf, r5);
    return !!(u8)result;
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
        void* r = func_80157C4C_1(ii);
        if (r == NULL) continue;
        u32 v = *(u32*)r;
        if (v == 0) continue;
        u16 cat = func_80139358(v >> 20);
        result += (u8)func_801361E8((u32)g, (char*)&lbl_eu_805063BC[0x1e2], cat);
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
    u16 v1 = func_801392E4(c);
    u16 v2 = func_80139358((u32)c);
    void* lookup = func_8009EC9C(b);
    u32 max = func_801393CC(lookup);
    for (u32 i = 0; i < max; i++) {
        void* r = func_80157C4C_1(i);
        if (r != NULL && *(u32*)r != 0) {
            u16 cat = func_80139358(*(u32*)r >> 20);
            func_801361E8((u32)lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], cat);
        }
    }
    char buf[0x20];
    func_80136254(buf, (char*)&lbl_eu_805063BC[0x1f4], v2);
    return false;
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

char* func_801D3C74(void* item_data, u8 index) {
    if (index >= 12) return NULL;
    u16 item_id = *(u16*)((u8*)item_data + index * 2);
    if (item_id == 0) return NULL;
    u8 category = *(u8*)((u8*)item_data + index + 0xB4);
    s16 val = *(s16*)((u8*)item_data + index * 2 + 0xC0);
    char buf[0x80];
    sprintf(buf, (char*)&lbl_eu_805063BC, val);
    func_801361E8((u32)lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x3], item_id);
    func_8013639C(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0xC]);
    char* result = (char*)item_data + 0xD9;
    sprintf(result, (char*)&lbl_eu_805063BC[0x18], 0);
    u8 is_jp = 0;
    u8 lang = getLanguage__9CDeviceSCFv();
    if (lang == 3 || lang == 2) is_jp = 1;
    char* scan = result;
    char temp_buf[0x20];
    char* format_base = (char*)&lbl_eu_805063BC;
    char* alt_base = (char*)&lbl_eu_80506330;
    while (*scan) {
        if (*scan == '$') {
            scan++;
            if (*scan == '1' || *scan == '2') {
                memset(temp_buf, 0, 0x20);
                // Format based on scan character
            }
        }
        scan++;
    }
    return result;
}

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

void func_801D4260(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4) {
    char* base = (char*)&lbl_eu_805063BC;
    void* layout = info->state.layout;
    void* pane;
    void* r12;
    #define FP(_off, _a2) do { \
        void* r3 = *(void**)((u8*)layout + 0x10); \
        r12 = *(void**)((u8*)r3 + 0); \
        r12 = *(void**)((u8*)r12 + 0x3C); \
        pane = ((void*(*)(void*,char*,u32))r12)(r3, base + _off, 1); \
        func_80124270(pane, (u32)(_a2)); \
    } while(0)
    FP(0xc2, 0x0);
    FP(0xce, 0x0);
    FP(0xda, 0x0);
    FP(0xe6, 0x0);
    FP(0xf2, 0x0);
    FP(0xfe, 0x0);
    FP(0x10b, 0x0);
    FP(0x113, 0x0);
    FP(0x121, 0x0);
    FP(0xfe, 0x1);
    FP(0x121, 0x1);
    FP(0x113, 0x1);
    FP(0xc2, 0x1);
    FP(0x10b, 0x1);
    FP(0xda, 0x1);
    FP(0xc2, 0x1);
    FP(0x10b, 0x1);
    FP(0xce, 0x1);
    FP(0xe6, 0x1);
    FP(0xf2, 0x1);
    FP(0x113, 0x1);
    FP(0xc2, 0x1);
    FP(0x10b, 0x1);
    FP(0xda, 0x1);
    FP(0xc2, 0x1);
    FP(0x10b, 0x1);
    FP(0xce, 0x1);
    FP(0xe6, 0x1);
    FP(0xf2, 0x1);
    #undef FP
}

void func_801D47D4(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4) {
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)info->state.layout;
    char* base = (char*)&lbl_eu_805063BC;
    func_801D4A2C((u8*)info + 0xB0);
    u8 mode = *(u8*)((u8*)info + 0x9A);
    if (mode == 4) {
        func_801D8E34(info, arg2, arg3, arg4);
    }
    void* item = arg3 ? arg3 : NULL;
    u16 r29 = arg2;
    if (item && mode == 4) {
        r29 = *(u32*)item >> 20;
    }
    u8 r27 = (u8)func_801392E4((void*)(u32)r29);
    if (item != NULL) {
        if (func_801C6E90(item) != 0 || func_801D4AB0(item) == 0) {
            r27 = 9;
        }
    }
    u16 cond = (mode <= 2 && arg2 != 0) ? arg2 : (u16)-1;
    if (r27 - 4 <= 4) {
        func_801D6394(info, r29, arg3, cond);
    } else if (r27 == 2) {
        func_801D5DA4(info, r29, arg3, cond);
    } else if (r27 == 3) {
        func_801D79F8(info, r29, arg3, cond);
    } else if (r27 == 9) {
        func_801D80EC(info, r29, arg3);
    } else if (r27 == 0xA) {
        func_801D8058(info, r29);
    } else if (r27 == 0xD) {
        func_801D77BC(info, r29);
    } else {
        func_801D8318(info);
        func_801D85D8(info);
        func_801D885C(info);
        func_801D8930(info);
        func_801D8A88(info);
        func_801D8B08(info);
        func_801D8B60(info);
    }
    char* s = (char*)func_80136190(&lbl_eu_805063BC[0x130], &lbl_eu_805063BC[0x139], 3);
    char buf[0x20];
    buf[0] = 0;
    *(u32*)(buf + 0x20) = 0;
    func_80136B4C(layout, &lbl_eu_805063BC[0x143], buf, 0);
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

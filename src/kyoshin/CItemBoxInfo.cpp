// Auto-scaffolded catalog TU for kyoshin/CItemBoxInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CItemBoxInfo.hpp"

// --- Forward declarations ---
namespace nw4r { namespace lyt { class Layout; class DrawInfo; class AnimTransform; } }
extern "C" char lbl_eu_805063BC[];
void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
void func_80136910(nw4r::lyt::Layout*, const char*, u8);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
int sprintf(char*, const char*, ...);
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void* func_8009EC9C(u16);
u32 func_8026178C(void*, u32);
u32 func_8025FB10(void*, u32);
void func_80124270(void*, void*);
void func_80127BD8(void*, float*);
void func_801390E0__FPP11CFileHandle(void*);
void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
void func_8045F778__17UnkClass_8045F564Fv(void*);
u32 func_801361E8(void*, char*, u32);
char* func_80136190(char*, char*, u32);
char* func_8013639C(void*, char*);
u16 func_80139358(u32);
u32 func_801392E4(void*);
void* func_80157C4C(u32);
u32 func_801392B4(u32);
u32 func_801392C0();
u8 func_8013600C(void*, void*, u32);
u32 func_800A32BC();
void func_80136254(char*, char*, u16);
void func_800A082C(void*);
void func_8013B380(u32);
void func_80139C98(u32);
extern void* lbl_eu_806645A8;
extern void* lbl_eu_806645B0;
void func_80139A18(nw4r::lyt::Layout*, char*, void*, void*);
void func_801D8B08(CItemBoxInfo*);
void func_801D85D8(CItemBoxInfo*);
void func_801E3918(CItemBoxInfo2*);
void func_80137B44(nw4r::lyt::Layout*, const char*, u32);
void func_801D62F8(void*, u32, const void*);
void func_801D59C0(u32*, void*, void*);
extern void* lbl_eu_80664104;
extern void* lbl_eu_806640A8;
extern void* lbl_eu_806640F8;
extern void* lbl_eu_806640D8;
extern void* lbl_eu_80506330;
u32 getLanguage__9CDeviceSCFv();
void* CItem_initItemImplInstances(void*);
extern void* lbl_eu_80664110;
void func_801D1F9C(void*, u32);
void func_801C4B60(void*, u32, u32, u32, u32);
void __as__11_GXColorS10FRC11_GXColorS10(void* dst, const void* src);
void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);

u32 getHandleMEM2__Q23mtl10MemManagerFv();
void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
void setHandleFlag1__11CDeviceFileFP11CFileHandle(void*);

void resetCItemBox() {}

// --- CItemBoxInfo methods ---

u8 CItemBoxInfo::getItemBoxState() {
    return state.current;
}

void CItemBoxInfo::advanceItemBoxState() {
    if (state.state == 3) {
        state.state = 4;
        state.visible = 0;
    }
}

void CItemBoxInfo::tryActivateItemBox() {
    if (state.layout == 0) return;
    if (state.resource) {
        state.current = 1;
        state.active = 1;
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
    float f = -0.0f;
    for (int i = 0; i < 12; i++) {
        ((s16*)sub)[i] = 0;
        float temp[3] = {f, f, f};
        func_80127BD8((u8*)sub + 0x18 + i * 0xC, temp);
    }
}


void func_801D4054(CItemBoxInfo* info) {
    u32 handle = getHandleMEM2__Q23mtl10MemManagerFv();
    info->state.fileHandle1 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, &lbl_eu_805063BC[0x8e], info, 0, 0);
    setHandleFlag1__11CDeviceFileFP11CFileHandle(info->state.fileHandle1);
    handle = getHandleMEM2__Q23mtl10MemManagerFv();
    info->state.fileHandle2 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, &lbl_eu_805063BC[0xa6], info, 0, 0);
    setHandleFlag1__11CDeviceFileFP11CFileHandle(info->state.fileHandle2);
}
void func_801D4174(CItemBoxInfo* info) {
    func_801390E0__FPP11CFileHandle(&info->state.fileHandle1);
    func_801390E0__FPP11CFileHandle(&info->state.fileHandle2);
    info->state.active = 0;
    if (info->state.layout != 0) {
        void** vtable = *(void***)info->state.layout;
        ((void(*)(void*, u32))vtable[2])(info->state.layout, 1);
        info->state.layout = 0;
    }
    info->state.animTransform1 = 0;
    info->state.animTransform2 = 0;
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(info->state.arcResourceAccessor);
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(&info->state.resource);
    info->state.arcResourceAccessor = 0;
    info->state.resource = 0;
    func_8045F778__17UnkClass_8045F564Fv(&info->state.memRegion1);
    func_8045F778__17UnkClass_8045F564Fv(&info->state.memRegion2);
}



void func_80137924(void*, void*, void*, void*);

void func_801D4B3C(void* arg0, CItemBoxInfo* info, u32 arg2) {
    char buf[0x20];
    sprintf(buf, (char*)&lbl_eu_805063BC[0x161], arg2 + 1);
    void* layout = info->state.layout;
    void* child = *(void**)((u8*)layout + 0x10);
    void** vtable = *(void***)child;
    void* r1 = ((void*(*)(void*, const char*, u32))vtable[15])(child, (char*)&lbl_eu_805063BC[0x16e], 1);
    void* r2 = ((void*(*)(void*, const char*, u32))vtable[15])(child, buf, 1);
    func_80137924(arg0, r2, r1, child);
}

void func_801D4C3C(CItemBoxInfo* info, void* arg2) {
    if (info->state.layout == 0) return;
    void* child = *(void**)((u8*)info->state.layout + 0x10);
    void* result = ((void*(*)(void*, const char*, u32))(*(void***)child)[15])(child, &lbl_eu_805063BC[0x193], 1);
    func_80124270(result, arg2);
}

void func_801D4C9C(CItemBoxInfo* info) {
    if (func_80137444((nw4r::lyt::AnimTransform*)info->state.animTransform1, -0.0f) != 0) {
        void* layout = info->state.layout;
        void** vtable = *(void***)layout;
        ((void(*)(void*, void*, u32))vtable[11])(layout, info->state.animTransform1, 0);
        ((void(*)(void*, void*, u32))vtable[11])(layout, info->state.animTransform2, 1);
        info->state.state = 2;
    }
}

void func_801D4D18(CItemBoxInfo* info) {
    if (func_80137444((nw4r::lyt::AnimTransform*)info->state.animTransform2, -0.0f) != 0) {
        info->state.state = 3;
        info->state.visible = 1;
    }
}

void func_801D4D64(CItemBoxInfo* info) {
    if (func_80137510((nw4r::lyt::AnimTransform*)info->state.animTransform2, -0.0f) != 0) {
        void* layout = info->state.layout;
        void** vtable = *(void***)layout;
        ((void(*)(void*, void*, u32))vtable[11])(layout, info->state.animTransform2, 0);
        ((void(*)(void*, void*, u32))vtable[11])(layout, info->state.animTransform1, 1);
        info->state.state = 5;
    }
}

void func_801D4DE0(CItemBoxInfo* info) {
    if (func_80137510((nw4r::lyt::AnimTransform*)info->state.animTransform1, -0.0f) != 0) {
        info->state.visible = 1;
        info->state.state = 0;
    }
}

void func_801D4E2C(void* out, u16 arg2, void* arg3) {
    if (arg3 == NULL) return;
    u16 v1 = func_801392E4(arg3);
    u16 v2 = func_80139358((u32)arg3);
    char buf[0x20];
    func_80136254(buf, (char*)&lbl_eu_805063BC[0x1ab], v2);
    func_80136254(buf, (char*)&lbl_eu_805063BC[0x1b3], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x193], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1eb], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], v2);
    void* lookup = func_8009EC9C(v2);
    func_800A082C(lookup);
    func_800A082C(lookup);
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 v = func_801392B4(i);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], v);
        func_801361E8(lbl_eu_806640F8, buf, v2);
        func_8013B380(v);
        void* lookup2 = func_8009EC9C(v);
        void* r = func_80157C4C(v + 4);
        if (r != NULL && *(u32*)r != 0) {
            u16 cat = func_80139358(*(u32*)r >> 20);
            func_80136254(buf, (char*)&lbl_eu_805063BC[0x1f4], cat);
            func_801361E8(lbl_eu_806640F8, buf, v2);
        }
        func_80139C98(v);
        func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], v2);
        func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
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
    ((u16*)out)[0] = (u16)func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
    ((u16*)out)[1] = (u16)func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], v2);
    ((u16*)out)[2] = (u16)func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1eb], v2);
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 v = func_801392B4(i);
        char buf[0x10];
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], v);
        ((u8*)out)[i] = func_801361E8(lbl_eu_806640F8, buf, v2);
        void* lookup = func_8009EC9C(v);
        u32 t1 = func_8026178C(lookup, v2);
        u32 t2 = func_8026178C(lookup, v2);
        ((u32*)((u8*)out + 4))[i] = t1;
        ((u32*)((u8*)out + 8))[i] = t2;
        void* r = func_80157C4C(v);
        if (r != NULL && *(u32*)r != 0) {
            u16 cat = func_80139358(*(u32*)r >> 20);
            ((u8*)out)[i + 12] = func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], cat);
        }
    }
}
void func_801D5564(){}

void func_801D59C0(u32* out, void* arg2, void* arg3) {
    u32 v0 = func_801392E4(arg2);
    u16 v2 = func_80139358((u32)arg2);
    u8 r1 = func_801361E8(lbl_eu_80664104, (char*)&lbl_eu_805063BC[0x214], v2);
    char* r2 = func_80136190((char*)&lbl_eu_805063BC[0x219], (char*)&lbl_eu_805063BC[0x139], r1);
    u8 r3 = func_801361E8(lbl_eu_80664104, (char*)&lbl_eu_805063BC[0x225], v2);
    char* r4;
    if (r3 == 0x1A) {
        r4 = func_80136190((char*)&lbl_eu_805063BC[0x219], (char*)&lbl_eu_805063BC[0x139], 0x14);
    } else {
        r4 = func_8013639C(lbl_eu_806640A8, (char*)&lbl_eu_805063BC[0x139]);
    }
    out[0] = r1;
    out[1] = (u32)r2;
    out[2] = r3;
    out[3] = (u32)r4;
}

void func_801D5AA0(CItemBoxInfo* info, u16 arg2, void* arg3) {
    void* global = lbl_eu_80664110;
    u16 v1 = func_801392E4(arg3);
    u16 v2 = func_80139358((u32)arg3);
    u8 flag1 = func_801361E8(global, (char*)&lbl_eu_805063BC[0x22b], v2);
    u8 count = func_801361E8(global, (char*)&lbl_eu_805063BC[0x237], v2);
    u8 arr[4] = {flag1, 0, 0, 0};
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 a = func_801392B4(i);
        arr[i + 1] = (a != count ? 1 : 0);
    }
    u8 r5 = func_801361E8(global, (char*)&lbl_eu_805063BC[0x23f], v2);
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
void func_801D5C38(){}
void func_801D5DA4(){}

void func_801D62F8(void* arr, u32 index, const void* color) {
    if (index >= 3) {
        Panic__Q24nw4r2dbFPCciPCce("file", 0x8f, "message");
    }
    __as__11_GXColorS10FRC11_GXColorS10((u8*)arr + index * 8 + 0x10, color);
}
void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
void func_80137924(void*, void*, void*, void*);
void func_801D4C9C(CItemBoxInfo*);
void func_801D4D64(CItemBoxInfo*);
void func_801E17EC(CItemBoxInfo2*);
void func_801E1868(CItemBoxInfo2*);
void func_801E18B4(CItemBoxInfo2*);
void func_801E1930(CItemBoxInfo2*);
void __ct__17UnkClass_8045F564Fv(void*);
extern void* lbl_eu_80534B28;
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
        void* layout = info->state.layout;
        void** vtable = *(void***)layout;
        ((void(*)(void*, u32))vtable[14])(layout, 0);
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
        void* layout = info->state.layout;
        void** vtable = *(void***)layout;
        ((void(*)(void*, u32))vtable[14])(layout, 0);
    }
}

void __as__11_GXColorS10FRC11_GXColorS10(void* dst, const void* src) {
    ((s16*)dst)[0] = ((s16*)src)[0];
    ((s16*)dst)[1] = ((s16*)src)[1];
    ((s16*)dst)[2] = ((s16*)src)[2];
    ((s16*)dst)[3] = ((s16*)src)[3];
}
void func_801D6394(){}
void func_801D69FC(){}

void CItemBoxInfo::setItemBoxIndex(unsigned char index, short value) {
    if (index >= 12) return;
    state.values[index] = value;
}

void func_801D77BC(){}
void func_801D79F8(){}
void func_801D8058(CItemBoxInfo* info, u16 arg2) {
    func_801D8B08(info);
    func_801D85D8(info);
    u32 buf[4];
    func_801D59C0(buf, info, (void*)arg2);
    void* layout = info->state.layout;
    func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_805063BC[0x48f], (char*)buf[0], 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_805063BC[0x49b], (char*)buf[3], 0);
}
void func_801D80EC(){}
void func_801D8318(){}
void func_801D85D8(CItemBoxInfo* info) {
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
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x135, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x165, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x2f8, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x303, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x136, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x166, base + 0x2aa, 0);
}
void func_801D885C(CItemBoxInfo* info) {
    char buf[0x20];
    for (int i = 0; i < 3; i++) {
        int index = i + 1;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x35f], index);
        void* layout = info->state.layout;
        void* child = *(void**)((u8*)layout + 0x10);
        void** vtable = *(void***)child;
        void* r = ((void*(*)(void*, const char*, u32))vtable[15])(child, buf, 1);
        func_80124270(r, 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x408], index);
        func_80136B4C((nw4r::lyt::Layout*)layout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x426], index);
        func_80136B4C((nw4r::lyt::Layout*)layout, buf, (char*)&lbl_eu_805063BC[0x2aa], 0);
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
        void* child = *(void**)((u8*)layout + 0x10);
        void** vt = *(void***)child;
        void* pane = ((void*(*)(void*, const char*, u32))vt[15])(child, buf, 1);
        if (pane != NULL) {
            void** pvt = *(void***)pane;
            u16 count = ((u16(*)(void*))pvt[26])(pane);
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
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x4a7],
                  &lbl_eu_805063BC[0x2aa], 0);
    for (int i = 0; i < 4; i++) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_805063BC[0x4b3], i + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, &lbl_eu_805063BC[0x4c0], i + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
    }
}

void func_801D8B08(CItemBoxInfo* info) {
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x48f],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x49b],
                  &lbl_eu_805063BC[0x2aa], 0);
}

void func_801D8C0C(){}
void func_801D8E34(){}

void copyItemBoxEntry(CItemBoxInfoEntry* dst, const CItemBoxInfoEntry* src) {
    dst->itemId = src->itemId;
    dst->value = src->value;
    dst->state = src->state;
}

void CItemBoxInfoEntry::setItemBoxEntry(u16 r4, u32 r5, u8 r6) {
    itemId = r4;
    value = r5;
    state = r6;
}

void func_801DF4E0(){}
void func_801DF578(){}
void func_801DF610(){}
void func_801DF988(){}
u32 func_801DFD60(u16 arg1, u32 arg2) {
    void* obj = (u8*)func_8009EC9C(arg1) + 0x3534;
    u32 result = 0;
    if (func_8026178C(obj, arg2) != 0) {
        result = func_8025FB10(obj, arg2);
    }
    return result;
}

u32 func_801E9690(u16 arg1, u32 arg2) {
    void* obj = (u8*)func_8009EC9C(arg1) + 0x3534;
    u32 result = 0;
    if (func_8026178C(obj, arg2) != 0) {
        result = func_8025FB10(obj, arg2);
    }
    return result;
}

u32 func_801E92B8(void* dummy, u8 arg1) {
    void* obj = (u8*)func_8009EC9C(arg1) + 0x3534;
    if (func_8026178C(obj, 0x2d) != 0) {
        return func_8025FB10(obj, 0x2d);
    }
    return 0;
}


u32 func_801DFDC0(void* dummy, u32 arg1, void* arg2) {
    if (arg1 == 0 || arg2 == 0) return 0;
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x1f4], arg1);
    u32 val = *(u32*)arg2;
    void* global = lbl_eu_806640F8;
    u16 r5 = func_80139358(val >> 20);
    u8 result = func_801361E8(global, buf, r5);
    return (result != 0);
}
u32 func_801DFE48(void* global, u16 arg2, void* arg3) {
    if (global == NULL) return 0;
    u16 v1 = arg3 ? (u16)func_801392E4(arg3) : 0;
    u16 v2 = arg3 ? func_80139358((u32)arg3) : 0;
    void* lookup = func_8009EC9C(arg2);
    u32 result = 0;
    for (u32 i = 4; i <= 8; i++) {
        if (i == v1) continue;
        s16 val = -1;
        switch (i) {
            case 4: val = *(s16*)((u8*)lookup + 0x1C); break;
            case 5: val = *(s16*)((u8*)lookup + 0x1E); break;
            case 6: val = *(s16*)((u8*)lookup + 0x20); break;
            case 7: val = *(s16*)((u8*)lookup + 0x22); break;
            case 8: val = *(s16*)((u8*)lookup + 0x24); break;
        }
        if (val == -1) continue;
        void* r = func_80157C4C(i);
        if (r == NULL) continue;
        u32 v = *(u32*)r;
        if (v == 0) continue;
        u16 cat = func_80139358(v >> 20);
        result += (u8)func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], cat);
    }
    if (arg3 != NULL) {
        result += (u8)func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    }
    return result;
}
void func_801DFFB8(){}

bool CItemBoxInfo::OnFileEvent(CEventFile* file) {
    return false;
}

CItemBoxInfo* __ct__CItemBoxInfo(CItemBoxInfo* info, u8 arg2, u16 arg3) {
    *(void**)info = (void*)&lbl_eu_80534B28;
    __ct__17UnkClass_8045F564Fv((u8*)info + 0x4);
    __ct__17UnkClass_8045F564Fv((u8*)info + 0x14);
    for (int i = 0; i < 27; i++) *(u32*)((u8*)info + 0x24 + i * 4) = 0;
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

CItemBoxInfo* __dt__12CItemBoxInfoFv(CItemBoxInfo* info, u32 flags) {
    if (info != NULL) {
        func_8045F778__17UnkClass_8045F564Fv((u8*)info + 0x14);
        func_8045F778__17UnkClass_8045F564Fv((u8*)info + 0x4);
        if ((s32)flags > 0) __dl__FPv(info);
    }
    return info;
}

void func_801D3FF0(void*) {}

CItemBoxInfo2* __ct__CItemBoxInfo2(CItemBoxInfo2* info, u8 arg2, u16 arg3) {
    *(void**)info = (void*)&lbl_eu_80534B28;
    __ct__17UnkClass_8045F564Fv((u8*)info + 0x4);
    __ct__17UnkClass_8045F564Fv((u8*)info + 0x14);
    for (int i = 0; i < 27; i++) *(u32*)((u8*)info + 0x24 + i * 4) = 0;
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

CItemBoxInfo2* __dt__13CItemBoxInfo2Fv(CItemBoxInfo2* info, u32 flags) {
    if (info != NULL) {
        func_8045F778__17UnkClass_8045F564Fv((u8*)info + 0x14);
        func_8045F778__17UnkClass_8045F564Fv((u8*)info + 0x4);
        if ((s32)flags > 0) __dl__FPv(info);
    }
    return info;
}

void func_801E12E0(CItemBoxInfo2* info) {
    u32 handle = getHandleMEM2__Q23mtl10MemManagerFv();
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
    info->state.active = 0;
    if (info->state.layout != 0) {
        void** vtable = *(void***)info->state.layout;
        ((void(*)(void*, u32))vtable[2])(info->state.layout, 1);
        info->state.layout = 0;
    }
    info->state.animTransform1 = 0;
    info->state.animTransform2 = 0;
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(info->state.arcResourceAccessor);
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(&info->state.resource);
    info->state.arcResourceAccessor = 0;
    info->state.resource = 0;
    func_8045F778__17UnkClass_8045F564Fv(&info->state.memRegion1);
    func_8045F778__17UnkClass_8045F564Fv(&info->state.memRegion2);
}

u8 CItemBoxInfo2::getItemBox2State() {
    return state.current;
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

void func_801E14DC(){}
void func_801E16F0(CItemBoxInfo2* info, char* arg1, char* arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x151], arg1);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, arg2, 0);
}

void func_801E174C(void* arg0, CItemBoxInfo2* info, u32 arg2) {
    char buf[0x20];
    sprintf(buf, (char*)&lbl_eu_805063BC[0x161], arg2 + 1);
    void* layout = info->state.layout;
    void* child = *(void**)((u8*)layout + 0x10);
    void** vtable = *(void***)child;
    void* r1 = ((void*(*)(void*, const char*, u32))vtable[15])(child, (char*)&lbl_eu_805063BC[0x16e], 1);
    void* r2 = ((void*(*)(void*, const char*, u32))vtable[15])(child, buf, 1);
    func_80137924(arg0, r2, r1, child);
}

void func_801E17EC(){}

void func_801E1868(CItemBoxInfo2* info) {
    if (func_80137444((nw4r::lyt::AnimTransform*)info->state.animTransform2, -0.0f) != 0) {
        info->state.state = 3;
        info->state.visible = 1;
    }
}

void func_801E18B4(CItemBoxInfo2* info) {
    if (func_80137510((nw4r::lyt::AnimTransform*)info->state.animTransform2, -0.0f) != 0) {
        void* layout = info->state.layout;
        void** vtable = *(void***)layout;
        ((void(*)(void*, void*, u32))vtable[11])(layout, info->state.animTransform2, 0);
        ((void(*)(void*, void*, u32))vtable[11])(layout, info->state.animTransform1, 1);
        info->state.state = 5;
    }
}

void func_801E1930(CItemBoxInfo2* info) {
    if (func_80137510((nw4r::lyt::AnimTransform*)info->state.animTransform1, -0.0f) != 0) {
        info->state.visible = 1;
        info->state.state = 0;
    }
}

void func_801E197C(void* out, u16 arg2, void* arg3) {
    if (arg3 == NULL) return;
    u16 v1 = func_801392E4(arg3);
    u16 v2 = func_80139358((u32)arg3);
    char buf[0x20];
    func_80136254(buf, (char*)&lbl_eu_805063BC[0x1ab], v2);
    func_80136254(buf, (char*)&lbl_eu_805063BC[0x1b3], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x193], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1eb], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], v2);
    void* lookup = func_8009EC9C(v2);
    func_800A082C(lookup);
    func_800A082C(lookup);
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 v = func_801392B4(i);
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], v);
        func_801361E8(lbl_eu_806640F8, buf, v2);
        func_8013B380(v);
        void* lookup2 = func_8009EC9C(v);
        void* r = func_80157C4C(v + 4);
        if (r != NULL && *(u32*)r != 0) {
            u16 cat = func_80139358(*(u32*)r >> 20);
            func_80136254(buf, (char*)&lbl_eu_805063BC[0x1f4], cat);
            func_801361E8(lbl_eu_806640F8, buf, v2);
        }
        func_80139C98(v);
        func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], v2);
        func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
        func_80139C98(v + 1);
    }
}
void func_801E1E0C(void* out, u16 arg2, void* arg3) {
    if (arg3 == NULL) {
        memset(out, 0, 0x1C);
        return;
    }
    u16 v1 = func_801392E4(arg3);
    u16 v2 = func_80139358((u32)arg3);
    ((u16*)out)[0] = (u16)func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1ab], v2);
    ((u16*)out)[1] = (u16)func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1b3], v2);
    ((u16*)out)[2] = (u16)func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1eb], v2);
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 v = func_801392B4(i);
        char buf[0x10];
        sprintf(buf, (char*)&lbl_eu_805063BC[0x1f4], v);
        ((u8*)out)[i] = func_801361E8(lbl_eu_806640F8, buf, v2);
        void* lookup = func_8009EC9C(v);
        u32 t1 = func_8026178C(lookup, v2);
        u32 t2 = func_8026178C(lookup, v2);
        ((u32*)((u8*)out + 4))[i] = t1;
        ((u32*)((u8*)out + 8))[i] = t2;
        void* r = func_80157C4C(v);
        if (r != NULL && *(u32*)r != 0) {
            u16 cat = func_80139358(*(u32*)r >> 20);
            ((u8*)out)[i + 12] = func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1f4], cat);
        }
    }
}
void func_801E20FC(){}

void func_801E2558(u32* out, void* arg2) {
    u32 v0 = func_801392E4(arg2);
    u16 v2 = func_80139358((u32)arg2);
    u8 r1 = func_801361E8(lbl_eu_80664104, (char*)&lbl_eu_805063BC[0x214], v2);
    char* r2 = func_80136190((char*)&lbl_eu_805063BC[0x219], (char*)&lbl_eu_805063BC[0x139], r1);
    u8 r3 = func_801361E8(lbl_eu_80664104, (char*)&lbl_eu_805063BC[0x225], v2);
    char* r4;
    if (r3 == 0x1A) {
        r4 = func_80136190((char*)&lbl_eu_805063BC[0x219], (char*)&lbl_eu_805063BC[0x139], 0x14);
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
    u8 flag1 = func_801361E8(global, (char*)&lbl_eu_805063BC[0x22b], v2);
    u8 count = func_801361E8(global, (char*)&lbl_eu_805063BC[0x237], v2);
    u8 arr[4] = {flag1, 0, 0, 0};
    u32 max = func_801392C0();
    for (u32 i = 0; i < max; i++) {
        u8 a = func_801392B4(i);
        arr[i + 1] = (a != count ? 1 : 0);
    }
    u8 r5 = func_801361E8(global, (char*)&lbl_eu_805063BC[0x23f], v2);
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
    void** vt = *(void***)inst;
    u8 r = ((u8(*)(void*, void*))vt[2])(inst, arg2);
    char* s = func_80136190((char*)&lbl_eu_805063BC[0x130], (char*)&lbl_eu_805063BC[0x139], 0x1e - (r - 1));
    ((u32*)out)[0] = 0;
    ((u8*)out)[4] = 0;
    for (u32 i = 0; i < 3; i++) {
        void* inst2 = CItem_initItemImplInstances(arg2);
        void** vt2 = *(void***)inst2;
        u16 count = ((u16(*)(void*, void*, u32))vt2[19])(inst2, arg2, i);
        if (count == 0) continue;
        char* t = func_8013639C(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x139]);
        ((u32*)((u8*)out + 8))[i * 4] = (u32)t;
        void* inst3 = CItem_initItemImplInstances(arg2);
        void** vt3 = *(void***)inst3;
        u8 val = ((u8(*)(void*, void*, u32))vt3[25])(inst3, arg2, i);
        ((u8*)out)[i + 0x1C] = val;
    }
}
void func_801E2928(){}
void func_801E2C5C(){}
void func_801E2FEC(){}
void func_801E3228(){}
void func_801E3730(CItemBoxInfo2* info, u16 arg2) {
    func_801D8B08((CItemBoxInfo*)info);
    func_801E3918(info);
    u32 buf[4];
    func_801D59C0(buf, info, (void*)arg2);
    void* layout = *(void**)((u8*)info + 0x34);
    func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_805063BC[0x48f], (char*)buf[0], 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_805063BC[0x49b], (char*)buf[3], 0);
}
void func_801E37C4(){}
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
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x135, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x165, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x2f8, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x303, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x136, base + 0x2aa, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, base + 0x166, base + 0x2aa, 0);
}
void func_801E3B9C(){}
void func_801E3DE4(CItemBoxInfo2* info) {
    char buf[0x20];
    for (int i = 0; i < 3; i++) {
        int index = i + 1;
        sprintf(buf, (char*)&lbl_eu_805063BC[0x35f], index);
        void* layout = *(void**)((u8*)info + 0x34);
        void* child = *(void**)((u8*)layout + 0x10);
        void** vtable = *(void***)child;
        void* r = ((void*(*)(void*, const char*, u32))vtable[15])(child, buf, 1);
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
        void** vt = *(void***)child;
        void* pane = ((void*(*)(void*, const char*, u32))vt[15])(child, buf, 1);
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
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x4a7],
                  &lbl_eu_805063BC[0x2aa], 0);
    for (int i = 0; i < 4; i++) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_805063BC[0x4b3], i + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
        sprintf(buf, &lbl_eu_805063BC[0x4c0], i + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf,
                      &lbl_eu_805063BC[0x2aa], 0);
    }
}

void func_801E4194(){}
void func_801E4390(CItemBoxInfo2* info) {
    if (info->state.layout == 0) return;
    if (info->state.resource) {
        info->state.current = 1;
        info->state.active = 1;
    }
}
void func_801E43BC(){}

void copyItemBoxCopy(ItemBoxInfoCopy* dst, const ItemBoxInfoCopy* src) {
    dst->itemId = src->itemId;
    dst->value = src->value;
    dst->state = src->state;
}

void CItemBoxInfoEntry::setItemBoxCopy(unsigned short a, unsigned int b, unsigned char c) {
    itemId = a;
    value = b;
    state = c;
}

void func_801E9190(){}
void func_801E9224(){}
void func_801E9310(){}
u32 func_801E96F0(void* dummy, u32 arg1, u32 arg2) {
    if (arg1 == 0 || arg2 == 0) return 0;
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x1f4], arg1);
    void* global = lbl_eu_806640F8;
    u16 r5 = func_80139358(arg2);
    u8 result = func_801361E8(global, buf, r5);
    return (result != 0);
}
u32 func_801E9774(void* global, u16 arg2, void* arg3) {
    if (global == NULL) return 0;
    u16 v1 = arg3 ? func_801392E4(arg3) : 0;
    u16 v2 = arg3 ? func_80139358((u32)arg3) : 0;
    void* lookup = func_8009EC9C(arg2);
    u32 result = 0;
    for (u32 i = 4; i <= 8; i++) {
        if (i == v1) continue;
        s16 val = -1;
        switch (i) {
            case 4: val = *(s16*)((u8*)lookup + 0x1C); break;
            case 5: val = *(s16*)((u8*)lookup + 0x1E); break;
            case 6: val = *(s16*)((u8*)lookup + 0x20); break;
            case 7: val = *(s16*)((u8*)lookup + 0x22); break;
            case 8: val = *(s16*)((u8*)lookup + 0x24); break;
        }
        if (val == -1) continue;
        void* r = func_80157C4C(i);
        if (r == NULL) continue;
        u32 v = *(u32*)r;
        if (v == 0) continue;
        u16 cat = func_80139358(v >> 20);
        result += (u8)func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], cat);
    }
    if (arg3 != NULL) {
        result += (u8)func_801361E8(lbl_eu_806640F8, (char*)&lbl_eu_805063BC[0x1e2], v2);
    }
    return result;
}
void func_801E98E4(){}

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

void sinint_801EABC4() {
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
}

char* func_801D3C74(void* item_data, u8 index) {
    if (index >= 12) return NULL;
    u16 item_id = *(u16*)((u8*)item_data + index * 2);
    if (item_id == 0) return NULL;
    u8 category = *(u8*)((u8*)item_data + index + 0xB4);
    s16 val = *(s16*)((u8*)item_data + index * 2 + 0xC0);
    char buf[0x80];
    sprintf(buf, (char*)&lbl_eu_805063BC, val);
    func_801361E8(lbl_eu_806640D8, (char*)&lbl_eu_805063BC[0x3], item_id);
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

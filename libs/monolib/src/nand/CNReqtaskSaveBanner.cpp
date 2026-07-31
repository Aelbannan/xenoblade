// Decompiled: CNReqtaskSaveBanner
#include <types.h>
#include <string.h>
#include <monolib/util/FixStr.hpp>

// Global symbols
extern "C" {
    void* lbl_eu_80665A90;
    extern char lbl_eu_80570358[];  // vtable data - array type prevents sda21
    u8 lbl_eu_806659D0;
    s32 lbl_eu_806659D4;
    u8 lbl_eu_80665A94;
    s32 lbl_eu_80663CD0;
    s32 lbl_eu_80663CD4;
    char lbl_eu_80661850[];
    extern const char lbl_eu_805248A8[];

    u32* func_804DA98C(u8 arg);
    s32 func_804DA91C(u32* ptr);
    s32 func_804DA540(void* ptr, u32 arg);
    s32 func_804DA82C(void* data);
    s32 func_804DA69C(void);
    void* func_804F50D0(void* data);
    s32 func_804DA70C(void* data, u32 arg1, u32 arg2);
    s32 func_804F53DC(void* ptr);
    void func_804DA4CC(void* data, void* dealloc);
    s32 func_804DA628(u32 addr, u32 size);
    s32 func_804DA7CC(void* data, void* bannerPath);
    void __dt__804F5738(void* ptr);
}

// CNReqtaskSaveBanner data layout:
//   +0x00: u32  field_0 (pointer to some struct with unk324, unk325)
//   +0x04: u8   state
//   +0x08: u32  field_8
//   +0x0C: u8   field_0C

// us-804f92d8: func_804F4D7C
void** func_804F4D7C(void* data, void* arg1, u8 arg2) {
    ((void**)data)[0] = arg1;
    ((u8*)data)[0x0C] = arg2;
    ((u8*)data)[0x04] = 0;
    return &lbl_eu_80665A90;
}

// us-804f9638: func_804F50D0
void* func_804F50D0(void* data) {
    if ((s8)lbl_eu_80665A94 == 0) {
        char* base = lbl_eu_80661850;
        base[0] = 0;
        *(u32*)(base + 0x20) = 0;
        lbl_eu_80665A94 = 1;
    }
    ((ml::FixStr<32>*)lbl_eu_80661850)->format(lbl_eu_805248A8, lbl_eu_80663CD0, lbl_eu_80663CD4);
    return lbl_eu_80661850;
}

// us-804f95e8: func_804F5080
void func_804F5080(void* data, void* dealloc) {
    if (dealloc != 0) {
        void* ptr = *(void**)dealloc;
        if (ptr != 0) {
            __dt__804F5738(ptr);
        }
    }
    func_804DA4CC(data, dealloc);
}

// SaveBanner task data layout:
//   +0x00: void* f0   (points to object with unk324/unk325 flags)
//   +0x04: u8   state
//   +0x08: u32  f8
//   +0x0C: u8   fC
struct CNReqtaskSaveBannerData {
    void* f0;
    u8 state;
    u8 pad5[3];
    u32 f8;
    u8 fC;
};

struct CNReqtaskSaveBannerTarget {
    u8 pad[0x324];
    u8 unk324;
    u8 unk325;
};

// us-804f92f0: func_804F4D90
s32 func_804F4D90(void* vtable_ptr, void* data) {
    CNReqtaskSaveBannerData* d = (CNReqtaskSaveBannerData*)data;
    CNReqtaskSaveBannerTarget* t = (CNReqtaskSaveBannerTarget*)d->f0;
    if (t->unk324 != 0) {
        return 0;
    }
    if (lbl_eu_806659D0 != 0) {
        return 0;
    }

    if ((s8)d->state > 0) {
        if ((s8)d->state == 2) {
            if (lbl_eu_806659D4 == -12) {
                d->state = 5;
            } else if (lbl_eu_806659D4 == -15 || lbl_eu_806659D4 == -5) {
                d->state = 5;
            } else if (lbl_eu_806659D4 < 0) {
                return 2;
            }
        } else {
            if ((s8)d->state == 7) {
                if (t->unk325 != 0) return 2;
            } else {
                if (lbl_eu_806659D4 < 0) return 2;
            }
        }
    }

    switch ((s8)d->state) {
        s32 r;
        void* ctx;
        void* p;
        u32 addr;
        u32 size;
        u32* bannerPath;

        case 0:
            bannerPath = func_804DA98C(d->fC);
            if (bannerPath == 0) return 2;
            r = func_804DA91C(bannerPath);
            if (r != 0) return 2;
            d->state = 1;
            break;

        case 1:
            r = func_804DA540((void*)lbl_eu_80663CD4, 1);
            if (r != 0) return 2;
            d->state = 2;
            break;

        case 2:
            ctx = &d->f8;
            r = func_804DA82C(ctx);
            if (r != 0) return 2;
            d->state = 3;
            break;

        case 3:
            r = func_804DA69C();
            if (r != 0) return 2;
            if (d->f8 == 0) {
                d->state = 5;
            } else {
                d->state = 4;
            }
            break;

        case 4:
            return 1;

        case 5: {
            void* pathStr = func_804F50D0(data);
            r = func_804DA70C(pathStr, 0x34, 0);
            if (r != 0 && r != -6) return 2;
            d->state = 6;
            break;
        }

        case 6:
            r = func_804F53DC(d->f0);
            if (r == 0) return 2;
            d->state = 7;
            break;

        case 7: {
            void* pathStr = func_804F50D0(data);
            r = func_804DA540(pathStr, 2);
            if (r != 0) return 2;
            d->state = 8;
            break;
        }

        case 8: {
            struct BannerBuf { u32 pad0; u32 f4; u32 f8; }* b = (struct BannerBuf*)d->f0;
            addr = b->f4;
            size = b->f8;
            r = func_804DA628(addr, size);
            if (r != 0) return 2;
            d->state = 9;
            break;
        }

        case 9:
            r = func_804DA69C();
            if (r != 0) return 2;
            d->state = 0xA;
            break;

        case 0xA: {
            p = d->f0;
            __dt__804F5738(p);
            bannerPath = func_804DA98C(d->fC);
            void* pathStr = func_804F50D0(data);
            r = func_804DA7CC(pathStr, bannerPath);
            if (r != 0) return 2;
            d->state = 0xB;
            break;
        }

        case 0xB:
            d->state = 4;
            break;

        default:
            break;
    }
    return 0;
}

// us-804f96a8: sinit_804F5140
void** sinit_804F5140() {
    void** p = &lbl_eu_80665A90;
    void* v = (void*)lbl_eu_80570358;
    *p = v;
    return p;
}

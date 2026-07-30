// Decompiled: CNReqtaskSaveBanner
#include <types.h>
#include <string.h>
#include <monolib/util/FixStr.hpp>

// Global symbols
extern "C" {
    void* lbl_eu_80665A90;
    extern void* lbl_eu_80570358;
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

// us-804f92f0: func_804F4D90
s32 func_804F4D90(void* vtable_ptr, void* data) {
    void* ptr = *(void**)data;
    if (*(u8*)((u8*)ptr + 0x324) != 0) {
        return 0;
    }
    if (lbl_eu_806659D0 != 0) {
        return 0;
    }

    s8 state = (s8)((u8*)data)[0x04];

    if (state > 0) {
        if (state == 2) {
            s32 err = lbl_eu_806659D4;
            if (err == -5 || err == -12 || err == -15) {
                ((u8*)data)[0x04] = 5;
            } else if (err < 0) {
                return 2;
            } else {
                if (*(u8*)((u8*)ptr + 0x325) != 0) return 2;
            }
        } else if (state == 7) {
            if (*(u8*)((u8*)ptr + 0x325) != 0) return 2;
        } else {
            if (lbl_eu_806659D4 < 0) return 2;
        }
    }

    switch (state) {
        s32 r;
        void* ctx;
        void* p;
        u32 addr;
        u32 size;
        u32* bannerPath;

        case 0:
            bannerPath = func_804DA98C(((u8*)data)[0x0C]);
            if (bannerPath == 0) return 2;
            r = func_804DA91C(bannerPath);
            if (r != 0) return 2;
            ((u8*)data)[0x04] = 1;
            break;

        case 1:
            r = func_804DA540((void*)lbl_eu_80663CD4, 1);
            if (r != 0) return 2;
            ((u8*)data)[0x04] = 2;
            break;

        case 2:
            ctx = (void*)((u8*)data + 8);
            r = func_804DA82C(ctx);
            if (r != 0) return 2;
            ((u8*)data)[0x04] = 3;
            break;

        case 3:
            r = func_804DA69C();
            if (r != 0) return 2;
            if (((u32*)data)[2] != 0) {
                ((u8*)data)[0x04] = 5;
            } else {
                ((u8*)data)[0x04] = 4;
            }
            break;

        case 4:
            return 1;

        case 5: {
            void* pathStr = func_804F50D0(data);
            r = func_804DA70C(pathStr, 0x34, 0);
            if (r != 0 && r != -6) return 2;
            ((u8*)data)[0x04] = 6;
            break;
        }

        case 6:
            r = func_804F53DC(*(void**)data);
            if (r == 0) return 2;
            ((u8*)data)[0x04] = 7;
            break;

        case 7: {
            void* pathStr = func_804F50D0(data);
            r = func_804DA540(pathStr, 2);
            if (r != 0) return 2;
            ((u8*)data)[0x04] = 8;
            break;
        }

        case 8:
            p = *(void**)data;
            addr = *(u32*)((u8*)p + 4);
            size = *(u32*)((u8*)p + 8);
            r = func_804DA628(addr, size);
            if (r != 0) return 2;
            ((u8*)data)[0x04] = 9;
            break;

        case 9:
            r = func_804DA69C();
            if (r != 0) return 2;
            ((u8*)data)[0x04] = 0xA;
            break;

        case 0xA: {
            p = *(void**)data;
            __dt__804F5738(p);
            bannerPath = func_804DA98C(((u8*)data)[0x0C]);
            void* pathStr = func_804F50D0(data);
            r = func_804DA7CC(pathStr, bannerPath);
            if (r != 0) return 2;
            ((u8*)data)[0x04] = 0xB;
            break;
        }

        case 0xB:
            return 1;

        default:
            break;
    }
    return 0;
}

// us-804f96a8: sinit_804F5140
void sinit_804F5140() {
    void** dest = &lbl_eu_80665A90;
    *dest = (void*)&lbl_eu_80570358;
}

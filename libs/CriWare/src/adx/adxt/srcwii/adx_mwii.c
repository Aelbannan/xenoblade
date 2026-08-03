// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/srcwii/adx_mwii
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <revolution/OS.h>
#include <revolution/VI/vi.h>

// Forward declarations for functions defined in other TUs
extern void ADXM_ShutdownThrd(void);
extern void ADXMNG_SetFramework(u32 val);
extern void ADXMNG_CallMainServerFunctions(void);
extern void SVM_ExecSvrFs(void);
extern void SVM_CallErr1(const char* msg);
extern char lbl_eu_805196D4[];

// Base struct for the ADXM framework state at lbl_eu_805F3A50.
// Accessed by adxm_safe_proc, adxm_fs_proc, and adxm_unlock.
struct AdxmBase {
    u32 field_0x00;
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2C;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3C;
    s32 field_0x40;  // lock count (decremented in adxm_unlock)
    u32 field_0x44;
    s32 field_0x48;  // incremented in adxm_safe_proc
    u32 field_0x4C;
    s32 field_0x50;  // incremented in adxm_fs_proc
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5C;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6C;
    u32 field_0x70;
    s32 field_0x74;  // thread priority
    OSThread field_0x78;  // thread struct (suspended in adxm_unlock)
    u32 field_0x390[(0x9D0 - 0x390) / 4];
    s32 field_0x9D0;  // flag checked in adxm_safe_proc
    s32 field_0x9D4;  // flag set in adxm_safe_proc
    u32 field_0x9D8;
    u32 field_0x9DC;
    s32 field_0x9E0;  // flag checked in adxm_fs_proc
    s32 field_0x9E4;  // flag set in adxm_fs_proc
};
extern struct AdxmBase lbl_eu_805F3A50;

void ADXM_WaitVsync(void) { VIWaitForRetrace(); }

void ADXM_ExecMain(void) { ADXMNG_CallMainServerFunctions(); }

void ADXM_Lock(void) { SVM_Lock(); }

void ADXM_Unlock(void) { SVM_Unlock(); }

void adxm_lock() {}

void adxm_unlock(void) {
    struct AdxmBase* base = &lbl_eu_805F3A50;
    volatile s32* p = &base->field_0x40;
    *p -= 1;
    if (*p == 0) {
        OSThread* currThread = OSGetCurrentThread();
        OSSuspendThread(&base->field_0x78);
        OSSetThreadPriority(currThread, base->field_0x74);
    }
}

void adxm_goto_mwidle_border(void) {
    u8* base = (u8*)&lbl_eu_805F3A50;
    u32 prio;
    s32 spin = 0;

    if (*(s32*)(base + 0x390) != 1)
        return;
    prio = *(u32*)(base + 0x28);
    *(u32*)(base + 0x44) = 1;
    OSSetThreadPriority((OSThread*)(base + 0x398), *(s32*)(base + 0x10));
    do {
        OSResumeThread((OSThread*)(base + 0x398));
        if (*(u32*)(base + 0x44) == 0)
            break;
        spin++;
    } while (spin < 0xBEBC200);
    if ((u32)(spin + 0xF4150000) <= 0xC200)
        SVM_CallErr1((const char*)lbl_eu_805196D4);
    OSSetThreadPriority((OSThread*)(base + 0x398), (s32)prio);
}

void adxm_safe_proc(void) {
    struct AdxmBase* base = &lbl_eu_805F3A50;
    while (base->field_0x9D0 == 1) {
        base->field_0x48 += 1;
    }
    base->field_0x9D4 = 1;
}

extern u32 lbl_eu_805E26DC;
extern void VIWaitForRetrace(void);
extern void SVM_ExecSvrVsync(void);

void adxm_vsync_proc(void) {
    struct VsyncCb {
        void (*fn)(void*);
        void* obj;
    };
    u8* base = (u8*)&lbl_eu_805F3A50;
    struct VsyncCb* cb = (struct VsyncCb*)(base + 0x68);
    u32* cnt = &lbl_eu_805E26DC;
    while (*(s32*)(base + 0x9D8) == 1) {
        u32 a;
        u32 b;
        VIWaitForRetrace();
        a = *(u32*)(base + 0x4C) + 1;
        b = *cnt + 1;
        *(u32*)(base + 0x4C) = a;
        *cnt = b;
        SVM_ExecSvrVsync();
        if (*(u32*)(base + 0x390) == 0) {
            OSResumeThread((OSThread*)(base + 0x398));
            if (cb->fn != NULL) {
                cb->fn(cb->obj);
            }
        }
    }
    *(u32*)(base + 0x9DC) = 1;
}

void adxm_fs_proc(void) {
    u8* base = (u8*)&lbl_eu_805F3A50;
    while (*(s32*)(base + 0x9E0) == 1) {
        VIWaitForRetrace();
        (*(u32*)(base + 0x50))++;
        SVM_ExecSvrFs();
    }
    *(u32*)(base + 0x9E4) = 1;
}

extern int SVM_ExecSvrMwIdle(void);

void adxm_mwidle_proc(void) {
    u8* base = (u8*)&lbl_eu_805F3A50;
    u32* p16 = (u32*)(base + 0x10);
    u32* cb = (u32*)(base + 0x68);
    s32 r;
    while (*(s32*)(base + 0x9E8) == 1) {
        (*(u32*)(base + 0x54))++;
        r = SVM_ExecSvrMwIdle();
        if (r != 0) {
            if (*(s32*)(base + 0x44) != 1)
                continue;
        }
        if (*(s32*)(base + 0x44) == 1) {
            *(s32*)(base + 0x44) = 0;
            OSSetThreadPriority((OSThread*)(base + 0x398), (s32)p16[6]);
        }
        if ((s32)cb[0] != 0) {
            ((void (*)(void*))cb[0])((void*)cb[1]);
        }
        if (r == 0) {
            if (*(s32*)(base + 0x08) == 1) {
                OSResumeThread((OSThread*)(base + 0x6B8));
            }
        }
        OSSuspendThread((OSThread*)(base + 0x398));
    }
    *(s32*)(base + 0x390) = 1;
}

void ADXM_SetCbErr(void) { SVM_SetCbErr(); }

extern OSThread* lbl_eu_805FBA78;
extern u32 lbl_eu_805FBA7C;

static void adxm_create_base_thread(void) {
    u8* base = (u8*)&lbl_eu_805F3A50;
    u32* p16 = (u32*)(base + 0x10);
    OSCreateThread((OSThread*)(base + 0x78), (void*)adxm_safe_proc, NULL,
                   base + 0x19F8, 4096, (s32)p16[1], 1);
    OSCreateThread((OSThread*)(base + 0x19F8), (void*)adxm_vsync_proc, base + 0x1D10,
                   base + 0x3D10, 8192, (s32)p16[3], 1);
    OSCreateThread((OSThread*)(base + 0x3D10), (void*)adxm_fs_proc, base + 0x4028,
                   base + 0x6028, 8192, (s32)p16[4], 1);
    OSCreateThread((OSThread*)(base + 0x398), (void*)adxm_mwidle_proc, base + 0x6028,
                   base + 0x8028, 8192, (s32)p16[6], 1);
    lbl_eu_805FBA78 = OSGetCurrentThread();
    if (*(s32*)(base + 0x24) != 16) {
        s32 old = OSDisableInterrupts();
        OSDisableScheduler();
        *(s32*)(base + 0x70) = 1;
        s32 prio = OSGetThreadPriority(lbl_eu_805FBA78);
        OSSetThreadPriority(lbl_eu_805FBA78, prio);
        lbl_eu_805FBA7C = (u32)prio;
        *(s32*)(base + 0x70) = 0;
        OSEnableScheduler();
        OSRestoreInterrupts(old);
    }
}

void ADXM_SetupThrd() {}

extern u32 lbl_eu_805F3A54;
u32 ADXM_IsSetupThrd(void) {
    return (lbl_eu_805F3A54 != 0) ? 1 : 0;
}

extern s32 lbl_eu_805FDD9C;
s32 ADXM_ShutdownFramework(void) {
    s32 result = 1;
    s32 state = lbl_eu_805FDD9C;
    if (state == 0 || state == 2)
        goto call;
    if (state != 1)
        result = 0;
    goto done;
call:
    ADXM_ShutdownThrd();
done:
    ADXMNG_SetFramework(-1);
    return result;
}

void ADXM_SetupFramework() {}

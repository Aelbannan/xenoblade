// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/srcwii/adx_mwii
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <revolution/OS.h>
#include <revolution/VI/vi.h>

// Forward declarations for functions defined in other TUs
extern void ADXM_ShutdownThrd(void);
extern void ADXM_SetupThrd(u8* arg);
extern void ADXMNG_SetFramework(u32 val);
extern void ADXMNG_CallMainServerFunctions(void);
extern void SVM_ExecSvrFs(void);
extern u32 SVM_ExecSvrVsync(void);
extern u32 SVM_ExecSvrMwIdle(void);
extern void SVM_CallErr1(const char* msg);
extern void SVM_Init(void);
extern void SVM_SetCbLock(void* cb, void* ctx);
extern void SVM_SetCbUnlock(void* cb, void* ctx);
extern void SVM_SetCbBdr(s32 idx, void* fn, void* ctx);
extern char lbl_eu_805196D4[];
extern volatile u32 lbl_eu_805196D0;

// Callback pair stored in the framework state (fn at 0x68, context at 0x6C).
struct AdxMwCb {
    void (*fn)(u8* obj);    // 0x068 idle/vsync callback
    u8* obj;                // 0x06C callback context
};

// Thread parameter block at 0x10 (priorities used by the thread lifecycle).
struct AdxParams {
    u32 field_0x00;         // 0x010 mwidle thread priority
    u32 field_0x04;         // 0x014
    u32 field_0x08;         // 0x018
    u32 field_0x0C;         // 0x01C
    u32 field_0x10;         // 0x020
    u32 field_0x14;         // 0x024
    u32 field_0x18;         // 0x028 saved mwidle priority
    u32 field_0x1C;         // 0x02C
    u32 field_0x20;         // 0x030
    u32 field_0x24;         // 0x034
    u32 field_0x28;         // 0x038
    u32 field_0x2C;         // 0x03C
};

// Parameter block passed to ADXM_SetupThrd; copied into AdxParams at 0x10.
// Offsets differ from AdxParams (0x08 maps onto AdxParams.field_0x0C).
struct AdxSetupArgs {
    u32 field_0x00;         // 0x00
    u32 field_0x04;         // 0x04
    u32 field_0x08;         // 0x08
    u32 field_0x0C;         // 0x0C
    u32 field_0x10;         // 0x10
    u32 field_0x14;         // 0x14
};

// Base struct for the ADXM framework state at lbl_eu_805F3A50.
// Accessed by adxm_safe_proc, adxm_fs_proc, adxm_unlock, and the thread
// lifecycle functions (vsync / mwidle / shutdown).
struct AdxmBase {
    u32 field_0x00;                 // 0x000
    u32 field_0x04;                 // 0x004 setup-thread refcount
    s32 field_0x08;                 // 0x008
    u32 field_0x0C;                 // 0x00C
    struct AdxParams field_0x10;    // 0x010 thread parameter block
    s32 field_0x40;                 // 0x040 lock count (decremented in adxm_unlock)
    s32 field_0x44;                 // 0x044 mwidle border flag
    s32 field_0x48;                 // 0x048 incremented in adxm_safe_proc
    u32 field_0x4C;                 // 0x04C vsync counter
    s32 field_0x50;                 // 0x050 incremented in adxm_fs_proc
    u32 field_0x54;                 // 0x054 mwidle counter
    u32 field_0x58;                 // 0x058
    u32 field_0x5C;                 // 0x05C
    u32 field_0x60;                 // 0x060
    u32 field_0x64;                 // 0x064
    struct AdxMwCb field_0x68;      // 0x068 idle/vsync callback pair
    u32 field_0x70;                 // 0x070 lock flag
    s32 field_0x74;                 // 0x074 thread priority
    OSThread field_0x78;            // 0x078 safe-proc thread
    s32 field_0x390;                // 0x390 mwidle state flag
    u32 field_0x394;                // 0x394
    OSThread field_0x398;           // 0x398 mwidle thread
    u32 field_0x6B0;                // 0x6B0
    u32 field_0x6B4;                // 0x6B4
    OSThread field_0x6B8;           // 0x6B8 frame/vsync thread
    s32 field_0x9D0;                // 0x9D0 flag checked in adxm_safe_proc
    s32 field_0x9D4;                // 0x9D4 flag set in adxm_safe_proc
    s32 field_0x9D8;                // 0x9D8 vsync loop flag
    s32 field_0x9DC;                // 0x9DC vsync done flag
    s32 field_0x9E0;                // 0x9E0 flag checked in adxm_fs_proc
    s32 field_0x9E4;                // 0x9E4 flag set in adxm_fs_proc
    s32 field_0x9E8;                // 0x9E8 mwidle loop flag
    u8 field_pad_0x9EC[0x0C];       // 0x9EC padding
    u8 field_stack_0x9F8[0x1000];   // 0x9F8 safe-proc thread stack (top = base+0x19F8)
    OSThread field_0x19F8;          // 0x19F8 vsync thread
    u8 field_stack_0x1D10[0x2000];  // 0x1D10 vsync thread stack (top = base+0x3D10)
    OSThread field_0x3D10;          // 0x3D10 fs thread
    u8 field_stack_0x4028[0x2000];  // 0x4028 fs thread stack (top = base+0x6028)
    u8 field_stack_0x6028[0x2000];  // 0x6028 mwidle thread stack (top = base+0x8028)
};
extern struct AdxmBase lbl_eu_805F3A50;

void ADXM_WaitVsync(void) { VIWaitForRetrace(); }

void ADXM_ExecMain(void) { ADXMNG_CallMainServerFunctions(); }

void ADXM_Lock(void) { SVM_Lock(); }

void ADXM_Unlock(void) { SVM_Unlock(); }


void adxm_lock(void) {
    u8* g = (u8*)&lbl_eu_805F3A50;
    volatile u32* lockcnt = (volatile u32*)((u8*)g + 0x40);
    if (*lockcnt == 0) {
        u32 irq;
        OSThread* t;
        s32 p;
        irq = OSDisableInterrupts();
        OSDisableScheduler();
        *(u32*)((u8*)g + 0x70) = 1;
        t = OSGetCurrentThread();
        p = OSGetThreadPriority(t);
        OSSetThreadPriority(t, *(s32*)((u8*)g + 0x10));
        *(s32*)((u8*)g + 0x74) = p;
        *(u32*)((u8*)g + 0x70) = 0;
        OSEnableScheduler();
        OSRestoreInterrupts(irq);
        OSResumeThread((OSThread*)((u8*)g + 0x78));
    }
    *lockcnt += 1;
}

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

// Raise the mwidle thread to its "border" priority and spin until it has
// actually started running (flag at 0x44 cleared), then restore the priority.
// If the thread never acknowledges within the spin limit, report the error
// string and carry on.
void adxm_goto_mwidle_border(void) {
    struct AdxmBase* base = &lbl_eu_805F3A50;
    struct AdxParams* prm;
    s32 prio;
    s32 spin;
    s32 limit;

    if (base->field_0x390 != 1) {
        struct AdxParams* prm = &base->field_0x10;
        base->field_0x44 = 1;
        prio = prm->field_0x18;
        OSSetThreadPriority(&base->field_0x398, prm->field_0x00);
        limit = 0xBEBC200;
        spin = 0;
        do {
            OSResumeThread(&base->field_0x398);
            if (base->field_0x44 == 0)
                break;
            spin += 1;
        } while (spin < limit);
        if (spin == limit)
            SVM_CallErr1(lbl_eu_805196D4);
        OSSetThreadPriority(&base->field_0x398, prio);
    }
}

void adxm_safe_proc(void) {
    struct AdxmBase* base = &lbl_eu_805F3A50;
    while (base->field_0x9D0 == 1) {
        base->field_0x48 += 1;
    }
    base->field_0x9D4 = 1;
}

extern u32 lbl_eu_805E26DC;

void adxm_vsync_proc(void) {
    struct AdxmBase* base = &lbl_eu_805F3A50;
    while (base->field_0x9D8 == 1) {
        u32 a;
        u32 b;
        VIWaitForRetrace();
        /* counters: read both before either store (retail shape) */
        b = lbl_eu_805E26DC + 1;
        a = base->field_0x4C + 1;
        lbl_eu_805E26DC = b;
        base->field_0x4C = a;
        SVM_ExecSvrVsync();
        /* independent derivation keeps this pointer out of base's value graph */
        struct AdxMwCb* cb = &lbl_eu_805F3A50.field_0x68;
        if (base->field_0x390 == 0) {
            OSResumeThread(&base->field_0x398);
            if (base->field_0x68.fn != NULL) {
                cb->fn(cb->obj);
            }
        }
    }
    base->field_0x9DC = 1;
}

void adxm_fs_proc(void) {
    u8* base = (u8*)&lbl_eu_805F3A50;
    while (*(s32*)(base + 0x9E0) == 1) {
        VIWaitForRetrace();
        (*(u32*)(base + 0x50))++;
        SVM_ExecSvrFs();
    }
    *(volatile u32*)(base + 0x9E4) = 1;
}

void adxm_mwidle_proc(void) {
    struct AdxmBase* base = &lbl_eu_805F3A50;
    s32 zero = 0;
    /* Derive prm/cb straight from the global (not via base) so MWCC keeps
     * them as separate addi webs off the shared base register, like retail */
    struct AdxParams* prm = &lbl_eu_805F3A50.field_0x10;
    struct AdxMwCb* cb = &lbl_eu_805F3A50.field_0x68;
    s32 r;
    while (base->field_0x9E8 == 1) {
        base->field_0x54 += 1;
        r = (s32)SVM_ExecSvrMwIdle();
        if (r != 0) {
            if (base->field_0x44 != 1)
                continue;
        }
        if (base->field_0x44 == 1) {
            base->field_0x44 = zero;
            OSSetThreadPriority(&base->field_0x398, (s32)prm->field_0x18);
        }
        if (cb->fn != NULL) {
            cb->fn(cb->obj);
        }
        if (r == 0 && base->field_0x08 == 1) {
            OSResumeThread(&base->field_0x6B8);
        }
        OSSuspendThread(&base->field_0x398);
    }
    base->field_0x390 = 1;
}

void ADXM_SetCbErr(void) { SVM_SetCbErr(); }

extern OSThread* lbl_eu_805FBA78;
extern u32 lbl_eu_805FBA7C;

// Create the four worker threads (safe / vsync / fs / mwidle). Each thread's
// stack top is the *end* of a dedicated stack buffer inside the framework state.
// All addresses are derived straight from the global (not via a base local) so
// MWCC keeps them as separate addi webs off the shared anchor, like retail.
// Create the four worker threads (safe / vsync / fs / mwidle). Each thread's
// stack top is the *end* of a dedicated stack buffer inside the framework state.
// NOTE: retail emits each stack top as two addi (buffer base, then + size) and
// materializes &prm once (r31); every source shape we tried (locals, casts,
// volatile, inline helper, global-direct webs) folds to single addi/lwz under
// Wii/1.1 -O4 - same wall class as the svm.c "two-add materialization" note
// in docs/MWCC_CASES.md.
static void adxm_create_base_thread(void) {
    struct AdxmBase* base = &lbl_eu_805F3A50;
    struct AdxParams* prm = &base->field_0x10;

    OSCreateThread(&base->field_0x78, (OSThreadFunc)adxm_safe_proc, NULL,
                   base->field_stack_0x9F8 + 0x1000, 0x1000,
                   (s32)prm->field_0x04, 1);
    OSCreateThread(&base->field_0x19F8, (OSThreadFunc)adxm_vsync_proc, NULL,
                   base->field_stack_0x1D10 + 0x2000, 0x2000,
                   (s32)prm->field_0x0C, 1);
    OSCreateThread(&base->field_0x3D10, (OSThreadFunc)adxm_fs_proc, NULL,
                   base->field_stack_0x4028 + 0x2000, 0x2000,
                   (s32)prm->field_0x10, 1);
    OSCreateThread(&base->field_0x398, (OSThreadFunc)adxm_mwidle_proc, NULL,
                   base->field_stack_0x6028 + 0x2000, 0x2000,
                   (s32)prm->field_0x18, 1);
    lbl_eu_805FBA78 = OSGetCurrentThread();
    if (prm->field_0x14 != 16) {
        u32 irq = OSDisableInterrupts();
        OSDisableScheduler();
        base->field_0x70 = 1;
        s32 prio = OSGetThreadPriority(lbl_eu_805FBA78);
        OSSetThreadPriority(lbl_eu_805FBA78, prio);
        lbl_eu_805FBA7C = (u32)prio;
        base->field_0x70 = 0;
        OSEnableScheduler();
        OSRestoreInterrupts(irq);
    }
}

// Set up (or re-enter) the ADXM thread set. First entry initialises SVM,
// installs the lock/unlock callbacks, copies the caller's parameter block
// (or uses built-in defaults), spawns the base threads and resumes them.
void ADXM_SetupThrd(u8* arg) {
    /* Retail performs a dead volatile read of this counter before anything else */
    (void)lbl_eu_805196D0;
    struct AdxmBase* base = &lbl_eu_805F3A50;

    if (base->field_0x04 == 0) {
        struct AdxParams* prm;
        prm = &base->field_0x10;
        SVM_Init();
        SVM_SetCbLock((void*)adxm_lock, NULL);
        SVM_SetCbUnlock((void*)adxm_unlock, NULL);
        if (arg == NULL) {
            /* Built-in defaults */
            prm->field_0x14 = 0x10;
            base->field_0x10.field_0x00 = 1;
            prm->field_0x04 = 8;
            prm->field_0x0C = 0xC;
            prm->field_0x10 = 0xE;
            prm->field_0x18 = 0x18;
        } else {
            struct AdxSetupArgs* a = (struct AdxSetupArgs*)arg;
            prm->field_0x14 = a->field_0x10;
            base->field_0x10.field_0x00 = a->field_0x00;
            prm->field_0x04 = a->field_0x04;
            prm->field_0x0C = a->field_0x08;
            prm->field_0x10 = a->field_0x0C;
            prm->field_0x18 = a->field_0x14;
        }
        adxm_create_base_thread();
        base->field_0x9E0 = 1;
        base->field_0x9E8 = 1;
        base->field_0x9D8 = 1;
        base->field_0x9D0 = 1;
        base->field_0x9E4 = 0;
        base->field_0x390 = 0;
        base->field_0x9DC = 0;
        base->field_0x9D4 = 0;
        base->field_0x70 = 0;
        OSResumeThread(&base->field_0x19F8);
        OSResumeThread(&base->field_0x3D10);
        OSResumeThread(&base->field_0x398);
        SVM_SetCbBdr(6, (void*)adxm_goto_mwidle_border, NULL);
    }
    base->field_0x04 += 1;
}


extern u32 lbl_eu_805F3A54;
u32 ADXM_IsSetupThrd(void) {
    return (lbl_eu_805F3A54 != 0) ? 1 : 0;
}

extern s32 lbl_eu_805FDD9C;
s32 ADXM_ShutdownFramework(void) {
    s32 result = 1;
    s32 state = lbl_eu_805FDD9C;
    if (state == 0 || state == 2) {
        goto call;
    }
    if (state == 1) {
        goto done;
    }
    goto result0;
call:
    ADXM_ShutdownThrd();
    goto done;
result0:
    result = 0;
done:
    ADXMNG_SetFramework(-1);
    return result;
}

// Drop one reference; only the last release actually tears the threads down.
void ADXM_ShutdownThrd(void) {
    struct AdxmBase* base = &lbl_eu_805F3A50;
    struct AdxParams* prm = &base->field_0x10;
    if (--base->field_0x04 != 0)
        return;
    base->field_0x9E8 = 0;
    OSSetThreadPriority(&base->field_0x398, 1);
    while (base->field_0x390 == 0)
        OSResumeThread(&base->field_0x398);
    OSCancelThread(&base->field_0x19F8);
    OSCancelThread(&base->field_0x3D10);
    base->field_0x9D0 = 0;
    OSResumeThread(&base->field_0x78);
    while (base->field_0x9D4 == 0)
        OSResumeThread(&base->field_0x78);
    if ((s32)prm->field_0x14 != 16) {
        s32 irq = OSDisableInterrupts();
        OSDisableScheduler();
        base->field_0x70 = 1;
        OSSetThreadPriority(lbl_eu_805FBA78, lbl_eu_805FBA7C);
        base->field_0x70 = 0;
        OSEnableScheduler();
        OSRestoreInterrupts(irq);
    }
    SVM_Finish();
}

s32 ADXM_SetupFramework(s32 arg1, u8* arg2) {
    s32 mode = 2;
    s32 ret = 1;
    if (arg1 == 1)
        mode = 1;
    ADXMNG_SetFramework((u32)mode);
    lbl_eu_805FDD9C = arg1;
    switch (arg1) {
    case 0:
    case 2:
        ADXM_SetupThrd(arg2);
        break;
    case 1:
        /* keep ret == 1 */
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}

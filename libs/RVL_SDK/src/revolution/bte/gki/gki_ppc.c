// Decompiled from RVL_SDK/src/revolution/bte/gki/gki_ppc
// GKI OS port: task-id, interrupt nesting, exception / allocation hooks.

#include <harness_catalog.h>
#include <revolution/OS.h>

typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned int   UINT32;

/* Local mirror of the GKI control-block interrupt fields (retail layout).
   The public tGKI_CB carries a modernized com block, so access the byte
   nesting counter / saved-interrupt array directly at their retail
   offsets inside the gki_cb object (OSIntNesting @ 0x00,
   IntDisableCnt @ 0x04). */
typedef struct {
    UINT8   OSIntNesting;                  /* 0x00 */
    UINT32  IntDisableCnt[26];             /* 0x04 */
    UINT8   _pad[0x28802 - 0x6C];          /* up to OSInitFlag */
    UINT8   OSInitFlag;                    /* 0x28802 */
    UINT8   _pad2[0x28808 - 0x28803];      /* 0x28803..0x28807 */
    UINT16  OSWaitEvt[8];                  /* 0x28808 */
} tGKI_INT_MIRROR;

#define GKI_MAX_TASKS 8
#define GKI_SUCCESS   0x00
#define GKI_FAILURE   0x01

extern tGKI_INT_MIRROR gki_cb;

void GKI_disable(void);
void GKI_enable(void);

void GKI_init() {}

void GKI_shutdown(void) {
    GKI_disable();
    gki_cb.OSInitFlag = 0;
    GKI_enable();
    while (gki_cb.OSIntNesting != 0) {
        GKI_enable();
    }
}

void GKI_run(void) {
}

void GKI_sched_lock(void) {
}

void GKI_sched_unlock(void) {
}

void GKI_delay(void) {
}

UINT8 GKI_send_event(UINT8 task_id, UINT16 event) {
    if (task_id >= GKI_MAX_TASKS) {
        return GKI_FAILURE;
    }

    GKI_disable();

    gki_cb.OSWaitEvt[task_id] |= event;

    GKI_enable();

    return GKI_SUCCESS;
}

int GKI_get_taskid(void) { return 0x2; }

/*******************************************************************************
**
** Function         GKI_enable
**
** Description      This function allows re-entrancy of the GKI.
**
** Returns          void
**
*******************************************************************************/
void GKI_enable(void)
{
    gki_cb.OSIntNesting--;
    OSRestoreInterrupts (gki_cb.IntDisableCnt[gki_cb.OSIntNesting]);
}

/*******************************************************************************
**
** Function         GKI_disable
**
** Description      This function disables GKI re-entrancy.
**
** Returns          void
**
*******************************************************************************/
void GKI_disable(void)
{
    gki_cb.IntDisableCnt[gki_cb.OSIntNesting] = OSDisableInterrupts();
    gki_cb.OSIntNesting++;
}

void GKI_exception(void) {}

void* GKI_os_malloc(size_t size) {
    extern void* App_MEMalloc(size_t);
    void* result = App_MEMalloc(size);
    if (result == 0) return 0;
    return result;
}

void GKI_os_free(void) { App_MEMfree(); }

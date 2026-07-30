// High-level C reconstruction of GAP_Init for RVL_SDK BTE stack.
// Zeroes the GAP control block, installs BTM callbacks, sets trace
// level to DEBUG, and initialises the connection subsystem.

#include <string.h>
#include <revolution/BTE/gki/platform/data_types.h>

/* GAP control block structure -- defined here since no public header exists.
   Total size 0x3AC bytes, allocated as a global at 0x805CEAC0. */
typedef struct {
    UINT8  reserved1[0x20];           // 0x00 - 0x1F  (early fields, all zeroed)
    void*  btm_cback0;                // 0x20 - 0x23  BTM callback 0
    void*  btm_cback1;                // 0x24 - 0x27  BTM callback 1
    UINT8  trace_level;               // 0x28         trace verbosity
    UINT8  reserved2[0x3AC - 0x29];   // 0x29 - 0x3AB  remaining fields
} tGAP_CB;

/* The single GAP control-block instance, defined in gap_conn.c
   (linker resolves the symbol). */
extern tGAP_CB gap_cb;

/* Forward declarations for functions in sibling TUs */
extern void gap_btm_cback0(void*);
extern void gap_btm_cback1(void*);
extern void gap_conn_init(void);

void GAP_Init(void)
{
    memset(&gap_cb, 0, sizeof(gap_cb));

    gap_cb.btm_cback0 = gap_btm_cback0;
    gap_cb.btm_cback1 = gap_btm_cback1;
    gap_cb.trace_level = 5;

    gap_conn_init();
}

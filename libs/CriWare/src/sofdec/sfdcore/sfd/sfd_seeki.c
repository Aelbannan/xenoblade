// CriWare Sofdec sfd_seeki: seek-skip control (SFSEEKI_*).
//
// While a seek is in progress the player drops frames until the playback
// position reaches the seek target. The seek-skip state lives in a small
// control block embedded in the player handle at +0x2680; SFSEEKI_IsSkip()
// is polled once per frame and reports the pending seek through the
// software-filter trace callback (gpSfdCoreTraceCb, vtable slot 9).

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

extern void *gpSfdCoreTraceCb;

/* ------------------------------------------------------------------ */
/* SFSEEKI_InitHn                                                      */
/* ------------------------------------------------------------------ */

/* Clear the seek-skip control block (SfdHandle::seeki).
   Store order mirrors the retail codegen. */
void SFSEEKI_InitHn(SfdSeeki* sk) {
    sk->skipCount = 0;      /* 0x00 */
    sk->targetEnd = 0;      /* 0x18 */
    sk->field_0x04 = 0;     /* 0x04 */
    sk->targetTime = 0;     /* 0x08 */
    sk->field_0x0c = 0;     /* 0x0C */
    sk->field_0x10 = 0;     /* 0x10 */
    sk->field_0x14 = 0;     /* 0x14 */
}

/* ------------------------------------------------------------------ */
/* Seek/trace callback dispatch                                       */
/* ------------------------------------------------------------------ */

/* Trace entry filled in before each dispatch, at sTraceSfdIsSkip.
   (The full retail entry is 0x6C bytes; only the head is modeled.) */
typedef struct SfseekiCbArg {
    u32 field_0x0;          /* 0x00 */
    u32 base;               /* 0x04 - &base is passed as the callback's 2nd arg */
    u32 field_0x8;          /* 0x08 */
    SfdHandle* hn;          /* 0x0C - player handle */
    u32 field_0x10;         /* 0x10 */
    u32 field_0x14;         /* 0x14 */
    char* str;              /* 0x18 - trace label */
} SfseekiCbArg;

/* Trace context object; full layout below (forward decl for the callback). */
struct SfseekiCtx;

/* Software-filter trace callback: fn(ctx, entry). */
typedef void (*SfseekiTraceFn)(struct SfseekiCtx* ctx, u32* entry);

/* Generic vtable slot: untyped callback (never invoked from this TU). */
typedef void (*SfseekiVtblFn)(void);

/* Vtable of the trace context; slot 9 (+0x24) is the trace callback. */
typedef struct SfseekiCtxVtbl {
    SfseekiVtblFn field_0x00;   /* 0x00 */
    SfseekiVtblFn field_0x04;   /* 0x04 */
    SfseekiVtblFn field_0x08;   /* 0x08 */
    SfseekiVtblFn field_0x0C;   /* 0x0C */
    SfseekiVtblFn field_0x10;   /* 0x10 */
    SfseekiVtblFn field_0x14;   /* 0x14 */
    SfseekiVtblFn field_0x18;   /* 0x18 */
    SfseekiVtblFn field_0x1C;   /* 0x1C */
    SfseekiVtblFn field_0x20;   /* 0x20 */
    SfseekiTraceFn trace;       /* 0x24 */
} SfseekiCtxVtbl;

/* Trace context object behind gpSfdCoreTraceCb. */
typedef struct SfseekiCtx {
    SfseekiCtxVtbl* vtbl;   /* 0x00 */
} SfseekiCtx;

/* Shared trace entry used by the seek dispatches. Declared here rather than
   in lbls_criware.hpp: the label is excluded from the generated header
   (type conflict with sfd_mpv.c's u32[] usage). */
extern SfseekiCbArg sTraceSfdIsSkip;

/* ------------------------------------------------------------------ */
/* SFSEEKI_IsSkip                                                      */
/* ------------------------------------------------------------------ */

/* Called once per frame while seeking: returns 1 when the frame should be
   dropped. While the seek target has not been reached it fills the shared
   trace entry (sTraceSfdIsSkip) and dispatches the software-filter trace
   callback with the "before seeki" / "after seeki" label. */
int SFSEEKI_IsSkip(SfdHandle* hn, u32* outSkipCount) {
    *outSkipCount = hn->seeki.skipCount;
    if (hn->seeki.skipCount == 0) {
        return 0;
    }

    if (hn->field_0x0964 < hn->seeki.targetEnd) {
        /* seek target not reached yet: keep dropping frames */
        if (gpSfdCoreTraceCb != NULL) {
            sTraceSfdIsSkip.hn = hn;
            sTraceSfdIsSkip.str = "before seeki";
            ((SfseekiCtx*)gpSfdCoreTraceCb)->vtbl->trace((SfseekiCtx*)gpSfdCoreTraceCb, &sTraceSfdIsSkip.base);
        }
        return 1;
    }

    if (hn->field_0x0960 >= hn->seeki.targetTime) {
        /* seek target passed: keep dropping frames */
        if (gpSfdCoreTraceCb != NULL) {
            sTraceSfdIsSkip.hn = hn;
            sTraceSfdIsSkip.str = "after seeki";
            ((SfseekiCtx*)gpSfdCoreTraceCb)->vtbl->trace((SfseekiCtx*)gpSfdCoreTraceCb, &sTraceSfdIsSkip.base);
        }
        return 1;
    }

    return 0;
}

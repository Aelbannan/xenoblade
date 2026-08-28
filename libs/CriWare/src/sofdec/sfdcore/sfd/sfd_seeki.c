// CriWare Sofdec sfd_seeki: seek-skip control (SFSEEKI_*).
//
// While a seek is in progress the player drops frames until the playback
// position reaches the seek target. The seek-skip state lives in a small
// control block embedded in the player handle at +0x2680; SFSEEKI_IsSkip()
// is polled once per frame and reports the pending seek through the
// software-filter trace callback (lbl_eu_80606E34, vtable slot 9).

#include <harness_catalog.h>
#include "lbls_criware.hpp"

// Absorbed from criware_data (retail bytes) - generated
__declspec(section ".rodata") __attribute__((aligned(8))) char lbl_eu_8051CEC0[0x20] = {"before seeki\x00""after seeki\x00""\x00""\x00""\x00""\x00""\x00""\x00"}; // off 0x0

/* ------------------------------------------------------------------ */
/* Seek-skip control block                                            */
/* ------------------------------------------------------------------ */

/* Embedded in the player handle at +0x2680; SFSEEKI_InitHn() clears it. */
typedef struct SfseekiHn {
    u32 skipCount;          /* 0x00 - frames still to skip */
    u32 field_0x04;         /* 0x04 */
    u32 field_0x08;         /* 0x08 - seek target */
    u32 field_0x0C;         /* 0x0C */
    u32 field_0x10;         /* 0x10 */
    u32 field_0x14;         /* 0x14 */
    u32 field_0x18;         /* 0x18 - seek target */
} SfseekiHn;                /* 0x1C bytes */

/* Player-handle view covering the fields the seek logic reads. */
typedef struct SfplyHnSeekiView {
    u8 pad_0x00[0x960];
    u32 field_0x960;        /* 0x960 */
    u32 field_0x964;        /* 0x964 */
    u8 pad_0x968[0x2680 - 0x968];
    SfseekiHn seeki;        /* 0x2680 */
} SfplyHnSeekiView;

/* ------------------------------------------------------------------ */
/* SFSEEKI_InitHn                                                      */
/* ------------------------------------------------------------------ */

/* Clear the seek-skip control block (player handle +0x2680).
   Store order mirrors the retail codegen. */
void SFSEEKI_InitHn(SfseekiHn* hn) {
    hn->skipCount = 0;      /* 0x00 */
    hn->field_0x18 = 0;     /* 0x18 */
    hn->field_0x04 = 0;     /* 0x04 */
    hn->field_0x08 = 0;     /* 0x08 */
    hn->field_0x0C = 0;     /* 0x0C */
    hn->field_0x10 = 0;     /* 0x10 */
    hn->field_0x14 = 0;     /* 0x14 */
}

/* ------------------------------------------------------------------ */
/* Seek/trace callback dispatch                                       */
/* ------------------------------------------------------------------ */

/* Trace entry filled in before each dispatch, at lbl_eu_80568CF0.
   (The full retail entry is 0x6C bytes; only the head is modeled.) */
typedef struct SfseekiCbArg {
    u32 field_0x0;          /* 0x00 */
    u32 base;               /* 0x04 - &base is passed as the callback's 2nd arg */
    u32 field_0x8;          /* 0x08 */
    SfplyHnSeekiView* hn;   /* 0x0C - player handle */
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

/* Trace context object behind lbl_eu_80606E34. */
typedef struct SfseekiCtx {
    SfseekiCtxVtbl* vtbl;   /* 0x00 */
} SfseekiCtx;

/* Shared trace entry used by the seek dispatches. Declared here rather than
   in lbls_criware.hpp: the label is excluded from the generated header
   (type conflict with sfd_mpv.c's u32[] usage). */
extern SfseekiCbArg lbl_eu_80568CF0;

/* ------------------------------------------------------------------ */
/* SFSEEKI_IsSkip                                                      */
/* ------------------------------------------------------------------ */

/* Called once per frame while seeking: returns 1 when the frame should be
   dropped. While the seek target has not been reached it fills the shared
   trace entry (lbl_eu_80568CF0) and dispatches the software-filter trace
   callback with the "before seeki" / "after seeki" label. */
int SFSEEKI_IsSkip(SfplyHnSeekiView* hn, u32* outSkipCount) {
    *outSkipCount = hn->seeki.skipCount;
    if (hn->seeki.skipCount == 0) {
        return 0;
    }

    if (hn->field_0x964 < hn->seeki.field_0x18) {
        /* seek target not reached yet: keep dropping frames */
        if (lbl_eu_80606E34 != NULL) {
            SfseekiCtx* ctx = (SfseekiCtx*)lbl_eu_80606E34;
            SfseekiCbArg* cb = &lbl_eu_80568CF0;
            cb->hn = hn;
            cb->str = &lbl_eu_8051CEC0[0];      /* "before seeki" */
            ctx->vtbl->trace(ctx, &cb->base);
        }
        return 1;
    }

    if (hn->field_0x960 >= hn->seeki.field_0x08) {
        /* seek target passed: keep dropping frames */
        if (lbl_eu_80606E34 != NULL) {
            SfseekiCtx* ctx = (SfseekiCtx*)lbl_eu_80606E34;
            SfseekiCbArg* cb = &lbl_eu_80568CF0;
            cb->hn = hn;
            cb->str = &lbl_eu_8051CEC0[0x0D];   /* "after seeki" */
            ctx->vtbl->trace(ctx, &cb->base);
        }
        return 1;
    }

    return 0;
}

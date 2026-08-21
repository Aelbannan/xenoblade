// sfd_uo.c - CriWare Sofdec user-operation (UO) unit
//
// Provides the "user operation" channel of an SofDec movie handle: a small
// table of 16-byte-stride entries (one per stream index) that the buffer
// unit (SFBUF) watches via SFBUF_SetUoch. Most lifecycle entry points are
// no-ops in this build; only SetUsrSj / ExecServer / Create carry logic.

#include <harness_catalog.h>

/* View of the SofDec movie handle fields touched by the UO unit.
 * The UO entry table is a small array of 16-byte records embedded in the
 * handle at 0x2638; its base address is cached in the handle at 0x2200,
 * next to an opaque state word at 0x2208. */
typedef struct SfdUoEntry {
    /* 0x00 */ u32 field_0x00;
    /* 0x04 */ u32 field_0x04; /* callback/function word handed to SFBUF */
    /* 0x08 */ u32 field_0x08;
    /* 0x0C */ u32 field_0x0C;
} SfdUoEntry; /* 16-byte stride */

typedef struct SfdHandle {
    /* 0x0000 */ u8 _00[0x2200];
    /* 0x2200 */ SfdUoEntry* uoTable;
    /* 0x2204 */ u8 _2204[4];
    /* 0x2208 */ s32 state;
    /* 0x220C */ u8 _220C[0x2638 - 0x220C];
    /* 0x2638 */ SfdUoEntry uoEntries[4];
} SfdHandle;

/* sfd_lib.c */
extern s32 SFLIB_SetErr(s32 val, u32 err_code);
extern s32 SFLIB_CheckHn(SfdHandle* h);
/* sfd_buf.c */
extern void SFBUF_SetUoch(SfdHandle* self, s32 state, s32 idx, u32* entry);
extern s32 SFBUF_GetTermFlg(SfdHandle* self, s32 idx);
extern s32 SFBUF_GetPrepFlg(SfdHandle* self, s32 idx);
/* sfd_trn.c */
extern s32 SFTRN_GetTermFlg(SfdHandle* self, s32 ch);
extern s32 SFTRN_GetPrepFlg(SfdHandle* self, s32 ch);
extern void SFTRN_SetTermFlg(SfdHandle* self, s32 ch, s32 val);
extern void SFTRN_SetPrepFlg(SfdHandle* self, s32 ch, s32 val);

/*
 * Register a user-operation entry for stream `idx`.
 *
 * Each record's payload words at +4/+8/+12/+16 are written; the +4 word
 * address is what gets handed to the buffer unit together with the opaque
 * state word from the handle.
 */
s32 SFD_SetUsrSj(SfdHandle* self, u32 idx, u32 b, u32 c) {
    if (SFLIB_CheckHn(self))
        return SFLIB_SetErr(NULL, 0xFF000191);
    {
        s32 state = self->state;
        SfdUoEntry* base = self->uoTable;
        if (state == 8)
            return SFLIB_SetErr((s32)self, 0xFF000602);
        {
            SfdUoEntry* entry = &base[idx];
            entry->field_0x04 = b;
            entry->field_0x08 = c;
            entry->field_0x0C = 0;
            entry[1].field_0x00 = 0;
            SFBUF_SetUoch(self, state, idx, &entry->field_0x04);
        }
    }
    return 0;
}

int SFUO_Init(void) { return 0x0; }

int SFUO_Finish(void) { return 0x0; }

/*
 * Per-frame server callback: mirror the buffer unit's terminate/prepare
 * status (channel 8) into the transport-unit flags once both sides agree.
 */
s32 SFUO_ExecServer(SfdHandle* self) {
    if (SFTRN_GetTermFlg(self, 8) != 1 && SFBUF_GetTermFlg(self, self->state) == 1) {
        SFTRN_SetTermFlg(self, 8, 1);
    }
    if (SFTRN_GetPrepFlg(self, 8) != 1 && SFBUF_GetPrepFlg(self, self->state) == 1) {
        SFTRN_SetPrepFlg(self, 8, 1);
    }
    return 0;
}

/*
 * Initialise the UO entry table.
 *
 * The table lives inside the handle (its address is also stored back into
 * the uoTable cache field); each of the 3 records has its four payload
 * words cleared and is registered with SFBUF_SetUoch together with the
 * opaque state word.
 */
s32 SFUO_Create(SfdHandle* self) {
    s32 zero = 0;
    s32 i;
    SfdUoEntry* entry = self->uoEntries;
    u32* payload = &entry->field_0x04;
    self->uoTable = entry;
    {
        s32 state = self->state;
        entry->field_0x00 = zero;
        i = 0;
        do {
            entry->field_0x04 = zero;
            entry->field_0x08 = zero;
            entry->field_0x0C = zero;
            entry[1].field_0x00 = zero;
            SFBUF_SetUoch(self, state, i, payload);
            i++;
            entry++;
            payload += 4;
        } while (i < 3);
    }
    return 0;
}

int SFUO_Destroy(void) { return 0x0; }

int SFUO_RequestStop(void) { return 0x0; }

int SFUO_Start(void) { return 0x0; }

int SFUO_Stop(void) { return 0x0; }

int SFUO_Pause(void) { return 0x0; }

/* Read/write accessors are unsupported on this unit's channel type. */
s32 SFUO_GetWrite(SfdHandle* h) {
    return SFLIB_SetErr((s32)h, 0xff000601);
}

s32 SFUO_AddWrite(SfdHandle* h) {
    return SFLIB_SetErr((s32)h, 0xff000601);
}

s32 SFUO_GetRead(SfdHandle* h) {
    return SFLIB_SetErr((s32)h, 0xff000601);
}

s32 SFUO_AddRead(SfdHandle* h) {
    return SFLIB_SetErr((s32)h, 0xff000601);
}

int SFUO_Seek(void) { return 0x0; }

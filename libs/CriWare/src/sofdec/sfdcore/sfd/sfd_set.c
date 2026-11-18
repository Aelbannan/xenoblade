// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_set
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(s32 val, u32 err_code);
u32 SFD_GetHnStat(SfdHandle* self) {
    if (SFLIB_CheckHn(self))
        SFLIB_SetErr(0, 0xff000111);
    return self->errorState;
}

extern u32 lbl_eu_80606E38[]; /* SFD work area */

s32 SFD_SetCond(SfdHandle* self, s32 idx, s32 val) {
    if (self == NULL) {
        s32 ok;
        void* h;
        s32 i;
        u32* p = lbl_eu_80606E38 + 0x7F;
        s32 off = idx * 4;
        for (i = 0; i < 8; i++, p++) {
            h = (void*)*p;
            if (SFLIB_CheckHn(h) != 0)
                continue;
            if (idx == 6 && val == 1 && SFTRN_IsSetup(h, 3) == 0)
                ok = 0;
            else if (idx == 5 && val == 1 && SFTRN_IsSetup(h, 2) == 0)
                ok = 0;
            else
                ok = 1;
            if (ok)
                *(u32*)((u8*)h + (off + 0xA1C)) = val;
        }
        *(u32*)((u8*)lbl_eu_80606E38 + off) = val;
        } else {
        if (SFLIB_CheckHn(self) != 0)
            return SFLIB_SetErr(0, 0xFF000112);

        {
            s32 ok;
            if (idx == 6 && val == 1 && SFTRN_IsSetup(self, 3) == 0)
                ok = 0;
            else if (idx == 5 && val == 1 && SFTRN_IsSetup(self, 2) == 0)
                ok = 0;
            else
                ok = 1;
            if (ok)
                self->cond[idx] = val;
        }
        {
            s32 ok;
            if (idx == 6 && val == 1 && SFTRN_IsSetup(self, 3) == 0)
                ok = 0;
            else if (idx == 5 && val == 1 && SFTRN_IsSetup(self, 2) == 0)
                ok = 0;
            else
                ok = 1;
            if (ok)
                self->condBak[idx] = val;
        }
    }
    return 0;
}

void SFSET_SetCond(SfdHandle* self, s32 idx, s32 val) {
    s32 ok;
    if (idx == 6 && val == 1 && SFTRN_IsSetup(self, 3) == 0)
        ok = 0;
    else if (idx == 5 && val == 1 && SFTRN_IsSetup(self, 2) == 0)
        ok = 0;
    else
        ok = 1;
    if (ok)
        self->cond[idx] = val;
}

s32 SFD_GetCond(SfdHandle* self, u32 idx, s32* out) {
    if (self == NULL) {
        *out = (s32)lbl_eu_80606E38[idx];
    } else {
        if (SFLIB_CheckHn(self) != 0)
            return SFLIB_SetErr(0, 0xFF000113);
        *out = self->cond[idx];
    }
    return 0;
}

u32 SFSET_GetCond(SfdHandle* self, u32 idx) {
    return self->cond[idx];
}

void SFD_SetSfdHeaderFn(SfdHandle* self, void* fn, void* ctx) {
    self->hdrCbArg = ctx;
    self->hdrCb = fn;
}

s32 SFD_GetSofdecHeader(SfdHandle* self, s32 idx, void** out) {
    if (SFLIB_CheckHn(self) != 0)
        return SFLIB_SetErr(0, 0xFF000117);
    {
        void* hdr = self->trn[idx].hdr;
        if (hdr == NULL) {
            *out = NULL;
        } else {
            *out = *(void**)hdr;
        }
    }
    return 0;
}

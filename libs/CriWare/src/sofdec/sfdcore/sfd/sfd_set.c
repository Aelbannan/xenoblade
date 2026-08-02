// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_set
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(s32 val, u32 err_code);
u32 SFD_GetHnStat(void* self) {
    if (SFLIB_CheckHn(self))
        SFLIB_SetErr(0, 0xff000111);
    return *(u32*)((u8*)self + 0x54);
}

extern u32 lbl_eu_80606E38[]; /* SFD work area */

s32 SFD_SetCond(void* self, s32 idx, s32 val) {
    if (self == NULL) {
        s32 off = idx * 4;
        u32* p = lbl_eu_80606E38 + 0x7F;
        s32 i;
        for (i = 0; i < 8; i++) {
            void* h = (void*)*p;
            p++;
            s32 ok;
            if (SFLIB_CheckHn(h) != 0)
                continue;
            if (idx == 6 && val == 1 && SFTRN_IsSetup(h, 3) == 0)
                ok = 0;
            else if (idx == 5 && val == 1 && SFTRN_IsSetup(h, 2) == 0)
                ok = 0;
            else
                ok = 1;
            if (ok)
                *(u32*)((u8*)h + off + 0xA1C) = val;
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
                *(u32*)((u8*)self + idx * 4 + 0xA1C) = val;
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
                *(u32*)((u8*)self + idx * 4 + 0xBAC) = val;
        }
    }
    return 0;
}

void SFSET_SetCond(void* self, s32 idx, s32 val) {
    s32 ok;
    if (idx == 6 && val == 1 && SFTRN_IsSetup(self, 3) == 0)
        ok = 0;
    else if (idx == 5 && val == 1 && SFTRN_IsSetup(self, 2) == 0)
        ok = 0;
    else
        ok = 1;
    if (ok)
        *(u32*)((u8*)self + idx * 4 + 0xA1C) = val;
}

s32 SFD_GetCond(void* self, u32 idx, s32* out) {
    if (self == NULL) {
        *out = (s32)lbl_eu_80606E38[idx];
    } else {
        if (SFLIB_CheckHn(self) != 0)
            return SFLIB_SetErr(0, 0xFF000113);
        *out = *(s32*)((u8*)self + idx * 4 + 0xA1C);
    }
    return 0;
}

u32 SFSET_GetCond(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0xa1c + idx * 4);
}

void SFD_SetSfdHeaderFn(void* self, void* fn, void* ctx) {
    *(void**)((u8*)self + 0xd3c) = fn;
    *(void**)((u8*)self + 0xd40) = ctx;
}

void SFD_GetSofdecHeader() {}

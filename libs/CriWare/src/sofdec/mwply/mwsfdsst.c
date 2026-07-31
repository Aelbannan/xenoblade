// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsst
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

/* ---- Shared types for the Sofdec SST (stream set) subsystem ---- */

typedef struct MWSFDPLY MWSFDPLY;
typedef struct SstHn SstHn;
typedef struct SstObj SstObj;
typedef struct SstHnObj SstHnObj;
typedef struct SstHnObjVtable SstHnObjVtable;
typedef struct SstCore SstCore;

/* Object referenced from SstHn::obj (0x08). Its method table lives inline in
 * the object memory, so the SJ control methods are plain members. */
struct SstObj {
    u8 pad_0x00[0x14];
    void* (*startSj)(void* handle, SstHnObj* hn);  /* 0x14 */
    void* (*stop)(void* handle);                   /* 0x18 */
    s32 (*getStat)(void* handle);                  /* 0x1c */
    u8 pad_0x20[0x4];
    void* (*getTime)(void* handle, s32 mode);      /* 0x24 */
    void* (*setOutVol)(void* handle, s32 vol);     /* 0x28 */
    s32 (*getOutVol)(void* handle);                /* 0x2c */
    void* (*setLnkSw)(void* handle, s32 sw);       /* 0x30 */
    void* (*create)(void* handle);                 /* 0x34 */
};

/* Object referenced from SstHn::hn (0x14); vtable methods at 0x0c and 0x14. */
struct SstHnObjVtable {
    u8 pad_0x00[0x0c];
    void (*vf0c)(SstHnObj* self);                  /* 0x0c */
    u8 pad_0x10[0x4];
    void (*vf14)(SstHnObj* self);                  /* 0x14 */
};

struct SstHnObj {
    const SstHnObjVtable* vtable;
};

/* Registered SST core object; methods at 0x04 (terminate) and 0x10 (release
 * the SST handle) live inline in the object memory. */
struct SstCore {
    u8 pad_0x00[0x4];
    void (*term)(SstCore* self);                   /* 0x04 */
    u8 pad_0x08[0x8];
    void (*release)(void* handle);                 /* 0x10 */
};

/* Global table of registered SST cores (2 entries of {core, refcount}). */
typedef struct SstCoreTblEntry {
    SstCore* core;                                 /* 0x00 */
    s32 refcount;                                  /* 0x04 */
} SstCoreTblEntry;

extern SstCoreTblEntry lbl_eu_80602A00[];

/* SST (stream set) handle: also the array element at MWSFDPLY+0x5d8 with a
 * stride of 0x28. */
struct SstHn {
    s32 state;                                     /* 0x00 (1 = active) */
    s32 type;                                      /* 0x04 (index into lbl_eu_80602A00) */
    SstObj* obj;                                   /* 0x08 */
    u8 pad_0x0c[0x8];                              /* 0x0c..0x13 */
    SstHnObj* hn;                                  /* 0x14 */
    s32 flag;                                      /* 0x18 */
    s32 outChan;                                   /* 0x1c */
    s32 outSj;                                     /* 0x20 (-1 = none) */
    void* handle;                                  /* 0x24 */
};

/* MWSFD movie-player handle; only the SFD handle and the SST array are
 * touched by this TU. */
struct MWSFDPLY {
    u8 pad_0x00[0x58];
    void* sfdHn;                                   /* 0x58 */
    u8 pad_0x5c[0x57c];                            /* 0x5c..0x5d7 */
    SstHn sst[2];                                  /* 0x5d8..0x627 */
};

/* ---- External helpers ---- */

void SFD_SetElementOutSj(void* sfd, s32 ch, void* elem, s32 a, s32 b);
void MWSFSVM_GotoIdleBorder(void);

/* ---- Internal helpers ---- */

static inline s32 MWSST_IsActive(const SstHn* sst) {
    if (lbl_eu_80602A00[sst->type].core == NULL) {
        return 0;
    }
    if (sst->state != 1) {
        return 0;
    }
    return sst->handle != NULL;
}

/* ---- Target implementations ---- */

/* (Re)route the SST output channel of one stream set: detach any previously
 * attached output element, then attach the stream set's SJ output element when
 * the set is flagged as active. */
void MWSFD_SetSstCh(MWSFDPLY* self, s32 idx, s32 ch) {
    SstHn* sst = &self->sst[idx];
    SstHnObj* hn = sst->hn;
    s32 outChan = sst->outChan;
    if (sst->outSj != -1) {
        SFD_SetElementOutSj(self->sfdHn, outChan + 0xc0, NULL, 0, 0);
        sst->outSj = -1;
    }
    if (sst->flag == 1) {
        SFD_SetElementOutSj(self->sfdHn, ch + 0xc0, hn, 0, 0);
        sst->outSj = ch;
    }
    sst->outChan = ch;
}

/* Detach the output element of one stream set, if any is attached. */
void criware_803A326C(MWSFDPLY* self, s32 idx) {
    SstHn* sst = &self->sst[idx];
    s32 outChan = sst->outChan;
    if (sst->outSj != -1) {
        SFD_SetElementOutSj(self->sfdHn, outChan + 0xc0, 0, 0, 0);
        sst->outSj = -1;
    }
}

/* Get the SST core handle by index. */
void* mwPlyGetSstCoreHnByIdx(MWSFDPLY* self, s32 idx) {
    return self->sst[idx].handle;
}

/* Start SJ (SoftDec audio) output for an active stream set. */
void* MWSST_StartSj(SstHn* sst) {
    if (MWSST_IsActive(sst) == 1) {
        SstObj* obj = sst->obj;
        void* h = sst->handle;
        SstHnObj* hn = sst->hn;
        if (obj != NULL && obj->startSj != NULL) {
            return obj->startSj(h, hn);
        }
    }
    return (void*)sst;
}

/* Stop SJ output for an active stream set. */
void MWSST_Stop(SstHn* sst) {
    if (MWSST_IsActive(sst) == 1) {
        void* h = sst->handle;
        SstObj* obj = sst->obj;
        if (h != NULL && obj != NULL && obj->stop != NULL) {
            obj->stop(h);
        }
    }
}

/* Return the SJ status of an active stream set, or 0. */
s32 MWSST_GetStat(SstHn* sst) {
    s32 stat = 0;
    if (MWSST_IsActive(sst) != 1) {
        return 0;
    }
    SstObj* obj = sst->obj;
    void* h = sst->handle;
    if (obj != NULL && obj->getStat != NULL) {
        stat = obj->getStat(h);
    }
    return stat;
}

/* Return the SJ playback time of an active stream set for the given mode. */
void MWSST_GetTime(SstHn* sst, s32 mode) {
    if (MWSST_IsActive(sst) == 1) {
        SstObj* obj = sst->obj;
        void* h = sst->handle;
        if (obj != NULL && obj->getTime != NULL) {
            obj->getTime(h, mode);
        }
    }
}

/* Set the SJ output volume of an active stream set. */
void MWSST_SetOutVol(SstHn* sst, s32 vol) {
    if (MWSST_IsActive(sst) == 1) {
        SstObj* obj = sst->obj;
        void* h = sst->handle;
        if (obj != NULL && obj->setOutVol != NULL) {
            obj->setOutVol(h, vol);
        }
    }
}

/* Return the SJ output volume of an active stream set, or 0. */
s32 MWSST_GetOutVol(SstHn* sst) {
    s32 vol = 0;
    if (MWSST_IsActive(sst) != 1) {
        return 0;
    }
    SstObj* obj = sst->obj;
    void* h = sst->handle;
    if (obj != NULL && obj->getOutVol != NULL) {
        vol = obj->getOutVol(h);
    }
    return vol;
}

/* Set the link-switch flag of an active stream set. */
void MWSST_SetLnkSw(SstHn* sst, s32 sw) {
    if (MWSST_IsActive(sst) == 1) {
        SstObj* obj = sst->obj;
        void* h = sst->handle;
        if (obj != NULL && obj->setLnkSw != NULL) {
            obj->setLnkSw(h, sw);
        }
    }
}

/* (Re)create the SJ output of an active stream set. */
void MWSST_Create(SstHn* sst) {
    if (MWSST_IsActive(sst) == 1) {
        SstObj* obj = sst->obj;
        void* h = sst->handle;
        if (obj != NULL && obj->create != NULL) {
            obj->create(h);
        }
    }
}

/* Reset one stream set: stop its SJ output while active, then notify the
 * stream set handle object. */
void MWSST_Reset(MWSFDPLY* self, s32 idx) {
    SstHn* sst = &self->sst[idx];
    SstHnObj* hn = sst->hn;
    if (MWSST_IsActive(sst) == 1) {
        if (MWSST_IsActive(sst) == 1) {
            void* h = sst->handle;
            SstObj* obj = sst->obj;
            if (h != NULL && obj != NULL && obj->stop != NULL) {
                obj->stop(h);
            }
        }
        hn->vtable->vf14(hn);
    }
}

/* Destroy an active stream set: leave the idle border, stop SJ output, clear
 * the state, release the core handle, tear down the handle object, and drop
 * the core registration (terminating the core when the last reference goes). */
void MWSST_Destroy(SstHn* sst) {
    SstCoreTblEntry* coreTbl = &lbl_eu_80602A00[sst->type];
    s32 active = 0;
    if (lbl_eu_80602A00[sst->type].core == NULL) {
        active = 0;
    } else if (sst->state != 1) {
        active = 0;
    } else {
        active = (sst->handle != NULL);
    }
    if (active == 1) {
        void* h = sst->handle;
        SstHnObj* hn = sst->hn;
        if (h != NULL) {
            MWSFSVM_GotoIdleBorder();
            if (MWSST_IsActive(sst) == 1) {
                void* h2 = sst->handle;
                SstObj* obj = sst->obj;
                if (h2 != NULL && obj != NULL && obj->stop != NULL) {
                    obj->stop(h2);
                }
            }
            sst->state = 0;
            SstCore* core = coreTbl->core;
            if (h != NULL && core != NULL && core->release != NULL) {
                core->release(h);
            }
            hn->vtable->vf0c(hn);
            sst->handle = NULL;
            core = coreTbl->core;
            if (core != NULL && coreTbl->refcount != 0) {
                coreTbl->refcount--;
                if (coreTbl->refcount == 0 && core->term != NULL) {
                    core->term(core);
                }
            }
        }
    }
}

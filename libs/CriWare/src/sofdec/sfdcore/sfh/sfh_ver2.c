// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfh/sfh_ver2
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern int SFHLOCAL_GetSizeofMember(int, int);
extern int SFHLOCAL_GetNbyteB(const void *, int);
extern u64 SFHLOCAL_GetNbyteB64(const void *, int);
extern int memcmp(const void *, const void *, unsigned long);
extern void *memset(void *, int, unsigned long);
extern void *memcpy(void *, const void *, unsigned long);
extern char *strstr(const char *, const char *);
extern char lbl_eu_8051CF00[];
extern int SFH_AnlyNumElemAud(void *, int *);
extern int SFH_AnlyNumElemVid(void *, int *);

u8 *searchStmId(void *work, u32 stm_id) {
    u8 *base;
    u8 *cur;
    u8 *found = NULL;
    int n_aud;
    int n_vid;
    u32 kind;
    u32 index;
    u32 is_v2;
    int i;

    base = ((u8 **)work)[1];
    if (*(s32 *)((u8 *)work + 0x10) < 0xCA) {
        is_v2 = 0;
        if (!SFH_AnlyNumElemAud(work, &n_aud)) {
            n_aud = 0x20;
        }
        if (!SFH_AnlyNumElemVid(work, &n_vid)) {
            n_vid = 0x10;
        }
    } else {
        n_aud = 0;
        n_vid = 0;
        is_v2 = 1;
    }
    kind = stm_id & 0xE0;
    index = stm_id & 0x1F;
    switch (kind) {
    case 0xC0:
        if (is_v2 != 0) {
            cur = base + (index << 4) + 0x1C0;
            if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
                found = cur;
            }
        } else {
            i = 0;
            cur = base + 0x1C0;
            do {
                if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
                    found = cur;
                    break;
                }
                i++;
                cur += 0x10;
            } while (i < n_aud);
        }
        break;
    case 0xE0:
        if (is_v2 != 0) {
            cur = base + (index << 6) + 0x3C0;
            if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
                found = cur;
            }
        } else {
            i = 0;
            cur = base + 0x3C0;
            do {
                if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
                    found = cur;
                    break;
                }
                i++;
                cur += 0x40;
            } while (i < n_vid);
        }
        break;
    case 0xA0:
        cur = base + 0x7C0;
        if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
            found = cur;
        }
        cur = base + 0x7E0;
        if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
            found = cur;
        }
        break;
    }
    return found;
}

int VER2_IsSfdHeader(void *work, int *out) {
    char buf[0x40];
    char *p;
    u32 major;
    u32 minor;
    u32 t1 = 0;
    u32 t2 = 0;
    u32 v1 = 0;
    u32 v2 = 0;
    u32 ok = 0;
    char c;

    *out = 0;
    if (memcmp(((u8 **)work)[1] + 0x20, lbl_eu_8051CF00, 0x18) != 0) {
        *(s32 *)work = -1;
        return 0;
    }
    major = SFHLOCAL_GetNbyteB(((u8 **)work)[1] + 0x38, SFHLOCAL_GetSizeofMember(0x38, 0x39));
    minor = SFHLOCAL_GetNbyteB(((u8 **)work)[1] + 0x39, SFHLOCAL_GetSizeofMember(0x39, 0x3A));
    *(u32 *)((u8 *)work + 0x10) = (major << 8) | minor;
    memset(buf, 0, 0x40);
    memcpy(buf, ((u8 **)work)[1] + 0x40, 0x40);
    p = strstr(buf, lbl_eu_8051CF00 + 0x19);
    if (p != NULL) {
        p += 4;
        for (;;) {
            c = *p;
            if (c == '.' || c == ' ' || c == 0) {
                break;
            }
            u32 d = 0;
            if ((u32)(c - '0') <= 9) {
                d = 1;
            }
            if (d == 0) {
                break;
            }
            t1 = t1 * 10 + c - '0';
            p++;
        }
        p++;
        for (;;) {
            c = *p;
            if (c == '.' || c == ' ' || c == 0) {
                break;
            }
            u32 d = 0;
            if ((u32)(c - '0') <= 9) {
                d = 1;
            }
            if (d == 0) {
                break;
            }
            t2 = t2 * 10 + c - '0';
            p++;
        }
        ok = 1;
    }
    if (ok != 0) {
        v1 = t1;
        v2 = t2;
    }
    ok = (ok != 0);
    if (ok != 0) {
        *(u32 *)((u8 *)work + 0xC) = (v1 << 8) | v2;
    }
    major = SFHLOCAL_GetNbyteB(((u8 **)work)[1] + 0x3A, SFHLOCAL_GetSizeofMember(0x3A, 0x3B));
    minor = SFHLOCAL_GetNbyteB(((u8 **)work)[1] + 0x3B, SFHLOCAL_GetSizeofMember(0x3B, 0x3C));
    *(u32 *)((u8 *)work + 0x14) = (major << 8) | minor;
    *(u32 *)work = 2;
    *out = 1;
    return 1;
}

int VER2_IsExistStmId(void *work, u32 stm_id, u32 *out) {
    *out = 0;
    if (searchStmId(work, stm_id) != 0) {
        *out = 1;
    } else {
        *out = 0;
    }
    return *out;
}

int VER2_IsEffFtrInf(void *work, int stm_id, u32 *out) {
    int kind;
    u8 *f;

    *out = 0;
    if ((u32)(stm_id - 0xC0) <= 0x1F) {
        kind = 0xC0;
    } else if ((u32)(stm_id - 0xE0) <= 0x0F) {
        kind = 0xE0;
    } else if (stm_id == 0xBD || stm_id == 0xBF) {
        kind = 0xBD;
    } else {
        kind = 0;
    }
    if (kind != 0xE0) {
        return 0;
    }
    f = searchStmId(work, (u32)stm_id);
    if (f == 0) {
        return 0;
    }
    u32 r = (SFHLOCAL_GetNbyteB(f + 0x20, SFHLOCAL_GetSizeofMember(0x20, 0x21)) != 0);
    *out = r;
    return r;
}

int VER2_AnlyHdrToolVer(void *work, u32 *out1, u32 *out2) {
    char buf[0x40];
    char *p;
    u32 t1;
    u32 t2;
    u32 ok;
    char c;

    *out1 = 0;
    *out2 = 0;
    memset(buf, 0, 0x40);
    memcpy(buf, ((u8 **)work)[1] + 0x40, 0x40);
    t1 = 0;
    t2 = 0;
    p = strstr(buf, lbl_eu_8051CF00 + 0x19);
    if (p != NULL) {
        p += 4;
        for (;;) {
            c = *p;
            if (c == '.' || c == ' ' || c == 0) {
                break;
            }
            u32 d = 0;
            if ((u32)(c - '0') <= 9) {
                d = 1;
            }
            if (d == 0) {
                break;
            }
            t1 = t1 * 10 + c - '0';
            p++;
        }
        p++;
        for (;;) {
            c = *p;
            if (c == '.' || c == ' ' || c == 0) {
                break;
            }
            u32 d = 0;
            if ((u32)(c - '0') <= 9) {
                d = 1;
            }
            if (d == 0) {
                break;
            }
            t2 = t2 * 10 + c - '0';
            p++;
        }
        ok = 1;
    }
    if (ok == 0) {
        return 0;
    }
    *out1 = t1;
    *out2 = t2;
    return 1;
}

int VER2_AnlyHdrSfhVer(void* work, unsigned int* out1, unsigned int* out2) {
    int sz;
    *out1 = 0;
    *out2 = 0;
    sz = SFHLOCAL_GetSizeofMember(0x38, 0x39);
    *out1 = SFHLOCAL_GetNbyteB(*(char**)((char*)work + 4) + 0x38, sz);
    sz = SFHLOCAL_GetSizeofMember(0x39, 0x3a);
    *out2 = SFHLOCAL_GetNbyteB(*(char**)((char*)work + 4) + 0x39, sz);
    return 1;
}

int VER2_AnlyHdrModuleVer(void* work, unsigned int* out1, unsigned int* out2) {
    int sz;
    *out1 = 0;
    *out2 = 0;
    sz = SFHLOCAL_GetSizeofMember(0x3a, 0x3b);
    *out1 = SFHLOCAL_GetNbyteB(*(char**)((char*)work + 4) + 0x3a, sz);
    sz = SFHLOCAL_GetSizeofMember(0x3b, 0x3c);
    *out2 = SFHLOCAL_GetNbyteB(*(char**)((char*)work + 4) + 0x3b, sz);
    return 1;
}


int VER2_AnlyHdrSiz(const unsigned int *hdr, int *out_size) {
    int sz;
    *out_size = 0;
    sz = SFHLOCAL_GetSizeofMember(0x12, 0x14);
    *out_size = SFHLOCAL_GetNbyteB((const unsigned char *)hdr[1] + 0x12, sz);
    return 1;
}

u32 VER2_AnlyPackType(void* self, u32* out) {
    *out = 0;
    return 1;
}

u32 VER2_AnlyPketSizLen(void* self, u32* out) {
    *out = 2;
    return 1;
}

int VER2_AnlyPackSiz(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteB(((u8 **)work)[1] + 0x12, SFHLOCAL_GetSizeofMember(0x12, 0x14));
    return 1;
}

int VER2_AnlyNumElemTot(char *work, int *out) {
    *out = 0;
    int size = SFHLOCAL_GetSizeofMember(0xc0, 0xc1);
    *out = SFHLOCAL_GetNbyteB(*(char **)(work + 4) + 0xc0, size);
    return 1;
}

int VER2_AnlyNumElemAud(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteB(((u8 **)work)[1] + 0xC1, SFHLOCAL_GetSizeofMember(0xC1, 0xC2));
    return 1;
}


int VER2_AnlyNumElemVid(unsigned char **ctx, int *out) {
    *out = 0;
    int size = SFHLOCAL_GetSizeofMember(0xc2, 0xc3);
    *out = SFHLOCAL_GetNbyteB(ctx[1] + 0xc2, size);
    return 1;
}

int VER2_AnlyNumElemPrv(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteB(((u8 **)work)[1] + 0xC3, SFHLOCAL_GetSizeofMember(0xC3, 0xC4));
    return 1;
}

int VER2_AnlyByteRate(unsigned char **work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteB(work[1] + 0xc4, SFHLOCAL_GetSizeofMember(0xc4, 0xc8));
    return 1;
}

int VER2_AnlyMaxPlyLenAud(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteB(((u8 **)work)[1] + 0xC8, SFHLOCAL_GetSizeofMember(0xC8, 0xCC));
    return 1;
}

int VER2_AnlyMaxPlyLenVid(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteB(((u8 **)work)[1] + 0xCC, SFHLOCAL_GetSizeofMember(0xCC, 0xD0));
    return 1;
}

int VER2_AnlyMaxFrmNum(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteB(((u8 **)work)[1] + 0xD0, SFHLOCAL_GetSizeofMember(0xD0, 0xD4));
    return 1;
}

int VER2_AnlyMaxPicSiz(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteB(((u8 **)work)[1] + 0xD4, SFHLOCAL_GetSizeofMember(0xD4, 0xD8));
    return 1;
}

int VER2_AnlyDiffTime(void *work, u64 *out) {
    *out = 0;
    if (*(s32 *)((u8 *)work + 0x10) < 0xCB) {
        return 0;
    }
    *out = SFHLOCAL_GetNbyteB64(((u8 **)work)[1] + 0xE0, SFHLOCAL_GetSizeofMember(0xE0, 0xE8));
    return 1;
}

void VER2_AnlyElemCodecAud() {}

void VER2_AnlyElemLayer() {}

void VER2_AnlyElemChNum() {}

void VER2_AnlyElemSmpHz() {}

void VER2_AnlyElemCodecVid() {}

void VER2_AnlyElemAvrBitRate() {}

void VER2_AnlyElemPicSz() {}

void VER2_AnlyElemPicRate() {}

void VER2_AnlyFtrColType() {}

void VER2_AnlyFtrPicType() {}

void VER2_AnlyFtrFixFlg() {}

void VER2_AnlyFtrShcFixFlg() {}

void VER2_AnlyFtrExpand() {}

void VER2_AnlyFtrGopN() {}

void VER2_AnlyFtrGopM() {}

void VER2_AnlyFtrFxType() {}

void VER2_AnlyFtrNetWidth() {}

void VER2_AnlyFtrNetHeight() {}

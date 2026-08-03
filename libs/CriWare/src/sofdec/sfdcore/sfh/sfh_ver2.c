// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfh/sfh_ver2
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <decomp.h>

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
    u8 *found;
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
        is_v2 = 1;
        n_vid = 0;
    }
    kind = stm_id & 0xE0;
    index = stm_id & 0x1F;
    found = NULL;
    if (kind == 0xC0) goto caseC0;
    else if (kind == 0xE0) goto caseE0;
    else if (kind == 0xA0) goto caseA0;
    else goto done;
caseC0:
    if (is_v2 != 0) {
        u8 *tmp = base + (index << 4);
        cur = tmp + 0x1C0;
        if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
            found = cur;
        }
        goto done;
    }
    cur = base + 0x1C0;
    i = 0;
    while (i < n_aud) {
        if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
            found = cur;
            break;
        }
        cur += 0x10;
        i++;
    }
    goto done;
caseE0:
    if (is_v2 != 0) {
        u8 *tmp = base + (index << 6);
        cur = tmp + 0x3C0;
        if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
            found = cur;
        }
        goto done;
    }
    cur = base + 0x3C0;
    i = 0;
    while (i < n_vid) {
        if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
            found = cur;
            break;
        }
        cur += 0x40;
        i++;
    }
    goto done;
caseA0:
    cur = base + 0x7C0;
    if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
        found = cur;
    }
    cur = base + 0x7E0;
    if (SFHLOCAL_GetNbyteB(cur, SFHLOCAL_GetSizeofMember(0, 1)) == stm_id) {
        found = cur;
    }
    goto done;
done:
    return found;
}

int VER2_IsSfdHeader(void *work, int *out) {
    char buf[0x40];
    char *p;
    u32 major;
    u32 minor;
    u32 t1;
    u32 t2;
    u32 v1;
    u32 v2;
    s32 ok;
    int c;

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
    t1 = 0;
    t2 = 0;
    p = strstr(buf, lbl_eu_8051CF00 + 0x19);
    if (p == NULL) {
        ok = 0;
    } else {
        p += 4;
        t1 = 0;
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
        t2 = 0;
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
    int c;

    *out1 = 0;
    *out2 = 0;
    memset(buf, 0, 0x40);
    memcpy(buf, ((u8 **)work)[1] + 0x40, 0x40);
    t1 = 0;
    t2 = 0;
    p = strstr(buf, lbl_eu_8051CF00 + 0x19);
    if (p == NULL) {
        ok = 0;
    } else {
        p += 4;
        t1 = 0;
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
        for (t2 = 0; ; ) {
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

int VER2_AnlyElemCodecAud(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    int codec;
    u32 result;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    codec = SFHLOCAL_GetNbyteB(f + 0x1, SFHLOCAL_GetSizeofMember(0x1, 0x2));
    if (*(s32 *)((u8 *)work + 0x10) == 0xC8) {
        codec = ((codec >> 4) & 0xF) + 0x20;
    }
    if (codec == 0x21) goto set1;
    if (codec == 0x22) goto set7;
    if (codec == 0x23) goto set8;
    if (codec == 0x24) goto set3;
    if (codec == 0x25) goto set4;
    if (codec == 0x26) goto set5;
    if (codec != 0x27) goto setdef;
    goto set6;
set1:
    result = 1;
    goto store;
set7:
    result = 7;
    goto store;
set8:
    result = 8;
    goto store;
set3:
    result = 3;
    goto store;
set4:
    result = 4;
    goto store;
set5:
    result = 5;
    goto store;
set6:
    result = 6;
    goto store;
setdef:
    result = 0;
store:
    *out = result;
    return 1;
}
int VER2_AnlyElemLayer(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    u32 layer;
    u32 result;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    layer = SFHLOCAL_GetNbyteB(f + 0x1, SFHLOCAL_GetSizeofMember(0x1, 0x2));
    if (*(s32 *)((u8 *)work + 0x10) == 0xC8) {
        result = layer & 0xF;
        goto store;
    }
    if (layer == 0x24) goto set1;
    if (layer == 0x25) goto set2;
    if (layer != 0x26) goto ret0;
    goto set3;
set1:
    result = 1;
    goto store;
set2:
    result = 2;
    goto store;
set3:
    result = 3;
    goto store;
ret0:
    return 0;
store:
    *out = result;
    return 1;
}
int VER2_AnlyElemChNum(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    u32 result;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    result = (SFHLOCAL_GetNbyteB(f + 0x2, SFHLOCAL_GetSizeofMember(0x2, 0x3)) >> 4) & 0xF;
    *out = result;
    return 1;
}


int VER2_AnlyElemSmpHz(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    u32 val;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    val = SFHLOCAL_GetNbyteB(f + 0x2, SFHLOCAL_GetSizeofMember(0x2, 0x5));
    *out = val & 0xFFFFF;
    return 1;
}

int VER2_AnlyElemCodecVid(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    u32 codec;
    u32 result = 0;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    codec = SFHLOCAL_GetNbyteB(f + 0x1, SFHLOCAL_GetSizeofMember(0x1, 0x2));
    if (*(s32 *)((u8 *)work + 0x10) == 0xC8) {
        codec = ((codec >> 4) & 0xF) + 0x40;
    }
    if (codec == 0x41) goto set1;
    if (codec == 0x42) goto set3;
    if (codec == 0x43) goto set4;
    if (codec == 0x47) goto set5;
    if (codec == 0x48) goto set6;
    if (codec == 0x49) goto set7;
    if (codec != 0x4A) goto setdef;
    goto set8;
set1:
    result = 1;
    goto done;
set3:
    result = 3;
    goto done;
set4:
    result = 4;
    goto done;
set5:
    result = 5;
    goto done;
set6:
    result = 6;
    goto done;
set7:
    result = 7;
    goto done;
set8:
    result = 8;
    goto done;
setdef:
    result = 0;
done:
    *out = result;
    return 1;
}
int VER2_AnlyElemAvrBitRate(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    u32 val;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    val = SFHLOCAL_GetNbyteB(f + 0xC, SFHLOCAL_GetSizeofMember(0xC, 0x10));
    if (val + 0x10000u == 0xFFFFu) {
        val = 0;
    }
    *out = val;
    return 1;
}

int VER2_AnlyElemPicSz(void *work, u32 stm_id, u32 *width, u32 *height) {
    u8 *f;

    *width = 0;
    *height = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    *width = SFHLOCAL_GetNbyteB(f + 0x2, SFHLOCAL_GetSizeofMember(0x2, 0x4));
    *height = SFHLOCAL_GetNbyteB(f + 0x4, SFHLOCAL_GetSizeofMember(0x4, 0x6));
    return 1;
}

int VER2_AnlyElemPicRate(void *work, u32 stm_id, u32 *out) {
    u8 *f;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    *out = SFHLOCAL_GetNbyteB(f + 0xA, SFHLOCAL_GetSizeofMember(0xA, 0xC));
    return 1;
}

int VER2_AnlyFtrColType(void *work, u32 stm_id, u32 *out) {
    u8 *f;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    *out = SFHLOCAL_GetNbyteB(f + 0x21, SFHLOCAL_GetSizeofMember(0x21, 0x22));
    return 1;
}

int VER2_AnlyFtrPicType(void *work, u32 stm_id, u32 *out) {
    u8 *f;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    *out = SFHLOCAL_GetNbyteB(f + 0x22, SFHLOCAL_GetSizeofMember(0x22, 0x23));
    return 1;
}

int VER2_AnlyFtrFixFlg(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    u32 val;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    val = SFHLOCAL_GetNbyteB(f + 0x23, SFHLOCAL_GetSizeofMember(0x23, 0x24));
    *out = val & 1;
    return 1;
}

int VER2_AnlyFtrShcFixFlg(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    u32 val;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    val = SFHLOCAL_GetNbyteB(f + 0x23, SFHLOCAL_GetSizeofMember(0x23, 0x24));
    *out = (val >> 4) & 1;
    return 1;
}

int VER2_AnlyFtrExpand(void *work, u32 stm_id, u32 *out) {
    u8 *f;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    *out = SFHLOCAL_GetNbyteB(f + 0x24, SFHLOCAL_GetSizeofMember(0x24, 0x25));
    return 1;
}

int VER2_AnlyFtrGopN(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    u32 val;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    val = SFHLOCAL_GetNbyteB(f + 0x25, SFHLOCAL_GetSizeofMember(0x25, 0x26));
    if ((s32)val > 0x3F) {
        val = -1;
    }
    *out = val;
    return 1;
}

int VER2_AnlyFtrGopM(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    u32 val;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    val = SFHLOCAL_GetNbyteB(f + 0x26, SFHLOCAL_GetSizeofMember(0x26, 0x27));
    if ((s32)val > 0x3F) {
        val = -1;
    }
    *out = val;
    return 1;
}

int VER2_AnlyFtrFxType(void *work, u32 stm_id, u32 *out) {
    u8 *f;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    *out = SFHLOCAL_GetNbyteB(f + 0x27, SFHLOCAL_GetSizeofMember(0x27, 0x28));
    return 1;
}

int VER2_AnlyFtrNetWidth(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    u32 val;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    val = SFHLOCAL_GetNbyteB(f + 0x28, SFHLOCAL_GetSizeofMember(0x28, 0x2A));
    val = ((val >> 8 & 0x7F) << 7) | (val & 0x7F);
    if (val == 0) {
        return 0;
    }
    *out = val;
    return 1;
}

int VER2_AnlyFtrNetHeight(void *work, u32 stm_id, u32 *out) {
    u8 *f;
    u32 val;

    *out = 0;
    f = searchStmId(work, stm_id);
    if (f == NULL) {
        return 0;
    }
    val = SFHLOCAL_GetNbyteB(f + 0x2A, SFHLOCAL_GetSizeofMember(0x2A, 0x2C));
    val = ((val >> 8 & 0x7F) << 7) | (val & 0x7F);
    if (val == 0) {
        return 0;
    }
    *out = val;
    return 1;
}

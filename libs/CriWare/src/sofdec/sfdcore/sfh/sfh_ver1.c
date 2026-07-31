// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfh/sfh_ver1
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern int SFHLOCAL_GetSizeofMember(int, int);
extern int SFHLOCAL_GetNbyteL(const void *, int);
extern int SFHLOCAL_GetNbyteB(const void *, int);
extern int memcmp(const void *, const void *, unsigned long);
extern void *memset(void *, int, unsigned long);
extern void *memcpy(void *, const void *, unsigned long);
extern char *strstr(const char *, const char *);
extern char lbl_eu_8051CEE0[];

int VER1_AnlyHdrToolVer(void *work, u32 *out1, u32 *out2);
int criware_803D2C98(u32 pic_rate_code);

int VER1_IsSfdHeader(void *work, int *out) {
    u8 *src;
    u32 tool_ver;
    u32 tool_sub;

    *out = 0;
    src = ((u8 **)work)[1] + 0x20;
    if (memcmp(src, lbl_eu_8051CEE0, 0x18) != 0) {
        *(s32 *)work = -1;
        return 0;
    }
    *(u32 *)((u8 *)work + 0x10) = 0x100;
    if (VER1_AnlyHdrToolVer(work, &tool_ver, &tool_sub)) {
        *(u32 *)((u8 *)work + 0xC) = (tool_ver << 8) | tool_sub;
    }
    *(u32 *)work = 2;
    *out = 1;
    return 1;
}

int VER1_IsExistStmId(void *work, u32 stm_id, u32 *out) {
    u8 *found;
    u8 *cur;
    int i;
    u32 v;

    *out = 0;
    found = NULL;
    i = 0;
    cur = ((u8 **)work)[1] + 0x180;
    do {
        v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
        if (v == stm_id) {
            found = cur;
            break;
        }
        i++;
        cur += 0x40;
    } while (i < 0x1A);
    if (found != NULL) {
        *out = 1;
    } else {
        *out = 0;
    }
    return *out;
}

int VER1_IsEffFtrInf(void *work, u32 stm_id, u32 *out) {
    u32 kind;
    u32 v;
    u32 cond;
    u32 ok;
    int i;
    u8 *cur;
    u8 *found;
    u32 f;
    u32 r;

    *out = 0;
    if (*(s32 *)((u8 *)work + 0xC) < 0x6E) {
        return 0;
    }
    if (stm_id - 0xC0 <= 0x1F) {
        kind = 0xC0;
    } else if (stm_id - 0xE0 <= 0x0F) {
        kind = 0xE0;
    } else if (stm_id == 0xBD || stm_id == 0xBF) {
        kind = 0xBD;
    } else {
        kind = 0;
    }
    if (kind != 0xC0 && kind != 0xE0) {
        return 0;
    }
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        ok = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        cond = ((int)v == 0x6B || (int)v >= 0x6E);
        ok = cond != 0;
    }
    if (ok == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    f = SFHLOCAL_GetNbyteL(found + 0x20, SFHLOCAL_GetSizeofMember(0x20, 0x21));
    if (f > 1) {
        r = 0;
    } else {
        r = f != 0;
    }
    *out = r;
    return r;
}

int VER1_AnlyHdrToolVer(void *work, u32 *out1, u32 *out2) {
    char buf[0x50];
    char *p;
    u32 hdr_major;
    u32 t1;
    u32 hdr_minor;
    u32 t2;
    u32 ok;
    int c;

    *out1 = 0;
    *out2 = 0;
    hdr_major = SFHLOCAL_GetNbyteL(((u8 **)work)[1] + 0x38, SFHLOCAL_GetSizeofMember(0x38, 0x39));
    hdr_minor = SFHLOCAL_GetNbyteL(((u8 **)work)[1] + 0x39, SFHLOCAL_GetSizeofMember(0x39, 0x3A));
    memset(buf, 0, 0x20);
    memcpy(buf, ((u8 **)work)[1] + 0x60, 0x20);
    t1 = 0;
    t2 = 0;
    p = strstr(buf, lbl_eu_8051CEE0 + 0x19);
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
        for (p++, t2 = 0; ; ) {
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
    if ((s32)(hdr_major * 100 + hdr_minor) >= (s32)(t1 * 100 + t2)) {
        *out1 = hdr_major;
        *out2 = hdr_minor;
    } else {
        *out1 = t1;
        *out2 = t2;
    }
    return 1;
}

u32 VER1_AnlyHdrSfhVer(void* buf, u32* out1, u32* out2) {
    u32 r;
    *out1 = 1;
    r = 0;
    *out2 = r;
    return 1;
}

u32 VER1_AnlyHdrModuleVer(void* self, u32* out1, u32* out2) {
    *out1 = 0;
    *out2 = 0;
    return 0;
}

int VER1_AnlyHdrSiz(void *obj, int *out_siz) {
    int sz;
    *out_siz = 0;
    sz = SFHLOCAL_GetSizeofMember(0x80, 0x84);
    *out_siz = SFHLOCAL_GetNbyteL((const char *)((void **)obj)[1] + 0x80, sz);
    return 1;
}

int VER1_AnlyPackType(void *work, int *out) {
    *out = -1;
    *out = SFHLOCAL_GetNbyteL(((u8 **)work)[1] + 0x84, SFHLOCAL_GetSizeofMember(0x84, 0x85));
    return 1;
}

int VER1_AnlyPketSizLen(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteL(((u8 **)work)[1] + 0x88, SFHLOCAL_GetSizeofMember(0x88, 0x8A));
    return 1;
}

int VER1_AnlyPackSiz(const unsigned char **work, int *out_size) {
    *out_size = 0;
    *out_size = SFHLOCAL_GetNbyteL(work[1] + 0x8c, SFHLOCAL_GetSizeofMember(0x8c, 0x90));
    return 1;
}

int VER1_AnlyNumElemTot(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteL(((u8 **)work)[1] + 0xB0, SFHLOCAL_GetSizeofMember(0xB0, 0xB1));
    return 1;
}

int VER1_AnlyNumElemAud(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteL(((u8 **)work)[1] + 0xB1, SFHLOCAL_GetSizeofMember(0xB1, 0xB2));
    return 1;
}

int VER1_AnlyNumElemVid(unsigned char **h, int *num) {
    *num = 0;
    int size = SFHLOCAL_GetSizeofMember(0xb2, 0xb3);
    *num = SFHLOCAL_GetNbyteL(h[1] + 0xb2, size);
    return 1;
}

int VER1_AnlyNumElemPrv(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteL(((u8 **)work)[1] + 0xB3, SFHLOCAL_GetSizeofMember(0xB3, 0xB4));
    return 1;
}

int VER1_AnlyByteRate(void *work, int *out) {
    *out = 0;
    if (*(s32 *)((u8 *)work + 0xC) < 0x6E) {
        return 0;
    }
    *out = SFHLOCAL_GetNbyteL(((u8 **)work)[1] + 0xB4, SFHLOCAL_GetSizeofMember(0xB4, 0xB8));
    return 1;
}

int VER1_AnlyMaxPlyLenAud(const unsigned char **work, unsigned int *out_len) {
    int size;
    *out_len = 0;
    size = SFHLOCAL_GetSizeofMember(0xb8, 0xbc);
    *out_len = SFHLOCAL_GetNbyteL(work[1] + 0xb8, size);
    return 1;
}

int VER1_AnlyMaxPlyLenVid(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteL(((u8 **)work)[1] + 0xBC, SFHLOCAL_GetSizeofMember(0xBC, 0xC0));
    return 1;
}

int VER1_AnlyMaxFrmNum(void *work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteL(((u8 **)work)[1] + 0xC0, SFHLOCAL_GetSizeofMember(0xC0, 0xC4));
    return 1;
}

int VER1_AnlyMaxPicSiz(void *work, int *out) {
    *out = 0;
    if (*(s32 *)((u8 *)work + 0xC) < 0xE1) {
        return 0;
    }
    *out = SFHLOCAL_GetNbyteL(((u8 **)work)[1] + 0xC4, SFHLOCAL_GetSizeofMember(0xC4, 0xC8));
    return 1;
}

int VER1_AnlyElemCodecAud(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;
    u32 r;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    v = (u32)SFHLOCAL_GetNbyteL(found + 0x19, SFHLOCAL_GetSizeofMember(0x19, 0x1A));
    if ((int)v == 0) goto set1;
    if ((int)v == 1) goto set2;
    if ((int)v == 2) goto set6;
    if ((int)v == 3) goto set7;
    if ((int)v != 4) {
        goto setdef;
    } else {
        goto set8;
    }
set1:
    r = 1;
    goto store;
set2:
    r = 2;
    goto store;
set6:
    r = 6;
    goto store;
set7:
    r = 7;
    goto store;
set8:
    r = 8;
    goto store;
setdef:
    r = 0;
store:
    *out = r;
    return 1;
}

int VER1_AnlyElemLayer(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 x;
    u32 flag;
    u32 bit;
    u32 ok;
    u32 codec;

    *out = 0;
    x = *(u32 *)work + 1;
    if ((x << __cntlzw(x ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        flag = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)flag == 0x6B || (int)flag >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            x = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (x == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    if (!VER1_AnlyElemCodecAud(work, stm_id, &codec)) {
        return 0;
    }
    if ((int)codec != 2) {
        return 0;
    }
    *out = (u32)SFHLOCAL_GetNbyteL(found + 0x1A, SFHLOCAL_GetSizeofMember(0x1A, 0x1B));
    return 1;
}

int VER1_AnlyElemChNum(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    *out = (u32)SFHLOCAL_GetNbyteL(found + 0x1B, SFHLOCAL_GetSizeofMember(0x1B, 0x1C));
    return 1;
}

int VER1_AnlyElemSmpHz(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    *out = (u32)SFHLOCAL_GetNbyteL(found + 0x1C, SFHLOCAL_GetSizeofMember(0x1C, 0x20));
    return 1;
}

int VER1_AnlyElemCodecVid(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;
    u32 r;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    v = (u32)SFHLOCAL_GetNbyteL(found + 0x19, SFHLOCAL_GetSizeofMember(0x19, 0x1A));
    if ((int)v == 0) goto set1;
    if ((int)v == 1) goto set3;
    if ((int)v == 2) goto set3b;
    if ((int)v == 3) goto set4;
    if ((int)v == 4) goto set5;
    if ((int)v == 5) goto set6;
    if ((int)v != 6) {
        goto setdef;
    } else {
        goto set7;
    }
set1:
    r = 1;
    goto store;
set3:
    r = 3;
    goto store;
set3b:
    r = 3;
    goto store;
set4:
    r = 4;
    goto store;
set5:
    r = 5;
    goto store;
set6:
    r = 6;
    goto store;
set7:
    r = 7;
    goto store;
setdef:
    r = 0;
store:
    *out = r;
    return 1;
}

int VER1_AnlyElemBitRate(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    v = (u32)SFHLOCAL_GetNbyteL(found + 0x1A, SFHLOCAL_GetSizeofMember(0x1A, 0x1C));
    if (v == 0xFFFF) {
        v = 0;
    }
    *out = v;
    return 1;
}

int VER1_AnlyElemPicSz(void *work, u32 stm_id, u32 *out_w, u32 *out_h) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out_w = 0;
    *out_h = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    v = (u32)SFHLOCAL_GetNbyteB(found + 0x1C, SFHLOCAL_GetSizeofMember(0x1C, 0x1F));
    *out_w = (v >> 12) & 0xFFF;
    *out_h = v & 0xFFF;
    return 1;
}

int VER1_AnlyElemPicRate(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    v = (u32)SFHLOCAL_GetNbyteL(found + 0x1F, SFHLOCAL_GetSizeofMember(0x1F, 0x20));
    *out = (u32)criware_803D2C98(v);
    return 1;
}

int VER1_AnlyFtrColType(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    *out = (u32)SFHLOCAL_GetNbyteL(found + 0x21, SFHLOCAL_GetSizeofMember(0x21, 0x22));
    return 1;
}

int VER1_AnlyFtrPicType(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    *out = (u32)SFHLOCAL_GetNbyteL(found + 0x22, SFHLOCAL_GetSizeofMember(0x22, 0x23));
    return 1;
}

int VER1_AnlyFtrFixFlg(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    v = (u32)SFHLOCAL_GetNbyteL(found + 0x23, SFHLOCAL_GetSizeofMember(0x23, 0x24));
    v &= 1;
    *out = v;
    return 1;
}

int VER1_AnlyFtrShcFixFlg(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    v = (u32)SFHLOCAL_GetNbyteL(found + 0x23, SFHLOCAL_GetSizeofMember(0x23, 0x24));
    v = (v >> 4) & 1;
    *out = v;
    return 1;
}

int VER1_AnlyFtrExpand(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    *out = (u32)SFHLOCAL_GetNbyteL(found + 0x24, SFHLOCAL_GetSizeofMember(0x24, 0x25));
    return 1;
}

int VER1_AnlyFtrGopN(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    v = (u32)SFHLOCAL_GetNbyteL(found + 0x25, SFHLOCAL_GetSizeofMember(0x25, 0x26));
    *out = v;
    if ((int)v > 0x3F) {
        *out = (u32)-1;
    }
    return 1;
}

int VER1_AnlyFtrGopM(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    v = (u32)SFHLOCAL_GetNbyteL(found + 0x26, SFHLOCAL_GetSizeofMember(0x26, 0x27));
    *out = v;
    if ((int)v > 0x3F) {
        *out = (u32)-1;
    }
    return 1;
}

int VER1_AnlyFtrFxType(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    if (*(s32 *)((u8 *)work + 0xC) < 0xD2) {
        return 0;
    }
    *out = (u32)SFHLOCAL_GetNbyteL(found + 0x27, SFHLOCAL_GetSizeofMember(0x27, 0x28));
    return 1;
}

int VER1_AnlyFtrNetWidth(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    v = (u32)SFHLOCAL_GetNbyteL(found + 0x28, SFHLOCAL_GetSizeofMember(0x28, 0x2A));
    v = ((v >> 8 & 0x7F) << 7) | (v & 0x7F);
    if (v == 0) {
        return 0;
    }
    *out = v;
    return 1;
}

int VER1_AnlyFtrNetHeight(void *work, u32 stm_id, u32 *out) {
    int i;
    u8 *cur;
    u8 *found;
    u32 v;
    u32 bit;
    u32 ok;

    *out = 0;
    v = *(u32 *)work + 1;
    if ((v << __cntlzw(v ^ 2)) >> 31 == 0) {
        bit = 0;
    } else {
        v = *(u32 *)((u8 *)work + 0xC);
        ok = ((int)v == 0x6B || (int)v >= 0x6E);
        bit = ok != 0;
    }
    if (bit == 0) {
        found = NULL;
    } else {
        found = NULL;
        i = 0;
        cur = ((u8 **)work)[1] + 0x180;
        do {
            v = (u32)SFHLOCAL_GetNbyteL(cur + 0x18, SFHLOCAL_GetSizeofMember(0x18, 0x19));
            if (v == stm_id) {
                found = cur;
                break;
            }
            i++;
            cur += 0x40;
        } while (i < 0x1A);
    }
    if (found == NULL) {
        return 0;
    }
    v = (u32)SFHLOCAL_GetNbyteL(found + 0x2A, SFHLOCAL_GetSizeofMember(0x2A, 0x2C));
    v = ((v >> 8 & 0x7F) << 7) | (v & 0x7F);
    if (v == 0) {
        return 0;
    }
    *out = v;
    return 1;
}

int criware_803D2C98(u32 pic_rate_code) {
    switch (pic_rate_code) {
    case 1: return 23976;
    case 2: return 24000;
    case 3: return 25000;
    case 4: return 29970;
    case 5: return 30000;
    case 6: return 50000;
    case 7: return 59940;
    case 8: return 60000;
    case 0:
    default: return 0;
    }
}

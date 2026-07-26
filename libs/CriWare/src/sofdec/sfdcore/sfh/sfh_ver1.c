// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfh/sfh_ver1
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void VER1_IsSfdHeader() {}

void VER1_IsExistStmId() {}

void VER1_IsEffFtrInf() {}

void VER1_AnlyHdrToolVer() {}

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

extern int SFHLOCAL_GetSizeofMember(int, int);
extern int SFHLOCAL_GetNbyteL(const void *, int);

int VER1_AnlyHdrSiz(void *obj, int *out_siz) {
    int sz;
    *out_siz = 0;
    sz = SFHLOCAL_GetSizeofMember(0x80, 0x84);
    *out_siz = SFHLOCAL_GetNbyteL((const char *)((void **)obj)[1] + 0x80, sz);
    return 1;
}

void VER1_AnlyPackType() {}

void VER1_AnlyPketSizLen() {}

int SFHLOCAL_GetSizeofMember(int start, int end);
int SFHLOCAL_GetNbyteL(const void *ptr, int count);

int VER1_AnlyPackSiz(const unsigned char **work, int *out_size) {
    *out_size = 0;
    *out_size = SFHLOCAL_GetNbyteL(work[1] + 0x8c, SFHLOCAL_GetSizeofMember(0x8c, 0x90));
    return 1;
}

void VER1_AnlyNumElemTot() {}

void VER1_AnlyNumElemAud() {}

int VER1_AnlyNumElemVid(unsigned char **h, int *num) {
    *num = 0;
    int size = SFHLOCAL_GetSizeofMember(0xb2, 0xb3);
    *num = SFHLOCAL_GetNbyteL(h[1] + 0xb2, size);
    return 1;
}

void VER1_AnlyNumElemPrv() {}

void VER1_AnlyByteRate() {}

extern int SFHLOCAL_GetNbyteL(const void *, int); int VER1_AnlyMaxPlyLenAud(const unsigned char **work, unsigned int *out_len) { int size; *out_len = 0; size = SFHLOCAL_GetSizeofMember(0xb8, 0xbc); *out_len = SFHLOCAL_GetNbyteL(work[1] + 0xb8, size); return 1; }

void VER1_AnlyMaxPlyLenVid() {}

void VER1_AnlyMaxFrmNum() {}

void VER1_AnlyMaxPicSiz() {}

void VER1_AnlyElemCodecAud() {}

void VER1_AnlyElemLayer() {}

void VER1_AnlyElemChNum() {}

void VER1_AnlyElemSmpHz() {}

void VER1_AnlyElemCodecVid() {}

void VER1_AnlyElemBitRate() {}

void VER1_AnlyElemPicSz() {}

void VER1_AnlyElemPicRate() {}

void VER1_AnlyFtrColType() {}

void VER1_AnlyFtrPicType() {}

void VER1_AnlyFtrFixFlg() {}

void VER1_AnlyFtrShcFixFlg() {}

void VER1_AnlyFtrExpand() {}

void VER1_AnlyFtrGopN() {}

void VER1_AnlyFtrGopM() {}

void VER1_AnlyFtrFxType() {}

void VER1_AnlyFtrNetWidth() {}

void VER1_AnlyFtrNetHeight() {}

void criware_803D2C98() {}

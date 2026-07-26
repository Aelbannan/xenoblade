// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfh/sfh_ver1
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803d35a8
void VER1_IsSfdHeader() {}
// LLM-HARNESS-END: us-803d35a8

// LLM-HARNESS-BEGIN: us-803d365c
void VER1_IsExistStmId() {}
// LLM-HARNESS-END: us-803d365c

// LLM-HARNESS-BEGIN: us-803d36f8
void VER1_IsEffFtrInf() {}
// LLM-HARNESS-END: us-803d36f8

// LLM-HARNESS-BEGIN: us-803d3888
void VER1_AnlyHdrToolVer() {}
// LLM-HARNESS-END: us-803d3888

// LLM-HARNESS-BEGIN: us-803d3a48
u32 VER1_AnlyHdrSfhVer(void* buf, u32* out1, u32* out2) {
    u32 r;
    *out1 = 1;
    r = 0;
    *out2 = r;
    return 1;
}
// LLM-HARNESS-END: us-803d3a48

// LLM-HARNESS-BEGIN: us-803d3a60
u32 VER1_AnlyHdrModuleVer(void* self, u32* out1, u32* out2) {
    *out1 = 0;
    *out2 = 0;
    return 0;
}
// LLM-HARNESS-END: us-803d3a60

// LLM-HARNESS-BEGIN: us-803d3a74
extern int SFHLOCAL_GetSizeofMember(int, int);
extern int SFHLOCAL_GetNbyteL(const void *, int);

int VER1_AnlyHdrSiz(void *obj, int *out_siz) {
    int sz;
    *out_siz = 0;
    sz = SFHLOCAL_GetSizeofMember(0x80, 0x84);
    *out_siz = SFHLOCAL_GetNbyteL((const char *)((void **)obj)[1] + 0x80, sz);
    return 1;
}
// LLM-HARNESS-END: us-803d3a74

// LLM-HARNESS-BEGIN: us-803d3ad4
void VER1_AnlyPackType() {}
// LLM-HARNESS-END: us-803d3ad4

// LLM-HARNESS-BEGIN: us-803d3b34
void VER1_AnlyPketSizLen() {}
// LLM-HARNESS-END: us-803d3b34

// LLM-HARNESS-BEGIN: us-803d3b94
int SFHLOCAL_GetSizeofMember(int start, int end);
int SFHLOCAL_GetNbyteL(const void *ptr, int count);

int VER1_AnlyPackSiz(const unsigned char **work, int *out_size) {
    *out_size = 0;
    *out_size = SFHLOCAL_GetNbyteL(work[1] + 0x8c, SFHLOCAL_GetSizeofMember(0x8c, 0x90));
    return 1;
}
// LLM-HARNESS-END: us-803d3b94

// LLM-HARNESS-BEGIN: us-803d3bf4
void VER1_AnlyNumElemTot() {}
// LLM-HARNESS-END: us-803d3bf4

// LLM-HARNESS-BEGIN: us-803d3c54
void VER1_AnlyNumElemAud() {}
// LLM-HARNESS-END: us-803d3c54

// LLM-HARNESS-BEGIN: us-803d3cb4
int VER1_AnlyNumElemVid(unsigned char **h, int *num) {
    *num = 0;
    int size = SFHLOCAL_GetSizeofMember(0xb2, 0xb3);
    *num = SFHLOCAL_GetNbyteL(h[1] + 0xb2, size);
    return 1;
}
// LLM-HARNESS-END: us-803d3cb4

// LLM-HARNESS-BEGIN: us-803d3d14
void VER1_AnlyNumElemPrv() {}
// LLM-HARNESS-END: us-803d3d14

// LLM-HARNESS-BEGIN: us-803d3d74
void VER1_AnlyByteRate() {}
// LLM-HARNESS-END: us-803d3d74

// LLM-HARNESS-BEGIN: us-803d3de8
extern int SFHLOCAL_GetNbyteL(const void *, int); int VER1_AnlyMaxPlyLenAud(const unsigned char **work, unsigned int *out_len) { int size; *out_len = 0; size = SFHLOCAL_GetSizeofMember(0xb8, 0xbc); *out_len = SFHLOCAL_GetNbyteL(work[1] + 0xb8, size); return 1; }
// LLM-HARNESS-END: us-803d3de8

// LLM-HARNESS-BEGIN: us-803d3e48
void VER1_AnlyMaxPlyLenVid() {}
// LLM-HARNESS-END: us-803d3e48

// LLM-HARNESS-BEGIN: us-803d3ea8
void VER1_AnlyMaxFrmNum() {}
// LLM-HARNESS-END: us-803d3ea8

// LLM-HARNESS-BEGIN: us-803d3f08
void VER1_AnlyMaxPicSiz() {}
// LLM-HARNESS-END: us-803d3f08

// LLM-HARNESS-BEGIN: us-803d3f7c
void VER1_AnlyElemCodecAud() {}
// LLM-HARNESS-END: us-803d3f7c

// LLM-HARNESS-BEGIN: us-803d40d8
void VER1_AnlyElemLayer() {}
// LLM-HARNESS-END: us-803d40d8

// LLM-HARNESS-BEGIN: us-803d4214
void VER1_AnlyElemChNum() {}
// LLM-HARNESS-END: us-803d4214

// LLM-HARNESS-BEGIN: us-803d4318
void VER1_AnlyElemSmpHz() {}
// LLM-HARNESS-END: us-803d4318

// LLM-HARNESS-BEGIN: us-803d441c
void VER1_AnlyElemCodecVid() {}
// LLM-HARNESS-END: us-803d441c

// LLM-HARNESS-BEGIN: us-803d4598
void VER1_AnlyElemBitRate() {}
// LLM-HARNESS-END: us-803d4598

// LLM-HARNESS-BEGIN: us-803d46a8
void VER1_AnlyElemPicSz() {}
// LLM-HARNESS-END: us-803d46a8

// LLM-HARNESS-BEGIN: us-803d47c0
void VER1_AnlyElemPicRate() {}
// LLM-HARNESS-END: us-803d47c0

// LLM-HARNESS-BEGIN: us-803d48c8
void VER1_AnlyFtrColType() {}
// LLM-HARNESS-END: us-803d48c8

// LLM-HARNESS-BEGIN: us-803d49cc
void VER1_AnlyFtrPicType() {}
// LLM-HARNESS-END: us-803d49cc

// LLM-HARNESS-BEGIN: us-803d4ad0
void VER1_AnlyFtrFixFlg() {}
// LLM-HARNESS-END: us-803d4ad0

// LLM-HARNESS-BEGIN: us-803d4bd8
void VER1_AnlyFtrShcFixFlg() {}
// LLM-HARNESS-END: us-803d4bd8

// LLM-HARNESS-BEGIN: us-803d4ce0
void VER1_AnlyFtrExpand() {}
// LLM-HARNESS-END: us-803d4ce0

// LLM-HARNESS-BEGIN: us-803d4de4
void VER1_AnlyFtrGopN() {}
// LLM-HARNESS-END: us-803d4de4

// LLM-HARNESS-BEGIN: us-803d4ef8
void VER1_AnlyFtrGopM() {}
// LLM-HARNESS-END: us-803d4ef8

// LLM-HARNESS-BEGIN: us-803d500c
void VER1_AnlyFtrFxType() {}
// LLM-HARNESS-END: us-803d500c

// LLM-HARNESS-BEGIN: us-803d5128
void VER1_AnlyFtrNetWidth() {}
// LLM-HARNESS-END: us-803d5128

// LLM-HARNESS-BEGIN: us-803d5240
void VER1_AnlyFtrNetHeight() {}
// LLM-HARNESS-END: us-803d5240

// LLM-HARNESS-BEGIN: us-803d5358
void criware_803D2C98() {}
// LLM-HARNESS-END: us-803d5358

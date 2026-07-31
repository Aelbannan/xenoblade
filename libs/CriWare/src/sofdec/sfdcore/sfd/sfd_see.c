// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_see
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(void* h, u32 err_code);
u32 SFTRN_IsSetup(void* self, u32 idx);
u32 SFSET_GetCond(void* self, u32 idx);
s32 SFCON_IsEndcodeSkip(void* self);
s32 SFHDS_GetMuxVerNum(void* self);
s32 UTY_MulDiv(s32 a, s32 b, s32 c);

void sfsee_UpdateEByteRate(void* self);

void SFSEE_InitHn(void* self) {
    *(u32*)((u8*)self + 0x00) = 0;
    *(u32*)((u8*)self + 0x04) = 0;
    *(u32*)((u8*)self + 0x08) = -3;
    *(u32*)((u8*)self + 0x0C) = 1;
}

s32 SFD_EntrySeek(void* self, s32 seekPos) {
    s32 result = 0;
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(NULL, 0xFF000151);
    } else {
        *(s32*)((u8*)self + 0x2670) = seekPos;
    }
    return result;
}

void SFSEE_FixAvPlay(void *arg, int arg2, int arg3) {
    void *ptr = *(void**)((char*)arg + 0x2670);
    if (ptr == NULL) return;
    if (*(int*)((char*)ptr + 0xdb8) < 0) {
        *(int*)((char*)ptr + 0xdb8) = arg2;
    }
    if (*(int*)((char*)ptr + 0xdbc) < 0) {
        *(int*)((char*)ptr + 0xdbc) = arg3;
    }
}

void sfsee_ExecHeadAnaly(void* self);
void sfsee_ExecFinAnaly(void* self);

void SFSEE_ExecServer(void* self) {
    if (*(u32*)((u8*)self + 0x2670) != 0) {
        sfsee_ExecHeadAnaly(self);
        sfsee_ExecFinAnaly(self);
    }
}

void sfsee_ExecHeadAnaly(void* self) {
    void* avplay = *(void**)((u8*)self + 0x2670);
    s32 val1, val2;
    s32 setup3, setup2, setup1;
    
    if (*(s32*)avplay != 0) return;
    
    setup3 = SFTRN_IsSetup(self, 3);
    if (setup3 && SFSET_GetCond(self, 6) == 1) {
        val1 = 1;
        if (*(s32*)((u8*)avplay + 0xD0C) == 0) {
            val1 = 1;
        } else {
            val1 = 0;
        }
    } else {
        val1 = 0;
    }
    
    if (val1) return;
    
    setup2 = SFTRN_IsSetup(self, 2);
    if (setup2 && SFSET_GetCond(self, 5) == 1) {
        val2 = 1;
        if (*(s32*)((u8*)avplay + 0xAD0) == 0) {
            val2 = 1;
        } else {
            val2 = 0;
        }
    } else {
        val2 = 0;
    }
    
    if (val2) return;
    
    setup1 = SFTRN_IsSetup(self, 1);
    if (setup1 || setup2 || setup3) {
        s32 fileSize, totalSize;
        
        *(s32*)((u8*)avplay + 0x8A0) = 1;
        
        if (*(s32*)((u8*)avplay + 0x0C) != 0 && *(s32*)((u8*)avplay + 0x18) > 0) {
            fileSize = *(s32*)((u8*)avplay + 0xDC4);
            totalSize = *(s32*)((u8*)avplay + 0x40);
            
            if (fileSize > 0 && totalSize > 0) {
                *(s32*)((u8*)self + 0x04) = UTY_MulDiv(fileSize, totalSize, 1000);
            } else {
                *(s32*)((u8*)self + 0x04) = *(s32*)((u8*)avplay + 0x18);
            }
        } else if (*(s32*)((u8*)avplay + 0x0C) != 0) {
            s32 muxVer = SFHDS_GetMuxVerNum(self);
            if (muxVer < 0x6C) {
                s32 val = *(s32*)((u8*)avplay + 0x8A4);
                val = (val << 11) * 0x81E722C3;
                val = (val + (val >> 31)) >> 10;
                *(s32*)((u8*)self + 0x04) = val;
            } else {
                *(s32*)((u8*)self + 0x04) = *(s32*)((u8*)avplay + 0x8A4);
            }
        } else {
            *(s32*)((u8*)self + 0x04) = *(s32*)((u8*)avplay + 0x8A4);
        }
        
        *(s32*)((u8*)self + 0x08) = *(s32*)((u8*)avplay + 0x8A8);
        *(s32*)avplay = 1;
        sfsee_UpdateEByteRate(self);
    } else if (val2) {
        *(s32*)((u8*)self + 0x04) = *(s32*)((u8*)avplay + 0xAD4);
        *(s32*)((u8*)self + 0x08) = *(s32*)((u8*)avplay + 0xAD8);
        *(s32*)avplay = 1;
        sfsee_UpdateEByteRate(self);
    } else if (val1) {
        *(s32*)((u8*)self + 0x04) = *(s32*)((u8*)avplay + 0xD10);
        *(s32*)((u8*)self + 0x08) = *(s32*)((u8*)avplay + 0xD14);
        *(s32*)avplay = 1;
        sfsee_UpdateEByteRate(self);
    }
}

s32 SFD_SetFileSize(void* self, s32 fileSize) {
    void* avplay;
    s32 result = 0;
    
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(NULL, 0xFF000159);
    } else {
        avplay = *(void**)((u8*)self + 0x2670);
        if (avplay == NULL) {
            return 0;
        }
        *(s32*)((u8*)avplay + 0xDC4) = fileSize;
        sfsee_UpdateEByteRate(self);
    }
    return result;
}

s32 SFD_SetTotTime(void* self, s32 timeHi, s32 timeLo) {
    void* avplay;
    s32 result = 0;
    
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(NULL, 0xFF00015A);
    } else {
        avplay = *(void**)((u8*)self + 0x2670);
        if (avplay == NULL) {
            return 0;
        }
        *(s32*)((u8*)avplay + 0xDC8) = timeHi;
        *(s32*)((u8*)avplay + 0xDCC) = timeLo;
        sfsee_UpdateEByteRate(self);
    }
    return result;
}

s32 SFD_SetByteRate(void* self, s32 byteRate) {
    void* avplay;
    s32 result = 0;
    
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(NULL, 0xFF00015B);
    } else {
        avplay = *(void**)((u8*)self + 0x2670);
        if (avplay == NULL) {
            return 0;
        }
        *(s32*)((u8*)avplay + 0xDD0) = byteRate;
        sfsee_UpdateEByteRate(self);
    }
    return result;
}

s32 SFD_SetSeekPos(void* self, s32 seekPos) {
    void* avplay;
    s32 result = 0;
    
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(NULL, 0xFF00015C);
    } else {
        avplay = *(void**)((u8*)self + 0x2670);
        if (avplay == NULL) {
            return 0;
        }
        *(s32*)((u8*)avplay + 0xDD4) = seekPos;
    }
    return result;
}

void sfsee_ExecFinAnaly(void* self) {
    void* avplay = *(void**)((u8*)self + 0x2670);
    s32* fieldPtr = (s32*)((u8*)self + 0x2674);
    s32 updated = 0;
    
    if (SFCON_IsEndcodeSkip(self)) return;
    
    if (*(s32*)((u8*)avplay + 0xDAC) <= 0) {
        s32 seekPos = fieldPtr[1];
        s32 finalPos;
        
        if (seekPos == -3) {
            finalPos = 0;
        } else {
            finalPos = *(s32*)((u8*)avplay + 0xDD4);
        }
        
        if (finalPos >= 0) {
            s32 idx1 = *(s32*)((u8*)self + 0x1FEC);
            s32 idx2;
            s32 result;
            
            idx2 = *(s32*)((u8*)self + 0x1408 + idx1 * 0x74);
            result = *(s32*)((u8*)self + 0x1FD8 + idx2 * 0x44 + 0x20);
            
            if (result >= 0) {
                *(s32*)((u8*)avplay + 0xDAC) = finalPos + result;
                updated = 1;
            }
        }
    }
    
    if (*(s32*)((u8*)avplay + 0xDB0) <= 0) {
        s32 val = *(s32*)((u8*)self + 0xE50);
        if (val > 0) {
            *(s32*)((u8*)avplay + 0xDB0) = val;
            *(s32*)((u8*)avplay + 0xDB4) = *(s32*)((u8*)self + 0xE54);
            updated = 1;
        }
    }
    
    if (updated) {
        sfsee_UpdateEByteRate(self);
    }
}

void sfsee_UpdateEByteRate(void* self) {
    void* avplay = *(void**)((u8*)self + 0x2670);
    s32 byteRate = *(s32*)((u8*)avplay + 0xDD0);
    
    if (byteRate > 0) {
        *(s32*)((u8*)avplay + 0xDA8) = byteRate;
        return;
    }
    
    {
        s32 fileSize = *(s32*)((u8*)avplay + 0xDC4);
        s32 totalSize = *(s32*)((u8*)avplay + 0xDC8);
        s32 totalTime = *(s32*)((u8*)avplay + 0xDCC);
        
        if (fileSize > 0 && totalSize > 0) {
            *(s32*)((u8*)avplay + 0xDA8) = UTY_MulDiv(fileSize, totalSize, totalTime);
            return;
        }
        
        {
            s32 headerRate = *(s32*)((u8*)avplay + 0x04);
            if (headerRate > 0) {
                *(s32*)((u8*)avplay + 0xDA8) = headerRate;
                return;
            }
            
            if (fileSize <= 0) {
                fileSize = *(s32*)((u8*)avplay + 0xDAC);
            }
            if (totalSize <= 0) {
                totalSize = *(s32*)((u8*)avplay + 0xDB0);
                totalTime = *(s32*)((u8*)avplay + 0xDB4);
            }
            
            if (fileSize > 0 && totalSize > 0) {
                *(s32*)((u8*)avplay + 0xDA8) = UTY_MulDiv(fileSize, totalSize, totalTime);
            } else {
                *(s32*)((u8*)avplay + 0xDA8) = headerRate;
            }
        }
    }
}

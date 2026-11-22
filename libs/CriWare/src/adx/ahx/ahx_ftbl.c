#include "adx/ahx/ahx_ftbl.h"

extern float AdxBwavF32Table[];
float* AHXFTBL_FtblPtr = AdxBwavF32Table; // name-guess: retail .data pointer at 0x805620D8, returned by AHXTBL_GetFtblInfo
s32 AHXFTBL_FtblSize = 0x2080; // name-guess: retail .data size at 0x805620DC, returned by AHXTBL_GetFtblInfo

void AHXTBL_GetFtblInfo(float** array_ptr, s32* size_ptr){
    if(array_ptr != NULL){
        *array_ptr = AHXFTBL_FtblPtr;
    }
    if(size_ptr != NULL){
        *size_ptr = AHXFTBL_FtblSize;
    }
}

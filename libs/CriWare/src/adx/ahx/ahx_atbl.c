#include "adx/ahx/ahx_atbl.h"

//old name: ahxdcd_atbl


extern ATblEntry lbl_eu_805629A8[];
extern float lbl_eu_805659A8[];

ATblEntry* lbl_eu_80565AA8 = lbl_eu_805629A8;
s32 lbl_eu_80565AAC = 0x3000;
float* lbl_eu_80565AB0 = lbl_eu_805659A8;
s32 lbl_eu_80565AB4 = 0x100;

void AHXTBL_GetAtblInfo(ATblEntry** array_ptr, s32* size_ptr){
    if(array_ptr != NULL){
        *array_ptr = lbl_eu_80565AA8;
    }
    if(size_ptr != NULL){
        *size_ptr = lbl_eu_80565AAC;
    }
}

void AHXTBL_GetMtblInfo(float** array_ptr, s32* size_ptr){
    if(array_ptr != NULL){
        *array_ptr = lbl_eu_80565AB0;
    }
    if(size_ptr != NULL){
        *size_ptr = lbl_eu_80565AB4;
    }
}

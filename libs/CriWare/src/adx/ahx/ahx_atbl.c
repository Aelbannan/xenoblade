#include "adx/ahx/ahx_atbl.h"

//old name: ahxdcd_atbl


// Retail table/pointer storage ships from the criware data unit (criware_data.s):
//   lbl_eu_80565AA8 = ahxtbl_atbl_ptr   (pointer to lbl_eu_805629A8)
//   lbl_eu_80565AAC = ahxtbl_atbl_size  (0x3000)
//   lbl_eu_80565AB0 = ahxtbl_mtbl_ptr   (pointer to lbl_eu_805659A8)
//   lbl_eu_80565AB4 = ahxtbl_mtbl_size  (0x100)
extern ATblEntry* lbl_eu_80565AA8;
extern s32 lbl_eu_80565AAC;
extern float* lbl_eu_80565AB0;
extern s32 lbl_eu_80565AB4;

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

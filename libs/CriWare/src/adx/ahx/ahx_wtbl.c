#include "adx/ahx/ahx_wtbl.h"

/*
 * The synthesis window table (ahxtbl_wtbl) lives in the retail data blob
 * (criware_data.s, .data range). The retail ahx_wtbl.o references the table
 * INDIRECTLY through two .data pointer/size slots at 0x805629A0 / 0x805629A4,
 * which are also defined in criware_data.s. Declaring those slots as sized
 * externs (16 bytes each so MWCC keeps lis/addi addressing, not sda21) makes
 * this TU data-free, matching the retail split object.
 */
extern float* lbl_eu_805629A0[4]; /* slot 0: pointer to the wave table */
extern s32 lbl_eu_805629A4[4];    /* slot 1: table size in bytes */

void AHXTBL_GetWtblInfo(float** array_ptr, s32* size_ptr){
    if(array_ptr != NULL){
        *array_ptr = *lbl_eu_805629A0;
    }
    if(size_ptr != NULL){
        *size_ptr = *lbl_eu_805629A4;
    }
}

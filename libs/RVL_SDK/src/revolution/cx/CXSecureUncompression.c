#include <revolution/cx/CXSecureUncompression.h>

//Doesn't match on the 1.0 compiler
/* Verify a packed LH (length-head) table. Retail register mapping for
   audit: r3=pTable, r4=shift, r30=pCur, r31_28=pEntry, r31_24=count,
   r31_20=pEnd, r7=mask, r8=bitHi, r9=bitLo, r29=len, r11=addr, r10=bit. */
BOOL CXiLHVerifyTable(u16* pTable, u32 shift) {
    u16* pCur = pTable;
    u16* pEntry = pCur + 1;
    u32 count = pCur[0];
    u16* pEnd = count + pTable;
    u16 mask = (1 << (shift - 2)) - 1;
    u16 bitHi = 1 << (shift - 1);
    u16 bitLo = 1 << (shift - 2);
    u32 i, j;

    u8 array[0x80];
    
    for(j = 0; j < 0x80; j++){
        array[j] = 0;
    }

    if(count > (1 << (shift + 1))){
        return FALSE;
    }

    i = 1;
    pCur = pEntry;
    
    while (pCur < pEnd) {
        if((array[i/8] & (1 << (i & 0x7))) == 0){
            u32 len = ((pCur[0] & mask) + 1) * 2;
            u32 addr = ((u32)pCur & ~0x3) + len * 2;
            
            if(pCur[0] != 0 || i < count - 4){
                if(addr >= (u32)pEnd){
                    return FALSE;
                }

                if((pCur[0] & bitHi) != 0){
                    u32 bit = (i & ~1) + len;
                    array[bit/8] |= 1 << (bit & 0x7);
                }

                if((pCur[0] & bitLo) != 0){
                    u32 bit = (i & ~1) + len + 1;
                    array[bit/8] |= 1 << (bit & 0x7);
                }
            }
        }

        i++;
        pCur++;
    }

    return TRUE;
}


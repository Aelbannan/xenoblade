#include "PowerPC_EABI_Support/MSL_C/MSL_Common/arith.h"

int abs(int x) {
    int mask = x >> 31;
    return (mask ^ x) - mask;
}

//might be llabs
long labs(long x) {
    return x > 0 ? x : -x;
}

//not present in the retail binary; kept commented out for reference
//s64 llabs(s64 __x){
//}

//div_t div(s32 __numer, s32 __denom){
//}

//void ldiv(){
//}

//void lldiv(){
//}

//void __msl_add(){
//}

//void __msl_ladd(){
//}

//void __lladd(){
//}

//void __msl_mul(){
//}

//void __msl_lmul(){
//}

//void __llmul(){
//}

//void __msl_div(){
//}

//void __msl_ldiv(){
//}

//void __lldiv(){
//}

//void __msl_mod(){
//}

//void __msl_lmod(){
//}

//void __llmod(){
//}

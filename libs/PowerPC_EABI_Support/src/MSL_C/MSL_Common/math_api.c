#include <types.h>

//IEEE-754 field masks used by the classification helpers
#define FLOAT_EXP_MASK 0x7F800000
#define FLOAT_MANTISSA_MASK 0x007FFFFF
#define FLOAT_SIGN_MASK 0x80000000
#define DOUBLE_EXP_MASK 0x7FF00000
#define DOUBLE_MANTISSA_MASK 0x000FFFFF

#include "PowerPC_EABI_Support/MSL_C/MSL_Common/math_api.h"

//not present in the retail binary; kept commented out for reference
//void __msl_generic_count_bits32(){
//}

//void __msl_generic_count_bits64(){
//}

//void __msl_relation(){
//}

//int __signbitf(float x){
//}

int __fpclassifyf(float x) {
    switch((*(s32*)&x) & FLOAT_EXP_MASK) {
        case FLOAT_EXP_MASK: {
            if((*(s32*)&x) & FLOAT_MANTISSA_MASK) return 1;
            else return 2;
            break;
        }
        case 0: {
            if((*(s32*)&x) & FLOAT_MANTISSA_MASK) return 5;
            else return 3;
            break;
        }
    }
    return 4;
}

int __signbitd(double x) {
    return __HI(x) & FLOAT_SIGN_MASK;
}

int __fpclassifyd(double x) {
    switch(__HI(x) & DOUBLE_EXP_MASK) {
        case DOUBLE_EXP_MASK: {
            if((__HI(x) & DOUBLE_MANTISSA_MASK) || (__LO(x) & 0xffffffff)) return 1;
            else return 2;
            break;
        }
        case 0: {
            if((__HI(x) & DOUBLE_MANTISSA_MASK) || (__LO(x) & 0xffffffff)) return 5;
            else return 3;
            break;
        }
    }
    return 4;
}

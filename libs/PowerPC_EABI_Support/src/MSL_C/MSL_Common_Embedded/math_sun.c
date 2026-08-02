#include "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/fdlibm.h"

double scalbn(double x, int y) {
    int sp8;

    double temp_f1 = frexp(x, &sp8);
    sp8 += y;
    return ldexp(temp_f1, sp8);
}

//not present in the retail binary; kept commented out for reference
//void exp2(){
//}

//void llrint(){
//}

//void llround(){
//}

//void llroundf(){
//}

//void llroundl(){
//}

//void lrint(){
//}

//void lrintf(){
//}

//void lrintl(){
//}

//void lround(){
//}

//void lroundf(){
//}

//void lroundl(){
//}

//void nearbyint(){
//}

//void nearbyintf(){
//}

//void nearbyintl(){
//}

//void remquo(){
//}

//void remquof(){
//}

//void remquol(){
//}

//void round(){
//}

//void roundf(){
//}

//void roundl(){
//}

//void scalbnl(){
//}

//void tgamma(){
//}

//void trunc(){
//}

//void truncf(){
//}

//void truncl(){
//}

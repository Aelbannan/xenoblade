#include <stdio.h>

static void parse_format() {}

static void __wsformatter() {}

//not present in the retail binary; kept commented out for reference
//void __wFileRead(){
//}

wint_t __wStringRead(void* isc, wint_t ch, int Action) {
    wchar_t ret;
    __wInStrCtrl* Iscp = (__wInStrCtrl*)isc;

    switch(Action) {
        case __GetAwChar:
            ret = *(Iscp->wNextChar);
            if(ret == 0) {
                Iscp->wNullCharDetected = 1;
                return 0xFFFF;
            } else {
                Iscp->wNextChar++;
                return ret;
            }
        case __UngetAwChar:
            if(!Iscp->wNullCharDetected) {
                Iscp->wNextChar--;
            } else {
                Iscp->wNullCharDetected = FALSE;
            }
            return ch;
        case __TestForwcsError:
            return Iscp->wNullCharDetected;
    }

    return 0;
}

//not present in the retail binary; kept commented out for reference
//void fwscanf(){
//}

//void fwscanf_s(){
//}

//void wscanf(){
//}

//void wscanf_s(){
//}

//void vwscanf(){
//}

//void vwscanf_s(){
//}

//void vfwscanf(){
//}

//void vfwscanf_s(){
//}

//void vswscanf(){
//}

//void vswscanf_s(){
//}

//void swscanf(){
//}

//void swscanf_s(){
//}

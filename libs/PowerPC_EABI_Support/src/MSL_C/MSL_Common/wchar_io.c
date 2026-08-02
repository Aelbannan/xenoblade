#ifndef _MSL_WIDE_CHAR
#define _MSL_WIDE_CHAR
#endif

#include "PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h"

// not present in the retail binary; kept commented out for reference
//void putwc(){
//}

//void putwchar(){
//}

//void fputwc(){
//}

//void getwc(){
//}

//void __getwc(){
//}

//void __getwchar(){
//}

//void fgetwc(){
//}

//void __fgetwc(){
//}

//void ungetwc(){
//}

//void __ungetwc(){
//}

//void fputws(){
//}

//void fgetws(){
//}

//void __fwide(){
//}

int fwide(FILE* stream, int mode)
{
    int orientation;
    int result;

    if (stream == nullptr || stream->mode.file_kind == __closed_file)
        return 0;

    orientation = stream->mode.file_orientation;
    switch (orientation) {
    case __unoriented:
        if (mode > 0)
            stream->mode.file_orientation = __wide_oriented;
        else if (mode < 0)
            stream->mode.file_orientation = __char_oriented;

        result = mode;
        break;

    case __wide_oriented:
        result = 1;
        break;

    case __char_oriented:
        result = -1;
        break;
    }

    return result;
}

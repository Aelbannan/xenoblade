#include <wchar.h>
#include <string.h>

wchar_t* wmemcpy(wchar_t* dest, const wchar_t* src, size_t n) {
    return (wchar_t*)memcpy(dest, src, n * sizeof(wchar_t));
}

//unused
void wmemcpy_s(){
}

//unused
void wmemmove(){
}

//unused
void wmemmove_s(){
}

//unused
void wmemset(){
}

wchar_t* wmemchr(wchar_t* s, wchar_t c, int n) {
    while (n--) {
        if (*s == c) return s;
        s++;
    }
    return NULL;
}

//unused
void wmemcmp(){
}

#include <types.h>
#include <ctype.h>

//unused
void strdup(){
}

//unused
void strlwr(){
}

//unused
void ultoa(){
}

//unused
void gcvt(){
}

//unused
void heapmin(){
}

int stricmp(const char* s1, const char* s2) {
    int c1, c2;
    do {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
    } while (c1 != 0);
    return 0;
}


//unused
void strnicmp(){
}

//unused
void strupr(){
}

//unused
void strdate(){
}

//unused
void strset(){
}

//unused
void strnset(){
}

//unused
void strspnp(){
}

//unused
void strncasecmp(){
}

//unused
void strcmpi(){
}

//unused
void strncmpi(){
}

//unused
void strcasecmp(){
}

//unused
void stricoll(){
}

//unused
void strncoll(){
}

//unused
void strnicoll(){
}

//unused
void itoa(){
}

//unused
void strrev(){
}

//unused
void filelength(){
}

//unused
void wtoi(){
}

//unused
void wcslwr(){
}

//unused
void wcsupr(){
}

//unused
void wcsicmp(){
}

//unused
void wcsnicmp(){
}

//unused
void wcsrev(){
}

//unused
void wcsset(){
}

//unused
void wcsnset(){
}

//unused
void wcsspnp(){
}

//unused
void wcsdup(){
}

//unused
void wstrrev(){
}

//unused
void wcsicoll(){
}

//unused
void wcsncoll(){
}

//unused
void wcsnicoll(){
}

//unused
void itow(){
}

//unused
void watof(){
}

//unused
void asctime_r(){
}

//unused
void ctime_r(){
}

//unused
void localtime_r(){
}

//unused
void gmtime_r(){
}

//unused
void rand_r(){
}

//unused
void strerror_r(){
}

//unused
void strtok_r(){
}

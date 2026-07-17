#include "PowerPC_EABI_Support/Runtime/NMWException.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"
#include <exception>


#define ARRAY_HEADER_SIZE 16

extern "C"{
    extern void abort();
}

namespace std{

    static void dthandler(){
        abort();
    }

    static terminate_handler thandler = dthandler;


    static void duhandler(){
        terminate();
    }

    static unexpected_handler uhandler = duhandler;


    //unused
    extern terminate_handler set_terminate(terminate_handler handler){
        terminate_handler old = thandler;
        thandler = handler;
        return old;
    }

    extern void terminate(){
        thandler();
    }

    //unused
    extern unexpected_handler set_unexpected(unexpected_handler handler){
        unexpected_handler old = uhandler;
        uhandler = handler;
        return old;
    }

    //unused
    extern void unexpected(){
        uhandler();
    }

}

int __throw_catch_compare(const char* throwtype, const char* catchtype, int* offset_result) {
    const char* p1;
    const char* p2;

    *offset_result = 0;

    if (catchtype == 0) {
        return 1;
    }

    p2 = catchtype;
    if (*p2 == 'P') {
        p2++;
        if (*p2 == 'C') p2++;
        if (*p2 == 'V') p2++;
        if (*p2 == 'v') {
            if (*throwtype == 'P' || *throwtype == '*') {
                return 1;
            }
        }
        p2 = catchtype; /* reset */
    }

    p1 = throwtype;
    if (*p1 == '*' || *p1 == '!') {
        while (1) {
            if (*p1 == *p2) {
                p1++;
                p2++;
                if (p1[-1] == '!') {
                    int offset = 0;
                    while (*p1 != '!') {
                        offset = offset * 10 + (*p1 - '0');
                        p1++;
                    }
                    *offset_result = offset;
                    return 1;
                }
            } else {
                /* skip to next '!' in throwtype */
                while (*p1 != '!') p1++;
                p1++; /* skip '!' */
                while (*p1 != '!') p1++;
                p1++; /* skip second '!' */
                if (*p1 == 0) return 0;
                p2 = catchtype + 1;
            }
        }
        return 0;
    }

    /* skip leading qualifiers */
    while ((*p1 == 'P' || *p1 == 'R') && *p1 == *p2) {
        p1++;
        p2++;
        if (*p2 == 'C') {
            if (*p1 == 'C') p1++;
            p2++;
        }
        if (*p1 == 'C') return 0;
        if (*p2 == 'V') {
            if (*p1 == 'V') p1++;
            p2++;
        }
        if (*p1 == 'V') return 0;
    }

    while (*p1 == *p2) {
        if (*p1 == 0) return 1;
        p1++;
        p2++;
    }

    return 0;
}

class __partial_array_destructor {
private:
    void* p;
    size_t size;
    size_t n;
    ConstructorDestructor dtor;

public:
    size_t i;

    __partial_array_destructor(void* array, size_t elementsize, size_t nelements, ConstructorDestructor destructor) {
        p = array;
        size = elementsize;
        n = nelements;
        dtor = destructor;
        i = n;
    }

    ~__partial_array_destructor() {
          char* ptr;

          if (i < n && dtor) {
                for (ptr = (char*)p + size * i; i > 0; i--) {
                      ptr -= size;
                      DTORCALL_COMPLETE(dtor, ptr);
                }
          }
    }
};


extern void* __construct_new_array(void* block, ConstructorDestructor ctor, ConstructorDestructor dtor, size_t size, size_t n){
    char* ptr;

    if((ptr = (char*)block) != 0L){
        size_t* p = (size_t*)ptr;

        p[0] = size;
        p[1] = n;
        ptr += ARRAY_HEADER_SIZE;

        if(ctor){
            __partial_array_destructor pad(ptr, size, n, dtor);
            char* p;

            for(pad.i = 0, p = (char*)ptr; pad.i < n; pad.i++, p += size){
                CTORCALL_COMPLETE(ctor, p);
            }
        }
    }
    return ptr;
}

extern void __construct_array(void* ptr, ConstructorDestructor ctor, ConstructorDestructor dtor, size_t size, size_t n) {
    __partial_array_destructor pad(ptr, size, n, dtor);
    char* p;

    for(pad.i = 0, p = (char*)ptr; pad.i < n; pad.i++, p += size){
        CTORCALL_COMPLETE(ctor, p);
    }
}

extern void __destroy_arr(void* block, ConstructorDestructor* dtor, size_t size, size_t n) {
    char* p;

    for (p = (char*)block + size * n; n > 0; n--) {
        p -= size;
        DTORCALL_COMPLETE(dtor, p);
      }
}


extern void __destroy_new_array(void* block, ConstructorDestructor dtor){
    if(block){
        if(dtor){
            size_t i, objects, objectsize;
            char* p;

            objectsize = *(size_t*)((char*)block - ARRAY_HEADER_SIZE);
            objects = ((size_t*)((char*)block - ARRAY_HEADER_SIZE))[1];
            p = (char *)block + (objectsize * objects);
            
            for(i = 0; i < objects; i++){
                p -= objectsize;
                DTORCALL_COMPLETE(dtor, p);
            }
        }

        ::operator delete[] ((char*)block - ARRAY_HEADER_SIZE);
    }
}

//unused
extern void __destroy_new_array2(){
}

//unused
extern void __destroy_new_array3(){
}

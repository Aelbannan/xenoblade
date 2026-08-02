#include <new>
#include <types.h>

//Unused file. Needed to generate rtti info for std::exception in the right place.

//Dummy function to force the rtti info to generate
static void dummy(){
    throw std::bad_alloc();
}

/*
// operator new/delete overrides not present in the retail binary; kept
// commented out for reference
DECL_WEAK void* operator new(size_t count){
    return 0;
}

DECL_WEAK void operator delete(void* p) throw() {
}

DECL_WEAK void* operator new[](size_t count){
    return 0;
}

DECL_WEAK void operator delete[](void* p) throw() {
}
*/

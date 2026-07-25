#ifndef _NMWEXCEPTION
#define _NMWEXCEPTION

#include <types.h>
#include "PowerPC_EABI_Support/Runtime/__ppc_eabi_linker.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef short vbase_ctor_arg_type;
typedef char local_cond_type;

typedef struct CatchInfo {
    void* location;
    void* typeinfo;
    void* dtor;
    void* sublocation;
    long pointercopy;
    void* stacktop;
} CatchInfo;

typedef struct DestructorChain {
  struct DestructorChain* next;
  void* destructor;
  void* object;
} DestructorChain;

extern void* __register_global_object(void* object, void* destructor, void* registration);
extern void __destroy_global_chain(void);

extern void __end__catch(CatchInfo* catchinfo);
extern void __throw(char* throwtype, void* location, void* dtor);
extern void __unexpected(CatchInfo* catchinfo);

extern int __register_fragment(struct __eti_init_info* info, char* TOC);

#ifdef __cplusplus
}
#endif

// Defined in Gecko_ExceptionPPC.cp with C++ linkage; callers and the
// definition must agree on the mangled name.
void __unregister_fragment(unsigned int fragmentID);

#endif // _NMWEXCEPTION

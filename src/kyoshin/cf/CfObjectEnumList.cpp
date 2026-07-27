// Auto-scaffolded catalog TU for kyoshin/cf/CfObjectEnumList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CfObjectEnumList.hpp"

namespace cf {
    class CfObjectSelectorObj {
    public:
        virtual ~CfObjectSelectorObj();
    };

    void* CfObjEnumList::func_800F6E98(int index) {
        return *static_cast<void**>(mPtrArray[index]);
    }

    void* CfObjEnumList::func_800F6EAC(unsigned long idx) {
        return static_cast<void*>(static_cast<void**>(mPtrArray[idx])[1]);
    }

    void* CfObjEnumList::func_800F6EC0(int index) {
        return mPtrArray[index];
    }
}

void __ct__cf_CfObjEnumList(){}

void func_800F49F8(){}

void func_800F4A98(){}

void func_800F4B5C(){}

void func_800F6D50(){}

void func_800F6E08(){}

void func_800F6ED0(){}

void func_800F7DEC(){}

void func_800F8794(){}

void func_800F8890(){}

void func_800F89DC(){}

void func_800F9AEC(){}

void __ct__800FA9B4(){}

void __ct__800FAE3C(){}

void __ct__800FB044(){}

void func_800FB270(){}

void func_800FB5AC(){}

void __ct__800FBA18(){}

void __ct__800FBF08(){}

void __ct__800FC040(){}

void __ct__800FC19C(){}

void __ct__800FC32C(){}

void __ct__800FC4FC(){}

void __ct__800FC648(){}

void __ct__800FC790(){}

void __ct__800FC8DC(){}

void __ct__800FCA24(){}

void __ct__800FCB80(){}

void __ct__800FCD10(){}

void __ct__800FCF1C(){}

void __ct__800FD0B4(){}

void __ct__800FD250(){}

void func_800FD2FC(){}

void func_800FD378(){}

void func_800FD3FC(){}

void func_800FD68C(void* self){}

void func_800FD698(void* self) { ((void(*)(void*))func_800FD68C)((char*)self - 0x604); }

extern "C" void func_800F4798(void* self);
extern "C" void func_800FD6A0(void* self) { ((void(*)(void*))func_800F4798)((char*)self - 0x604); }

void cf::CfObjectSelectorObj::~CfObjectSelectorObj() {}

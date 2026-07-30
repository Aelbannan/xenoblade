// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPlayAward
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuPlayAward.hpp"

// forward declarations for scaffold thunk references
void cbRenderBefore__14CMenuPlayAwardFv(void*);
void __dt__14CMenuPlayAwardFv(void*);

void __ct__CMenuPlayAward(){}

CMenuPlayAward::~CMenuPlayAward() {}

void CMenuPlayAward::Init() {}

void CMenuPlayAward::Term() {}

void CMenuPlayAward::Move() {}

void CMenuPlayAward::cbRenderBefore() {}

void func_80270308(){}

void stub_us_80272800() {}

void func_8027038C(){}

void func_80270404(){}

void func_80270454(){}

void func_802705F4(){}

void func_80270644(void* self) { ((void(*)(void*))cbRenderBefore__14CMenuPlayAwardFv)((char*)self - 0x58); }

void func_8027064C(void* self) { ((void(*)(void*))__dt__14CMenuPlayAwardFv)((char*)self - 0x58); }

void func_80270654(){}

void func_802706C4(void* self) {
    *(unsigned long*)((char*)self + 0) = 0;
    *(unsigned long*)((char*)self + 4) = 0;
}

void __dt__802706D4(){}

void __dt__80270714(){}

void func_80270770(){}

void func_80270AD8(){}

void* func_80270AEC(void* self, int param) {
    if (param >= 256) return 0;
    CMenuPlayAward* menu = static_cast<CMenuPlayAward*>(self);
    return (void*)((unsigned char*)menu + (menu->mPageIndex << 11) + ((param & 0xFF) << 3));
}

void __ct__CPlayAwardList(){}

CPlayAwardList::~CPlayAwardList() {}

void func_80270CEC(){}

void func_80270D64(){}

void func_80270E04(){}

void func_80270E64(){}

bool CScrollBar_isVisible(void*);

unsigned char func_80270F28(void* this_) {
    if (CScrollBar_isVisible((void*)((char*)this_ + 0x48))) {
        return *((unsigned char*)this_ + 0x8a);
    }
    return 0;
}

u8 func_80270F6C(void* self) { return static_cast<CMenuPlayAward*>(self)->mField_8B; }

void func_80270F74(){}

void func_80271070(){}

void func_802710D4(){}

void func_80271190(){}

void func_80271260(){}

void func_80271300(){}

void func_802713BC(){}

void func_80271468(){}

void func_80271480(){}

void func_802714D4(){}

void func_80271528(){}

void func_80271574(){}

void func_802715C0(){}

void func_80271620(){}

void func_80271680(){}

void func_80271730(){}

void func_802717F8(){}

void func_802719F8(){}

void CPlayAwardList::OnFileEvent() {}

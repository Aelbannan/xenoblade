// TU for kyoshin/CPcKizunagram — PC affinity chart window.
// func_8025DA40 / func_8025DA48: FULL_MATCH (byte-identical).

#include "kyoshin/CPcKizunagram.hpp"

extern "C" u8 func_8025DA40(CPcKizunagram* pKizunagram) { return pKizunagram->mIsHidden; }










extern "C" void func_8025D8C4() {}

extern "C" void func_8025D954() {}

extern "C" void func_8025D9C4() {}


extern "C" u8 func_8025DA48(CPcKizunagram* pKizunagram) { return pKizunagram->mIsOpen; }

extern "C" void func_8025DA50() {}

extern "C" void func_8025DA78() {}

extern "C" void func_8025DAE8() {}

extern "C" void func_8025DB30() {}

extern "C" void func_8025DB78() {}

extern "C" void func_8025DBC0() {}

extern "C" void func_8025DC08() {}

extern "C" void func_8025DC8C() {}

extern "C" void func_8025DCB0() {}

extern "C" void func_8025DCFC() {}

extern "C" void func_8025E0D8() {}

extern "C" void func_8025E3A4() {}

extern "C" void func_8025E4A4() {}

extern "C" void func_8025E56C() {}

extern "C" void func_8025E5A8() {}

extern "C" void func_8025E5E4() {}

extern "C" void func_8025E904() {}

extern "C" void func_8025E960() {}

extern "C" void func_8025E9E4() {}

extern "C" void OnFileEvent__13CPcKizunagramFP10CEventFile() {}

extern "C" void func_8025EC0C() {}

extern "C" void func_8025ECE4() {}

extern "C" void func_8025EDC8() {}

extern "C" void func_8025EE7C(void* self, u32 r4) {
    if (*(u32*)((u8*)self + 0x89C) != r4) {
        *(u32*)((u8*)self + 0x89C) = r4;
        func_8025EE94(self);
    }
}

extern "C" void func_8025EE94(void*) { volatile int _x = 0; (void)_x; }

extern "C" void func_8025F114() {}

extern "C" void func_8025F290() {}

extern "C" void func_8025F2E8() {}

extern "C" void func_8025F528() {}

extern "C" void func_8025F768() {}

extern "C" void func_8025F9AC() {}

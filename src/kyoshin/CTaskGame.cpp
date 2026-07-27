#include "kyoshin/CTaskGame.hpp"

void* CTaskGame_cLoadInstance;
u32 lbl_80666624;
u32 lbl_80666630;
u32 lbl_80666634;

void func_8004041C(void* self, u8 a1, float a2, int a3, u32 a4, u8 a5, u32 a6, u32 a7) {
    struct Fields {
        u8 field0;
        float field4;
        int field8;
        u32 fieldC;
        u8 field10;
        u32 field14;
        u32 field18;
    };
    Fields& f = *(Fields*)self;
    f.field0 = a1;
    f.field4 = a2;
    f.field8 = a3;
    f.fieldC = a4;
    f.field10 = a5;
    f.field14 = a6;
    f.field18 = a7;
}

CTaskGame::CTaskGame(CView* pView, CWorkThread* pThread, int r6) :
    unk68(0),
    unk6C(pThread),
    unk70(pView),
    unk74(0),
    unk78(0),
    unk7C(0),
    unk80(1),
    unk82(1),
    unk84(0),
    unk86(0),
    unk88(0),
    unk8A(0),
    unk8E(0),
    unk90(0),
    unkA0(0),
    unkA4(),
    unkC8(r6),
    unkCC(0),
    unkD0(0),
    unkD4(0),
    unkD8(-1),
    unkDC(2),
    unkE0(1),
    unkE4(0),
    unkE8(-1),
    unkEC(0),
    unkF0(0),
    unkF4(0),
    unkF8(-1),
    unkFC(0),
    unk100(0),
    unk104(0),
    unk124(0),
    unk128(0),
    unk130(0),
    unk170(0),
    unk188(0) {
    unk18C.func_8004041C(0, -1, 2, 0, 0, 0, 1);
    spInstance = this;
    CTaskGame_cLoadInstance = nullptr;
    lbl_80666624 = 0;
    lbl_80666630 = 0;
    lbl_80666634 = 0;
}

CTaskGame::~CTaskGame(){
    spInstance = nullptr;
}

CTaskGame* CTaskGame::getInstance(){
    return spInstance;
}

u32 CTaskGame::func_800404F0() {
    CTaskGame* instance = spInstance;
    if (instance == NULL) {
        return 0;
    }
    return (instance->unk68 >> 13) & 1;
}

void CTaskGame::Init(){

}

CTaskGame* CTaskGame::create(CView* pView, CWorkThread* pThread, int r5){
    mtl::ALLOC_HANDLE handle = CWorkThreadSystem::getWorkMem();
    CTaskGame* taskGame = new (handle) CTaskGame(pView, pThread, r5);
    //Register CTaskGame under the root game process
    CProcess* gameProcess = CTaskManager::GetRootProcGame();
    taskGame->Regist(gameProcess, false);
    return taskGame;
}

void func_800407C8() {}
void func_80040AF4(){}
void func_800419BC(){}
void func_80041AFC(){}
void func_800426A4() {}
int func_800426F0__9CTaskGameFv() {
    extern void* lbl_eu_80663D18;
    if (lbl_eu_80663D18 == 0) return 1;
    return *(reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(lbl_eu_80663D18) + 0x68)) & 1;
}
void func_80042710__9CTaskGameFv() {}
int lbl_eu_80663D1C;

int func_80042864() {
    return lbl_eu_80663D1C != 0;
}
void func_80042FBC(){}
int func_80043024(void* self) { return 0x0; }
void func_80043310(){}
void func_8004335C(){}
bool func_800433A8() { return true; }
void func_800433B0(void* self, bool enabled, unsigned int mode) { unsigned int& flags = *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x68); flags &= ~0x100u; if (enabled) flags |= 0x200u; else flags &= ~0x200u; if (!enabled) { if (mode == 1u) flags |= 0x20000u; else if (mode == 2u) flags |= 0x40000u; } }
void func_80043410(void* self, int enabled, unsigned int mode, unsigned int value) {
    volatile unsigned int* flags_ptr = reinterpret_cast<volatile unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x68);
    unsigned int flags = *flags_ptr;
    flags &= ~0x100u;
    *flags_ptr = flags;
    if (enabled != 0) {
        flags |= 0x400u;
        *flags_ptr = flags;
    } else {
        flags &= ~0x400u;
        *flags_ptr = flags;
    }
    if (enabled != 0)
        return;
    if (mode == 0) {
        flags = *flags_ptr;
        flags |= 0x4000u;
        *flags_ptr = flags;
        return;
    }
    if (mode != 5 && mode != 3)
        return;
    flags = *flags_ptr;
    *reinterpret_cast<volatile unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0xfc) = value;
    flags |= 0x8000u;
    *flags_ptr = flags;
}
void func_8004347C(void* this_, int arg1, int arg2, unsigned int arg3) { unsigned int flags = *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this_) + 0x68); flags &= ~0x00000100u; *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this_) + 0x68) = flags; if (arg1 != 0) { flags |= 0x00080000u; *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this_) + 0x68) = flags; } else { flags &= ~0x00080000u; *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this_) + 0x68) = flags; } *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this_) + 0xfc) = arg3; }
void func_800434AC(unsigned char* this_, int enabled, int unused, unsigned int value) { unsigned int& flags = *reinterpret_cast<unsigned int*>(this_ + 0x68); flags &= 0xFFFFFEFFu; if (enabled) flags |= 0x800u; else flags &= 0xFFFFF7FFu; *reinterpret_cast<unsigned int*>(this_ + 0xFC) = value; }
void func_800434DC(unsigned char* self, int enabled, int unused, unsigned int value) { unsigned int flags = *(unsigned int*)(self + 0x68); flags &= ~0x00000100u; *(unsigned int*)(self + 0x68) = flags; if (enabled != 0) { flags |= 0x00100000u; *(unsigned int*)(self + 0x68) = flags; } else { flags &= ~0x00100000u; *(unsigned int*)(self + 0x68) = flags; } *(unsigned int*)(self + 0xfc) = value; }
void func_8004350C(){}
void func_80043538(void* self, int enabled) { volatile unsigned int* flags = reinterpret_cast<volatile unsigned int*>(static_cast<unsigned char*>(self) + 0x68); unsigned int value = *flags; value &= ~0x00000100u; *flags = value; if (enabled != 0) { value |= 0x01000000u; *flags = value; } else { value &= ~0x01000000u; *flags = value; } }
void func_80043564(){}
void func_80039364__5CGameFv();

void func_80043628() {
    func_80039364__5CGameFv();
}
s32 func_800436A8();
s32 CTaskGame::func_8004368C() {
    extern void* lbl_eu_80663D18;
    if (lbl_eu_80663D18 == NULL) {
        return 0;
    }
    return func_800436A8();
}
void func_800436F4(void* self, int value) { unsigned int& flags = *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x68); if ((flags & 0x2000) != 0 && value == 1) return; void* object = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(self) + 0xf0); *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(self) + 0xf4) = value; flags |= 0x1000; if (object != 0) *reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned char*>(object) + 0xea) = 1; }
void func_80043730(void* obj, int val) {
    *(int*)((char*)obj + 0xf8) = val;
}
int func_80043BA4() {
    extern void* lbl_eu_80663D18;
    extern int func_80459AA4__7CLibCriFv(unsigned int arg);
    if (lbl_eu_80663D18 == 0) return 0;
    return func_80459AA4__7CLibCriFv(*(reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(lbl_eu_80663D18) + 0xd8)));
}
bool func_80043D68() {
    extern void* lbl_eu_80663D18;
    if (lbl_eu_80663D18 == 0) return false;
    unsigned int value = *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(lbl_eu_80663D18) + 0xd8);
    return value != 0xffffffffu;
}
u32 func_80043F18(void *ptr) { return *(u32 *)ptr; }
void Tail__8CProcessFv() {}
void __dt__9CTaskGameFv(CTaskGame*);
void func_80044120(void *self) {
    __dt__9CTaskGameFv(reinterpret_cast<CTaskGame*>((char*)self - 0x54));
}
void cbRenderBefore__9CTaskGameFv(void*);
void func_80044128(void* self) { cbRenderBefore__9CTaskGameFv((void*)((char*)self - 0x58)); }
void func_80044130(void* p) {
    p = (char*)p - 0x58;
    __dt__9CTaskGameFv((CTaskGame*)p);
}
bool func_80044138(void* ptr) { return ((bool(*)(char*))func_800433A8)((char*)ptr - 0x5c); }
void func_80044140(void* p) {
    __dt__9CTaskGameFv((CTaskGame*)((char*)p - 0x5c));
}
bool func_80044148(void* obj) { return ((bool (*)(void*))func_800436F4)((void*)((char*)obj - 0x60)); }
void func_80044150(void* p) {
    return __dt__9CTaskGameFv((CTaskGame*)((char*)p - 0x60));
}
bool func_80044158(void* p) {
    return ((bool(*)(char*))func_80043730)((char*)p - 0x64);
}
void func_80044160(void* p) {
    __dt__9CTaskGameFv((CTaskGame*)((char*)p - 0x64));
}

void CTaskGame::Term() {}
void CTaskGame::func_80040A3C() {}
void func_80040B38(){}
void func_80040C2C(){}
void func_80040CD8(){}
void func_80040DE4(){}
void func_80040EB4(){}
void func_80041024(){}
void func_800411A4(){}
void func_8004125C(){}
void func_80041348(){}
void func_80041390(){}
void func_80041448(){}
void func_800414F4(){}
void func_800415AC(){}
void func_80041658(){}
void func_800416FC(){}
void func_800417AC(){}
void func_8004185C(){}
void func_80041908(){}
void func_80041A48(){}
void func_80041B94(){}
void func_80041BC0(){}
void func_80041CC8(){}
void func_80041E54(){}
void func_80041F54(){}
void func_80042048(){}
void func_8004213C(){}
void func_80042274(){}
void func_800424E0(){}
void func_8004256C(){}
void func_80042630(){}
void func_800426A8(){}
void CTaskGame::func_80042720() {}
void func_80042784(){}
void func_80042874(){}
void func_8004302C(){}
// Forward declaration only - body kept in separate TU to prevent MWCC inlining
void func_8004312C();
void func_8004362C(){}
#pragma dont_inline on
s32 func_800436A8(){ return 0; }
#pragma dont_inline reset
void func_80043738(){}
void func_8004392C(){}
void func_80043B04(){}
void func_80043B54(){}
void func_80043BC4(){}
void func_80043C88(){}
void func_80043CD8(){}
void func_80043D90(){}
void func_80043E08(){}
void func_80044070(){}
char* func_800440C4(char* r3_this, const char* r4_str) {
    size_t len = strlen(r4_str);
    strcat(r3_this, r4_str);
    *(int*)(r3_this + 0x20) += (int)len;
    return r3_this;
}

void func_8004312C(); void Draw__9CTaskGameFv() {
    func_8004312C();
}

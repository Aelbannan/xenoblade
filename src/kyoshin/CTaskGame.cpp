#include "kyoshin/CTaskGame.hpp"

void* CTaskGame_cLoadInstance;
u32 lbl_80666624;
u32 lbl_80666630;
u32 lbl_80666634;

extern "C" void func_8004041C(void* self, u8 a1, float a2, int a3, u32 a4, u8 a5, u32 a6, u32 a7) {
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

// LLM-HARNESS-BEGIN: us-80040da0
extern "C" void func_800407C8() {}
// LLM-HARNESS-END: us-80040da0
// LLM-HARNESS-BEGIN: us-800410cc
extern "C" void func_80040AF4() {}
// LLM-HARNESS-END: us-800410cc
// LLM-HARNESS-BEGIN: us-80041f80
extern "C" void func_800419BC() {}
// LLM-HARNESS-END: us-80041f80
// LLM-HARNESS-BEGIN: us-80042078
extern "C" void func_80041AFC() {}
// LLM-HARNESS-END: us-80042078
// LLM-HARNESS-BEGIN: us-80042c1c
extern "C" void func_800426A4() {}
// LLM-HARNESS-END: us-80042c1c
// LLM-HARNESS-BEGIN: us-80042c68
extern "C" void func_800426F0__9CTaskGameFv() {}
// LLM-HARNESS-END: us-80042c68
// LLM-HARNESS-BEGIN: us-80042c88
extern "C" void func_80042710__9CTaskGameFv() {}
// LLM-HARNESS-END: us-80042c88
// LLM-HARNESS-BEGIN: us-80042ddc
extern "C" int lbl_eu_80663D1C;

extern "C" int func_80042864() {
    return lbl_eu_80663D1C != 0;
}
// LLM-HARNESS-END: us-80042ddc
// LLM-HARNESS-BEGIN: us-80043534
extern "C" void func_80042FBC() {}
// LLM-HARNESS-END: us-80043534
// LLM-HARNESS-BEGIN: us-8004359c
extern "C" bool func_80043024() { return false; }
// LLM-HARNESS-END: us-8004359c
// LLM-HARNESS-BEGIN: us-80043888
extern "C" void func_80043310() {}
// LLM-HARNESS-END: us-80043888
// LLM-HARNESS-BEGIN: us-800438d4
extern "C" void func_8004335C() {}
// LLM-HARNESS-END: us-800438d4
// LLM-HARNESS-BEGIN: us-80043920
extern "C" bool func_800433A8() { return true; }
// LLM-HARNESS-END: us-80043920
// LLM-HARNESS-BEGIN: us-80043928
extern "C" void func_800433B0(void* self, bool enabled, unsigned int mode) { unsigned int& flags = *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x68); flags &= ~0x100u; if (enabled) flags |= 0x200u; else flags &= ~0x200u; if (!enabled) { if (mode == 1u) flags |= 0x20000u; else if (mode == 2u) flags |= 0x40000u; } }
// LLM-HARNESS-END: us-80043928
// LLM-HARNESS-BEGIN: us-80043988
extern "C" void func_80043410(void* self, int enabled, unsigned int mode, unsigned int value) {
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
// LLM-HARNESS-END: us-80043988
// LLM-HARNESS-BEGIN: us-800439f4
extern "C" void func_8004347C(void* this_, int arg1, int arg2, unsigned int arg3) { unsigned int flags = *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this_) + 0x68); flags &= ~0x00000100u; *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this_) + 0x68) = flags; if (arg1 != 0) { flags |= 0x00080000u; *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this_) + 0x68) = flags; } else { flags &= ~0x00080000u; *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this_) + 0x68) = flags; } *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this_) + 0xfc) = arg3; }
// LLM-HARNESS-END: us-800439f4
// LLM-HARNESS-BEGIN: us-80043a24
extern "C" void func_800434AC(unsigned char* this_, int enabled, int unused, unsigned int value) { unsigned int& flags = *reinterpret_cast<unsigned int*>(this_ + 0x68); flags &= 0xFFFFFEFFu; if (enabled) flags |= 0x800u; else flags &= 0xFFFFF7FFu; *reinterpret_cast<unsigned int*>(this_ + 0xFC) = value; }
// LLM-HARNESS-END: us-80043a24
// LLM-HARNESS-BEGIN: us-80043a54
extern "C" void func_800434DC(unsigned char* self, int enabled, int unused, unsigned int value) { unsigned int flags = *(unsigned int*)(self + 0x68); flags &= ~0x00000100u; *(unsigned int*)(self + 0x68) = flags; if (enabled != 0) { flags |= 0x00100000u; *(unsigned int*)(self + 0x68) = flags; } else { flags &= ~0x00100000u; *(unsigned int*)(self + 0x68) = flags; } *(unsigned int*)(self + 0xfc) = value; }
// LLM-HARNESS-END: us-80043a54
// LLM-HARNESS-BEGIN: us-80043a84
extern "C" void func_8004350C() {}
// LLM-HARNESS-END: us-80043a84
// LLM-HARNESS-BEGIN: us-80043ab0
extern "C" void func_80043538(void* self, int enabled) { volatile unsigned int* flags = reinterpret_cast<volatile unsigned int*>(static_cast<unsigned char*>(self) + 0x68); unsigned int value = *flags; value &= ~0x00000100u; *flags = value; if (enabled != 0) { value |= 0x01000000u; *flags = value; } else { value &= ~0x01000000u; *flags = value; } }
// LLM-HARNESS-END: us-80043ab0
// LLM-HARNESS-BEGIN: us-80043adc
extern "C" void func_80043564() {}
// LLM-HARNESS-END: us-80043adc
// LLM-HARNESS-BEGIN: us-80043ba0
extern "C" void func_80039364__5CGameFv();

extern "C" void func_80043628() {
    func_80039364__5CGameFv();
}
// LLM-HARNESS-END: us-80043ba0
// LLM-HARNESS-BEGIN: us-80043c08
extern "C" void func_8004368C__9CTaskGameFv() {}
// LLM-HARNESS-END: us-80043c08
// LLM-HARNESS-BEGIN: us-80043c70
extern "C" void func_800436F4(void* self, int value) { unsigned int& flags = *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x68); if ((flags & 0x2000) != 0 && value == 1) return; void* object = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(self) + 0xf0); *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(self) + 0xf4) = value; flags |= 0x1000; if (object != 0) *reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned char*>(object) + 0xea) = 1; }
// LLM-HARNESS-END: us-80043c70
// LLM-HARNESS-BEGIN: us-80043cac
extern "C" void func_80043730(void* obj, int val) {
    *(int*)((char*)obj + 0xf8) = val;
}
// LLM-HARNESS-END: us-80043cac
// LLM-HARNESS-BEGIN: us-80044140
extern "C" void func_80043BA4() {}
// LLM-HARNESS-END: us-80044140
// LLM-HARNESS-BEGIN: us-80044304
extern "C" bool func_80043D68() {
    extern void* lbl_eu_80663D18;
    if (lbl_eu_80663D18 == 0) return false;
    unsigned int value = *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(lbl_eu_80663D18) + 0xd8);
    return value != 0xffffffffu;
}
// LLM-HARNESS-END: us-80044304
// LLM-HARNESS-BEGIN: us-800444b4
extern "C" u32 func_80043F18(void *ptr) { return *(u32 *)ptr; }
// LLM-HARNESS-END: us-800444b4
// LLM-HARNESS-BEGIN: us-800444bc
extern "C" void Tail__8CProcessFv() {}
// LLM-HARNESS-END: us-800444bc
// LLM-HARNESS-BEGIN: us-800446bc
extern "C" void __dt__9CTaskGameFv(CTaskGame*);

extern "C" void func_80044120(void *self) {
    __dt__9CTaskGameFv(reinterpret_cast<CTaskGame*>((char*)self - 0x54));
}
// LLM-HARNESS-END: us-800446bc
// LLM-HARNESS-BEGIN: us-800446c4
extern "C" void cbRenderBefore__9CTaskGameFv(void*);
extern "C" void func_80044128(void* self) { cbRenderBefore__9CTaskGameFv((void*)((char*)self - 0x58)); }
// LLM-HARNESS-END: us-800446c4
// LLM-HARNESS-BEGIN: us-800446cc
extern "C" void func_80044130(void* p) {
    p = (char*)p - 0x58;
    __dt__9CTaskGameFv((CTaskGame*)p);
}
// LLM-HARNESS-END: us-800446cc
// LLM-HARNESS-BEGIN: us-800446d4
extern "C" bool func_80044138(void* ptr) { return ((bool(*)(char*))func_800433A8)((char*)ptr - 0x5c); }
// LLM-HARNESS-END: us-800446d4
// LLM-HARNESS-BEGIN: us-800446dc
extern "C" void func_80044140(void* p) {
    __dt__9CTaskGameFv((CTaskGame*)((char*)p - 0x5c));
}
// LLM-HARNESS-END: us-800446dc
// LLM-HARNESS-BEGIN: us-800446e4
extern "C" bool func_80044148(void* obj) { return ((bool (*)(void*))func_800436F4)((void*)((char*)obj - 0x60)); }
// LLM-HARNESS-END: us-800446e4
// LLM-HARNESS-BEGIN: us-800446ec
extern "C" void func_80044150(void* p) {
    return __dt__9CTaskGameFv((CTaskGame*)((char*)p - 0x60));
}
// LLM-HARNESS-END: us-800446ec
// LLM-HARNESS-BEGIN: us-800446f4
extern "C" bool func_80044158(void* p) {
    return ((bool(*)(char*))func_80043730)((char*)p - 0x64);
}
// LLM-HARNESS-END: us-800446f4
// LLM-HARNESS-BEGIN: us-800446fc
extern "C" void func_80044160(void* p) {
    __dt__9CTaskGameFv((CTaskGame*)((char*)p - 0x64));
}
// LLM-HARNESS-END: us-800446fc

// LLM-HARNESS-BEGIN: us-80040db4
extern "C" void Term__9CTaskGameFv() {}
// LLM-HARNESS-END: us-80040db4
// LLM-HARNESS-BEGIN: us-80041014
extern "C" void func_80040A3C__9CTaskGameFUsUsPCcs() {}
// LLM-HARNESS-END: us-80041014
// LLM-HARNESS-BEGIN: us-80041110
extern "C" void func_80040B38() {}
// LLM-HARNESS-END: us-80041110
// LLM-HARNESS-BEGIN: us-80041204
extern "C" void func_80040C2C() {}
// LLM-HARNESS-END: us-80041204
// LLM-HARNESS-BEGIN: us-800412b0
extern "C" void func_80040CD8() {}
// LLM-HARNESS-END: us-800412b0
// LLM-HARNESS-BEGIN: us-800413bc
extern "C" void func_80040DE4() {}
// LLM-HARNESS-END: us-800413bc
// LLM-HARNESS-BEGIN: us-8004148c
extern "C" void func_80040EB4() {}
// LLM-HARNESS-END: us-8004148c
// LLM-HARNESS-BEGIN: us-800415fc
extern "C" void func_80041024() {}
// LLM-HARNESS-END: us-800415fc
// LLM-HARNESS-BEGIN: us-8004177c
extern "C" void func_800411A4() {}
// LLM-HARNESS-END: us-8004177c
// LLM-HARNESS-BEGIN: us-8004184c
extern "C" void func_8004125C() {}
// LLM-HARNESS-END: us-8004184c
// LLM-HARNESS-BEGIN: us-80041940
extern "C" void func_80041348() {}
// LLM-HARNESS-END: us-80041940
// LLM-HARNESS-BEGIN: us-800419b4
extern "C" void func_80041390() {}
// LLM-HARNESS-END: us-800419b4
// LLM-HARNESS-BEGIN: us-80041a5c
extern "C" void func_80041448() {}
// LLM-HARNESS-END: us-80041a5c
// LLM-HARNESS-BEGIN: us-80041af8
extern "C" void func_800414F4() {}
// LLM-HARNESS-END: us-80041af8
// LLM-HARNESS-BEGIN: us-80041ba0
extern "C" void func_800415AC() {}
// LLM-HARNESS-END: us-80041ba0
// LLM-HARNESS-BEGIN: us-80041c3c
extern "C" void func_80041658() {}
// LLM-HARNESS-END: us-80041c3c
// LLM-HARNESS-BEGIN: us-80041ce0
extern "C" void func_800416FC() {}
// LLM-HARNESS-END: us-80041ce0
// LLM-HARNESS-BEGIN: us-80041da0
extern "C" void func_800417AC() {}
// LLM-HARNESS-END: us-80041da0
// LLM-HARNESS-BEGIN: us-80041e40
extern "C" void func_8004185C() {}
// LLM-HARNESS-END: us-80041e40
// LLM-HARNESS-BEGIN: us-80041edc
extern "C" void func_80041908() {}
// LLM-HARNESS-END: us-80041edc
// LLM-HARNESS-BEGIN: us-80041fd4
extern "C" void func_80041A48() {}
// LLM-HARNESS-END: us-80041fd4
// LLM-HARNESS-BEGIN: us-800420d8
extern "C" void func_80041B94() {}
// LLM-HARNESS-END: us-800420d8
// LLM-HARNESS-BEGIN: us-80042130
extern "C" void func_80041BC0() {}
// LLM-HARNESS-END: us-80042130
// LLM-HARNESS-BEGIN: us-80042240
extern "C" void func_80041CC8() {}
// LLM-HARNESS-END: us-80042240
// LLM-HARNESS-BEGIN: us-800423cc
extern "C" void func_80041E54() {}
// LLM-HARNESS-END: us-800423cc
// LLM-HARNESS-BEGIN: us-800424cc
extern "C" void func_80041F54() {}
// LLM-HARNESS-END: us-800424cc
// LLM-HARNESS-BEGIN: us-800425c0
extern "C" void func_80042048() {}
// LLM-HARNESS-END: us-800425c0
// LLM-HARNESS-BEGIN: us-800426b4
extern "C" void func_8004213C() {}
// LLM-HARNESS-END: us-800426b4
// LLM-HARNESS-BEGIN: us-800427ec
extern "C" void func_80042274() {}
// LLM-HARNESS-END: us-800427ec
// LLM-HARNESS-BEGIN: us-80042a58
extern "C" void func_800424E0() {}
// LLM-HARNESS-END: us-80042a58
// LLM-HARNESS-BEGIN: us-80042ae4
extern "C" void func_8004256C() {}
// LLM-HARNESS-END: us-80042ae4
// LLM-HARNESS-BEGIN: us-80042ba8
extern "C" void func_80042630() {}
// LLM-HARNESS-END: us-80042ba8
// LLM-HARNESS-BEGIN: us-80042c20
extern "C" void func_800426A8() {}
// LLM-HARNESS-END: us-80042c20
// LLM-HARNESS-BEGIN: us-80042c98
extern "C" void func_80042720__9CTaskGameFv() {}
// LLM-HARNESS-END: us-80042c98
// LLM-HARNESS-BEGIN: us-80042cfc
extern "C" void func_80042784() {}
// LLM-HARNESS-END: us-80042cfc
// LLM-HARNESS-BEGIN: us-80042dec
extern "C" void func_80042874() {}
// LLM-HARNESS-END: us-80042dec
// LLM-HARNESS-BEGIN: us-800435a4
extern "C" void func_8004302C() {}
// LLM-HARNESS-END: us-800435a4
// LLM-HARNESS-BEGIN: us-800436a4
extern "C" void func_8004312C() {}
// LLM-HARNESS-END: us-800436a4
// LLM-HARNESS-BEGIN: us-80043ba4
extern "C" void func_8004362C() {}
// LLM-HARNESS-END: us-80043ba4
// LLM-HARNESS-BEGIN: us-80043c24
extern "C" void func_800436A8() {}
// LLM-HARNESS-END: us-80043c24
// LLM-HARNESS-BEGIN: us-80043cb4
extern "C" void func_80043738() {}
// LLM-HARNESS-END: us-80043cb4
// LLM-HARNESS-BEGIN: us-80043ec8
extern "C" void func_8004392C() {}
// LLM-HARNESS-END: us-80043ec8
// LLM-HARNESS-BEGIN: us-800440a0
extern "C" void func_80043B04() {}
// LLM-HARNESS-END: us-800440a0
// LLM-HARNESS-BEGIN: us-800440f0
extern "C" void func_80043B54() {}
// LLM-HARNESS-END: us-800440f0
// LLM-HARNESS-BEGIN: us-80044160
extern "C" void func_80043BC4() {}
// LLM-HARNESS-END: us-80044160
// LLM-HARNESS-BEGIN: us-80044224
extern "C" void func_80043C88() {}
// LLM-HARNESS-END: us-80044224
// LLM-HARNESS-BEGIN: us-80044274
extern "C" void func_80043CD8() {}
// LLM-HARNESS-END: us-80044274
// LLM-HARNESS-BEGIN: us-8004432c
extern "C" void func_80043D90() {}
// LLM-HARNESS-END: us-8004432c
// LLM-HARNESS-BEGIN: us-800443a4
extern "C" void func_80043E08() {}
// LLM-HARNESS-END: us-800443a4
// LLM-HARNESS-BEGIN: us-8004460c
extern "C" void func_80044070() {}
// LLM-HARNESS-END: us-8004460c
// LLM-HARNESS-BEGIN: us-80044660
extern "C" void func_800440C4() {}
// LLM-HARNESS-END: us-80044660

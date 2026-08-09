// Auto-scaffolded catalog TU for kyoshin/plugin/ocUnit
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/plugin/ocUnit.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

struct CfObjIf {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void _v0048();
    virtual void _v004C();
    virtual void _v0050();
    virtual void _v0054();
    virtual void _v0058();
    virtual void _v005C();
    virtual void _v0060();
    virtual void _v0064();
    virtual void _v0068();
    virtual void _v006C();
    virtual void _v0070();
    virtual void _v0074();
    virtual void _v0078();
    virtual void _v007C();
    virtual void _v0080();
    virtual void _v0084();
    virtual void _v0088();
    virtual void _v008C();
    virtual void _v0090();
    virtual void _v0094();
    virtual void _v0098();
    virtual void _v009C();
    virtual void _v00A0();
    virtual void _v00A4();
    virtual void _v00A8();
    virtual void _v00AC();
    virtual void _v00B0();
    virtual void _v00B4();
    virtual void _v00B8();
    virtual void _v00BC();
    virtual void _v00C0();
    virtual void vf00C4();
};

// C-linkage retail symbols referenced by the plugin functions below.
extern "C" {
    extern u32 lbl_eu_80663E24;
    extern char lbl_eu_804FA74C[];
    extern void* lbl_eu_806618D8;
    extern void* lbl_eu_806618F0;
    extern void* lbl_eu_806618E8;
    extern "C" void* __RTTI__Q22cf13CfObjectActor;
    extern float lbl_eu_80665C30;
    extern float lbl_eu_8066A20C;

    // VM/script helpers
    void func_800BE12C(void* obj, int a, int b, int c, int d);
    void func_800BE824(void* obj, int flag);
    void func_800BE0F8(void* obj, int target);
    void func_800BE33C(void* obj, int flag);
    void* func_800BBC0C();
    void func_800BF314(void* obj, int flag);
    void func_800F38E0(void* battleMgr, void* actor, int flag);
    void func_800F3958(void* battleMgr, void* actor, int index);
    void func_800EC8FC(void* battleMgr, void* actor, void* data, int flag);
    u32 func_800FE68C();
    void func_800ACC14(void* obj, s8 val);
    void func_800ACF78(void* obj, void* target, void* child);
    void func_800ACFD8(void* obj, void* target);
    void func_800AC4A8(void* obj, u16 param);
    void func_800ABF24(void* obj, void* pos, void* offset, float f);
    void func_8013D07C(void* subObj, const char* str, int flag);
    void func_8013D448(void* subObj, const char* str);
    unsigned int func_8013EC58();
    int func_80174C98(void* player, u32* val, int flag);
    unsigned char code80135FDC_getByte_64058();
    void* __dynamic_cast(void* obj, int offset, void* rtti, void* targetRtti, int flag);
    int strcmp(const char* s1, const char* s2);
    void* memset(void* ptr, int val, u32 size);
    void* getInstance__Q22cf14CBattleManagerFv();
    void* func_80081694__Q22cf13CfGameManagerFv(unsigned short a, unsigned short b);
    void* func_80081CBC__Q22cf13CfGameManagerFv(const char* name, unsigned short param);
    void* func_80081CB8__Q22cf13CfGameManagerFv();
    void* func_800817BC__Q22cf13CfGameManagerFv(int index, int mode);
    void* func_8008187C__Q22cf13CfGameManagerFv(int index);
}

void func_8003BC10(){}

int cf::CfObjectMove::CfObject_UnkVirtualFunc50() { return field_6CE; }

int cf::CfObjectMove::CfObject_UnkVirtualFunc51() { return field_6CF; }

void func_8003BD7C(){}

extern "C" int func_8003C044(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    VMArg retVal;
    retVal.type = 4;
    retVal.value.intVal = (int)(lbl_eu_80665C30 * *(float*)obj->CfObject_UnkVirtualFunc23());
    vmRetValSet(pThread, &retVal);
    return 1;
}

u32 cf::CfObject::CfObject_UnkVirtualFunc23() { return (u32)&mPos3C; }

extern "C" int func_8003C0D0(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    VMArg retVal;
    retVal.type = 4;
    retVal.value.intVal = (int)(lbl_eu_80665C30 * *(float*)((u8*)obj->CfObject_UnkVirtualFunc23() + 4));
    vmRetValSet(pThread, &retVal);
    return 1;
}

extern "C" int func_8003C154(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    VMArg retVal;
    retVal.type = 4;
    retVal.value.intVal = (int)(lbl_eu_80665C30 * *(float*)((u8*)obj->CfObject_UnkVirtualFunc23() + 8));
    vmRetValSet(pThread, &retVal);
    return 1;
}

extern "C" int func_8003C1D8(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    VMArg retVal;
    retVal.type = 3;
    retVal.value.intVal = (int)obj->CfObject_UnkVirtualFunc34();
    vmRetValSet(pThread, &retVal);
    return 1;
}

float cf::CfObject::CfObject_UnkVirtualFunc34() { return mField4C * 0.019592438f; }

extern "C" int func_8003C260(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    VMArg retVal;
    retVal.type = 5;
    retVal.unk2 = (u16)strlen((const char*)((cf::CObjectParam*)obj)->CObjectParam_UnkVirtualFunc2());
    retVal.value.pointerVal = ((cf::CObjectParam*)obj)->CObjectParam_UnkVirtualFunc2();
    vmRetValSet(pThread, &retVal);
    return 1;
}

void* cf::CObjectParam::CObjectParam_UnkVirtualFunc2() { return &mPtr10; }

extern "C" int func_8003C2F4(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    VMArg retVal;
    retVal.type = 3;
    retVal.value.uintVal = *(u16*)((u8*)obj + 0x8C);
    vmRetValSet(pThread, &retVal);
    return 1;
}

extern "C" int func_8003C354(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    VMArg retVal;
    if (obj) {
        retVal.type = 3;
        retVal.value.uintVal = *(u32*)((u8*)obj + 0x74);
    } else {
        retVal.type = 3;
        retVal.value.uintVal = 0;
    }
    vmRetValSet(pThread, &retVal);
    return 1;
}

void func_8003C3D0(){}

float cf::CActorParam::CActorParam_UnkVirtualFunc37() { return *(float*)((u8*)this + 0x17E8); }

void func_8003C480(){}

void CfObject_UnkVirtualFunc22__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

void func_8003C560(){}

void func_8003C624(){}

void func_8003C6E8(){}

void cf::CfObject::CfObject_UnkVirtualFunc30() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[0xC4 / 4];
    func(this);
}

void cf::CfObject::CfObject_UnkVirtualFunc29(float value) { mField4C = value; }

void func_8003C78C(){}

extern "C" void CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv(void* self, const char* str) {
    *(u32*)((u8*)self + 0x30) = strlen(str);
    strcpy((char*)((u8*)self + 0x10), str);
}

bool isValid() { return false; }

void func_8003C84C(){}

void cf::CfObject::CfObject_UnkVirtualFunc26(u32 value, float amount) {}

void CfObject_UnkVirtualFunc19__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

void dispOn(){}

void dispOff(){}

// CObjectState_UnkVirtualFunc8 defined below as extern "C" free function

int CObjectState_UnkVirtualFunc2__Q22cf12CObjectStateFv(void* self, int mask) {
    return (*(int*)((char*)self + 4) & mask) != 0 ? 1 : 0;
}

extern void func_800BC458(void* obj);
extern "C" int func_8003CB70(VMThread* pThread, int handle) {
    int arg;
    if (vmArgOmitChk(pThread, 1)) {
        arg = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        arg = vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (arg == 0) {
        obj->CfObject_UnkVirtualFunc66(0);
    } else {
        func_800BC458(obj);
    }
    return 0;
}

void func_8003CC0C(){}

void func_8003CC9C(){}

void CfObject_UnkVirtualFunc49__Q22cf8CfObjectFv() {}

extern "C" int func_8003CD6C(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    VMArg retVal;
    retVal.type = 3;
    retVal.value.intVal = obj->CfObject_UnkVirtualFunc50();
    vmRetValSet(pThread, &retVal);
    return 1;
}

int cf::CfObject::CfObject_UnkVirtualFunc50() { return -1; }

extern int func_8003BC10(void* obj);
extern "C" int func_8003CDE0(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    int r = func_8003BC10(obj);
    VMArg retVal;
    retVal.type = !r + 1;
    vmRetValSet(pThread, &retVal);
    return 1;
}

void walkR(){}

void func_8003CED0(){}

void func_8003CF48(){}

void func_8003D060(){}

void func_8003D2B8(){}

void func_8003D32C(){}

void moveTo(){}

extern "C" int func_8003D570(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int arg = vmArgIntGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    ((void(*)(void*, int))(*(void***)obj)[0x1F8/4])(obj, arg);
    return 0;
}

void func_8003D5DC(){}

void isTalk(){}

extern "C" void* CObjectState_UnkVirtualFunc11__Q22cf12CObjectStateFv(void* self) {
    return (void*)((u8*)self + 8);
}

void onEvent(){}

// CObjectState_UnkVirtualFunc10: deferred (needs virtual-dispatch reconstruction)
extern "C" int CObjectState_UnkVirtualFunc10__Q22cf12CObjectStateFv(void* self, void* arg, int arg2) {
    (void)self; (void)arg; (void)arg2;
    return 0;
}

extern "C" int CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv(void* self, int mask) {
    if ((u32)mask < 0x3F) {
        u32 f = *(u32*)((u8*)self + 0xC) & 0x3F;
        return (u32)mask == f;
    }
    return (*(u32*)((u8*)self + 0xC) & (u32)mask) != 0;
}

extern "C" int CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv(void* self, int mask) {
    if ((u32)mask < 0x3F) {
        u32 f = *(u32*)((u8*)self + 0x8) & 0x3F;
        return (u32)mask == f;
    }
    return (*(u32*)((u8*)self + 0x8) & (u32)mask) != 0;
}

// --- Batch 2 targets start here ---

// us-8003df40: func_8003D9C4
// Checks if a CfObject supports a talk/hybridheal flag, returns 1 or 2
extern "C" int func_8003D9C4(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj->unk64 & 0x8) {
        VMArg retVal;
        if (((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc10((void*)0x2000, 1)) {
            retVal.type = 1;
        } else {
            retVal.type = 2;
        }
        vmRetValSet(pThread, &retVal);
        return 1;
    }
    VMArg retVal;
    retVal.type = 2;
    vmOCExceptionThrow(pThread);
    return 0;
}

// us-8003dfec: winTalk
// Starts a talk window interaction
extern "C" int winTalk(VMThread* pThread, int handle) {
    VMArg* arg1 = vmArgPtrGet(pThread, 1);
    const char* str = vmArgStringGet(2, arg1);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    cf::CfObjectMove* player = (cf::CfObjectMove*)cf::CfGameManager::getPlayer(0);
    if (player) {
        player = (cf::CfObjectMove*)((u8*)player - 0x3E9C);
    }
    func_8013D07C(*(void**)((u8*)obj + 0x74), str, 1);
    if (obj->unk64 & 0x8) {
        if (!((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc10((void*)1, 1)) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(1);
        }
        if (!code80135FDC_getByte_64058()) {
            u32* valPtr = (u32*)((cf::CObjectState*)player->mTargetC4)->CObjectState_UnkVirtualFunc8(0);
            u32 val = *valPtr;
            if (!func_80174C98((void*)player, (u32*)&val, 1)) {
                cf::CObjectState* state = (cf::CObjectState*)((u8*)player + 0x3E9C);
                state->CObjectState_UnkVirtualFunc1(1);
                ((void(*)(void*, void*))(*(void***)((u8*)player + 0x3E9C))[0x50/4])((u8*)player + 0x3E9C, *(void**)((u8*)obj + 0x74));
            }
        }
    } else if ((obj->unk64 & 0x4000) || (obj->unk64 & 0x8000)) {
        if (!((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc8(1)) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(1);
        }
        u32* valPtr = (u32*)((cf::CObjectState*)player->mTargetC4)->CObjectState_UnkVirtualFunc8(0);
        u32 val = *valPtr;
        if (!func_80174C98((void*)player, (u32*)&val, 1)) {
            cf::CObjectState* state = (cf::CObjectState*)((u8*)player + 0x3E9C);
            state->CObjectState_UnkVirtualFunc1(1);
            ((void(*)(void*, void*))(*(void***)((u8*)player + 0x3E9C))[0x50/4])((u8*)player + 0x3E9C, *(void**)((u8*)obj + 0x74));
        }
    }
    return 0;
}

// us-8003e1f8: func_8003DC7C
// Sets or clears a CfObject flag based on bool arg
extern "C" int func_8003DC7C(VMThread* pThread, int handle) {
    int boolVal;
    if (vmArgOmitChk(pThread, 1)) {
        boolVal = 1;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        boolVal = vmArgBoolGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj->unk64 & 0x8) {
        if (boolVal) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(0x10);
        } else {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc3(0x10);
        }
    }
    return 0;
}

// us-8003e2c0: func_8003DD44
// Gets a string arg and calls func_8013D07C on the object's sub-field
extern "C" int func_8003DD44(VMThread* pThread, int handle) {
    VMArg* arg1 = vmArgPtrGet(pThread, 1);
    const char* str = vmArgStringGet(2, arg1);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    func_8013D07C(*(void**)((u8*)obj + 0x74), str, 1);
    return 0;
}

// us-8003e328: func_8003DDAC
// Returns the current object handle (OC search result)
extern "C" int func_8003DDAC(VMThread* pThread, int handle) {
    VMArg retVal;
    retVal.type = 3;
    retVal.value.uintVal = func_8013EC58();
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003e370: func_8003DDF4
// Gets a string and calls a func, then handles notification flags
extern "C" int func_8003DDF4(VMThread* pThread, int handle) {
    VMArg* arg1 = vmArgPtrGet(pThread, 1);
    const char* str = vmArgStringGet(2, arg1);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    func_8013D448(*(void**)((u8*)obj + 0x74), str);
    if (obj->unk64 & 0x8) {
        if (!((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc10((void*)1, 1)) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(1);
        }
    } else if ((obj->unk64 & 0x4000) || (obj->unk64 & 0x8000)) {
        if (!((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc8(1)) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(1);
        }
    }
    return 0;
}

// us-8003e46c: setAct
// Sets an action on an object: action ID and optional fixed-point parameter
extern "C" int setAct(VMThread* pThread, int handle) {
    VMArg* arg1 = vmArgPtrGet(pThread, 1);
    int actionId = vmArgIntGet(2, arg1);
    int fixedParam;
    if (vmArgOmitChk(pThread, 2)) {
        fixedParam = 1;
    } else {
        VMArg* arg2 = vmArgPtrGet(pThread, 2);
        fixedParam = vmArgFixedGet(3, arg2);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    func_800BE12C(obj, actionId, 0, -1, 1);
    if (fixedParam != 1) {
        float f = (float)(s32)fixedParam / 2048.0f;
        obj->CfObject_UnkVirtualFunc14(f);
    }
    return 0;
}

// us-8003e560: func_8003DFE4
// Calls a virtual function with a fixed-point parameter converted to float
extern "C" int func_8003DFE4(VMThread* pThread, int handle) {
    VMArg* arg1 = vmArgPtrGet(pThread, 1);
    int fixedVal = vmArgFixedGet(2, arg1);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float f = (float)(s32)fixedVal / 2048.0f;
    obj->CfObject_UnkVirtualFunc14(f);
    return 0;
}

// us-8003e5ec: lookAt
// Makes an object look at a target, with optional snap/rotate flags
extern "C" int lookAt(VMThread* pThread, int handle) {
    int targetOC;
    if (vmArgOmitChk(pThread, 1)) {
        targetOC = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        targetOC = (int)vmArgOCGet(2, ptr);
    }
    int snap;
    if (vmArgOmitChk(pThread, 2)) {
        snap = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 2);
        snap = vmArgBoolGet(3, ptr);
    }
    int rotate;
    if (vmArgOmitChk(pThread, 3)) {
        rotate = 1;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 3);
        rotate = vmArgBoolGet(4, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* self = (cf::CfObject*)func_801864DC(ctx, handle);
    if (targetOC) {
        ctx = func_801862C0();
        cf::CfObject* target = (cf::CfObject*)func_801864DC(ctx, *(int*)((u8*)targetOC + 4));
        void* actor = (void*)__dynamic_cast(target, 0, (void*)&lbl_eu_806618D8, (void*)&lbl_eu_806618F0, 0);
        if (actor) {
            if (((cf::CfObject*)actor)->unk64 & 0x2 || ((cf::CfObject*)actor)->unk64 & 0x8) {
                if (*(void**)((u8*)self + 0xC4)) {
                    if (((int(*)(void*, const char*))(*(void***)actor)[0x120/4])(actor, &lbl_eu_804FA74C[0x3C])) {
                        if (snap) {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1B0/4])(self, actor, &lbl_eu_804FA74C[0x3C]);
                        } else {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1AC/4])(self, actor, &lbl_eu_804FA74C[0x3C]);
                        }
                    } else if (((int(*)(void*, const char*))(*(void***)actor)[0x120/4])(actor, &lbl_eu_804FA74C[0x43])) {
                        if (snap) {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1B0/4])(self, actor, &lbl_eu_804FA74C[0x43]);
                        } else {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1AC/4])(self, actor, &lbl_eu_804FA74C[0x43]);
                        }
                    } else if (((int(*)(void*, const char*))(*(void***)actor)[0x120/4])(actor, &lbl_eu_804FA74C[0x4B])) {
                        if (snap) {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1B0/4])(self, actor, &lbl_eu_804FA74C[0x4B]);
                        } else {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1AC/4])(self, actor, &lbl_eu_804FA74C[0x4B]);
                        }
                    }
                }
            } else {
                if (snap) {
                    ((void(*)(void*, void*, const char*))(*(void***)self)[0x1B0/4])(self, actor, &lbl_eu_804FA74C[0x4B]);
                } else {
                    ((void(*)(void*, void*, const char*))(*(void***)self)[0x1AC/4])(self, actor, &lbl_eu_804FA74C[0x4B]);
                }
            }
        } else {
            void* pos = (void*)(uintptr_t)((cf::CfObject*)target)->CfObject_UnkVirtualFunc23();
            ((void(*)(void*, void*))(*(void***)self)[0x1A8/4])(self, pos);
        }
        ((void(*)(void*, int))(*(void***)self)[0x1B4/4])(self, rotate ? 1 : 0);
    } else {
        ((void(*)(void*, void*, const char*))(*(void***)self)[0x1B0/4])(self, 0, &lbl_eu_804FA74C[0x3C]);
    }
    return 0;
}

// us-8003e94c: turn
// Turns an object by a fixed-point angle
extern "C" int turn(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int angle = vmArgIntGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float f = (float)(s32)angle / 2048.0f * 0.0000958738f;
    ((void(*)(void*, float))(*(void***)obj)[0xCC/4])(obj, f);
    func_800BE12C(obj, 3, 0, -1, 1);
    return 0;
}

// us-8003e9f4: func_8003E478
// Sets a flag on the object if it's alive
extern "C" int func_8003E478(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj && (obj->unk64 & 0x80000000)) {
        *(u32*)((u8*)obj + 0x6C) |= 0x00080000;
    }
    return 0;
}

// us-8003ea4c: func_8003E4D0
// Sets a flag on the object if it's alive
extern "C" int func_8003E4D0(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj && (obj->unk64 & 0x80000000)) {
        *(u32*)((u8*)obj + 0x6C) |= 0x00100000;
    }
    return 0;
}

// us-8003eaa4: func_8003E528
// Gets the current OC handle for the object's party member
extern "C" int func_8003E528(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    u32 ocHandle = obj->CObjectParam_UnkVirtualFunc5();
    if (!ocHandle) {
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
        if (obj == (cf::CfObject*)player) {
            ocHandle = func_800FE68C();
            ocHandle = *(u32*)((u8*)ocHandle + 0x10000 - 0x6F1C);
        }
    }
    if (ocHandle) {
        ctx = func_801862C0();
        cf::CfObject* target = (cf::CfObject*)func_801864DC(ctx, ocHandle);
        const char* typeName;
        if (target->unk64 & 0x01000000) {
            typeName = &lbl_eu_804FA74C[0x50];
        } else if (target->unk64 & 0x4000) {
            typeName = &lbl_eu_804FA74C[0x54];
        } else if (target->unk64 & 0x04000000) {
            typeName = &lbl_eu_804FA74C[0x5A];
        } else if (target->unk64 & 0x8000) {
            typeName = &lbl_eu_804FA74C[0x61];
        } else {
            typeName = &lbl_eu_804FA74C[0x66];
        }
        u8 type = 9;
        u16 ocId = vmOCSearch(typeName);
        VMArg retVal;
        retVal.type = type;
        retVal.unk2 = ocId;
        retVal.value.pointerVal = *(void**)((u8*)target + 0x74);
        vmRetValSet(pThread, &retVal);
        return 1;
    } else {
        vmOCExceptionThrow(pThread);
        return 0;
    }
}

// us-8003ebe8: func_8003E66C
// Gets the current battle target's OC handle
extern "C" int func_8003E66C(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    u32 ocHandle = obj->CObjectParam_UnkVirtualFunc5();
    if (ocHandle) {
        ctx = func_801862C0();
        obj = (cf::CfObject*)func_801864DC(ctx, ocHandle);
        u8 type = 3;
        u16 hp = *(u16*)((u8*)obj + 0x8C);
        VMArg retVal;
        retVal.type = type;
        retVal.value.uintVal = hp;
        vmRetValSet(pThread, &retVal);
    } else {
        VMArg retVal;
        retVal.type = 0;
        vmRetValSet(pThread, &retVal);
    }
    return 1;
}

// us-8003ec88: isPC
// Checks if the object is a player character
extern "C" int isPC(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    u32 isType = (obj->unk64 >> 1) & 1;
    VMArg retVal;
    retVal.type = isType ? 1 : 2;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003ecec: isNPC
// Checks if the object is an NPC
extern "C" int isNPC(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    u32 isType = (obj->unk64 >> 3) & 1;
    VMArg retVal;
    retVal.type = isType ? 1 : 2;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003ed50: isENE
// Checks if the object is an enemy
extern "C" int isENE(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    u32 isType = (obj->unk64 >> 2) & 1;
    VMArg retVal;
    retVal.type = isType ? 1 : 2;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003edb4: isPT
// Checks if the object is a party member
extern "C" int isPT(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    u8 retVal;
    if ((obj->unk64 & 0x4000) || (obj->unk64 & 0x8000)) {
        retVal = 1;
    } else {
        retVal = 2;
    }
    vmRetValSet(pThread, (VMArg*)&retVal);
    return 1;
}

// us-8003ee2c: invin
// Toggles invincibility on a CfObjectActor
extern "C" int invin(VMThread* pThread, int handle) {
    int enable;
    if (vmArgOmitChk(pThread, 1)) {
        enable = 1;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        enable = vmArgBoolGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* actor = (void*)__dynamic_cast(obj, 0, (void*)&__RTTI__Q22cf13CfObjectActor, (void*)&lbl_eu_806618F0, 0);
    if (actor) {
        if (enable) {
            func_800F38E0(getInstance__Q22cf14CBattleManagerFv(), actor, 1);
        } else {
            func_800F3958(getInstance__Q22cf14CBattleManagerFv(), actor, 1);
        }
    }
    return 0;
}

// us-8003eef0: func_8003E974
// Complex battle AI function - sets up a battle event with ID, type, etc.
extern "C" int func_8003E974(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int eventId = vmArgIntGet(2, ptr);
    int arg2;
    if (vmArgOmitChk(pThread, 2)) {
        arg2 = 0;
    } else {
        VMArg* ptr2 = vmArgPtrGet(pThread, 2);
        arg2 = vmArgIntGet(3, ptr2);
    }
    int arg3;
    if (vmArgOmitChk(pThread, 3)) {
        arg3 = 0;
    } else {
        VMArg* ptr3 = vmArgPtrGet(pThread, 3);
        arg3 = vmArgIntGet(4, ptr3);
    }
    int arg4;
    if (vmArgOmitChk(pThread, 4)) {
        arg4 = 0;
    } else {
        VMArg* ptr4 = vmArgPtrGet(pThread, 4);
        arg4 = vmArgIntGet(5, ptr4);
    }
    int arg5;
    if (vmArgOmitChk(pThread, 5)) {
        arg5 = 0;
    } else {
        VMArg* ptr5 = vmArgPtrGet(pThread, 5);
        arg5 = vmArgIntGet(6, ptr5);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* actor = (void*)__dynamic_cast(obj, 0, (void*)&lbl_eu_806618D8, (void*)&lbl_eu_806618F0, 0);
    if (actor) {
        u8 data[0x34];
        memset(data, 0, sizeof(data));
        float f1 = (float)(s32)arg2 / 2048.0f;
        float f2 = (float)(s32)arg5 / 2048.0f;
        *(u32*)&data[0] = *(u32*)((u8*)actor + 0x3F10);
        *(u32*)&data[4] = 0;
        *(u16*)&data[0x14] = (u16)eventId;
        *(u32*)&data[0x18] = arg3;
        *(u16*)&data[0x1C] = (u16)arg4;
        *(float*)&data[0x28] = f1;
        *(float*)&data[0x2C] = f2;
        void* battleMgr = getInstance__Q22cf14CBattleManagerFv();
        func_800EC8FC(battleMgr, actor, data, 0);
    }
    return 0;
}

// us-8003f0e0: func_8003EB64
// Removes buff/debuff from an actor by index
extern "C" int func_8003EB64(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int index = vmArgIntGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* actor = (void*)__dynamic_cast(obj, 0, (void*)&lbl_eu_806618D8, (void*)&lbl_eu_806618F0, 0);
    if (actor) {
        void* battleMgr = getInstance__Q22cf14CBattleManagerFv();
        func_800F3958(battleMgr, actor, index);
    }
    return 0;
}

// us-8003f168: setColi
// Enables/disables collision on an actor
extern "C" int setColi(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* actor = (void*)__dynamic_cast(obj, 0, (void*)&lbl_eu_806618D8, (void*)&lbl_eu_806618F0, 0);
    if (actor) {
        func_800BE824(actor, enable ? 1 : 0);
    }
    return 0;
}

// us-8003f1ec: setEye
// Sets the eye/attention target
extern "C" int setEye(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int target = vmArgIntGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj) {
        func_800BE0F8(obj, target);
    }
    return 0;
}

// us-8003f254: func_8003ECD8
// Enables/disables gravity on an object
extern "C" int func_8003ECD8(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj) {
        obj->CfObject_UnkVirtualFunc64(enable ? 0 : 1);
    }
    return 0;
}

// us-8003f2cc: func_8003ED50
// Enables/disables a visibility flag on an object and its model
extern "C" int func_8003ED50(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj) {
        if (enable) {
            obj->mFlags68 |= 0x8000;
        } else {
            obj->mFlags68 &= ~0x8000;
        }
        void* model = *(void**)((u8*)obj + 0x98);
        if (model) {
            if (enable) {
                *(u32*)((u8*)model + 0x7A8) |= 0x1000;
            } else {
                *(u32*)((u8*)model + 0x7A8) &= ~0x1000;
            }
        }
    }
    return 0;
}

// us-8003f380: func_8003EE04
// Enables/disables a flag on the object
extern "C" int func_8003EE04(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj) {
        func_800BE33C(obj, enable ? 1 : 0);
    }
    return 0;
}

// us-8003f3f0: func_8003EE74
// Calls a game-level function with a bool parameter
extern "C" int func_8003EE74(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0();
    func_801864DC(ctx, handle);
    void* gameObj = func_800BBC0C();
    if (gameObj) {
        func_800BF314(gameObj, enable);
    }
    return 0;
}

// us-8003f45c: func_8003EEE0
// Battle party selection/generation
extern "C" int func_8003EEE0(VMThread* pThread, int handle, int r5) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int partyId = vmArgIntGet(2, ptr);
    int arg2;
    if (vmArgOmitChk(pThread, 2)) {
        arg2 = 0;
    } else {
        VMArg* ptr2 = vmArgPtrGet(pThread, 2);
        arg2 = vmArgIntGet(3, ptr2);
    }
    int arg3;
    if (vmArgOmitChk(pThread, 3)) {
        arg3 = 0;
    } else {
        VMArg* ptr3 = vmArgPtrGet(pThread, 3);
        arg3 = vmArgIntGet(4, ptr3);
    }
    u32 flag = lbl_eu_80663E24;
    int hadFlag = (flag >> 13) & 1;
    u32 newFlag = flag & ~0x00020000;
    if (arg3) {
        newFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = newFlag;
    void* result = func_80081694__Q22cf13CfGameManagerFv((u16)partyId, (u16)arg2);
    newFlag = lbl_eu_80663E24;
    u32 restoredFlag = newFlag & ~0x00020000;
    if (hadFlag) {
        restoredFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = restoredFlag;
    void* ctx;
    if (!result) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    ctx = func_801862C0();
    func_801863F4(ctx);
    *(u32*)((u8*)result + 0x68) |= 0x40000000;
    u8 type = 9;
    VMArg retVal;
    retVal.type = type;
    retVal.unk2 = (u16)r5;
    retVal.value.pointerVal = ctx;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003f5b0: setRot
// Sets the rotation of an object from 3 fixed-point angles
extern "C" int setRot(VMThread* pThread, int handle) {
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    int rotX = vmArgFixedGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int rotY = vmArgFixedGet(3, ptr2);
    VMArg* ptr3 = vmArgPtrGet(pThread, 3);
    int rotZ = vmArgFixedGet(4, ptr3);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float fx = (float)(s32)rotX / 4096.0f * 0.017453292519943295f;
    float fy = (float)(s32)rotY / 4096.0f * 0.017453292519943295f;
    float fz = (float)(s32)rotZ / 4096.0f * 0.017453292519943295f;
    float rot[3] = { fx, fy, fz };
    obj->CfObject_UnkVirtualFunc27(rot);
    return 0;
}

// us-8003f6f4: gravity
// Toggles gravity on an object
extern "C" int gravity(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    obj->CfObject_UnkVirtualFunc64(enable ? 0 : 1);
    return 0;
}

// us-8003f78c: func_8003F210
// Battle party lookup by name, returns OC handle
extern "C" int func_8003F210(VMThread* pThread, int handle, int r5) {
    void* ctx;
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    const char* name = vmArgStringGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int param = vmArgIntGet(3, ptr2);
    int arg3;
    if (vmArgOmitChk(pThread, 3)) {
        arg3 = 0;
    } else {
        VMArg* ptr3 = vmArgPtrGet(pThread, 3);
        arg3 = vmArgIntGet(4, ptr3);
    }
    u32 flag = lbl_eu_80663E24;
    int hadFlag = (flag >> 13) & 1;
    u32 newFlag = flag & ~0x00020000;
    if (arg3) {
        newFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = newFlag;
    void* result = 0;
    if (strcmp(name, &lbl_eu_804FA74C[0x6B]) == 0 || strcmp(name, &lbl_eu_804FA74C[0x54]) == 0) {
        result = func_80081CBC__Q22cf13CfGameManagerFv(&lbl_eu_804FA74C[0x6B], (u16)param);
    } else if (strcmp(name, &lbl_eu_804FA74C[0x79]) == 0) {
        result = func_80081CB8__Q22cf13CfGameManagerFv();
    }
    if (!result) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    newFlag = lbl_eu_80663E24;
    u32 restoredFlag = newFlag & ~0x00020000;
    if (hadFlag) {
        restoredFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = restoredFlag;
    ctx = func_801862C0();
    func_801863F4(ctx);
    *(u32*)((u8*)result + 0x68) |= 0x40000000;
    u8 type = 9;
    VMArg retVal;
    retVal.type = type;
    retVal.unk2 = (u16)r5;
    retVal.value.pointerVal = ctx;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003f914: func_8003F398
// Sets a virtual function on the object (enable)
extern "C" int func_8003F398(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    obj->CfObject_UnkVirtualFunc66(1);
    return 0;
}

// us-8003f994: func_8003F418
// Sets a virtual function on the object (disable)
extern "C" int func_8003F418(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    obj->CfObject_UnkVirtualFunc66(0);
    return 0;
}

// us-8003fa14: func_8003F498
// Battle party member lookup by name/type
extern "C" int func_8003F498(VMThread* pThread, int handle, int r5) {
    void* ctx;
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    const char* name = vmArgStringGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int index = vmArgIntGet(3, ptr2);
    int arg3;
    if (vmArgOmitChk(pThread, 3)) {
        arg3 = 0;
    } else {
        VMArg* ptr3 = vmArgPtrGet(pThread, 3);
        arg3 = vmArgIntGet(4, ptr3);
    }
    u32 flag = lbl_eu_80663E24;
    int hadFlag = (flag >> 13) & 1;
    u32 newFlag = flag & ~0x00020000;
    if (arg3) {
        newFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = newFlag;
    void* result = 0;
    if (strcmp(name, &lbl_eu_804FA74C[0x85]) == 0 || strcmp(name, &lbl_eu_804FA74C[0x89]) == 0) {
        result = func_800817BC__Q22cf13CfGameManagerFv(index, 0);
    } else if (strcmp(name, &lbl_eu_804FA74C[0x8C]) == 0 || strcmp(name, &lbl_eu_804FA74C[0x93]) == 0) {
        result = func_8008187C__Q22cf13CfGameManagerFv(index);
    }
    if (!result) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    cf::CfGameManager::getInstance();
    if (lbl_eu_80663E24 & 0x00040000) {
        *(u32*)((u8*)result + 0x64) |= 0x00010000;
    }
    newFlag = lbl_eu_80663E24;
    u32 restoredFlag = newFlag & ~0x00020000;
    if (hadFlag) {
        restoredFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = restoredFlag;
    ctx = func_801862C0();
    func_801863F4(ctx);
    *(u32*)((u8*)result + 0x68) |= 0x40000000;
    u8 type = 9;
    VMArg retVal;
    retVal.type = type;
    retVal.unk2 = (u16)r5;
    retVal.value.pointerVal = ctx;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003fbc8: func_8003F64C
// Enables a virtual function on the object (same as func_8003F398)
extern "C" int func_8003F64C(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    obj->CfObject_UnkVirtualFunc66(1);
    return 0;
}

// us-8003fc48: func_8003F6CC
// Disables a virtual function on the object (same as func_8003F418)
extern "C" int func_8003F6CC(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    obj->CfObject_UnkVirtualFunc66(0);
    return 0;
}

// us-8003fcc8: func_8003F74C
// Sets a property on an object with a signed byte value
extern "C" int func_8003F74C(VMThread* pThread, int handle) {
    int value;
    if (vmArgOmitChk(pThread, 1)) {
        value = 1;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        value = vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    func_800ACC14(obj, (s8)value);
    return 0;
}

// us-8003fd48: func_8003F7CC
// Sets a follow/attention relationship between two objects
extern "C" int func_8003F7CC(VMThread* pThread, int handle) {
    int targetOC;
    if (vmArgOmitChk(pThread, 1)) {
        targetOC = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        targetOC = (int)vmArgOCGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* target = 0;
    if (targetOC) {
        ctx = func_801862C0();
        target = func_801864DC(ctx, *(int*)((u8*)targetOC + 4));
    }
    func_800ACF78(obj, target, 0);
    return 0;
}

// us-8003fdec: func_8003F870
// Sets a follow relationship with parent/child lookup
extern "C" int func_8003F870(VMThread* pThread, int handle) {
    int targetOC;
    if (vmArgOmitChk(pThread, 1)) {
        targetOC = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        targetOC = (int)vmArgOCGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* parent = 0;
    void* child = 0;
    if (targetOC) {
        ctx = func_801862C0();
        parent = func_801864DC(ctx, *(int*)((u8*)targetOC + 4));
        if (parent) {
            u32* fieldC8 = (u32*)((u8*)parent + 0xC8);
            void* followTarget = 0;
            if (*fieldC8) {
                followTarget = *(void**)((u8*)parent + 0x6F8);
            } else {
                u32* fieldCC = (u32*)((u8*)parent + 0xCC);
                if (*fieldCC) {
                    followTarget = *(void**)((u8*)parent + 0x6FC);
                }
            }
            if (followTarget) {
                ((cf::CfObject*)followTarget)->CfObject_UnkVirtualFunc22();
                child = followTarget;
            }
        }
    }
    func_800ACF78(obj, parent, child);
    return 0;
}

// us-8003fef8: func_8003F97C
// Unfollow/clear a relationship
extern "C" int func_8003F97C(VMThread* pThread, int handle) {
    int targetOC;
    if (vmArgOmitChk(pThread, 1)) {
        targetOC = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        targetOC = (int)vmArgOCGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* target = 0;
    if (targetOC) {
        ctx = func_801862C0();
        target = func_801864DC(ctx, *(int*)((u8*)targetOC + 4));
    }
    func_800ACFD8(obj, target);
    return 0;
}

// us-8003ff98: func_8003FA1C
// Sets up a battle encounter by name
extern "C" int func_8003FA1C(VMThread* pThread, int handle, int r5) {
    void* ctx;
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    const char* name = vmArgStringGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int param = vmArgIntGet(3, ptr2);
    void* battleMgr = func_80081CB8__Q22cf13CfGameManagerFv();
    if (battleMgr) {
        *(u32*)((u8*)battleMgr + 0x94) = 5;
        func_800AC4A8(battleMgr, (u16)param);
        ((cf::CfObject*)battleMgr)->CfObject_UnkVirtualFunc66(0);
    }
    if (!battleMgr) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    ctx = func_801862C0();
    func_801863F4(ctx);
    *(u32*)((u8*)battleMgr + 0x68) |= 0x40000000;
    u8 type = 9;
    VMArg retVal;
    retVal.type = type;
    retVal.unk2 = (u16)r5;
    retVal.value.pointerVal = ctx;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-80040094: func_8003FB18
// Enables a virtual function on the object (same as func_8003F398)
extern "C" int func_8003FB18(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    obj->CfObject_UnkVirtualFunc66(1);
    return 0;
}

// us-80040114: func_8003FB98
// Disables a virtual function on the object (same as func_8003F418)
extern "C" int func_8003FB98(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    obj->CfObject_UnkVirtualFunc66(0);
    return 0;
}

// us-80040194: func_8003FC18
// Moves an object by a fixed-point offset in its local X direction
extern "C" int func_8003FC18(VMThread* pThread, int handle) {
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    int distX = vmArgFixedGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int distY = vmArgFixedGet(3, ptr2);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float fDistX = (float)(s32)distX / 2048.0f;
    float fDistY = (float)(s32)distY / 2048.0f;
    if (obj) {
        void* pos = (void*)(uintptr_t)obj->CfObject_UnkVirtualFunc23();
        float x = *(float*)((u8*)pos + 0);
        float y = *(float*)((u8*)pos + 4);
        float z = *(float*)((u8*)pos + 8);
        float offset[3] = { x, y + fDistY, z };
        func_800ABF24(obj, pos, offset, fDistX);
    }
    return 0;
}

// us-800402c4: func_8003FD48
// Moves an object to a position using 3 fixed-point coordinates
extern "C" int func_8003FD48(VMThread* pThread, int handle) {
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    int x = vmArgFixedGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int y = vmArgFixedGet(3, ptr2);
    VMArg* ptr3 = vmArgPtrGet(pThread, 3);
    int z = vmArgFixedGet(4, ptr3);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float fx = (float)(s32)x / 2048.0f;
    float fy = (float)(s32)y / 2048.0f;
    float fz = (float)(s32)z / 2048.0f;
    float pos[3] = { fx, fy, fz };
    ((void(*)(void*, void*))(*(void***)obj)[0xBC/4])(obj, pos);
    return 0;
}

// us-80040458: func_8003FEDC
// Rotation function with 3 fixed-point angles
extern "C" int func_8003FEDC(VMThread* pThread, int handle) {
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    int rotX = vmArgFixedGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int rotY = vmArgFixedGet(3, ptr2);
    VMArg* ptr3 = vmArgPtrGet(pThread, 3);
    int rotZ = vmArgFixedGet(4, ptr3);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float fx = (float)(s32)rotX / 2048.0f * 0.0000958738f;
    float fy = (float)(s32)rotY / 2048.0f * 0.0000958738f;
    float fz = (float)(s32)rotZ / 2048.0f * 0.0000958738f;
    float rot[3] = { fx, fy, fz };
    ((void(*)(void*, void*))(*(void***)obj)[0xBC/4])(obj, rot);
    return 0;
}

// us-80040570: func_8003FFF4
// Sets a position on an object using 3 fixed-point coords
extern "C" int func_8003FFF4(VMThread* pThread, int handle) {
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    int x = vmArgFixedGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int y = vmArgFixedGet(3, ptr2);
    VMArg* ptr3 = vmArgPtrGet(pThread, 3);
    int z = vmArgFixedGet(4, ptr3);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float fx = (float)(s32)x / 2048.0f;
    float fy = (float)(s32)y / 2048.0f;
    float fz = (float)(s32)z / 2048.0f;
    float pos[3] = { fx, fy, fz };
    ((void(*)(void*, void*))(*(void***)obj)[0xBC/4])(obj, pos);
    return 0;
}

// us-80040664: ocUnitRegist
// Registers all OC unit plugin functions
extern "C" void ocUnitRegist() {}

void CObjectState_UnkVirtualFunc1__Q22cf12CObjectStateFv(void* self, unsigned long bits) {
    *(unsigned long*)((char*)self + 4) |= bits;
}

void CObjectParam_UnkVirtualFunc6__Q22cf12CObjectParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x34) = val; }

void CObjectState_UnkVirtualFunc3__Q22cf12CObjectStateFv(void* self, unsigned long mask) {
    *(unsigned long*)((char*)self + 4) &= ~mask;
}

void CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv(void* self, u8 val) { static_cast<cf::CfObjectModel*>(self)->field_BD = val; }

u32 CObjectParam_UnkVirtualFunc5__Q22cf12CObjectParamFv(void* self) { return *(u32*)((u8*)self + 0x34); }

void cf::CfObject::CfObject_UnkVirtualFunc27(void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)this + 0x48) = a;
    *(u32*)((u8*)this + 0x4C) = b;
    *(u32*)((u8*)this + 0x50) = c;
}

void cf::CfObject::CfObject_UnkVirtualFunc64(int flag) {
    u32* field = (u32*)((char*)this + 0x68);
    if (flag) {
        *field |= 0x01000000;
    } else {
        *field &= ~0x01000000;
    }
}

extern float lbl_eu_8066A20C;
extern float lbl_eu_80665C30;
extern "C" float CfObject_UnkVirtualFunc34__Q22cf8CfObjectFv(void* self) { return *(float*)((u8*)self + 0x4c) * lbl_eu_8066A20C; }

extern "C" void CfObject_UnkVirtualFunc30__Q22cf8CfObjectFv(void* self) { reinterpret_cast<CfObjIf*>(self)->vf00C4(); }
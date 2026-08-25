// Auto-scaffolded catalog TU for kyoshin/plugin/ocUnit
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/plugin/ocUnit.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/device/CDeviceVI.hpp"

// .sdata2 pool plant: MWCC's implicit int->double conversion magic
// (2^52 + 2^31). With this planted under its retail name AND the other
// sdata2 scales referenced through named externs, the literal pool unifies
// every conversion lfd/lfs reloc onto the retail symbols
// (MWCC_CASES "naming ONE pooled scale constant unifies the WHOLE pool").

struct CfObjIf {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual int _v0024(u32 value);
    virtual int _v0028();
    virtual int _v002C(u32 mask, int flag);
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C(u32 arg);
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
    virtual void _v00A8(void* pos);
    virtual Vec3f* _v00AC();
    virtual void _v00B0();
    virtual void _v00B4();
    virtual void _v00B8();
    virtual void _v00BC();
    virtual void _v00C0();
    virtual void vf00C4();
    // CfObjectMove tail virtuals (slots 0xC8..0x1F0) for dispatch from this TU.
    virtual void _v00C8();
    virtual void _v00CC();
    virtual void _v00D0();
    virtual void _v00D4();
    virtual void _v00D8();
    virtual void _v00DC();
    virtual void _v00E0();
    virtual void _v00E4();
    virtual void _v00E8();
    virtual void _v00EC();
    virtual void _v00F0();
    virtual void _v00F4();
    virtual void _v00F8();
    virtual void _v00FC();
    virtual void _v0100();
    virtual void _v0104();
    virtual void _v0108();
    virtual void _v010C();
    virtual void _v0110();
    virtual void _v0114(u32 value);
    virtual void _v0118();
    virtual void _v011C();
    virtual int _v0120(const char* name);
    virtual void _v0124();
    virtual float _v0128();
    virtual void _v012C();
    virtual void _v0130();
    virtual void _v0134();
    virtual void _v0138();
    virtual void _v013C();
    virtual void _v0140();
    virtual void _v0144();
    virtual void _v0148();
    virtual void _v014C();
    virtual void _v0150();
    virtual void _v0154();
    virtual void _v0158();
    virtual void _v015C();
    virtual void _v0160();
    virtual void _v0164();
    virtual void _v0168();
    virtual void _v016C();
    virtual void _v0170();
    virtual void _v0174();
    virtual void _v0178();
    virtual void _v017C();
    virtual void _v0180();
    virtual void _v0184();
    virtual void _v0188();
    virtual void _v018C();
    virtual void _v0190();
    virtual void _v0194();
    virtual void _v0198();
    virtual void _v019C();
    virtual void _v01A0();
    virtual void _v01A4();
    virtual void _v01A8(Vec3f* pos);
    virtual void _v01AC(cf::CfObject* target, const char* name);
    virtual void _v01B0(cf::CfObject* target, const char* name);
    virtual void _v01B4(int flag);
    virtual void _v01B8();
    virtual void _v01BC();
    virtual void _v01C0();
    virtual void _v01C4();
    virtual void _v01C8();
    virtual void _v01CC();
    virtual void _v01D0();
    virtual void _v01D4();
    virtual void _v01D8();
    virtual void _v01DC();
    virtual void _v01E0(int a, int b, int n, Vec3f* pts, double angle);
    virtual void _v01E4();
    virtual int vf01E8();          // 0x1E8: busy/idle check
    virtual void _v01EC(float rate); // 0x1EC: walk-rate setter
    virtual void vf01F0(int arg); // 0x1F0: time-unit setter
    virtual void _v01F4(void* src);
    virtual void _v01F8();
    virtual void _v01FC(int a, int b, int c); // 0x1FC
};

// C-linkage retail symbols referenced by the plugin functions below.
extern "C" {
    extern u32 lbl_eu_80663E24;  // non-volatile extern (see CfObjectMove.hpp); reads needing separate loads cast explicitly
    extern u32 lbl_eu_80663E28;
    extern u16 lbl_eu_80663E42;
    extern u16 lbl_eu_80663E44;
    extern float lbl_eu_80665C40;
    void func_800BDB4C(void* obj);
    void func_80085878__Q22cf13CfGameManagerFv();
    extern char lbl_eu_804FA74C[];
    extern void* lbl_eu_806618D8;
    extern void* lbl_eu_806618F0;
    extern void* lbl_eu_806618E8;
    extern "C" void* __RTTI__Q22cf13CfObjectActor;
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
    int code80135FDC_getByte_64058();
    void* __dynamic_cast(void* obj, int offset, void* rtti, void* targetRtti, int flag);
    int strcmp(const char* s1, const char* s2);
    void* memset(void* ptr, int val, u32 size);
    void* func_80081CBC__Q22cf13CfGameManagerFv(const char* name, unsigned short param);
    void* func_80081CB8__Q22cf13CfGameManagerFv();
    void* func_800817BC__Q22cf13CfGameManagerFv(int index, int mode);
    void* func_8008187C__Q22cf13CfGameManagerFv(int index);
}

// Checks whether the current OC context object can start a battle/talk
// interaction. Returns 1 to block, 0 to allow.
extern "C" int func_8003BC10(void* obj) {
    // Declaration order controls MWCC's r28/r29 naming (cur -> r28).
    s32 result = 1;
    int bit;
    int talkable;
    void* cur = func_800BBC0C();
    if (cur == NULL) {
        goto done;
    }
    if (lbl_eu_80663E28 & 0x04000000) {
        goto done;
    }
    // Booleanize the state byte's low bit (MWCC clrlwi/neg/or/srwi idiom).
    bit = *(u8*)((u8*)cur + 0x6C9) & 1;
    talkable = (u32)(-bit | bit) >> 31;
    func_800BF314(cur, 0);
    if (lbl_eu_80663E24 & 0x00400000) {
        goto done;
    }
    if (!(*(u32*)((u8*)cur + 0x64) & 8)) {
        goto done;
    }
    cf::CfObject* ca = (cf::CfObject*)cur;
    s32 v1 = ca->CfObject_UnkVirtualFunc50();
    s32 v2 = ca->CfObject_UnkVirtualFunc51();
    s32 ok = 0;
    if (lbl_eu_80663E42 == 4 && lbl_eu_80663E44 == 1) {
        ok = 1;
    }
    if (ok == 0) {
        talkable = 0;
    }
    // Two separate compares in retail (not range-folded).
    s32 is12 = (v2 == 1 || v2 == 2) ? 1 : 0;
    if (is12 != 0) {
        // Busy states 4/5 always block; state 3 blocks only when talkable.
        if (v1 == 4 || v1 == 5 || (talkable != 0 && v1 == 3)) {
            return 1;
        }
    }
    if (is12 == 0 && !(v2 == 3 && (u32)(v1 - 1) <= 1)) {
        goto done;
    }
    result = 0;
done:
    return result;
}

int cf::CfObjectMove::CfObject_UnkVirtualFunc50() { return field_6CE; }

int cf::CfObjectMove::CfObject_UnkVirtualFunc51() { return field_6CF; }

// us-8003c2f8: spawns/looks up an object by bdat name and returns it as an
// OC-reference retval. Third parameter is stored raw into the retval.
extern "C" int func_8003BD7C(VMThread* pThread, int handle, u16 unk) {
    const char* str = vmArgStringGet(2, vmArgPtrGet(pThread, 1));
    int argIdx;
    int idx;
    if (vmArgOmitChk(pThread, 2)) {
        idx = 0;
        argIdx = 3;
    } else {
        argIdx = 3;
        idx = vmArgIntGet(argIdx, vmArgPtrGet(pThread, 2));
    }
    int extra;
    if (vmArgOmitChk(pThread, argIdx)) {
        extra = 0;
    } else {
        VMArg* ptr3 = vmArgPtrGet(pThread, argIdx);
        ++argIdx;
        extra = vmArgIntGet(argIdx, ptr3);
    }
    // Save the talk-lock bit (volatile read keeps this load separate from
    // the reloads in the ternaries below, matching retail), then lift bit
    // 0x2000 into 0x40000 depending on the omitted-arg value.
    u32 talkBit = (*(volatile u32*)&lbl_eu_80663E24 >> 13) & 1;
    u32 flags = *(volatile u32*)&lbl_eu_80663E24;
    lbl_eu_80663E24 = (extra != 0) ? (flags | 0x40000u) : (flags & ~0x2000u);
    OcSpawnObjView* found = NULL;
    if (strcmp(str, lbl_eu_804FA74C) == 0 || strcmp(str, lbl_eu_804FA74C + 0xC) == 0) {
        found = (OcSpawnObjView*)func_80081990__Q22cf13CfGameManagerFv(lbl_eu_804FA74C, (u16)idx);
        if (found != NULL) {
            func_8003AA34();
            getFP__FPCc(lbl_eu_804FA74C); // init side effect only; result unused
            u32 col = getBdatStringColumnValue((void*)lbl_eu_804FA74C, lbl_eu_804FA74C + 0x10, idx);
            if (*(u8*)&col == 0) {
                found->field_0x64 |= 0x1000;
            }
        }
    } else if (strcmp(str, lbl_eu_804FA74C + 0x17) == 0 || strcmp(str, lbl_eu_804FA74C + 0x23) == 0) {
        found = (OcSpawnObjView*)func_80081A40__Q22cf13CfGameManagerFv(lbl_eu_804FA74C + 0x17, (u16)idx, 0, 0);
    } else if (strcmp(str, lbl_eu_804FA74C + 0x27) == 0 || strcmp(str, lbl_eu_804FA74C + 0x32) == 0) {
        found = (OcSpawnObjView*)func_80081358__Q22cf13CfGameManagerFv(lbl_eu_804FA74C + 0x27, (u16)idx, 1, 0);
    } else if (strcmp(str, lbl_eu_804FA74C + 0x35) == 0) {
        if (idx > 0) {
            idx--;
        }
        found = (OcSpawnObjView*)cf::CfGameManager::getPlayer(idx);
    }

    // Same bit lift as above, keyed on the saved talk bit this time.
    u32 flags2 = *(volatile u32*)&lbl_eu_80663E24;
    lbl_eu_80663E24 = (talkBit != 0) ? (flags2 | 0x40000u) : (flags2 & ~0x2000u);
    if (found == NULL) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    void* oc;
    if (strcmp(str, lbl_eu_804FA74C + 0x35) == 0) {
        oc = func_80186460(func_801862C0(), found);
    } else {
        oc = func_801863F4(func_801862C0(), found);
    }
    found->field_0x68 |= 0x4000;
    VMArg retVal;
    retVal.type = 9;
    retVal.unk2 = unk;
    retVal.value.pointerVal = oc;
    vmRetValSet(pThread, &retVal);
    return 1;
}

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

int func_8003C3D0(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    // Cast to CfObjectActor; if it is one, read the float at vtable slot
    // 0x128 and return it truncated to int, else return 0.
    void* actor = __dynamic_cast(obj, 0, (void*)&__RTTI__Q22cf13CfObjectActor, (void*)&lbl_eu_806618F0, 0);
    VMArg retVal;
    if (actor) {
        retVal.type = 3;
        retVal.value.intVal = (int)((CfObjIf*)actor)->_v0128();
    } else {
        retVal.type = 3;
        retVal.value.intVal = 0;
    }
    vmRetValSet(pThread, &retVal);
    return 1;
}

float cf::CActorParam::CActorParam_UnkVirtualFunc37() { return *(float*)((u8*)this + 0x17E8); }

// us-8003c9fc: read the OC property as a signed value scaled by
// 1/lbl_eu_80665C30, overwrite the object position vector's X with it and
// pass the vector to vtable slot 0xA8.
extern "C" int func_8003C480(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    CfObjIf* obj = (CfObjIf*)func_801864DC(ctx, handle);
    VMArg* prop = (VMArg*)vmOCPropertyGet(pThread);
    Vec3f vec;
    Vec3f* p = obj->_v00AC();
    vec.x = p->x;
    vec.y = p->y;
    vec.z = p->z;
    // Signed OC property -> float via the 0x43300000 double-conversion trick
    // (fsubs, no frsp); magic subtrahend is the retail lbl_eu_80665C38.
    vec.x = (s32)prop->value.uintVal / lbl_eu_80665C30;
    obj->_v00A8(&vec);
    return 0;
}

// copy 3 u32 from src to self+0x3C (retail lwz x3; stw x3); const src avoids interleave
extern "C" void CfObject_UnkVirtualFunc22__Q22cf8CfObjectFv(void* self, const void* src) {
    ((u32*)((u8*)self + 0x3C))[0] = *(const u32*)((const u8*)src + 0);
    ((u32*)((u8*)self + 0x3C))[1] = *(const u32*)((const u8*)src + 4);
    ((u32*)((u8*)self + 0x3C))[2] = *(const u32*)((const u8*)src + 8);
}

// us-8003cadc: same shape as func_8003C480 but writes the computed angle to
// the vector's Y component.
extern "C" int func_8003C560(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    CfObjIf* obj = (CfObjIf*)func_801864DC(ctx, handle);
    VMArg* prop = (VMArg*)vmOCPropertyGet(pThread);
    Vec3f vec;
    Vec3f* p = obj->_v00AC();
    vec.x = p->x;
    vec.y = p->y;
    vec.z = p->z;
    // Same conversion as func_8003C480, written to the vector's Y.
    vec.y = (s32)prop->value.uintVal / lbl_eu_80665C30;
    obj->_v00A8(&vec);
    return 0;
}

// us-8003cba0: same shape as func_8003C480/func_8003C560 but writes the
// computed angle to the vector's Z component.
extern "C" int func_8003C624(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    CfObjIf* obj = (CfObjIf*)func_801864DC(ctx, handle);
    VMArg* prop = (VMArg*)vmOCPropertyGet(pThread);
    Vec3f vec;
    Vec3f* p = obj->_v00AC();
    vec.x = p->x;
    vec.y = p->y;
    vec.z = p->z;
    // Signed OC property -> float via the 0x43300000 double-conversion trick
    // (fsubs, no frsp); magic subtrahend is the retail lbl_eu_80665C38.
    vec.z = (s32)prop->value.uintVal / lbl_eu_80665C30;
    obj->_v00A8(&vec);
    return 0;
}

int func_8003C6E8(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    // OC property value (u32) -> float via MWCC's direct 2^52 conversion
    // (fsubs, no frsp), scaled by the degrees-to-radians constant, then
    // dispatched to vtable slot 0xC8.
    VMArg* prop = (VMArg*)vmOCPropertyGet(pThread);
    float f = (float)(s32)prop->value.uintVal * lbl_eu_8066A210;
    (*(CfObjVt_C8**)obj)->fn(obj, f);
    return 0;
}

void cf::CfObject::CfObject_UnkVirtualFunc30() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[0xC4 / 4];
    func(this);
}

void cf::CfObject::CfObject_UnkVirtualFunc29(float value) { mField4C = value; }

int func_8003C78C(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* prop = vmOCPropertyGet(pThread);
    // OC property setter: dispatch vtable[0x3C] with the property getter value.
    ((CfObjIf*)obj)->_v003C(*(u32*)((u8*)prop + 4));
    return 0;
}

extern "C" void CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv(void* self, const char* str) {
    *(u32*)((u8*)self + 0x30) = strlen(str);
    strcpy((char*)((u8*)self + 0x10), str);
}

bool isValid() { return false; }

extern "C" int func_8003C84C(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float* p = (float*)obj->CfObject_UnkVirtualFunc23();
    struct {
        float x;
        float y;
        float z;
    } vec;
    vec.x = p[0];
    vec.y = p[1];
    vec.z = p[2];
    obj->CfObject_UnkVirtualFunc26((u32)&vec, lbl_eu_80665C40);
    if (!(lbl_eu_80663E24 & 0xAFA40000) && (*(u32*)((u8*)obj + 0x64) & 8)) {
        func_800BDB4C(obj);
    }
    if (obj != 0 && obj == (cf::CfObject*)cf::CfGameManager::getPlayer(0) &&
        (lbl_eu_80663E24 & 0x00400000)) {
        func_80085878__Q22cf13CfGameManagerFv();
    }
    return 0;
}

extern "C" void CfObject_UnkVirtualFunc26__Q22cf8CfObjectFv(cf::CfObject* self) {
    // Dispatch the "action start" hook, then mark the object as interacted.
    self->CfObject_UnkVirtualFunc19();
    self->mFlags68 |= 0x100;
}

void CfObject_UnkVirtualFunc19__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

int dispOn(VMThread* pThread, int handle) {
    int flag;
    if (vmArgOmitChk(pThread, 1)) {
        flag = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        flag = vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    // flag == 0: show via vtable slot 0x158; otherwise route through the
    // external display helper.
    if (flag == 0) {
        obj->CfObject_UnkVirtualFunc66(1);
    } else {
        func_800BC3F0(obj);
    }
    return 0;
}

extern void func_800BC458(void* obj);

int dispOff(VMThread* pThread, int handle) {
    int flag;
    if (vmArgOmitChk(pThread, 1)) {
        flag = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        flag = vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    // If the current display object still accepts display-off, bail out
    // before hiding this object.
    cf::CfObject* disp = (cf::CfObject*)func_800BF324(obj);
    if (disp != 0 && !(disp->unk64 & 0x10000)) {
        if (((cf::CObjectState*)disp)->CObjectState_UnkVirtualFunc8(1) != 0 ||
            ((cf::CObjectState*)disp)->CObjectState_UnkVirtualFunc2(1) != 0) {
            return 0;
        }
    }
    // flag == 0: hide via vtable slot 0x158; otherwise route through the
    // external display helper.
    if (flag == 0) {
        obj->CfObject_UnkVirtualFunc66(0);
    } else {
        func_800BC458(obj);
    }
    return 0;
}

// CObjectState_UnkVirtualFunc8 defined below as extern "C" free function

int CObjectState_UnkVirtualFunc2__Q22cf12CObjectStateFv(void* self, int mask) {
    return (*(int*)((char*)self + 4) & mask) != 0 ? 1 : 0;
}

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

int func_8003CC0C(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int b = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    // Cast the OC object to a CfObjectActor and forward the booleanized
    // flag to vtable slot 0x1C0.
    void* actor = __dynamic_cast(obj, 0, (void*)&lbl_eu_806618D8, (void*)&lbl_eu_806618F0, 0);
    if (actor) {
        (*(CfObjVt_1C0**)actor)->fn((cf::CfObject*)actor, (u32)(-b | b) >> 31);
    }
    return 0;
}

int func_8003CC9C(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int index = vmArgIntGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    // Copy the 6-entry event-arg table into a local buffer, then dispatch
    // the selected entry through vtable slot 0x114.
    u32 args[6];
    const u32* src = lbl_eu_804FA4C0;
    args[0] = *src++;
    args[1] = *src++;
    args[2] = *src++;
    args[3] = *src++;
    args[4] = *src++;
    args[5] = *src++;
    if ((u32)index >= 6) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    ((CfObjIf*)obj)->_v0114(args[index]);
    return 0;
}

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

int walkR(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int fixed = vmArgFixedGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    // Fixed-point walk rate -> float via MWCC's s32 double-trick, then
    // dispatched to vtable slot 0x1EC.
    float f = (float)(s32)fixed / lbl_eu_80665C30;
    ((CfObjIf*)obj)->_v01EC(f);
    return 0;
}

int func_8003CED0(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int arg = vmArgIntGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    // Convert the frame count to time units using the target framerate.
    ((CfObjIf*)obj)->vf01F0(arg * CDeviceVI::getTargetFramerate());
    return 0;
}

extern "C" int func_8003CF48(VMThread* pThread, int handle) {
    VMArg* a1 = vmArgPtrGet(pThread, 1);
    int rotX = vmArgFixedGet(2, a1);
    VMArg* a2 = vmArgPtrGet(pThread, 2);
    int rotY = vmArgFixedGet(3, a2);
    VMArg* a3 = vmArgPtrGet(pThread, 3);
    int rotZ = vmArgFixedGet(4, a3);
    void* ctx = func_801862C0();
    CfObjIf* obj = (CfObjIf*)func_801864DC(ctx, handle);
    // Fixed-point angles -> floats via MWCC's direct 2^52 signed conversion,
    // scaled back into the object's units, then handed to vtable slot 0x1F4.
    Vec3f vec;
    vec.x = (float)(s32)rotX / lbl_eu_80665C30;
    vec.y = (float)(s32)rotY / lbl_eu_80665C30;
    vec.z = (float)(s32)rotZ / lbl_eu_80665C30;
    obj->_v01F4(&vec);
    return 0;
}

// us-8003d5dc: reads a VM array of fixed-point positions and hands them to
// vtable slot 0x1E0 (path/move-list setter).
extern "C" int func_8003D060(VMThread* pThread, int handle) {
    // Declaration order controls MWCC's callee-saved register assignment;
    // ordered here to match retail's allocation.
    int angle;
    cf::CfObject* obj;
    Vec3f pts[9];
    Vec3f* out;
    int modeA;
    int modeB;
    VMArg* arr;

    modeA = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    modeB = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    arr = (VMArg*)vmArgArrayGet(4, vmArgPtrGet(pThread, 3));
    if (vmArgOmitChk(pThread, 4)) {
        angle = 0x168;
    } else {
        angle = vmArgIntGet(5, vmArgPtrGet(pThread, 4));
    }
    obj = (cf::CfObject*)func_801864DC(func_801862C0(), handle);
    u16 count = *(u16*)((u8*)arr + 2);
    // Element count must be a non-zero multiple of 3, at most 0x18.
    if ((count % 3) != 0 || (u32)(count - 3) > 0x18) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    Vec3f start = *((CfObjIf*)obj)->_v00AC();
    out = pts;
    for (int i = 0; i < count; i += 3) {
        // Fixed-point array elements -> floats via the signed 2^52 conversion
        // divided by lbl_eu_80665C30 (2048.0).
        out->x = (s32)((VMArg*)vmArrayGet(pThread, arr, i))->value.intVal / lbl_eu_80665C30;
        out->y = (s32)((VMArg*)vmArrayGet(pThread, arr, i + 1))->value.intVal / lbl_eu_80665C30;
        out->z = (s32)((VMArg*)vmArrayGet(pThread, arr, i + 2))->value.intVal / lbl_eu_80665C30;
        out++;
    }
    ((CfObjIf*)obj)->_v01E0(modeA, modeB, count / 3 + 1, &start, (float)angle);
    return 0;
}

int func_8003D2B8(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    int busy = ((CfObjIf*)obj)->vf01E8();
    // VM bool retval: type 2 (false) when idle, 1 (true) when busy.
    VMArg retVal;
    retVal.type = (u8)((((-busy) | busy) >> 31) + 2);
    vmRetValSet(pThread, &retVal);
    return 1;
}

int func_8003D32C(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    // If the object's "busy" check (vtable[0x1E8]) is false, hold the script.
    if (((CfObjIf*)obj)->vf01E8() == 0) {
        vmWaitModeSet(pThread);
        return 0;
    }
    return 0;
}

void moveTo(){}

extern "C" int func_8003D570(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int arg = vmArgIntGet(2, ptr);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void** vt = *(void***)obj;
    ((void(*)(void*, int))vt[0x1F8/4])(obj, arg);
    return 0;
}

// us-8003db58: three (ptr,int) arg pairs dispatched to vtable slot 0x1FC.
extern "C" int func_8003D5DC(VMThread* pThread, int handle) {
    VMArg* a1 = vmArgPtrGet(pThread, 1);
    int a = vmArgIntGet(2, a1);
    VMArg* a2 = vmArgPtrGet(pThread, 2);
    int b = vmArgIntGet(3, a2);
    VMArg* a3 = vmArgPtrGet(pThread, 3);
    int c = vmArgIntGet(4, a3);
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    ((CfObjIf*)obj)->_v01FC(a, b, c);
    return 0;
}

int isTalk(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    u32 flags = obj->unk64;
    // volatile keeps the per-arm type stores as separate memory writes
    // (MWCC would otherwise merge the diff-1 select into branchless arithmetic)
    volatile VMArg retVal;
    if ((flags & 0x4) || (flags & 0x2)) {
        // The talk state lives in an object embedded 0x3E9C bytes before the
        // CfObject; query it through its vtable slot 0x30 and pass the result
        // to the battle-state checker.
        u8* base = (u8*)obj;
        if (obj != NULL) {
            base -= 0x3E9C;
        }
        cf::CfObject* sub = *(cf::CfObject**)(base + 4);
        u32 val = *((TalkSubIf*)sub)->_v0030();
        // Battle-side talk permission check.
        if (func_80174C98(base, &val, 1) != 0) {
            retVal.type = 1;
        } else {
            retVal.type = 2;
        }
        vmRetValSet(pThread, (VMArg*)&retVal);
        return 1;
    }
    if (flags & 0x8) {
        if (((CfObjIf*)obj)->_v0024(1) != 0) {
            retVal.type = 1;
        } else {
            retVal.type = 2;
        }
        vmRetValSet(pThread, (VMArg*)&retVal);
        return 1;
    }
    retVal.type = 2;
    vmOCExceptionThrow(pThread);
    return 0;
}

extern "C" void* CObjectState_UnkVirtualFunc11__Q22cf12CObjectStateFv(void* self) {
    return (void*)((u8*)self + 8);
}

int onEvent(VMThread* pThread, int handle) {
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    u32 flags = obj->unk64;
    VMArg retVal;
    if ((flags & 0x8) || (flags & 0x2) || (flags & 0x4000) || (flags & 0x8000)) {
        // Talkable / event object: ask the state machine (vtable slot 0x2C)
        // which talk mode is active and report TRUE/FALSE type to the VM.
        if (((CfObjIf*)obj)->_v002C(1, 1) != 0 || ((CfObjIf*)obj)->_v002C(2, 1) != 0) {
            if ((lbl_eu_80663E24 & 0x08000000) == 0) {
                retVal.type = 1;
            } else {
                retVal.type = 2;
            }
        } else {
            retVal.type = 2;
        }
    } else {
        // Not a talk/event object: the OC query fails.
        retVal.type = 2;
        vmOCExceptionThrow(pThread);
        return 0;
    }
    vmRetValSet(pThread, &retVal);
    return 1;
}

extern "C" int CObjectState_UnkVirtualFunc10__Q22cf12CObjectStateFv(cf::CObjectState* self, void* arg, int flag) {
    // State query: slot 0x28 is a gate check; slot 0x24 (UnkVirtualFunc8)
    // tests the requested state bit. flag selects the required polarity.
    s32 result;
    if (flag != 0) {
        result = 0;
        if (((CfObjIf*)self)->_v0028() != 0) {
            goto done;
        }
        if (self->CObjectState_UnkVirtualFunc8((int)arg) == 0) {
            goto done;
        }
        result = 1;
    } else {
        result = 0;
        if (((CfObjIf*)self)->_v0028() == 0) {
            goto done;
        }
        if (self->CObjectState_UnkVirtualFunc8((int)arg) != 0) {
            goto done;
        }
        result = 1;
    }
done:
    return result;
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
    VMArg retVal;
    if (obj->unk64 & 0x8) {
        // State-mask query through vtable slot 0x2C: report TRUE/FALSE type.
        if (((CfObjIf*)obj)->_v002C(0x2000, 1) != 0) {
            retVal.type = 1;
        } else {
            retVal.type = 2;
        }
    } else {
        retVal.type = 2;
        vmOCExceptionThrow(pThread);
        return 0;
    }
    vmRetValSet(pThread, &retVal);
    return 1;
}

// Interface view of the talk-state controller object embedded 0x3E9C bytes
// into the player actor: slot 0x8 sets a talk flag, slot 0x50 opens the talk
// window targeting the given object.
struct TalkCtrlIf {
    virtual void _v0008(int flag);
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
    virtual void _v0050(cf::CfObject* target);
};

// View of the player actor whose talk controller is an EMBEDDED member at
// +0x3E9C (member access lets MWCC use displaced vtable loads).
struct PlayerTalkView {
    u8 _0000[0x3E9C];
    TalkCtrlIf talk;
};

// us-8003dfec: winTalk
// Starts a talk window interaction
extern "C" int winTalk(VMThread* pThread, int handle) {
    const char* str = vmArgStringGet(2, vmArgPtrGet(pThread, 1));
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(func_801862C0(), handle);
    cf::CfObjectMove* player;
    // Talk state lives 0x3E9C bytes below the player object base.
    player = cf::CfGameManager::getPlayer(0);
    if (player != NULL) {
        player = (cf::CfObjectMove*)((u8*)player - 0x3E9C);
    }
    func_8013D07C(*(void**)((u8*)obj + 0x74), str, 1);
    u32 flags = obj->unk64;
    if (flags & 0x8) {
        if (((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc10((void*)1, 1) == 0) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(1);
        }
        if (!code80135FDC_getByte_64058()) {
            // Query the actor's sub-object (offset 4) for the current value,
            // then ask the battle side whether talking is allowed.
            u32 val = *((TalkSubIf*)*(cf::CfObject**)((u8*)player + 4))->_v0030();
            if (func_80174C98(player, &val, 1) == 0) {
                ((PlayerTalkView*)player)->talk._v0008(1);
                ((PlayerTalkView*)player)->talk._v0050(*(cf::CfObject**)((u8*)obj + 0x74));
            }
        }
    } else if ((flags & 0x4000) || (flags & 0x8000)) {
        if (((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc8(1) == 0) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(1);
        }
        u32 val = *((TalkSubIf*)*(cf::CfObject**)((u8*)player + 4))->_v0030();
        if (func_80174C98(player, &val, 1) == 0) {
            ((PlayerTalkView*)player)->talk._v0008(1);
            ((PlayerTalkView*)player)->talk._v0050(*(cf::CfObject**)((u8*)obj + 0x74));
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
    cf::CfObject* obj;
    VMArg* arg1;
    const char* str;
    arg1 = vmArgPtrGet(pThread, 1);
    str = vmArgStringGet(2, arg1);
    void* ctx = func_801862C0();
    obj = (cf::CfObject*)func_801864DC(ctx, handle);
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
    int fixedParam;
    int actionId;
    VMArg* arg1 = vmArgPtrGet(pThread, 1);
    actionId = vmArgIntGet(2, arg1);
    if (vmArgOmitChk(pThread, 2)) {
        fixedParam = 1;
    } else {
        VMArg* arg2 = vmArgPtrGet(pThread, 2);
        fixedParam = vmArgFixedGet(3, arg2);
    }
    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    // func_800BE12C is header-declared as the 4-arg form; retail call sites
    // pass a 5th arg (r7=1) the callee ignores — cast to keep the r3-r7 setup.
    ((void (*)(void*, int, int, int, int))&func_800BE12C)(obj, actionId, 0, -1, 1);
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

// Shared tail of lookAt: rotate-or-snap the self object onto the target's
// named bone (retail inlines this helper at each call site).
static void lookAtBone(cf::CfObject* self, cf::CfObject* actor, int snap, const char* name) {
    if (snap) {
        ((CfObjIf*)self)->_v01B0(actor, name);
    } else {
        ((CfObjIf*)self)->_v01AC(actor, name);
    }
}

// us-8003e5ec: makes an object look at another OC object (by bone name when
// both support it) or directly at its position vector.
extern "C" int lookAt(VMThread* pThread, int handle) {
    // Running VM argument index, advanced as each optional argument is consumed.
    int targetOC;
    int idx;
    int snap;
    int rotate;
    if (vmArgOmitChk(pThread, 1)) {
        targetOC = 0;
        idx = 2;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        idx = 2;
        targetOC = (int)vmArgOCGet(2, ptr);
    }
    if (vmArgOmitChk(pThread, idx)) {
        snap = 0;
        idx++;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, idx);
        idx++;
        snap = vmArgBoolGet(idx, ptr);
    }
    if (vmArgOmitChk(pThread, idx)) {
        rotate = 1;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, idx);
        idx++;
        rotate = vmArgBoolGet(idx, ptr);
    }
    cf::CfObject* self = (cf::CfObject*)func_801864DC(func_801862C0(), handle);
    if (targetOC != 0) {
        OcTargetView* oc = (OcTargetView*)targetOC;
        cf::CfObject* target = (cf::CfObject*)func_801864DC(func_801862C0(), (int)oc->field_0x04);
        CfObjIf* actor = (CfObjIf*)__dynamic_cast(target, 0, lbl_eu_806618E8, lbl_eu_806618F0, 0);
        if (actor == NULL) {
            // Target is not an actor: aim straight at its position vector.
            Vec3f* pos = ((CfObjIf*)target)->_v00AC();
            ((CfObjIf*)self)->_v01A8(pos);
        } else if ((((cf::CfObject*)actor)->unk64 & 0x2) || (((cf::CfObject*)actor)->unk64 & 0x8)) {
            // Talkable/event actor: only look when the self object has a bone
            // target set and the actor owns one of the known attach bones.
            if (((CfObjBoneView*)self)->field_0xC4 != NULL) {
                if (((CfObjIf*)actor)->_v0120(&lbl_eu_804FA74C[0x3C])) {
                    lookAtBone(self, (cf::CfObject*)actor, snap, &lbl_eu_804FA74C[0x3C]);
                } else if (((CfObjIf*)actor)->_v0120(&lbl_eu_804FA74C[0x43])) {
                    lookAtBone(self, (cf::CfObject*)actor, snap, &lbl_eu_804FA74C[0x43]);
                } else if (((CfObjIf*)actor)->_v0120(&lbl_eu_804FA74C[0x4B])) {
                    lookAtBone(self, (cf::CfObject*)actor, snap, &lbl_eu_804FA74C[0x4B]);
                }
            }
        } else {
            lookAtBone(self, (cf::CfObject*)actor, snap, &lbl_eu_804FA74C[0x4B]);
        }
        ((CfObjIf*)self)->_v01B4(rotate != 0);
    } else {
        // No target: face the default direction via slot 0x1B0 with NULL
        // (no rotate-flag call in this path).
        ((CfObjIf*)self)->_v01B0(NULL, &lbl_eu_804FA74C[0x3C]);
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
    float f = (float)(s32)angle * lbl_eu_8066A210;
    obj->CfObject_UnkVirtualFunc29(f);
    // func_800BE12C is header-declared as the 4-arg form; retail call sites
    // pass a 5th arg (r7=1) the callee ignores — cast to keep the r3-r7 setup.
    ((void (*)(void*, int, int, int, int))&func_800BE12C)(obj, 3, 0, -1, 1);
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
    // Declaration order controls MWCC's r30/r31 naming here: the OC handle
    // must land in r30 and the object pointer in r31 to match retail.
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(func_801862C0(), handle);
    u32 ocHandle;
    ocHandle = obj->CObjectParam_UnkVirtualFunc5();
    if (!ocHandle) {
        if (obj == (cf::CfObject*)cf::CfGameManager::getPlayer(0)) {
            ocHandle = *(u32*)((u8*)func_800FE68C() + 0x90E4);
        }
    }
    VMArg retVal;
    if (ocHandle) {
        cf::CfObject* target = (cf::CfObject*)func_801864DC(func_801862C0(), ocHandle);
        const char* typeName;
        if (target->unk64 & 0x80) {
            typeName = &lbl_eu_804FA74C[0x50];
        } else if (target->unk64 & 0x4000) {
            typeName = &lbl_eu_804FA74C[0x54];
        } else if (target->unk64 & 0x20) {
            typeName = &lbl_eu_804FA74C[0x5A];
        } else if (target->unk64 & 0x8000) {
            typeName = &lbl_eu_804FA74C[0x61];
        } else {
            typeName = &lbl_eu_804FA74C[0x66];
        }
        u8 type = 9;
        retVal.type = type;
        u16 ocId = vmOCSearch(typeName);
        retVal.unk2 = ocId;
        retVal.value.pointerVal = *(void**)((u8*)target + 0x74);
    } else {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003ebe8: func_8003E66C
// Gets the current battle target's unit. The +0x4C virtual returns this
// unit's own OC handle; when valid it is fed through the same virtual again
// and resolved back to an object whose +0x8C halfword becomes the result.
extern "C" int func_8003E66C(VMThread* pThread, int handle) {
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(func_801862C0(), handle);
    VMArg retVal;
    if (((cf::CObjectParam*)obj)->CObjectParam_UnkVirtualFunc5()) {
        u32 id = ((cf::CObjectParam*)obj)->CObjectParam_UnkVirtualFunc5();
        void* target = func_801864DC(func_801862C0(), id);
        retVal.type = 3;
        retVal.value.uintVal = *(u16*)((u8*)target + 0x8C);
    }
    vmRetValSet(pThread, &retVal);
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
// Reads up to 5 VM args (arg 1 always present, 2..5 optional/omittable), looks
// up the handle's object as a CfObjectActor and queues a battle event for it.
extern "C" int func_8003E974(VMThread* pThread, int handle) {
    // Sequential omittable-arg reader: idx is a running counter shared by
    // all blocks (each block advances it whether or not its arg was omitted).
    int idx;
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int eventId = vmArgIntGet(2, ptr);

    int arg2;
    if (vmArgOmitChk(pThread, 2)) {
        arg2 = 0;
        idx = 3;
    } else {
        VMArg* p = vmArgPtrGet(pThread, 2);
        arg2 = vmArgIntGet(3, p);
        idx = 3;
    }

    int arg3;
    if (vmArgOmitChk(pThread, idx)) {
        arg3 = 0;
        idx++;
    } else {
        VMArg* p = vmArgPtrGet(pThread, idx);
        idx++;
        arg3 = vmArgIntGet(idx, p);
    }

    int arg4;
    if (vmArgOmitChk(pThread, idx)) {
        arg4 = 0;
        idx++;
    } else {
        VMArg* p = vmArgPtrGet(pThread, idx);
        idx++;
        arg4 = vmArgIntGet(idx, p);
    }

    int arg5;
    if (vmArgOmitChk(pThread, idx)) {
        arg5 = 0;
    } else {
        VMArg* p = vmArgPtrGet(pThread, idx);
        idx++;
        arg5 = vmArgIntGet(idx, p);
    }

    void* ctx = func_801862C0();
    CfObjectActorView* actor =
        (CfObjectActorView*)__dynamic_cast(func_801864DC(ctx, handle), 0,
                                               (void*)&__RTTI__Q22cf13CfObjectActor,
                                               (void*)&lbl_eu_806618F0, 0);
    if (actor) {
        BattleEventWork data;
        memset(&data, 0, sizeof(data));
        // Plain int->float converts: MWCC lowers these via its shared
        // sdata2 0x43300000-magic double (one lfd, two fsubs in retail).
        data.field_00 = actor->field_0x3F10;
        data.field_0C = (u16)eventId;
        data.field_10 = arg3;
        data.field_14 = (u16)arg4;
        data.field_20 = arg2;
        data.field_24 = arg5;
        func_800EC8FC(getInstance__Q22cf14CBattleManagerFv(), actor, &data, 0);
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
// Spawns a battle party object: reads (partyId[, arg2[, arg3]]), toggles the
// 0x40000 bit of the global flag word around the spawn, throws a VM exception
// when the spawn fails, and returns the spawned context as a VM object.
extern "C" int func_8003EEE0(VMThread* pThread, int handle, int r5) {
    void* ctx;
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int partyId = vmArgIntGet(2, ptr);
    int arg2;
    int idx;
    if (vmArgOmitChk(pThread, 2)) {
        arg2 = 0;
        idx = 3;
    } else {
        VMArg* ptr2 = vmArgPtrGet(pThread, 2);
        idx = 3;
        arg2 = vmArgIntGet(idx, ptr2);
    }
    int arg3;
    if (vmArgOmitChk(pThread, idx)) {
        arg3 = 0;
    } else {
        VMArg* ptr3 = vmArgPtrGet(pThread, idx);
        ++idx;
        arg3 = vmArgIntGet(idx, ptr3);
    }
    // Save the spawn-flag bit (volatile read keeps this load separate from
    // the reloads in the ternaries below, matching retail), toggle the bit
    // around the spawn, then restore the saved state.
    u32 flags = *(volatile u32*)&lbl_eu_80663E24;
    u32 savedFlag = (flags >> 14) & 1;
    lbl_eu_80663E24 = (arg3 != 0) ? (lbl_eu_80663E24 | 0x40000u)
                                  : (lbl_eu_80663E24 & ~0x40000u);
    cf::CfObject* obj = (cf::CfObject*)func_80081694__Q22cf13CfGameManagerFv((u16)partyId, (u16)arg2);
    lbl_eu_80663E24 = (savedFlag != 0) ? (lbl_eu_80663E24 | 0x40000u)
                                       : (lbl_eu_80663E24 & ~0x40000u);
    if (!obj) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    // func_801863F4's shared header declares it 1-arg; retail passes (ctx, obj).
    typedef void* (*CtxObjFn)(void*, void*);
    ctx = ((CtxObjFn)&func_801863F4)(func_801862C0(), obj);
    obj->mFlags68 |= 0x40000000;
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
    // Fixed-point degrees -> radians via MWCC's direct 2^52 conversion.
    Vec3f vec;
    vec.x = ((float)(s32)rotX / lbl_eu_80665C30) * lbl_eu_8066A210;
    vec.y = ((float)(s32)rotY / lbl_eu_80665C30) * lbl_eu_8066A210;
    vec.z = ((float)(s32)rotZ / lbl_eu_80665C30) * lbl_eu_8066A210;
    obj->CfObject_UnkVirtualFunc27(&vec);
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
    int param;
    int hadFlag;
    const char* name;
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    name = vmArgStringGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    param = vmArgIntGet(3, ptr2);
    int arg3;
    if (vmArgOmitChk(pThread, 3)) {
        arg3 = 0;
    } else {
        VMArg* ptr3 = vmArgPtrGet(pThread, 3);
        arg3 = vmArgIntGet(4, ptr3);
    }
    void* result = NULL;
    // Save event-flag bit 0x40000, force it on/off around the lookup.
    // Volatile cast keeps this load independent of the ternary-arm loads.
    u32 prevFlags = *(volatile u32*)&lbl_eu_80663E24;
    hadFlag = (prevFlags & 0x00040000) != 0;
    lbl_eu_80663E24 = (arg3 != 0) ? (lbl_eu_80663E24 | 0x00040000u)
                                  : (lbl_eu_80663E24 & ~0x00040000u);
    if (strcmp(name, &lbl_eu_804FA74C[0x6B]) == 0 || strcmp(name, &lbl_eu_804FA74C[0x54]) == 0) {
        result = func_80081CBC__Q22cf13CfGameManagerFv(&lbl_eu_804FA74C[0x6B], (u16)param);
    } else if (strcmp(name, &lbl_eu_804FA74C[0x79]) == 0) {
        result = func_80081CB8__Q22cf13CfGameManagerFv();
    }
    if (!result) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    lbl_eu_80663E24 = (hadFlag != 0) ? (lbl_eu_80663E24 | 0x00040000u)
                                     : (lbl_eu_80663E24 & ~0x00040000u);
    // Retail chains these: the ctx flowing into the retval is the return
    // value of func_801863F4 (kept in r3 across the call).
    void* ctx = func_801863F4(func_801862C0(), result);
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
// Battle party member lookup by name/type. Saves event-flag bit 0x40000
// aside, forces it per arg3 around the lookup, then restores it.
extern "C" int func_8003F498(VMThread* pThread, int handle, int r5) {
    int hadFlag;
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
    u32 prevFlags = *(volatile u32*)&lbl_eu_80663E24;
    hadFlag = (prevFlags & 0x00040000) != 0;
    lbl_eu_80663E24 = (arg3 != 0) ? (lbl_eu_80663E24 | 0x00040000u)
                                  : (lbl_eu_80663E24 & ~0x00040000u);
    void* result = NULL;
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
    if ((lbl_eu_80663E24 & 0x00040000) != 0) {
        *(u32*)((char*)result + 0x64) |= 0x00010000;
    }
    lbl_eu_80663E24 = (hadFlag != 0) ? (lbl_eu_80663E24 | 0x00040000u)
                                     : (lbl_eu_80663E24 & ~0x00040000u);
    // Retail chains these: the ctx flowing into the retval is the return
    // value of func_801863F4 (kept in r3 across the call).
    void* ctx = func_801863F4(func_801862C0(), result);
    *(u32*)((char*)result + 0x68) |= 0x40000000;
    VMArg retVal;
    retVal.type = 9;
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
                child = ((cf::CfObject*)followTarget)->CfObject_UnkVirtualFunc22();
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
    // func_801863F4 is declared single-arg in code_801862C0.hpp, but the
    // retail callee receives (container, object). Route through a matching
    // function-pointer cast so both argument registers are set up.
    typedef void* (*GetEntryFn)(void*, void*);
    void* entry = ((GetEntryFn)&func_801863F4)(ctx, battleMgr);
    *(u32*)((u8*)battleMgr + 0x68) |= 0x40000000;
    u8 type = 9;
    VMArg retVal;
    retVal.type = type;
    retVal.unk2 = (u16)r5;
    retVal.value.pointerVal = entry;
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
// Moves an object to a fixed-point position plus yaw angle: fetches four
// fixed-point args, resolves the object, converts to floats (/2048 via the
// 0x43300000 double trick), queries the move object through vtable slot 0xAC,
// then forwards everything to func_800ABFC4.
extern "C" int func_8003FD48(VMThread* pThread, int handle) {
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    int x = vmArgFixedGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int y = vmArgFixedGet(3, ptr2);
    VMArg* ptr3 = vmArgPtrGet(pThread, 3);
    int z = vmArgFixedGet(4, ptr3);
    VMArg* ptr4 = vmArgPtrGet(pThread, 4);
    int angle = vmArgFixedGet(5, ptr4);

    void* ctx = func_801862C0();
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);

    float fx = (float)(s32)x / lbl_eu_80665C30;
    float fy = (float)(s32)y / lbl_eu_80665C30;
    float fz = (float)(s32)z / lbl_eu_80665C30;
    float fAngle = (float)(s32)angle / lbl_eu_80665C30;

    if (obj != NULL) {
        void* moveObj = ((CfObjIf_AC*)obj)->getMoveObj();
        Vec3f pos;
        pos.x = fx;
        pos.y = fy;
        pos.z = fz;
        func_800ABFC4(obj, moveObj, (ml::CVec3*)&pos, fAngle * lbl_eu_8066A210);
    }
    return 0;
}

// us-80040458: func_8003FEDC
// Checks whether two OC objects are related (position/talk link), reporting
// the result as a VM bool.
extern "C" int func_8003FEDC(VMThread* pThread, int handle) {
    cf::CfObject* target;
    if (vmArgOmitChk(pThread, 1)) {
        target = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        target = (cf::CfObject*)vmArgOCGet(2, ptr);
    }
    if (target != 0) {
        void* ctx2 = func_801862C0();
        target = (cf::CfObject*)func_801864DC(ctx2, *(u32*)((u8*)target + 4));
    } else {
        target = (cf::CfObject*)func_80082D90__Q22cf13CfGameManagerFv();
    }
    void* ctx = func_801862C0();
    cf::CfObject* self = (cf::CfObject*)func_801864DC(ctx, handle);
    int ok = 0;
    if (target != 0) {
        ok = func_800AB580((cf::CfObjectColl*)self, target, (ml::CVec3*)0,
                           lbl_eu_80665D44);
        // When the target is an embedded move object, gate on the game state.
        if (ok != 0 && (target->unk64 & 0x2)) {
            if (func_80082FE4__Q22cf13CfGameManagerFv(0) == false) {
                ok = 0;
            }
        }
        if (lbl_eu_80663E28 & 0x20000000) {
            ok = 0;
        }
    }
    // VM bool retval: type 1 (true) / 2 (false).
    VMArg retVal;
    retVal.type = (u8)((ok == 0) + 1);
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-80040570: func_8003FFF4
// Sets a position on an object using 3 fixed-point coords
// us-80040570
extern "C" int func_8003FFF4(VMThread* pThread, int handle) {
    cf::CfObject* target;
    if (vmArgOmitChk(pThread, 1)) {
        target = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        target = (cf::CfObject*)vmArgOCGet(2, ptr);
    }
    if (target != 0) {
        void* ctx2 = func_801862C0();
        target = (cf::CfObject*)func_801864DC(ctx2, *(u32*)((u8*)target + 4));
    } else {
        target = cf::CfGameManager::getPlayer(0);
    }
    void* ctx = func_801862C0();
    cf::CfObject* self = (cf::CfObject*)func_801864DC(ctx, handle);
    int ok = 0;
    if (target != 0) {
        ok = func_800AB580((cf::CfObjectColl*)self, target, (ml::CVec3*)0,
                           lbl_eu_80665D44);
        if (!(target->unk64 & 0x2)) {
            ok = 0;
        }
    }
    // VM bool retval: type 1 (true) / 2 (false).
    VMArg retVal;
    retVal.type = (u8)((ok == 0) + 1);
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-80040664: ocUnitRegist
// Registers all OC unit plugin functions
// Register the OC unit: init contexts then vmOCRegist each of the five
// OCData sub-objects in the shared plugin area (lbl_eu_80524E50).
extern char lbl_eu_80524E50[];
extern "C" int vmOCRegist(void* pOC);
extern "C" void ocUnitRegist() {
    void* base = (void*)&lbl_eu_80524E50;
    func_801862E0(func_801862C0());
    vmOCRegist((char*)base + 0x2D0);
    vmOCRegist((char*)base + 0x398);
    vmOCRegist((char*)base + 0x488);
    vmOCRegist((char*)base + 0x560);
    vmOCRegist((char*)base + 0x638);
}

void CObjectState_UnkVirtualFunc1__Q22cf12CObjectStateFv(void* self, unsigned long bits) {
    *(unsigned long*)((char*)self + 4) |= bits;
}

void CObjectParam_UnkVirtualFunc6__Q22cf12CObjectParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x34) = val; }

void CObjectState_UnkVirtualFunc3__Q22cf12CObjectStateFv(void* self, unsigned long mask) {
    *(unsigned long*)((char*)self + 4) &= ~mask;
}

void CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv(void* self, u8 val) { static_cast<cf::CfObjectModel*>(self)->field_BD = val; }

u32 CObjectParam_UnkVirtualFunc5__Q22cf12CObjectParamFv(void* self) { return *(u32*)((u8*)self + 0x34); }

// us-8003f6d8: 12-byte word copy src -> this+0x48. Retail symbol is
// Fv-mangled but the body consumes a pointer in r4 (decompiler-name guess,
// same scheme as CfObjectModel's CfObject_UnkVirtualFunc27). A TYPED source
// pointer is required for MWCC's grouped load-all/store-all 3-word copy
// (lwz r6/r5/r0 + stw 0x48/0x4C/0x50); a void* param emits interleaved
// load-store pairs. Defined as a global whose name IS the retail mangled
// name so the vtable slot 0xBC and the virtual call sites resolve.
struct CfObjCopy12 { u32 a; u32 b; u32 c; };
struct CfObjVec48View {
    u8 _pad[0x48];
    CfObjCopy12 vec48;
};
extern "C" void CfObject_UnkVirtualFunc27__Q22cf8CfObjectFPv(CfObjVec48View* self, const CfObjCopy12* src) {
    self->vec48 = *src;
}

void cf::CfObject::CfObject_UnkVirtualFunc64(int flag) {
    u32* field = (u32*)((char*)this + 0x68);
    if (flag) {
        *field |= 0x01000000;
    } else {
        *field &= ~0x01000000;
    }
}

extern const float lbl_eu_8066A20C;
extern "C" float CfObject_UnkVirtualFunc34__Q22cf8CfObjectFv(void* self) { return *(float*)((u8*)self + 0x4c) * lbl_eu_8066A20C; }

extern "C" void CfObject_UnkVirtualFunc30__Q22cf8CfObjectFv(void* self) { reinterpret_cast<CfObjIf*>(self)->vf00C4(); }
#include "kyoshin/plugin/pluginGame.hpp"
#include "kyoshin/plugin/pluginCam.hpp"
#include "monolib/vm/yvm2.h"
#include "monolib/math.hpp"
#include "kyoshin/code_801862C0.hpp"
#include "monolib/core.hpp"

extern "C" {
extern char lbl_eu_804FB1A4[];
extern PluginFuncData lbl_eu_80526560[];
}

// Camera interface (from CfGameManager)
class UnkCamIntf {
public:
    virtual void vfunc_0x08();
    virtual void vfunc_0x0C();
    virtual void vfunc_0x10();
    virtual void vfunc_0x14();
    virtual void vfunc_0x18();
    virtual void vfunc_0x1C();
    virtual void vfunc_0x20();
    virtual void vfunc_0x24();
    virtual void vfunc_0x28();
    virtual void vfunc_0x2C();
    virtual void vfunc_0x30();
    virtual void vfunc_0x34();
    virtual void vfunc_0x38();
    virtual void vfunc_0x3C();
    virtual void vfunc_0x40();
    virtual void vfunc_0x44();
    virtual void vfunc_0x48();
    virtual void vfunc_0x4C(void* dir);
    virtual void vfunc_0x50(float rotX);
    virtual void vfunc_0x54(float rotY);
    virtual void vfunc_0x58();
    virtual void vfunc_0x5C(u32 targetId);
    virtual void vfunc_0x60();
    virtual void vfunc_0x64(void* lookat);
    virtual void vfunc_0x68(void* obj, void* pos, int flags);
    virtual void vfunc_0x6C(void* obj, void* lookat, int flags);
    u8 field_0x04[0x0C - 0x04];
    void* objPtr; // 0x0C
};

// Incomplete struct for shake data (0x34 bytes)
struct ShakeData {
    f32 values[13];
};

extern "C" {
UnkCamIntf* func_800821F8__Q22cf13CfGameManagerFv();
void func_80085878__Q22cf13CfGameManagerFv();
void func_80082060__Q22cf13CfGameManagerFv();
bool func_80082104__Q22cf13CfGameManagerFv();
void func_8008212C__Q22cf13CfGameManagerFv(u32 mode);
void func_80081E90__Q22cf13CfGameManagerFv(u32 third, u32 fourth, u32 seventh);
u32 func_80082008__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third,
                                          u32 fourth, u32 fifth);
void func_80082088__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third,
                                           u32 fourth, float value);
void func_8007B044(ShakeData* data);
void func_8007B078(s32 val);
void func_8016FD84(f32 val1, f32 val2);
}

static inline f32 fixedToFloat(s32 fixed) {
    return (f32)fixed / 4096.0f;
}

static inline s32 floatToFixed(f32 val) {
    return (s32)(val * 4096.0f);
}

static inline f32 degToRad(s32 fixedDeg) {
    return fixedToFloat(fixedDeg) * 0.01745329238474369f;
}

static inline s32 radToDegFixed(f32 rad) {
    return (s32)(rad * 57.2957763671875f * 4096.0f);
}

extern "C" int select(VMThread* pThread) {
    s32 mode = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    func_8008212C__Q22cf13CfGameManagerFv(mode);
    return 0;
}

extern "C" int restore(VMThread* pThread) {
    s32 cameraId;
    s32 targetCamId;

    if (vmArgOmitChk(pThread, 1)) {
        cameraId = 0;
    } else {
        cameraId = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    }

    s32 transitionTime;
    if (vmArgOmitChk(pThread, 2)) {
        transitionTime = 0;
    } else {
        transitionTime = vmArgIntGet(2, vmArgPtrGet(pThread, 2));
    }

    func_80081E90__Q22cf13CfGameManagerFv(
        (cameraId < 0) ? 1 : 0,
        (transitionTime < 0) ? 1 : 0,
        0);
    return 0;
}

extern "C" int setPos(VMThread* pThread) {
    s32 fixedX = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedY = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedZ = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));

    ml::CVec3 pos = {
        fixedToFloat(fixedX),
        fixedToFloat(fixedY),
        fixedToFloat(fixedZ)
    };

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x14(&pos);
    func_80085878__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int setLookat(VMThread* pThread) {
    s32 fixedX = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedY = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedZ = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));

    ml::CVec3 lookat = {
        fixedToFloat(fixedX),
        fixedToFloat(fixedY),
        fixedToFloat(fixedZ)
    };

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x64(&lookat);
    func_8016FD84(0.0f, 1.0f);
    return 0;
}

extern "C" int setDir(VMThread* pThread) {
    s32 fixedAngle = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedDist = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));

    ml::CVec3 dir = {
        degToRad(fixedAngle),
        fixedToFloat(fixedDist),
        0.0f
    };

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x4C(&dir);
    func_80085878__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int setRotX(VMThread* pThread) {
    s32 fixedAngle = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    f32 rotX = degToRad(fixedAngle);

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x50(rotX);
    return 0;
}

extern "C" int setRotY(VMThread* pThread) {
    s32 fixedAngle = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    f32 rotY = degToRad(fixedAngle);

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x54(rotY);
    return 0;
}

extern "C" int setFov(VMThread* pThread) {
    s32 fixedFov = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    f32 fov = fixedToFloat(fixedFov);

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x3C(fov);
    return 0;
}

extern "C" int setTarget(VMThread* pThread) {
    void* oc = vmArgOCGet(2, vmArgPtrGet(pThread, 1));
    u32 targetId = *(u32*)((u8*)oc + 4);

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x5C(targetId);
    func_80085878__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int setPosOfs(VMThread* pThread) {
    void* oc = vmArgOCGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedX = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedY = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    s32 fixedZ = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));

    s32 flags;
    if (vmArgOmitChk(pThread, 5)) {
        flags = 0;
    } else {
        flags = vmArgIntGet(6, vmArgPtrGet(pThread, 5));
    }

    ml::CVec3 pos = {
        fixedToFloat(fixedX),
        fixedToFloat(fixedY),
        fixedToFloat(fixedZ)
    };

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    void* obj = func_801862C0(pThread);
    void* slot = func_801864DC(obj, *(u32*)((u8*)oc + 4));
    cam->vfunc_0x68(slot, &pos, (flags < 0) ? 1 : 0);
    return 0;
}

extern "C" int setLookatOfs(VMThread* pThread) {
    void* oc = vmArgOCGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedX = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedY = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    s32 fixedZ = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));

    s32 flags;
    if (vmArgOmitChk(pThread, 5)) {
        flags = 0;
    } else {
        flags = vmArgIntGet(6, vmArgPtrGet(pThread, 5));
    }

    ml::CVec3 lookat = {
        fixedToFloat(fixedX),
        fixedToFloat(fixedY),
        fixedToFloat(fixedZ)
    };

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    void* obj = func_801862C0(pThread);
    void* slot = func_801864DC(obj, *(u32*)((u8*)oc + 4));
    cam->vfunc_0x6C(slot, &lookat, (flags < 0) ? 1 : 0);
    func_8016FD84(0.0f, 1.0f);
    return 0;
}

extern "C" int getPos(VMThread* pThread) {
    s32 index = vmArgIntGet(2, vmArgPtrGet(pThread, 1));

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    void* camObj = cam->objPtr;

    VMArg ret;
    ret.type = VM_TYPE_INT;

    if (index == 0) {
        ret.value.intVal = floatToFixed(*(f32*)((u8*)camObj + 0x10C));
    } else if (index == 1) {
        ret.value.intVal = floatToFixed(*(f32*)((u8*)camObj + 0x110));
    } else if (index == 2) {
        ret.value.intVal = floatToFixed(*(f32*)((u8*)camObj + 0x114));
    }

    vmRetValSet(pThread, &ret);
    return 1;
}

extern "C" int getRot(VMThread* pThread) {
    s32 index = vmArgIntGet(2, vmArgPtrGet(pThread, 1));

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    void* camObj = cam->objPtr;

    VMArg ret;
    ret.type = VM_TYPE_INT;

    if (index == 0) {
        ret.value.intVal = radToDegFixed(*(f32*)((u8*)camObj + 0x118));
    } else if (index == 1) {
        ret.value.intVal = radToDegFixed(*(f32*)((u8*)camObj + 0x11C));
    } else if (index == 2) {
        ret.value.intVal = radToDegFixed(*(f32*)((u8*)camObj + 0x120));
    }

    vmRetValSet(pThread, &ret);
    return 1;
}

extern "C" int keyBegin(VMThread* pThread) {
    s32 first = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    s32 second = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    s32 third;
    if (vmArgOmitChk(pThread, 3)) {
        third = 0;
    } else {
        third = vmArgIntGet(4, vmArgPtrGet(pThread, 3));
    }

    s32 fourth;
    if (vmArgOmitChk(pThread, 4)) {
        fourth = 0;
    } else {
        fourth = vmArgIntGet(2, vmArgPtrGet(pThread, 4));
    }

    void* oc;
    if (vmArgOmitChk(pThread, 5)) {
        oc = nullptr;
    } else {
        void* obj = func_801862C0(pThread);
        void* ocRaw = vmArgOCGet(2, vmArgPtrGet(pThread, 5));
        oc = func_801864DC(obj, *(u32*)((u8*)ocRaw + 4));
    }

    func_80082008__Q22cf13CfGameManagerFv(
        first, second, third, (u32)fourth & 0xFFFF, (oc != nullptr) ? 1 : 0);
    return 0;
}

extern "C" int keyEnd(VMThread* pThread) {
    func_80082060__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int keyAdd(VMThread* pThread) {
    s32 first = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    s32 fixed1 = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixed2 = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    s32 fixed3 = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    s32 fixed4 = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    s32 fixed5 = vmArgFixedGet(7, vmArgPtrGet(pThread, 6));
    s32 fixed6 = vmArgFixedGet(8, vmArgPtrGet(pThread, 7));

    s32 seventh;
    if (vmArgOmitChk(pThread, 8)) {
        seventh = 0;
    } else {
        seventh = vmArgFixedGet(9, vmArgPtrGet(pThread, 8));
    }

    s32 extra;
    if (vmArgOmitChk(pThread, 9)) {
        extra = 0;
    } else {
        extra = vmArgIntGet(2, vmArgPtrGet(pThread, 9));
    }

    func_80082088__Q22cf13CfGameManagerFv(
        (u32)first & 0xFFFF,
        fixedToFloat(fixed1),
        fixedToFloat(fixed2),
        fixedToFloat(fixed3),
        (extra != 0) ? ((u32)extra & 0xFFFF) : 0);
    return 0;
}

extern "C" int isKeyMove(VMThread* pThread) {
    bool isMoving = func_80082104__Q22cf13CfGameManagerFv();

    VMArg ret;
    ret.type = VM_TYPE_INT;
    ret.value.intVal = isMoving ? 2 : 1;
    vmRetValSet(pThread, &ret);
    return 1;
}

extern "C" int waitKeyMove(VMThread* pThread) {
    if (func_80082104__Q22cf13CfGameManagerFv()) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

extern "C" int shake(VMThread* pThread) {
    s32 intParam = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    s32 fixed1 = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixed2 = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    s32 fixed3 = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    s32 fixed4 = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    s32 fixed5 = vmArgFixedGet(7, vmArgPtrGet(pThread, 6));
    s32 fixed6 = vmArgFixedGet(8, vmArgPtrGet(pThread, 7));
    s32 fixed7 = vmArgFixedGet(9, vmArgPtrGet(pThread, 8));
    s32 fixed8 = vmArgFixedGet(10, vmArgPtrGet(pThread, 9));
    s32 fixed9 = vmArgFixedGet(11, vmArgPtrGet(pThread, 10));
    s32 fixed10 = vmArgFixedGet(12, vmArgPtrGet(pThread, 11));
    s32 fixed11 = vmArgFixedGet(13, vmArgPtrGet(pThread, 12));
    bool boolParam = vmArgBoolGet(14, vmArgPtrGet(pThread, 13));

    ShakeData data;
    memset(&data, 0, sizeof(data));

    data.values[0] = (f32)intParam;
    data.values[1] = fixedToFloat(fixed1);
    data.values[2] = fixedToFloat(fixed2);
    data.values[6] = 0.0f;
    data.values[3] = fixedToFloat(fixed3);
    data.values[4] = fixedToFloat(fixed4);
    data.values[5] = fixedToFloat(fixed5);
    data.values[7] = fixedToFloat(fixed6);
    data.values[8] = fixedToFloat(fixed7);
    data.values[9] = fixedToFloat(fixed8);
    data.values[10] = fixedToFloat(fixed9);
    data.values[11] = fixedToFloat(fixed10);
    data.values[12] = fixedToFloat(fixed11);

    func_8007B044(&data);
    return 0;
}

extern "C" int stopShake(VMThread* pThread) {
    func_8007B078(0);
    return 0;
}

extern "C" void pluginCamRegist() {
    vmPluginRegist(lbl_eu_804FB1A4, lbl_eu_80526560);
}
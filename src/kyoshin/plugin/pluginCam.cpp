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

// 0x4330000080000000: MWCC (double)(s32) conversion magic (2^52 + 0x8000),
// defined under its retail sdata2 pool name so the literal pool relocs to
// lbl_eu_80666170 instead of a synthesised @N entry.
__declspec(section ".sdata2") extern const f64 lbl_eu_80666170 = 4503601774854144.0;

// Camera interface (from CfGameManager)
class UnkCamIntf {
public:
    virtual void vfunc_0x08();
    virtual void vfunc_0x0C();
    virtual void vfunc_0x10();
    virtual void vfunc_0x14(void* pos);
    virtual void vfunc_0x18();
    virtual void vfunc_0x1C();
    virtual void vfunc_0x20();
    virtual void vfunc_0x24();
    virtual void vfunc_0x28();
    virtual void vfunc_0x2C();
    virtual void vfunc_0x30();
    virtual void vfunc_0x34();
    virtual void vfunc_0x38();
    virtual void vfunc_0x3C(f32 fov);
    virtual void vfunc_0x40();
    virtual void vfunc_0x44();
    virtual void vfunc_0x48();
    virtual void vfunc_0x4C(void* dir);
    virtual void vfunc_0x50(f32 rotX);
    virtual void vfunc_0x54(f32 rotY);
    virtual void vfunc_0x58();
    virtual void vfunc_0x5C(u32 targetId);
    virtual void vfunc_0x60();
    virtual void vfunc_0x64(void* lookat);
    virtual void vfunc_0x68(void* obj, void* pos, int flags);
    virtual void vfunc_0x6C(void* obj, void* lookat, int flags);
    u8 field_0x04[0x0C - 0x04];
    CamStateObj* objPtr;
};

extern "C" {
UnkCamIntf* func_800821F8__Q22cf13CfGameManagerFv();
void func_80085878__Q22cf13CfGameManagerFv();
void func_80082060__Q22cf13CfGameManagerFv();
int func_80082104__Q22cf13CfGameManagerFv();
void func_8008212C__Q22cf13CfGameManagerFv(u32 mode);
void func_80081E90__Q22cf13CfGameManagerFv(u32 third, u32 fourth, u32 seventh);
u32 func_80082008__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third,
                                          u32 fourth, u32 fifth);
void func_80082088__Q22cf13CfGameManagerFv(u32 first, ml::CVec3* pos,
                                           ml::CVec3* lookat, u32 extra, f32 seventh);
void func_8007B044(void* data, int flag);
void func_8007B078(s32 val);
void func_8016FD84(f32 val1, f32 val2);
}

static inline f32 fixedToFloat(s32 fixed) {
    return (f32)fixed / lbl_eu_80666168;
}

static inline s32 floatToFixed(f32 val) {
    return (s32)(lbl_eu_80666168 * val);
}

static inline f32 degToRad(s32 fixedDeg) {
    return fixedToFloat(fixedDeg) * lbl_eu_8066A210;
}

static inline s32 radToDegFixed(f32 rad) {
    return (s32)(lbl_eu_80666168 * (rad * lbl_eu_8066A20C));
}

// s32 -> f32 via the shared sdata2 magic double (pluginCam.hpp F64Conv
// convention): builds the 0x43300000-prefixed bit pattern and subtracts the
// named pool constant so the reloc matches retail.
static inline f32 s32ToF32(s32 v) {
    F64Conv conv;
    conv.w[1] = (u32)v ^ 0x80000000;
    conv.w[0] = 0x43300000;
    return (f32)(conv.d - lbl_eu_80666170);
}

extern "C" int select(VMThread* pThread) {
    s32 mode = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    func_8008212C__Q22cf13CfGameManagerFv(mode);
    return 0;
}

extern "C" int restore(VMThread* pThread) {
    s32 cameraId;
    s32 transitionTime;
    s32 nextArg = 1;

    if (vmArgOmitChk(pThread, 1)) {
        cameraId = 0;
        nextArg++;
    } else {
        // Post-increment: MWCC folds the pre-increment value (1) to an
        // immediate and emits the r30 update just before the call.
        VMArg* arg = vmArgPtrGet(pThread, nextArg++);
        cameraId = vmArgIntGet(nextArg, arg);
    }

    if (vmArgOmitChk(pThread, nextArg)) {
        transitionTime = 0;
    } else {
        // Arg 3: optional; ptr index is nextArg (2), int value index nextArg+1 (3).
        // Post-increment keeps the side effect observable (retail stores r30 back).
        VMArg* arg = vmArgPtrGet(pThread, nextArg++);
        transitionTime = vmArgIntGet(nextArg, arg);
    }

    // Retinal bool args: neg/or/srwi booleanize of each value (nonzero -> 1).
    func_80081E90__Q22cf13CfGameManagerFv(
        (u32)(-cameraId | cameraId) >> 31,
        (u32)(-transitionTime | transitionTime) >> 31,
        0);
    return 0;
}

extern "C" int setPos(VMThread* pThread) {
    s32 fixedX = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedY = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedZ = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));

    ml::CVec3 pos;
    pos.x = (f32)fixedX / lbl_eu_80666168;
    pos.y = (f32)fixedY / lbl_eu_80666168;
    pos.z = (f32)fixedZ / lbl_eu_80666168;

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x14(&pos);
    func_80085878__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int setLookat(VMThread* pThread) {
    s32 fixedX = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedY = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedZ = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));

    ml::CVec3 lookat;
    lookat.x = (f32)fixedX / lbl_eu_80666168;
    lookat.y = (f32)fixedY / lbl_eu_80666168;
    lookat.z = (f32)fixedZ / lbl_eu_80666168;

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x64(&lookat);
    func_8016FD84(lbl_eu_80666178, lbl_eu_8066617C);
    return 0;
}

extern "C" int setDir(VMThread* pThread) {
    s32 fixedAngle = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedDist = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));

    ml::CVec3 dir;
    volatile f32 deg = lbl_eu_8066A210;
    dir.x = (f32)fixedAngle / lbl_eu_80666168 * deg;
    dir.y = (f32)fixedDist / lbl_eu_80666168 * deg;
    dir.z = lbl_eu_80666178;

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x4C(&dir);
    func_80085878__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int setRotX(VMThread* pThread) {
    s32 fixedAngle = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    f32 value = fixedToFloat(fixedAngle);
    f32 rotX = value * lbl_eu_8066A210;

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x50(rotX);
    return 0;
}

extern "C" int setRotY(VMThread* pThread) {
    s32 fixedAngle = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    f32 value = fixedToFloat(fixedAngle);
    f32 rotY = value * lbl_eu_8066A210;

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x54(rotY);
    return 0;
}

extern "C" int setFov(VMThread* pThread) {
    s32 fixedFov = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x3C(fixedToFloat(fixedFov));
    return 0;
}

extern "C" int setTarget(VMThread* pThread) {
    CamOCHandle* oc = (CamOCHandle*)vmArgOCGet(2, vmArgPtrGet(pThread, 1));

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    cam->vfunc_0x5C(oc->field_0x04);
    func_80085878__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int setPosOfs(VMThread* pThread) {
    CamOCHandle* oc = (CamOCHandle*)vmArgOCGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedX = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedY = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    s32 fixedZ = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));

    s32 flags;
    if (vmArgOmitChk(pThread, 5)) {
        flags = 0;
    } else {
        flags = vmArgIntGet(6, vmArgPtrGet(pThread, 5));
    }

    ml::CVec3 pos;
    pos.x = (f32)fixedX / lbl_eu_80666168;
    pos.y = (f32)fixedY / lbl_eu_80666168;
    pos.z = (f32)fixedZ / lbl_eu_80666168;

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    void* obj = func_801862C0();
    void* slot = func_801864DC(obj, oc->field_0x04);
    cam->vfunc_0x68(slot, &pos, -flags);
    return 0;
}

extern "C" int setLookatOfs(VMThread* pThread) {
    CamOCHandle* oc = (CamOCHandle*)vmArgOCGet(2, vmArgPtrGet(pThread, 1));
    s32 fixedX = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    s32 fixedY = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    s32 fixedZ = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));

    s32 flags;
    if (vmArgOmitChk(pThread, 5)) {
        flags = 0;
    } else {
        flags = vmArgIntGet(6, vmArgPtrGet(pThread, 5));
    }

    ml::CVec3 lookat;
    lookat.x = (f32)fixedX / lbl_eu_80666168;
    lookat.y = (f32)fixedY / lbl_eu_80666168;
    lookat.z = (f32)fixedZ / lbl_eu_80666168;

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    void* obj = func_801862C0();
    void* slot = func_801864DC(obj, oc->field_0x04);
    cam->vfunc_0x6C(slot, &lookat, -flags);
    func_8016FD84(lbl_eu_80666178, lbl_eu_8066617C);
    return 0;
}

extern "C" int getPos(VMThread* pThread) {
    s32 index = vmArgIntGet(2, vmArgPtrGet(pThread, 1));

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    CamStateObj* camObj = cam->objPtr;

    VMArg ret;

    if (index == 0) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = floatToFixed(camObj->field_0x10C);
    } else if (index == 1) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = floatToFixed(camObj->field_0x110);
    } else if (index == 2) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = floatToFixed(camObj->field_0x114);
    }

    vmRetValSet(pThread, &ret);
    return 1;
}

extern "C" int getRot(VMThread* pThread) {
    s32 index = vmArgIntGet(2, vmArgPtrGet(pThread, 1));

    UnkCamIntf* cam = func_800821F8__Q22cf13CfGameManagerFv();
    CamStateObj* camObj = cam->objPtr;

    VMArg ret;

    if (index == 0) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = radToDegFixed(camObj->field_0x118);
    } else if (index == 1) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = radToDegFixed(camObj->field_0x11C);
    } else if (index == 2) {
        ret.type = VM_TYPE_FIXED;
        ret.value.intVal = radToDegFixed(camObj->field_0x120);
    }

    vmRetValSet(pThread, &ret);
    return 1;
}

extern "C" int keyBegin(VMThread* pThread) {
    // Optional-arg chain state: declared first so MWCC assigns the callee-saved
    // registers the same way as retail (nextArg=r31, third=r30, fourth=r29,
    // first=r28, second=r27, pThread=r26).
    s32 nextArg = 3;
    s32 third;
    s32 fourth;
    VMArg* arg;
    s32 first = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    s32 second = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    if (vmArgOmitChk(pThread, nextArg)) {
        third = 0;
        nextArg = 4;
    } else {
        arg = vmArgPtrGet(pThread, nextArg++);
        third = vmArgIntGet(nextArg, arg);
    }

    if (vmArgOmitChk(pThread, nextArg)) {
        fourth = 0;
        nextArg++;
    } else {
        arg = vmArgPtrGet(pThread, nextArg++);
        fourth = vmArgIntGet(nextArg, arg);
    }

    CamOCHandle* oc;
    if (vmArgOmitChk(pThread, nextArg)) {
        oc = nullptr;
    } else {
        arg = vmArgPtrGet(pThread, nextArg++);
        oc = (CamOCHandle*)vmArgOCGet(nextArg, arg);
    }

    // Resolve the OC instance to a camera key id (0 when omitted/invalid).
    u32 ocId = 0;
    if (oc != nullptr) {
        ocId = (u32)func_801864DC(func_801862C0(), oc->field_0x04);
    }

    func_80082008__Q22cf13CfGameManagerFv(
        first, second, third, (u32)fourth & 0xFFFF, ocId);
    return 0;
}

extern "C" int keyEnd(VMThread* pThread) {
    func_80082060__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" int keyAdd(VMThread* pThread) {
    // Callee-saved allocation follows declaration order descending from r31:
    // fixed6=r31, fixed5=r30, fixed4=r29, fixed3=r28, fixed2=r27, fixed1=r26,
    // first=r25, seventh=r24, nextArg=r23, pThread=r22.
    s32 extra;
    s32 fixed6;
    s32 fixed5;
    s32 fixed4;
    s32 fixed3;
    s32 fixed2;
    s32 fixed1;
    s32 first = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    fixed1 = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    fixed2 = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    fixed3 = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    fixed4 = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    fixed5 = vmArgFixedGet(7, vmArgPtrGet(pThread, 6));
    fixed6 = vmArgFixedGet(8, vmArgPtrGet(pThread, 7));

    // Optional-arg chain: nextArg carries the running index (retail r23), so
    // the post-increment form reproduces the `li r4, N; li r23, N+1; bl` order.
    s32 seventh;
    s32 nextArg = 8;
    if (vmArgOmitChk(pThread, nextArg)) {
        seventh = 0;
        nextArg = 9;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, nextArg++);
        seventh = vmArgFixedGet(nextArg, arg);
    }

    if (vmArgOmitChk(pThread, nextArg)) {
        extra = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, nextArg++);
        extra = vmArgIntGet(nextArg, arg);
    }
    u32 extraArg = (u32)extra & 0xFFFF;

    // Key-frame: pos (fixed1-3), lookat (fixed4-6) and an optional 7th
    // fixed value passed as the trailing float. lookat declared first so it
    // gets the lower stack slot (retail lookat@+8, pos@+0x14).
    struct {
        ml::CVec3 lookat;
        ml::CVec3 pos;
    } cam;
    cam.pos.x = (f32)fixed1 / lbl_eu_80666168;
    cam.pos.y = (f32)fixed2 / lbl_eu_80666168;
    cam.pos.z = (f32)fixed3 / lbl_eu_80666168;
    cam.lookat.x = (f32)fixed4 / lbl_eu_80666168;
    cam.lookat.y = (f32)fixed5 / lbl_eu_80666168;
    cam.lookat.z = (f32)fixed6 / lbl_eu_80666168;

    func_80082088__Q22cf13CfGameManagerFv(
        (u32)first & 0xFFFF, &cam.pos, &cam.lookat, extraArg,
        (f32)seventh / lbl_eu_80666168);
    return 0;
}

extern "C" int isKeyMove(VMThread* pThread) {
    // Retail stores only the VMArg type byte: VM_TYPE_TRUE (1) when the key
    // is moving, VM_TYPE_FALSE (2) otherwise. `!x + 1` reproduces the
    // branchless cntlzw/srwi/addi idiom.
    VMArg ret;
    ret.type = !func_80082104__Q22cf13CfGameManagerFv() + 1;
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
    // Args 2..0xE: intensity (int), 11 fixed-point values (fixed1..11), then a
    // bool flag. The data block is 13 floats: data[0] is the raw int,
    // data[1..6]/data[8..12] are the fixed values /4096, and data[7] is 0.0f
    // (the gap between the two fixed groups). The bool is passed separately as
    // the second arg, booleanized via the neg/or/srwi idiom.
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
    s32 boolParam = vmArgBoolGet(14, vmArgPtrGet(pThread, 13));

    f32 data[13];
    memset(data, 0, sizeof(data));

    data[0] = (f32)intParam;
    data[1] = (f32)fixed1 / lbl_eu_80666168;
    data[2] = (f32)fixed2 / lbl_eu_80666168;
    data[3] = (f32)fixed3 / lbl_eu_80666168;
    data[4] = (f32)fixed4 / lbl_eu_80666168;
    data[5] = (f32)fixed5 / lbl_eu_80666168;
    data[6] = (f32)fixed6 / lbl_eu_80666168;
    data[7] = lbl_eu_80666178; // 0.0f
    data[8] = (f32)fixed7 / lbl_eu_80666168;
    data[9] = (f32)fixed8 / lbl_eu_80666168;
    data[10] = (f32)fixed9 / lbl_eu_80666168;
    data[11] = (f32)fixed10 / lbl_eu_80666168;
    data[12] = (f32)fixed11 / lbl_eu_80666168;

    func_8007B044(data, (u32)(-boolParam | boolParam) >> 31);
    return 0;
}

extern "C" int stopShake(VMThread* pThread) {
    func_8007B078(0);
    return 0;
}

void pluginCamRegist() {
    vmPluginRegist(lbl_eu_804FB1A4, lbl_eu_80526560);
}
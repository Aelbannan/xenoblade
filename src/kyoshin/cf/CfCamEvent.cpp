#include "kyoshin/cf/CfCamEvent.hpp"

#include "kyoshin/cf/CfGameManagerData.hpp"
#include <string.h>
#include <math.h>

extern "C" {
s8 lbl_eu_80663DB0;
char* lbl_eu_80663DB4;
s8 lbl_eu_80663DB8;
char* lbl_eu_80663DBC;
s8 lbl_eu_80663DC0;
char* lbl_eu_80663DC4;

#pragma push
#pragma section data_type ".data" ".data"
#pragma push
#pragma section data_type ".data" ".data"
extern "C" const volatile unsigned char lbl_eu_80527130[];
extern "C" const volatile unsigned char lbl_eu_80527108[];
extern "C" const volatile unsigned char lbl_eu_805270D8[];
extern "C" const volatile unsigned char lbl_eu_805270C0[];
extern "C" const volatile unsigned char lbl_eu_805270F0[];
extern "C" const volatile unsigned char lbl_eu_80527120[];
extern "C" const volatile unsigned char lbl_eu_80527148[];
extern "C" const volatile unsigned char lbl_eu_80527154[];
extern "C" const volatile unsigned char lbl_eu_805271FC[];
extern "C" const volatile unsigned char lbl_eu_80527048[];
#pragma pop
}

char** cf::CfCamEvent::initCamIntfInstances() {
    if (!lbl_eu_80663DB0) {
        lbl_eu_80663DB4 = (char*)lbl_eu_80527130;
        lbl_eu_80663DB0 = 1;
    }
    if (!lbl_eu_80663DB8) {
        lbl_eu_80663DBC = (char*)lbl_eu_80527108;
        lbl_eu_80663DB8 = 1;
    }
    if (!lbl_eu_80663DC0) {
        lbl_eu_80663DC4 = (char*)lbl_eu_805270D8;
        lbl_eu_80663DC0 = 1;
    }

    s32 type = unk8;
    char** result = &lbl_eu_80663DBC;
    if (type == 8) {
        return &lbl_eu_80663DBC;
    }
    if (type == 9) {
        return &lbl_eu_80663DB4;
    }
    if (type == 10) {
        return &lbl_eu_80663DC4;
    }
    return result;
}

extern "C" void func_8006ACB0(u8* self, float value) {
    *(float*)((uintptr_t)self + 0x280) = value;
}
extern "C" float func_8006ACB8(u8* self) {
    return *(float*)((char*)self + 0x280);
}

using namespace cf;

// Refresh the camera vectors: zero the direction/lookat/pos block, adopt the
// source position and lookat, recompute the direction via func_800749AC, and
// capture the resulting distance into the speed factor.
void func_8006A82C(CfCamEvent* self) {
    CfCamBody25C* body = (CfCamBody25C*)&self->unk25C;
    memset(body, 0, 0x34);
    body->pos = self->unk34;
    body->lookat = self->unk10;
    body->unk24 = self->field_0x1E0;
    body->unk28 = func_800749AC(&self->unk34, &self->unk10, &body->dir);
    body->dir.z = lbl_eu_80666268;
}

// Select a cam type id, then refresh the direction vector from the camera's
// v_05 accessor (which returns the current direction).
void func_8006A90C(CfCamEvent* self, s32 type) {
    self->unk8 = type;
    self->unk25C = *self->virt05();
}

// Adopt a new lookat target, then hand it to the interface's setLookat.
void func_8006AA84(CfCamEvent* self, ml::CVec3* lookat) {
    self->mLookat = *lookat;
    CfCamDispatch* intf = CfCamEvent_initCamIntfInstances(self);
    intf->setLookat(self, lookat);
}

// Set the direction vector's x component, then forward it to setDir.
void func_8006AAEC(CfCamEvent* self, f32 value) {
    ml::CVec3* dir = &self->unk25C;
    dir->x = value;
    CfCamDispatch* intf = CfCamEvent_initCamIntfInstances(self);
    intf->setDir(self, dir);
}

// Set the direction vector's y component, then forward it to setDir.
void func_8006AB40(CfCamEvent* self, f32 value) {
    ml::CVec3* dir = &self->unk25C;
    dir->y = value;
    CfCamDispatch* intf = CfCamEvent_initCamIntfInstances(self);
    intf->setDir(self, dir);
}

// Reset the speed factor, clear the 0x1200 flag bit, adopt the direction
// vector, then forward it to setDir.
void func_8006AB94(CfCamEvent* self, ml::CVec3* vec) {
    self->unk288 = lbl_eu_80666268;
    func_8006BC1C(self, 0x1200);
    self->unk25C = *vec;
    CfCamDispatch* intf = CfCamEvent_initCamIntfInstances(self);
    intf->setDir(self, vec);
}

// Recompute the lookat point from the base position, then hand back the
// direction vector (used as the camera "get direction" entry).
ml::CVec3* func_8006A9F8(CfCamEvent* self) {
    ml::CVec3* out = &self->unk25C;
    func_800749AC((ml::CVec3*)((u8*)out + 0x18), (ml::CVec3*)((u8*)out + 0xC), out);
    return out;
}

// Reset the camera event state: run the virtual reset hook, then restore the
// default factor and clear the flag word.
void func_8006A8C4(CfCamEvent* self) {
    self->reset();
    self->field_0x238 = lbl_eu_80666268;
    self->field_0x1D4 = 0;
}

// Dispatch a "set position" request to the interface selected by the cam type.
void func_8006AA34(CfCamEvent* self, ml::CVec3* pos) {
    CfCamDispatch* intf = CfCamEvent_initCamIntfInstances(self);
    intf->setPos(self, pos);
}

// Conditional "set follow" dispatch: build the follow parameter block and
// hand it to the follow virtual.
void func_8006AC10(CfCamEvent* self, s32 cond) {
    if (cond != 0) {
        ml::CVec3 local;
        func_80074090(&local);
        self->dispatchFollow(&local);
    }
}

// Same shape as func_8006AC10 but dispatched through a later vtable slot.
void func_8006AC60(CfCamEvent* self, s32 cond) {
    if (cond != 0) {
        ml::CVec3 local;
        func_80074090(&local);
        self->dispatchFollow64(&local);
    }
}

// Factory constructor for the camera event state: allocate a 0x298-byte
// block, run the CfCamFollow base constructor on it, install the CfCamEvent
// vtable, then attach the event object returned by the camera manager.
extern "C" CfCamEvent* __ct__8006B310(void* self, void* arg2) {
    u32 heap = func_80061FE8();
    CfCamEvent* obj = (CfCamEvent*)allocate__Q23mtl10MemManagerFUlUl(0x298, heap);
    if (obj != nullptr) {
        __ct__cf_CfCamFollow(obj, self, arg2);
        ((CfCamFollowSlot*)obj)->table = (void*)lbl_eu_80527048;
        obj->field_0x290 = 0;
        obj->field_0x294 = 0;
        obj->unk8 = 8;
        obj->unk25C = ml::CVec3::zero;
        obj->virt16(getActiveCameraObject__Q22cf13CfGameManagerFv());
        obj->reset();
        cleanupMapEffects__Q22cf13CfGameManagerFv();
    }
    return obj;
}

// Per-frame camera event update: refresh the aim/position vectors, run the
// follow hook, recompute the lookat from the source block, and gate a node
// flush on the angle trigger countdown.
void func_8006ACC0(CfCamEvent* self) {
    // All locals declared at function top: MWCC's allocation order for this
    // function matches the original TU's C-style declaration layout.
    ml::CVec3 aim;
    ml::CVec3 pos;
    ml::CVec3 diff;
    ml::CVec3 probe1;
    ml::CVec3 probe2;
    CfCamBlock48 srcBlock;
    CfCamNodeList* nodeList;
    CfCamNode* node;
    CfCamNodePayload* payload;
    void* o164;
    int follow;
    u8 flagByte;
    u32 evtFlags;

    stubEmptyE__Q22cf13CfGameManagerFv(0x2000);

    // Paused/cutscene gate: when flagged (or the manager state matches), run
    // the follow dispatch once and bail out.
    if ((self->unk4 & 0x04000000) != 0 || getNullPtrC__Q22cf13CfGameManagerFv(8) != 0) {
        if ((self->unk4 & 0x04000000) != 0) {
            self->virt22();
            func_80071B78(self);
            return;
        }
    }

    if (CfRes_getD80Flag() == 0 || func_80496288() == lbl_eu_80666268) {
        return;
    }

    // Refresh the position/aim copies and recompute the error-checked
    // direction vector from the last lookat target.
    self->unk28 = self->unk274;
    aim = self->unk28;
    pos = self->mLookat;
    diff = pos - self->unk58;
    self->unk18C = diff;
    if (self->unk18C.isErr()) {
        self->unk18C = ml::CVec3::zero;
    }

    o164 = self->unk164;
    if (func_800B8920(o164) == 0) {
        o164 = 0;
    }
    follow = 0;
    flagByte = self->field_0x294;
    if ((u32)self->unk8 >= 0x10 && (u32)self->unk8 <= 0x2B) {
        follow = 1;
    }
    evtFlags = lbl_eu_80663E24;
    if ((evtFlags & 0x00400000) != 0 || (evtFlags & 0x00040000) != 0) {
        flagByte = 0;
        follow = 0;
    } else if (isTimerActive__Q22cf13CfGameManagerFv() != 0) {
        follow = 1;
    }

    if (flagByte != 0 || follow != 0) {
        func_8006D8D0(self, func_800BBC0C(o164));
        if (follow != 0) {
            // Follow path: keep the aim point on-screen by clamping it
            // against the ground-probe results, then forward to the
            // follow hook.
            if (func_804BE398(&pos, 0, 0x04000000, 0, lbl_eu_8066626C,
                              lbl_eu_80666268) != 0) {
                func_804BE4B4(&probe1, 0);
                if (pos.y < probe1.y + lbl_eu_80661B58) {
                    pos.y = probe1.y + lbl_eu_80661B58;
                }
            }
            ml::CVec3 probe2_unused;
            if (func_804BE398(&aim, 0, 0x04000000, 0, lbl_eu_80666270,
                              lbl_eu_80666268) != 0) {
                func_804BE4B4(&probe2, 0);
                if (aim.y < lbl_eu_80666274 + probe2.y) {
                    aim.y = lbl_eu_80666274 + probe2.y;
                }
            }
            func_800707C0(self, &aim, &pos);
        } else {
            // Non-follow path: pull the aim point part-way toward the
            // camera position.
            if (func_80073640(self, &self->mLookat, &aim, &self->unk34, 0) != 0) {
                diff = aim - self->unk34;
                probe1 = diff * lbl_eu_80666278;
                aim = self->unk34 + probe1;
                self->field_0x1DC = lbl_eu_80666268;
            }
        }
    }

    // Copy the source block, run the camera-position hook, then compute
    // the yaw angle from the direction vector and gate the node flush on
    // the countdown trigger.
    srcBlock = self->unk160->block;
    func_80071AB0(self, &aim, &pos, 0, self->unk280, self->unk25C.z);
    self->unk34 = aim;
    self->unk130 = self->unk160->block;
    self->unk58 = pos;
    self->unk40 = pos;
    if (follow == 0) {
        self->unk64 = pos;
    }
    f32 ang = ml::CVec3::dot(srcBlock.dir, self->unk160->block.dir);
    if (ang < lbl_eu_8066627C) {
        ang = lbl_eu_8066627C;
    } else if (ang > lbl_eu_80666280) {
        ang = lbl_eu_80666280;
    }
    if (!(ang <= lbl_eu_80666280 && ang >= lbl_eu_8066627C)) {
        Warning__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_805262F0, 0xEF,
                                     (const char*)lbl_eu_805262C8);
    }
    // acos result is narrowed to f32 before the scaling multiply.
    if ((f32)acos(ang) * lbl_eu_8066A20C < lbl_eu_80666284) {
        self->field_0x290 = 1;
    }
    if (self->field_0x290 > 0 && --self->field_0x290 == 0) {
        // Flush the camera event nodes once the trigger countdown hits 0.
        nodeList = getFirstGimmick__Q22cf13CfGameManagerFv();
        node = nodeList->sentinel->next;
        while (node != getFirstGimmick__Q22cf13CfGameManagerFv()->sentinel) {
            payload = (CfCamNodePayload*)node->field_8;
            if (payload->field_98 != 0) {
                if ((payload->field_68 & 0x8000) == 0) {
                    func_804876DC();
                }
                func_80484E04(payload->field_98, 1);
                func_804876C0(payload->field_98);
            }
            node = node->next;
        }
    }
}

// fixed absorb for CfCamEvent - typed arrays (rodata strings + data/sdata)
extern "C" {
void func_80071B74();
void func_80073C74();
void func_80073DDC();
void func_80074A3C();
void func_80074AA4();
extern const char lbl_eu_804FB470[];
extern const char lbl_eu_804FB480[];
extern const char lbl_eu_804FB494[];
extern const char lbl_eu_804FB4A8[];
extern const char lbl_eu_804FB4C0[];
void CfCamTargetIntf_update();
void CfCamTargetIntf_setPos();
void CfCamTargetIntf_setDir();
void CfCamTargetIntf_setLookat();
void CamLookatIntf_update();
void CamLookatIntf_setPos();
void CamLookatIntf_setDir();
void CamLookatIntf_setLookat();
void CfCamDirectionIntf_update();
void CfCamDirectionIntf_setPos();
void CfCamDirectionIntf_setDir();
void CfCamDirectionIntf_setLookat();
extern char lbl_eu_80661B28[];
extern char lbl_eu_80661B30[];
extern char lbl_eu_80666288[];
extern char lbl_eu_80666290[];
extern char lbl_eu_80666298[];
extern const void* lbl_eu_80661B00[];
extern const void* lbl_eu_80661B08[];
extern const void* lbl_eu_80661B10[];
extern const void* lbl_eu_80661B18[];
extern const void* lbl_eu_80661B20[];
}



















__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) const void* lbl_eu_80661B00[2] = {
    (void*)lbl_eu_804FB470,
    (void*)lbl_eu_805270C0
};

__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) const void* lbl_eu_80661B08[2] = {
    (void*)lbl_eu_804FB480,
    (void*)lbl_eu_805270F0
};

__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) const void* lbl_eu_80661B10[2] = {
    (void*)lbl_eu_804FB494,
    (void*)lbl_eu_80527120
};

__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) const void* lbl_eu_80661B18[2] = {
    (void*)lbl_eu_804FB4A8,
    (void*)lbl_eu_80527148
};

__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) const void* lbl_eu_80661B20[2] = {
    (void*)lbl_eu_804FB4C0,
    0
};

__declspec(section ".rodata") __attribute__((aligned(1))) __attribute__((used)) const char lbl_eu_804FB470[] = "cf::CfCamEvent";
__declspec(section ".rodata") __attribute__((aligned(1))) __attribute__((used)) const char lbl_eu_804FB480[] = "cf::CfCamTargetIntf";
__declspec(section ".rodata") __attribute__((aligned(1))) __attribute__((used)) const char lbl_eu_804FB494[] = "cf::CfCamLookatIntf";
__declspec(section ".rodata") __attribute__((aligned(1))) __attribute__((used)) const char lbl_eu_804FB4A8[] = "cf::CfCamDirectionIntf";
__declspec(section ".rodata") __attribute__((aligned(1))) __attribute__((used)) const char lbl_eu_804FB4C0[] = "cf::CfCamIntf";






// Force .data to be file-backed with 0x1C0 zeros (retail .data is 0x1C0 with zeros for first part and 3F80... for float table)
// The actual data objects (lbl_eu_80527*) are currently in bss with zeros, but we need data with 0x1C0
// Add a single pad object in .data with 0x1C1 bytes (last byte 0x01 to force D), then drop the last byte via UNIT_RULES



#pragma push
#pragma section data_type ".data" ".data"
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char lbl_eu_80527048[0x78] = {0};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char lbl_eu_805270C0[0x18] = {0};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char lbl_eu_805270D8[0x18] = {0};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char lbl_eu_805270F0[0x18] = {0};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char lbl_eu_80527108[0x18] = {0};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char lbl_eu_80527120[0x10] = {0};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char lbl_eu_80527130[0x18] = {0};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(4))) const volatile unsigned char lbl_eu_80527148[0x0C] = {0};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(4))) const volatile unsigned char lbl_eu_80527154[0x0C] = {0};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const unsigned int lbl_eu_80527160[39] = {
    0x3F800000,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0x3FC00000,
    0x3F800000,
    0x3E4CCCCD,
    0x3F800000,
    0x3F800000,
    0x40000000,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0x3FC00000,
    0,
    0x3F800000,
    0x3F000000,
    0x3F000000,
    0x40000000,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0x3F800000,
    0,
    0x3F000000,
    0x3F000000,
    0x3F000000
};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(4))) const volatile unsigned char lbl_eu_805271FC[0x0C] = {0};
#pragma pop

__declspec(section ".sdata2") __attribute__((aligned(8))) __attribute__((used)) const unsigned char __absorb_kyoshin_cf_CfCamEvent_sdata2[0x34] = {
    0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00,
    0x3D, 0xCC, 0xCC, 0xCD, 0x3F, 0x00, 0x00, 0x00, 0xBF, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x40, 0xA0, 0x00, 0x00, 0x48, 0x69, 0x67, 0x68,
    0x00, 0x00, 0x00, 0x00, 0x4E, 0x6F, 0x72, 0x6D, 0x61, 0x6C, 0x00, 0x00,
    0x4C, 0x6F, 0x77, 0x00
};

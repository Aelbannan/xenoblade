#include "kyoshin/cf/CfCamEvent.hpp"
#include "monolib/scn/CScnTimeApi.hpp"

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

extern char lbl_eu_80527130[];
extern char lbl_eu_80527108[];
extern char lbl_eu_805270D8[];
}

char** cf::CfCamEvent::initCamIntfInstances() {
    if (!lbl_eu_80663DB0) {
        lbl_eu_80663DB4 = lbl_eu_80527130;
        lbl_eu_80663DB0 = 1;
    }
    if (!lbl_eu_80663DB8) {
        lbl_eu_80663DBC = lbl_eu_80527108;
        lbl_eu_80663DB8 = 1;
    }
    if (!lbl_eu_80663DC0) {
        lbl_eu_80663DC4 = lbl_eu_805270D8;
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
    self->unk25C = *self->v_05();
}

// Adopt a new lookat target, then hand it to the interface's setLookat.
void func_8006AA84(CfCamEvent* self, ml::CVec3* lookat) {
    self->mLookat = *lookat;
    CfCamIntf* intf = CfCamEvent_initCamIntfInstances(self);
    intf->fn_0x14(self, lookat);
}

// Set the direction vector's x component, then forward it to setDir.
void func_8006AAEC(CfCamEvent* self, f32 value) {
    ml::CVec3* dir = &self->unk25C;
    dir->x = value;
    CfCamIntf* intf = CfCamEvent_initCamIntfInstances(self);
    intf->fn_0x10(self, dir);
}

// Set the direction vector's y component, then forward it to setDir.
void func_8006AB40(CfCamEvent* self, f32 value) {
    ml::CVec3* dir = &self->unk25C;
    dir->y = value;
    CfCamIntf* intf = CfCamEvent_initCamIntfInstances(self);
    intf->fn_0x10(self, dir);
}

// Reset the speed factor, clear the 0x1200 flag bit, adopt the direction
// vector, then forward it to setDir.
void func_8006AB94(CfCamEvent* self, ml::CVec3* vec) {
    self->unk288 = lbl_eu_80666268;
    func_8006BC1C(self, 0x1200);
    self->unk25C = *vec;
    CfCamIntf* intf = CfCamEvent_initCamIntfInstances(self);
    intf->fn_0x10(self, vec);
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
    self->fn_0x28();
    self->field_0x238 = lbl_eu_80666268;
    self->field_0x1D4 = 0;
}

// Dispatch a "set position" request to the interface selected by the cam type.
void func_8006AA34(CfCamEvent* self, ml::CVec3* pos) {
    CfCamIntf* intf = CfCamEvent_initCamIntfInstances(self);
    intf->fn_0x0C(self, pos);
}

// Conditional "set follow" dispatch: build the follow parameter block and
// hand it to the follow virtual.
void func_8006AC10(CfCamEvent* self, s32 cond) {
    if (cond != 0) {
        ml::CVec3 local;
        func_80074090(&local);
        self->fn_0x14(&local);
    }
}

// Same shape as func_8006AC10 but dispatched through a later vtable slot.
void func_8006AC60(CfCamEvent* self, s32 cond) {
    if (cond != 0) {
        ml::CVec3 local;
        func_80074090(&local);
        self->fn_0x64(&local);
    }
}

// Factory constructor for the camera event state: allocate a 0x298-byte
// block, run the CfCamFollow base constructor on it, install the CfCamEvent
// vtable, then attach the event object returned by the camera manager.
extern "C" CfCamEvent* __ct__8006B310(void* self, void* arg2) {
    u32 heap = func_80061FE8();
    CfCamEvent* obj = (CfCamEvent*)allocate__Q23mtl10MemManagerFUlUl(0x298, heap);
    if (obj != nullptr) {
        __ct__cf_CfCamFollow((CfCamFollow*)obj, self, arg2);
        ((CfCamFollow*)obj)->vtable = lbl_eu_80527048;
        obj->field_0x290 = 0;
        obj->field_0x294 = 0;
        obj->unk8 = 8;
        obj->unk25C = ml::CVec3::zero;
        obj->v_16(func_8008221C__Q22cf13CfGameManagerFv());
        obj->fn_0x28();
        func_80085878__Q22cf13CfGameManagerFv();
    }
    return obj;
}

// Per-frame camera event update: refresh the aim/position vectors, run the
// follow hook, recompute the lookat from the source block, and gate a node
// flush on the angle trigger countdown.
void func_8006ACC0(CfCamEvent* self) {
    func_8007FE20__Q22cf13CfGameManagerFv(0x2000);

    if ((self->unk4 & 0x04000000) != 0 || func_8007FE24__Q22cf13CfGameManagerFv(8) != 0) {
        if ((self->unk4 & 0x04000000) != 0) {
            self->v_20();
            func_80071B78(self);
            return;
        }
    }

    if (CfRes_getD80Flag() != 0 && func_80496288(lbl_eu_80663E14) != lbl_eu_80666268) {
        // Refresh the position/aim copies and recompute the error-checked
        // direction vector from the last lookat target.
        self->unk28 = self->unk274;
        ml::CVec3 aim = self->unk28;
        ml::CVec3 pos = self->mLookat;
        ml::CVec3 diff = pos - self->unk58;
        self->unk18C = diff;
        if (self->unk18C.isErr()) {
            self->unk18C = ml::CVec3::zero;
        }

        void* o164 = self->unk164;
        if (func_800B8920(o164) == 0) {
            o164 = 0;
        }
        int r31 = 0;
        u8 r25 = self->field_0x294;
        if ((u32)self->unk8 >= 0x10 && (u32)self->unk8 <= 0x2B) {
            r31 = 1;
        }
        u32 m = lbl_eu_80663E24;
        if ((m & 0x00400000) != 0 || (m & 0x00040000) != 0) {
            r25 = 0;
            r31 = 0;
        } else if (func_8007F91C__Q22cf13CfGameManagerFv() != 0) {
            r31 = 1;
        }

        if (r25 != 0 || r31 != 0) {
            func_8006D8D0(self, func_800BBC0C(o164));
            if (r31 != 0) {
                // Follow path: keep the aim point on-screen by clamping it
                // against the ground-probe results, then forward to the
                // follow hook.
                ml::CVec3 probe1;
                if (func_804BE398(&pos, 0, 0x04000000, 0, lbl_eu_8066626C,
                                  lbl_eu_80666268) != 0) {
                    func_804BE4B4(&probe1, 0);
                    if (pos.y < probe1.y + lbl_eu_80661B58) {
                        pos.y = probe1.y + lbl_eu_80661B58;
                    }
                }
                ml::CVec3 probe2;
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
                    ml::CVec3 subTmp, subOut;
                    nw4r::math::VEC3Sub(subTmp, aim, self->unk34);
                    subOut.set(subTmp);
                    ml::CVec3 scaleTmp, scaleOut;
                    nw4r::math::VEC3Scale(scaleTmp, subOut, lbl_eu_80666278);
                    scaleOut.set(scaleTmp);
                    ml::CVec3 addTmp, addOut;
                    nw4r::math::VEC3Add(addTmp, self->unk34, scaleOut);
                    addOut.set(addTmp);
                    aim = addOut;
                    self->field_0x1DC = lbl_eu_80666268;
                }
            }
        }

        // Copy the source block, run the camera-position hook, then compute
        // the yaw angle from the direction vector and gate the node flush on
        // the countdown trigger.
        CfCamSrc160* o160 = self->unk160;
        CfCamBlock48 tmp = o160->block;
        func_80071AB0(self, &aim, &pos, 0, self->unk280, self->unk25C.z);
        self->unk34 = aim;
        self->unk130 = o160->block;
        self->unk58 = pos;
        self->unk40 = pos;
        if (r31 == 0) {
            self->unk64 = pos;
        }
        f32 ang = ml::CVec3::dot(tmp.dir, o160->block.dir);
        if (ang < lbl_eu_8066627C) {
            ang = lbl_eu_8066627C;
        } else if (ang > lbl_eu_80666280) {
            ang = lbl_eu_80666280;
        }
        if (!(ang <= lbl_eu_80666280 && ang >= lbl_eu_8066627C)) {
            Warning__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_805262F0, 0xEF,
                                         (const char*)lbl_eu_805262C8);
        }
        if (acos(ang) * lbl_eu_8066A20C < lbl_eu_80666284) {
            self->field_0x290 = 1;
        }
        if (self->field_0x290 > 0 && --self->field_0x290 == 0) {
            // Flush the camera event nodes once the trigger countdown hits 0.
            for (CfCamNode* node =
                     ((CfCamNodeList*)func_80086B0C__Q22cf13CfGameManagerFv())->sentinel->next;
                 node !=
                     ((CfCamNodeList*)func_80086B0C__Q22cf13CfGameManagerFv())->sentinel;
                 node = node->next) {
                CfCamNodePayload* p = (CfCamNodePayload*)node->field_8;
                if (p->field_98 != 0) {
                    if ((p->field_68 & 0x8000) == 0) {
                        func_804876DC();
                    }
                    func_80484E04(p->field_98, 1);
                    func_804876C0(p->field_98);
                }
            }
        }
    }
}

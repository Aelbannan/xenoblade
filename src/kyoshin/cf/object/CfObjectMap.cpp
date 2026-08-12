// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectMap.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/math/CVec3.hpp"

// Retail data + dtor imports for the D1 (the header declares the virtual
// dtor; the symbol is provided by the freestanding definition below).
extern "C" { extern char lbl_eu_80529128[]; }
extern "C" void __dt__8047BDA8(void* self);
extern "C" void __dt__Q22cf13CfObjectModelFv(void* self, int flag);

struct CMIf {
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
    virtual void _v00C8();
    virtual void _v00CC();
    virtual void vf00D0();
};

// Calls vtable slot 0x18C (CfObjectModel_UnkVirtualFunc6) when the pointer at
// +0x70 is set; returns the call's result, or 1 when the pointer is NULL.
// Real-member form gives retail's r12 virtual dispatch. Open item: the
// retval-default register (li r0,1 + or r0,r3,r3 / or r3,r0,r0 in retail vs
// li r4/r5 + or r4 + lwz-r0-first epilogue here) is allocator-fixed across
// ~16 shapes (decl order, if/else, early-return, ternary, pointer local,
// named member, -O4,s, volatile); the order diffs at +8/+0xc and +0x30/+0x34
// are consequences of the r0-vs-r4 choice.
// Calls vtable slot 0x18C (CfObjectModel_UnkVirtualFunc6) when the pointer at
// +0x70 is set; returns the call's result, or 1 when the pointer is NULL.
// Real-member form gives retail's r12 virtual dispatch. Open item: the
// retval-default register (li r0,1 + or r0,r3,r3 / or r3,r0,r0 in retail vs
// li r4/r5 + or r4 + lwz-r0-first epilogue here) is allocator-fixed across
// ~16 shapes (decl order, if/else, early-return, ternary, pointer local,
// named member, -O4,s, volatile); the order diffs at +8/+0xc and +0x30/+0x34
// are consequences of the r0-vs-r4 choice.
void* cf::CfObjectMap::func_800B9A70() {
    void* ret = (void*)1;
    if (this->mTarget70) {
        ret = this->CfObjectModel_UnkVirtualFunc6();
    }
    return ret;
}

void cf::CfObjectMap::func_800B9AB4() {
    this->CfObjectModel_UnkVirtualFunc1();
    if (this->field_0xEC != 0) {
        func_80495E60(this->field_0xEC);
        this->field_0xEC = 0;
    }
    func_8047CFD0__17UnkClass_8047CD0CFv(&this->field_0x2F2C);
    if (this->field_0xE0 != 0) {
        this->field_0xE0 = 0;
        CDeviceVI::waitForDrawDone();
        func_8047BD9C__17UnkClass_8047BB54Fv(&this->field_0xF0);
        *reinterpret_cast<u32*>(this->_pad6C) &= ~0x8000;
    }
    if (this->field_0x2F40 != 0) {
        func_80462AC0__8CTaskLODFv(0);
        this->field_0x2F40 = 0;
    }
    void* resource = this->field_0x2F3C;
    if (resource != 0) {
        func_804C1D7C(reinterpret_cast<UnkSceneView*>(lbl_eu_80663E14)->field_0x7C, resource);
        this->field_0x2F3C = 0;
    }
    func_801A2C94__Q22cf12CTaskCullingFv();
}

void cf::CfObjectMap::func_800B9B78() {}

void func_800B9C14(){}

void func_800B9C64__Q22cf11CfObjectMapFv(void* self) {
    *(unsigned short*)((char*)self + 0x8e) = 0;
}

void func_800B9C70() {}

extern "C" int func_800B9C74(cf::CfObjectMap* self, u32 a, u32 b) {
    self->CfObjectModel_UnkVirtualFunc1();
    if (self->field_0xEC != 0) {
        func_80495E60(self->field_0xEC);
        self->field_0xEC = 0;
    }
    void* handle = func_800624A8(a, b, 3);
    if (handle != 0) {
        self->field_0x2F38 = func_800AA2BC(a, b);
        self->mFlags68 &= 0x40000000;
        self->field_0x8E = 1;
        self->mTarget70 = handle;
    }
    u32 packed = self->field_0x2F38;
    if (packed != 0) {
        char buf[0x40];
        u32 len = 0;
        buf[0] = 0;
        int field1 = (packed >> 5) & 0x7F;
        int field2 = (packed >> 12) & 0x3FF;
        func_800AA33C(buf, packed, 1, 0);
        strcat(buf, lbl_eu_804FC538);
        len += strlen(lbl_eu_804FC538);
        int size = getFileSize__11CDeviceFileFPCc(buf, 1);
        if (size > 0) {
            u32 rounded = (size + 0x7FF) & ~0x7FF;
            UnkRes866A0* res = func_80062FF0();
            if (res->field_0x2C->vfunc_0x44(res) != 0 && func_800A7FBC() != 0) {
                self->field_0xE4 = func_800A7FBC();
                self->field_0x2F40 = 1;
            } else {
                func_80065CA4(res->field_0x2C, res);
                res->field_0x4 = 0;
                res->field_0x8 = 0;
                res->field_0x28 = 0;
                res->field_0x0 = 0;
                res->field_0x24 = 0;
                res->field_0x20 = 0;
                self->field_0xE4 = func_800A9784(rounded);
                if (func_800626F4((u32)self->field_0xE4, field1, field2, 4) != 0) {
                    self->field_0x2F40 = 1;
                }
            }
        }
    }
    return handle != 0;
}

// Retail symbol name is func_800B9E3C__Q22cf11CfObjectMapFv (vtable slot); the
// body consumes r4, so keep the retail name via extern "C" (see func_800BA650).
extern "C" void func_800B9E3C__Q22cf11CfObjectMapFv(cf::CfObjectMap* self,
                                                    unsigned long v) {
    func_800B9C74(self, (v >> 20) & 0x7F, (v >> 10) & 0x3FF);
}

extern "C" void func_800B9E4C(cf::CfObjectMap* self) {
    UnkRes866A0* resA = func_80062F60();
    UnkRes866A0* resB = func_80062F18();
    UnkRes866A0* resC = func_80062FA8();
    u32 packed = self->field_0x2F38;
    u32 field1 = (packed >> 5) & 0x7F;
    u32 field2 = (packed >> 12) & 0x3FF;

    if ((resB->field_0x0 & 0x10) != 0) {
        // Per-frame map event processing (model-space delegate list).
        if (resA->field_0x2C->vfunc_0x40(resA) != 0) {
            UnkResObj* iter = static_cast<UnkResObj*>(resA->field_0x2C->vfunc_0x08(resA, 0));
            u32 count = func_800621A0();
            for (s32 i = 0; i < (s32)count; i++) {
                u32 type;
                u32 out10;
                u32 outC;
                void* delegate = func_800621F4(iter, (s16)i, &type, &out10, &outC);
                void* obj = 0;
                u32 flag = 0;
                if (type == 1) {
                    func_800A9534((void*)outC);
                    obj = resB->field_0x2C->vfunc_0x08(resB, 0);
                } else if (type == 2) {
                    obj = func_800A965C((void*)outC);
                    self->field_0xE0 = obj;
                } else if (type == 3) {
                    obj = func_80065D00(resC->field_0x2C, resC);
                    self->field_0xDC = obj;
                } else if (type == 4) {
                    obj = func_800A81FC();
                    self->field_0xE8 = obj;
                } else if (type == 6) {
                    obj = func_800A98A8((void*)outC);
                    func_800627BC((void*)outC);
                } else if (type == 5) {
                    obj = func_801A2C04__Q22cf12CTaskCullingFv();
                } else if (type == 7) {
                    obj = func_800A9CD0();
                    flag = 1;
                }
                if (obj != 0) {
                    CfRes_delegateOp2(obj, delegate, (void*)out10);
                    if (flag != 0) {
                        CBdat::func_8003AA78(1, obj);
                    }
                }
            }
            resB->field_0x0 &= ~0x10;
            if (self->field_0xDC == 0) {
                func_80062680(field1, field2, 3);
            }
        }
        goto done;
    }

    // Map re-entry / setup path.
    if (resB->field_0x2C->vfunc_0x40(resB) == 0) {
        goto done;
    }
    if (self->field_0xDC == 0 && resC->field_0x2C->vfunc_0x40(resC) == 0) {
        goto done;
    }
    void* loaded;
    if (self->field_0x2F40 != 0) {
        UnkRes866A0* res = func_80062FF0();
        loaded = res->field_0x2C->vfunc_0x40(res);
    } else {
        loaded = (void*)1;
    }
    if (loaded == 0) {
        goto done;
    }
    {
        char buf[0x40];
        u32 len = 0;
        self->field_0x8E += 1;
        buf[0] = 0;
        func_800AA33C(buf, resB->field_0x4, 1, 0);
        *reinterpret_cast<void**>(self->field_0x90) = func_80065D04(resB->field_0x2C, resB);
        self->field_0xDC = func_80065D00(resC->field_0x2C, resC);
        void* r3 = func_80489A60(lbl_eu_80663E14, *reinterpret_cast<void**>(self->field_0x90), 5, 1, 0, 0x30);
        func_800BBADC(self, r3);
        func_8047CFBC__17UnkClass_8047CD0CFv(&self->field_0x2F2C);
        if (self->field_0xDC != 0) {
            self->field_0xEC = func_80495EB0(lbl_eu_80663E14, self->field_0xDC, buf);
            func_8047CDBC__17UnkClass_8047CD0CFv(&self->field_0x2F2C,
                                                 getHandleMEM1__Q23mtl10MemManagerFv(), 0xB0000, 0x32);
            func_8047CD0C__17UnkClass_8047CD0CFv(&self->field_0x2F2C, self->field_0xDC);
        }
        if (self->field_0xE0 != 0) {
            if (func_8047BB54__17UnkClass_8047BB54Fv(&self->field_0xF0, self->field_0xE0,
                                                     lbl_eu_80663E14, lbl_eu_80663E10) != 0) {
                *reinterpret_cast<u32*>(self->_pad6C) |= 0x8000;
            }
        }
        if (self->field_0xE4 != 0) {
            func_80462A08__8CTaskLODFv(self->field_0xE4, 0);
            func_80462BC8__8CTaskLODFv(*reinterpret_cast<void**>(&self->field_0x90[8]));
            func_80462BE4__8CTaskLODFv(lbl_eu_80666A38);
        }
        if (self->field_0xE8 != 0) {
            UnkMapFxObj* fx = static_cast<UnkMapFxObj*>(
                func_804C1BA0(reinterpret_cast<UnkSceneView*>(lbl_eu_80663E14)->field_0x7C,
                              self->field_0xE8, 7));
            self->field_0x2F3C = fx;
            if (fx != 0) {
                fx->vfunc_0x84(1, 1);

                ml::CVec3 dir1(lbl_eu_80666A38, lbl_eu_80666A3C, lbl_eu_80666A3C);
                f32 len1 = dir1.y * dir1.y + dir1.x * dir1.x + dir1.z * dir1.z;
                if (dir1.x == len1) {
                    dir1 = ml::CVec3::zero;
                } else {
                    PSVECNormalize(dir1, dir1);
                }
                ml::CVec3 scale1(lbl_eu_80666A40, lbl_eu_80666A40, lbl_eu_80666A40);
                fx->vfunc_0x88(1, 0, &dir1, &scale1);

                ml::CVec3 dir2(lbl_eu_80666A38, lbl_eu_80666A3C, lbl_eu_80666A44);
                f32 len2 = dir2.y * dir2.y + dir2.x * dir2.x + dir2.z * dir2.z;
                if (dir2.x == len2) {
                    dir2 = ml::CVec3::zero;
                } else {
                    PSVECNormalize(dir2, dir2);
                }
                ml::CVec3 scale2(lbl_eu_80666A40, lbl_eu_80666A40, lbl_eu_80666A40);
                fx->vfunc_0x88(1, 1, &dir2, &scale2);

                u32 bdat = getBdatStringColumnValue(lbl_eu_806640A8, &lbl_eu_804FC538[5], lbl_eu_80664184);
                if ((bdat & 1) == 0) {
                    fx->vfunc_0x84(1, 0);
                    ml::CVec3 dir3(lbl_eu_80666A48, lbl_eu_80666A4C, lbl_eu_80666A48);
                    fx->vfunc_0x8C(0, 0, &dir3);
                    fx->vfunc_0x94(0, 0, 0x12, 0x1E, 5, 0x1E, 0x1E);
                }
                self->field_0x2F41 = 0;
            }
        }
    }
    self->field_0x2F44 = 10;
    func_801A2CAC__Q22cf12CTaskCullingFv();
    if (func_80496288(lbl_eu_80663E14) > lbl_eu_80666A50) {
        func_80496294(lbl_eu_80663E14, lbl_eu_80666A3C);
    }
    func_801C028C(0, 0);
    func_801C028C(1, 0);
done:
    ;
}

extern "C" void func_800BA440(cf::CfObjectMap* self) {
    float scale = func_80496288(lbl_eu_80663E14);
    float value = self->field_0x2F48 * scale;
    if (lbl_eu_80663E24 & 0x04000000) {
        value = lbl_eu_80666A38;
    }
    if (&self->field_0xF0 != 0) {
        func_8047BD7C__17UnkClass_8047BB54Fv(&self->field_0xF0, value);
    }
    func_80462BFC__8CTaskLODFv(value);
    u32 v = func_8016E08C();
    if (getUnk80664658()->field_214 & 0x80) {
        v = getUnk80664658()->field_210 & 0xFFFF;
    }
    value = lbl_eu_80666A54;
    s32 count = (s32)self->field_0x2F44 - 1;
    self->field_0x2F44 = (u32)count;
    if (count < 0) {
        self->field_0x2F44 = 0;
    } else {
        value = lbl_eu_80666A38;
    }
    func_80462C80__8CTaskLODFv((u16)v);
    func_804C1F10(reinterpret_cast<UnkSceneView*>(lbl_eu_80663E14)->field_0x7C, (u16)v, value);
    if (lbl_eu_80663E28 & 0x80) {
        v = 2;
    } else {
        v = func_8016E094() & 0xFFFF;
    }
    func_80462CBC__8CTaskLODFv(v);
    if (&self->field_0xF0 != 0) {
        func_8047BD84__17UnkClass_8047BB54Fv(&self->field_0xF0, v);
    }
    UnkMapFxObj* fx = static_cast<UnkMapFxObj*>(self->field_0x2F3C);
    if (fx != 0) {
        if (lbl_eu_80663E24 & 0x02000000) {
            if (self->field_0x2F41 == 0) {
                float vec[4] = {lbl_eu_80666A58, lbl_eu_80666A5C, lbl_eu_80666A60, lbl_eu_80666A64};
                fx->vfunc_0x8C(0, 0, vec);
                self->field_0x2F41 = !self->field_0x2F41;
            }
        } else if (self->field_0x2F41 != 0) {
            float vec[3] = {lbl_eu_80666A48, lbl_eu_80666A4C, lbl_eu_80666A48};
            fx->vfunc_0x8C(0, 0, vec);
            self->field_0x2F41 = !self->field_0x2F41;
        }
    }
}

// PTMF dispatch: field_0x8E (u16 index) selects a CfObjectMap method from
// the 3-entry point-to-member-function table lbl_eu_80529100 (0x28 bytes).
// MWCC lowers (this->*table[idx])() to mulli/lis/addi + `bl __ptmf_scall`.
void cf::CfObjectMap::func_800BA610() {
    typedef void (cf::CfObjectMap::*MapPMF)();
    extern MapPMF lbl_eu_80529100[3];
    u16 idx = *(u16*)((u8*)this + 0x8E);
    if (idx < 3) {
        (this->*lbl_eu_80529100[idx])();
    }
}

extern "C" void func_800BA650__Q22cf11CfObjectMapFv(cf::CfObjectMap* self, float f) {
    extern void func_80484E5C(void*);
    void* unk = *(void**)((char*)self + 0x98);
    *(float*)((char*)self + 0x2F48) = f;
    if (unk != 0) {
        func_80484E5C(unk);
    }
}

extern "C" void func_800BA66C__Q22cf11CfObjectMapFv(cf::CfObjectMap* self, void* task) {
    cf::CfObject* model = *reinterpret_cast<cf::CfObject**>(&self->field_0x90[8]);
    if (model != 0) {
        model->CfObject_UnkVirtualFunc25();
    }
    UnkMapSubF0* sub = &self->field_0xF0;
    if (sub != 0) {
        if (task != 0) {
            sub->flags_0x10 |= 8;
        } else {
            sub->flags_0x10 &= ~8;
        }
    }
    func_80462C48__8CTaskLODFv(task);
    if (func_800828DC__Q22cf13CfGameManagerFv() != 0) {
        func_8016FBA8(func_800828DC__Q22cf13CfGameManagerFv(), task);
    }
}

extern "C" void CfObjectModel_UnkVirtualFunc19__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, int arg) {
    unsigned int* field = (unsigned int*)((char*)self + 0x68);
    if (arg != 0)
        *field |= 0x200000;
    else
        *field &= ~0x200000;
}

extern "C" int CfObjectModel_UnkVirtualFunc5__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return 0; }

extern "C" int CfObjectModel_UnkVirtualFunc4__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return 0; }



extern "C" void func_800BA764__Q22cf11CfObjectMapFv() {}

// Retail D1 dtor for CfObjectMap: vtable reset (lbl_eu_80529128) then the
// CfObject_UnkVirtualFunc6 virtual at vtable+0x68 called via the constant
// (retail lwz r12,104(r12) on the just-written vtable), the embedded
// UnkClass_8047BB54 sub-object at +0xF0, the CfObjectModel base with
// NON-deleting flag 0, then delete-on-flag; returns this.
extern "C" void* __dt__Q22cf11CfObjectMapFv(void* self, int flag) {
    cf::CfObjectMap* this_ = reinterpret_cast<cf::CfObjectMap*>(self);
    if (this_ != 0) {
        u32 vt = (u32)lbl_eu_80529128;
        *(u32*)this_ = vt;
        this_->CfObject_UnkVirtualFunc6();
        void* sub = (char*)this_ + 0xF0;
        if (sub != 0) {
            __dt__8047BDA8(sub);
        }
        __dt__Q22cf13CfObjectModelFv(this_, 0);
        if (flag > 0) {
            ::operator delete(this_);
        }
    }
    return self;
}

extern "C" void CfObject_UnkVirtualFunc33__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { reinterpret_cast<CMIf*>(self)->vf00D0(); }

extern "C" void CfObject_UnkVirtualFunc30__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { reinterpret_cast<CMIf*>(self)->vf00C4(); }

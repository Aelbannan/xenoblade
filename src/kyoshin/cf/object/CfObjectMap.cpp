// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectMap
// Replace stubs with high-level C/C++ during decomp.

#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/object/CfObjectMap.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "kyoshin/cf/IResInfo.hpp"  // real CResLookup owner (getResourceBase/isInUse/cmpField4Eq)
#include "libs/monolib/src/scn/CScnEnvLgtCtrl.hpp"  // real CScnEnvLgtCtrlListItem owner (v25/v31/v32/v33/v35)

// (Local CResLookup deleted: uses real owner in kyoshin/cf/IResInfo.hpp.)
// (Local CScnEnvLgtCtrlListItem deleted: uses real owner in
// libs/monolib/src/scn/CScnEnvLgtCtrl.hpp.)

extern const float lbl_eu_80666A84;

// Retail data + dtor imports for the D1 (the header declares the virtual
// dtor; the symbol is provided by the freestanding definition below).
extern "C" { extern char lbl_eu_80529128[]; }
extern "C" void __dt__8047BDA8(void* self);
extern "C" void __dt__Q22cf13CfObjectModelFv(void* self, int flag);

// Fake table removed: retail slots at 0xC4/0xD0 are CfObject 29/32.
// Wrappers below now call the real CfObject virtuals directly.

// Calls vtable slot 0x18C (CfObjectModel_checkTargetNode) when the pointer at
// +0x70 is set; returns the call's result, or 1 when the pointer is NULL.
// The slot takes a void* arg (CfObjectMove.hpp vtable decl): passing
// this->mTarget70 keeps r4 live as the call argument, so MWCC keeps the ret
// default in r0 — retail's exact allocation (FULL_MATCH).
#pragma scheduling off
void* cf::CfObjectMap::checkTarget() {
    void* ret = (void*)1;
    if (this->mTarget70) {
        ret = this->CfObjectModel_checkTargetNode(this->mTarget70);
    }
    return ret;
}
#pragma scheduling on

void cf::CfObjectMap::cleanupMap() {
    this->CfObjectModel_releaseModelSub();
    if (this->field_0xEC != 0) {
        Scn_IsAnimActiveOrNull(this->field_0xEC);
        this->field_0xEC = 0;
    }
    resetPoolState__17UnkClass_8047CD0CFv(&this->field_0x2F2C);
    if (this->field_0xE0 != 0) {
        this->field_0xE0 = 0;
        CDeviceVI::waitForDrawDone();
        resetMpfInstance__17UnkClass_8047BB54Fv(&this->field_0xF0);
        *reinterpret_cast<u32*>(this->_pad6C) &= ~0x8000;
    }
    if (this->field_0x2F40 != 0) {
        resetActiveLOD__8CTaskLODFv(0);
        this->field_0x2F40 = 0;
    }
    CScnEnvLgtCtrlListItem* resource = this->field_0x2F3C;
    if (resource != 0) {
        scnLgtRemoveLgtItem(reinterpret_cast<UnkSceneView*>(lbl_eu_80663E14)->field_0x7C, resource);
        this->field_0x2F3C = nullptr;
    }
    clearOccFrustums__Q22cf12CTaskCullingFv();
}

// Retail symbol is fake-Fv (MWCC_CASES §fake-Fv): the splitter mangled
// no-args but the body reads r4 as a genuine input (used 3x: simSetFlag2OnTree
// flag, the field_100 |=4 / &=~2 select, and the vfn6C boolean). Defined as
// extern "C" with the explicit Fv-mangled name so the symbol matches retail.
extern "C" void setMapVisibility__Q22cf11CfObjectMapFv(cf::CfObjectMap* self, u32 arg) {
    struct V {
        u8 _00[0x6C];
        u32 field_6C;      // 0x6C
        u8 _70[0x98 - 0x70];
        void* mSub98;      // 0x98
        u8 _9C[0x100 - 0x9C];
        u32 field_100;     // 0x100
    };
    V* v = reinterpret_cast<V*>(self);
    if (v->mSub98) {
        simSetFlag2OnTree((cf::CfObjectModelSub98*)v->mSub98, arg);
        if (v->field_6C & 0x8000) {
            if (arg) {
                v->field_100 |= 4;
            } else {
                v->field_100 = DECOMP_PPC_RLWINM(v->field_100, 0, 30, 28);
            }
        }
        if (self->field_0x2F3C) {
            self->field_0x2F3C->v25(arg == 0);
        }
    }
}

extern "C" void* checkMpfFlags__17UnkClass_8047BB54Fv(void*, void*);
// When the +0x6C bit-15 flag is set, refresh the region pairing through the
// shared scene-view globals and re-bind the +0xF0 sub-object's +0x9C offset.
extern "C" void func_800B9C14(void* self) {
    if (*(u32*)((u8*)self + 0x6C) & 0x8000) {
        extern void* Scn_HasCamItem(void*, void*);
        void* r = Scn_HasCamItem((void*)lbl_eu_80663E14, (void*)lbl_eu_80663E10);
        checkMpfFlags__17UnkClass_8047BB54Fv((u8*)self + 0xF0, (u8*)r + 0x9C);
    }
}

void clearStatus__Q22cf11CfObjectMapFv(void* self) {
    *(unsigned short*)((char*)self + 0x8e) = 0;
}

void func_800B9C70() {}

extern "C" int func_800B9C74(cf::CfObjectMap* self, u32 a, u32 b) {
    self->CfObjectModel_releaseModelSub();
    if (self->field_0xEC != 0) {
        Scn_IsAnimActiveOrNull(self->field_0xEC);
        self->field_0xEC = 0;
    }
    void* handle = CfRes_tryResolveLink(a, b, 3);
    if (handle != 0) {
        self->field_0x2F38 = func_800AA2BC(a, b);
        self->mFlags68 &= 0x40000000;
        self->field_0x8E = 1;
        self->mTarget70 = handle;
    }
    u32 packed = self->field_0x2F38;
    if (packed != 0) {
        char buf[0x40];
        volatile u32 len = 0;
        buf[0] = 0;
        int field1 = (packed >> 20) & 0x7F;
        int field2 = (packed >> 10) & 0x3FF;
        func_800AA33C(reinterpret_cast<ml::FixStr<64>&>(buf), packed, 1, 0);
        len += strlen(lbl_eu_804FC538);
        strcat(buf, lbl_eu_804FC538);
        int size = getFileSize__11CDeviceFileFPCc(buf, 1);
        if (size > 0) {
            u32 rounded = (size + 0x7FF) & ~0x7FF;
            UnkRes866A0* res = CfRes_getInstPtr134();
            if (res->field_0x2C->cmpField4Eq(res, func_800AA2BC(a, b)) != 0 && KyoshinHeap_GetField18() != 0) {
                self->field_0xE4 = KyoshinHeap_GetField18();
                self->field_0x2F40 = 1;
            } else {
                CfRes_cancelPendingRead(res->field_0x2C, res);
                res->field_0x4 = 0;
                res->field_0x8 = 0;
                res->field_0x28 = 0;
                res->field_0x0 = 0;
                res->field_0x24 = 0;
                res->field_0x20 = 0;
                self->field_0xE4 = KyoshinHeap_AllocFld18(rounded);
                if (CfRes_tryResolveSlot130((u32)self->field_0xE4, field1, field2, 4) != 0) {
                    self->field_0x2F40 = 1;
                }
            }
        }
    }
    return handle != 0;
}

// Retail symbol name is setMapId__Q22cf11CfObjectMapFv (vtable slot); the
// body consumes r4, so keep the retail name via extern "C" (see func_800BA650).
extern "C" void setMapId__Q22cf11CfObjectMapFv(cf::CfObjectMap* self,
                                                    unsigned long v) {
    func_800B9C74(self, (v >> 20) & 0x7F, (v >> 10) & 0x3FF);
}

extern "C" void func_800B9E4C(cf::CfObjectMap* self) {
    UnkRes866A0* resA = CfRes_getInstPtrBC();
    UnkRes866A0* resB = CfRes_getInstPtr80();
    UnkRes866A0* resC = CfRes_getInstPtrF8();
    u32 packed = self->field_0x2F38;
    u32 field1 = (packed >> 20) & 0x7F;
    u32 field2 = (packed >> 10) & 0x3FF;

    if ((resB->field_0x0 & 0x10) != 0) {
        // Per-frame map event processing (model-space delegate list).
        if (resA->field_0x2C->isInUse(resA) != 0) {
            void* iter = resA->field_0x2C->getResourceBase(resA, 0);
            u32 count = CfRes_getNameRecCount();
            for (s32 i = 0; i < (s32)count; i++) {
                u32 type;
                u32 out10;
                u32 outC;
                void* delegate = func_800621F4(iter, (s16)i, &type, &out10, &outC);
                void* obj = 0;
                u32 flag = 0;
                if (type == 1) {
                    KyoshinHeap_AllocFld10((void*)outC);
                    obj = resB->field_0x2C->getResourceBase(resB, 0);
                } else if (type == 2) {
                    obj = KyoshinHeap_AllocFld14((void*)outC);
                    self->field_0xE0 = obj;
                } else if (type == 3) {
                    obj = CfRes_getLazyField20(resC->field_0x2C, resC);
                    self->field_0xDC = obj;
                } else if (type == 4) {
                    obj = KyoshinHeap_GetField1C();
                    self->field_0xE8 = obj;
                } else if (type == 6) {
                    obj = KyoshinHeap_AllocFld34((void*)outC);
                    CfRes_tryUpdateSlot16C((void*)outC);
                } else if (type == 5) {
                    obj = func_801A2C04__Q22cf12CTaskCullingFv();
                } else if (type == 7) {
                    obj = KyoshinHeap_GetField40();
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
                CfRes_tryResolveSlotF4(field1, field2, 3);
            }
        }
        goto done;
    }

    // Map re-entry / setup path.
    if (resB->field_0x2C->isInUse(resB) == 0) {
        goto done;
    }
    if (self->field_0xDC == 0 && resC->field_0x2C->isInUse(resC) == 0) {
        goto done;
    }
    int loadedFlag;
    if (self->field_0x2F40 != 0) {
        UnkRes866A0* res = CfRes_getInstPtr134();
        loadedFlag = res->field_0x2C->isInUse(res);
    } else {
        loadedFlag = 1;
    }
    if (loadedFlag == 0) {
        goto done;
    }
    {
        char buf[0x40];
        u32 len = 0;
        self->field_0x8E += 1;
        buf[0] = 0;
        func_800AA33C(reinterpret_cast<ml::FixStr<64>&>(buf), resB->field_0x4, 1, 0);
        *reinterpret_cast<void**>(self->field_0x90) = CfRes_getLazyField10(resB->field_0x2C, resB);
        self->field_0xDC = CfRes_getLazyField20(resC->field_0x2C, resC);
        void* r3 = scnImN4BuildByIdx(lbl_eu_80663E14, *reinterpret_cast<void**>(self->field_0x90), 5, 1, 0, 0x30);
        func_800BBADC(self, r3);
        clearPoolData__17UnkClass_8047CD0CFv(&self->field_0x2F2C);
        if (self->field_0xDC != 0) {
            self->field_0xEC = Scn_InitGlobalB(lbl_eu_80663E14, self->field_0xDC, buf);
            initNodePool__17UnkClass_8047CD0CFv(&self->field_0x2F2C,
                                                 getHandleMEM1__Q23mtl10MemManagerFv(), 0xB0000, 0x32);
            func_8047CD0C__17UnkClass_8047CD0CFv(&self->field_0x2F2C, self->field_0xDC);
        }
        if (self->field_0xE0 != 0) {
            if (initMpfSystem__17UnkClass_8047BB54Fv(&self->field_0xF0, self->field_0xE0,
                                                     lbl_eu_80663E14, lbl_eu_80663E10) != 0) {
                *reinterpret_cast<u32*>(self->_pad6C) |= 0x8000;
            }
        }
        if (self->field_0xE4 != 0) {
            acquireLODResource__8CTaskLODFv(self->field_0xE4, 0);
            bindTaskToLOD__8CTaskLODFv(*reinterpret_cast<void**>(&self->field_0x90[8]));
            notifyLODTick__8CTaskLODFv(lbl_eu_80666A38);
        }
        if (self->field_0xE8 != 0) {
            CScnEnvLgtCtrlListItem* fx = static_cast<CScnEnvLgtCtrlListItem*>(
                func_804C1BA0(reinterpret_cast<UnkSceneView*>(lbl_eu_80663E14)->field_0x7C,
                              self->field_0xE8, 7));
            self->field_0x2F3C = fx;
            if (fx != 0) {
                fx->v31(1, 1);

                ml::CVec3 dir1(lbl_eu_80666A38, lbl_eu_80666A3C, lbl_eu_80666A3C);
                f32 len1 = dir1.y * dir1.y + dir1.x * dir1.x + dir1.z * dir1.z;
                if (dir1.x == len1) {
                    dir1 = ml::CVec3::zero;
                } else {
                    PSVECNormalize(dir1, dir1);
                }
                ml::CVec3 scale1(lbl_eu_80666A40, lbl_eu_80666A40, lbl_eu_80666A40);
                fx->v32(1, 0, &dir1, &scale1);

                ml::CVec3 dir2(lbl_eu_80666A38, lbl_eu_80666A3C, lbl_eu_80666A44);
                f32 len2 = dir2.y * dir2.y + dir2.x * dir2.x + dir2.z * dir2.z;
                if (dir2.x == len2) {
                    dir2 = ml::CVec3::zero;
                } else {
                    PSVECNormalize(dir2, dir2);
                }
                ml::CVec3 scale2(lbl_eu_80666A40, lbl_eu_80666A40, lbl_eu_80666A40);
                fx->v32(1, 1, &dir2, &scale2);

                u32 bdat = getBdatStringColumnValue(lbl_eu_806640A8, &lbl_eu_804FC538[5], lbl_eu_80664184);
                if ((bdat & 1) == 0) {
                    fx->v31(1, 0);
                    ml::CVec3 dir3(lbl_eu_80666A48, lbl_eu_80666A4C, lbl_eu_80666A48);
                    fx->v33(0, 0, &dir3);
                    fx->v35(0, 0, 0x12, 0x1E, 5, 0x1E, 0x1E);
                }
                self->field_0x2F41 = 0;
            }
        }
    }
    self->field_0x2F44 = 10;
    func_801A2CAC__Q22cf12CTaskCullingFv();
    if (Scn_GetFrameDelta(lbl_eu_80663E14) > lbl_eu_80666A50) {
        Scn_SetTimeScale(lbl_eu_80663E14, lbl_eu_80666A3C);
    }
    func_801C028C(0, 0);
    func_801C028C(1, 0);
done:
    ;
}

extern "C" void func_800BA440(cf::CfObjectMap* self) {
    float scale = Scn_GetFrameDelta(lbl_eu_80663E14);
    float value = self->field_0x2F48 * scale;
    if (lbl_eu_80663E24 & 0x04000000) {
        value = lbl_eu_80666A38;
    }
    if (&self->field_0xF0 != 0) {
        setMpfFloatParam__17UnkClass_8047BB54Fv(&self->field_0xF0, value);
    }
    updateLODFrame__8CTaskLODFv(value);
    u32 v = func_8016E08C();
    if (getUnk80664658()->field_214 & 0x80) {
        v = getUnk80664658()->field_210 & 0xFFFF;
    }
    value = lbl_eu_80666A54;
    self->field_0x2F44--;
    if ((s32)self->field_0x2F44 < 0) {
        self->field_0x2F44 = 0;
    } else {
        value = lbl_eu_80666A38;
    }
    // v passed uncast so MWCC emits a separate clrlwi per u16 parameter site
    updateLODRange__8CTaskLODFv(v, value);
    scnLgtPushLgtValue(reinterpret_cast<UnkSceneView*>(lbl_eu_80663E14)->field_0x7C, v, value);
    if (lbl_eu_80663E28 & 0x01000000) {
        v = 2;
    } else {
        v = func_8016E094() & 0xFFFF;
    }
    syncLODTask__8CTaskLODFv(v);
    if (&self->field_0xF0 != 0) {
        forwardMpfCallA__17UnkClass_8047BB54Fv(&self->field_0xF0, v);
    }
    CScnEnvLgtCtrlListItem* fx = self->field_0x2F3C;
    if (fx != 0) {
        if (lbl_eu_80663E24 & 0x02000000) {
            if (self->field_0x2F41 == 0) {
                float vec[4] = {lbl_eu_80666A58, lbl_eu_80666A5C, lbl_eu_80666A60, lbl_eu_80666A64};
                fx->v33(0, 0, vec);
                self->field_0x2F41 = !self->field_0x2F41;
            }
        } else if (self->field_0x2F41 != 0) {
            float vec[3] = {lbl_eu_80666A48, lbl_eu_80666A4C, lbl_eu_80666A48};
            fx->v33(0, 0, vec);
            self->field_0x2F41 = !self->field_0x2F41;
        }
    }
}

// PTMF dispatch: field_0x8E (u16 index) selects a CfObjectMap method from
// the 3-entry point-to-member-function table lbl_eu_80529100 (0x28 bytes).
// MWCC lowers (this->*table[idx])() to mulli/lis/addi + `bl __ptmf_scall`.
// The table is declared at file scope with C linkage: a block-scope extern
// inside the member function lands in ns cf and mangles to
// lbl_eu_80529100__2cf, drifting from retail's plain name.
typedef void (cf::CfObjectMap::*CfObjectMapDispatchPMF)();
extern "C" { extern CfObjectMapDispatchPMF lbl_eu_80529100[3]; }
void cf::CfObjectMap::dispatchMapState() {
    u16 idx = *(u16*)((u8*)this + 0x8E);
    if (idx < 3) {
        (this->*lbl_eu_80529100[idx])();
    }
}

extern "C" void setMapScale__Q22cf11CfObjectMapFv(cf::CfObjectMap* self, float f) {
    extern void simSetLeafDist7B0(void*);
    void* unk = *(void**)((char*)self + 0x98);
    *(float*)((char*)self + 0x2F48) = f;
    if (unk != 0) {
        simSetLeafDist7B0(unk);
    }
}

extern "C" void setMapEffectFlag__Q22cf11CfObjectMapFv(cf::CfObjectMap* self, void* task) {
    cf::CfObject* model = *reinterpret_cast<cf::CfObject**>(&self->field_0x90[8]);
    if (model != 0) {
        model->CfObject_snapMoveTarget(
            reinterpret_cast<ml::CVec3*>(&model->mPos3C), lbl_eu_80666A84);
    }
    UnkMapSubF0* sub = &self->field_0xF0;
    if (sub != 0) {
        if (task != 0) {
            sub->flags_0x10 |= 8;
        } else {
            sub->flags_0x10 &= ~8;
        }
    }
    setLODFilterFlag__8CTaskLODFv(task);
    if (getMapEffectManager__Q22cf13CfGameManagerFv() != 0) {
        func_8016FBA8(getMapEffectManager__Q22cf13CfGameManagerFv(), task);
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



extern "C" void nopMap__Q22cf11CfObjectMapFv() {}

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
        this_->CfObject_releaseMoveTargets();
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

extern "C" void CfObject_UnkVirtualFunc33__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float value) {
    // Retail forwarder at 0xD4 -> slot 0xD0 (CfObject_UnkVirtualFunc32). The float
    // in f1 is forwarded unchanged via bcctr; Model's 32 ignores it.
    reinterpret_cast<cf::CfObject*>(self)->CfObject_syncMoveHead();
}

extern "C" void CfObject_UnkVirtualFunc30__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float value) {
    // Retail forwarder at 0xC8 -> slot 0xC4 (CfObject_setMoveHeadAngle).
    reinterpret_cast<cf::CfObject*>(self)->CfObject_setMoveHeadAngle(value);
}

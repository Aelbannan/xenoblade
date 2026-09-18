// Auto-scaffolded catalog TU for monolib/src/scn/CScnItemModel
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/code_804BF59C.hpp"
#include "monolib/util/MemManager.hpp"

// Cross-unit pool iterator (retail C-ABI, unmangled symbol; defined in
// CScnItemPool.cpp). Iterates the pool's reslist and clears matching
// references on every item via simClearNodeRefs.
extern "C" void CScnItemPool_forEachClearNodeRefs(CScnItemPool* pool, CScnItemModel* item);

// Tail-called from simGetLeafAnimDist / simGetLeafAnimDist2 (retail C-ABI, unmangled
// symbols; defined in CScn_80496B0C.cpp; declared in CScnItemModel.hpp so
// the call reloc names stay verbatim).
// Defined below in this TU; forward-declared for simSetLeafFlag4000's depth-5
// tail call. noinline: retail makes a real tail call here (MWCC_CASES
// MWCC_CASES 9135 - same-TU helpers get auto-inlined otherwise).
extern "C" {
__declspec(noinline) void simToggleWordBits(u8* self, u32 flags, u32 enable);
}

// Same-TU chain-walk helpers (defined below in this TU); forward-declared
// so func_80483448 can inline their walk shapes with the depth-5/4 handle
// calling the helper (the retail keeps those as real calls).
extern "C" float simGetLeafDist7B0(void* self);
extern "C" void simSetLeafDist7B0(CScnItemModel* self, float value);
extern "C" void simSetLeafFlag4000(CScnItemModel* self, u32 param);
extern "C" void func_80482B3C(CScnItemModel* self, u32 param);
extern "C" u32 simQueryLeafAnim(CScnItemModel* node);

// __ct__CScnItemModel: CScnItemModel constructor (retail symbol
// __ct__CScnItemModel). Fragment-function form like __dt__13CScnItemModelFv:
// the vtable label is installed explicitly (a real member ctor would emit a
// reloc to the compiler-generated __vt__ symbol, drifting the reloc name),
// and the field_04 / 0x8 stores precede the vtable store (retail order).
// Initialises the anim sub-object at 0xC, the act-data floats at 0x2D8..0x318,
// the embedded light env at 0x31C, the model fields, clears the embedded
// transform, then zeroes both reference lists.
CScnItemModel* __ct__CScnItemModel(CScnItemModel* self, CScnItemModelOwner* owner,
                                   u32 param) {
    self->field_04 = owner;
    self->value08 = 1;
    *(void**)self = (void*)lbl_eu_8056DD70;
    __ct__80496B0C(&self->field_0xC);
    // act-data accesses fold to self+offset (retail uses r28 directly; a
    // materialised `act` local would take an extra callee-saved register and
    // switch MWCC to the _savegpr_27 helper). Declared reverse-order: MWCC
    // assigns the first-declared temp the LOWEST FPR here, so zero must be
    // declared first to land one in f1 (retail).
    f32 zero = lbl_eu_8066A8C4;
    f32 one = lbl_eu_8066A8C0;
    ((CScnItemModelActData*)self->field_0x1F8)->transform.mFlags = 0;
    ((CScnItemModelActData*)self->field_0x1F8)->f2DC[0] = one;
    ((CScnItemModelActData*)self->field_0x1F8)->f2DC[1] = one;
    ((CScnItemModelActData*)self->field_0x1F8)->f2DC[2] = one;
    ((CScnItemModelActData*)self->field_0x1F8)->value2E8 = one;
    ((CScnItemModelActData*)self->field_0x1F8)->f2EC[0] = one;
    ((CScnItemModelActData*)self->field_0x1F8)->f2EC[1] = one;
    ((CScnItemModelActData*)self->field_0x1F8)->f2EC[2] = one;
    ((CScnItemModelActData*)self->field_0x1F8)->f2EC[3] = one;
    ((CScnItemModelActData*)self->field_0x1F8)->f2EC[4] = one;
    ((CScnItemModelActData*)self->field_0x1F8)->f2EC[5] = one;
    ((CScnItemModelActData*)self->field_0x1F8)->value304 = zero;
    ((CScnItemModelActData*)self->field_0x1F8)->f308[0] = zero;
    ((CScnItemModelActData*)self->field_0x1F8)->f308[1] = zero;
    ((CScnItemModelActData*)self->field_0x1F8)->f308[2] = one;
    ((CScnItemModelActData*)self->field_0x1F8)->f308[3] = one;
    ((CScnItemModelActData*)self->field_0x1F8)->f308[4] = one;
    __ct__804BF5A0((CScnEnvLgtData*)&((CScnItemModelActData*)self->field_0x1F8)->envLight);
    // const globals: MWCC CSEs the repeated C0 references into one load
    // (non-const would reload per store), and allocates the loads like retail
    // (one2->f2, zero2->f1, c8->f0).
    self->flags7A4 = 0;
    self->flags7A8 = 0;
    self->value7AC = lbl_eu_8066A8C0;
    self->value7B0 = lbl_eu_8066A8C4;
    self->field_0x7C4 = 0;
    self->field_0x7C8 = 0;
    self->value7CC = lbl_eu_8066A8C0;
    self->value7D0 = lbl_eu_8066A8C0;
    self->value7D4 = lbl_eu_8066A8C0;
    self->value7D8 = lbl_eu_8066A8C0;
    self->value7DC = lbl_eu_8066A8C0;
    self->value7E0 = lbl_eu_8066A8C0;
    self->value7E4 = param;
    self->value7E8 = 1;
    self->field_7EC = 0;
    self->count820 = 0;
    self->member824.field_0 = 0;
    self->member824.field_4 = 0;
    self->member824.field_C = -1;
    self->field_854 = 0;
    self->value858 = lbl_eu_8066A8C8;
    self->field_85C = 0;
    self->field_860 = 0;
    ((CScnItemModelActData*)self->field_0x1F8)->transform.clear();
    self->slots7B4[0] = 0;
    self->slots7B4[1] = 0;
    self->slots7B4[2] = 0;
    self->slots7B4[3] = 0;
    self->slots834[0] = 0;
    self->field_844 = 0;
    self->slots834[1] = 0;
    self->field_848 = 0;
    self->slots834[2] = 0;
    self->field_84C = 0;
    self->slots834[3] = 0;
    self->field_850 = 0;
    return self;
}

extern "C" void scn80496SetNodeIdByName(u8* self);
extern "C" void simResetAnimAtC(u8* self) { ((void(*)(void*))scn80496SetNodeIdByName)((char*)self + 0xc); }

// func_80482DF4: set/clear the 0x100000 flag at 0x7A4 on self, then walk
// func_80482DF4: set/clear the 0x100000 flag at 0x7A4 on self, then walk
// the model tree: for every live model in the 4-slot list, then the linked
// model, repeat the flag and recurse two levels down the reference list
// before recursing func_80482DF4 itself on the deepest nodes. The 7C4 chain
// is followed 2 levels at the top (the recursion covers deeper levels).
// Same shape as func_80482B3C (retail sizes identical, 0x2B8); extern "C"
// keeps the self-recursion reloc name verbatim (reloc-site gate).
// Flag helper comment retained at site; inline set/clear form matches retail
// branch-for-branch.
extern "C" void func_80482DF4(CScnItemModel* self, u32 param) {
    u32 i, j, k;
    if (param) {
        self->flags7A4 |= 0x100000;
    } else {
        self->flags7A4 &= ~0x100000;
    }
    for (i = 0; i < 4; i++) {
        CScnItemModel* a = self->slots7B4[i];
        if (a != 0) {
            if (param != 0) {
                a->flags7A4 |= 0x100000;
            } else {
                a->flags7A4 &= ~0x100000;
            }
            for (j = 0; j < 4; j++) {
                CScnItemModel* b = a->slots7B4[j];
                if (b != 0) {
                    if (param != 0) {
                        b->flags7A4 |= 0x100000;
                    } else {
                        b->flags7A4 &= ~0x100000;
                    }
                    for (k = 0; k < 4; k++) {
                        CScnItemModel* c = b->slots7B4[k];
                        if (c != 0) {
                            func_80482DF4(c, param);
                        }
                    }
                    if (b->field_0x7C4 != 0) {
                        func_80482DF4(b->field_0x7C4, param);
                    }
                }
            }
            CScnItemModel* b = a->field_0x7C4;
            if (b != 0) {
                if (param != 0) {
                    b->flags7A4 |= 0x100000;
                } else {
                    b->flags7A4 &= ~0x100000;
                }
                for (k = 0; k < 4; k++) {
                    CScnItemModel* c = b->slots7B4[k];
                    if (c != 0) {
                        func_80482DF4(c, param);
                    }
                }
                if (b->field_0x7C4 != 0) {
                    func_80482DF4(b->field_0x7C4, param);
                }
            }
        }
    }
    CScnItemModel* b = self->field_0x7C4;
    if (b != 0) {
        if (param != 0) {
            b->flags7A4 |= 0x100000;
        } else {
            b->flags7A4 &= ~0x100000;
        }
        for (j = 0; j < 4; j++) {
            CScnItemModel* c = b->slots7B4[j];
            if (c != 0) {
                if (param != 0) {
                    c->flags7A4 |= 0x100000;
                } else {
                    c->flags7A4 &= ~0x100000;
                }
                for (k = 0; k < 4; k++) {
                    CScnItemModel* d = c->slots7B4[k];
                    if (d != 0) {
                        func_80482DF4(d, param);
                    }
                }
                if (c->field_0x7C4 != 0) {
                    func_80482DF4(c->field_0x7C4, param);
                }
            }
        }
        CScnItemModel* c = b->field_0x7C4;
        if (c != 0) {
            if (param != 0) {
                c->flags7A4 |= 0x100000;
            } else {
                c->flags7A4 &= ~0x100000;
            }
            for (k = 0; k < 4; k++) {
                CScnItemModel* d = c->slots7B4[k];
                if (d != 0) {
                    func_80482DF4(d, param);
                }
            }
            if (c->field_0x7C4 != 0) {
                func_80482DF4(c->field_0x7C4, param);
            }
        }
    }
}

// simRemoveFromPool: remove `self` from every item in the pool owned by the
// scene object at self->field_04. Tail-calls the pool iterator.
void simRemoveFromPool(CScnItemModel* self) {
    CScnItemPool_forEachClearNodeRefs(self->field_04->pool, self);
}

void simSetFlag200000(){}

void simSetFlag7A8_100(){}

void simSetFlag8000000(){}

// retail: cnt=*(self+0x820); *(u32*)(self+cnt*4+0x7F0)=r4; *(self+0x820)=cnt+1
// Single-expression postfix form fixes the Chaitin colors (MWCC_CASES
// array-push-with-counter, pushWordToBuf).
extern "C" void simPushWordToBuf(void* self, u32 val) {
    u32* cnt = (u32*)((char*)self + 0x820);
    *(u32*)((char*)self + 0x7F0 + ((*cnt)++) * 4) = val;
}

void simSetFlag2000000(){}

// simGetLeafActData: walk the field_0x7C4 chain to its last node and return the
// base of its act-data region (node + 0x1F8). Hand-unrolled 5 levels with a
// self tail-call, mirroring simSetLeafDist7B0 (MWCC does not auto-unroll
// pointer-chasing loops; retail is this shape). extern "C" keeps the self
// tail-call reloc name verbatim (reloc-site gate, same as simSetLeafDist7B0).
extern "C" u8* simGetLeafActData(CScnItemModel* self) {
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    CScnItemModel* n5 = n4->field_0x7C4;
                    if (n5 != 0) {
                        return simGetLeafActData(n5);
                    }
                    return (u8*)n4->field_0x1F8;
                }
                return (u8*)n3->field_0x1F8;
            }
            return (u8*)n2->field_0x1F8;
        }
        return (u8*)n1->field_0x1F8;
    }
    return (u8*)self->field_0x1F8;
}

// func_804831C4: when self has no linked model, fetch the model owned by
// self's scene owner (Scn_SetupAnimDefault), notify via the 1-arg virtual at vtable
// 0xAC, mark the whole model tree with the 0x80000 flag (func_80482B3C walk
// shape, recursing func_80482B3C(c, 1) at depth 3), then sync bit 9 of
// self->flags7A4 onto the model (set 0x40 / clear 0x400000), notify via
// vtable 0x64 and return 1. extern "C" keeps the call reloc names verbatim.
extern "C" u32 func_804831C4(CScnItemModel* self) {
    CScnItemModel* model = Scn_SetupAnimDefault(self->field_04);
    if (model == 0) {
        return 0;
    }
    self->vfuncAC(model);
    // 0x80000 flag tree walk (func_80482B3C shape, always set; recursion
    // hands the depth-3+ nodes to func_80482B3C with enable = 1).
    u32 i, j, k;
    model->flags7A4 |= 0x80000;
    for (i = 0; i < 4; i++) {
        CScnItemModel* a = model->slots7B4[i];
        if (a != 0) {
            a->flags7A4 |= 0x80000;
            for (j = 0; j < 4; j++) {
                CScnItemModel* b = a->slots7B4[j];
                if (b != 0) {
                    b->flags7A4 |= 0x80000;
                    for (k = 0; k < 4; k++) {
                        CScnItemModel* c = b->slots7B4[k];
                        if (c != 0) {
                            func_80482B3C(c, 1);
                        }
                    }
                    if (b->field_0x7C4 != 0) {
                        func_80482B3C(b->field_0x7C4, 1);
                    }
                }
            }
            CScnItemModel* b = a->field_0x7C4;
            if (b != 0) {
                b->flags7A4 |= 0x80000;
                for (k = 0; k < 4; k++) {
                    CScnItemModel* c = b->slots7B4[k];
                    if (c != 0) {
                        func_80482B3C(c, 1);
                    }
                }
                if (b->field_0x7C4 != 0) {
                    func_80482B3C(b->field_0x7C4, 1);
                }
            }
        }
    }
    CScnItemModel* b = model->field_0x7C4;
    if (b != 0) {
        b->flags7A4 |= 0x80000;
        for (j = 0; j < 4; j++) {
            CScnItemModel* c = b->slots7B4[j];
            if (c != 0) {
                c->flags7A4 |= 0x80000;
                for (k = 0; k < 4; k++) {
                    CScnItemModel* d = c->slots7B4[k];
                    if (d != 0) {
                        func_80482B3C(d, 1);
                    }
                }
                if (c->field_0x7C4 != 0) {
                    func_80482B3C(c->field_0x7C4, 1);
                }
            }
        }
        CScnItemModel* c2 = b->field_0x7C4;
        if (c2 != 0) {
            c2->flags7A4 |= 0x80000;
            for (k = 0; k < 4; k++) {
                CScnItemModel* d = c2->slots7B4[k];
                if (d != 0) {
                    func_80482B3C(d, 1);
                }
            }
            if (c2->field_0x7C4 != 0) {
                func_80482B3C(c2->field_0x7C4, 1);
            }
        }
    }
    // sync bit 9 of self->flags7A4 onto the model.
    if (self->flags7A4 & 0x400000) {
        model->flags7A4 |= 0x400000;
    } else {
        model->flags7A4 &= ~0x400000;
    }
    model->vfunc64(4);
    return 1;
}

// func_80483448: model-coupling setup. When self has no linked model, create
// one: fetch the model owned by self's scene owner (Scn_SetupAnimDefault), copy
// self's chain-last 0x7B0 fade distance onto the model's chain-last node
// (the two 0x7B0 chain walks are inlined - shallow leaves use lfs/stfs,
// depth-5/4 handles call simGetLeafDist7B0 / simSetLeafDist7B0), notify via the
// 1-arg virtual at vtable 0xB0, sync the 0x4000 / 0x400000 flags from self
// onto the model chain's last node (simSetLeafFlag4000 shape; the 0x4000 param is
// bit 14 of self->flags7A4), sync the 0x4 flag at 0x7A8, mark the whole
// model tree with the 0x80000 flag (func_80482B3C walk shape, recursing
// func_80482B3C(c, 1) at depth 3), then notify via vtable 0x64 and return 1.
// extern "C" keeps the call reloc names verbatim (reloc-site gate).
extern "C" u32 func_80483448(CScnItemModel* self) {
    if (self->field_0x7C4 != 0) {
        return 1;
    }
    CScnItemModel* model = Scn_SetupAnimDefault(self->field_04);
    if (model == 0) {
        return 0;
    }
    // walk 1: last node's 0x7B0 distance of self's chain (simGetLeafDist7B0 /
    // lfs leaves), cur/next shape (alternating rotation with self in r30).
    f32 value;
    CScnItemModel* cur = self;
    CScnItemModel* next = cur->field_0x7C4;
    if (next != 0) {
        cur = next;
        next = cur->field_0x7C4;
        if (next != 0) {
            cur = next;
            next = cur->field_0x7C4;
            if (next != 0) {
                cur = next;
                next = cur->field_0x7C4;
                if (next != 0) {
                    cur = next;
                    next = cur->field_0x7C4;
                    if (next != 0) {
                        value = simGetLeafDist7B0(next);
                    } else {
                        value = cur->value7B0;
                    }
                } else {
                    value = cur->value7B0;
                }
            } else {
                value = cur->value7B0;
            }
        } else {
            value = cur->value7B0;
        }
    } else {
        value = cur->value7B0;
    }
    // walk 2: store the distance onto model's chain last node (simSetLeafDist7B0
    // / stfs leaves).
    CScnItemModel* n1 = model->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    simSetLeafDist7B0(n4, value);
                } else {
                    n3->value7B0 = value;
                }
            } else {
                n2->value7B0 = value;
            }
        } else {
            n1->value7B0 = value;
        }
    } else {
        model->value7B0 = value;
    }
    self->vfuncB0(model);
    // walk 3: set/clear the 0x4000 flag on model's chain last node (param =
    // bit 14 of self->flags7A4). Inline set/clear at depths 1-4, the depth-5
    // handle calls simSetLeafFlag4000 (which itself uses simToggleWordBits deeper).
    u32 param = (self->flags7A4 >> 14) & 1;
    CScnItemModel* k1 = model->field_0x7C4;
    if (k1 != 0) {
        CScnItemModel* k2 = k1->field_0x7C4;
        if (k2 != 0) {
            CScnItemModel* k3 = k2->field_0x7C4;
            if (k3 != 0) {
                CScnItemModel* k4 = k3->field_0x7C4;
                if (k4 != 0) {
                    CScnItemModel* k5 = k4->field_0x7C4;
                    if (k5 != 0) {
                        simSetLeafFlag4000(k5, param);
                    } else {
                        if (param != 0) {
                            k4->flags7A4 |= 0x4000;
                        } else {
                            k4->flags7A4 &= ~0x4000;
                        }
                    }
                } else {
                    if (param != 0) {
                        k3->flags7A4 |= 0x4000;
                    } else {
                        k3->flags7A4 &= ~0x4000;
                    }
                }
            } else {
                if (param != 0) {
                    k2->flags7A4 |= 0x4000;
                } else {
                    k2->flags7A4 &= ~0x4000;
                }
            }
        } else {
            if (param != 0) {
                k1->flags7A4 |= 0x4000;
            } else {
                k1->flags7A4 &= ~0x4000;
            }
        }
    } else {
        if (param != 0) {
            model->flags7A4 |= 0x4000;
        } else {
            model->flags7A4 &= ~0x4000;
        }
    }
    // sync the 0x4 flag at 0x7A8 from self onto model.
    if (self->flags7A8 & 0x4) {
        model->flags7A8 |= 0x4;
    } else {
        model->flags7A8 &= ~0x4;
    }
    // 0x80000 flag tree walk (func_80482B3C shape, always set; recursion
    // hands the depth-3+ nodes to func_80482B3C with enable = 1).
    u32 i, j, k;
    model->flags7A4 |= 0x80000;
    for (i = 0; i < 4; i++) {
        CScnItemModel* a = model->slots7B4[i];
        if (a != 0) {
            a->flags7A4 |= 0x80000;
            for (j = 0; j < 4; j++) {
                CScnItemModel* b = a->slots7B4[j];
                if (b != 0) {
                    b->flags7A4 |= 0x80000;
                    for (k = 0; k < 4; k++) {
                        CScnItemModel* c = b->slots7B4[k];
                        if (c != 0) {
                            func_80482B3C(c, 1);
                        }
                    }
                    if (b->field_0x7C4 != 0) {
                        func_80482B3C(b->field_0x7C4, 1);
                    }
                }
            }
            CScnItemModel* b = a->field_0x7C4;
            if (b != 0) {
                b->flags7A4 |= 0x80000;
                for (u32 k = 0; k < 4; k++) {
                    CScnItemModel* c = b->slots7B4[k];
                    if (c != 0) {
                        func_80482B3C(c, 1);
                    }
                }
                if (b->field_0x7C4 != 0) {
                    func_80482B3C(b->field_0x7C4, 1);
                }
            }
        }
    }
    CScnItemModel* b = model->field_0x7C4;
    if (b != 0) {
        b->flags7A4 |= 0x80000;
        for (j = 0; j < 4; j++) {
            CScnItemModel* c = b->slots7B4[j];
            if (c != 0) {
                c->flags7A4 |= 0x80000;
                for (u32 k = 0; k < 4; k++) {
                    CScnItemModel* d = c->slots7B4[k];
                    if (d != 0) {
                        func_80482B3C(d, 1);
                    }
                }
                if (c->field_0x7C4 != 0) {
                    func_80482B3C(c->field_0x7C4, 1);
                }
            }
        }
        CScnItemModel* c = b->field_0x7C4;
        if (c != 0) {
            c->flags7A4 |= 0x80000;
            for (u32 k = 0; k < 4; k++) {
                CScnItemModel* d = c->slots7B4[k];
                if (d != 0) {
                    func_80482B3C(d, 1);
                }
            }
            if (c->field_0x7C4 != 0) {
                func_80482B3C(c->field_0x7C4, 1);
            }
        }
    }
    // sync the 0x400000 flag from self onto model.
    if (self->flags7A4 & 0x400000) {
        model->flags7A4 |= 0x400000;
    } else {
        model->flags7A4 &= ~0x400000;
    }
    model->vfunc64(4);
    return 1;
}

// simSetFlag2OnTree: set/clear the 0x2 flag (bit 30) on self to match `param`
// (skipping the whole body when the flag already equals it), notify via the
// 1-arg virtual at vtable 0xA4, then recurse into every live model in both
// 4-slot reference lists and the linked model at 0x7C4. extern "C" keeps
// the self-recursion reloc names verbatim (reloc-site gate, same as
// simGetLeafAnimDist).
extern "C" void simSetFlag2OnTree(CScnItemModel* self, u32 param) {
    if (param ^ ((self->flags7A4 >> 1) & 1)) {
        // volatile: retail re-reads the field in each branch (MWCC CSE would
        // otherwise reuse the guard's loaded value and shrink the function).
        if (param != 0) {
            *(volatile u32*)&self->flags7A4 |= 0x2;
        } else {
            *(volatile u32*)&self->flags7A4 &= ~0x2;
        }
        self->vfuncA4(1);
        for (u32 i = 0; i < 4; i++) {
            CScnItemModel* item = self->slots7B4[i];
            if (item != 0) {
                simSetFlag2OnTree(item, param);
            }
        }
        if (self->field_0x7C4 != 0) {
            simSetFlag2OnTree(self->field_0x7C4, param);
        }
        for (u32 i = 0; i < 4; i++) {
            CScnItemModel* item = self->slots834[i];
            if (item != 0) {
                simSetFlag2OnTree(item, param);
            }
        }
    }
}

// func_804839D4: bind the chr animation resource to the model tree. After an
// entry guard on the resFile's chr-entry count vs `index`, walks the 7C4
// chain re-checking the count at each of the first 3 nodes (a failed check
// clears the result to 0 but still runs the trailing 0x80 flag on self).
// Deeper than 3 the function recurses on the next node. When the chain ends
// at n1/n2 the node is bound directly: simQueryLeafAnim finds the leaf (the
// deep walk), scn80496BindChrAnmPack binds the resource on the node's 0xC sub-object,
// and the 0x200 flag is set iff the first leaf is null but the re-walk finds
// a leaf. When self has no 7C4 child, the deep walk runs from the 7C4 / 7C8
// / self chain (simQueryLeafAnim shape inlined twice) and self gets the same
// 0x200 treatment, then self's 0x80 flag is set and the resource result is
// returned. extern "C" keeps the call reloc names verbatim.
extern "C" u32 func_804839D4(CScnItemModel* self, CScnItemAnimResFile* resFile,
                             int index, u32 c, s32 f, u32 g, u32 h) {
    if ((int)resFile->resFile.GetResAnmChrNumEntries() <= index || index < 0) {
        return 0;
    }
    u32 result;
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        if ((int)resFile->resFile.GetResAnmChrNumEntries() <= index || index < 0) {
            result = 0;
        } else {
            CScnItemModel* n2 = n1->field_0x7C4;
            if (n2 != 0) {
                if ((int)resFile->resFile.GetResAnmChrNumEntries() <= index || index < 0) {
                    result = 0;
                } else {
                    CScnItemModel* n3 = n2->field_0x7C4;
                    if (n3 != 0) {
                        result = func_804839D4(n3, resFile, index, c, f, g, h);
                    } else {
                        u32 leaf = simQueryLeafAnim(n2);
                        result = scn80496BindChrAnmPack(n2->field_0xC, resFile, index, c, f, g, h);
                        if (leaf == 0 && simQueryLeafAnim(n2) != 0) {
                            n2->flags7A4 |= 0x200;
                        } else {
                            n2->flags7A4 &= ~0x200;
                        }
                    }
                    n2->flags7A4 |= 0x80;
                }
            } else {
                u32 leaf = simQueryLeafAnim(n1);
                result = scn80496BindChrAnmPack(n1->field_0xC, resFile, index, c, f, g, h);
                if (leaf == 0 && simQueryLeafAnim(n1) != 0) {
                    n1->flags7A4 |= 0x200;
                } else {
                    n1->flags7A4 &= ~0x200;
                }
            }
            n1->flags7A4 |= 0x80;
        }
    } else {
        // deep walk #1: leaf of the tree below self. E(base, 3) descent
        // (simQueryLeafAnim shape, one level shallower than the helper: the
        // depth-3 handle calls simQueryLeafAnim itself; H budgets 0..2).
        u32 r22;
        CScnItemModel* base = self->field_0x7C4;
        if (base != 0) {
            CScnItemModel* e1 = base->field_0x7C4;
            if (e1 != 0) {
                CScnItemModel* e2 = e1->field_0x7C4;
                if (e2 != 0) {
                    CScnItemModel* e3 = e2->field_0x7C4;
                    if (e3 != 0) {
                        r22 = simQueryLeafAnim(e3);
                    } else if (e2->field_0x7C8 != 0) {
                        r22 = simQueryLeafAnim(e2->field_0x7C8);
                    } else {
                        r22 = scn80496HasChild1Node(e2->field_0xC);
                    }
                } else {
                    CScnItemModel* m1 = e1->field_0x7C8;
                    if (m1 != 0) {
                        CScnItemModel* d1 = m1->field_0x7C4;
                        if (d1 != 0) {
                            r22 = simQueryLeafAnim(d1);
                        } else if (m1->field_0x7C8 != 0) {
                            r22 = simQueryLeafAnim(m1->field_0x7C8);
                        } else {
                            r22 = scn80496HasChild1Node(m1->field_0xC);
                        }
                    } else {
                        r22 = scn80496HasChild1Node(e1->field_0xC);
                    }
                }
            } else {
                CScnItemModel* m1 = base->field_0x7C8;
                if (m1 != 0) {
                    CScnItemModel* d1 = m1->field_0x7C4;
                    if (d1 != 0) {
                        CScnItemModel* d2 = d1->field_0x7C4;
                        if (d2 != 0) {
                            r22 = simQueryLeafAnim(d2);
                        } else if (d1->field_0x7C8 != 0) {
                            r22 = simQueryLeafAnim(d1->field_0x7C8);
                        } else {
                            r22 = scn80496HasChild1Node(d1->field_0xC);
                        }
                    } else {
                        CScnItemModel* m2 = m1->field_0x7C8;
                        if (m2 != 0) {
                            CScnItemModel* e1b = m2->field_0x7C4;
                            if (e1b != 0) {
                                r22 = simQueryLeafAnim(e1b);
                            } else if (m2->field_0x7C8 != 0) {
                                r22 = simQueryLeafAnim(m2->field_0x7C8);
                            } else {
                                r22 = scn80496HasChild1Node(m2->field_0xC);
                            }
                        } else {
                            r22 = scn80496HasChild1Node(m1->field_0xC);
                        }
                    }
                } else {
                    r22 = scn80496HasChild1Node(base->field_0xC);
                }
            }
        } else {
            CScnItemModel* base2 = self->field_0x7C8;
            if (base2 != 0) {
                CScnItemModel* e1 = base2->field_0x7C4;
                if (e1 != 0) {
                    CScnItemModel* e2 = e1->field_0x7C4;
                    if (e2 != 0) {
                        CScnItemModel* e3 = e2->field_0x7C4;
                        if (e3 != 0) {
                            r22 = simQueryLeafAnim(e3);
                        } else if (e2->field_0x7C8 != 0) {
                            r22 = simQueryLeafAnim(e2->field_0x7C8);
                        } else {
                            r22 = scn80496HasChild1Node(e2->field_0xC);
                        }
                    } else {
                        CScnItemModel* m1 = e1->field_0x7C8;
                        if (m1 != 0) {
                            CScnItemModel* d1 = m1->field_0x7C4;
                            if (d1 != 0) {
                                r22 = simQueryLeafAnim(d1);
                            } else if (m1->field_0x7C8 != 0) {
                                r22 = simQueryLeafAnim(m1->field_0x7C8);
                            } else {
                                r22 = scn80496HasChild1Node(m1->field_0xC);
                            }
                        } else {
                            r22 = scn80496HasChild1Node(e1->field_0xC);
                        }
                    }
                } else {
                    CScnItemModel* m1 = base2->field_0x7C8;
                    if (m1 != 0) {
                        CScnItemModel* d1 = m1->field_0x7C4;
                        if (d1 != 0) {
                            CScnItemModel* d2 = d1->field_0x7C4;
                            if (d2 != 0) {
                                r22 = simQueryLeafAnim(d2);
                            } else if (d1->field_0x7C8 != 0) {
                                r22 = simQueryLeafAnim(d1->field_0x7C8);
                            } else {
                                r22 = scn80496HasChild1Node(d1->field_0xC);
                            }
                        } else {
                            CScnItemModel* m2 = m1->field_0x7C8;
                            if (m2 != 0) {
                                CScnItemModel* e1b = m2->field_0x7C4;
                                if (e1b != 0) {
                                    r22 = simQueryLeafAnim(e1b);
                                } else if (m2->field_0x7C8 != 0) {
                                    r22 = simQueryLeafAnim(m2->field_0x7C8);
                                } else {
                                    r22 = scn80496HasChild1Node(m2->field_0xC);
                                }
                            } else {
                                r22 = scn80496HasChild1Node(m1->field_0xC);
                            }
                        }
                    } else {
                        r22 = scn80496HasChild1Node(base2->field_0xC);
                    }
                }
            } else {
                r22 = scn80496HasChild1Node(self->field_0xC);
            }
        }
        result = scn80496BindChrAnmPack(self->field_0xC, resFile, index, c, f, g, h);
        if (r22 == 0) {
            // deep walk #2: re-walk when the first leaf was null.
            u32 w2;
            CScnItemModel* base = self->field_0x7C4;
            if (base != 0) {
                CScnItemModel* e1 = base->field_0x7C4;
                if (e1 != 0) {
                    CScnItemModel* e2 = e1->field_0x7C4;
                    if (e2 != 0) {
                        CScnItemModel* e3 = e2->field_0x7C4;
                        if (e3 != 0) {
                            w2 = simQueryLeafAnim(e3);
                        } else if (e2->field_0x7C8 != 0) {
                            w2 = simQueryLeafAnim(e2->field_0x7C8);
                        } else {
                            w2 = scn80496HasChild1Node(e2->field_0xC);
                        }
                    } else {
                        CScnItemModel* m1 = e1->field_0x7C8;
                        if (m1 != 0) {
                            CScnItemModel* d1 = m1->field_0x7C4;
                            if (d1 != 0) {
                                w2 = simQueryLeafAnim(d1);
                            } else if (m1->field_0x7C8 != 0) {
                                w2 = simQueryLeafAnim(m1->field_0x7C8);
                            } else {
                                w2 = scn80496HasChild1Node(m1->field_0xC);
                            }
                        } else {
                            w2 = scn80496HasChild1Node(e1->field_0xC);
                        }
                    }
                } else {
                    CScnItemModel* m1 = base->field_0x7C8;
                    if (m1 != 0) {
                        CScnItemModel* d1 = m1->field_0x7C4;
                        if (d1 != 0) {
                            CScnItemModel* d2 = d1->field_0x7C4;
                            if (d2 != 0) {
                                w2 = simQueryLeafAnim(d2);
                            } else if (d1->field_0x7C8 != 0) {
                                w2 = simQueryLeafAnim(d1->field_0x7C8);
                            } else {
                                w2 = scn80496HasChild1Node(d1->field_0xC);
                            }
                        } else {
                            CScnItemModel* m2 = m1->field_0x7C8;
                            if (m2 != 0) {
                                CScnItemModel* e1b = m2->field_0x7C4;
                                if (e1b != 0) {
                                    w2 = simQueryLeafAnim(e1b);
                                } else if (m2->field_0x7C8 != 0) {
                                    w2 = simQueryLeafAnim(m2->field_0x7C8);
                                } else {
                                    w2 = scn80496HasChild1Node(m2->field_0xC);
                                }
                            } else {
                                w2 = scn80496HasChild1Node(m1->field_0xC);
                            }
                        }
                    } else {
                        w2 = scn80496HasChild1Node(base->field_0xC);
                    }
                }
            } else {
                CScnItemModel* base2 = self->field_0x7C8;
                if (base2 != 0) {
                    CScnItemModel* e1 = base2->field_0x7C4;
                    if (e1 != 0) {
                        CScnItemModel* e2 = e1->field_0x7C4;
                        if (e2 != 0) {
                            CScnItemModel* e3 = e2->field_0x7C4;
                            if (e3 != 0) {
                                w2 = simQueryLeafAnim(e3);
                            } else if (e2->field_0x7C8 != 0) {
                                w2 = simQueryLeafAnim(e2->field_0x7C8);
                            } else {
                                w2 = scn80496HasChild1Node(e2->field_0xC);
                            }
                        } else {
                            CScnItemModel* m1 = e1->field_0x7C8;
                            if (m1 != 0) {
                                CScnItemModel* d1 = m1->field_0x7C4;
                                if (d1 != 0) {
                                    w2 = simQueryLeafAnim(d1);
                                } else if (m1->field_0x7C8 != 0) {
                                    w2 = simQueryLeafAnim(m1->field_0x7C8);
                                } else {
                                    w2 = scn80496HasChild1Node(m1->field_0xC);
                                }
                            } else {
                                w2 = scn80496HasChild1Node(e1->field_0xC);
                            }
                        }
                    } else {
                        CScnItemModel* m1 = base2->field_0x7C8;
                        if (m1 != 0) {
                            CScnItemModel* d1 = m1->field_0x7C4;
                            if (d1 != 0) {
                                CScnItemModel* d2 = d1->field_0x7C4;
                                if (d2 != 0) {
                                    w2 = simQueryLeafAnim(d2);
                                } else if (d1->field_0x7C8 != 0) {
                                    w2 = simQueryLeafAnim(d1->field_0x7C8);
                                } else {
                                    w2 = scn80496HasChild1Node(d1->field_0xC);
                                }
                            } else {
                                CScnItemModel* m2 = m1->field_0x7C8;
                                if (m2 != 0) {
                                    CScnItemModel* e1b = m2->field_0x7C4;
                                    if (e1b != 0) {
                                        w2 = simQueryLeafAnim(e1b);
                                    } else if (m2->field_0x7C8 != 0) {
                                        w2 = simQueryLeafAnim(m2->field_0x7C8);
                                    } else {
                                        w2 = scn80496HasChild1Node(m2->field_0xC);
                                    }
                                } else {
                                    w2 = scn80496HasChild1Node(m1->field_0xC);
                                }
                            }
                        } else {
                            w2 = scn80496HasChild1Node(base2->field_0xC);
                        }
                    }
                } else {
                    w2 = scn80496HasChild1Node(self->field_0xC);
                }
            }
            if (w2 != 0) {
                self->flags7A4 |= 0x200;
            } else {
                self->flags7A4 &= ~0x200;
            }
        } else {
            self->flags7A4 &= ~0x200;
        }
    }
    self->flags7A4 |= 0x80;
    return result;
}

// simBindChrAnimChain: walk the field_0x7C4 chain binding the chr animation
// resource. At each of the first 4 nodes (self..n3) the resFile's entry
// count must exceed `index` (signed compare), else 0 is returned; the
// walk's tail calls func_804972E8 on the last node's 0xC sub-object, and
// chains longer than 4 recurse. Nested form: the four func_804972E8
// return-handlers fall to the bottom of the function (retail order) and
// the `return 0` blocks stay inline after each count check.
extern "C" u32 simBindChrAnimChain(CScnItemModel* self, CScnItemAnimResFile* resFile,
                             int index, u32 c, s32 f) {
    if ((int)resFile->resFile.GetResAnmChrNumEntries() <= index) {
        return 0;
    }
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        if ((int)resFile->resFile.GetResAnmChrNumEntries() <= index) {
            return 0;
        }
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            if ((int)resFile->resFile.GetResAnmChrNumEntries() <= index) {
                return 0;
            }
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                if ((int)resFile->resFile.GetResAnmChrNumEntries() <= index) {
                    return 0;
                }
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    return simBindChrAnimChain(n4, resFile, index, c, f);
                }
                return func_804972E8(n3->field_0xC, resFile, index, c, f);
            }
            return func_804972E8(n2->field_0xC, resFile, index, c, f);
        }
        return func_804972E8(n1->field_0xC, resFile, index, c, f);
    }
    return func_804972E8(self->field_0xC, resFile, index, c, f);
}

// simQueryLeafAnim: depth-first walk of the field_0x7C4 / field_0x7C8 tree,
// preferring the 7C4 child, down to the first leaf (both children null),
// then tail-calls scn80496HasChild1Node on the leaf's 0xC sub-object. The 7C4
// descent is unrolled 4 levels from the entry node; after a 7C8 escape the
// remaining descent budget shrinks by one (retail shape: E(node,4) with
// H(c,b) = { if c->7C8 == 0 -> scn80496HasChild1Node(c+0xC); else E(c->7C8, b) }).
// Every call is a tail call, so no frame is needed (retail starts with the
// first load).
extern "C" u32 simQueryLeafAnim(CScnItemModel* node) {
    // E(node, 4): descend the 7C4 chain 4 levels.
    CScnItemModel* n1 = node->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    return simQueryLeafAnim(n4);
                }
                // H(n3, 0)
                if (n3->field_0x7C8 != 0) {
                    return simQueryLeafAnim(n3->field_0x7C8);
                }
                return scn80496HasChild1Node(n3->field_0xC);
            }
            // H(n2, 1)
            CScnItemModel* m1 = n2->field_0x7C8;
            if (m1 != 0) {
                CScnItemModel* d1 = m1->field_0x7C4;
                if (d1 != 0) {
                    return simQueryLeafAnim(d1);
                }
                // H(m1, 0)
                if (m1->field_0x7C8 != 0) {
                    return simQueryLeafAnim(m1->field_0x7C8);
                }
                return scn80496HasChild1Node(m1->field_0xC);
            }
            return scn80496HasChild1Node(n2->field_0xC);
        }
        // H(n1, 2)
        CScnItemModel* m1 = n1->field_0x7C8;
        if (m1 != 0) {
            CScnItemModel* d1 = m1->field_0x7C4;
            if (d1 != 0) {
                CScnItemModel* d2 = d1->field_0x7C4;
                if (d2 != 0) {
                    return simQueryLeafAnim(d2);
                }
                // H(d1, 0)
                if (d1->field_0x7C8 != 0) {
                    return simQueryLeafAnim(d1->field_0x7C8);
                }
                return scn80496HasChild1Node(d1->field_0xC);
            }
            // H(m1, 1)
            CScnItemModel* m2 = m1->field_0x7C8;
            if (m2 != 0) {
                CScnItemModel* e1 = m2->field_0x7C4;
                if (e1 != 0) {
                    return simQueryLeafAnim(e1);
                }
                // H(m2, 0)
                if (m2->field_0x7C8 != 0) {
                    return simQueryLeafAnim(m2->field_0x7C8);
                }
                return scn80496HasChild1Node(m2->field_0xC);
            }
            return scn80496HasChild1Node(m1->field_0xC);
        }
        return scn80496HasChild1Node(n1->field_0xC);
    }
    // H(node, 3)
    CScnItemModel* m1 = node->field_0x7C8;
    if (m1 != 0) {
        CScnItemModel* d1 = m1->field_0x7C4;
        if (d1 != 0) {
            CScnItemModel* d2 = d1->field_0x7C4;
            if (d2 != 0) {
                CScnItemModel* d3 = d2->field_0x7C4;
                if (d3 != 0) {
                    return simQueryLeafAnim(d3);
                }
                // H(d2, 0)
                if (d2->field_0x7C8 != 0) {
                    return simQueryLeafAnim(d2->field_0x7C8);
                }
                return scn80496HasChild1Node(d2->field_0xC);
            }
            // H(d1, 1)
            CScnItemModel* m2 = d1->field_0x7C8;
            if (m2 != 0) {
                CScnItemModel* e1 = m2->field_0x7C4;
                if (e1 != 0) {
                    return simQueryLeafAnim(e1);
                }
                // H(m2, 0)
                if (m2->field_0x7C8 != 0) {
                    return simQueryLeafAnim(m2->field_0x7C8);
                }
                return scn80496HasChild1Node(m2->field_0xC);
            }
            return scn80496HasChild1Node(d1->field_0xC);
        }
        // H(m1, 2)
        CScnItemModel* m2 = m1->field_0x7C8;
        if (m2 != 0) {
            CScnItemModel* e1 = m2->field_0x7C4;
            if (e1 != 0) {
                CScnItemModel* e2 = e1->field_0x7C4;
                if (e2 != 0) {
                    return simQueryLeafAnim(e2);
                }
                // H(e1, 0)
                if (e1->field_0x7C8 != 0) {
                    return simQueryLeafAnim(e1->field_0x7C8);
                }
                return scn80496HasChild1Node(e1->field_0xC);
            }
            // H(m2, 1)
            CScnItemModel* m3 = m2->field_0x7C8;
            if (m3 != 0) {
                CScnItemModel* f1 = m3->field_0x7C4;
                if (f1 != 0) {
                    return simQueryLeafAnim(f1);
                }
                // H(m3, 0)
                if (m3->field_0x7C8 != 0) {
                    return simQueryLeafAnim(m3->field_0x7C8);
                }
                return scn80496HasChild1Node(m3->field_0xC);
            }
            return scn80496HasChild1Node(m2->field_0xC);
        }
        return scn80496HasChild1Node(m1->field_0xC);
    }
    return scn80496HasChild1Node(node->field_0xC);
}

// simGetLeafFlagBit9: walk the field_0x7C4 / field_0x7C8 tree preferring the
// 7C4 child. E(node) unrolls a 4-level 7C4 descent; when 7C4 is null at
// depth k the fallback H(node_at_depth_k, 3-k) is entered: check node->7C8,
// then descend its 7C4 chain (with the budget shrinking by 2), falling back
// to H(m, b-1) when the 7C8 node's 7C4 is null. Returns bit 9 of the
// terminal node's 0x7A4. All calls are tail calls, so no frame is needed
// (retail starts with the first load). extern "C" keeps the self-recursion
// reloc name verbatim (reloc-site gate, same as simQueryLeafAnim).
extern "C" u32 simGetLeafFlagBit9(CScnItemModel* node) {
    // E(node): 4-level 7C4 descent.
    CScnItemModel* n1 = node->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    return simGetLeafFlagBit9(n4);
                }
                // H(n3, 0)
                if (n3->field_0x7C8 != 0) {
                    return simGetLeafFlagBit9(n3->field_0x7C8);
                }
                return (n3->flags7A4 >> 9) & 1;
            }
            // H(n2, 1)
            CScnItemModel* m1 = n2->field_0x7C8;
            if (m1 != 0) {
                CScnItemModel* d1 = m1->field_0x7C4;
                if (d1 != 0) {
                    return simGetLeafFlagBit9(d1);
                }
                // H(m1, 0)
                if (m1->field_0x7C8 != 0) {
                    return simGetLeafFlagBit9(m1->field_0x7C8);
                }
                return (m1->flags7A4 >> 9) & 1;
            }
            return (n2->flags7A4 >> 9) & 1;
        }
        // H(n1, 2): the 7C8 node's 7C4 chain descends one extra level
        // (budget 2 => d1->7C4 checked, then H(d1, 0) / E(d2)).
        CScnItemModel* m1 = n1->field_0x7C8;
        if (m1 != 0) {
            CScnItemModel* d1 = m1->field_0x7C4;
            if (d1 != 0) {
                CScnItemModel* d2 = d1->field_0x7C4;
                if (d2 != 0) {
                    return simGetLeafFlagBit9(d2);
                }
                // H(d1, 0)
                if (d1->field_0x7C8 != 0) {
                    return simGetLeafFlagBit9(d1->field_0x7C8);
                }
                return (d1->flags7A4 >> 9) & 1;
            }
            // H(m1, 1)
            CScnItemModel* m2 = m1->field_0x7C8;
            if (m2 != 0) {
                CScnItemModel* e1 = m2->field_0x7C4;
                if (e1 != 0) {
                    return simGetLeafFlagBit9(e1);
                }
                // H(m2, 0)
                if (m2->field_0x7C8 != 0) {
                    return simGetLeafFlagBit9(m2->field_0x7C8);
                }
                return (m2->flags7A4 >> 9) & 1;
            }
            return (m1->flags7A4 >> 9) & 1;
        }
        return (n1->flags7A4 >> 9) & 1;
    }
    // H(node, 3): the 7C8 node's 7C4 chain descends two extra levels
    // (d1->7C4, d2->7C4, then H(d1, 1) / H(d2, 0) / E(d3)).
    CScnItemModel* m1 = node->field_0x7C8;
    if (m1 != 0) {
        CScnItemModel* d1 = m1->field_0x7C4;
        if (d1 != 0) {
            CScnItemModel* d2 = d1->field_0x7C4;
            if (d2 != 0) {
                CScnItemModel* d3 = d2->field_0x7C4;
                if (d3 != 0) {
                    return simGetLeafFlagBit9(d3);
                }
                // H(d2, 0)
                if (d2->field_0x7C8 != 0) {
                    return simGetLeafFlagBit9(d2->field_0x7C8);
                }
                return (d2->flags7A4 >> 9) & 1;
            }
            // H(d1, 1)
            CScnItemModel* x1 = d1->field_0x7C8;
            if (x1 != 0) {
                CScnItemModel* y1 = x1->field_0x7C4;
                if (y1 != 0) {
                    return simGetLeafFlagBit9(y1);
                }
                // H(x1, 0)
                if (x1->field_0x7C8 != 0) {
                    return simGetLeafFlagBit9(x1->field_0x7C8);
                }
                return (x1->flags7A4 >> 9) & 1;
            }
            return (d1->flags7A4 >> 9) & 1;
        }
        // H(m1, 2)
        CScnItemModel* m2 = m1->field_0x7C8;
        if (m2 != 0) {
            CScnItemModel* d = m2->field_0x7C4;
            if (d != 0) {
                CScnItemModel* d2 = d->field_0x7C4;
                if (d2 != 0) {
                    return simGetLeafFlagBit9(d2);
                }
                // H(d, 0)
                if (d->field_0x7C8 != 0) {
                    return simGetLeafFlagBit9(d->field_0x7C8);
                }
                return (d->flags7A4 >> 9) & 1;
            }
            // H(m2, 1)
            CScnItemModel* m3 = m2->field_0x7C8;
            if (m3 != 0) {
                CScnItemModel* e1 = m3->field_0x7C4;
                if (e1 != 0) {
                    return simGetLeafFlagBit9(e1);
                }
                // H(m3, 0)
                if (m3->field_0x7C8 != 0) {
                    return simGetLeafFlagBit9(m3->field_0x7C8);
                }
                return (m3->flags7A4 >> 9) & 1;
            }
            return (m2->flags7A4 >> 9) & 1;
        }
        return (m1->flags7A4 >> 9) & 1;
    }
    return (node->flags7A4 >> 9) & 1;
}

// simSyncBuf824Flags: update the flags of every element in the 0x824 buffer
// holder, then notify via the 1-arg virtual at vtable 0xA4. When `param`
// is zero every element's 0x4 flag is set; otherwise each element's 0x4
// flag is synced to its 0x2 flag. Returns 0 when the model's 0x2 flag is
// clear; otherwise 1 (or the model's 0x4 flag bit for the sync path). The
// loop condition re-reads the buffer base/count every iteration (retail
// shape - MWCC cannot hoist them past the store through the loaded
// pointer).
int simSyncBuf824Flags(CScnItemModel* self, u32 param) {
    if (!(self->flags7A4 & 0x2)) {
        return 0;
    }
    if (param == 0) {
        CScnItemModel824Entry* p = (CScnItemModel824Entry*)self->member824.field_0;
        while (p != (CScnItemModel824Entry*)((u8*)self->member824.field_0 + self->member824.field_4 * 4)) {
            p->flags |= 0x4;
            p++;
        }
        self->vfuncA4(1);
        return 1;
    }
    {
        CScnItemModel824Entry* p = (CScnItemModel824Entry*)self->member824.field_0;
        while (p != (CScnItemModel824Entry*)((u8*)self->member824.field_0 + self->member824.field_4 * 4)) {
            // volatile: retail re-reads the u16 after the bit test in each
            // branch (MWCC CSE would otherwise reuse the test's loaded value).
            if (p->flags & 0x2) {
                *(volatile u16*)&p->flags |= 0x4;
            } else {
                *(volatile u16*)&p->flags &= ~0x4;
            }
            p++;
        }
        self->vfuncA4((self->flags7A4 >> 2) & 1);
        return (self->flags7A4 >> 2) & 1;
    }
}

// simSetAndPropRate: store the float at 0x858, then propagate it via the
// 1-arg float virtual (vtable 0x48) to every live model in the 4-slot
// reference list (0x7B4), the linked model at 0x7C4, then the second
// 4-slot list (0x834).
void simSetAndPropRate(CScnItemModel* self, float param) {
    self->value858 = param;
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            item->vfunc48(param);
        }
    }
    if (self->field_0x7C4 != 0) {
        self->field_0x7C4->vfunc48(param);
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots834[i];
        if (item != 0) {
            item->vfunc48(param);
        }
    }
}

float simGetRate858(u8* self) { return ((CScnItemModel*)self)->value858; }

void simNotifyVfuncB4(CScnItemModel* self, u32 param) {
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            item->vfuncB4(param);
        }
    }
    if (self->field_0x7C4 != 0) {
        self->field_0x7C4->vfuncB4(param);
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots834[i];
        if (item != 0) {
            item->vfuncB4(param);
        }
    }
}

// simNotifyVfunc8C: same triple-list traversal as simNotifyVfuncB4, calling the
// 1-arg virtual at vtable 0x8C.
void simNotifyVfunc8C(CScnItemModel* self, u32 param) {
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            item->vfunc8C(param);
        }
    }
    if (self->field_0x7C4 != 0) {
        self->field_0x7C4->vfunc8C(param);
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots834[i];
        if (item != 0) {
            item->vfunc8C(param);
        }
    }
}

// simProbeModelVec: call the 2-arg query virtual (vtable 0x90) on every live
// model in the 4-slot reference list (0x7B4), the linked model at 0x7C4,
// then the second 4-slot list (0x834). Returns whether ANY call reported
// handled - the accumulator is re-bool-ized after each call (retail shape:
// `or` first, then the neg/or/srwi setnz).
int simProbeModelVec(CScnItemModel* self, u32 a, u32 b) {
    int result = 0;
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            result = (result | item->vfunc90((const void*)a, b)) != 0;
        }
    }
    if (self->field_0x7C4 != 0) {
        result = (result | self->field_0x7C4->vfunc90((const void*)a, b)) != 0;
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots834[i];
        if (item != 0) {
            result = (result | item->vfunc90((const void*)a, b)) != 0;
        }
    }
    return result;
}

// simNotifyVfunc94: same triple-list traversal as simNotifyVfuncB4, calling the
// 1-arg virtual at vtable 0x94.
void simNotifyVfunc94(CScnItemModel* self, u32 param) {
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            item->vfunc94(param);
        }
    }
    if (self->field_0x7C4 != 0) {
        self->field_0x7C4->vfunc94(param);
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots834[i];
        if (item != 0) {
            item->vfunc94(param);
        }
    }
}

// simNotifyReadyTree: invoke the no-arg virtual (vtable 0x98) on every live model
// in the 4-slot reference list (0x7B4), on the linked model at 0x7C4, then
// on every live model in the second 4-slot list (0x834).
void simNotifyReadyTree(CScnItemModel* self) {
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            item->vfunc98();
        }
    }
    if (self->field_0x7C4 != 0) {
        self->field_0x7C4->vfunc98();
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots834[i];
        if (item != 0) {
            item->vfunc98();
        }
    }
}

// __dt__8048268C: destructor of the 0x10-byte buffer-holder sub-object at
// CScnItemModel+0x824. Releases the owned buffer (field_C != 0xFFFFFFFF)
// via mtl::MemManager::deallocate, resets the fields, and frees the object
// when the deletion flag is positive. Fragment-function form: the class
// name is an address fragment and MWCC emits `__dt__` names verbatim.
CScnItemModel824* __dt__8048268C(CScnItemModel824* self, int deleting) {
    if (self != nullptr) {
        self->field_4 = 0;
        if (self->field_C != 0xFFFFFFFF) {
            if (self->field_0 != nullptr) {
                mtl::MemManager::deallocate(self->field_0);
                self->field_0 = nullptr;
            }
        }
        self->field_0 = nullptr;
        self->field_8 = nullptr;
        self->field_C = -1;
        if (deleting > 0) {
            operator delete(self);
        }
    }
    return self;
}

// __dt__13CScnItemModelFv: CScnItemModel destructor, fragment-function form
// (mirrors CScnItemCamera's __dt__14CScnItemCameraFv so no vtable store is
// emitted). Destroys the 0x824 buffer holder (inlined by MWCC), the
// light-environment sub-object at 0x31C, the anim sub-object at 0xC, then
// frees the object when the deletion flag is positive.
CScnItemModel* __dt__13CScnItemModelFv(CScnItemModel* self, int deleting) {
    if (self != nullptr) {
        __dt__8048268C(&self->member824, 0);
        __dt__804BF6F4(&((CScnItemModelActData*)self->field_0x1F8)->envLight, -1);
        __dt__80496BB4(self->field_0xC, -1);
        if (deleting > 0) {
            operator delete(self);
        }
    }
    return self;
}

// simNotifyVfunc9C: call the 2-arg virtual (vtable 0x9C) on every live model in
// the 4-slot reference list (0x7B4), on the linked model at 0x7C4, then on
// every live model in the second 4-slot list (0x834).
void simNotifyVfunc9C(CScnItemModel* self, u32 a, u32 b) {
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            item->vfunc9C(a, b);
        }
    }
    if (self->field_0x7C4 != 0) {
        self->field_0x7C4->vfunc9C(a, b);
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots834[i];
        if (item != 0) {
            item->vfunc9C(a, b);
        }
    }
}

void simSetValue7E8(u8* self, u32 val) { ((CScnItemModel*)self)->value7E8 = val; }

void simNopVfuncSlot(CScnItemModel* self, u32 param) { (void)self; (void)param; }

extern "C" void simSetFlags7A8_12(u8* self, u32 a, u32 b) {
    volatile u32* f = (volatile u32*)((u8*)self + 0x7A8);
    if (a != 0) {
        *f |= 0x1;
    } else {
        *f &= ~0x1;
    }
    if (b != 0) {
        *f |= 0x2;
    } else {
        *f &= ~0x2;
    }
}

// simSetLeafDist7B0: walk the field_0x7C4 chain to its last node and store
// `value` at its 0x7B0 field. Hand-unrolled 4 levels with a self tail-call
// (MWCC does not auto-unroll pointer-chasing loops; retail is this shape).
// extern "C" keeps the self tail-call reloc name verbatim (reloc-site gate,
// same as KizunaEntryFindListHead).
extern "C" void simSetLeafDist7B0(CScnItemModel* self, float value) {
    CScnItemModel* cur = self;
    CScnItemModel* next = cur->field_0x7C4;
    if (next != 0) {
        cur = next;
        next = cur->field_0x7C4;
        if (next != 0) {
            cur = next;
            next = cur->field_0x7C4;
            if (next != 0) {
                cur = next;
                next = cur->field_0x7C4;
                if (next != 0) {
                    simSetLeafDist7B0(next, value);
                } else {
                    cur->value7B0 = value;
                }
            } else {
                cur->value7B0 = value;
            }
        } else {
            cur->value7B0 = value;
        }
    } else {
        cur->value7B0 = value;
    }
}

// simGetLeafDist7B0: walk the field_0x7C4 chain to its last node and return its
// 0x7B0 value. Same hand-unrolled 5-level self tail-call shape as
// simSetLeafDist7B0 (MWCC does not auto-unroll pointer-chasing loops; retail is
// this shape). extern "C" keeps the self tail-call reloc name verbatim.
extern "C" float simGetLeafDist7B0(void* self) {
    CScnItemModel* s = (CScnItemModel*)self;
    CScnItemModel* n1 = s->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    CScnItemModel* n5 = n4->field_0x7C4;
                    if (n5 != 0) {
                        return simGetLeafDist7B0(n5);
                    }
                    return n4->value7B0;
                }
                return n3->value7B0;
            }
            return n2->value7B0;
        }
        return n1->value7B0;
    }
    return s->value7B0;
}

// simGetLeafAnimDist: walk the field_0x7C4 chain to its last node and tail-call
// scn80496GetChild1V6 with the node's 0xC sub-object. Same hand-unrolled 5-level
// self tail-call shape as simSetLeafDist7B0. extern "C" keeps both the self and
// the scn80496GetChild1V6 tail-call reloc names verbatim (reloc-site gate).
extern "C" float simGetLeafAnimDist(CScnItemModel* self) {
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    CScnItemModel* n5 = n4->field_0x7C4;
                    if (n5 != 0) {
                        return simGetLeafAnimDist(n5);
                    }
                    return scn80496GetChild1V6(n4->field_0xC);
                }
                return scn80496GetChild1V6(n3->field_0xC);
            }
            return scn80496GetChild1V6(n2->field_0xC);
        }
        return scn80496GetChild1V6(n1->field_0xC);
    }
    return scn80496GetChild1V6(self->field_0xC);
}

// simRefreshFlag8: refresh the 0x8 flag at 0x7A8 on self and every model in
// the field_0x7C4 chain when the sub-object distance (scn80496GetChild1V6) no
// longer equals `value`. The first 4 chain nodes inline the ori; the walk's
// tail calls scn80496CallChild1V5 on the last node's 0xC sub-object, and chains
// longer than 4 recurse. Hand-unrolled like the other chain walks.
extern "C" void simRefreshFlag8(CScnItemModel* self, float value) {
    if (scn80496GetChild1V6(self->field_0xC) != value) {
        self->flags7A8 |= 0x8;
    }
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        if (scn80496GetChild1V6(n1->field_0xC) != value) {
            n1->flags7A8 |= 0x8;
        }
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            if (scn80496GetChild1V6(n2->field_0xC) != value) {
                n2->flags7A8 |= 0x8;
            }
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                if (scn80496GetChild1V6(n3->field_0xC) != value) {
                    n3->flags7A8 |= 0x8;
                }
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    simRefreshFlag8(n4, value);
                } else {
                    scn80496CallChild1V5(n3->field_0xC, value);
                }
            } else {
                scn80496CallChild1V5(n2->field_0xC, value);
            }
        } else {
            scn80496CallChild1V5(n1->field_0xC, value);
        }
    } else {
        scn80496CallChild1V5(self->field_0xC, value);
    }
}

// simGetLeafAnimDist2: walk the field_0x7C4 chain to its last node and tail-call
// scn80496GetChild1Float with the node's 0xC sub-object. Same shape as simGetLeafAnimDist.
extern "C" float simGetLeafAnimDist2(CScnItemModel* self) {
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    CScnItemModel* n5 = n4->field_0x7C4;
                    if (n5 != 0) {
                        return simGetLeafAnimDist2(n5);
                    }
                    return scn80496GetChild1Float(n4->field_0xC);
                }
                return scn80496GetChild1Float(n3->field_0xC);
            }
            return scn80496GetChild1Float(n2->field_0xC);
        }
        return scn80496GetChild1Float(n1->field_0xC);
    }
    return scn80496GetChild1Float(self->field_0xC);
}

// simGetLeafAnimDist3: walk the field_0x7C4 chain to its last node and tail-call
// scn80496GetChild2Float with the node's 0xC sub-object. Five named locals reproduce
// the retail register rotation (r4,r3,r4,r5,r3); the cur/next shape used by
// the simSetLeafDist7B0 family rotates differently (reg-swap-only there).
extern "C" float simGetLeafAnimDist3(CScnItemModel* self) {
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    CScnItemModel* n5 = n4->field_0x7C4;
                    if (n5 != 0) {
                        return simGetLeafAnimDist3(n5);
                    }
                    return scn80496GetChild2Float(n4->field_0xC);
                }
                return scn80496GetChild2Float(n3->field_0xC);
            }
            return scn80496GetChild2Float(n2->field_0xC);
        }
        return scn80496GetChild2Float(n1->field_0xC);
    }
    return scn80496GetChild2Float(self->field_0xC);
}

// simGetLeafAnimDist4: same 5-level chain walk as simGetLeafAnimDist3, tail-calling
// scn80496GetChild2V6 with the last node's 0xC sub-object.
extern "C" f32 simGetLeafAnimDist4(CScnItemModel* self) {
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    CScnItemModel* n5 = n4->field_0x7C4;
                    if (n5 != 0) {
                        return simGetLeafAnimDist4(n5);
                    }
                    return scn80496GetChild2V6(n4->field_0xC);
                }
                return scn80496GetChild2V6(n3->field_0xC);
            }
            return scn80496GetChild2V6(n2->field_0xC);
        }
        return scn80496GetChild2V6(n1->field_0xC);
    }
    return scn80496GetChild2V6(self->field_0xC);
}

// simGetLeafAnimStatus: same 5-level chain walk as simGetLeafAnimDist3, tail-calling
// scn80496IsNodeFlagBit with the last node's 0xC sub-object.
extern "C" u32 simGetLeafAnimStatus(CScnItemModel* self) {
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    CScnItemModel* n5 = n4->field_0x7C4;
                    if (n5 != 0) {
                        return simGetLeafAnimStatus(n5);
                    }
                    return scn80496IsNodeFlagBit(n4->field_0xC);
                }
                return scn80496IsNodeFlagBit(n3->field_0xC);
            }
            return scn80496IsNodeFlagBit(n2->field_0xC);
        }
        return scn80496IsNodeFlagBit(n1->field_0xC);
    }
    return scn80496IsNodeFlagBit(self->field_0xC);
}

// simRefreshFadeDist: when value7B0 reaches 1.0, re-seed the fade distances from
// the TV-format constant; returns whether the fade is still active. The two
// chain walks are written inline (MWCC does not auto-inline the 0x68-byte
// helpers; retail shows the 5-level unrolled walk with a real call to
// simGetLeafAnimDist2 / simGetLeafAnimDist for chains deeper than 5).
extern "C" bool simRefreshFadeDist(CScnItemModel* self) {
    // b starts as the >= 1.0 test (MWCC materialises it with the extrwi.,
    // whose r3 = 0 also serves as b = false on the < 1.0 path).
    bool b = self->value7B0 >= lbl_eu_8066A8C0;
    f32 f31, d1, d2;
    if (b) {
        f31 = CDeviceVI::isTvFormatPal() ? lbl_eu_8066A8CC : lbl_eu_8066A8C4;
        // walk 1: last node's distance (scn80496GetChild1Float leaves). Written as one
        // nested-ternary assignment (loads stay conditional via short-circuit)
        // so MWCC places the single fmr f30, f1 at the chain's join (retail
        // shares one move instead of emitting it in every leaf).
        CScnItemModel* n1 = self->field_0x7C4;
        CScnItemModel* n2 = 0;
        CScnItemModel* n3 = 0;
        CScnItemModel* n4 = 0;
        CScnItemModel* n5 = 0;
        d1 = (n1 != 0)
                 ? ((n2 = n1->field_0x7C4) != 0
                        ? ((n3 = n2->field_0x7C4) != 0
                               ? ((n4 = n3->field_0x7C4) != 0
                                      ? ((n5 = n4->field_0x7C4) != 0
                                             ? simGetLeafAnimDist2(n5)
                                             : scn80496GetChild1Float(n4->field_0xC))
                                      : scn80496GetChild1Float(n3->field_0xC))
                               : scn80496GetChild1Float(n2->field_0xC))
                        : scn80496GetChild1Float(n1->field_0xC))
                 : scn80496GetChild1Float(self->field_0xC);
        // walk 2: last node's distance (scn80496GetChild1V6 leaves)
        CScnItemModel* m1 = self->field_0x7C4;
        if (m1 != 0) {
            CScnItemModel* m2 = m1->field_0x7C4;
            if (m2 != 0) {
                CScnItemModel* m3 = m2->field_0x7C4;
                if (m3 != 0) {
                    CScnItemModel* m4 = m3->field_0x7C4;
                    if (m4 != 0) {
                        CScnItemModel* m5 = m4->field_0x7C4;
                        if (m5 != 0) {
                            d2 = simGetLeafAnimDist(m5);
                        } else {
                            d2 = scn80496GetChild1V6(m4->field_0xC);
                        }
                    } else {
                        d2 = scn80496GetChild1V6(m3->field_0xC);
                    }
                } else {
                    d2 = scn80496GetChild1V6(m2->field_0xC);
                }
            } else {
                d2 = scn80496GetChild1V6(m1->field_0xC);
            }
        } else {
            d2 = scn80496GetChild1V6(self->field_0xC);
        }
        b = (d1 - f31) <= d2;
    }
    if (!b) {
        // materialised via srwi. (retail reuses its r3 = 0 as the path-2
        // return value, so no `li r3, 0` appears)
        bool c = self->value7B0 < lbl_eu_8066A8C0;
        if (c) {
            // walk 3: last node's distance (scn80496GetChild1V6 leaves)
            f32 d3;
            CScnItemModel* p1 = self->field_0x7C4;
            if (p1 != 0) {
                CScnItemModel* p2 = p1->field_0x7C4;
                if (p2 != 0) {
                    CScnItemModel* p3 = p2->field_0x7C4;
                    if (p3 != 0) {
                        CScnItemModel* p4 = p3->field_0x7C4;
                        if (p4 != 0) {
                            CScnItemModel* p5 = p4->field_0x7C4;
                            if (p5 != 0) {
                                d3 = simGetLeafAnimDist(p5);
                            } else {
                                d3 = scn80496GetChild1V6(p4->field_0xC);
                            }
                        } else {
                            d3 = scn80496GetChild1V6(p3->field_0xC);
                        }
                    } else {
                        d3 = scn80496GetChild1V6(p2->field_0xC);
                    }
                } else {
                    d3 = scn80496GetChild1V6(p1->field_0xC);
                }
            } else {
                d3 = scn80496GetChild1V6(self->field_0xC);
            }
            return lbl_eu_8066A8C0 >= d3;
        }
        return c;
    }
    return b;
}

// simRefreshFadeDist2: twin of simRefreshFadeDist using the scn80496GetChild2Float /
// scn80496GetChild2V6 leaf walks (simGetLeafAnimDist3 / simGetLeafAnimDist4 deep-chain calls).
extern "C" bool simRefreshFadeDist2(CScnItemModel* self) {
    bool b = self->value7B0 >= lbl_eu_8066A8C0;
    f32 f31, d1, d2;
    if (b) {
        f31 = CDeviceVI::isTvFormatPal() ? lbl_eu_8066A8CC : lbl_eu_8066A8C4;
        // walk 1: last node's distance (scn80496GetChild2Float leaves), nested-ternary
        // single assignment so MWCC shares the fmr f30, f1 at the join.
        CScnItemModel* n1 = self->field_0x7C4;
        CScnItemModel* n2 = 0;
        CScnItemModel* n3 = 0;
        CScnItemModel* n4 = 0;
        CScnItemModel* n5 = 0;
        d1 = (n1 != 0)
                 ? ((n2 = n1->field_0x7C4) != 0
                        ? ((n3 = n2->field_0x7C4) != 0
                               ? ((n4 = n3->field_0x7C4) != 0
                                      ? ((n5 = n4->field_0x7C4) != 0
                                             ? simGetLeafAnimDist3(n5)
                                             : scn80496GetChild2Float(n4->field_0xC))
                                      : scn80496GetChild2Float(n3->field_0xC))
                               : scn80496GetChild2Float(n2->field_0xC))
                        : scn80496GetChild2Float(n1->field_0xC))
                 : scn80496GetChild2Float(self->field_0xC);
        f32 d2;
        CScnItemModel* m1 = self->field_0x7C4;
        if (m1 != 0) {
            CScnItemModel* m2 = m1->field_0x7C4;
            if (m2 != 0) {
                CScnItemModel* m3 = m2->field_0x7C4;
                if (m3 != 0) {
                    CScnItemModel* m4 = m3->field_0x7C4;
                    if (m4 != 0) {
                        CScnItemModel* m5 = m4->field_0x7C4;
                        if (m5 != 0) {
                            d2 = simGetLeafAnimDist4(m5);
                        } else {
                            d2 = scn80496GetChild2V6(m4->field_0xC);
                        }
                    } else {
                        d2 = scn80496GetChild2V6(m3->field_0xC);
                    }
                } else {
                    d2 = scn80496GetChild2V6(m2->field_0xC);
                }
            } else {
                d2 = scn80496GetChild2V6(m1->field_0xC);
            }
        } else {
            d2 = scn80496GetChild2V6(self->field_0xC);
        }
        b = (d1 - f31) <= d2;
    }
    if (!b) {
        bool c = self->value7B0 < lbl_eu_8066A8C0;
        if (c) {
            f32 d3;
            CScnItemModel* p1 = self->field_0x7C4;
            if (p1 != 0) {
                CScnItemModel* p2 = p1->field_0x7C4;
                if (p2 != 0) {
                    CScnItemModel* p3 = p2->field_0x7C4;
                    if (p3 != 0) {
                        CScnItemModel* p4 = p3->field_0x7C4;
                        if (p4 != 0) {
                            CScnItemModel* p5 = p4->field_0x7C4;
                            if (p5 != 0) {
                                d3 = simGetLeafAnimDist4(p5);
                            } else {
                                d3 = scn80496GetChild2V6(p4->field_0xC);
                            }
                        } else {
                            d3 = scn80496GetChild2V6(p3->field_0xC);
                        }
                    } else {
                        d3 = scn80496GetChild2V6(p2->field_0xC);
                    }
                } else {
                    d3 = scn80496GetChild2V6(p1->field_0xC);
                }
            } else {
                d3 = scn80496GetChild2V6(self->field_0xC);
            }
            return lbl_eu_8066A8C0 >= d3;
        }
        return c;
    }
    return b;
}

// simSetLeafFlag4000: walk the field_0x7C4 chain to its last node and set/clear
// the 0x4000 flag at its 0x7A4 according to `param`. The depth-5 handle
// tail-calls the shared bit-set helper simToggleWordBits (retail shape); the
// four shallow handles inline the ori/rlwinm. Chains longer than 5 recurse
// (tail call) with the next node. extern "C" keeps the self-recursion and
// simToggleWordBits tail-call reloc names verbatim (reloc-site gate, same as
// simGetLeafAnimDist).
extern "C" void simSetLeafFlag4000(CScnItemModel* self, u32 param) {
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    CScnItemModel* n5 = n4->field_0x7C4;
                    if (n5 != 0) {
                        simSetLeafFlag4000(n5, param);
                    } else {
                        simToggleWordBits((u8*)&n4->flags7A4, 0x4000, param);
                    }
                } else {
                    if (param != 0) {
                        n3->flags7A4 |= 0x4000;
                    } else {
                        n3->flags7A4 &= ~0x4000;
                    }
                }
            } else {
                if (param != 0) {
                    n2->flags7A4 |= 0x4000;
                } else {
                    n2->flags7A4 &= ~0x4000;
                }
            }
        } else {
            if (param != 0) {
                n1->flags7A4 |= 0x4000;
            } else {
                n1->flags7A4 &= ~0x4000;
            }
        }
    } else {
        if (param != 0) {
            self->flags7A4 |= 0x4000;
        } else {
            self->flags7A4 &= ~0x4000;
        }
    }
}

// simSetLeafAnimTag: walk the field_0x7C4 chain to its last node and tail-call
// scn80496RefreshBlendTag with the node's 0xC sub-object, forwarding `tag`. The live
// tag occupies r4, so the walk rotates r5,r3,r5,r6,r3 (retail shape).
extern "C" void simSetLeafAnimTag(CScnItemModel* self, u32 tag) {
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    CScnItemModel* n5 = n4->field_0x7C4;
                    if (n5 != 0) {
                        simSetLeafAnimTag(n5, tag);
                    } else {
                        scn80496RefreshBlendTag(n4->field_0xC, tag);
                    }
                } else {
                    scn80496RefreshBlendTag(n3->field_0xC, tag);
                }
            } else {
                scn80496RefreshBlendTag(n2->field_0xC, tag);
            }
        } else {
            scn80496RefreshBlendTag(n1->field_0xC, tag);
        }
    } else {
        scn80496RefreshBlendTag(self->field_0xC, tag);
    }
}

extern "C" void simSetFlag20Link(u8* self, u32 param) {
    *(u32*)((u8*)self + 0x7A4) |= 0x20;
    *(u32*)((u8*)self + 0x7C8) = param;
}

extern "C" void simSetFlag100Link(u8* self, u32 param) {
    *(u32*)((u8*)self + 0x7A4) |= 0x100;
    *(u32*)((u8*)self + 0x7C8) = param;
}

// simPropagateEnvLgt: propagate a light-environment to every live model in both
// reference lists (0x7B4 / 0x834) and the linked model at 0x7C4. When a
// non-null env is supplied it is first copied into self's own embedded
// CScnEnvLgtData at 0x31C (copy-assign via func_804BF774); when null, self's
// env is used as the default for the recursion.
// extern "C" keeps the self-recursion reloc names verbatim (reloc-site
// gate, same as simSetLeafDist7B0).
extern "C" void simPropagateEnvLgt(CScnItemModel* self, CScnEnvLgtData* param) {
    if (param != 0) {
        func_804BF774((CScnEnvLgtData*)&((CScnItemModelActData*)self->field_0x1F8)->envLight, param);
    } else {
        param = (CScnEnvLgtData*)&((CScnItemModelActData*)self->field_0x1F8)->envLight;
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            simPropagateEnvLgt(item, param);
        }
    }
    if (self->field_0x7C4 != 0) {
        simPropagateEnvLgt(self->field_0x7C4, param);
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots834[i];
        if (item != 0) {
            simPropagateEnvLgt(item, param);
        }
    }
}

// simLinkSlot7B4: link `node` into the first free slot of the 4-slot
// reference list (0x7B4), rejecting a node that is already present, then
// notify the node via the slot-list link callback (vtable 0xD4) with `self`
// as the parent. Returns 1 when a slot was found (or the list was full), 0
// when the node was already linked. Both scans are constant-trip loops:
// MWCC fully unrolls the duplicate check, folding the first three slot
// reads to direct offsets and re-materialising a self+8 base register for
// the last two (same unroll shape as simClearNodeRefs), and strength-reduces
// the fill loop into a walked self+0x7B4 pointer while keeping the store
// address as self + i*4.
int simLinkSlot7B4(CScnItemModel* self, CScnItemModel* node) {
    for (u32 i = 0; i < 4; i++) {
        if (self->slots7B4[i] == node) return 0;
    }
    for (u32 i = 0; i < 4; i++) {
        if (self->slots7B4[i] == 0) {
            self->slots7B4[i] = node;
            node->vfuncD4(self);
            return 1;
        }
    }
    return 1;
}

// simLinkModel7C4: link `node` into self's field_0x7C4 slot (rejecting an
// already-linked node), mark node's 0x800 flag, then notify via the vtable
// method at 0xD8.
int simLinkModel7C4(CScnItemModel* self, CScnItemModel* node) {
    if (self->field_0x7C4 == node) {
        return 0;
    }
    self->field_0x7C4 = node;
    node->flags7A8 |= 0x800;
    self->vfuncD8(node);
    return 1;
}

// simClearNodeRefs: clear every reference slot equal to `node`. The slots live
// in the self+8 view; retail unrolls the 4-slot loop, folding the first slot
// to direct offsets and walking the rest via a base register.
void simClearNodeRefs(CScnItemModel* self, CScnItemModel* node) {
    // Constant-trip loop: MWCC fully unrolls it, hoisting the zero constant
    // into one register and folding the last two slots onto a self+8 base.
    // The final 0x7C8 check uses a not-equal early return (retail bnelr).
    for (int i = 0; i < 4; i++) {
        if (self->slots7B4[i] == node) self->slots7B4[i] = 0;
    }
    if (self->field_0x7C8 != node) return;
    self->field_0x7C8 = 0;
}

// simNotifyVfunc28: call the 2-arg virtual (vtable 0x28) on every live model in
// the 4-slot reference list (0x7B4), then on the linked model at 0x7C4.
void simNotifyVfunc28(CScnItemModel* self, u32 a, u32 b) {
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            item->vfunc28(a, b);
        }
    }
    if (self->field_0x7C4 != 0) {
        self->field_0x7C4->vfunc28(a, b);
    }
}

// simProbeVfunc2C: call the 1-arg virtual (vtable 0x2C) on every live model in
// the 4-slot reference list (0x7B4), then on the linked model at 0x7C4,
// returning 1 as soon as any call reports handled (non-zero).
int simProbeVfunc2C(CScnItemModel* self, u32 param) {
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            if (item->vfunc2C(param)) {
                return 1;
            }
        }
    }
    if (self->field_0x7C4 != 0) {
        if (self->field_0x7C4->vfunc2C(param)) {
            return 1;
        }
    }
    return 0;
}

// simNotifyVfunc30: call the 2-arg virtual (vtable 0x30) on every live model in
// the 4-slot reference list (0x7B4), then on the linked model at 0x7C4.
void simNotifyVfunc30(CScnItemModel* self, u32 a, u32 b) {
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            item->vfunc30(a, b);
        }
    }
    if (self->field_0x7C4 != 0) {
        self->field_0x7C4->vfunc30(a, b);
    }
}

// simProbeVfunc34: call the 1-arg virtual (vtable 0x34) on every live model in
// the 4-slot reference list (0x7B4), then on the linked model at 0x7C4,
// returning 1 as soon as any call reports handled (non-zero).
int simProbeVfunc34(CScnItemModel* self, u32 param) {
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            if (item->vfunc34(param)) {
                return 1;
            }
        }
    }
    if (self->field_0x7C4 != 0) {
        if (self->field_0x7C4->vfunc34(param)) {
            return 1;
        }
    }
    return 0;
}

// simRefreshActDist: refresh the sub-object's distance (0x2E8) from the
// transform scale, seeding the sub-object with the TV-format constant (flag
// bit 0 set) or the stored value7B0, then refresh the sub-object and the
// embedded CAttrTransform at 0x1F8.
extern "C" void simRefreshActDist(CScnItemModel* self) {
    if (self->flags7A4 & 0x80000000) {
        scn80496SetFloat178(self->field_0xC, lbl_eu_8066A8C0);
        self->flags7A4 &= ~0x80000000u;
    } else {
        scn80496SetFloat178(self->field_0xC, self->value7B0);
    }
    ((CScnItemModelActData*)self->field_0x1F8)->value2E8 =
        self->value7AC * ((CScnItemModelActData*)self->field_0x1F8)->value304;
    func_80497AA8(self->field_0xC);
    ((CScnItemModelActData*)self->field_0x1F8)->transform.update();
}

// simRefreshFadeChain: fade-distance refresh. Three 5-level chain walks run inline
// (retail shape: shallow leaves call scn80496IsNodeFlagBit / scn80496GetChild1Float /
// scn80496GetChild1V6 directly, only the depth-5 handle calls the walk helper
// simGetLeafAnimStatus / simGetLeafAnimDist2 / simGetLeafAnimDist). Walk 1 and 3 use the
// if/else form (retail: the self leaf falls into the result join); walk 2
// uses one nested-ternary single assignment (retail: the n1 leaf falls into
// the fmr f31 join placed before the self leaf - same shape as
// simRefreshFadeDist's walk 1). Guards: walk-1 result must be 0, then the last
// node's distance (f31) must be >= the TV constant, then walk-3's distance
// must be >= f31 - C4. Finally the 0x8 flag at 0x7A8 is refreshed on the
// first 4 chain nodes (scn80496GetChild1V6 distance mismatch) with the walk
// tail-calling simRefreshFlag8 / scn80496CallChild1V5. extern "C" keeps the call
// reloc names verbatim (reloc-site gate).
extern "C" void simRefreshFadeChain(CScnItemModel* self) {
    // walk 1: last node's status (simGetLeafAnimStatus / scn80496IsNodeFlagBit leaves).
    u32 result;
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    CScnItemModel* n5 = n4->field_0x7C4;
                    if (n5 != 0) {
                        result = simGetLeafAnimStatus(n5);
                    } else {
                        result = scn80496IsNodeFlagBit(n4->field_0xC);
                    }
                } else {
                    result = scn80496IsNodeFlagBit(n3->field_0xC);
                }
            } else {
                result = scn80496IsNodeFlagBit(n2->field_0xC);
            }
        } else {
            result = scn80496IsNodeFlagBit(n1->field_0xC);
        }
    } else {
        result = scn80496IsNodeFlagBit(self->field_0xC);
    }
    if (result != 0) {
        return;
    }
    // walk 2: last node's distance (simGetLeafAnimDist2 / scn80496GetChild1Float leaves),
    // nested-ternary single assignment (saved into f31 for the later checks).
    CScnItemModel* m1 = self->field_0x7C4;
    CScnItemModel* m2 = 0;
    CScnItemModel* m3 = 0;
    CScnItemModel* m4 = 0;
    CScnItemModel* m5 = 0;
    f32 f31 = (m1 != 0)
                  ? ((m2 = m1->field_0x7C4) != 0
                         ? ((m3 = m2->field_0x7C4) != 0
                                ? ((m4 = m3->field_0x7C4) != 0
                                       ? ((m5 = m4->field_0x7C4) != 0
                                              ? simGetLeafAnimDist2(m5)
                                              : scn80496GetChild1Float(m4->field_0xC))
                                       : scn80496GetChild1Float(m3->field_0xC))
                                : scn80496GetChild1Float(m2->field_0xC))
                         : scn80496GetChild1Float(m1->field_0xC))
                  : scn80496GetChild1Float(self->field_0xC);
    // The float guards are written as >= (not <) so MWCC emits the retail
    // `fcmpo; cror eq,gt,eq; bne` idiom (a < guard would lower to blt).
    if (f31 >= lbl_eu_8066A8C4) {
        // walk 3: last node's distance (simGetLeafAnimDist / scn80496GetChild1V6
        // leaves), if/else form (used immediately by the guard below).
        f32 d;
        CScnItemModel* p1 = self->field_0x7C4;
        if (p1 != 0) {
            CScnItemModel* p2 = p1->field_0x7C4;
            if (p2 != 0) {
                CScnItemModel* p3 = p2->field_0x7C4;
                if (p3 != 0) {
                    CScnItemModel* p4 = p3->field_0x7C4;
                    if (p4 != 0) {
                        CScnItemModel* p5 = p4->field_0x7C4;
                        if (p5 != 0) {
                            d = simGetLeafAnimDist(p5);
                        } else {
                            d = scn80496GetChild1V6(p4->field_0xC);
                        }
                    } else {
                        d = scn80496GetChild1V6(p3->field_0xC);
                    }
                } else {
                    d = scn80496GetChild1V6(p2->field_0xC);
                }
            } else {
                d = scn80496GetChild1V6(p1->field_0xC);
            }
        } else {
            d = scn80496GetChild1V6(self->field_0xC);
        }
        if (d >= f31 - lbl_eu_8066A8C4) {
            // refresh the 0x8 flag: first 4 chain nodes inline, the tail
            // handled by simRefreshFlag8 (chain deeper than 4) or
            // scn80496CallChild1V5 (leaf).
            if (scn80496GetChild1V6(self->field_0xC) != f31 - lbl_eu_8066A8C4) {
                self->flags7A8 |= 0x8;
            }
            CScnItemModel* q1 = self->field_0x7C4;
            if (q1 != 0) {
                if (scn80496GetChild1V6(q1->field_0xC) != f31 - lbl_eu_8066A8C4) {
                    q1->flags7A8 |= 0x8;
                }
                CScnItemModel* q2 = q1->field_0x7C4;
                if (q2 != 0) {
                    if (scn80496GetChild1V6(q2->field_0xC) != f31 - lbl_eu_8066A8C4) {
                        q2->flags7A8 |= 0x8;
                    }
                    CScnItemModel* q3 = q2->field_0x7C4;
                    if (q3 != 0) {
                        if (scn80496GetChild1V6(q3->field_0xC) != f31 - lbl_eu_8066A8C4) {
                            q3->flags7A8 |= 0x8;
                        }
                        CScnItemModel* q4 = q3->field_0x7C4;
                        if (q4 != 0) {
                            simRefreshFlag8(q4, f31 - lbl_eu_8066A8C4);
                        } else {
                            scn80496CallChild1V5(q3->field_0xC, f31 - lbl_eu_8066A8C4);
                        }
                    } else {
                        scn80496CallChild1V5(q2->field_0xC, f31 - lbl_eu_8066A8C4);
                    }
                } else {
                    scn80496CallChild1V5(q1->field_0xC, f31 - lbl_eu_8066A8C4);
                }
            } else {
                scn80496CallChild1V5(self->field_0xC, f31 - lbl_eu_8066A8C4);
            }
        }
    }
}

extern "C" void scnImN4BuildByIdx(void* a, void* b, void* c, void* d, void* e, int f);
extern "C" void simFwdAnimSetup102(u8* self, void* b, void* c, void* d, void* e) { scnImN4BuildByIdx(self, b, c, d, e, 102); }

extern "C" void func_80489C94(void* self);
extern "C" void simFwdAnimReset(void* self) { func_80489C94(self); }

// simGetTvFadeConst: return the TV-format dependent float constant.
float simGetTvFadeConst() {
    return CDeviceVI::isTvFormatPal() ? lbl_eu_8066A8CC : lbl_eu_8066A8C4;
}

extern "C" int simVtableFalse(u8* self) { return 0; }

extern "C" int simVtableTrue(u8* self) { return 1; }

extern "C" void simVtableNopVoid(void) {}

extern "C" int simVtableFalse2(u8* self) { return 0; }

extern "C" u32 simGetValue7E4(u8* self) { return ((CScnItemModel*)self)->value7E4; }

extern "C" void simSetFlag200000(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A4) |= 0x200000;
    } else {
        *(u32*)((u8*)self + 0x7A4) &= ~0x200000;
    }
}

extern "C" void simSetFlag7A8_100(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A8) |= 0x100;
    } else {
        *(u32*)((u8*)self + 0x7A8) &= ~0x100;
    }
}

extern "C" void simSetFlag8000000(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A4) |= 0x08000000;
    } else {
        *(u32*)((u8*)self + 0x7A4) &= ~0x08000000;
    }
}

extern "C" void simSetFlag2000000(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A4) |= 0x02000000;
    } else {
        *(u32*)((u8*)self + 0x7A4) &= ~0x02000000;
    }
}

// simSetFlag2000Chain: set/clear the 0x2000 flag at 0x7A4 on self and every
// model in the field_0x7C4 chain according to `param`. The first 3 chain
// nodes inline the ori/rlwinm; the depth-4 handle calls the shared bit-set
// helper simToggleWordBits; chains longer than 4 recurse. Same hand-unrolled
// register shape as simSetLeafFlag4000 (retail).
extern "C" void simSetFlag2000Chain(CScnItemModel* self, u32 param) {
    if (param != 0) {
        self->flags7A4 |= 0x2000;
    } else {
        self->flags7A4 &= ~0x2000;
    }
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        if (param != 0) {
            n1->flags7A4 |= 0x2000;
        } else {
            n1->flags7A4 &= ~0x2000;
        }
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            if (param != 0) {
                n2->flags7A4 |= 0x2000;
            } else {
                n2->flags7A4 &= ~0x2000;
            }
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                if (param != 0) {
                    n3->flags7A4 |= 0x2000;
                } else {
                    n3->flags7A4 &= ~0x2000;
                }
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    simToggleWordBits((u8*)&n4->flags7A4, 0x2000, param);
                    if (n4->field_0x7C4 != 0) {
                        simSetFlag2000Chain(n4->field_0x7C4, param);
                    }
                }
            }
        }
    }
}
// func_80482918: set/clear bit 0 of +0x7A4 on self, then recurse on every
// live model in both 4-slot reference lists (0x7B4, 0x834) and the linked
// model at 0x7C4. Declared in CScnItemModelNw4r.hpp over the nw4r overlay
// class; the referenced offsets share CScnItemModel's layout, so the walk
// runs on a CScnItemModel view. extern "C" keeps the self-recursion reloc
// name verbatim (reloc-site gate).
// The flag update mirrors simToggleWordBits's body inlined with flags = 1.
static inline void SetItemEnableBits(u32* word, u32 mask, u32 enable) {
    if (enable != 0) {
        *word |= mask;
    } else {
        *word &= ~mask;
    }
}
extern "C" void func_80482918(CScnItemModelNw4r* overlay, u32 enable) {
    CScnItemModel* self = (CScnItemModel*)overlay;
    SetItemEnableBits(&self->flags7A4, 1, enable);
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            func_80482918((CScnItemModelNw4r*)item, enable);
        }
    }
    if (self->field_0x7C4 != 0) {
        func_80482918((CScnItemModelNw4r*)self->field_0x7C4, enable);
    }
    for (int j = 0; j < 4; j++) {
        CScnItemModel* item = self->slots834[j];
        if (item != 0) {
            func_80482918((CScnItemModelNw4r*)item, enable);
        }
    }
}
// simSetFlag1MTree: like func_80482918, but toggles the 0x1000000 flag at 0x7A4
// and recurses on itself down the model tree.
extern "C" void simSetFlag1MTree(CScnItemModel* self, u32 enable) {
    if (enable != 0) {
        self->flags7A4 |= 0x1000000;
    } else {
        self->flags7A4 &= ~0x1000000;
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            simSetFlag1MTree(item, enable);
        }
    }
    if (self->field_0x7C4 != 0) {
        simSetFlag1MTree(self->field_0x7C4, enable);
    }
    for (int j = 0; j < 4; j++) {
        CScnItemModel* item = self->slots834[j];
        if (item != 0) {
            simSetFlag1MTree(item, enable);
        }
    }
}
// simRemoveWordFromBuf: remove the first field_0x7F0 element equal to `value` by
// shifting the tail down one slot and decrementing count820. count820 is
// read fresh inside the shift loop (retail reloads it each iteration).
void simRemoveWordFromBuf(CScnItemModel* self, u32 value) {
    int count = self->count820;
    for (int i = 0; i < count; i++) {
        u32 item = self->field_0x7F0[i];
        if (item == value) {
            for (int j = i; j < self->count820 - 1; j++) {
                self->field_0x7F0[j] = self->field_0x7F0[j + 1];
            }
            self->count820 = self->count820 - 1;
            return;
        }
    }
}
// func_80482B3C: set/clear the 0x80000 flag at 0x7A4 on `self`, then walk
// the model tree: for every live model in the 4-slot list, then the linked
// model, repeat the flag and recurse two levels down the reference list
// before recursing func_80482B3C itself on the deepest nodes. The 7C4 chain
// is followed 2 levels at the top (the recursion covers deeper levels).
// func_80482B3C: set/clear the 0x80000 flag at 0x7A4 on `self`, then walk
// the model tree: for every live model in the 4-slot list, then the linked
// model, repeat the flag and recurse two levels down the reference list
// before recursing func_80482B3C itself on the deepest nodes. The 7C4 chain
// is followed 2 levels at the top (the recursion covers deeper levels).
// Flag helper: set/clear the 0x80000 bit at +0x7A4 (inlined at every site).
static inline void SetFlag80(CScnItemModel* m, u32 en) {
    if (en != 0) {
        m->flags7A4 |= 0x80000;
    } else {
        m->flags7A4 &= ~0x80000;
    }
}

extern "C" void func_80482B3C(CScnItemModel* self, u32 param) {
    CScnItemModel* a;
    u32 i;
    SetFlag80(self, param);
    for (i = 0; i < 4; i++) {
        a = self->slots7B4[i];
        if (a != 0) {
            SetFlag80(a, param);
            for (u32 j = 0; j < 4; j++) {
                CScnItemModel* b = a->slots7B4[j];
                if (b != 0) {
                    SetFlag80(b, param);
                    for (u32 k = 0; k < 4; k++) {
                        CScnItemModel* c = b->slots7B4[k];
                        if (c != 0) {
                            func_80482B3C(c, param);
                        }
                    }
                    if (b->field_0x7C4 != 0) {
                        func_80482B3C(b->field_0x7C4, param);
                    }
                }
            }
            CScnItemModel* b = a->field_0x7C4;
            if (b != 0) {
                SetFlag80(b, param);
                for (u32 k = 0; k < 4; k++) {
                    CScnItemModel* c = b->slots7B4[k];
                    if (c != 0) {
                        func_80482B3C(c, param);
                    }
                }
                if (b->field_0x7C4 != 0) {
                    func_80482B3C(b->field_0x7C4, param);
                }
            }
        }
    }
    CScnItemModel* b = self->field_0x7C4;
    if (b != 0) {
        SetFlag80(b, param);
        for (u32 j = 0; j < 4; j++) {
            CScnItemModel* c = b->slots7B4[j];
            if (c != 0) {
                SetFlag80(c, param);
                for (u32 k = 0; k < 4; k++) {
                    CScnItemModel* d = c->slots7B4[k];
                    if (d != 0) {
                        func_80482B3C(d, param);
                    }
                }
                if (c->field_0x7C4 != 0) {
                    func_80482B3C(c->field_0x7C4, param);
                }
            }
        }
        CScnItemModel* c = b->field_0x7C4;
        if (c != 0) {
            SetFlag80(c, param);
            for (u32 k = 0; k < 4; k++) {
                CScnItemModel* d = c->slots7B4[k];
                if (d != 0) {
                    func_80482B3C(d, param);
                }
            }
            if (c->field_0x7C4 != 0) {
                func_80482B3C(c->field_0x7C4, param);
            }
        }
    }
}

// simToggleWordBits: set/clear the `flags` bits at self+0.
extern "C" {
__declspec(noinline) void simToggleWordBits(u8* self, u32 flags, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0) |= flags;
    } else {
        *(u32*)((u8*)self + 0) &= ~flags;
    }
}
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
extern "C" u32 lbl_eu_806624D0;   // foreign .sdata
namespace SIMBlob {
extern "C" void __dt__13CScnItemModelFv();
extern "C" void simRefreshActDist(); extern "C" void ScnCamNw4r_EmptyHook48();
extern "C" void scnItemIdNoopA(); extern "C" void simGetValue7E4();
extern "C" void simNotifyVfunc28(); extern "C" void simProbeVfunc2C();
extern "C" void simNotifyVfunc30(); extern "C" void simProbeVfunc34();
extern "C" void simSetAndPropRate(); extern "C" void simGetRate858();
extern "C" void simNopVfuncSlot(); extern "C" void simVtableFalse2();
extern "C" void func_80183264(); extern "C" void simVtableNopVoid();
extern "C" void simSetFlag7A8_100(); extern "C" void simSetFlag200000();
extern "C" void simSetFlag8000000(); extern "C" void simNotifyVfunc8C();
extern "C" void simProbeModelVec(); extern "C" void simNotifyVfunc94();
extern "C" void simNotifyReadyTree(); extern "C" void simNotifyVfunc9C();
extern "C" void simLinkSlot7B4(); extern "C" void simLinkModel7C4();
extern "C" void simNotifyVfuncB4(); extern "C" void simVtableTrue();
extern "C" void simVtableFalse(); extern "C" void simVtableFalse3();
extern "C" void simVtableFalse4(); extern "C" void simRemoveFromPool();
extern "C" void simRefreshFadeChain(); extern "C" void simSetFlag20Link();
extern "C" void simSetFlag100Link();
}

// [.data] 0x8056DD70-0x8056DE80 (272B): CScnItemModel vtable + float constants
extern "C" u32 lbl_eu_8056DD70[68] = {
    (u32)&lbl_eu_806624D0, 0x00000000, (u32)&SIMBlob::__dt__13CScnItemModelFv,
    (u32)&SIMBlob::simRefreshActDist, (u32)&SIMBlob::ScnCamNw4r_EmptyHook48, (u32)&SIMBlob::scnItemIdNoopA,
    0x00000000, (u32)&SIMBlob::simGetValue7E4, 0x00000000, 0x00000000,
    (u32)&SIMBlob::simNotifyVfunc28, (u32)&SIMBlob::simProbeVfunc2C, (u32)&SIMBlob::simNotifyVfunc30,
    (u32)&SIMBlob::simProbeVfunc34, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    (u32)&SIMBlob::simSetAndPropRate, (u32)&SIMBlob::simGetRate858, (u32)&SIMBlob::simNopVfuncSlot,
    (u32)&SIMBlob::simVtableFalse2, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    (u32)&SIMBlob::func_80183264, (u32)&SIMBlob::simVtableNopVoid, (u32)&SIMBlob::simSetFlag7A8_100,
    (u32)&SIMBlob::simSetFlag200000, (u32)&SIMBlob::simSetFlag8000000, (u32)&SIMBlob::simNotifyVfunc8C,
    (u32)&SIMBlob::simProbeModelVec, (u32)&SIMBlob::simNotifyVfunc94, (u32)&SIMBlob::simNotifyReadyTree,
    (u32)&SIMBlob::simNotifyVfunc9C, 0x00000000, 0x00000000, 0x00000000,
    (u32)&SIMBlob::simLinkSlot7B4, (u32)&SIMBlob::simLinkModel7C4, (u32)&SIMBlob::simNotifyVfuncB4,
    0x00000000, (u32)&SIMBlob::simVtableTrue, (u32)&SIMBlob::simVtableFalse,
    (u32)&SIMBlob::simVtableFalse3, (u32)&SIMBlob::simVtableFalse4, (u32)&SIMBlob::simRemoveFromPool,
    (u32)&SIMBlob::simRefreshFadeChain, (u32)&SIMBlob::simSetFlag20Link, (u32)&SIMBlob::simSetFlag100Link,
    0x00000000,
    0x3F333333, 0x3F000000, 0x3F000000, 0x3F400000, 0x3F800000, 0x3F800000,
    0x3F333333, 0x3E99999A, 0x3E99999A, 0x3E99999A, 0x3F333333, 0x3F333333,
};

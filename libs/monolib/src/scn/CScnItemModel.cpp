// Auto-scaffolded catalog TU for monolib/src/scn/CScnItemModel
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/code_804BF59C.hpp"
#include "monolib/util/MemManager.hpp"

// Cross-unit pool iterator (retail C-ABI, unmangled symbol; defined in
// CScnItemPool.cpp). Iterates the pool's reslist and clears matching
// references on every item via func_804859E8.
extern "C" void func_8048CC40(CScnItemPool* pool, CScnItemModel* item);

// Tail-called from func_80484F18 / func_804850A4 (retail C-ABI, unmangled
// symbols; defined in CScn_80496B0C.cpp; declared in CScnItemModel.hpp so
// the call reloc names stay verbatim).
// Defined below in this TU; forward-declared for func_80485684's depth-5
// tail call. noinline: retail makes a real tail call here (MWCC_CASES
// MWCC_CASES 9135 - same-TU helpers get auto-inlined otherwise).
extern "C" {
__declspec(noinline) void func_804828F0(u8* self, u32 flags, u32 enable);
}

// Same-TU chain-walk helpers (defined below in this TU); forward-declared
// so func_80483448 can inline their walk shapes with the depth-5/4 handle
// calling the helper (the retail keeps those as real calls).
extern "C" float func_80484EB0(CScnItemModel* self);
extern "C" void func_80484E5C(CScnItemModel* self, float value);
extern "C" void func_80485684(CScnItemModel* self, u32 param);
extern "C" void func_80482B3C(CScnItemModel* self, u32 param);
extern "C" u32 func_804842B0(CScnItemModel* node);

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

void func_804970D0(u8* self);
void func_8048472C(u8* self) { ((void(*)(void*))func_804970D0)((char*)self + 0xc); }

// func_80482DF4: set/clear the 0x100000 flag at 0x7A4 on self, then walk
// func_80482DF4: set/clear the 0x100000 flag at 0x7A4 on self, then walk
// the model tree: for every live model in the 4-slot list, then the linked
// model, repeat the flag and recurse two levels down the reference list
// before recursing func_80482DF4 itself on the deepest nodes. The 7C4 chain
// is followed 2 levels at the top (the recursion covers deeper levels).
// Same shape as func_80482B3C (retail sizes identical, 0x2B8); extern "C"
// keeps the self-recursion reloc name verbatim (reloc-site gate).
extern "C" void func_80482DF4(CScnItemModel* self, u32 param) {
    if (param) {
        self->flags7A4 |= 0x100000;
    } else {
        self->flags7A4 &= ~0x100000;
    }
    u32 i, j, k;
    for (i = 0; i < 4; i++) {
        CScnItemModel* a = self->slots7B4[i];
        if (a) {
            if (param) {
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

// func_804830AC: remove `self` from every item in the pool owned by the
// scene object at self->field_04. Tail-calls the pool iterator.
void func_804830AC(CScnItemModel* self) {
    func_8048CC40(self->field_04->pool, self);
}

void func_804830BC(){}

void func_804830E4(){}

void func_8048310C(){}

// retail: cnt=*(self+0x820); *(u32*)(self+cnt*4+0x7F0)=r4; *(self+0x820)=cnt+1
// Single-expression postfix form fixes the Chaitin colors (MWCC_CASES
// array-push-with-counter, func_800B5978).
extern "C" void func_80482AB8(void* self, u32 val) {
    u32* cnt = (u32*)((char*)self + 0x820);
    *(u32*)((char*)self + 0x7F0 + ((*cnt)++) * 4) = val;
}

void func_80483134(){}

// func_8048315C: walk the field_0x7C4 chain to its last node and return the
// base of its act-data region (node + 0x1F8). Hand-unrolled 5 levels with a
// self tail-call, mirroring func_80484E5C (MWCC does not auto-unroll
// pointer-chasing loops; retail is this shape). extern "C" keeps the self
// tail-call reloc name verbatim (reloc-site gate, same as func_80484E5C).
extern "C" u8* func_8048315C(CScnItemModel* self) {
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
                        return func_8048315C(n5);
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

void func_804831C4(){}

// func_80483448: model-coupling setup. When self has no linked model, create
// one: fetch the model owned by self's scene owner (func_80495E94), copy
// self's chain-last 0x7B0 fade distance onto the model's chain-last node
// (the two 0x7B0 chain walks are inlined - shallow leaves use lfs/stfs,
// depth-5/4 handles call func_80484EB0 / func_80484E5C), notify via the
// 1-arg virtual at vtable 0xB0, sync the 0x4000 / 0x400000 flags from self
// onto the model chain's last node (func_80485684 shape; the 0x4000 param is
// bit 14 of self->flags7A4), sync the 0x4 flag at 0x7A8, mark the whole
// model tree with the 0x80000 flag (func_80482B3C walk shape, recursing
// func_80482B3C(c, 1) at depth 3), then notify via vtable 0x64 and return 1.
// extern "C" keeps the call reloc names verbatim (reloc-site gate).
extern "C" u32 func_80483448(CScnItemModel* self) {
    if (self->field_0x7C4 != 0) {
        return 1;
    }
    CScnItemModel* model = func_80495E94(self->field_04);
    if (model == 0) {
        return 0;
    }
    // walk 1: last node's 0x7B0 distance of self's chain (func_80484EB0 /
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
                        value = func_80484EB0(next);
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
    // walk 2: store the distance onto model's chain last node (func_80484E5C
    // / stfs leaves).
    CScnItemModel* n1 = model->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    func_80484E5C(n4, value);
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
    // handle calls func_80485684 (which itself uses func_804828F0 deeper).
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
                        func_80485684(k5, param);
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

// func_804838DC: set/clear the 0x2 flag (bit 30) on self to match `param`
// (skipping the whole body when the flag already equals it), notify via the
// 1-arg virtual at vtable 0xA4, then recurse into every live model in both
// 4-slot reference lists and the linked model at 0x7C4. extern "C" keeps
// the self-recursion reloc names verbatim (reloc-site gate, same as
// func_80484F18).
extern "C" void func_804838DC(CScnItemModel* self, u32 param) {
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
                func_804838DC(item, param);
            }
        }
        if (self->field_0x7C4 != 0) {
            func_804838DC(self->field_0x7C4, param);
        }
        for (u32 i = 0; i < 4; i++) {
            CScnItemModel* item = self->slots834[i];
            if (item != 0) {
                func_804838DC(item, param);
            }
        }
    }
}

// func_804839D4: bind the chr animation resource to the model tree. After an
// entry guard on the resFile's chr-entry count vs `index`, walks the 7C4
// chain re-checking the count at each of the first 3 nodes (a failed check
// clears the result to 0 but still runs the trailing 0x80 flag on self).
// Deeper than 3 the function recurses on the next node. When the chain ends
// at n1/n2 the node is bound directly: func_804842B0 finds the leaf (the
// deep walk), func_80497190 binds the resource on the node's 0xC sub-object,
// and the 0x200 flag is set iff the first leaf is null but the re-walk finds
// a leaf. When self has no 7C4 child, the deep walk runs from the 7C4 / 7C8
// / self chain (func_804842B0 shape inlined twice) and self gets the same
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
                        u32 leaf = func_804842B0(n2);
                        result = func_80497190(n2->field_0xC, resFile, index, c, f, g, h);
                        if (leaf == 0 && func_804842B0(n2) != 0) {
                            n2->flags7A4 |= 0x200;
                        } else {
                            n2->flags7A4 &= ~0x200;
                        }
                    }
                    n2->flags7A4 |= 0x80;
                }
            } else {
                u32 leaf = func_804842B0(n1);
                result = func_80497190(n1->field_0xC, resFile, index, c, f, g, h);
                if (leaf == 0 && func_804842B0(n1) != 0) {
                    n1->flags7A4 |= 0x200;
                } else {
                    n1->flags7A4 &= ~0x200;
                }
            }
            n1->flags7A4 |= 0x80;
        }
    } else {
        // deep walk #1: leaf of the tree below self. E(base, 3) descent
        // (func_804842B0 shape, one level shallower than the helper: the
        // depth-3 handle calls func_804842B0 itself; H budgets 0..2).
        u32 r22;
        CScnItemModel* base = self->field_0x7C4;
        if (base != 0) {
            CScnItemModel* e1 = base->field_0x7C4;
            if (e1 != 0) {
                CScnItemModel* e2 = e1->field_0x7C4;
                if (e2 != 0) {
                    CScnItemModel* e3 = e2->field_0x7C4;
                    if (e3 != 0) {
                        r22 = func_804842B0(e3);
                    } else if (e2->field_0x7C8 != 0) {
                        r22 = func_804842B0(e2->field_0x7C8);
                    } else {
                        r22 = func_804978B8(e2->field_0xC);
                    }
                } else {
                    CScnItemModel* m1 = e1->field_0x7C8;
                    if (m1 != 0) {
                        CScnItemModel* d1 = m1->field_0x7C4;
                        if (d1 != 0) {
                            r22 = func_804842B0(d1);
                        } else if (m1->field_0x7C8 != 0) {
                            r22 = func_804842B0(m1->field_0x7C8);
                        } else {
                            r22 = func_804978B8(m1->field_0xC);
                        }
                    } else {
                        r22 = func_804978B8(e1->field_0xC);
                    }
                }
            } else {
                CScnItemModel* m1 = base->field_0x7C8;
                if (m1 != 0) {
                    CScnItemModel* d1 = m1->field_0x7C4;
                    if (d1 != 0) {
                        CScnItemModel* d2 = d1->field_0x7C4;
                        if (d2 != 0) {
                            r22 = func_804842B0(d2);
                        } else if (d1->field_0x7C8 != 0) {
                            r22 = func_804842B0(d1->field_0x7C8);
                        } else {
                            r22 = func_804978B8(d1->field_0xC);
                        }
                    } else {
                        CScnItemModel* m2 = m1->field_0x7C8;
                        if (m2 != 0) {
                            CScnItemModel* e1b = m2->field_0x7C4;
                            if (e1b != 0) {
                                r22 = func_804842B0(e1b);
                            } else if (m2->field_0x7C8 != 0) {
                                r22 = func_804842B0(m2->field_0x7C8);
                            } else {
                                r22 = func_804978B8(m2->field_0xC);
                            }
                        } else {
                            r22 = func_804978B8(m1->field_0xC);
                        }
                    }
                } else {
                    r22 = func_804978B8(base->field_0xC);
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
                            r22 = func_804842B0(e3);
                        } else if (e2->field_0x7C8 != 0) {
                            r22 = func_804842B0(e2->field_0x7C8);
                        } else {
                            r22 = func_804978B8(e2->field_0xC);
                        }
                    } else {
                        CScnItemModel* m1 = e1->field_0x7C8;
                        if (m1 != 0) {
                            CScnItemModel* d1 = m1->field_0x7C4;
                            if (d1 != 0) {
                                r22 = func_804842B0(d1);
                            } else if (m1->field_0x7C8 != 0) {
                                r22 = func_804842B0(m1->field_0x7C8);
                            } else {
                                r22 = func_804978B8(m1->field_0xC);
                            }
                        } else {
                            r22 = func_804978B8(e1->field_0xC);
                        }
                    }
                } else {
                    CScnItemModel* m1 = base2->field_0x7C8;
                    if (m1 != 0) {
                        CScnItemModel* d1 = m1->field_0x7C4;
                        if (d1 != 0) {
                            CScnItemModel* d2 = d1->field_0x7C4;
                            if (d2 != 0) {
                                r22 = func_804842B0(d2);
                            } else if (d1->field_0x7C8 != 0) {
                                r22 = func_804842B0(d1->field_0x7C8);
                            } else {
                                r22 = func_804978B8(d1->field_0xC);
                            }
                        } else {
                            CScnItemModel* m2 = m1->field_0x7C8;
                            if (m2 != 0) {
                                CScnItemModel* e1b = m2->field_0x7C4;
                                if (e1b != 0) {
                                    r22 = func_804842B0(e1b);
                                } else if (m2->field_0x7C8 != 0) {
                                    r22 = func_804842B0(m2->field_0x7C8);
                                } else {
                                    r22 = func_804978B8(m2->field_0xC);
                                }
                            } else {
                                r22 = func_804978B8(m1->field_0xC);
                            }
                        }
                    } else {
                        r22 = func_804978B8(base2->field_0xC);
                    }
                }
            } else {
                r22 = func_804978B8(self->field_0xC);
            }
        }
        result = func_80497190(self->field_0xC, resFile, index, c, f, g, h);
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
                            w2 = func_804842B0(e3);
                        } else if (e2->field_0x7C8 != 0) {
                            w2 = func_804842B0(e2->field_0x7C8);
                        } else {
                            w2 = func_804978B8(e2->field_0xC);
                        }
                    } else {
                        CScnItemModel* m1 = e1->field_0x7C8;
                        if (m1 != 0) {
                            CScnItemModel* d1 = m1->field_0x7C4;
                            if (d1 != 0) {
                                w2 = func_804842B0(d1);
                            } else if (m1->field_0x7C8 != 0) {
                                w2 = func_804842B0(m1->field_0x7C8);
                            } else {
                                w2 = func_804978B8(m1->field_0xC);
                            }
                        } else {
                            w2 = func_804978B8(e1->field_0xC);
                        }
                    }
                } else {
                    CScnItemModel* m1 = base->field_0x7C8;
                    if (m1 != 0) {
                        CScnItemModel* d1 = m1->field_0x7C4;
                        if (d1 != 0) {
                            CScnItemModel* d2 = d1->field_0x7C4;
                            if (d2 != 0) {
                                w2 = func_804842B0(d2);
                            } else if (d1->field_0x7C8 != 0) {
                                w2 = func_804842B0(d1->field_0x7C8);
                            } else {
                                w2 = func_804978B8(d1->field_0xC);
                            }
                        } else {
                            CScnItemModel* m2 = m1->field_0x7C8;
                            if (m2 != 0) {
                                CScnItemModel* e1b = m2->field_0x7C4;
                                if (e1b != 0) {
                                    w2 = func_804842B0(e1b);
                                } else if (m2->field_0x7C8 != 0) {
                                    w2 = func_804842B0(m2->field_0x7C8);
                                } else {
                                    w2 = func_804978B8(m2->field_0xC);
                                }
                            } else {
                                w2 = func_804978B8(m1->field_0xC);
                            }
                        }
                    } else {
                        w2 = func_804978B8(base->field_0xC);
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
                                w2 = func_804842B0(e3);
                            } else if (e2->field_0x7C8 != 0) {
                                w2 = func_804842B0(e2->field_0x7C8);
                            } else {
                                w2 = func_804978B8(e2->field_0xC);
                            }
                        } else {
                            CScnItemModel* m1 = e1->field_0x7C8;
                            if (m1 != 0) {
                                CScnItemModel* d1 = m1->field_0x7C4;
                                if (d1 != 0) {
                                    w2 = func_804842B0(d1);
                                } else if (m1->field_0x7C8 != 0) {
                                    w2 = func_804842B0(m1->field_0x7C8);
                                } else {
                                    w2 = func_804978B8(m1->field_0xC);
                                }
                            } else {
                                w2 = func_804978B8(e1->field_0xC);
                            }
                        }
                    } else {
                        CScnItemModel* m1 = base2->field_0x7C8;
                        if (m1 != 0) {
                            CScnItemModel* d1 = m1->field_0x7C4;
                            if (d1 != 0) {
                                CScnItemModel* d2 = d1->field_0x7C4;
                                if (d2 != 0) {
                                    w2 = func_804842B0(d2);
                                } else if (d1->field_0x7C8 != 0) {
                                    w2 = func_804842B0(d1->field_0x7C8);
                                } else {
                                    w2 = func_804978B8(d1->field_0xC);
                                }
                            } else {
                                CScnItemModel* m2 = m1->field_0x7C8;
                                if (m2 != 0) {
                                    CScnItemModel* e1b = m2->field_0x7C4;
                                    if (e1b != 0) {
                                        w2 = func_804842B0(e1b);
                                    } else if (m2->field_0x7C8 != 0) {
                                        w2 = func_804842B0(m2->field_0x7C8);
                                    } else {
                                        w2 = func_804978B8(m2->field_0xC);
                                    }
                                } else {
                                    w2 = func_804978B8(m1->field_0xC);
                                }
                            }
                        } else {
                            w2 = func_804978B8(base2->field_0xC);
                        }
                    }
                } else {
                    w2 = func_804978B8(self->field_0xC);
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

// func_80484164: walk the field_0x7C4 chain binding the chr animation
// resource. At each of the first 4 nodes (self..n3) the resFile's entry
// count must exceed `index` (signed compare), else 0 is returned; the
// walk's tail calls func_804972E8 on the last node's 0xC sub-object, and
// chains longer than 4 recurse. Nested form: the four func_804972E8
// return-handlers fall to the bottom of the function (retail order) and
// the `return 0` blocks stay inline after each count check.
extern "C" u32 func_80484164(CScnItemModel* self, CScnItemAnimResFile* resFile,
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
                    return func_80484164(n4, resFile, index, c, f);
                }
                return func_804972E8(n3->field_0xC, resFile, index, c, f);
            }
            return func_804972E8(n2->field_0xC, resFile, index, c, f);
        }
        return func_804972E8(n1->field_0xC, resFile, index, c, f);
    }
    return func_804972E8(self->field_0xC, resFile, index, c, f);
}

// func_804842B0: depth-first walk of the field_0x7C4 / field_0x7C8 tree,
// preferring the 7C4 child, down to the first leaf (both children null),
// then tail-calls func_804978B8 on the leaf's 0xC sub-object. The 7C4
// descent is unrolled 4 levels from the entry node; after a 7C8 escape the
// remaining descent budget shrinks by one (retail shape: E(node,4) with
// H(c,b) = { if c->7C8 == 0 -> func_804978B8(c+0xC); else E(c->7C8, b) }).
// Every call is a tail call, so no frame is needed (retail starts with the
// first load).
extern "C" u32 func_804842B0(CScnItemModel* node) {
    // E(node, 4): descend the 7C4 chain 4 levels.
    CScnItemModel* n1 = node->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    return func_804842B0(n4);
                }
                // H(n3, 0)
                if (n3->field_0x7C8 != 0) {
                    return func_804842B0(n3->field_0x7C8);
                }
                return func_804978B8(n3->field_0xC);
            }
            // H(n2, 1)
            CScnItemModel* m1 = n2->field_0x7C8;
            if (m1 != 0) {
                CScnItemModel* d1 = m1->field_0x7C4;
                if (d1 != 0) {
                    return func_804842B0(d1);
                }
                // H(m1, 0)
                if (m1->field_0x7C8 != 0) {
                    return func_804842B0(m1->field_0x7C8);
                }
                return func_804978B8(m1->field_0xC);
            }
            return func_804978B8(n2->field_0xC);
        }
        // H(n1, 2)
        CScnItemModel* m1 = n1->field_0x7C8;
        if (m1 != 0) {
            CScnItemModel* d1 = m1->field_0x7C4;
            if (d1 != 0) {
                CScnItemModel* d2 = d1->field_0x7C4;
                if (d2 != 0) {
                    return func_804842B0(d2);
                }
                // H(d1, 0)
                if (d1->field_0x7C8 != 0) {
                    return func_804842B0(d1->field_0x7C8);
                }
                return func_804978B8(d1->field_0xC);
            }
            // H(m1, 1)
            CScnItemModel* m2 = m1->field_0x7C8;
            if (m2 != 0) {
                CScnItemModel* e1 = m2->field_0x7C4;
                if (e1 != 0) {
                    return func_804842B0(e1);
                }
                // H(m2, 0)
                if (m2->field_0x7C8 != 0) {
                    return func_804842B0(m2->field_0x7C8);
                }
                return func_804978B8(m2->field_0xC);
            }
            return func_804978B8(m1->field_0xC);
        }
        return func_804978B8(n1->field_0xC);
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
                    return func_804842B0(d3);
                }
                // H(d2, 0)
                if (d2->field_0x7C8 != 0) {
                    return func_804842B0(d2->field_0x7C8);
                }
                return func_804978B8(d2->field_0xC);
            }
            // H(d1, 1)
            CScnItemModel* m2 = d1->field_0x7C8;
            if (m2 != 0) {
                CScnItemModel* e1 = m2->field_0x7C4;
                if (e1 != 0) {
                    return func_804842B0(e1);
                }
                // H(m2, 0)
                if (m2->field_0x7C8 != 0) {
                    return func_804842B0(m2->field_0x7C8);
                }
                return func_804978B8(m2->field_0xC);
            }
            return func_804978B8(d1->field_0xC);
        }
        // H(m1, 2)
        CScnItemModel* m2 = m1->field_0x7C8;
        if (m2 != 0) {
            CScnItemModel* e1 = m2->field_0x7C4;
            if (e1 != 0) {
                CScnItemModel* e2 = e1->field_0x7C4;
                if (e2 != 0) {
                    return func_804842B0(e2);
                }
                // H(e1, 0)
                if (e1->field_0x7C8 != 0) {
                    return func_804842B0(e1->field_0x7C8);
                }
                return func_804978B8(e1->field_0xC);
            }
            // H(m2, 1)
            CScnItemModel* m3 = m2->field_0x7C8;
            if (m3 != 0) {
                CScnItemModel* f1 = m3->field_0x7C4;
                if (f1 != 0) {
                    return func_804842B0(f1);
                }
                // H(m3, 0)
                if (m3->field_0x7C8 != 0) {
                    return func_804842B0(m3->field_0x7C8);
                }
                return func_804978B8(m3->field_0xC);
            }
            return func_804978B8(m2->field_0xC);
        }
        return func_804978B8(m1->field_0xC);
    }
    return func_804978B8(node->field_0xC);
}

// func_804844D0: walk the field_0x7C4 / field_0x7C8 tree preferring the
// 7C4 child. E(node) unrolls a 4-level 7C4 descent; when 7C4 is null at
// depth k the fallback H(node_at_depth_k, 3-k) is entered: check node->7C8,
// then descend its 7C4 chain (with the budget shrinking by 2), falling back
// to H(m, b-1) when the 7C8 node's 7C4 is null. Returns bit 9 of the
// terminal node's 0x7A4. All calls are tail calls, so no frame is needed
// (retail starts with the first load). extern "C" keeps the self-recursion
// reloc name verbatim (reloc-site gate, same as func_804842B0).
extern "C" u32 func_804844D0(CScnItemModel* node) {
    // E(node): 4-level 7C4 descent.
    CScnItemModel* n1 = node->field_0x7C4;
    if (n1 != 0) {
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    return func_804844D0(n4);
                }
                // H(n3, 0)
                if (n3->field_0x7C8 != 0) {
                    return func_804844D0(n3->field_0x7C8);
                }
                return (n3->flags7A4 >> 9) & 1;
            }
            // H(n2, 1)
            CScnItemModel* m1 = n2->field_0x7C8;
            if (m1 != 0) {
                CScnItemModel* d1 = m1->field_0x7C4;
                if (d1 != 0) {
                    return func_804844D0(d1);
                }
                // H(m1, 0)
                if (m1->field_0x7C8 != 0) {
                    return func_804844D0(m1->field_0x7C8);
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
                    return func_804844D0(d2);
                }
                // H(d1, 0)
                if (d1->field_0x7C8 != 0) {
                    return func_804844D0(d1->field_0x7C8);
                }
                return (d1->flags7A4 >> 9) & 1;
            }
            // H(m1, 1)
            CScnItemModel* m2 = m1->field_0x7C8;
            if (m2 != 0) {
                CScnItemModel* e1 = m2->field_0x7C4;
                if (e1 != 0) {
                    return func_804844D0(e1);
                }
                // H(m2, 0)
                if (m2->field_0x7C8 != 0) {
                    return func_804844D0(m2->field_0x7C8);
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
                    return func_804844D0(d3);
                }
                // H(d2, 0)
                if (d2->field_0x7C8 != 0) {
                    return func_804844D0(d2->field_0x7C8);
                }
                return (d2->flags7A4 >> 9) & 1;
            }
            // H(d1, 1)
            CScnItemModel* x1 = d1->field_0x7C8;
            if (x1 != 0) {
                CScnItemModel* y1 = x1->field_0x7C4;
                if (y1 != 0) {
                    return func_804844D0(y1);
                }
                // H(x1, 0)
                if (x1->field_0x7C8 != 0) {
                    return func_804844D0(x1->field_0x7C8);
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
                    return func_804844D0(d2);
                }
                // H(d, 0)
                if (d->field_0x7C8 != 0) {
                    return func_804844D0(d->field_0x7C8);
                }
                return (d->flags7A4 >> 9) & 1;
            }
            // H(m2, 1)
            CScnItemModel* m3 = m2->field_0x7C8;
            if (m3 != 0) {
                CScnItemModel* e1 = m3->field_0x7C4;
                if (e1 != 0) {
                    return func_804844D0(e1);
                }
                // H(m3, 0)
                if (m3->field_0x7C8 != 0) {
                    return func_804844D0(m3->field_0x7C8);
                }
                return (m3->flags7A4 >> 9) & 1;
            }
            return (m2->flags7A4 >> 9) & 1;
        }
        return (m1->flags7A4 >> 9) & 1;
    }
    return (node->flags7A4 >> 9) & 1;
}

// func_80484734: update the flags of every element in the 0x824 buffer
// holder, then notify via the 1-arg virtual at vtable 0xA4. When `param`
// is zero every element's 0x4 flag is set; otherwise each element's 0x4
// flag is synced to its 0x2 flag. Returns 0 when the model's 0x2 flag is
// clear; otherwise 1 (or the model's 0x4 flag bit for the sync path). The
// loop condition re-reads the buffer base/count every iteration (retail
// shape - MWCC cannot hoist them past the store through the loaded
// pointer).
int func_80484734(CScnItemModel* self, u32 param) {
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

// func_80484838: store the float at 0x858, then propagate it via the
// 1-arg float virtual (vtable 0x48) to every live model in the 4-slot
// reference list (0x7B4), the linked model at 0x7C4, then the second
// 4-slot list (0x834).
void func_80484838(CScnItemModel* self, float param) {
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

float func_8048490C(u8* self) { return ((CScnItemModel*)self)->value858; }

void func_80484914(CScnItemModel* self, u32 param) {
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

// func_804849E4: same triple-list traversal as func_80484914, calling the
// 1-arg virtual at vtable 0x8C.
void func_804849E4(CScnItemModel* self, u32 param) {
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

// func_80484AB4: call the 2-arg query virtual (vtable 0x90) on every live
// model in the 4-slot reference list (0x7B4), the linked model at 0x7C4,
// then the second 4-slot list (0x834). Returns whether ANY call reported
// handled - the accumulator is re-bool-ized after each call (retail shape:
// `or` first, then the neg/or/srwi setnz).
int func_80484AB4(CScnItemModel* self, u32 a, u32 b) {
    int result = 0;
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            result = (result | item->vfunc90(a, b)) != 0;
        }
    }
    if (self->field_0x7C4 != 0) {
        result = (result | self->field_0x7C4->vfunc90(a, b)) != 0;
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots834[i];
        if (item != 0) {
            result = (result | item->vfunc90(a, b)) != 0;
        }
    }
    return result;
}

// func_80484BB4: same triple-list traversal as func_80484914, calling the
// 1-arg virtual at vtable 0x94.
void func_80484BB4(CScnItemModel* self, u32 param) {
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

// func_80484C84: invoke the no-arg virtual (vtable 0x98) on every live model
// in the 4-slot reference list (0x7B4), on the linked model at 0x7C4, then
// on every live model in the second 4-slot list (0x834).
void func_80484C84(CScnItemModel* self) {
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

// func_80484D3C: call the 2-arg virtual (vtable 0x9C) on every live model in
// the 4-slot reference list (0x7B4), on the linked model at 0x7C4, then on
// every live model in the second 4-slot list (0x834).
void func_80484D3C(CScnItemModel* self, u32 a, u32 b) {
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

void func_80484E04(u8* self, u32 val) { ((CScnItemModel*)self)->value7E8 = val; }

void func_80484E0C(void) {}

extern "C" void func_80484E10(u8* self, u32 a, u32 b) {
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

// func_80484E5C: walk the field_0x7C4 chain to its last node and store
// `value` at its 0x7B0 field. Hand-unrolled 4 levels with a self tail-call
// (MWCC does not auto-unroll pointer-chasing loops; retail is this shape).
// extern "C" keeps the self tail-call reloc name verbatim (reloc-site gate,
// same as func_8025F290).
extern "C" void func_80484E5C(CScnItemModel* self, float value) {
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
                    func_80484E5C(next, value);
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

// func_80484EB0: walk the field_0x7C4 chain to its last node and return its
// 0x7B0 value. Same hand-unrolled 5-level self tail-call shape as
// func_80484E5C (MWCC does not auto-unroll pointer-chasing loops; retail is
// this shape). extern "C" keeps the self tail-call reloc name verbatim.
extern "C" float func_80484EB0(CScnItemModel* self) {
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
                        return func_80484EB0(n5);
                    }
                    return n4->value7B0;
                }
                return n3->value7B0;
            }
            return n2->value7B0;
        }
        return n1->value7B0;
    }
    return self->value7B0;
}

// func_80484F18: walk the field_0x7C4 chain to its last node and tail-call
// func_80497724 with the node's 0xC sub-object. Same hand-unrolled 5-level
// self tail-call shape as func_80484E5C. extern "C" keeps both the self and
// the func_80497724 tail-call reloc names verbatim (reloc-site gate).
extern "C" float func_80484F18(CScnItemModel* self) {
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
                        return func_80484F18(n5);
                    }
                    return func_80497724(n4->field_0xC);
                }
                return func_80497724(n3->field_0xC);
            }
            return func_80497724(n2->field_0xC);
        }
        return func_80497724(n1->field_0xC);
    }
    return func_80497724(self->field_0xC);
}

// func_80484F80: refresh the 0x8 flag at 0x7A8 on self and every model in
// the field_0x7C4 chain when the sub-object distance (func_80497724) no
// longer equals `value`. The first 4 chain nodes inline the ori; the walk's
// tail calls func_80497760 on the last node's 0xC sub-object, and chains
// longer than 4 recurse. Hand-unrolled like the other chain walks.
extern "C" void func_80484F80(CScnItemModel* self, float value) {
    if (func_80497724(self->field_0xC) != value) {
        self->flags7A8 |= 0x8;
    }
    CScnItemModel* n1 = self->field_0x7C4;
    if (n1 != 0) {
        if (func_80497724(n1->field_0xC) != value) {
            n1->flags7A8 |= 0x8;
        }
        CScnItemModel* n2 = n1->field_0x7C4;
        if (n2 != 0) {
            if (func_80497724(n2->field_0xC) != value) {
                n2->flags7A8 |= 0x8;
            }
            CScnItemModel* n3 = n2->field_0x7C4;
            if (n3 != 0) {
                if (func_80497724(n3->field_0xC) != value) {
                    n3->flags7A8 |= 0x8;
                }
                CScnItemModel* n4 = n3->field_0x7C4;
                if (n4 != 0) {
                    func_80484F80(n4, value);
                } else {
                    func_80497760(n3->field_0xC, value);
                }
            } else {
                func_80497760(n2->field_0xC, value);
            }
        } else {
            func_80497760(n1->field_0xC, value);
        }
    } else {
        func_80497760(self->field_0xC, value);
    }
}

// func_804850A4: walk the field_0x7C4 chain to its last node and tail-call
// func_80497790 with the node's 0xC sub-object. Same shape as func_80484F18.
extern "C" float func_804850A4(CScnItemModel* self) {
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
                        return func_804850A4(n5);
                    }
                    return func_80497790(n4->field_0xC);
                }
                return func_80497790(n3->field_0xC);
            }
            return func_80497790(n2->field_0xC);
        }
        return func_80497790(n1->field_0xC);
    }
    return func_80497790(self->field_0xC);
}

// func_8048510C: walk the field_0x7C4 chain to its last node and tail-call
// func_804977C0 with the node's 0xC sub-object. Five named locals reproduce
// the retail register rotation (r4,r3,r4,r5,r3); the cur/next shape used by
// the func_80484E5C family rotates differently (reg-swap-only there).
extern "C" float func_8048510C(CScnItemModel* self) {
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
                        return func_8048510C(n5);
                    }
                    return func_804977C0(n4->field_0xC);
                }
                return func_804977C0(n3->field_0xC);
            }
            return func_804977C0(n2->field_0xC);
        }
        return func_804977C0(n1->field_0xC);
    }
    return func_804977C0(self->field_0xC);
}

// func_80485174: same 5-level chain walk as func_8048510C, tail-calling
// func_804977F0 with the last node's 0xC sub-object.
extern "C" f32 func_80485174(CScnItemModel* self) {
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
                        return func_80485174(n5);
                    }
                    return func_804977F0(n4->field_0xC);
                }
                return func_804977F0(n3->field_0xC);
            }
            return func_804977F0(n2->field_0xC);
        }
        return func_804977F0(n1->field_0xC);
    }
    return func_804977F0(self->field_0xC);
}

// func_804851DC: same 5-level chain walk as func_8048510C, tail-calling
// func_8049715C with the last node's 0xC sub-object.
extern "C" u32 func_804851DC(CScnItemModel* self) {
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
                        return func_804851DC(n5);
                    }
                    return func_8049715C(n4->field_0xC);
                }
                return func_8049715C(n3->field_0xC);
            }
            return func_8049715C(n2->field_0xC);
        }
        return func_8049715C(n1->field_0xC);
    }
    return func_8049715C(self->field_0xC);
}

// func_80485244: when value7B0 reaches 1.0, re-seed the fade distances from
// the TV-format constant; returns whether the fade is still active. The two
// chain walks are written inline (MWCC does not auto-inline the 0x68-byte
// helpers; retail shows the 5-level unrolled walk with a real call to
// func_804850A4 / func_80484F18 for chains deeper than 5).
extern "C" bool func_80485244(CScnItemModel* self) {
    // b starts as the >= 1.0 test (MWCC materialises it with the extrwi.,
    // whose r3 = 0 also serves as b = false on the < 1.0 path).
    bool b = self->value7B0 >= lbl_eu_8066A8C0;
    f32 f31, d1, d2;
    if (b) {
        f31 = CDeviceVI::isTvFormatPal() ? lbl_eu_8066A8CC : lbl_eu_8066A8C4;
        // walk 1: last node's distance (func_80497790 leaves). Written as one
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
                                             ? func_804850A4(n5)
                                             : func_80497790(n4->field_0xC))
                                      : func_80497790(n3->field_0xC))
                               : func_80497790(n2->field_0xC))
                        : func_80497790(n1->field_0xC))
                 : func_80497790(self->field_0xC);
        // walk 2: last node's distance (func_80497724 leaves)
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
                            d2 = func_80484F18(m5);
                        } else {
                            d2 = func_80497724(m4->field_0xC);
                        }
                    } else {
                        d2 = func_80497724(m3->field_0xC);
                    }
                } else {
                    d2 = func_80497724(m2->field_0xC);
                }
            } else {
                d2 = func_80497724(m1->field_0xC);
            }
        } else {
            d2 = func_80497724(self->field_0xC);
        }
        b = (d1 - f31) <= d2;
    }
    if (!b) {
        // materialised via srwi. (retail reuses its r3 = 0 as the path-2
        // return value, so no `li r3, 0` appears)
        bool c = self->value7B0 < lbl_eu_8066A8C0;
        if (c) {
            // walk 3: last node's distance (func_80497724 leaves)
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
                                d3 = func_80484F18(p5);
                            } else {
                                d3 = func_80497724(p4->field_0xC);
                            }
                        } else {
                            d3 = func_80497724(p3->field_0xC);
                        }
                    } else {
                        d3 = func_80497724(p2->field_0xC);
                    }
                } else {
                    d3 = func_80497724(p1->field_0xC);
                }
            } else {
                d3 = func_80497724(self->field_0xC);
            }
            return lbl_eu_8066A8C0 >= d3;
        }
        return c;
    }
    return b;
}

// func_80485464: twin of func_80485244 using the func_804977C0 /
// func_804977F0 leaf walks (func_8048510C / func_80485174 deep-chain calls).
extern "C" bool func_80485464(CScnItemModel* self) {
    bool b = self->value7B0 >= lbl_eu_8066A8C0;
    f32 f31, d1, d2;
    if (b) {
        f31 = CDeviceVI::isTvFormatPal() ? lbl_eu_8066A8CC : lbl_eu_8066A8C4;
        // walk 1: last node's distance (func_804977C0 leaves), nested-ternary
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
                                             ? func_8048510C(n5)
                                             : func_804977C0(n4->field_0xC))
                                      : func_804977C0(n3->field_0xC))
                               : func_804977C0(n2->field_0xC))
                        : func_804977C0(n1->field_0xC))
                 : func_804977C0(self->field_0xC);
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
                            d2 = func_80485174(m5);
                        } else {
                            d2 = func_804977F0(m4->field_0xC);
                        }
                    } else {
                        d2 = func_804977F0(m3->field_0xC);
                    }
                } else {
                    d2 = func_804977F0(m2->field_0xC);
                }
            } else {
                d2 = func_804977F0(m1->field_0xC);
            }
        } else {
            d2 = func_804977F0(self->field_0xC);
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
                                d3 = func_80485174(p5);
                            } else {
                                d3 = func_804977F0(p4->field_0xC);
                            }
                        } else {
                            d3 = func_804977F0(p3->field_0xC);
                        }
                    } else {
                        d3 = func_804977F0(p2->field_0xC);
                    }
                } else {
                    d3 = func_804977F0(p1->field_0xC);
                }
            } else {
                d3 = func_804977F0(self->field_0xC);
            }
            return lbl_eu_8066A8C0 >= d3;
        }
        return c;
    }
    return b;
}

// func_80485684: walk the field_0x7C4 chain to its last node and set/clear
// the 0x4000 flag at its 0x7A4 according to `param`. The depth-5 handle
// tail-calls the shared bit-set helper func_804828F0 (retail shape); the
// four shallow handles inline the ori/rlwinm. Chains longer than 5 recurse
// (tail call) with the next node. extern "C" keeps the self-recursion and
// func_804828F0 tail-call reloc names verbatim (reloc-site gate, same as
// func_80484F18).
extern "C" void func_80485684(CScnItemModel* self, u32 param) {
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
                        func_80485684(n5, param);
                    } else {
                        func_804828F0((u8*)&n4->flags7A4, 0x4000, param);
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

// func_80485774: walk the field_0x7C4 chain to its last node and tail-call
// func_8049782C with the node's 0xC sub-object, forwarding `tag`. The live
// tag occupies r4, so the walk rotates r5,r3,r5,r6,r3 (retail shape).
extern "C" void func_80485774(CScnItemModel* self, u32 tag) {
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
                        func_80485774(n5, tag);
                    } else {
                        func_8049782C(n4->field_0xC, tag);
                    }
                } else {
                    func_8049782C(n3->field_0xC, tag);
                }
            } else {
                func_8049782C(n2->field_0xC, tag);
            }
        } else {
            func_8049782C(n1->field_0xC, tag);
        }
    } else {
        func_8049782C(self->field_0xC, tag);
    }
}

extern "C" void func_804857DC(u8* self, u32 param) {
    *(u32*)((u8*)self + 0x7A4) |= 0x20;
    *(u32*)((u8*)self + 0x7C8) = param;
}

extern "C" void func_804857F0(u8* self, u32 param) {
    *(u32*)((u8*)self + 0x7A4) |= 0x100;
    *(u32*)((u8*)self + 0x7C8) = param;
}

// func_80485804: propagate a light-environment to every live model in both
// reference lists (0x7B4 / 0x834) and the linked model at 0x7C4. When a
// non-null env is supplied it is first copied into self's own embedded
// CScnEnvLgtData at 0x31C (copy-assign via func_804BF774); when null, self's
// env is used as the default for the recursion.
// extern "C" keeps the self-recursion reloc names verbatim (reloc-site
// gate, same as func_80484E5C).
extern "C" void func_80485804(CScnItemModel* self, CScnEnvLgtData* param) {
    if (param != 0) {
        func_804BF774((CScnEnvLgtData*)&((CScnItemModelActData*)self->field_0x1F8)->envLight, param);
    } else {
        param = (CScnEnvLgtData*)&((CScnItemModelActData*)self->field_0x1F8)->envLight;
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots7B4[i];
        if (item != 0) {
            func_80485804(item, param);
        }
    }
    if (self->field_0x7C4 != 0) {
        func_80485804(self->field_0x7C4, param);
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* item = self->slots834[i];
        if (item != 0) {
            func_80485804(item, param);
        }
    }
}

// func_804858C8: link `node` into the first free slot of the 4-slot
// reference list (0x7B4), rejecting a node that is already present, then
// notify the node via the slot-list link callback (vtable 0xD4) with `self`
// as the parent. Returns 1 when a slot was found (or the list was full), 0
// when the node was already linked. Both scans are constant-trip loops:
// MWCC fully unrolls the duplicate check, folding the first three slot
// reads to direct offsets and re-materialising a self+8 base register for
// the last two (same unroll shape as func_804859E8), and strength-reduces
// the fill loop into a walked self+0x7B4 pointer while keeping the store
// address as self + i*4.
int func_804858C8(CScnItemModel* self, CScnItemModel* node) {
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

// func_80485994: link `node` into self's field_0x7C4 slot (rejecting an
// already-linked node), mark node's 0x800 flag, then notify via the vtable
// method at 0xD8.
int func_80485994(CScnItemModel* self, CScnItemModel* node) {
    if (self->field_0x7C4 == node) {
        return 0;
    }
    self->field_0x7C4 = node;
    node->flags7A8 |= 0x800;
    self->vfuncD8(node);
    return 1;
}

// func_804859E8: clear every reference slot equal to `node`. The slots live
// in the self+8 view; retail unrolls the 4-slot loop, folding the first slot
// to direct offsets and walking the rest via a base register.
void func_804859E8(CScnItemModel* self, CScnItemModel* node) {
    // Constant-trip loop: MWCC fully unrolls it, hoisting the zero constant
    // into one register and folding the last two slots onto a self+8 base.
    // The final 0x7C8 check uses a not-equal early return (retail bnelr).
    for (int i = 0; i < 4; i++) {
        if (self->slots7B4[i] == node) self->slots7B4[i] = 0;
    }
    if (self->field_0x7C8 != node) return;
    self->field_0x7C8 = 0;
}

// func_80485A48: call the 2-arg virtual (vtable 0x28) on every live model in
// the 4-slot reference list (0x7B4), then on the linked model at 0x7C4.
void func_80485A48(CScnItemModel* self, u32 a, u32 b) {
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

// func_80485AD8: call the 1-arg virtual (vtable 0x2C) on every live model in
// the 4-slot reference list (0x7B4), then on the linked model at 0x7C4,
// returning 1 as soon as any call reports handled (non-zero).
int func_80485AD8(CScnItemModel* self, u32 param) {
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

// func_80485B98: call the 2-arg virtual (vtable 0x30) on every live model in
// the 4-slot reference list (0x7B4), then on the linked model at 0x7C4.
void func_80485B98(CScnItemModel* self, u32 a, u32 b) {
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

// func_80485C28: call the 1-arg virtual (vtable 0x34) on every live model in
// the 4-slot reference list (0x7B4), then on the linked model at 0x7C4,
// returning 1 as soon as any call reports handled (non-zero).
int func_80485C28(CScnItemModel* self, u32 param) {
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

// func_80485CE8: refresh the sub-object's distance (0x2E8) from the
// transform scale, seeding the sub-object with the TV-format constant (flag
// bit 0 set) or the stored value7B0, then refresh the sub-object and the
// embedded CAttrTransform at 0x1F8.
extern "C" void func_80485CE8(CScnItemModel* self) {
    if (self->flags7A4 & 0x80000000) {
        func_8049771C(self->field_0xC, lbl_eu_8066A8C0);
        self->flags7A4 &= ~0x80000000u;
    } else {
        func_8049771C(self->field_0xC, self->value7B0);
    }
    ((CScnItemModelActData*)self->field_0x1F8)->value2E8 =
        self->value7AC * ((CScnItemModelActData*)self->field_0x1F8)->value304;
    func_80497AA8(self->field_0xC);
    ((CScnItemModelActData*)self->field_0x1F8)->transform.update();
}

// func_80485D64: fade-distance refresh. Three 5-level chain walks run inline
// (retail shape: shallow leaves call func_8049715C / func_80497790 /
// func_80497724 directly, only the depth-5 handle calls the walk helper
// func_804851DC / func_804850A4 / func_80484F18). Walk 1 and 3 use the
// if/else form (retail: the self leaf falls into the result join); walk 2
// uses one nested-ternary single assignment (retail: the n1 leaf falls into
// the fmr f31 join placed before the self leaf - same shape as
// func_80485244's walk 1). Guards: walk-1 result must be 0, then the last
// node's distance (f31) must be >= the TV constant, then walk-3's distance
// must be >= f31 - C4. Finally the 0x8 flag at 0x7A8 is refreshed on the
// first 4 chain nodes (func_80497724 distance mismatch) with the walk
// tail-calling func_80484F80 / func_80497760. extern "C" keeps the call
// reloc names verbatim (reloc-site gate).
extern "C" void func_80485D64(CScnItemModel* self) {
    // walk 1: last node's status (func_804851DC / func_8049715C leaves).
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
                        result = func_804851DC(n5);
                    } else {
                        result = func_8049715C(n4->field_0xC);
                    }
                } else {
                    result = func_8049715C(n3->field_0xC);
                }
            } else {
                result = func_8049715C(n2->field_0xC);
            }
        } else {
            result = func_8049715C(n1->field_0xC);
        }
    } else {
        result = func_8049715C(self->field_0xC);
    }
    if (result != 0) {
        return;
    }
    // walk 2: last node's distance (func_804850A4 / func_80497790 leaves),
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
                                              ? func_804850A4(m5)
                                              : func_80497790(m4->field_0xC))
                                       : func_80497790(m3->field_0xC))
                                : func_80497790(m2->field_0xC))
                         : func_80497790(m1->field_0xC))
                  : func_80497790(self->field_0xC);
    // The float guards are written as >= (not <) so MWCC emits the retail
    // `fcmpo; cror eq,gt,eq; bne` idiom (a < guard would lower to blt).
    if (f31 >= lbl_eu_8066A8C4) {
        // walk 3: last node's distance (func_80484F18 / func_80497724
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
                            d = func_80484F18(p5);
                        } else {
                            d = func_80497724(p4->field_0xC);
                        }
                    } else {
                        d = func_80497724(p3->field_0xC);
                    }
                } else {
                    d = func_80497724(p2->field_0xC);
                }
            } else {
                d = func_80497724(p1->field_0xC);
            }
        } else {
            d = func_80497724(self->field_0xC);
        }
        if (d >= f31 - lbl_eu_8066A8C4) {
            // refresh the 0x8 flag: first 4 chain nodes inline, the tail
            // handled by func_80484F80 (chain deeper than 4) or
            // func_80497760 (leaf).
            if (func_80497724(self->field_0xC) != f31 - lbl_eu_8066A8C4) {
                self->flags7A8 |= 0x8;
            }
            CScnItemModel* q1 = self->field_0x7C4;
            if (q1 != 0) {
                if (func_80497724(q1->field_0xC) != f31 - lbl_eu_8066A8C4) {
                    q1->flags7A8 |= 0x8;
                }
                CScnItemModel* q2 = q1->field_0x7C4;
                if (q2 != 0) {
                    if (func_80497724(q2->field_0xC) != f31 - lbl_eu_8066A8C4) {
                        q2->flags7A8 |= 0x8;
                    }
                    CScnItemModel* q3 = q2->field_0x7C4;
                    if (q3 != 0) {
                        if (func_80497724(q3->field_0xC) != f31 - lbl_eu_8066A8C4) {
                            q3->flags7A8 |= 0x8;
                        }
                        CScnItemModel* q4 = q3->field_0x7C4;
                        if (q4 != 0) {
                            func_80484F80(q4, f31 - lbl_eu_8066A8C4);
                        } else {
                            func_80497760(q3->field_0xC, f31 - lbl_eu_8066A8C4);
                        }
                    } else {
                        func_80497760(q2->field_0xC, f31 - lbl_eu_8066A8C4);
                    }
                } else {
                    func_80497760(q1->field_0xC, f31 - lbl_eu_8066A8C4);
                }
            } else {
                func_80497760(self->field_0xC, f31 - lbl_eu_8066A8C4);
            }
        }
    }
}

extern "C" void func_80489A60(void* a, void* b, void* c, void* d, void* e, int f);
extern "C" void func_80486068(u8* self, void* b, void* c, void* d, void* e) { func_80489A60(self, b, c, d, e, 102); }

extern "C" void func_80489C94(void* self);
extern "C" void func_80486070(void* self) { func_80489C94(self); }

// func_eu_8048A084: return the TV-format dependent float constant.
float func_eu_8048A084() {
    return CDeviceVI::isTvFormatPal() ? lbl_eu_8066A8CC : lbl_eu_8066A8C4;
}

extern "C" int func_80486074(u8* self) { return 0; }

extern "C" int func_8048607C(u8* self) { return 1; }

extern "C" void func_80486084(void) {}

extern "C" int func_80486088(u8* self) { return 0; }

extern "C" u32 func_80486090(u8* self) { return ((CScnItemModel*)self)->value7E4; }

extern "C" void func_804830BC(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A4) |= 0x200000;
    } else {
        *(u32*)((u8*)self + 0x7A4) &= ~0x200000;
    }
}

extern "C" void func_804830E4(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A8) |= 0x100;
    } else {
        *(u32*)((u8*)self + 0x7A8) &= ~0x100;
    }
}

extern "C" void func_8048310C(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A4) |= 0x08000000;
    } else {
        *(u32*)((u8*)self + 0x7A4) &= ~0x08000000;
    }
}

extern "C" void func_80483134(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A4) |= 0x02000000;
    } else {
        *(u32*)((u8*)self + 0x7A4) &= ~0x02000000;
    }
}

// func_804827DC: set/clear the 0x2000 flag at 0x7A4 on self and every
// model in the field_0x7C4 chain according to `param`. The first 3 chain
// nodes inline the ori/rlwinm; the depth-4 handle calls the shared bit-set
// helper func_804828F0; chains longer than 4 recurse. Same hand-unrolled
// register shape as func_80485684 (retail).
extern "C" void func_804827DC(CScnItemModel* self, u32 param) {
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
                    func_804828F0((u8*)&n4->flags7A4, 0x2000, param);
                    if (n4->field_0x7C4 != 0) {
                        func_804827DC(n4->field_0x7C4, param);
                    }
                }
            }
        }
    }
}
extern "C" void func_80482918() {}
extern "C" void func_804829E8() {}
// func_80482AD4: remove the first field_0x7F0 element equal to `value` by
// shifting the tail down one slot and decrementing count820. count820 is
// read fresh inside the shift loop (retail reloads it each iteration).
void func_80482AD4(CScnItemModel* self, u32 value) {
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
extern "C" void func_80482B3C(CScnItemModel* self, u32 param) {
    if (param != 0) {
        self->flags7A4 |= 0x80000;
    } else {
        self->flags7A4 &= ~0x80000;
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModel* a = self->slots7B4[i];
        if (a != 0) {
            if (param != 0) {
                a->flags7A4 |= 0x80000;
            } else {
                a->flags7A4 &= ~0x80000;
            }
            for (u32 j = 0; j < 4; j++) {
                CScnItemModel* b = a->slots7B4[j];
                if (b != 0) {
                    if (param != 0) {
                        b->flags7A4 |= 0x80000;
                    } else {
                        b->flags7A4 &= ~0x80000;
                    }
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
                if (param != 0) {
                    b->flags7A4 |= 0x80000;
                } else {
                    b->flags7A4 &= ~0x80000;
                }
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
        if (param != 0) {
            b->flags7A4 |= 0x80000;
        } else {
            b->flags7A4 &= ~0x80000;
        }
        for (u32 j = 0; j < 4; j++) {
            CScnItemModel* c = b->slots7B4[j];
            if (c != 0) {
                if (param != 0) {
                    c->flags7A4 |= 0x80000;
                } else {
                    c->flags7A4 &= ~0x80000;
                }
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
            if (param != 0) {
                c->flags7A4 |= 0x80000;
            } else {
                c->flags7A4 &= ~0x80000;
            }
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

// func_804828F0: set/clear the `flags` bits at self+0.
extern "C" {
__declspec(noinline) void func_804828F0(u8* self, u32 flags, u32 enable) {
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
extern "C" void func_80485CE8(); extern "C" void func_80482048();
extern "C" void func_80482288(); extern "C" void func_80486090();
extern "C" void func_80485A48(); extern "C" void func_80485AD8();
extern "C" void func_80485B98(); extern "C" void func_80485C28();
extern "C" void func_80484838(); extern "C" void func_8048490C();
extern "C" void func_80484E0C(); extern "C" void func_80486088();
extern "C" void func_80183264(); extern "C" void func_80486084();
extern "C" void func_804830E4(); extern "C" void func_804830BC();
extern "C" void func_8048310C(); extern "C" void func_804849E4();
extern "C" void func_80484AB4(); extern "C" void func_80484BB4();
extern "C" void func_80484C84(); extern "C" void func_80484D3C();
extern "C" void func_804858C8(); extern "C" void func_80485994();
extern "C" void func_80484914(); extern "C" void func_8048607C();
extern "C" void func_80486074(); extern "C" void func_801FCAC0();
extern "C" void func_801FCBEC(); extern "C" void func_804830AC();
extern "C" void func_80485D64(); extern "C" void func_804857DC();
extern "C" void func_804857F0();
}

// [.data] 0x8056DD70-0x8056DE80 (272B): CScnItemModel vtable + float constants
extern "C" u32 lbl_eu_8056DD70[68] = {
    (u32)&lbl_eu_806624D0, 0x00000000, (u32)&SIMBlob::__dt__13CScnItemModelFv,
    (u32)&SIMBlob::func_80485CE8, (u32)&SIMBlob::func_80482048, (u32)&SIMBlob::func_80482288,
    0x00000000, (u32)&SIMBlob::func_80486090, 0x00000000, 0x00000000,
    (u32)&SIMBlob::func_80485A48, (u32)&SIMBlob::func_80485AD8, (u32)&SIMBlob::func_80485B98,
    (u32)&SIMBlob::func_80485C28, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    (u32)&SIMBlob::func_80484838, (u32)&SIMBlob::func_8048490C, (u32)&SIMBlob::func_80484E0C,
    (u32)&SIMBlob::func_80486088, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    (u32)&SIMBlob::func_80183264, (u32)&SIMBlob::func_80486084, (u32)&SIMBlob::func_804830E4,
    (u32)&SIMBlob::func_804830BC, (u32)&SIMBlob::func_8048310C, (u32)&SIMBlob::func_804849E4,
    (u32)&SIMBlob::func_80484AB4, (u32)&SIMBlob::func_80484BB4, (u32)&SIMBlob::func_80484C84,
    (u32)&SIMBlob::func_80484D3C, 0x00000000, 0x00000000, 0x00000000,
    (u32)&SIMBlob::func_804858C8, (u32)&SIMBlob::func_80485994, (u32)&SIMBlob::func_80484914,
    0x00000000, (u32)&SIMBlob::func_8048607C, (u32)&SIMBlob::func_80486074,
    (u32)&SIMBlob::func_801FCAC0, (u32)&SIMBlob::func_801FCBEC, (u32)&SIMBlob::func_804830AC,
    (u32)&SIMBlob::func_80485D64, (u32)&SIMBlob::func_804857DC, (u32)&SIMBlob::func_804857F0,
    0x00000000,
    0x3F333333, 0x3F000000, 0x3F000000, 0x3F400000, 0x3F800000, 0x3F800000,
    0x3F333333, 0x3E99999A, 0x3E99999A, 0x3E99999A, 0x3F333333, 0x3F333333,
};

// Auto-scaffolded catalog TU for monolib/src/scn/CScnEffectActNw4r
// Replace stubs with high-level C/C++ during decomp.

/* "libs/monolib/src/scn/CScnEffectActNw4r.cpp" line 3 "libs/monolib/src/scn/CScnEffectActNw4r.hpp" */
/* end "libs/monolib/src/scn/CScnEffectActNw4r.hpp" */
/* "libs/monolib/src/scn/CScnEffectActNw4r.cpp" line 4 "libs/monolib/src/scn/CScnItemModel.hpp" */
/* end "libs/monolib/src/scn/CScnItemModel.hpp" */

// Forward declarations for retail functions defined in other TUs
bool func_80495E60(CScnItemModel* self);
void func_80484E5C(CScnItemModel* self);
void func_804838DC(CScnItemModel* self);
CScnItemModel* func_8048315C(CScnItemModel* self);
void func_80482AB8(CScnItemModel* self);

// Thunk: load field_0x4 and tail-call
bool func_8049BEA4(CScnEffectActNw4r* self) {
    return func_80495E60(self->field_0x4);
}

// Thunk: load field_0x4 and tail-call
void func_8049BEAC(CScnEffectActNw4r* self) {
    func_80484E5C(self->field_0x4);
}

// Thunk: load field_0x4 and tail-call
void func_8049BEB4(CScnEffectActNw4r* self) {
    func_804838DC(self->field_0x4);
}

// Thunk: load field_0x4 and tail-call (forwards CScnItemModel* return)
CScnItemModel* func_8049BEE4(CScnEffectActNw4r* self) {
    return func_8048315C(self->field_0x4);
}

void func_8049BEEC(){}

void func_8049BF0C(){}

void func_8049BF34(){}

void func_8049BF5C(){}

void func_8049BF84(){}

void func_8049BFAC(){}

void func_8049C060(){}

void func_8049C18C(){}

void func_8049C244(){}

void func_8049C314(){}

// Thunk: load field_0x4 and tail-call
void func_8049C394(CScnEffectActNw4r* self) {
    func_80482AB8(self->field_0x4);
}

extern void func_80482AD4();
void func_8049C39C(void* self){ func_80482AD4(); }

extern void func_8048856C();
void func_8049C3A4(void* self){ func_8048856C(); }

void func_8049C3AC(){}

void func_8049C420(){}

extern "C" u32 func_8049C430(void* self) { return *(u32*)((u8*)self + 0x188); }

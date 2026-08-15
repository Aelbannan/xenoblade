#include "monolib/util/CChildListNode.hpp"
#include "monolib/data_vtables.hpp"

// ==== blob monolibdata1/1d dissolve: retail data owned by this TU ====
// rodata 0x80522848 (0x21): RTTI class-name string for
// TChildListHeader<CChildListNode>.
const char lbl_eu_80522848[] = "TChildListHeader<CChildListNode>";
// sdata 0x806635E0 (0x8): RTTI typeinfo { class-name, cast-table=0 }.
u32 lbl_eu_806635E0[2] = { (u32)&lbl_eu_80522848, 0 };
// data 0x8056BBA0 (0x10): CChildListNode vtable {&lbl_eu_80661950, 0,
// &__dt__14CChildListNodeFv, &Reset__14CChildListNodeFv} (the ctor below
// stores this label at +0x10).
void* lbl_eu_8056BBA0[4] = {
    (void*)&lbl_eu_80661950, 0, (void*)&__dt__14CChildListNodeFv,
    (void*)&Reset__14CChildListNodeFv,
};
/* UNRESOLVED: lbl_eu_8056BBB0 (0x10, TChildListHeader<CChildListNode> vtable
 * {&lbl_eu_806635E0, 0, &__dt__34TChildListHeader<14CChildListNode>Fv, 0}) —
 * the dtor reloc name cannot be written in C++ source (MWCC rejects '<' in
 * extern "C" ids and &T::~T). MWCC emits the equivalent weak
 * __vt__34TChildListHeader<14CChildListNode> (0xC, .data, dtor reloc correct)
 * in this TU; coordinator must rename that to lbl_eu_8056BBB0 / retarget the
 * dtor slot. Not faked. */

CChildListNode::CChildListNode()
    : mParent(NULL) {

    // vptr slots (novtable): CChildListNode vtable at +0x10 (overrides the
    // base CDoubleListNode vptr), TChildListHeader vtable at +0x34 (mChildren).
    // Retail store order: mParent, vptr@0x10, vptr@0x34, mChildren.Reset().
    *(void**)((char*)this + 0x10) = (void*)lbl_eu_8056BBA0;
    *(void**)((char*)this + 0x34) = (void*)lbl_eu_8056BBB0;
    mChildren.Reset();
}

void CChildListNode::Reset() {
    CDoubleListNode::Reset();
    mParent = NULL;
    mChildren.Reset();
}

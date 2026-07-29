// Auto-scaffolded catalog TU for kyoshin/CMapSel
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMapSel.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"

// External function declarations (C linkage to match retail symbols)
extern "C" void func_80124270(nw4r::lyt::Pane*, u32);
extern "C" void func_801D216C(void*, u8);
extern "C" void func_801F34F4(void*);

// Forward declarations for local functions
extern void func_80243ED8(CMapSelFull*);
extern void func_80244020(CMapSelFull*);

// String table and float constants
extern "C" char lbl_eu_8050B4A8[];
extern "C" float lbl_eu_8066873C;

void __ct__CMapSel(){}

CMapSel::~CMapSel() {}

// Initializes the map select widget: reads the layout archive file,
// marks the widget as loading, runs layout setup, and initializes
// the sub-object at offset 0x34.
void func_8024343C(CMapSelFull* self) {
    self->mFileHandle = (u32)CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(),
        lbl_eu_8050B4A8 + 0xC,
        (IWorkEvent*)self,
        0,
        0
    );
    self->field_0xAF = 1;
    func_80243ED8(self);
    func_801F34F4(self->_34);
}

void func_802434A0(){}

void func_80243560(){}

void func_802435CC(){}

void func_80243680(){}

// FULL_MATCH: reads the initial-setup flag at +0x33 (set to 1 in ctor, cleared on play/close)

void func_802436CC(){}

void func_8024371C(){}

void func_80243768(){}

void func_80243838(){}

void func_8024391C(){}

void func_802439CC(){}

u8 func_80243A9C(void* self){
    CMapSelFull* s = static_cast<CMapSelFull*>(self);
    s8 x = s->field_8D;
    s8 y = s->field_8C;
    return s->mGridData[x + y];
}

void func_80243ABC(){}

// Transitions the widget to state 3 (closing): waits for anim transform 2
// to complete, then sets state=3/flag=1, shows the CBaseCur at +0x74,
// and runs post-close setup via func_80244020.
void func_80243B88(CMapSelFull* self) {
    if (func_80137444((nw4r::lyt::AnimTransform*)self->mAnimTransform2, lbl_eu_8066873C)) {
        self->mState = 3;
        self->mFlag33 = 1;
        func_801D216C(self->_74, 1);
        func_80244020(self);
    }
}

void func_80243BE8(){}

void func_80243C6C(){}

void func_80243CB8(){}

void func_80243CFC(){}

void func_80243E08(){}

void func_80243ED8(){}

// Hides a named pane in the layout. Sets field_0xAF = 1 (loading flag)
// and field_0xB0 = 1.0f (animation progress), then finds a pane by name
// in the root pane and hides it.
void func_80243FC4(CMapSelFull* self) {
    float f = lbl_eu_8066873C;
    self->field_0xAF = 1;
    self->field_0xB0 = f;
    nw4r::lyt::Layout* l = (nw4r::lyt::Layout*)self->mLayout;
    func_80124270(l->GetRootPane()->FindPaneByName(&lbl_eu_8050B4A8[0x66], 1), 0);
}

void func_80244020(){}

void CMapSel::OnFileEvent() {}

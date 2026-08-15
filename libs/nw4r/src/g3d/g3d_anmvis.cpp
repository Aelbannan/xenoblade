#include <nw4r/g3d.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace g3d {

// NOTE: the retail split for g3d_anmvis.cpp contains ONLY the two free
// ApplyVisAnmResult functions (0x803E3E04-0x803E3F4C, 0x148 bytes). The
// AnmObjVis / AnmObjVisNode / AnmObjVisOR / AnmObjVisRes class methods are
// NOT referenced by the game, so the retail linker garbage-collected them;
// the DOL-extracted retail object has no .data/.rodata/.sdata2 and no class
// code. Reconstructing the classes here would overshoot the split budget and
// emit vtables/type-names the retail object does not carry, so this TU only
// defines the two surviving functions. The class declarations (with the
// inline TestExistence/TestDefined) live in g3d_anmvis.h.

/******************************************************************************
 *
 * ApplyVisAnmResult
 *
 ******************************************************************************/
void ApplyVisAnmResult(ResMdl mdl, AnmObjVis* pObj) {
    u32 numNode = mdl.GetResNodeNumEntries();

    for (u32 i = 0; i < numNode; i++) {
        if (!pObj->TestExistence(i)) {
            continue;
        }

        ResNode node = mdl.GetResNode(i);
        node.SetVisibility(pObj->GetResult(i));
    }
}

void ApplyVisAnmResult(u8* byteVec, ResMdl mdl, AnmObjVis* pObj) {
    u32 numNode = mdl.GetResNodeNumEntries();

    for (u32 i = 0; i < numNode; i++) {
        if (!pObj->TestExistence(i)) {
            continue;
        }

        byteVec[i] = pObj->GetResult(i);
    }
}

} // namespace g3d
} // namespace nw4r

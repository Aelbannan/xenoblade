#pragma once

#include <types.h>

class CMCCrystalSupport;
namespace nw4r { namespace lyt { class Layout; class DrawInfo; } }

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(
    nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
extern "C" void func_8022EB0C(CMCCrystalSupport* self);

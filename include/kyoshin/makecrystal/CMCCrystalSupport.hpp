#pragma once

#include <types.h>

class CMCCrystalSupport;
namespace nw4r { namespace lyt { class Layout; class DrawInfo; } }

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
void drawLayout(nw4r::lyt::Layout* layout, nw4r::lyt::DrawInfo* drawInfo,
                int, int);  // code_80135FDC.cpp
extern "C" void func_8022EB0C(CMCCrystalSupport* self);

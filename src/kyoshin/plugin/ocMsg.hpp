#pragma once

#include "monolib/vm/yvm2.h"

#ifdef __cplusplus
extern "C" {
#endif

void ocMsgRegist();

#ifdef __cplusplus
}
#endif

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" void* func_8003A4E0();
extern "C" void* func_8003A4EC();
extern "C" BOOL vmOCRegist(OCData* pOC);

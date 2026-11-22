#pragma once

#include <types.h>

namespace cf {
struct CtrlObjectParamArtsList;  // defined in src/kyoshin/cf/CtrlObjectParam.hpp
}

// ---------------------------------------------------------------------------
// C-linkage imports
// ---------------------------------------------------------------------------

struct GlobalStruct_80572B94;

// ---------------------------------------------------------------------------
// Data objects referenced from this TU (unmangled global names; plain extern
// at global scope -- MWCC does not mangle global variable names)
// ---------------------------------------------------------------------------
extern char sHeapRegionBlocks[];
extern char sHeapList78[];
extern char sHeapList38[];
// Each stage table holds 14 byte offsets (indexed by the callers).
struct LblPtrTable {
    u32 slots[14];
};
extern LblPtrTable* sKyoshinHeapOffsetTablePtrs0[6];   // name-guess: array read by KyoshinHeap_GetPtrTblA
extern LblPtrTable* sKyoshinHeapOffsetTablePtrs1[6];   // name-guess: array read by KyoshinHeap_GetPtrTblB
extern char sHeapSingletonRegistration[];   // .bss: global-object registration node
extern char sHeapList58[];   // .bss
extern s8 sKyoshinHeapInitialized;       // .sbss: singleton-initialized flag

// External runtime / game functions (retail-unmangled symbols)
extern "C" void CtrlObjectParam_MergeArtsListNode(void*, void*);
extern "C" int func_800A3594(void*, int, int);
extern "C" int func_800A36A4(void*, int, int);
extern "C" u32 CtrlObjectParam_GetArtsListTotalSize(void*);
extern "C" void KyoshinHeap_CreateRegions(GlobalStruct_80572B94*);
extern "C" int func_800A39E8(void*, void*, void*, void*);
extern "C" void CtrlObjectParam_InitArtsListEntry(void*, int, int, int);
extern "C" void* __register_global_object(void* object, void* destructor, void* registration);
extern "C" void CtrlObjectParam_InitListElement(void*);

extern "C" void __dt__800A34E0();

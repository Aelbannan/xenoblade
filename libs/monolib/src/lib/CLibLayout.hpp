#pragma once

#include <types.h>
#include "monolib/work/CWorkThread.hpp"
#include "monolib/util/MemManager.hpp"
#include <revolution/mem/mem_allocator.h>
#include <revolution/mem/mem_frameHeap.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_picture.h>
#include <nw4r/lyt/lyt_texMap.h>
#include <nw4r/lyt/lyt_textBox.h>

class UnkClass_8045F564;

// Hash-table element referenced by CLibLayout's creator ebook. field_0 is an
// allocation handle for the accelerator allocator; field_4 (when non-NULL) is a
// frame heap that takes priority for the buffer.
struct CLibLayoutHashElem {
    mtl::ALLOC_HANDLE field_0; // 0x0: allocation handle
    MEMiHeapHead* field_4;     // 0x4: optional frame heap
};

// __declspec(novtable): the retail TU emits no implicit vtable/RTTI/vptr
// stores; the vtable (lbl_eu_8056D350) is defined explicitly at the bottom of
// CLibLayout.cpp and installed by hand in the ctor (CLibG3d pattern).
class __declspec(novtable) CLibLayout : public CWorkThread {
public:
    CLibLayout(const char* pName, CWorkThread* pParent);
    virtual ~CLibLayout();

    DECL_WORKTHREAD_CREATE(CLibLayout);

    bool isInitialized();
    CLibLayout* getInstance();
    nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor();
    nw4r::lyt::Layout* createLayout();
    nw4r::lyt::TextBox* createTextbox();
    nw4r::lyt::Picture* createPicture();
    mtl::ALLOC_HANDLE getAllocHandle();
    void deleteTextboxOrPicture();

    virtual void wkUpdate() override;  //0x88
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    //0x0: vtable
    //0x0-0x1C4: CWorkThread
    MEMAllocator mAllocator;       // 0x1C4: nw4r layout allocator (0x10)
    MEMAllocatorFuncs mAllocFuncs; // 0x1D4: custom alloc/free fns for mAllocator
    mtl::ALLOC_HANDLE mAllocHandle;   // 0x1DC: handle for the nw4r layout allocation region
    u8 mHashData[0x40];               // 0x1E0-0x21F: inline hash-table slot storage (16 slots)
    CLibLayoutHashElem** hashTable;    // 0x220: -> &mHashData[0]
    s32 hashAccum;                     // 0x224
    s32 hashCount;                     // 0x228
    s32 hashDivisor;                   // 0x22C
    u32 mRangeStart;                   // 0x230
    u32 mRangeEnd;                     // 0x234
    UnkClass_8045F564* instanceArray[32];     // 0x238-0x2B7: tracked layout-instance pointers
    u32 instanceCount;                 // 0x2B8
    u8 pad_2BC[0x4];                   // 0x2BC-0x2BF
};

// Default layout-region name string and the active layout-allocator pointer.
// Global-scope variables are not C++-mangled, so plain extern decls emit the
// exact retail lbl_eu_* symbols. lbl_eu_805231BC is DEFINED in CLibLayout.cpp
// (retail owns the storage there); no decl here -- a duplicate extern in this
// TU trips MWCC 10322 (illegal name overloading) at the definition site.
extern MEMAllocator* lbl_eu_80665478;

// func_8045F438/func_8045F4E4 are the CLibLayout allocator callbacks. Their
// retail symbols use synthetic "Fv" names even though the callbacks genuinely
// take (MEMAllocator*, size) args, so they are declared extern "C" to emit the
// exact retail linker symbols instead of C++-mangled member names.
extern "C" void* func_8045F438__10CLibLayoutFv(MEMAllocator* allocator, u32 size);
extern "C" void func_8045F4E4__10CLibLayoutFv(MEMAllocator* allocator, void* block);

// Import: nw4r::lyt::TextBox(unsigned short) ctor (retail member symbol
// __ct__Q34nw4r3lyt7TextBoxFUs, omitted from lyt_textBox.h). C linkage emits
// the exact retail linker symbol; used by createTextbox().
extern "C" void __ct__Q34nw4r3lyt7TextBoxFUs(nw4r::lyt::TextBox* pTextBox, u16 len);

// Import: nw4r::lyt::Picture(const TexMap&) ctor. Declared with C linkage so
// createPicture() can construct on a raw buffer without placement-new's NULL
// guard (retail calls the ctor directly).
extern "C" void __ct__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt6TexMap(
    nw4r::lyt::Picture* pPicture, const nw4r::lyt::TexMap& rTexMap);

// Runtime throw helper (rethrow). Declared noreturn so MWCC elides the
// __end__catch epilogue of a catch-all handler that ends in `bl __throw`
// (retail catch-all handlers end with the __throw call; see CScnItemAnim.hpp).
extern "C" __declspec(noreturn) void __throw(char* throwtype, void* location,
                                             void* dtor);
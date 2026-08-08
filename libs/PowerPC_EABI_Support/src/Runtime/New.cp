#include <types.h>

// Retail New.o is data-only (.text = 0): it carries ONLY std::exception's
// typeinfo - the name string (".rodata", 0xF = "std::exception\0") and the
// 8-byte {name ptr, NULL base} typeinfo struct (".sdata", 0x8, with a reloc
// from +0x0 to the .rodata string). There is no operator new/delete in this
// retail TU, and no throw code. The bad_alloc / exception RTTI that a dummy
// throw used to generate here lives in the retail data objects; emitting it
// locally blows the .rodata/.sdata/.text split budgets (old build: .text 0x8C,
// .rodata 0x42, .data 0x26, .sdata 0x10, extab + extabindex).

namespace {

// Retail .rodata: the RTTI name string, exactly 0xF bytes.
const char exceptionName[] = "std::exception";

// Retail .sdata: type_info_struct { const char* typeName; void* baseList; }.
struct ExceptionTypeInfo {
    const char* name; // +0x0: reloc -> exceptionName (.rodata)
    void* base;       // +0x4: NULL
} exceptionTi = { exceptionName, NULL };

} // namespace

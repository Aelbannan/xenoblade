#include <nw4r/ut.h>

// Retail ships DvdFileStream::typeInfo from the monolibdata1 data unit
// (lbl_eu_80665550) and keeps only the static initializer in this TU:
//   li r0, lbl_eu_80665548@sda21
//   stw r0, lbl_eu_80665550@sda21(r13)
// (see postprocess_reloc_names.py "ut_DvdFileStream.o" — the stw reloc is
// retargeted to the external retail label and the local .sbss is stripped).
nw4r::ut::detail::RuntimeTypeInfo nw4r::ut::DvdFileStream::typeInfo(&lbl_eu_80665548);

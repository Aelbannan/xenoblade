// Translation unit: monolib/src/coli/CColiProcBase
//
// CColiProc's constructor, kept in its own unit because retail places it
// between code_804A6C60 and CColiProc in link order (.text 0x804B66D4-0x804B66EC,
// right after code_804A6C60's static initialiser) and BOTH of those units'
// static initialisers call it out of line (a6c60 for the CColiObjCall singleton,
// CColiProc for the CColiResCall singleton). MWCC inlines a same-TU
// base-class ctor into a generated __sinit, so the ctor cannot live in either
// of them and still reproduce retail's 0x54 / 0x3C initialisers.

#include <types.h>
#include "monolib/coli/CColiProc.hpp"

CColiProc::CColiProc() {
    field_0x4 = 0;
}

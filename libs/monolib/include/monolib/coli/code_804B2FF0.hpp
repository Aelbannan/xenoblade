#pragma once

#include <types.h>
#include <revolution/MTX.h>

// Partial layouts / forward decls for cross-TU data declared below (retail
// C-linkage pseudo-imports relocated from code_804B2FF0.cpp). Keep
// signatures/linkage identical - the names are the retail symbol names.
struct CColiBounds;
struct CColiHx;
struct CColiQx;
struct CColiWalkState;
struct CColiQueryNode;
struct CColiMoveNode;
struct CColiProcLocal;

// C-linkage imports
extern "C" CColiBounds* lbl_eu_80665944;
extern "C" CColiHx* lbl_eu_8066594C;
extern "C" CColiQx* lbl_eu_80665948;
extern "C" s16 lbl_eu_80665950; // master node index (lha)
extern "C" f32 lbl_eu_8066AEB0; // 0.0f
extern "C" f32 lbl_eu_8066AEB4; // 1.0f
extern "C" void func_804B1DC0(void*, int);
extern "C" CColiWalkState lbl_eu_8065D138;
extern "C" u8 lbl_eu_8056F4F0;
extern "C" void __ct__CColiProc(CColiProcLocal* self);
extern "C" f32 lbl_eu_8066AEC0;
extern "C" f32 lbl_eu_8066AEC4;
extern "C" f32 lbl_eu_8066AEC8;
extern "C" void func_804A7ACC(void*, const Vec*, const Vec*, void*);
extern "C" void func_804B077C(void*, const Vec*, const Vec*);
extern "C" int func_804B0818(const void*, const CColiQueryNode*);
extern "C" int func_804B21A8(const CColiQueryNode*, const void*, int);
extern "C" void func_804B0CE8(CColiMoveNode* node);
extern "C" void func_804B0DF4(CColiMoveNode* node);
extern "C" int lbl_eu_80665958;

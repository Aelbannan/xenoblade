#pragma once

// Unit-private import declarations split out of CKizunaTalkList.hpp so the class header
// stays includable next to other headers.
#include "kyoshin/CKizunaTalkList.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"


// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - do not "fix" the mangled forms).
// Relocated from CKizunaTalkList.cpp so every TU that uses these symbols gets
// them from this header instead of inline pseudo-import declarations.
// ---------------------------------------------------------------------------

// External helpers (C-linkage retail symbols) for file loading, scrollbar /
// cursor widgets and sound effects.
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" int KyoshinHeap_GetField44();
extern "C" void Cur_DrawLayout(void*, void*);
extern "C" void Cur_Update(void*);
extern "C" void Cur_SetVisible(void*, u8);
void closeFileHandle(CFileHandle**);
void releaseArcResourceAccessor(nw4r::lyt::ArcResourceAccessor*);
extern "C" void deleteRegion(UnkClass_8045F564* self);

// Layout-build helpers used by CKizunaTalkList::OnFileEvent (mangled retail
// symbol names so the reloc targets line up).
extern "C" void setMemInitFlag__Q23mtl10MemManagerFb(bool value);
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor* self, void* data, const char* name);
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();
extern "C" void* Bdat_GetTable_AA34();
extern "C" int sprintf(char* str, const char* fmt, ...);

// Data/build helpers used by buildTalkList.
extern "C" u32 Bdat_GetMaxRow_B1EC(void* fp);
extern "C" u32 CtrlRemote_TouchBitByArg(u32);
extern "C" u16 LookupRelateValue(u8 a, u8 b);
extern "C" u8* lbl_eu_806640A8;                            // .sbss BDAT table pointer

// data / rodata labels
extern "C" char lbl_eu_8050E990[];            // file-name table (target 7)
extern "C" void* lbl_eu_80664090;            // .sbss shared BDAT character table (target 7)
extern "C" void* lbl_eu_806648B8;             // .sbss loaded file pointer (target 8); assigned from void* getFP -> kept void*
extern "C" void* lbl_eu_806648C0;             // .sbss colour entries (target 8) - address anchor
// lbl_eu_806648C0/C8/D0/D8 are address anchors (&lbl used directly): keep void*.
extern "C" void* lbl_eu_806648C8;
extern "C" void* lbl_eu_806648D0;
extern "C" void* lbl_eu_806648D8;
extern "C" f32 lbl_eu_806689C8;               // .sdata2 float constants (target 10)
extern "C" f32 lbl_eu_806689CC;
extern "C" f32 lbl_eu_806689D0;
extern "C" const f32 lbl_eu_806689D4;         // 2.0f - anim frame step for entry show/hide

// Misc helpers used by the row-draw / visibility paths.
extern "C" void setPaneVisible(void*, u32);

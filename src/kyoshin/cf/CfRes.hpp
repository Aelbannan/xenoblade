#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "kyoshin/cf/IResInfo.hpp"

struct CfResBuffer {
    u8 buffer[0x400]; // 0x400-byte ring (indexed as u32 words)
    u32 field_400;    // ring read index (byte 0-255)
    u32 field_404;    // remaining word count

    int pushBackwards(unsigned char byte1, unsigned short halfword, unsigned int dataVal, unsigned int* src, int count, unsigned int headerBits);
    int popRecord(u32* headerOut, u32* dataOut);
};

// Ring-record appender; the scratch header word is an intentionally
// uninitialized local (retail reads the raw frame slot).
int CfRes_pushRec(CfResBuffer* buffer, unsigned char byte1, unsigned short halfword,
                  unsigned int dataVal, unsigned int* src, int count);

// Entry-table helpers shared across CfRes (defined in CfRes.cpp).
extern "C" void CfRes_clearField4(u8* self);
extern "C" void CfResEntry_clearField8(u8* self);
extern "C" int CfRes_incField8(u8* self);
extern "C" bool initParticleSystem__Q22cf13CfGameManagerFv(u32 value);
extern "C" void cfResNopValueSink(u32 value);

// Acquire a resource-table entry for a resource id, bumping refcounts.
// extern "C" keeps the call reloc carrying the retail unmangled name.
extern "C" int CfRes_acquireSlot(int id, int incRef, int incCount);

// Helpers used by the CfRes_dispatchLoad load dispatcher (defined across cf TUs;
// C linkage keeps the call relocs carrying the retail unmangled names).
extern "C" bool isSceneLoading__Q22cf13CfGameManagerFv(void* self);
extern "C" void CfRes_clearE28Mask(u32 mask);
extern "C" void CfRes_setE28Mask(u32 mask);
extern "C" int CfRes_getE24Bit18();
extern "C" int CfRes_getE14();
extern "C" int CfRes_getE30();
extern "C" int CfRes_isPackedMarker(u32 packed);
extern "C" int CfRes_tryResolveByBits(s32 destination, u32 source, u32 size);
extern "C" int CfRes_tryResolveToken(int a, int b);
extern "C" int CfRes_getPackedFileSize(u8* res);
extern "C" char* CfRes_getInstPtrBC();
extern "C" u32 CfRes_isField4Zero(u8* res);
extern "C" void CfRes_initStruct_64994(u8* res);
extern "C" bool CfRes_findHighEntryById(int instField, u32 packed, u32* out10, u32* outC);
extern "C" void* KyoshinHeap_AllocNode(int size, int checkOnly);
extern "C" u8* KyoshinHeap_Alloc38(u32 size);
extern "C" void CfRes_orBits_649B4(u8* self, u32 bits);
extern "C" void CfRes_orBits_649CC(u8* self, u32 bits);
extern "C" u8* CfRes_findHighEntry(int instField, u32 packed, u32* out10, u32* outC);
extern "C" u8* CfRes_findLowEntryOrMark(int instField, u32 packed, u32* out10, u32* outC);
extern "C" bool findResEntry(int instField, u32 packed, u32* out10, u32* outC);
extern "C" u32 getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" u32 getMaxAllocSize__Q23mtl10MemManagerFUl(u32 handle);
extern "C" void* allocate_tail__Q23mtl10MemManagerFUlUli(u32 handle, u32 size, int align);
extern "C" void* allocate_head__Q23mtl10MemManagerFUlUli(u32 handle, u32 size, int align);
extern "C" void CfRes_attachEntryData(u8* res, void* buf, u32 kind);
extern "C" int KyoshinHeap_Contains38(void* buf);
extern "C" void KyoshinHeap_Register38(void* buf, int mode, u32 packed);
extern "C" int KyoshinHeap_Contains78(void* buf);
extern "C" void KyoshinHeap_Register78(void* buf, int mode, u32 packed);
extern "C" int CfRes_tryResolveType0(int a, int b, int c);
extern "C" int CfRes_getLookupFlags(void* self);
extern "C" int CfRes_checkMask_64A08(u8* res, u32 mask);

// Resource-load dispatcher: resolves a packed resource token to a resident
// buffer, allocating through the archive cache or the memory-manager
// fallback chain.
int CfRes_dispatchLoad(void* self, u32 packed, int flag, int kind);

struct CfRes_64994 {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u8 _0C[0x14];
    u32 field_20;
    u32 field_24;
    u32 field_28;

    void initStruct();
};

struct CfRes_65818 {
    u32 field_00;
    int field_04;

    int decRefCount();
};

// 0xC-byte table-entry record (handle at +0, refcount at +4) used by the
// manager's two entry tables and the CfResEntry_* helpers.
struct CfResSlot {
    u32 field_00;   // 0x00 - handle
    u32 field_04;   // 0x04 - refcount
    u32 field_08;   // 0x08
};

// 0x10-byte name-keyed table record walked by CfRes_lookupNameByType: the +0x00 word
// is a relative offset from the table base (the result pointer), +0x04 is
// the value written to *out, and +0x08 is the 8-byte name string hashed by
// Tok_ParseFile (bits 27-31 of the hash select the record).
struct CfResNameEntry {
    u32 field_00;   // 0x00 - relative offset
    u32 field_04;   // 0x04 - value
    char name[8];   // 0x08 - name string
};

// Table container read by CfRes_lookupNameByType: entry count at +0x08, the entry
// array at +0x10.
struct CfResNameTable {
    u8 _00[0x8];
    u32 field_08;                  // 0x08 - entry count
    u8 _0C[4];                     // 0x0C
    CfResNameEntry entries[1];     // 0x10 - entry array (count varies)
};

// 0x1FA4-byte CfRes manager object built by the __ct__Q22cf5CfResFv factory
// ctor. The CProcess/CTTask base (0x00-0x53) is constructed by
// __ct__8CProcessFv, then the +0x10 vtable and the +0x3C/+0x48 PTMF slots
// are set by hand; a storage container lives at +0x54, followed by two
// 0xC-stride entry tables at +0x1F2C/+0x1F50.
struct CfResManager {
    u8  _00[0x10];          // 0x00 - CProcess base head
    u32 vtable;             // 0x10 - CTTask<CfResTask> then CfRes vtable
    u8  _14[0x28];          // 0x14 - 0x3B
    u32 mMoveFunc[3];       // 0x3C - ptmf (__ptmf_null)
    u32 mDrawFunc[3];       // 0x48 - ptmf (__ptmf_null)
    u32 storageVtable;      // 0x54 - storage subobject vtable (lbl_eu_80526830)
    u8  storage[0x1E78];    // 0x58 - storage container (ctor __ct__80066F9C)
    u8  _1ED0[0x5C];        // 0x1ED0 - gap
    u32 tbl1Header[3];      // 0x1F2C
    CfResSlot tbl1[2];      // 0x1F38
    u32 tbl2Header[3];      // 0x1F50
    CfResSlot tbl2[6];      // 0x1F5C (object ends at 0x1FA4)
};

// CfRes manager vtables and the null pointer-to-member constant (the retail
// ctor stores the interim CTTask vtable at +0x10, then overwrites it with
// the CfRes vtable; the +0x54 storage subobject gets its own vtable).
extern u32 lbl_eu_805267EC[];   // interim CTTask<CfResTask> vtable
extern u32 lbl_eu_805267A4[];   // CfRes vtable
extern u32 lbl_eu_80526830[];   // +0x54 storage subobject vtable
extern u32 __ptmf_null[3];

// Factory-ctor imports (retail pre-mangled names: these are out-of-line
// member ctors / allocators from other TUs). extern "C" keeps the call
// relocs carrying the retail unmangled names (plain global declarations get
// an `__F...` arg suffix appended by the MWCC mangler).
extern "C" u32 getWorkMem__17CWorkThreadSystemFv();
extern "C" void __ct__8CProcessFv(CProcess* p);
extern "C" void __ct__80066F9C(u8* self);

namespace cf {

class CfRes {
public:
    CfRes();
    ~CfRes();

    /* 0x00 */ u32 field_00;            // manager header word
    /* 0x04 */ ResInfoStorage mStorage; // embedded resource storage (torn down by ~CfRes via __dt__8006754C)
};

// CRTP task base (CTTask<CfResTask>) occupies 0x00-0x54; the embedded resource
// storage lives past the base. Term() tears it down via CfRes_dtorStoragePlus4 and
// CfRes_resetAllEntries (the +0x58 region doubles as a ResInfoEntry array and a
// ResInfoStorage container). novtable: the retail ~CfResTask stores no vptr
// (same pattern as IUIWindow / CUIErrMesWin).
class __declspec(novtable) CfResTask : public CTTask<CfResTask> {
public:
    virtual ~CfResTask();
    void Term();

    virtual void Init();
    virtual void Move();
    virtual void Draw();

    /* 0x54 */ u8 mField54[4];              // header of the embedded storage object
    /* 0x58 */ ResInfoEntry mEntries[0x82]; // entry array (also a ResInfoStorage)
};
} // namespace cf

// The retail OnFileEvent symbol is a global-namespace CfRes member (MWCC
// emits the IWorkEvent override of the cf-namespace manager class with the
// base interface's global namespace context - same quirk as CTaskREvent).
// Declared here at global scope so the compiled mangled name matches retail.
class CEventFile;
class CfRes {
public:
    void OnFileEvent(CEventFile* ev);
};

// String-keyed record: 4-byte key string at +0 (compared via strcmp against
// gResKeyDap1, e.g. "DAP1") with a count word at +4.
struct CfResNameRec {
    char mKey[4];   // 0x00 - inline key string
    int mCount;     // 0x04
};

// 16-byte records of the name-keyed table walked by CfRes_lookupStrTable: the +0x10
// word is a relative offset from the table base, the +0x18 string resolves
// through Tok_GetPrefix.
struct CfResStrTableRec {
    u8 _00[0x10];
    u32 field_10;   // 0x10 - relative offset
    u8 _14[4];      // 0x14 - 0x17
    char str[4];    // 0x18 - name string
};

// 0x18-stride record of the entry table walked by CfRes_extType: field_08 is
// a path offset relative to the table base, +0xC/+0x10 are output words.
struct CfResExtRec {
    u8 _00[8];
    u32 field_08;
    u32 field_0C;
    u32 field_10;
    u32 field_14;
};

// Name-key string pointer (in .sdata; points at "DAP1" etc.).
extern const char* gResKeyDap1;

// Name-key string pointer (in .sdata; table key compared by CfRes_lookupStrTable).
extern const char* gResKeyKyp;

// Extension separator character (in .sdata, ".\0"): the key CfRes_stripExtension
// strips from the end of a path. Declared as a scalar (not an array) so MWCC
// emits the retail @sda21 addressing; use &lbl_eu_80661A40 as the string.
extern char lbl_eu_80661A40[8];   // '.' separator + pad (retail .sdata 8 bytes)

// Reset the ResInfoEntry array (defined in kyoshin/cf/IResInfo.cpp).
// C linkage so the retail unmangled symbol name is emitted (struct params
// would otherwise mangle it).
extern "C" void CfRes_resetAllEntries(ResInfoEntry* entry);

// Global CfRes manager state word (zeroed by CfResTask::Term).
extern u32 gCfResManager;

// Effect-singleton manager object (array form forces @ha/@l address refs;
// also declared in kyoshin/CTaskGameEff.hpp).
extern u32 lbl_eu_8065FC18[];

// Shared string table in .rodata (path-format strings; CfRes_resolveSlot1E4
// formats from +0x23).
extern char lbl_eu_804FB214[];

// C-linkage runtime import (retail symbol name - keep linkage/signatures verbatim)
extern "C" long __ptmf_test(void* ptmf);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" int CfRes_reloadLoadedEntries(int);
extern "C" int CfRes_updateFlaggedGrid(int);
extern "C" int CfSoundMan_CloseRecord(int);
extern "C" int KyoshinHeap_HasActive54();
extern "C" int KyoshinHeap_GetField54();
extern "C" void CfRes_initWork(u8* self);
// Same-TU archive-registration helper (defined in CfRes.cpp; C linkage so
// the call reloc from __ct__Q22cf5CfResFv carries the retail name).
extern "C" void CfRes_initArchiveTables(u8* self, int arg);
// Packed-token packer (kyoshin/code_800AA008.cpp) - retail C symbol name.
extern "C" u32 Tok_Pack08(u32 a, u32 b);
// Device-file allocation helper (retail C symbol name).
extern "C" int KyoshinHeap_GetActive54();
// Same-unit helpers under their retail unmangled names (CfRes.cpp stubs -
// linkage must stay C so call relocs carry the retail names).
extern "C" int CfResEntry_decRefCount(u8* entry);
extern "C" int CfResEntry_incRefCount(u8* entry);
extern "C" int CfRes_getResFileSize(u8* res);
extern "C" int CfRes_onFileEventDone(CfRes* self, CEventFile* evt, u32 field);
extern "C" void CfRes_reregisterTableEntry(int inst, int index, u8* ptr);
extern "C" u32 CfRes_packFourFields(u32 a, u32 b, u32 c, u32 d);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
extern "C" char* getEntryPtr(char* base, int a, int b);
extern "C" void CfRes_releaseCachedBase(ResInfoEntry* entry, bool cleanup);
extern "C" void Tok_Unpack(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);

// CfRes_dispatchArchiveRead (0x800645FC): async archive-read pipeline (path build + open
// + per-entry init). 6 args, int result; C linkage so the call reloc from
// CfRes_buildPathAndRead carries the retail unmangled name.
extern "C" int CfRes_dispatchArchiveRead(void* a, void* b, u32 c, void* d, void* e, int f);

// CfRes_buildPathAndRead (0x800646E8): path build + archive-read dispatch for a packed
// resource id (5 args, int result).
extern "C" int CfRes_buildPathAndRead(u8* a, u8* b, u32 c, u8* d, int e);

// CfRes_setTblHandle (0x8006415C): table-entry handle registration/refcount bump.
extern "C" void CfRes_setTblHandle(int a, int b);

// CfRes_rolloverResFields (0x80063C64): per-entry field_04/field_08 leftover rollover.
extern "C" void CfRes_rolloverResFields();

// C++ virtual thunk dispatching the +0x2C sub-object's m02 (retail returns
// the vtable call result in r3).

// Packed-field query on the CfGameManager singleton (defined in
// CfGameManagerUnityHelpers.hpp; the retail name is the mangled member
// symbol, kept verbatim via C linkage).
extern "C" void getEventHalfwordPair__Q22cf13CfGameManagerFv(u16* first, u16* second);

// Main per-entry update step (defined in kyoshin/cf/IResInfo.cpp); C linkage
// so the call reloc carries the retail unmangled name.
extern "C" bool CfRes_updateEntry(void* self, bool r4, bool r5, bool r6);

// Same-TU helpers (defined in CfRes.cpp; forward-declared so earlier callers
// in the file can reference them). CfRes_resolveSlot130 is C linkage so the call
// reloc from CfRes_tryResolveSlot130 carries the retail unmangled name.
extern "C" int CfRes_resolveSlot130(int inst, int a, int b, int c, int d);
int CfRes_resolveSlot16C(int inst, int a, int b);

// CfRes_tryLoadFromCache (0x8006429C): pre-open entry reservation/validation step in
// the archive-read pipeline (stub in CfRes.cpp; C linkage so the call reloc
// from CfRes_dispatchArchiveRead carries the retail unmangled name).
extern "C" int CfRes_tryLoadFromCache(void* a, void* b, u32 c, void* d, int size, void* e);

// Same-TU helpers (defined in CfRes.cpp). C linkage so call relocs carry the
// retail unmangled names (same rule as the CfRes_getResFileSize stub above).
extern "C" u32 CfRes_extractBits20_7(void* self);
extern "C" u32 CfRes_extractBits10_10(void* self);
extern "C" int CfRes_tryResolvePackedAB(u16 a, u16 b, int c);
extern "C" int CfRes_lookupNameByType(int a, int b, int* out);

// Same-TU entry-table helpers (defined in CfRes.cpp). C linkage so call
// relocs carry the retail unmangled names.
extern "C" void CfResEntry_init(u8* self);extern "C" void* CfRes_getResEntry(u8* self, u32 idx);
extern "C" void* CfRes_getTblEntry(u8* self, u32 idx);
extern "C" u32 CfResEntry_getHandle(u8* self);
extern "C" u32 CfResEntry_getField4(u8* self);
extern "C" void CfResEntry_setHandle(u8* self, u32 val);

// +0x28 load-request flag of the CfResPcTableEntry record (CfRes_getPcGridEntry's
// result; only this word is read by CfRes_isGridLoadIdle - the full record layout
// is declared in CfResPcImpl.hpp).
struct CfResPcEntry28View {
    u8 _00[0x28];
    u32 field_28;
};

// vtable +0x38 dispatch over the resource-entry lookup object (CResLookup::vfunc0A at +0x38).

// Same-unit resource-table accessor (stub defined in CfRes.cpp; C linkage so
// call relocs carry the retail unmangled name).
extern "C" CfResPcEntry28View* CfRes_getPcGridEntry(int id, int a);

// Same-TU bit-field helpers (defined in CfRes.cpp). C linkage so call relocs
// carry the retail unmangled names; the unit's -inline noauto keeps them
// out-of-line calls (retail calls them).
extern "C" u32 CfRes_extractBits27_5(void* self);
extern "C" u32 CfRes_getAddrLow10(void* self);

// Same-TU packed-token repacker and vtable dispatch thunks used by
// CfRes_resolveTokenEntry (defined in CfRes.cpp; C linkage so the call relocs carry
// the retail unmangled names). int-returning so callers capture r3.
extern "C" unsigned long CfRes_packShift27(unsigned long a, unsigned long b);

// Packed-name hash (kyoshin/code_800AA008.cpp - retail C symbol name; the
// plain C++ declaration in code_800AA008.hpp would mangle the call reloc).
extern "C" u32 Tok_ParseFile(const char* path);

// Same-TU resource resolvers called by the func_80062xxx wrappers. C linkage
// so the call relocs carry the retail unmangled names (same rule as the
// CfRes_getResFileSize / CfRes_onFileEventDone stubs above).
extern "C" int CfRes_resolveSlot220(int inst, int a, int b);
extern "C" int CfRes_resolveSlotByBits(int inst, int a, int b);
extern "C" int CfRes_resolveSlot298(int inst, int a, int b);
extern "C" int CfRes_resolveTokenEntry(int inst, int a, int b);

// Same-TU resource resolvers called by the func_80062xxx wrappers. C linkage
// so the call relocs carry the retail unmangled names (same rule as the
// CfRes_getResFileSize / CfRes_onFileEventDone stubs above).
extern "C" int CfRes_resolveAndLinkB8(int inst, int a, int b, int c);
extern "C" int CfRes_resolveSlotB8(int inst, int a, int b, int c);
extern "C" int CfRes_resolveSlotF4(int inst, int a, int b, int c);
extern "C" int CfRes_resolveSlot1E4(int inst, int a, int b, int c);

// +0x1E4 resource-slot getter (defined in CfRes.cpp; forward-declared so the
// earlier CfRes_resolveSlot1E4 caller can reference it).
extern "C" void* CfRes_ptrPlus1E4(void* self);

// Effect-singleton resource registration (C-ABI import, retail unmangled
// symbol name; same extern "C" convention as CREvtEffect.hpp).
extern "C" void EffSched_LookupA(void* mgr, void* data);

// Delegate-op dispatcher (defined in CfRes.cpp). Declared int-returning so
// callers can capture the r3 result (retail callers do `mr r3r31, r3` after
// the call); the body is a void call to CfRes_dispatchLoad followed by blr.
extern "C" int CfRes_delegateOp1(void* a, void* b, void* c);
extern "C" int CfRes_delegateOp0(void* a, void* b, void* c);

// Same-unit resource resolvers (CfRes.cpp stubs - retail unmangled names so
// call relocs carry the retail symbol names).
extern "C" int CfRes_resolveTokenByBits(int inst, int a, int b, int c);
extern "C" int CfRes_resolveTokenType0(int inst, int a, int b, int c);
extern "C" int CfRes_resolveTableToken(int inst, int a, int b, int d, int e);
extern "C" int CfRes_registerTblHandle(int handle);
extern "C" int CfRes_stripExtension(ml::FixStr<64>& dest, const char* src);
extern "C" int CfRes_resolveGridToken(int inst, int a, int b, int d, int e);
extern "C" int CfRes_getDeviceFileSize(void* self);
extern "C" ml::FixStr<64>* CfRes_stub_63ACC(ml::FixStr<64>* str);

// Work-cache list helpers (defined in monolib/src/work/CWorkSystemCache.cpp
// with C linkage; retail unmangled names). SysCacheCountListNodes counts the cached
// entries; SysCacheFetchIndexedEntry looks up the index-th entry and writes out its item
// fields (0x3C value, +0x4 buffer pointer, 0x40 value).
extern "C" u32 SysCacheCountListNodes(void);
extern "C" bool SysCacheFetchIndexedEntry(int index, u32* outField3C, u8** outField4Addr, u32* outField40);

// Same-TU string compare helper (defined in CfRes.cpp; C linkage so call
// relocs carry the retail unmangled name).
extern "C" int CfRes_streq(const char* s1, const char* s2);

// Same-TU entry helpers (defined in CfRes.cpp; C linkage so call relocs
// carry the retail unmangled names).
extern "C" void CfRes_delegateCleanup(void* self);
extern "C" void CfRes_initFields4(u8* self, int a, int b, int c, int d);
extern "C" void CfRes_setBits1_2(u8* self);
extern "C" void CfRes_resetState2(u8* self);

// Same-TU string helper (defined in CfRes.cpp; forward-declared so the later
// CfRes_resolveSlot130 caller can reference it). C linkage so the call reloc carries
// the retail unmangled name.
extern "C" char* CfRes_strcatAppend(char* buffer, const char* suffix);

// Packed-token formatter (kyoshin/code_800AA008.cpp) - retail C symbol name.
extern "C" u32 Tok_GetPrefix(const char* str);

struct CFileHandle; // full definition in monolib/device/CFileHandle.hpp

// Cancels a pending async file read (defined in the CDeviceFile TU).
void cancel__11CDeviceFileFP11CFileHandle(CFileHandle* handle);

// Destroying destructor of the embedded resource storage (defined with C
// linkage in kyoshin/cf/IResInfo.cpp). C linkage here so the call relocs
// carry the retail unmangled symbol name.
extern "C" void* __dt__8006754C(u8* self, int mode);

// Resource-table entry cleaned up by CfRes_cancelPendingRead (sibling of
// cf::CfResPcLookupEntry in CfResPcImpl.hpp, which declares only the fields
// other units touch; this mirrors the full cleanup access pattern).
struct CfResCleanupEntry {
    u32 field_00;           // 0x00
    u32 field_04;           // 0x04
    u32 field_08;           // 0x08
    u8  _0C[0x8];           // 0x0C - 0x13
    u32 field_14;           // 0x14
    u8  _18[0x8];           // 0x18 - 0x1F
    u32 field_20;           // 0x20
    u32 field_24;           // 0x24
    CFileHandle* field_28;  // 0x28 - pending device file handle (cancelled then cleared)
    u8  _2C[0x10];          // 0x2C - 0x3B (record stride 0x3C)
};

// Cleanup helper: cancels the parent's pending file handle and clears its
// state fields (defined in CfRes.cpp; the first arg is the +0x2C resource
// object, passed through but unused by the cleanup).
void CfRes_cancelPendingRead(CfResCleanupEntry* child, CfResCleanupEntry* parent);

// Layout of the object addressed by the singleton at gCfResInstance. The
// ctor stores the address of CfResManager's +0x54 storage-vtable subobject,
// so the two entry tables are fields of this object at +0x1ED8 (3 slots) and
// +0x1EFC (7 slots), ending exactly at the 0x1FA4-byte allocation.
struct CfResInstance {
    u8 _pad0[0x18 - 0x0];
    u8 mField18[4];
    // 0x18
    u8 _pad1C[0x7C - 0x1C];
    u8 mField7C;
    // 0x7C
    u8 _pad7D[0x80 - 0x7D];
    u8 mField80;
    // 0x80
    u8 _pad81[0xB8 - 0x81];
    u8 mFieldB8;
    // 0xB8
    u8 _padB9[0xBC - 0xB9];
    u8 mFieldBC;
    // 0xBC
    u8 _padBD[0xF4 - 0xBD];
    u8 mFieldF4;
    // 0xF4
    u8 _padF5[0xF8 - 0xF5];
    u8 mFieldF8;
    // 0xF8
    u8 _padF9[0x130 - 0xF9];
    u8 mField130;
    // 0x130
    u8 _pad131[0x134 - 0x131];
    u8 mField134;
    // 0x134
    u8 _pad135[0x16C - 0x135];
    u8 mField16C;
    // 0x16C
    u8 _pad16D[0x170 - 0x16D];
    u8 mField170;
    // 0x170
    u8 _pad171[0x1A8 - 0x171];
    u8 mField1A8[4];
    // 0x1A8
    u8 _pad1AC[0x1E4 - 0x1AC];
    u8 mField1E4[4];
    // 0x1E4
    u8 _pad1E8[0x220 - 0x1E8];
    u8 mField220;
    // 0x220
    u8 _pad221[0x224 - 0x221];
    u8 mField224;
    // 0x224
    u8 _pad225[0x25C - 0x225];
    u8 mField25C;
    // 0x25C
    u8 _pad25D[0x298 - 0x25D];
    u8 mField298;
    // 0x298
    u8 _pad299[0x29C - 0x299];
    u8 mField29C;
    // 0x29C
    u8 _pad29D[0x1ED8 - 0x29D];
    CfResSlot mTable3[3];  // 0x1ED8
    CfResSlot mTable7[7];  // 0x1EFC
};



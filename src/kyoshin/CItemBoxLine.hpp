#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "kyoshin/CNumSelect.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"
#include <monolib/util.hpp>
#include <nw4r/lyt.h>

#include <nw4r/math/math_types.h>

class CEventFile;
struct CIBLTabFormat;
struct CIBLTabFull;
class CItemBoxPage;

/* 12-byte item-box tab entry: halves/words/bytes as stored per slot. */
struct CIBLTabEntry {
    u16 f0;   // +0x0
    u32 f4;   // +0x4
    u8 f8;    // +0x8
    u8 f9;    // +0x9
};

/* Tab holder embedded in CItemBoxLine (+0x3A4): 12 entries, a halfword count
 * and two selector bytes (field93 written by ItemBoxLine_CompactTabList). */
struct CIBLTab {
    CIBLTabEntry entries[12]; // +0x00..0x8F
    u16 count;                // +0x90
    u8 field92;               // +0x92
    u8 field93;               // +0x93 (armed by ItemBoxLine_CompactTabList when field390 == 2)
};

/* 12-byte vector used by the copyVEC3 page entries. */
struct CIBLVec3 {
    float x;   // +0x0
    float y;   // +0x4
    float z;   // +0x8
};


// CBaseCur). Its vtable has no destructor slot: ~CBaseCur is non-virtual.
class CIBLTabCur : public CBaseCur {
public:
    CIBLTabCur(nw4r::lyt::ArcResourceAccessor* arcResAcc = nullptr);
    ~CIBLTabCur();
    virtual void initLayout();
};

// Item list widget (retail RTTI "CItemBoxLine", base list IWorkEvent; vtable
// slot +0x10 = OnFileEvent). Members are destroyed by the compiler-generated
// part of ~CItemBoxLine in reverse declaration order.
class CItemBoxLine : public IWorkEvent {
public:
    CItemBoxLine(u16 pageId, u8 mode);
    virtual ~CItemBoxLine();
    virtual bool OnFileEvent(CEventFile* evt);

    int GetSelectReady();
    void LeaveState3();
    void RefreshTabSelect();
    int IsReadyFlag();
    void ResetTabBytes();
    void LoadFiles();
    void UpdateStates();
    void DrawLayout(nw4r::lyt::DrawInfo* drawInfo);
    void UnloadFiles();
    int IsBusy();
    void OpenLine();
    void TabNext();
    void TabPrev();
    void CursorPageBack();
    void CursorPageDown();
    void CursorPageUp();
    void CursorPageDownRow();
    void ConfirmOverlayOrHint();
    void ResolveEntry();
    void RefreshTabPanes();
    int GetArmedFlag();
    void ResetCursorB8(bool show);
    void Info2SelectPrev();
    void Info2SelectNext();
    void ShowDetailWin();
    u8 ResolveFocusId();
    void EnterState0();
    void EnterState1();
    void EnterState2();
    void EnterState3();
    void PushSyswinCursor();
    void CommitOverlayState();
    void CompactTabList();
    void DrawTabRowText(u32 arg2, u32 arg3);
    void DrawTabRowIcon(u32 arg2, u32 arg3);
    void FormatName(u32 arg4, s8 arg5, u32 arg6);
    void SetPaneVisible(u32 index, bool visible);
    void RebuildTabList();
    void BindSlotTexture(u8 kind, u32 index);
    u32 RefreshLineState();
    void RefreshPageList();
    void RefreshTabLabels(unsigned int dir);
    void RefreshLineRows();
    void ResetPageTables(u32 itemData);
    void ShowSlotCounts(u32 itemData);
    void ShowSlotCounters(u32 itemData);
    void SetInfo2Item(u32 itemData);
    void ShowPageTabs(u32 itemData);
    void FormatGemName(u32 itemData);
    void FormatKindDName(u32 itemData);
    void BuildPageData(u32 itemData);
    void PushTabByte(unsigned char val);
    u16 TouchTabEntryItem();
    int ProbeAvailability(u32 key);

    UnkClass_8045F564 mRegion04;        // +0x04: layout scratch region
    UnkClass_8045F564 mRegion14;        // +0x14: second scratch region
    u32 field24;                                // +0x24: loaded file handle (item-box line arc)
    u32 field28;                                // +0x28: loaded file handle
    u32 field2C;                                // +0x2C: common-archive file handle
    u32 field30;                                // +0x30: loaded file handle
    u8* field34;                                // +0x34: owned scratch buffer (MemManager::deallocate)
    nw4r::lyt::ArcResourceAccessor* field38; // +0x38: common-archive resource accessor
    nw4r::lyt::ArcResourceAccessor* field3C; // +0x3C: item-line archive resource accessor
    nw4r::lyt::Layout* field40;         // +0x40: active layout
    nw4r::lyt::AnimTransform* field44;  // +0x44: animation transform 1
    nw4r::lyt::AnimTransform* field48;  // +0x48: animation transform 2
    u8 field4C;                         // +0x4C
    u8 pad_4D[0x50 - 0x4D];             // 0x4D..0x4F
    int field50;                        // +0x50: navigation state machine
    nw4r::ut::WideTagProcessor* field54; // +0x54: owned tag processor (ctored by __ct__CTagProcessor)
    u8 field58;                 // +0x58
    u8 unk59;               // +0x59: read by ItemBoxLine_IsReadyFlag
    u8 tabEntries[9];           // 0x5A..0x62: entry bytes, cleared by ItemBoxLine_ResetTabBytes
    u8 tabCount;                // +0x63: count
    u8 unk64[9];                // 0x64..0x6C: per-slot occupancy bytes (ItemBoxLine_TabNext/DB80 scan)
    u8 field6D;                 // +0x6D
    u8 pad_6E[0x70 - 0x6E];             // 0x6E..0x6F
    CCur18 mCur70;                      // +0x70
    CCur18 mCur88;                      // +0x88
    CIBLTabCur mCurA0;                      // +0xA0: embedded cursor (retail ctor __ct__CIBLTabCur; see CIBLTabCur note)
    CSubCur mCurB8;                     // +0xB8: embedded cursor (__ct__CSubCur, vtable[2] tab-name dispatcher)
    CItemBoxInfo2 mInfo2;               // +0xD0
    CNumSelect mNumSel;                 // +0x2DC
    CScrollBar mScrollBar;              // +0x310
    CSysWin mSysWin;                    // +0x350
    s16 unk38C;                 // +0x38C: read by ItemBoxLine_TouchTabEntryItem
    s16 unk38E;                 // +0x38E: read by ItemBoxLine_TouchTabEntryItem
    u8 field390;                // +0x390: read by ItemBoxLine_CompactTabList (== 2 arms unk3A4.field93)
    u8 pad_391;                 // +0x391
    s16 field392;               // +0x392: tab page counter (written by ItemBoxLine_CommitOverlayState)
    u16 field394;               // +0x394: page id read by ItemBoxLine_RefreshLineState
    GXColorS10 field396;        // +0x396..0x39D: nameplate colour (set by OnFileEvent)
    u8 unk39E;                  // +0x39E: read by ItemBoxLine_GetArmedFlag
    u8 field39F;                // +0x39F: current cursor position (ItemBoxLine_Info2SelectNext)
    u8 field3A0;                // +0x3A0: returned by ItemBoxLine_IsBusy
    u8 field3A1;                // +0x3A1: tab-cursor scan index (ItemBoxLine_CursorPageUp)
    u8 field3A2;                // +0x3A2: tab-cursor scan base (ItemBoxLine_CursorPageUp)
    u8 field3A3;                // +0x3A3: wrote by ItemBoxLine_ConfirmOverlayOrHint
    CIBLTab unk3A4;             // +0x3A4: tab entries (0x94 bytes, ends +0x437)
    ml::FixStr<32> str438;      // +0x438: tab name buffer (mLength at +0x458)
    ml::FixStr<128> str45C;     // +0x45C: second tab name buffer (mLength at +0x4DC)
    CItemBoxPage mPage;         // +0x4E0: per-page item table (ends +0x63C)
};

// Canonical declarations for the free tab helpers defined in CItemBoxLine.cpp.
u8 ItemBoxLine_GetTabEntryLock(const CIBLTab* self, unsigned int index);
u32 ItemBoxLine_GetTabEntryValue(const CIBLTab* self, unsigned int index);
u16 ItemBoxLine_GetTabEntryItem(const CIBLTab* self, unsigned int index);
u8 ItemBoxLine_GetTabEntryReady(const CIBLTab* self, unsigned int index);
ml::FixStr<32>* ItemBoxLine_FormatTabEntryName(CIBLTabFormat* self, unsigned int index);
void ItemBoxLine_AllocEntryStock(CIBLTabFull* self, u32 index, u32 count);
int ItemBoxLine_ProbeNameSlot(CIBLTabFull* self, u32 index);
u8 ItemBoxGetTab(const CItemBoxPage* page, unsigned int index);
void sinit_801F32EC();

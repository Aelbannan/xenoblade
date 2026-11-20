#pragma once

#include <types.h>
#include "monolib/work/CWorkThread.hpp"
#include "monolib/util/reslist.hpp"

class CPackItem;
class CArcItem;

// The work system owns the two resource lists; use the real template types so
// MWCC emits their vtables, RTTI locators and RTTI name strings.
typedef _reslist_node<CPackItem*> PackItemListNode;
typedef _reslist_base<CPackItem*> PackItemListBase;
typedef reslist<CPackItem*> PackItemList;

typedef _reslist_node<CArcItem*> ArcItemListNode;
typedef _reslist_base<CArcItem*> ArcItemListBase;
typedef reslist<CArcItem*> ArcItemList;

class __declspec(novtable) CWorkSystemPack : public CWorkThread {
public:
    CWorkSystemPack(const char* pName, CWorkThread* pParent);
    ~CWorkSystemPack();

    DECL_WORKTHREAD_CREATE(CWorkSystemPack);

    static bool isPackLoadIdle(UNKWORD r3);
    static bool findPackResource(const char* r3, void* r4, u32* r5);
    static bool arePacksLoaded();
    static bool areArcsReady();
    static void SavePkhFilenamesArrayPtr(const char* const[]);
    static void SaveStaticArcFilenameStringPtr(const char* const*);
    bool wkStandbyLogin();
    bool wkStandbyLogout();
    void wkUpdate();

    //0x0: vtable

    PackItemList mPackList; // 0x1C4
    ArcItemList mArcList;   // 0x1E4
    u32 unk204;             // 0x204 - name-guess: retail sizeof(CWorkSystemPack) == 0x208
};

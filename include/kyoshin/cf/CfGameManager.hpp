#pragma once

#include <types.h>

class CPad;
class CScn;
class CScnNw4r;
class CView;
struct VoiceSource;
class UnkClass_80186D20;
class CfCamEventManager;
class UnkClass_800821F8;
class UnkClass_80085334;
class UnkClass_8007E864;
class CSysWinBuff;
class Unk817A8Object;
class Unk80EE4Data;

namespace cf {
class CHelpManager;
// Party-slot list object returned by func_8009ECB0 (arr1[3] + arr2[6] @ +4);
// layout defined in src/kyoshin/cf/CtrlObjectParam.hpp - only pointers are
// passed around here.
struct CtrlObjectParamSlots;
}

struct CfGameManagerData1C {
    u8 field_0x0[0xC];
};

struct UnkClass_80083298SubF0 {
    u8 field_0x0;
};

class UnkClass_80083298 {
public:
    virtual void vfunc_0x08();
    virtual void vfunc_0x0C();
    virtual void vfunc_0x10();
    virtual void vfunc_0x14();
    virtual void vfunc_0x18();
    virtual void vfunc_0x1C();
    virtual void vfunc_0x20();
    virtual void vfunc_0x24();
    virtual void vfunc_0x28();
    virtual void vfunc_0x2C();
    virtual void vfunc_0x30();
    virtual void vfunc_0x34();
    virtual void vfunc_0x38();
    virtual void vfunc_0x3C();
    virtual void vfunc_0x40();
    virtual void vfunc_0x44();
    virtual void vfunc_0x48();
    virtual void vfunc_0x4C();
    virtual void vfunc_0x50();
    virtual void vfunc_0x54();
    virtual void vfunc_0x58();
    virtual void vfunc_0x5C();
    virtual void vfunc_0x60();
    virtual void vfunc_0x64();
    virtual void vfunc_0x68();
    virtual void vfunc_0x6C();
    virtual void vfunc_0x70();
    virtual u32 vfunc_0x74();

    u8 field_0x4[0xEC];
    UnkClass_80083298SubF0 field_0xF0;
};

// Heap-id slot block on the shared list-head object behind lbl_eu_80665958:
// func_8007C6C0 stores the current heap id at +0x7C.
struct Unk65958Object {
    u8 field_0x0[0x7C];
    u32 field_0x7C;
};

// Virtual container embedded at object+0x3E9C (func_80086B5C): the vptr sits
// at object+0x3E9C and slots 0x10 / 0x20 are dispatched with a u32 argument.
class UnkContainerIntf3E9C {
public:
    virtual void vfunc_0x08();
    virtual void vfunc_0x0C();
    virtual void vfunc_0x10(u32 value);
    virtual void vfunc_0x14();
    virtual void vfunc_0x18();
    virtual void vfunc_0x1C();
    virtual void vfunc_0x20(u32 value);
};

struct UnkObj3E9C {
    u8 field_0x0[0x3E9C];
    UnkContainerIntf3E9C container;   // 0x3E9C: embedded vptr'd subobject
};

// Data-area blocks at 0x80570CE0 (offsets +0x978 and +0xA18) cleared by
// func_800853C8 (byte/word stores at 0x00, 0x20, 0x24, 0x44, 0x50, 0x70,
// 0x74, 0x94).
struct Unk70CE0Entry {
    u8 field_0x0[0x20];
    u32 field_0x20;
    u8 field_0x24[0x20];
    u32 field_0x44;
    u8 field_0x48[0x8];
    u8 field_0x50;
    u8 field_0x51[0x1F];
    u32 field_0x70;
    u8 field_0x74;
    u8 field_0x75[0x1F];
    u32 field_0x94;
};

struct UnkArea70CE0 {
    u8 field_0x0[0x978];
    Unk70CE0Entry entryA;      // 0x978
    u8 gap[0x8];               // 0xA10-0xA17
    Unk70CE0Entry entryB;      // 0xA18
};

// Byte-offset view used by func_8007CF64: reads a text-area value/enabled
// pair at +0x48 / +0x4C from an entry base (or entry+0x24 secondary block).
struct UnkTextAreaView {
    u8 field_0x0[0x48];
    float value_0x48;
    u8 enabled_0x4C;
};

/* TODO: it's possible this file contains multiple separate classes, either just all being put in here,
or due to being in separate files, but compiled together in one file (unity compilation). For now,
to make things simpler, everything exists in a single class. */
namespace cf{
    class CfPadData;
    class CfObject;
    struct CfObjectSub54;
    class CfObjectMove;
    //unofficial name
    class CfGameManager{
    public:
        CfGameManager();

        static CfGameManager* getInstance();
        static CfGameManager* init(CScnNw4r* spSene, CView* pView, bool arg3);

        static void func_80086B5C(int arg1, int arg2, int arg3);
        static void enablePadFlags(u32 enableFlags, bool enable);
        static bool func_8007E1B4();
        static void func_8007E218();
        static void func_8007E514(int, int, char const*, int, int);
        static void func_8007F930(bool arg1);
        static UNKWORD func_800822F4();
        static bool func_800829B8();
        static u32 getCurrentPadChannel();
        static UnkClass_80083298* func_80083298();
        static CfObjectMove* getPlayer(int playerIndex);
        static u32 getEnabledInputFlags();
        static bool func_80086F9C();
        static bool func_80086F9C(s16) { return func_80086F9C(); }
        static void setCurrentPadPtr(const CPad* pad, u32 channel);
        static CPad* getPad(int channel);
        static void setPad(int r3, CPad* pPad, u32 r5);
        static CfPadData* getCfPadData();
        static CPad* getCurrentPad();

        static bool checkUnkFlag(int bit){
            return sUnkFlags & (1 << bit);
        }

        static void setUnkFlag(int bit, bool state){
            if(state == true) sUnkFlags |= (1 << bit);
            else sUnkFlags &= ~(1 << bit);
        }

        u32 unk0;
        CfObject* field_0x4;
        u32 mObjectFlags;
        u8 field_0xC[0xC];
        u16 field_0x18;
        u8 field_0x1A[2];
        CfGameManagerData1C field_0x1C;
        u8 unk28;
        u8 unk29[0x40 - 0x29];
        u16* field_0x40;
        s32 field_0x44;
        s32 field_0x48;
        s32 field_0x4C;
        u8 field_0x50[0x18];
        u32 unk68;
        u8 unk6C;
        u8 unk6D[3];          // 0x6D-0x6F
        u32 unk70;             // 0x70-0x73
        u8 unk74[8];           // 0x74-0x7B
        u32 unk7C;
        u8 unk80[0x86 - 0x80];
        u16 field_0x86;
        // Signed halfwords: func_800853C8 reads them with lha (reset-to -1
        // sentinel), func_80085E58 writes them through u16 casts.
        s16 field_0x88;
        s16 field_0x8A;
        s32 unk8C;
        UnkClass_80083298* unk90;
        //between CObjectParam - CfObjectMove
        //likely player character object array, seems to always store pointers
        //to CfObjectPc objects except pointing at the 4th vtable
        CfObjectMove* unk94[3];
        u32 unkA0;
        // opaque widget-list block torn down by func_80186664 (u8* ABI; the
        // func_80186D20 helper also takes void*), so keep the member as u8*.
        u8* field_0xA4;
        UnkClass_8007E864* unkA8;
        UnkClass_80085334* unkAC;
        UnkClass_800821F8* unkB0;
        CfCamEventManager* unkB4;
        u8 field_0xB8[0x4EC - 0xB8];
        u32 field_0x4EC;

        static u32 sUnkFlags;
        static CScnNw4r* spScene;
public:
    void func_8007C0F8();
    void func_8007C140();
    void func_8007C188(unsigned long flags);
    void func_8007C198();
    void func_8007C2F4();
    void func_8007C344();
    void func_8007C360();
    void func_8007C374();
    void* func_8007C4B4();
    ~CfGameManager();
    void func_8007C5B8();
    cf::CfObjectMove** func_8007C6B4(cf::CfObjectMove** slots, int index);
    void func_8007C6C0();
    void* func_8007C8C8();
    bool func_8007CBC8();
    void func_8007CBEC();
    void func_8007CDA8();
    void func_8007CE94();
    void func_8007CF64() const;
    void func_8007D190(unsigned long flags);
    void func_8007D1A0();
    void func_8007D794();
    void func_8007D7A4();
    void func_8007D834();
    void func_8007D84C();
    void* func_8007DA00();
    void func_8007DA0C();
    void func_8007DCB8();
    void func_8007DE94();
    void func_8007DECC();
    u16 func_8007E030();
    void func_8007E038();
    CfObject** func_8007E0C8();
    void func_8007E0D0();
    void func_8007E4DC();
    void func_8007E864();
    void func_8007E908();
    void func_8007E960();
    void func_8007E9CC();
    void func_8007EEF0();
    u32 func_8007EEF8();
    void func_8007EF04();
    void func_8007EF44();
    void func_8007EF48();
    void func_8007EF4C();
    void func_8007F054();
    void func_8007F0A4();
    void func_8007F0C4();
    void func_8007F114();
    void func_8007F11C();
    void func_8007F1FC();
    void func_8007F830();
    u16 func_8007F8B8();
    void** func_8007F8D0();
    void func_8007F8DC();
    void func_8007F8F4();
    bool func_8007F91C();
    void func_8007F990();
    void func_8007F9AC();
    void func_8007F9B4();
    void func_8007F9BC();
    void func_8007F9C4();
    void func_8007FBFC();
    void func_8007FC2C();
    void func_8007FC5C();
    void func_8007FD00();
    void func_8007FD8C();
    void func_8007FE18();
    void func_8007FE1C();
    void func_8007FE20();
    void func_8007FE24();
    void func_8007FE2C();
    void func_8007FECC();
    void func_8007FF6C();
    void func_8008064C();
    void func_800807BC();
    void func_80080888();
    u8 func_80080E20();
    u8 func_80080E28();
    void func_80080E30();
    void func_80080E44();
    void func_80080EE4();
    void func_80080F40();
    void func_80080F44();
    void func_80080F48();
    void func_80081258();
    void func_80081264();
    void func_8008126C();
    void func_80081274();
    void func_8008127C();
    void func_80081284();
    void func_8008128C();
    void func_80081294();
    void func_8008129C();
    void func_800812A4();
    void func_800812AC();
    void func_800812B4();
    void func_800812BC();
    void func_800812C4();
    void func_800812CC();
    void func_800812D4();
    void func_800812DC();
    void func_800812E4();
    void func_800812EC();
    void func_800812F4();
    void func_80081318();
    void func_80081330();
    void func_80081338();
    void func_80081340();
    void func_80081348();
    void func_80081350();
    void func_80081358();
    void func_80081694();
    Unk817A8Object* func_800817A8();
    bool func_800817B0();
    void func_800817BC();
    void func_80081874();
    void func_8008187C();
    void func_80081900();
    void func_80081988();
    void func_80081990();
    void func_80081A24();
    void func_80081A40();
    void func_80081CB0();
    static void* func_80081CB8();
    void func_80081CBC();
    void func_80081D2C();
    void func_80081D8C();
    void func_80081DD8();
    void func_80081E90();
    void func_80081F28();
    void func_80081F90();
    void func_80082008();
    void func_80082060();
    void func_80082088();
    bool func_80082104();
    void func_8008212C();
    UnkClass_800821F8* func_800821F8();
    CfObject* func_8008221C();
    void func_80082254();
    void func_80082258();
    void func_8008228C();
    void func_800822FC();
    void func_80082354();
    void func_8008235C();
    void func_800823A4();
    void func_80082418();
    void func_800824FC();
    void func_80082544();
    void func_80082568();
    void func_80082614();
    void func_8008261C();
    bool func_80082680();
    void func_80082694();
    void func_8008269C();
    void func_800826F0();
    void func_80082768();
    u16 func_80082770();
    void func_800827A8();
    void func_800827E4();
    void func_80082834();
    u32 func_800828DC();
    u32 func_80082900();
    void func_80082940();
    void func_8008294C();
    void func_80082A0C();
    void func_80082A7C();
    void func_80082B38();
    void func_80082C48();
    void func_80082D90();
    void func_80082E50();
    void func_80082EC0();
    void func_80082EC4();
    void func_80082F2C();
    void func_80082FE4();
    u32 func_80083100();
    u32 func_8008310C();
    void func_80083118();
    void func_80083284();
    void func_80083290();
    void func_800832BC();
    static void func_80083304();
    void func_80083328();
    u32 func_80083458();
    u32 func_80083460();
    void func_80083468(u32 value);
    void func_80083470();
    bool func_80083538();
    bool func_80083544();
    void func_80083550();
    void func_80083560();
    void func_800835FC();
    void func_8008360C();
    void func_80083718();
    void func_8008372C();
    void func_80083878();
    void func_80083888();
    void func_800838F4();
    void func_80083C70();
    void func_80083C78();
    void func_80083CC8();
    void func_80083CD8();
    void func_80083D50();
    void func_80083D70();
    void func_80083DEC();
    void func_80083EA4();
    void func_80083F28();
    void func_8008402C();
    void func_8008413C();
    void func_80084654();
    void func_80084A00();
    void func_80084AD4();
    bool func_80084B68();
    bool func_80084BAC();
    bool func_80084BF4();
    void func_80084C10();
    void func_80084CA4();
    void func_80084F50();
    void func_80085220();
    void func_80085248();
    void func_80085334();
    void func_800853C8();
    void func_8008566C();
    void func_80085838();
    bool func_80085840();
    bool func_8008585C();
    void func_80085878();
    void func_800858B8();
    void func_80085978();  // body lives in the extern "C" Fv-symbol definition below
    void func_80085E58();
    void func_80085FB8();
    void func_800862D0();
    void func_800863F4();
    void func_80086490();
    void func_800865E8();
    void func_800866A0();
    void func_8008670C();
    void func_80086778();
    void func_80086B04();
    void func_80086B08();
    void func_80086B0C();
    void func_80086B10();
    void func_80086B14();
    void func_80086B18();
    void func_80086B1C();
    void func_80086B24();
    void func_80086B2C();
    void func_80086B34();
    void func_80086B3C();
    void func_80086B44();
    void func_80086B48();
    void func_80086D90();
    void func_80086D94();
    void func_80086D98();
    bool func_80086D9C();
    static u32 func_80086DA0();
    void func_80086DA4();
    void func_80086DA8();
    void func_80086DAC();
    void func_80086DB0();
    void func_80086DB4();
    static u32 func_80086DBC();
    void func_80086E6C();
    bool func_80087244();
    bool func_80087250();
    void func_80087280();
    void func_80087330();
    void func_80087334();
    void func_80087348();
    void func_80087364();
    void func_80087378();
    void func_80087390();
    void func_800873AC();
    CfObjectSub54* func_800873C8();
    void func_800873D4();
    void func_800873E8();
    void func_800873FC();
    void func_80087410();
    u32 func_80087424();
    void func_8008742C();
    void func_8008743C();
    void func_80087448();
    }; //size = 0xB8
} // namespace cf

// Boot-time initializer holding the CfGameManager singleton construction
// (retail symbol init__17UnkClass_8007DAE0FP8CScnNw4rP5CViewUs).
class UnkClass_8007DAE0 {
public:
    static cf::CfGameManager* init(CScnNw4r* scene, CView* view, u16 arg);
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" void func_801A9FC0(UnkClass_8007E864* object);
extern "C" void func_8016EEB0(u32 object);
extern "C" void func_800754C0(CfCamEventManager* object);
extern "C" void* __dt__Q22cf13CfGameManagerFv(
    cf::CfGameManager* self, s32 deleteFlag);
// func_8009D790 is declared TU-locally where it is called (CfGameManager.cpp
// thunk passes 1 arg + r4 passthrough; pluginCfs.cpp declares the full
// 2-arg retail signature). Kept out of this header to avoid conflicting
// C-linkage redeclarations across TUs.
extern "C" void func_800B9340();
extern "C" void func_800B9364();
extern "C" void func_80079B34(CfCamEventManager* manager);
extern "C" bool func_80079DBC(CfCamEventManager* manager);
extern "C" cf::CfObject* func_8006E5A4(CfCamEventManager* manager);
extern "C" bool func_80068E44(u32 flag);
extern "C" bool func_800B45A0();
extern "C" void func_80141B20(cf::CfGameManager* manager);
extern "C" void func_800B1E2C(cf::CfGameManager* manager);
extern "C" void func_8047BDA0__17UnkClass_8047BB54Fv(UnkClass_80083298SubF0* subobject);
extern "C" bool func_800FF778__9CMainMenuFv();
extern "C" CSysWinBuff* getInstance__11CSysWinBuffFv();
extern "C" bool isInitialized__10CMenuPauseFv();
extern "C" bool func_80496044(CScn* scene);
extern "C" bool func_804960A8(CScn* scene);
extern "C" void func_80189510(float value);
extern "C" void func_801895F4(float value);
extern "C" void func_800B76CC();
extern "C" VoiceSource* func_800B76F4();
extern "C" void func_800B781C(void);
extern "C" VoiceSource* func_800B7854(VoiceSource* source = 0);
extern "C" u32 lbl_eu_8065FC18[];
extern "C" void func_800B93D0();
extern "C" void func_800B6C10();
extern "C" void func_80141B54();
extern "C" float func_80069EA0();
extern "C" void func_8006A12C();
extern "C" void func_8006A1A0();
extern "C" void func_8006A234(u16*, u16*);
extern "C" bool func_8006A2E0();
extern "C" u32 func_8006A33C();
extern "C" u32 func_8006A37C();
extern "C" u32 func_8006A3BC();
extern "C" void func_8006A3FC();
extern "C" void func_8006A404();
extern "C" void func_80141C6C(void*, void*);
// func_8009DBF4 / func_8009E0C4 are also declared (extern "C") in
// include/kyoshin/cf/CItem.hpp; the CfGameManager unity TU does not include
// that header, so they are re-declared here for the func_8007DCB8 bdat writer.
extern "C" void func_8009DBF4(void* a, unsigned long b, void* c);
extern "C" void func_8009E0C4(void* table, u16 index, u16 value);
extern "C" void func_8009EF9C(void* data, u32 value);
extern "C" void func_80158420(u32 value, void* result, s32 mode, void* other);
// func_8007DCB8 bdat-table scratch globals (.sbss /.data).
extern s8 lbl_eu_80663E78;
extern void* lbl_eu_80664090;
extern void* lbl_eu_80663E74;

// ---------------------------------------------------------------------------
// func_8007E218 / func_80084F50 / func_8007E514 teardown imports (retail
// C-linkage names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// CBattleManager vtable+0x1C dispatch view (the battle manager's slot is
// called with (mode 2, 0) after wiping the +0x94 payload). Cast-only: first
// declared virtual lands at vtable+0x08 (2 RTTI header entries), so the
// 6th declared virtual is slot +0x1C.
struct CBattleManagerVt1C {
    virtual void m08();
    virtual void m0C();
    virtual void m10();
    virtual void m14();
    virtual void m18();
    virtual void m1C(u32 first, u32 second);  // vtable +0x1C
};

// Minimal CBattleManager data view for func_80084F50: the +0x94 payload
// wiped with memset and the +0x194 party-gauge block handed to
// func_8018C8F4 before the vtable+0x1C dispatch.
struct CBattleManagerView {
    u8 field_0x0[0x94];
    u8 cleared_0x94[0x100];
    u8 field_0x194[8];
};

// Result of func_8009D5FC (file-event table query): the two u16 ids read by
// func_80084F50 (lhz +2 as first arg, lhz +0 as second arg).
struct CfFileEventIdsView {
    u16 field_0x0;
    u16 field_0x2;
    u8 field_0x4[0x8];
};

// func_8023C1B4 result view: caller loads the float at +0xC.
struct UnkC1B4Data {
    u8 field_0x0[0xC];
    float value_0xC;
};

// Cast-only dtor view used for delete-style release via vtable+0x08: retail
// `if (ptr) delete ptr` shows two identical null tests (the if plus the
// delete expansion's own guard), which a plain manual-cast virtual call
// cannot reproduce. Same shape as CItemBoxGrid.hpp CItemBoxObjVt08: MWCC
// prepends 2 RTTI header entries, so the first declared virtual (the dtor)
// lands at vtable+0x08 and delete dispatches there with the delete flag.
struct CfVt08Dtor {
    virtual ~CfVt08Dtor();  // vtable+0x08
};

// The player-file slots live at the very start of CfGameManager (0x00, 0x04,
// 0x08): func_8007C6C0 allocates the three CfTFile streams there and
// func_8007E218 tears them down with a 3-element pointer loop. The class
// proper types unk0/field_0x4/mObjectFlags as scalars, so the loop goes
// through this offset view.
struct CfGameManagerSlotArray {
    cf::CfObjectMove* slots[3];  // 0x00-0x0B
};

class CProcess;
// CProcess hooks removed by func_8007E218 (defined in other TUs, .sbss).
extern CProcess* lbl_eu_80663E1C;
extern CProcess* lbl_eu_80663E18;
extern CProcess* lbl_eu_80663E20;

extern "C" void func_8012F87C(u32 value);
extern "C" void func_802A1DA8();
extern "C" void func_800D91D0__Q22cf14CBattleManagerFv();
extern "C" void func_80295924();
extern "C" void __dt__80157150();
extern "C" void* __dt__801A9F78(void* self, s32 deleteFlag);
extern "C" void* __dt__801865C4(void* self, s32 deleteFlag);
extern "C" void* __dt__8007540C(void* self, s32 deleteFlag);
extern "C" void func_800B92D8();
extern "C" void* __dt__801886EC(void* self, s32 deleteFlag);
extern "C" void* __dt__801BF874(void* self, s32 deleteFlag);
extern "C" void* __dt__Q22cf17UnkClass_8018EF3CFv(void* self, s32 deleteFlag);
extern "C" void func_80068AC8();
extern "C" void func_8009CE88();
extern "C" void func_80069A18(cf::CfObjectMove* self);
extern "C" u32 func_801BF93C__Fv();
extern "C" void func_800A7D9C();
extern "C" void Remove__8CProcessFv(CProcess* process);
extern "C" void func_8003AA8C__5CBdatFUl(u32 value);

// --- UnkClass_8007DAE0::init / func_8008566C imports (retail C-linkage names)
extern "C" void __ct__8009D604();
extern "C" void __dt__801570F0();
extern "C" void func_800D9190__Q22cf14CBattleManagerFv();
extern "C" void __ct__Q22cf12CHelpManagerFv();
extern "C" void func_802A14B8();
extern "C" CProcess* GetRootProcRealTime__12CTaskManagerFv();
extern "C" CProcess* GetRootProcGame__12CTaskManagerFv();
extern "C" CProcess* create__Q22cf9CfPadTaskFP8CProcess(CProcess* parent,
                                                          void* scene);
extern "C" CProcess* __ct__Q22cf5CfResFv(CProcess* parent, void* scene);
extern "C" CProcess* __ct__800697E8(CProcess* parent, void* scene);
extern "C" void func_800A7CDC();
extern "C" void func_8019FB40();
extern "C" void func_800B92B4();
extern "C" void func_80068A80();
typedef void (*UnkGameManagerCallback)();
extern "C" void func_800C1E40();
extern "C" void func_800C1E9C(UnkGameManagerCallback callback, s32 value);
extern "C" void func_800627FC(u32 value, u32 mode);
extern "C" void func_80062860(u32 value, u32 mode);
extern "C" void func_80068A20(s32 value);
extern "C" bool func_8007DECC__Q22cf13CfGameManagerFv(s32 value, s32* current,
                                                        u32 size);
// Scene state query result: three words plus a float threshold at +0xC.
struct UnkScnResult {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    float field_0xC;
};
// Forward decl: full definition lives in CTaskGame.hpp (identical-layout
// view). The FUNCTION must return this exact type - divergent return types
// trip MWCC 10505 when both headers land in one TU.
struct CTaskGameCamView;
extern "C" void* func_8049603C(CScn* scene);
// NOTE: keep this prototype IDENTICAL to the ones in src/kyoshin/CTaskGame.hpp
// and src/kyoshin/cf/CTaskREvtSequence.hpp (void*, int, void*) - divergent
// extern "C" type lists for the same symbol make any TU that includes more
// than one of these headers fail with "illegal function overloading" (10197).
extern "C" void func_8049602C(void* scene, int mode, void* vec);
extern const float lbl_eu_80666568;

// --- func_80084F50 imports ---
extern "C" void func_80135FDC();
extern "C" void func_802062BC();
extern "C" void func_80164CFC();
extern "C" void func_802A1F9C();
extern "C" int func_8023C1C0();
extern "C" void* func_8023C1B4();
extern "C" void func_8009F6D4(void* object);
extern "C" void __dt__8023E448();
extern "C" void func_801592EC();
extern "C" CfFileEventIdsView* func_8009D5FC();
extern "C" void func_8018C8F4(u8* object, u32 value);
extern "C" CBattleManagerView* getInstance__Q22cf14CBattleManagerFv();
extern "C" void func_802959AC(cf::CHelpManager* object);
extern "C" void func_8007F1FC__Q22cf13CfGameManagerFv(void* buffer, s32 mode);
extern "C" void func_80083D50__Q22cf13CfGameManagerFv(u32 first, u32 second,
                                                         u32 third, u32 fourth,
                                                         float value);
// --- func_80086778 bdat-object collision imports (defined in CfObjectColl unit) ---
extern "C" void func_800AC110(void* self, const void* pA, const void* pB, float f);
extern "C" void func_800AC1BC(void* self, const void* pA, const void* pB, float f);
extern "C" void func_800AC450(void* self, unsigned long a, unsigned long b);
// CHelpManager singleton (func_802959AC argument; .sdata).
extern cf::CHelpManager* lbl_eu_80664A10;
// Event-done halfword countdown flag (.sbss) cleared by func_80084F50.
extern u16 lbl_eu_80664774;
// Fade/reset float pair written with the shared zero constant.
extern float lbl_eu_80663ED8;
extern float lbl_eu_80663EDC;

// --- func_8007C8C8 imports ---
extern u32 lbl_eu_80663D90;
extern u32 lbl_eu_80663E6C;
extern "C" void* func_80190840(void* object, u32* e04);
extern "C" void func_801889D0(u32 object);
extern "C" void func_800D9354(CBattleManagerView* self);
extern "C" void func_80295A88(cf::CHelpManager* object);
extern "C" void func_802A1610();
extern "C" void func_8006A75C();
extern "C" void func_8016F140(u32 object);
extern "C" void func_8016F144(u32 object);
extern "C" void func_800FE104();
extern "C" void func_8019FD2C();
extern "C" void func_80069F2C();
extern "C" void func_80462CD8__8CTaskLODFv(u16 first, u16 second, u16 third);
extern "C" u32 func_80082900__Q22cf13CfGameManagerFv();
extern "C" void func_800827E4__Q22cf13CfGameManagerFv();

// --- func_8007F1FC imports (CtrlObjectParam / cfsys CTaskParty units) ---
extern "C" void func_8009E3C0();
extern "C" int func_8009E740(cf::CtrlObjectParamSlots* self, int value);
extern "C" int func_8009E56C(cf::CtrlObjectParamSlots* self, int value, int type);
extern "C" int func_8009E574(cf::CtrlObjectParamSlots* self, int value, int type, int index);
extern "C" void func_8009E838(u8* self);
extern "C" s32 func_80291BF8();
extern "C" void func_8015720C(s32 value, s32 mode);
extern "C" u32 func_800A082C(void* data);
extern "C" void func_800A0860(void* data, u16 value);
extern "C" void func_800A21F8(void* data, u32 a, u32 b, u32 c);
extern "C" void func_800A2974(void* data, u16 value);
extern "C" void func_8025EE7C(void* block, u32 value);
extern "C" void func_8007E9CC__Q22cf13CfGameManagerFv(u16 value, u32 mode);
extern const double lbl_eu_80666518;
extern const double lbl_eu_80666520;
extern u32 lbl_eu_805276F0[];

// --- imports for func_80084654 / func_80085978 / func_8007E9CC / func_8008413C ---
namespace ml { class CVec3; }
// Mangled global C++ function (retail func_800AD860__FPv) - NOT extern "C";
// CfObjectMove.hpp declares the same prototype.
void* func_800AD860(void* object);
extern "C" void func_8009D018(u32 first, u32 second);
extern "C" void* func_801412D0(u32);
extern "C" void func_8013F244();
extern "C" void func_8015B11C();
extern "C" void* func_802A2424();
extern "C" s32 func_8011C2E8();
extern "C" s32 func_8011C2FC();
extern "C" void func_800B1C78(s32 value);
extern "C" void CfRes_callFunc_67F10(s32 value);
extern "C" bool func_80061D2C(UnkClass_80085334* object, u32 mode);
extern "C" cf::CfGameManager* func_8015783C(u32 mappedIndex, u16 dataId, u32 slot);
extern "C" void* func_8015730C(u32 kind, u32 slot, u32 mode);
extern "C" s8* func_80157948(u16 dataId, u32 slot);
extern "C" u32 func_80157FDC(u32 value);
class CItemImplInstances;
extern "C" CItemImplInstances* CItem_initItemImplInstances();
extern "C" void func_8009DB28(void* data, u32 index);
extern "C" void func_8009D790_full(void* data, u32 index);
extern "C" void func_80141D48(void* dst, void* src, u8 value);
extern "C" void func_8027F148();
extern "C" void func_80068C7C();
extern "C" void func_80068D14();
extern "C" void func_80068DAC();
extern "C" void func_800B93AC();
extern "C" void func_800F3C08(void* battle, u32 value);
extern "C" void* func_8004B7C0(void* out, const ml::CVec3* src);
extern "C" void func_80199678(void* ctrl, int flag); // 2-arg form per CtrlMovePC.cpp definition - keep in sync with TU-local decls
// zero vector constant exported by the ml unit. Declared by
// CfGameManagerUnityHelpers.hpp (ResetVectorWords view) in this TU; a global
// extern here would clash with that declaration's type.
// Event-done halfword counters read by func_8008413C.
extern u16 lbl_eu_80663E42;
extern u16 lbl_eu_80663E44;

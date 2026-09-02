#pragma once

#include <types.h>

#include "monolib/work/CTTask.hpp"
#include "monolib/lib/CLibCri.hpp"
#include "monolib/core/CViewFrame.hpp" // ml::CRect
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/device/CDeviceVICb.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/math.hpp"
#include "kyoshin/CGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/IFlagEvent.hpp"
#include "kyoshin/realtimeevt/CREvtMem.hpp"
#include "kyoshin/code_80296898.hpp"
#include <revolution/wpad/WPAD.h>
#include <revolution/vi/vi.h>

u32 isEventPending();

class CScnNw4r;
class CView;
class CFileHandle;
class CEventFile;

namespace cf {
class CTaskREvent;
struct CInfoCf;
}

namespace cf {
class CTaskREvent;
// Layout view exposing the CTTask PTMF slots (+0x3C/+0x48) for null-init.
struct CTaskREventPtms {
    u8 pad[0x3C];
    void (CTaskREvent::*move)();
    void (CTaskREvent::*draw)();
};
}

// Object pointed at by CEventMgr::field_0xB0; only byte +0x39 is touched
// (func_80164CFC clears it to 1 while tearing an event down).
struct CEventMgrB0 {
    u8 field_0x00[0x39];
    u8 field_0x39;          // +0x39 byte flag
};

// Manager object behind lbl_eu_80664240 (recovered layout; only the fields
// this TU reads are declared).
struct CEventMgr {
    u8 field_0x00[0x6C];
    volatile u32 field_0x6C;   // +0x6C flag word (bit0/bit6 read by this TU)
    u8 field_0x70[0x74 - 0x70]; // +0x70 CInfoCf subobject storage (shared with the task's mInfoCf)
    u32 field_0x74;          // +0x74 flag word (bit0 read by func_80164C48)
    u8 field_0x78[0xB0 - 0x78];
    CEventMgrB0* field_0xB0; // +0xB0 gate object (null-checked by func_80164410 / func_80164DB8 / func_80164C48)
    u8 field_0xB4[0x1BC - 0xB4];
    u8 field_0x1BC;          // +0x1BC byte flag
    u8 field_0x1BD[0x1D0 - 0x1BD];
    u32 field_0x1D0;         // +0x1D0 word cleared to zero by func_80164DB8
    s32 field_0x1D4;         // +0x1D4 signed counter/index
    u8 field_0x1D8[0x1E0 - 0x1D8];
    CLibCri* mCri;           // +0x1E0 CRI movie-play controller, or (CLibCri*)-1
};

// 32-slot event data table filled by func_8016455C: byte slots at +0x00
// (0x28 each, 20-byte sub-slots set to 1) and word slots at +0x500 (0x20
// each, 16-byte sub-slots zeroed), plus per-slot u8/u16 flags.
struct CEventDataTable {
    struct ByteSlot {
        u8 data[0x14];
    };
    struct WordSlot {
        u32 data[4];
    };
    ByteSlot mBytes[32][2];  // +0x000 (32 x 0x28)
    WordSlot mWords[32][2];  // +0x500 (32 x 0x20)
    u8 mFlags900[32];        // +0x900
    u16 mIds920[32];         // +0x920
};

// Event-file view consumed by func_801663A8 / the OnFileEvent thunk. The
// shared monolib CEventFile only declares the head/tail; this TU reads the
// mid fields (type, handle, packed word, data buffer).
struct CTaskREventFileEvent {
    s32 field_00;           // +0x00 event type (1 = data loaded)
    CFileHandle* field_04;  // +0x04 owning file handle
    u8 field_08[0xC - 0x08];
    u32 field_0C;           // +0x0C
    u32 field_10;           // +0x10 loaded-data buffer pointer
};

// Loaded event-data buffer walked by func_801663A8: entries start at +0x08
// (an offset into the buffer), count at +0x20.
struct CTaskREventDataBuf {
    u8 field_0x00[0x8];
    u32 field_0x8;          // +0x08 offset of the first entry
    u8 field_0xC[0x20 - 0xC];
    u32 field_0x20;         // +0x20 entry count
};

// Linked-list entry inside the event-data buffer (id -1 terminates the walk,
// id 3 is the target record; size advances to the next entry).
struct CTaskREventDataEntry {
    s32 field_0x0;          // +0x00 record id
    u32 field_0x4;          // +0x04 size (advance to next)
    u8 field_0x8[0x20 - 0x8];
    u32 field_0x20;         // +0x20 packed word passed to func_800AA318
    u8 field_0x24[0x28 - 0x24];
    u32 field_0x28;         // +0x28 parse flag (1 = parse)
};

// Camera/player object behind getCameraDataBlock's field_0xC (only the floats
// this TU reads are declared).
struct CfEvtCamPlayerObj {
    u8 field_0x00[0x1EC];
    f32 field_0x1EC;        // +0x1EC
    f32 field_0x1F0;        // +0x1F0
};

// Object returned by getCameraDataBlock (CfGameManager.hpp forward-declares it;
// only field_0xC is read here).
struct UnkClass_800821F8 {
    u8 field_0x00[0xC];
    CfEvtCamPlayerObj* field_0xC; // +0x0C
};

// Character/party data blob behind func_8009D5FC (recovered layout; only the
// regions this TU reads are declared). Byte rows at +0x388B are flat (the
// retail indexes them as index*0x28 + row*5 + col with row 1..7), word rows
// at +0x3D8C are [index][row], then per-index byte flags and halfwords.
struct CEventCharBlob {
    u8 field_0x00[0x388B];
    u8 mByteRows[32][0x28];   // +0x388B (32 x 0x28)
    u8 field_0x3D8B;          // +0x3D8B
    u32 mWordRows[32][8];     // +0x3D8C (32 x 0x20)
    u8 field_0x418C[0x4190 - 0x418C];
    u8 mByteFlags[32];        // +0x4190
    u16 mHalfSlots[32];       // +0x41B0
};

// The shared CfGameManager.hpp types func_8009D5FC's return as the
// 0xC-byte CfFileEventIdsView; this TU walks the full blob through this
// offset view, so funnel every access through one cast helper.
inline CEventCharBlob* evtCharBlob() {
    return reinterpret_cast<CEventCharBlob*>(func_8009D5FC());
}

// View over the vtable-pointer slots the dtor resets before destruction.
// 0x10 is the CTTask primary vtable slot (compiler-managed), 0x54-0x60 the
// four secondary subobject vtables; the retail dtor writes all five from the
// retail vtable symbol lbl_eu_80530790 by hand.
struct CTaskREventVtblView {
    u8 field_0x00[0x10];
    void* vtbl10;              // 0x10
    u8 field_0x14[0x54 - 0x14];
    void* vtbl54;              // 0x54
    void* vtbl58;              // 0x58
    void* vtbl5C;              // 0x5C
    void* vtbl60;              // 0x60
};

// The retail OnFileEvent symbol is a global-namespace CTaskREvent member
// (the IWorkEvent dispatch thunk on the cf::CTaskREvent +0x60 subobject):
// subtract the subobject offset and tail-call func_801663A8.
class CTaskREvent {
public:
    void OnFileEvent(CEventFile* ev);
};

class IWorkEvent;

// View of the embedded sub-object dispatched by cf::CTaskREvent::Move():
// only the vtable slot 0x168 (index 88) fade call is exercised.
struct REvtMoveSub {
    virtual void* m00(); virtual void* m01(); virtual void* m02(); virtual void* m03();
    virtual void* m04(); virtual void* m05(); virtual void* m06(); virtual void* m07();
    virtual void* m08(); virtual void* m09(); virtual void* m10(); virtual void* m11();
    virtual void* m12(); virtual void* m13(); virtual void* m14(); virtual void* m15();
    virtual void* m16(); virtual void* m17(); virtual void* m18(); virtual void* m19();
    virtual void* m20(); virtual void* m21(); virtual void* m22(); virtual void* m23();
    virtual void* m24(); virtual void* m25(); virtual void* m26(); virtual void* m27();
    virtual void* m28(); virtual void* m29(); virtual void* m30(); virtual void* m31();
    virtual void* m32(); virtual void* m33(); virtual void* m34(); virtual void* m35();
    virtual void* m36(); virtual void* m37(); virtual void* m38(); virtual void* m39();
    virtual void* m40(); virtual void* m41(); virtual void* m42(); virtual void* m43();
    virtual void* m44(); virtual void* m45(); virtual void* m46(); virtual void* m47();
    virtual void* m48(); virtual void* m49(); virtual void* m50(); virtual void* m51();
    virtual void* m52(); virtual void* m53(); virtual void* m54(); virtual void* m55();
    virtual void* m56(); virtual void* m57(); virtual void* m58(); virtual void* m59();
    virtual void* m60(); virtual void* m61(); virtual void* m62(); virtual void* m63();
    virtual void* m64(); virtual void* m65(); virtual void* m66(); virtual void* m67();
    virtual void* m68(); virtual void* m69(); virtual void* m70(); virtual void* m71();
    virtual void* m72(); virtual void* m73(); virtual void* m74(); virtual void* m75();
    virtual void* m76(); virtual void* m77(); virtual void* m78(); virtual void* m79();
    virtual void* m80(); virtual void* m81(); virtual void* m82(); virtual void* m83();
    virtual void* m84(); virtual void* m85(); virtual void* m86(); virtual void* m87();
    virtual void vfn88(f32 v);   // index 88 -> vtable 0x168 (fade)
};

// Battle gate object behind REvtActor::field_0x3F34 (bit 0x10 of +0x7A4).
struct REvtGateObj {
    u8 _00[0x7A4];
    u32 field_0x7A4;
};

// Actor/move-object view walked by Move()'s battle fade loops.
struct REvtActor {
    u8 _0000[0x3E9C];
    REvtMoveSub mSub;                       // +0x3E9C (own vtable)
    u8 _3EA0[0x3F0C - 0x3EA0];
    u32 field_0x3F0C;                       // packed word for func_800AA318
    u8 _3F10[0x3F34 - 0x3F10];
    REvtGateObj* field_0x3F34;
};

// Circular list node over actors (+0x00 next, +0x08 item).
struct REvtListNode {
    REvtListNode* next;
    REvtActor* item;
};
struct REvtListHead { REvtListNode* first; };

// Manager view returned by getGimmickListHead/getGimmickList (+0x04 list head).
struct REvtMgrView {
    u8 _00[0x4];
    REvtListHead* list;
};

// Data symbols (retail linker names - global scope, no extern "C" needed)
extern const char* lbl_eu_80662380; // base event-name string (func_801644AC returns it as u32)
extern u32 lbl_eu_80662384;
extern f32 lbl_eu_8066762C;
extern f32 lbl_eu_80667628; // ctor float initializer (field_0x1E4/0x1E8)
extern f64 lbl_eu_80667630; // u32->float conversion constant (2^52+2^31)
extern f32 lbl_eu_80667644; // func_80165DF4 camera-position write value (x)
extern f32 lbl_eu_80667648; // func_80165DF4 camera-position write value (y)
extern u16 lbl_eu_80663E42; // func_801663A8 expected event-id halfword
extern u16 lbl_eu_80663E44; // func_801663A8 expected event-id halfword
// Event-name table blob (retail .data:0x80530300): byte table at +0x130
// indexes the pointer table at +0x98; the +0x158 pointer table is indexed
// directly by the event id.
extern u8 lbl_eu_80530300[];
extern CEventMgr* lbl_eu_80664240;
extern u32 lbl_eu_80664244; // auto-sleep/dimming state word written by the ctor + siblings
// Frame-target word: getTargetFramerate() * 300 (ctor + func_80166050/80166150).
extern u32 lbl_eu_80664248;
extern u32 lbl_eu_80663E28; // event flag word (bit7 tested by isEventPending)
extern s16 lbl_eu_80502F90[]; // event-id table walked by func_801644D8 (32 entries)
extern const char lbl_eu_80503008[];  // Init: base string copied into each entry
// Init: 0xAE suffix strings appended to each entry (walked by pointer).
extern const char* lbl_eu_80530458[];
// Event-name suffix table walked by func_80164838 (32 const char* entries).
extern const char* lbl_eu_80530710[];
// 0x26-byte array shuffled by func_80164954 (retail .data:0x80530430).
extern u8 lbl_eu_80530430[];
// CTaskREvent vtable (retail .data:0x80530790; five sub-vtables: primary at
// +0x00, then +0x24 / +0x3C / +0x4C / +0x5C for the 0x54/0x58/0x5C/0x60
// secondary subobjects).
extern u8 lbl_eu_80530790[];
// Interim CTTask<cf::CTaskREvent> vtable (retail .data:0x805308A8): written by
// the inlined CTTask ctor at +0x10 before the final CTaskREvent vtable.
extern char lbl_eu_805308A8[];
// Null pointer-to-member-function constant (3 words, used by the ctor to
// clear the CTTask mMoveFunc/mDrawFunc members).
extern u32 __ptmf_null[3];

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
    int func_80164C28();
    void func_80165014();
    float func_80164478();
    int func_80164FB4();
    void func_80166150(CEventMgr* self, u32 arg);
    // Value-returning tail call through the CRI movie controller: CLibCri.hpp
    // declares isMovieGlobalPaused as void, but retail func_80164FB4 performs a
    // value tail call through it, so import the mangled symbol with an
    // explicit value signature.
    u32 isMovieGlobalPaused__7CLibCriFv(CLibCri* self);
    void func_80164F6C();
    void func_80165DF4(cf::CTaskREvent* self, int arg);
    void func_80166050(cf::CTaskREvent* self, int arg);
    void func_8016462C(u32 index);
    // CRI movie-player setup: retail func_80164ED0 passes 4 extra words even
    // though the retail symbol is Fv; declared here with the caller's shape.
    CLibCri* startMovie__7CLibCriFv(const char* self, u32 memHandle, u32 buffer, int flag, int zero);
    u32 func_8016847C();
    u32 func_80495FF0(CScn* scene);
    void func_80164ED0(const char* path, int flag, u8* handle);
    int func_80164C48();
    void func_80164CFC();
    void func_8016455C(CEventDataTable* self);
    u32 func_80164724(const char* key, u32 type, u32 slot);
    // Retail ctor symbol is the pre-mangled name __ct__cf_CTaskREvent (a
    // global function, not a cf::CTaskREvent member); declared here so the
    // definition in the .cpp keeps C linkage and emits the plain name.
    cf::CTaskREvent* __ct__cf_CTaskREvent(cf::CTaskREvent* pMem, CScnNw4r* pScene, CView* pView);
    // Targets defined in this TU (declared here so the definitions keep C
    // linkage and emit the retail unmangled symbol names).
    int func_80164410();
    void func_80164DB8();
    void func_801644D8(cf::CTaskREvent* self, int type, int upper, int lower);
    cf::CTaskREvent* func_801665A4(CProcess* pParent, CScnNw4r* pScene, CView* pView);
    int func_80164838(const char* key, int slot);
    void func_801662E8(cf::CTaskREvent* self);
    int func_80164954();
    // Targets defined in this TU: func_80164A50 (event-file load kick) and
    // func_801663A8 (async OnFileEvent handler) emit the retail unmangled
    // symbols through these C-linkage declarations.
    int func_80164A50(const char* path, int arg1, int arg2);
    int func_801663A8(cf::CTaskREvent* self, CTaskREventFileEvent* ev);
    // Imports for func_80164DB8
    void func_80168484(int arg);
    void func_80043BC4();
    // Imports for cf::CTaskREvent::cbRenderBefore
    CView* getCurrentView__5CViewFv();
    void func_8043EA88__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect, CView* view);
    void renderMovie__7CLibCriFv(CLibCri* self, ml::CRect& rect);
    // CfGameManager helpers: retail symbols are Fv but the call sites pass
    // real arguments / read the return (declared here with the caller shape,
    // same scheme as startMovie__7CLibCriFv above).
    u32 getBdatEntryColumn__Q22cf13CfGameManagerFv(u32 index, u32 value);
    void getControllerValues__Q22cf13CfGameManagerFv(u16* first, u16* second);
    // Play-time seconds getter (same signature as CfMapEffectManager.hpp).
    u16 func_8016DF2C();
    // Frame/timing helpers used by func_801662E8 (global retail names).
    int func_80043D68();
    int func_80043B54();
    int func_8016A3A8();
    int func_eu_8016DA48(u8* gate);
    // Event-callback unregister helper (retail global taking the IFlagEvent
    // subobject; same signature as CUICfManager.cpp / CUIWindowManager.cpp).
    void func_8009D514(cf::IFlagEvent* flagEvent);
    // Embedded subobject dtors driven by ~CTaskREvent (retail CW names).
    void __dt__Q22cf8CREvtMemFv(cf::CREvtMem* self, int flag);
    void __dt__Q22cf7CInfoCfFv(cf::CInfoCf* self, int flag);
    void __dt__11CDeviceVICbFv(CDeviceVICb* self, int flag);
    void __dt__8CProcessFv(CProcess* self, int flag);
    void __dl__FPv(void* p);
    // Base/subobject ctors driven by the free-function ctor (retail C-ABI
    // names: CProcess/CDeviceVICb ctors are out-of-line members, CInfoCf/
    // CREvtMem have no member ctor).
    void __ct__8CProcessFv(CProcess* p);
    void __ct__11CDeviceVICbFv(CDeviceVICb* p);
    cf::CInfoCf* __ct__cf_CInfoCf(cf::CInfoCf* p);
    void __ct__cf_CREvtMem(cf::CREvtMem* p);
    // IFlagEvent registration helper (mirror of func_8009D514 used by the
    // dtor; retail C-ABI name).
    cf::IFlagEvent* func_8009D414(cf::IFlagEvent* p);
    // Imports for func_80164A50
    void func_801667AC(cf::CInfoCf* self);
    void func_80166784(cf::CInfoCf* p);
    int getFileSize__11CDeviceFileFPCc(const char* path, int flag);
    // Spawns the realtime-event task for `name` under `parent` and returns
    // its gate object (same signature as CTaskREvtSequence.hpp).
    CEventMgrB0* func_8016AED4(CProcess* parent, const char* name);
    void func_8016C2C8();
    // Imports for func_801663A8
    void func_8016C450(u32 a, u32 b, u32 c);
    void func_800AA318(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
    // Imports for func_80165DF4
    s32 isMoveFuncActive__9CTaskGameFv();
    Class_80296898* getInstance__14Class_80296898Fv();
    bool func_8012CD24();
    UnkClass_800821F8* getCameraDataBlock__Q22cf13CfGameManagerFv();
    void func_8049EB60();
    // Imports for Move (CRI player state / frame timing / object lists)
    void func_80043B04(float v);
    u32 isMoviePlaying__7CLibCriFv(CLibCri* self);
    void setMoviePause__7CLibCriFv(CLibCri* self, u32 arg);
    REvtMgrView* getGimmickListHead__Q22cf13CfGameManagerFv();
    REvtMgrView* getGimmickList__Q22cf13CfGameManagerFv();
    REvtActor* getCfObjectPc__FPQ22cf12CfObjectMove(REvtActor* obj);
    REvtActor* getEffOwner____FPv(REvtActor* obj);
    void deactivateLOD__8CTaskLODFv(s16 taskID);
    void activateLOD__8CTaskLODFv(s16 taskID);
    // Imports for cf::CTaskREvent::Move
    int func_801684F4();
    void func_8016C6EC(int arg);
    int func_8016A35C();
    int func_80043BA4();
    void func_804962A8(u8* self, u32 flag);
    u32 func_800EA444(u32 battle);
    extern f32 lbl_eu_80667638;
    extern f32 lbl_eu_8066763C;
    extern f32 lbl_eu_80667640;
    void func_80086B5C__Q22cf13CfGameManagerFv(u32 a, u32 b);
    u8* getGlobalSda();
    void func_800599E0(u32 a, u32 b, u32 c, u32 d);
    CFileHandle* readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        u32 handle, const char* path, IWorkEvent* sink, int a, int b);
}

namespace cf{
    class CTaskREvent : public CTTask<CTaskREvent>{
    public:
        static CTaskREvent* getInstance();
        static CTaskREvent* create(CProcess* pParent, CScnNw4r* pScene, CView* pView);

        //0x000-0x054 CTTask
    void* mVtbl54;              //0x054 secondary vtable (CDeviceVICb subobject)
    void* mVtbl58;              //0x058 secondary vtable (IFlagEvent subobject)
    void* mVtbl5C;              //0x05C secondary vtable (IScnRender subobject)
    void* mVtbl60;              //0x060 secondary vtable (unknown base subobject)
    u32 field_0x64;             //0x064
    u32 field_0x68;             //0x068
    volatile u32 field_0x6C;    //0x06C flag word (|= / &= ~0x100 by func_801662E8)
    u8 mInfoCf[0x14];           //0x070 cf::CInfoCf storage (vtable + 0x10 bytes)
    CREvtMem mEvtMem;           //0x084 realtime-event memory subobject (0x2C bytes)
    u8* field_0xB0;             //0x0B0 gate object pointer (null-checked by func_801662E8)
    char mNameBuf[0x100];       //0x0B4 event-name string buffer (length in field_0x1B4)
    u32 field_0x1B4;            //0x1B4
    u32 field_0x1B8;            //0x1B8
    u8 field_0x1BC;             //0x1BC byte flag
    char unk1BD[0x1C0 - 0x1BD]; //0x1BD
    CFileHandle* field_0x1C0;   //0x1C0 file handle (cancelled by Term)
    CFileHandle* field_0x1C4;   //0x1C4 file handle (cancelled by Term)
    u8* mBuf;                   //0x1C8 Init: allocated string-table buffer (0x1400 bytes)
    u32 mIdx;                   //0x1CC Init: current table entry index
    u32 field_0x1D0;            //0x1D0
    s32 field_0x1D4;            //0x1D4
    u32 field_0x1D8;            //0x1D8
    u32 field_0x1DC;            //0x1DC
    CLibCri* mCri;              //0x1E0 CRI movie-play controller, or (CLibCri*)-1
    f32 field_0x1E4;            //0x1E4
    f32 field_0x1E8;            //0x1E8
    u16 field_0x1EC;            //0x1EC
    u16 field_0x1EE;            //0x1EE
    u32 field_0x1F0;            //0x1F0
    u32 field_0x1F4;            //0x1F4

    static CTaskREvent* spInstance;
public:
    virtual ~CTaskREvent();
    void Init();
    void Term();
    void Move();
    void Draw();
    void cbRenderBefore();
    void OnFileEvent();
    }; //size = 0x1F8
} //namespace cf

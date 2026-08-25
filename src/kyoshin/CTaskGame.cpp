#include "kyoshin/CTaskGame.hpp"
#include "libs/monolib/src/scn/CScn_8049603C.hpp" // func_8049603C (single owner decl)
#include "kyoshin/cf/CTaskGameCf.hpp"
#include "kyoshin/cf/CfObjectEnumList.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CGame.hpp"
#include "kyoshin/CLoad.hpp"

// --- CTTask<CTaskGame> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move/Draw/dtor symbols.
// (static_cast mirrors the CTTask.hpp inline body; MWCC lowers the PTMF
// dispatch to __ptmf_test/__ptmf_scall.)
template<>
void CTTask<CTaskGame>::Move() {
    if (mMoveFunc) {
        (static_cast<CTaskGame*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CTaskGame>::Draw() {
    if (mDrawFunc) {
        (static_cast<CTaskGame*>(this)->*mDrawFunc)();
    }
}

// Retail __dt__18CTTask<9CTaskGame>Fv is 0x50 (stmw r30 frame); the default
// frame spills stw individually (0x58), so keep optimize_for_size on like
// CTaskGameEff's dtor.
#pragma optimize_for_size on
template<>
CTTask<CTaskGame>::~CTTask() {}
#pragma optimize_for_size off

CTaskGame* lbl_eu_80663D18;
void* CTaskGame_cLoadInstance;
u32 lbl_80666624;
u32 lbl_80666630;
u32 lbl_80666634;

struct Fields {
    u8 field0;
    float field4;
    int field8;
    u32 fieldC;
    u8 field10;
    u32 field14;
    u32 field18;
};

// __declspec(noinline): retail callers (the ctor init / func_8004312C) call
// this 32-byte setter out of line; without it MWCC inlines the 7 stores.
// a5 is u32 (not u8): retail call sites (func_8004392C) pass the value with
// a plain mr, no rlwinm truncation; field10 is u8 so the store still emits
// stb.
extern "C" __declspec(noinline) void func_8004041C(Fields* self, u8 a1, float a2, int a3, u32 a4, u32 a5, u32 a6, u32 a7) {
    Fields& f = *self;
    f.field0 = a1;
    f.field4 = a2;
    f.field8 = a3;
    f.fieldC = a4;
    f.field10 = a5;
    f.field14 = a6;
    f.field18 = a7;
}

CTaskGame::CTaskGame(CView* pView, CWorkThread* pThread, int r6) :
    unk68(0),
    unk6C(pThread),
    unk70(pView),
    unk74(0),
    unk78(0),
    unk7C(0),
    unk80(1),
    unk82(1),
    unk84(0),
    unk86(0),
    unk88(0),
    unk8A(0),
    unk8E(0),
    unk90(0),
    unkA0(0),
    unkA4(),
    unkC8(r6),
    unkCC(0),
    unkD0(0),
    unkD4(0),
    unkD8(-1),
    unkDC(2),
    unkE0(1),
    unkE4(0),
    unkE8(-1),
    unkEC(0),
    unkF0(0),
    unkF4(0),
    unkF8(-1),
    unkFC(0),
    unk100(0),
    unk104(0),
    unk124(0),
    unk128(0),
    unk130(0),
    unk170(0),
    unk188(0) {
    unk18C.func_8004041C(0, -1, 2, 0, 0, 0, 1);
    lbl_eu_80663D18 = this;
    CTaskGame_cLoadInstance = nullptr;
    lbl_80666624 = 0;
    lbl_80666630 = 0;
    lbl_80666634 = 0;
}

// Retail dtor saves r30/r31 as stmw/lmw (size-mode prologue) and destroys
// ONLY the CProcess base. The IWorkEvent base-dtor call is elided because
// the strong empty copy below is in this TU (MWCC empty-function call
// elimination); the redundant this-guard branch retail shows stays.
#pragma push
#pragma optimize_for_size on
CTaskGame::~CTaskGame(){
    lbl_eu_80663D18 = nullptr;
}
#pragma pop

// Strong empty dtor for retail symbol placement (0x80040858 lives in this
// split). Declaration-only in the header (key function) so the IWorkEvent
// vtable is emitted here and no weak copies leak into other TUs; the empty
// body lets MWCC elide the base-dtor call in this TU's derived dtor.
IWorkEvent::~IWorkEvent() {}

CTaskGame* CTaskGame::getInstance(){
    return lbl_eu_80663D18;
}

u32 CTaskGame::func_800404F0() {
    CTaskGame* instance = lbl_eu_80663D18;
    if (instance == NULL) {
        return 0;
    }
    return (instance->unk68 >> 13) & 1;
}

void CTaskGame::Init(){

}

// Retail create saves r28-r31 with a single stmw r28 (MWCC merges the
// callee-saved saves only under optimize_for_size, same as the CTTask / reslist
// dtors in this TU).
#pragma optimize_for_size on
CTaskGame* CTaskGame::create(CView* pView, CWorkThread* pThread, int r5){
    mtl::ALLOC_HANDLE handle = CWorkThreadSystem::getWorkMem();
    CTaskGame* taskGame = new (handle) CTaskGame(pView, pThread, r5);
    //Register CTaskGame under the root game process
    CProcess* gameProcess = CTaskManager::GetRootProcGame();
    taskGame->Regist(gameProcess, false);
    return taskGame;
}
#pragma optimize_for_size off

// __declspec(noinline): retail callers (func_80040DE4 / func_80040B38) call
// this vec4 setter out of line; without it MWCC inlines the 4-store body.
extern "C" __declspec(noinline) func_800407C8_tmp* func_800407C8(func_800407C8_tmp* self, f32 f1, f32 f2, f32 f3, f32 f4) {
    self->unk00[0] = f1;
    self->unk00[1] = f2;
    self->unk00[2] = f3;
    self->unk00[3] = f4;
    return self;
}
extern "C" void func_80040AF4(CTaskGame* self) {
    self->unk100++;
    void* obj = reinterpret_cast<void*>(self->unkCC);
    if (*(u32*)((u8*)obj + 0x64) != 0) {
        self->unk78 = 1;
        extern u32 lbl_eu_80525580[3];
        u32 v0;
        u32* p = reinterpret_cast<u32*>(lbl_eu_80525580);
        v0 = p[0];
        *(u32*)((u8*)self + 0x40) = p[1];
        *(u32*)((u8*)self + 0x3C) = v0;
        *(u32*)((u8*)self + 0x44) = p[2];
    }
}
extern "C" void func_80294E58(void* self, u32 index, const u32* src);
extern const f32 lbl_eu_80665D74;

// Target us-800413bc: bump the frame counter; when the window-state gate
// (func_80042784) is clear: store fps*5 into unk78, switch the move-hook
// ptmf to pool lbl_eu_805255C8, clear the busy byte at unkCC+0x8C, push the
// lbl_eu_80665D74 constant through the vec4 setter into the unkCC object
// (index 0), then set the busy byte at unkD0+0x8C and repeat for unkD0.
// optimize_for_size: retail keeps a plain mulli for fps*5.
#pragma optimize_for_size on
extern "C" void func_80040DE4(CTaskGame* self) {
    self->unk100++;
    if (func_80042784(self) == 0) {
        self->unk78 = getTargetFramerate__9CDeviceVIFv() * 5;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_805255C8);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
        reinterpret_cast<CTaskGameFlag8C*>(self->unkCC)->field_0x8C = 0;
        func_800407C8_tmp buf1;
        func_80294E58(reinterpret_cast<void*>(self->unkCC), 0,
                      reinterpret_cast<const u32*>(func_800407C8(&buf1, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74)));
        reinterpret_cast<CTaskGameFlag8C*>(self->unkD0)->field_0x8C = 1;
        func_800407C8_tmp buf2;
        func_80294E58(reinterpret_cast<void*>(self->unkD0), 0,
                      reinterpret_cast<const u32*>(func_800407C8(&buf2, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74)));
    }
}
#pragma optimize_for_size off
// Target us-80041f80: set the move-hook ptmf from a .data pool entry. Bit
// 0x800000 of unk68 selects pool lbl_eu_8052573C, otherwise lbl_eu_80525730;
// no-op while bit 0x100 is set.
extern "C" void func_800419BC(CTaskGame* self) {
    u32 flags = self->unk68;
    if ((flags & 0x100) != 0) {
        return;
    }
    if ((flags & 0x800000) == 0) {
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525730);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_8052573C);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Retail func_80041AFC: move-hook ptmf swap gated on unk68 bits. Bit 0x100
// set: no-op. Otherwise with bit 0x01000000 clear: raise bit 0x2000 (retail
// reloads unk68 fresh before the |=) and copy pool lbl_eu_80525760; with it
// set: copy pool lbl_eu_8052576C instead.
extern "C" void func_80041AFC(CTaskGame* self) {
    u32 flags = self->unk68;
    if ((flags & 0x100) != 0) {
        return;
    }
    if ((flags & 0x01000000) == 0) {
        volatile u32* f68 = &self->unk68;
        *f68 = *f68 | 0x2000;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525760);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_8052576C);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
void CTaskGame_stub_800426A4() {}
// __declspec(noinline): retail callers (func_800424E0) call this out of
// line; without it MWCC inlines the instance/flag test into the caller.
__declspec(noinline) bool CTaskGame::func_800426F0() {
    extern CTaskGame* lbl_eu_80663D18;
    if (lbl_eu_80663D18 == nullptr) return 1;
    return lbl_eu_80663D18->unk68 & 1;
}
void CTaskGame_stub_80042710() {}
CLoad* lbl_eu_80663D1C;

int CTaskGame_checkLbl80663D1C() {
    return lbl_eu_80663D1C != 0;
}
// Retail func_80042FBC: flag gate over unk7C bits. Returns 1 when either the
// {0x80,0x40} or the {0x200,!0x100} bit-pair holds, or when bit 0x400 is set;
// only the all-clear path falls through to 0.
// Retail CTaskGame::func_80042720: request exit from the cf::CTaskGameCf
// task (when live), then switch the move-hook ptmf to pool lbl_eu_8052582C.
void CTaskGame::func_80042720() {
    unk68 |= 8;
    if (cf::CTaskGameCf::getInstance() != nullptr) {
        // Retail calls getInstance twice (no value CSE across the calls).
        cf::CTaskGameCf::getInstance()->reqExit();
    }
    u32 v0;
    u32* pool = reinterpret_cast<u32*>(lbl_eu_8052582C);
    v0 = pool[0];
    CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(this);
    words->field_0x40 = pool[1];
    words->field_0x3C = v0;
    words->field_0x44 = pool[2];
}
int func_80042FBC(CTaskGame* self) {
    int ret = 1;
    int r5 = 1;
    u32 f = self->unk7C;
    int r4 = 0;
    if ((f & 0x80) && (f & 0x40)) {
        r4 = 1;
    }
    if (r4 == 0) {
        r4 = 0;
        if ((f & 0x200) && !(f & 0x100)) {
            r4 = 1;
        }
        if (r4 == 0) {
            r5 = 0;
        }
    }
    if (r5 == 0 && !(f & 0x400)) {
        ret = 0;
    }
    return ret;
}
// retail func_80043024 = `li r3,0; blr` (returns 0); called by the
// IWorkEvent OnFileEvent adjusting thunk below
extern "C" __declspec(noinline) int func_80043024(void* self) { return 0; }
// Retail func_80043310: find the first non-busy slot in the global
// CTaskGame instance's 4-entry object registry, mark it busy, and return
// its object; null when all four slots are busy. __declspec(noinline):
// retail callers (func_80043D90 / func_80043E08) call it out of line.
extern "C" __declspec(noinline) void* func_80043310() {
    CTaskGame* game = lbl_eu_80663D18;
    CTaskGameObjSlots* slots = reinterpret_cast<CTaskGameObjSlots*>(game);
    for (u32 i = 0; i < 4; i++) {
        if (slots->busy[i] == 0) {
            slots->busy[i] = 1;
            // MWCC does not CSE the global reload (retail lwz lbl again).
            return reinterpret_cast<void*>(
                reinterpret_cast<CTaskGameObjSlots*>(lbl_eu_80663D18)->objs[i]);
        }
    }
    return nullptr;
}
// Retail func_8004335C: find the registry slot whose object matches the
// handle and clear its busy flag (two induction vars: slot byte index and
// u32 offset). Early-returns when the handle is null.
#pragma optimize_for_size on  // -O4,s keeps base+offset induction (retail add r5,r6,r4)
extern "C" void func_8004335C(void* obj) {
    if (obj == 0) return;
    CTaskGameObjSlots* slots = reinterpret_cast<CTaskGameObjSlots*>(lbl_eu_80663D18);
    for (u32 i = 0; i < 4; i++) {
        if ((void*)slots->objs[i] == obj) {
            slots->busy[i] = 0;
            return;
        }
    }
}
#pragma optimize_for_size off
bool CTaskGame_stubReturnTrue_800433A8() { return true; }
void CTaskGame::setFlag_200(bool enabled, unsigned int mode) {
    unsigned int flags = unk68;
    flags &= ~0x100u;
    if (enabled) flags |= 0x200u;
    else flags &= ~0x200u;
    if (!enabled) {
        if (mode == 1u) flags |= 0x20000u;
        else if (mode == 2u) flags |= 0x40000u;
    }
    unk68 = flags;
}
void CTaskGame::setFlag_400(int enabled, unsigned int mode, unsigned int value) {
    unsigned int flags = unk68;
    flags &= ~0x100u;
    unk68 = flags;
    if (enabled != 0) {
        flags |= 0x400u;
        unk68 = flags;
    } else {
        flags &= ~0x400u;
        unk68 = flags;
    }
    if (enabled != 0)
        return;
    if (mode == 0) {
        flags = unk68;
        flags |= 0x4000u;
        unk68 = flags;
        return;
    }
    if (mode != 5 && mode != 3)
        return;
    unkFC = value;
    flags = unk68;
    flags |= 0x8000u;
    unk68 = flags;
}
void CTaskGame_setFlag_80000(CTaskGame* this_, int arg1, int arg2, unsigned int arg3) {
    unsigned int flags = this_->unk68;
    flags &= ~0x00000100u;
    this_->unk68 = flags;
    if (arg1 != 0) {
        flags |= 0x00080000u;
        this_->unk68 = flags;
    } else {
        flags &= ~0x00080000u;
        this_->unk68 = flags;
    }
    this_->unkFC = arg3;
}
void CTaskGame_setFlag_800(CTaskGame* this_, int enabled, int unused, unsigned int value) {
    unsigned int flags = this_->unk68;
    flags &= 0xFFFFFEFFu;
    if (enabled) flags |= 0x800u;
    else flags &= 0xFFFFF7FFu;
    this_->unk68 = flags;
    this_->unkFC = value;
}
void CTaskGame::setFlag_100000(int enabled, int unused, unsigned int value) {
    unsigned int flags = unk68;
    flags &= ~0x00000100u;
    unk68 = flags;
    if (enabled != 0) {
        flags |= 0x00100000u;
        unk68 = flags;
    } else {
        flags &= ~0x00100000u;
        unk68 = flags;
    }
    unkFC = value;
}
void CTaskGame_stub_8004350C(){}
extern "C" __declspec(noinline) void func_8004350C(CTaskGame* self, int enabled) {
    unsigned int flags = self->unk68 & ~0x00000100u;
    self->unk68 = flags;
    if (enabled != 0) {
        self->unk68 = flags | 0x00800000u;
    } else {
        self->unk68 = flags & ~0x00800000u;
    }
}
// Retail func_80043564: NAND state-check callback dispatcher. When the
// CTaskGame singleton is live, tail-calls the per-mode helper with the saved
// arguments (the 4-arg helpers take (inst, b, c, a) - a is preserved across
// the b/c moves). Each case is an explicit early return so MWCC emits the
// retail tail-call `b` instead of bl+blr (cf. func_804EE60C).
extern "C" void func_80043564(u32 mode, u32 a, u32 b, u32 c) {
    CTaskGame* inst = lbl_eu_80663D18;
    if (inst == nullptr) {
        return;
    }
    if (mode == 0x10) {
        return func_800433B0(inst, b, c);
    }
    if (mode == 0x8) {
        return func_80043410(inst, b, c, a);
    }
    if (mode == 0xB) {
        return func_800434AC(inst, b, c, a);
    }
    if (mode == 0xC) {
        return func_800434DC(inst, b, c, a);
    }
    if (mode == 0x6) {
        return func_8004347C(inst, b, c, a);
    }
    if (mode == 0x9) {
        return func_8004350C(inst, b);
    }
    if (mode == 0xD) {
        return func_80043538(inst, b);
    }
}
void func_80039364__5CGameFv();

void CTaskGame_callCGameFunc() {
    func_80039364__5CGameFv();
}
extern "C" s32 func_800436A8(CTaskGame* self);
s32 CTaskGame::func_8004368C() {
    extern CTaskGame* lbl_eu_80663D18;
    if (lbl_eu_80663D18 == nullptr) {
        return 0;
    }
    return func_800436A8(lbl_eu_80663D18);
}
// Object pointed to by CTaskGame::unkF0 has a flag byte at offset 0xE9
// (func_80041F54 sets it to 1; func_80041E54 clears it).
struct CTaskGameFlagE9 {
    u8 gap0[0xE9];
    u8 flagE9;   // +0xE9
};

// Object pointed to by CTaskGame::unkF0 has a flag byte at offset 0xEA
struct CTaskGameFlag1000Object {
    u8 gap0[0xEA];
    volatile u8 flagEA;
};

void CTaskGame::setFlag_1000(int value) {
    unsigned int flags = unk68;
    if ((flags & 0x2000) != 0 && value == 1) return;
    void* object = reinterpret_cast<void*>(unkF0);
    unkF4 = static_cast<u32>(value);
    flags |= 0x1000;
    unk68 = flags;
    if (object != nullptr) {
        static_cast<CTaskGameFlag1000Object*>(object)->flagEA = 1;
    }
}
void CTaskGame_setF8(CTaskGame* obj, int val) {
    obj->unkF8 = val;
}
void CTaskGame_setFlag_1000(CTaskGame* obj) {
    obj->setFlag_1000(1);
}
int CTaskGame_checkUnkD8() {
    extern CTaskGame* lbl_eu_80663D18;
    extern int func_80459AA4__7CLibCriFv(unsigned int arg);
    if (lbl_eu_80663D18 == nullptr) return 0;
    return func_80459AA4__7CLibCriFv(static_cast<unsigned int>(lbl_eu_80663D18->unkD8));
}
bool CTaskGame_checkUnkD8NotNegOne() {
    extern CTaskGame* lbl_eu_80663D18;
    if (lbl_eu_80663D18 == nullptr) return false;
    unsigned int value = static_cast<unsigned int>(lbl_eu_80663D18->unkD8);
    return value != 0xffffffffu;
}
extern "C" u32 func_80043F18(u32* ptr) { return *ptr; }
// IScnRender::func_80043F20() = empty render-callback base impl (retail: blr)
void IScnRender::func_80043F20() {}
void Tail__8CProcessFv() {}
void __dt__9CTaskGameFv(CTaskGame*);
void CTaskGame_thunk_IWorkEvent_dtor(void *self) {
    __dt__9CTaskGameFv(static_cast<CTaskGame*>(static_cast<IWorkEvent*>(self)));
}
void cbRenderBefore__9CTaskGameFv(CTaskGame*, CScn*);
// IWorkEvent::OnFileEvent(CEventFile*) override: retail emits a this-adjusting
// thunk (subi r3,-0x54; b func_80043024) named OnFileEvent__9CTaskGameFP10CEventFile.
class CEventFile;
extern "C" void OnFileEvent__9CTaskGameFP10CEventFile(CEventFile* ev) {
    func_80043024(reinterpret_cast<CTaskGame*>(reinterpret_cast<char*>(ev) - 0x54));
}
void CTaskGame_thunk_IScnRender_cbRenderBefore(void* self, void* scene) { cbRenderBefore__9CTaskGameFv(static_cast<CTaskGame*>(static_cast<IScnRender*>(self)), static_cast<CScn*>(scene)); }
void CTaskGame_thunk_IScnRender_dtor(void* p) {
    __dt__9CTaskGameFv(static_cast<CTaskGame*>(static_cast<IScnRender*>(p)));
}
bool CTaskGame_thunk_IGameException_gameExceptionCB(void* ptr) { return CTaskGame_stubReturnTrue_800433A8(); }
void CTaskGame_thunk_IGameException_dtor(void* p) {
    __dt__9CTaskGameFv(static_cast<CTaskGame*>(static_cast<IGameException*>(p)));
}
bool CTaskGame_thunk_ITitleMenu_vfunc(void* obj) {
    CTaskGame* thiz = static_cast<CTaskGame*>(static_cast<ITitleMenu*>(obj));
    return ((bool (*)(void*))CTaskGame_setFlag_1000)(thiz);
}
void CTaskGame_thunk_ITitleMenu_dtor(void* p) {
    __dt__9CTaskGameFv(static_cast<CTaskGame*>(static_cast<ITitleMenu*>(p)));
}
bool CTaskGame_thunk_IErrMesWinSel_vfunc(void* p) {
    CTaskGame* thiz = static_cast<CTaskGame*>(static_cast<IErrMesWinSel*>(p));
    return ((bool(*)(char*))CTaskGame_setF8)(reinterpret_cast<char*>(thiz));
}
void CTaskGame_thunk_IErrMesWinSel_dtor(void* p) {
    __dt__9CTaskGameFv(static_cast<CTaskGame*>(static_cast<IErrMesWinSel*>(p)));
}

// Retail cbRenderBefore__9CTaskGameFv: IScnRender render-callback. Dispatched
// by CScn::Draw as vt+0xC(cb, scn) through the retail thunk func_80044128
// (subi r3,-0x58; b cbRenderBefore__9CTaskGameFv), so r3 = this, r4 = scene.
// Sequence:
//  1. Loading-screen state machine (D24 0..4) while Hbm is disabled.
//  2. Loading-screen draw gate (camera at idle pose, no active menus).
//  3. Battery-timer upkeep, then battery show/hide/create/delete logic.
extern "C" void cbRenderBefore__9CTaskGameFv(CTaskGame* self, CScn* scene) {
    CView::getCurrentView();   // retail bl getCurrentView__5CViewFv, result unused
    if (cf::CfGameManager::func_8007E1B4() == 0) {
        return;
    }

    if (CLibHbm::func_8045DE00() == 0) {

    // ===== Hbm disabled: loading-screen state machine =====
    if (lbl_eu_80663D1C != 0) {
        lbl_eu_80663D1C->field_29 = (lbl_eu_806649F4 == 0);
        // Pure if/else-if chain (no gotos): MWCC compiles each failed test
        // as a direct branch to the next test label, matching retail.
        if (lbl_eu_80663D24 == 0) {
            if (func_802AE6B4(lbl_eu_80663D1C) != 0) {
                lbl_eu_80663D24++;
                func_802AE6C4(lbl_eu_80663D1C);
            }
        } else if (lbl_eu_80663D24 == 1) {
            if (func_802AE6BC(lbl_eu_80663D1C) != 0) {
                if (((CTaskGameCamView*)func_8049603C(scene))->field_C < lbl_eu_80665D78) {
                    lbl_eu_80663D24++;
                    func_802AE758(lbl_eu_80663D1C);
                }
            }
        } else if (lbl_eu_80663D24 == 2) {
            if (func_802AE6BC(lbl_eu_80663D1C) != 0) {
                lbl_eu_80663D24++;
            }
        } else if (lbl_eu_80663D24 == 3) {
            func_802AE62C(lbl_eu_80663D1C);
            lbl_eu_80663D24++;
        } else if (lbl_eu_80663D24 == 4) {
            if (lbl_eu_80663D1C != 0) {
                // Virtual deleting-dtor dispatch (vt+8, flag 1); the delete
                // expansion supplies the redundant pointer test (two beq).
                delete reinterpret_cast<CLoadVtView*>(lbl_eu_80663D1C);
            }
            lbl_eu_80663D1C = 0;
        }
        self->unk8C = 0;
    } else {
        // ===== D1C == 0: frame-counter / idle-pose gate =====
        if (func_804960A8(scene) != 0) {
            if (lbl_eu_80663E24 & 0xafa40000) {
                self->unk8C = 0;
            }
            self->unk8C++;
            if (!(lbl_eu_80663E28 & 0x01000000) && !(lbl_eu_80663E24 & 0xafa40000)) {
                if ((s16)self->unk8C > 0x1e) {
                    if (((CTaskGameCamView*)func_8049603C(scene))->field_4 == ((CTaskGameCamView*)func_8049603C(scene))->field_0 &&
                        ((CTaskGameCamView*)func_8049603C(scene))->field_8 == ((CTaskGameCamView*)func_8049603C(scene))->field_4 &&
                        ((CTaskGameCamView*)func_8049603C(scene))->field_0 == lbl_eu_80665D74) {
                        lbl_eu_80663D28 = 1;
                    }
                }
            }
        } else {
            self->unk8C = 0;
        }
    }
    }
    // ===== loading-screen draw gate =====
    if (lbl_eu_80663D1C != 0 && lbl_eu_80663D24 <= 2) {
        if (CLibHbm::func_8045DE00() == 0 && cf::CfGameManager::func_800829B8() == 0) {
            if (CGame::getInstance() == 0 ||
                func_80042FBC(reinterpret_cast<CTaskGame*>(CGame::getInstance())) == 0) {
                func_802AE560(lbl_eu_80663D1C);
            }
        }
        // Camera at idle pose with non-origin x -> skip the draw.
        if (((CTaskGameCamView*)func_8049603C(scene))->field_4 == ((CTaskGameCamView*)func_8049603C(scene))->field_0 &&
            ((CTaskGameCamView*)func_8049603C(scene))->field_8 == ((CTaskGameCamView*)func_8049603C(scene))->field_4 &&
            ((CTaskGameCamView*)func_8049603C(scene))->field_0 != lbl_eu_80665D74) {
            goto L_8004316C;
        }
        if (func_801684F4() != 0) {
            goto L_8004316C;
        }
        if (func_802B0D10() != 0) {
            goto L_8004316C;
        }
        {
            u8 drawInfo[0x54];
            __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
            func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
            func_802AE5F0(lbl_eu_80663D1C, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
            lbl_eu_80663D28 = 1;
            __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
        }
    }

L_8004316C:
    // ===== battery timer upkeep =====
    if (CLibHbm::func_8045DE00() == 0) {
        if (getWiimoteBattery__Q22cf9CfPadTaskFv() == 1 && (lbl_eu_80663E24 & 0x02040000) == 0) {
            lbl_eu_80663D30++;
            if (lbl_eu_80663D30 >= getTargetFramerate__9CDeviceVIFv() * 0x258) {
                lbl_eu_80663D30 = 0;
                lbl_eu_80663D34 = getTargetFramerate__9CDeviceVIFv() * 0xa;
            }
        }
        if (lbl_eu_80663D34 > 0) {
            lbl_eu_80663D34--;
        }
        if (getWiimoteBattery__Q22cf9CfPadTaskFv() > 1) {
            lbl_eu_80663D34 = 0;
        }
    }

    // ===== battery show/hide/create/delete =====
    if (getWiimoteBattery__Q22cf9CfPadTaskFv() != 1) {
        goto L_800434CC;
    }
    if (lbl_eu_80663D28 > 0 || lbl_eu_80663D1C != 0 || lbl_eu_80663D34 != 0) {
        goto L_8004321C;
    }
    goto L_800434CC;

L_8004321C:
    {
        bool flag = false;
        if (lbl_eu_80663D1C == 0 && lbl_eu_80663D34 == 0) {
            if (func_804960A8(scene) == 0) {
                if (lbl_eu_80663D20 != 0) {
                    lbl_eu_80663D20->func_802B9364();
                }
                if (lbl_eu_80663D20 != 0) {
                    delete reinterpret_cast<CBatteryVtView*>(lbl_eu_80663D20);
                    lbl_eu_80663D20 = 0;
                }
                flag = true;
            } else {
                if (lbl_eu_80663D24 >= 3 && lbl_eu_80663D1C != 0) {
                    if (lbl_eu_80663D20 != 0) {
                        lbl_eu_80663D20->func_802B9364();
                    }
                    if (lbl_eu_80663D20 != 0) {
                        delete reinterpret_cast<CBatteryVtView*>(lbl_eu_80663D20);
                        lbl_eu_80663D20 = 0;
                    }
                    flag = true;
                }
            }
        }
        if (lbl_eu_80663D34 == 0 && lbl_eu_80663D1C == 0) {
            u32 fps3 = getTargetFramerate__9CDeviceVIFv() * 3;
            lbl_eu_80663D28++;
            if (lbl_eu_80663D28 >= fps3 || func_800FF738() != 0 || CMenuArtsSelect_isCreated() != 0) {
                lbl_eu_80663D28 = 0;
                if (lbl_eu_80663D20 != 0) {
                    lbl_eu_80663D20->func_802B9364();
                    if (lbl_eu_80663D20 != 0) {
                        delete reinterpret_cast<CBatteryVtView*>(lbl_eu_80663D20);
                        lbl_eu_80663D20 = 0;
                    }
                    flag = true;
                }
            }
        }
        if (lbl_eu_80663D20 != 0) {
            lbl_eu_80663D20->setBatteryLevel((u8)getWiimoteBattery__Q22cf9CfPadTaskFv());
            if (CLibHbm::func_8045DE00() == 0 && cf::CfGameManager::func_800829B8() == 0) {
                if (CGame::getInstance() == 0 ||
                    func_80042FBC(reinterpret_cast<CTaskGame*>(CGame::getInstance())) == 0) {
                    lbl_eu_80663D20->func_802B92FC();
                }
            }
            lbl_eu_80663D20->mLayoutReady = (lbl_eu_806649F4 == 0);
            // Camera at idle pose with non-origin x -> D34-gated draw.
            if (((CTaskGameCamView*)func_8049603C(scene))->field_4 == ((CTaskGameCamView*)func_8049603C(scene))->field_0 &&
                ((CTaskGameCamView*)func_8049603C(scene))->field_8 == ((CTaskGameCamView*)func_8049603C(scene))->field_4 &&
                ((CTaskGameCamView*)func_8049603C(scene))->field_0 != lbl_eu_80665D74) {
                goto L_80043454;
            }
            if (func_801684F4() != 0) {
                goto L_80043454;
            }
            if (func_802B0D10() != 0) {
                goto L_80043454;
            }
            goto L_80043460;
L_80043454:
            if (lbl_eu_80663D34 == 0) {
                goto L_80043514;
            }
L_80043460:
            {
                u8 drawInfo[0x54];
                __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
                func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
                lbl_eu_80663D20->func_802B9334((nw4r::lyt::DrawInfo*)&drawInfo[0]);
                __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
            }
            goto L_80043514;
        } else {
            // Create battery widget when idle pose + no delete flag this frame.
            if ((lbl_eu_80663E28 & 0x01000000) == 0 && flag == 0) {
                CBattery* battery = (CBattery*)mtl::MemManager::allocate(
                    0x28, CWorkThreadSystem::getWorkMem());
                if (battery != 0) {
                    new (battery) CBattery(0);
                }
                lbl_eu_80663D20 = battery;
                battery->func_802B92A4();
            }
            goto L_80043514;
        }
    }

L_800434CC:
    if (lbl_eu_80663D34 != 0) {
        goto L_80043514;
    }
    if (lbl_eu_80663D20 != 0) {
        lbl_eu_80663D20->func_802B9364();
        if (lbl_eu_80663D20 != 0) {
            delete reinterpret_cast<CBatteryVtView*>(lbl_eu_80663D20);
            lbl_eu_80663D20 = 0;
        }
    }

L_80043514:
    return;
}

// Retail Term: clear the object-slot pointers, run the reset helper, stop
// the unkE8 CRI stream, unregister the IScnRender subobject (+0x58) from the
// scene, virtually delete the four +0x174 slots and the loading-screen
// object, then tear down the scn/Hbm state.
// optimize_for_size: retail saves r27-r31 with a single stmw.
#pragma optimize_for_size on
void CTaskGame::Term() {
    unkCC = 0;
    unkD0 = 0;
    unkF0 = 0;
    func_80043BC4();
    if ((u32)(unkE8 + 0x10000) != 0xFFFF) {
        func_80459A7C__7CLibCriFv(unkE8);
        unkE8 = -1;
        unkEC = 0;
    }
    // removeRenderCB receiver idiom: null-this passes this(0), else this+0x58.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != nullptr) {
        render = reinterpret_cast<IScnRender*>(reinterpret_cast<char*>(this) + 0x58);
    }
    removeRenderCB__4CScnFP10IScnRender(unk74, render);
    __dt__8009D72C();
    // Dual induction mirrors retail: slot byte counter and u32 offset.
    u32 i = 0;
    for (; i < 4; i++) {
        CTaskGameSlotObj* obj = unk174[i];
        if (obj != nullptr) {
            // Virtual deleting-dtor dispatch (vt+8, flag 1); the delete
            // expansion supplies the redundant pointer test (two beq).
            delete obj;
            unk174[i] = nullptr;
        }
    }
    if (lbl_eu_80663D1C != nullptr) {
        func_802AE62C(lbl_eu_80663D1C);
        if (lbl_eu_80663D1C != nullptr) {
            delete reinterpret_cast<CLoadVtView*>(lbl_eu_80663D1C);
            lbl_eu_80663D1C = nullptr;
        }
    }
    __dt__8047BFFC();
    func_8047D028__17UnkClass_8047CD0CFv();
    CLibHbm::func_8045D470(false);
}
#pragma optimize_for_size off
// Retail func_80040A3C: set the title/loading text pair (unk86/unk88), copy
// the caption string (or the default lbl_eu_804FA890[0x6D] when null) into
// the unkA4 FixStr<32> (strlen+strcpy via the inline operator=), then raise
// the 0x2 unk68 flag, set the 0x128 request code, and when the unkD4 object
// is live, poke its +0x60 flag word. optimize_for_size: retail saves r29-r31
// with a single stmw (MWCC merges the callee-saved saves only under size
// optimization).
#pragma optimize_for_size on
__declspec(noinline) void CTaskGame::func_80040A3C(u16 r4, u16 r5, const char* r6, s16 r7) {
    unk86 = r4;
    unk88 = r5;
    if (r6 != nullptr) {
        unkA4 = r6;
    } else {
        // Hold the default caption in a local: retail materializes the full
        // &lbl_eu_804FA890[0x6D] address in r31 once and copies it with mr
        // for strlen/strcpy (an inline expression makes MWCC recompute the
        // +0x6D addi per call).
        const char* s = &lbl_eu_804FA890[0x6D];
        unkA4 = s;
    }
    unk8A = r7;
    unk68 |= 2;
    unk128 = 3;
    if (unkD4 != nullptr) {
        func_802956A8(reinterpret_cast<void*>(unkD4));
        reinterpret_cast<CTaskGameUnkD4Obj*>(unkD4)->field_0x60 &= ~2;
    }
}
#pragma optimize_for_size off

// ml::FixStr<32>::format (retail standalone symbol, referenced from other
// TUs): vsnprintf into a 0x100 stack buffer, then copy into the fixed string
// (mString at +0, mLength at +0x20). Same shape as the FixStr<128> version in
// CfScript.cpp. optimize_for_size: retail saves r29-r31 with a single stmw.
#pragma optimize_for_size on
template <>
void ml::FixStr<32>::format(const char* fmt, ...) {
    char buffer[0x100];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    mLength = (int)std::strlen(buffer);
    std::strcpy(mString, buffer);
}
#pragma optimize_for_size off
// Target us-80041110: per-frame tick. Decrement the unk78 frame budget and
// bump unk100; while the budget is exhausted: mark the unkCC object busy
// (+0x8C = 1), push the float constants through the vec4 setter into the
// unkCC/unk74 objects (the second pass halves the target frame rate), then
// switch the move-hook ptmf to pool lbl_eu_8052558C.
void func_80040B38(CTaskGame* self) {
    self->unk100++;
    self->unk78--;
    if ((s32)self->unk78 > 0) {
        return;
    }
    reinterpret_cast<CTaskGameFlag8C*>(self->unkCC)->field_0x8C = 1;
    func_800407C8_tmp buf1;
    func_80294E58(reinterpret_cast<void*>(self->unkCC), 0,
                  reinterpret_cast<const u32*>(func_800407C8(&buf1, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D6C)));
    func_800407C8_tmp buf2;
    func_80294E58(reinterpret_cast<void*>(self->unkCC),
                  getTargetFramerate__9CDeviceVIFv() >> 1,
                  reinterpret_cast<const u32*>(func_800407C8(&buf2, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74)));
    func_800407C8_tmp buf3;
    func_8049602C(self->unk74, 0, func_800407C8(&buf3, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74));
    u32 v0;
    u32* pool = reinterpret_cast<u32*>(lbl_eu_8052558C);
    v0 = pool[0];
    CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
    words->field_0x40 = pool[1];
    words->field_0x3C = v0;
    words->field_0x44 = pool[2];
}
// Target us-80041204: bump the frame counter, then when the window-state
// gate (func_80042784) is clear: store fps*5 into unk78, switch the
// move-hook ptmf to pool lbl_eu_80525598, clear the busy byte at unkD0+0x8C,
// then depending on the lbl_eu_80663D2C flag either switch the move-hook
// ptmf to pool lbl_eu_805255A4 or set the busy byte at unkCC+0x8C.
// optimize_for_size: retail keeps a plain mulli for fps*5 (no rlwinm+add
// strength reduction) and the stw/scheduling follow the size-optimized
// shape.
#pragma optimize_for_size on
void func_80040C2C(CTaskGame* self) {
    self->unk100++;
    if (func_80042784(self) == 0) {
        self->unk78 = getTargetFramerate__9CDeviceVIFv() * 5;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525598);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
        reinterpret_cast<CTaskGameFlag8C*>(self->unkD0)->field_0x8C = 0;
        if (lbl_eu_80663D2C != 0) {
            u32 v1;
            u32* pool2 = reinterpret_cast<u32*>(lbl_eu_805255A4);
            v1 = pool2[0];
            CTaskGamePtmfWords* words2 = reinterpret_cast<CTaskGamePtmfWords*>(self);
            words2->field_0x40 = pool2[1];
            words2->field_0x3C = v1;
            words2->field_0x44 = pool2[2];
        } else {
            reinterpret_cast<CTaskGameFlag8C*>(self->unkCC)->field_0x8C = 1;
        }
    }
}
#pragma optimize_for_size off
// Target us-800412b0: bump the frame counter; when the window-state gate
// (func_80042784) is clear, tick the unk78 frame budget. While the budget
// lasts (below fps*4 - fps/2) and the func_8004362C gate opens: halve the
// budget, push the float constants through the vec4 setter into the unkCC
// object (index framerate/2), run the func_8004302C reset pair, and switch
// the move-hook ptmf to pool lbl_eu_805255B0. When the budget hits zero the
// move-hook switches to pool lbl_eu_805255BC instead.
void func_80040CD8(CTaskGame* self) {
    self->unk100++;
    if (func_80042784(self) != 0) {
        return;
    }
    self->unk78--;
    if ((s32)self->unk78 > 0) {
        if ((u32)self->unk78 <= getTargetFramerate__9CDeviceVIFv() * 4 - (getTargetFramerate__9CDeviceVIFv() >> 1)) {
            if (func_8004362C(self) != 0) {
                self->unk78 = getTargetFramerate__9CDeviceVIFv() >> 1;
                func_800407C8_tmp buf;
                func_80294E58(reinterpret_cast<void*>(self->unkCC),
                              getTargetFramerate__9CDeviceVIFv() >> 1,
                              reinterpret_cast<const u32*>(func_800407C8(&buf, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D6C)));
                func_8004302C(1, 1);
                u32 v0;
                u32* pool = reinterpret_cast<u32*>(lbl_eu_805255B0);
                v0 = pool[0];
                CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
                words->field_0x40 = pool[1];
                words->field_0x3C = v0;
                words->field_0x44 = pool[2];
            }
        }
    } else {
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_805255BC);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
void CTaskGame_stub_80040DE4(){}
// Target us-8004148c: bump the frame counter; when the window-state gate
// (func_80042784) is clear, tick the unk78 frame budget. While the budget
// lasts and the func_8004362C gate opens: halve the budget, push the float
// constants through the vec4 setter into the unkD0 object (index
// framerate/2), run the func_8004302C reset pair, and switch the move-hook
// ptmf to pool lbl_eu_80525568+0x6C. When the budget runs out, bump the
// unk7C retry counter: at >= 2 the same vec4/reset sequence runs with pool
// lbl_eu_80525568+0x78, otherwise the move-hook switches to pool
// lbl_eu_80525568+0x84. The pool base is held in one register (retail
// materializes &lbl_eu_80525568 once before the gate).
// optimize_for_size: retail saves r29-r31 with the _savegpr_29 helper (MWCC
// -O4,s emits _savegpr already at 3 saved regs; plain -O4 would use
// individual stw's).
#pragma optimize_for_size on
void func_80040EB4(CTaskGame* self) {
    self->unk100++;
    u32* base = lbl_eu_80525568;
    if (func_80042784(self) != 0) {
        return;
    }
    self->unk78--;
    if ((s32)self->unk78 > 0) {
        if (func_8004362C(self) != 0) {
            self->unk78 = getTargetFramerate__9CDeviceVIFv() >> 1;
            func_800407C8_tmp buf1;
            func_80294E58(reinterpret_cast<void*>(self->unkD0),
                          getTargetFramerate__9CDeviceVIFv() >> 1,
                          reinterpret_cast<const u32*>(func_800407C8(&buf1, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D6C)));
            func_8004302C(1, 1);
            u32* pool1 = &base[0x6C / 4];
            u32 v1 = base[0x6C / 4];
            CTaskGamePtmfWords* words1 = reinterpret_cast<CTaskGamePtmfWords*>(self);
            words1->field_0x40 = pool1[1];
            words1->field_0x3C = v1;
            words1->field_0x44 = pool1[2];
        }
    } else {
        self->unk7C++;
        if ((s32)self->unk7C >= 2) {
            self->unk78 = getTargetFramerate__9CDeviceVIFv() >> 1;
            func_800407C8_tmp buf2;
            func_80294E58(reinterpret_cast<void*>(self->unkD0),
                          getTargetFramerate__9CDeviceVIFv() >> 1,
                          reinterpret_cast<const u32*>(func_800407C8(&buf2, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D6C)));
            func_8004302C(1, 1);
            u32* pool2 = &base[0x78 / 4];
            u32 v2 = base[0x78 / 4];
            CTaskGamePtmfWords* words2 = reinterpret_cast<CTaskGamePtmfWords*>(self);
            words2->field_0x40 = pool2[1];
            words2->field_0x3C = v2;
            words2->field_0x44 = pool2[2];
        } else {
            u32* pool3 = &base[0x84 / 4];
            u32 v3 = base[0x84 / 4];
            CTaskGamePtmfWords* words3 = reinterpret_cast<CTaskGamePtmfWords*>(self);
            words3->field_0x40 = pool3[1];
            words3->field_0x3C = v3;
            words3->field_0x44 = pool3[2];
        }
    }
}
#pragma optimize_for_size off
// Target us-800415fc: per-frame tick with a delayed scene-teardown. Bump the
// frame counter; when the window-state gate (func_80042784) is clear, tick
// the unk78 frame budget. While the budget is exhausted: push the float
// constants through the vec4 setter into the unkCC/unkD0 objects (index 0)
// and the scene (func_8049602C, index 0), clear the busy bytes at +0x39 and
// null the unkCC/unkD0 objects, and once the frame counter passes fps*8
// switch the move-hook ptmf to pool lbl_eu_805255F8 (lbl_eu_80663D2C set) or
// lbl_eu_80525604.
void func_80041024(CTaskGame* self) {
    self->unk100++;
    if (func_80042784(self) != 0) {
        return;
    }
    self->unk78--;
    if ((s32)self->unk78 > 0) {
        return;
    }
    if (self->unkCC != 0) {
        func_800407C8_tmp buf1;
        func_80294E58(reinterpret_cast<void*>(self->unkCC), 0,
                      reinterpret_cast<const u32*>(func_800407C8(&buf1, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D6C)));
    }
    if (self->unkD0 != 0) {
        func_800407C8_tmp buf2;
        func_80294E58(reinterpret_cast<void*>(self->unkD0), 0,
                      reinterpret_cast<const u32*>(func_800407C8(&buf2, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D6C)));
    }
    func_800407C8_tmp buf3;
    func_8049602C(self->unk74, 0, func_800407C8(&buf3, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D6C));
    if (self->unkCC != 0) {
        reinterpret_cast<CTaskGameFlag39*>(self->unkCC)->field_0x39 = 1;
        self->unkCC = 0;
    }
    if (self->unkD0 != 0) {
        reinterpret_cast<CTaskGameFlag39*>(self->unkD0)->field_0x39 = 1;
        self->unkD0 = 0;
    }
    if (cf::CTaskGameCf::getInstance() != 0) {
        if (self->unk100 >= getTargetFramerate__9CDeviceVIFv() * 8) {
            if (lbl_eu_80663D2C != 0) {
                u32 v0;
                u32* pool = reinterpret_cast<u32*>(lbl_eu_805255F8);
                v0 = pool[0];
                CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
                words->field_0x40 = pool[1];
                words->field_0x3C = v0;
                words->field_0x44 = pool[2];
            } else {
                u32 v1;
                u32* pool2 = reinterpret_cast<u32*>(lbl_eu_80525604);
                v1 = pool2[0];
                CTaskGamePtmfWords* words2 = reinterpret_cast<CTaskGamePtmfWords*>(self);
                words2->field_0x40 = pool2[1];
                words2->field_0x3C = v1;
                words2->field_0x44 = pool2[2];
            }
        }
    }
}
// Target us-8004177c: save/load invalid-reset sequence. Marks save/load
// reset-invalid, toggles the Hbm state off/on, resets the NAND state
// (func_8023FD4C), clears/sets unk68 flag bits ((unk68 | 0x200000) &
// 0xFE6030FF), then asks func_8023FC18 whether the reset path applies and
// switches the move-hook ptmf to pool lbl_eu_80525610 (bit 0x100 set) or
// lbl_eu_8052561C (bit 0x2000 set).
void func_800411A4(CTaskGame* self) {
    CWorkSystem::setSaveLoadInvalidReset(true);
    CLibHbm::func_8045D5C8(false);
    CLibHbm::func_8045D470(true);
    func_eu_804521BC(0);
    self->unk68 = (self->unk68 | 0x200000) & 0xFE6030FF;
    func_8023FD4C(0);
    if (func_8023FC18(&func_80043564) != 0) {
        self->unk68 |= 0x100;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525610);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        self->unk68 |= 0x2000;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_8052561C);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Target us-8004184c: move-hook / error-window selection. Bit 0x100 of unk68
// set: no-op. Bit 0x400 set: switch the move-hook ptmf to pool
// lbl_eu_80525568+0xD8. Otherwise with bit 0x8000 set: set bits 0x12000,
// pick the factory code (0x60 for unkFC==2, 0x61 for ==3, else 0x5F), create
// the error-message window (__ct__802B4DF4 with the scene pointer + code/4),
// mark save/load reset-invalid, and switch the move-hook ptmf to pool
// lbl_eu_80525568+0xC0. Otherwise: switch the move-hook ptmf to pool
// lbl_eu_80525568+0xCC.
// optimize_for_size: retail saves r30-r31 with a single stmw (MWCC merges
// the callee-saved saves only under size optimization).
#pragma optimize_for_size on
void func_8004125C(CTaskGame* self) {
    // Pool entries are whole 12-byte move-hook ptmf elements of the region
    // behind lbl_eu_80525568 (offsets 0xC0/0xCC/0xD8 = indices 16/17/18);
    // base is materialized once in r31 (retail shape).
    char* base = reinterpret_cast<char*>(lbl_eu_80525568);
    // One load shared by the three flag tests; the big block re-reads
    // unk68 fresh below (retail shape).
    u32 flags = self->unk68;
    if ((flags & 0x100) != 0) {
        return;
    }
    if ((flags & 0x400) == 0) {
        if ((flags & 0x8000) != 0) {
            // Volatile stops MWCC's value CSE from reusing the flag-test
            // register for the store (retail reloads unk68 fresh here).
            volatile u32* f68 = &self->unk68;
            *f68 = *f68 | 0x12000;
            int code = 0x5F;
            u32 fc = self->unkFC;
            if ((s32)fc == 2) {
                code = 0x60;
            } else if ((s32)fc == 3) {
                code = 0x61;
            }
            __ct__802B4DF4(self, static_cast<u32>(reinterpret_cast<uintptr_t>(self->unk74)), code, 4);
            CWorkSystem::setSaveLoadInvalidReset(false);
            self->setMoveFunc(reinterpret_cast<CTTask<CTaskGame>::MoveFunc*>(base)[16]);
            return;
        }
        self->setMoveFunc(reinterpret_cast<CTTask<CTaskGame>::MoveFunc*>(base)[17]);
        return;
    }
    self->setMoveFunc(reinterpret_cast<CTTask<CTaskGame>::MoveFunc*>(base)[18]);
}
#pragma optimize_for_size off
// Retail func_80041348: when no error-message window is active
// (lbl_eu_80664C28 == 0), mark save/load reset-invalid, clear the
// 0x2000/0x8000/0x10000 unk68 flags, switch the move-hook ptmf to pool
// lbl_eu_8052564C, and reset unkF8.
void func_80041348(CTaskGame* self) {
    if (lbl_eu_80664C28 == 0) {
        CWorkSystem::setSaveLoadInvalidReset(true);
        self->unk68 &= 0xFFFE5FFF;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_8052564C);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
        self->unkF8 = -1;
    }
}
// Target us-800419b4: clear the 0x100/0x800000/0x1000000 unk68 flag bits,
// ask the NAND state check (func_8023F2F4) with (unkFC, func_80043564
// callback) whether the reset path applies, then switch the move-hook ptmf
// to pool lbl_eu_80525658 (bit 0x100 set) or lbl_eu_80525664 (bit 0x2000
// set).
void func_80041390(CTaskGame* self) {
    self->unk68 &= 0xFFF7FEFF;
    if (func_8023F2F4(self->unkFC, &func_80043564) != 0) {
        self->unk68 |= 0x100;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525658);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        self->unk68 |= 0x2000;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525664);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Target us-80041a5c: error-window / move-hook setup. When bit 0x100 of unk68
// is clear: if bit 0x80000 is set, switch the move-hook ptmf to pool
// lbl_eu_8052567C (retail places this short block out-of-line at the end, so
// the else comes first); otherwise set bit 0x2000, create the error-message
// window (factory ctor __ct__802B4DF4 with the scene pointer + 0x56/3), mark
// save/load reset-invalid, and switch the move-hook ptmf to pool
// lbl_eu_80525670.
void func_80041448(CTaskGame* self) {
    if ((self->unk68 & 0x100) != 0) {
        return;
    }
    if ((self->unk68 & 0x80000) == 0) {
        // Error-window creation block stays inline; retail sinks the shorter
        // pool swap out of line at the end (bne from the gate).
        // The aliasing view forces a fresh unk68 load (retail frees r4 - the
        // cached flags word - for the unk74 argument) and lets the store sink
        // below the argument setup, matching retail scheduling.
        reinterpret_cast<CTaskGameFlags68*>(self)->flags |= 0x2000;
        __ct__802B4DF4(self, static_cast<u32>(reinterpret_cast<uintptr_t>(self->unk74)), 0x56, 3);
        CWorkSystem::setSaveLoadInvalidReset(false);
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525670);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_8052567C);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// func_8023F860 is declared void in CSaveLoad.hpp (call sites discard the
// result); retail func_800414F4 branches on the return value, so declare the
// caller's shape here (same scheme as the CLibCri caller-shape imports).
extern "C" int func_8023F860(u32 value, void (*cb)(u32, u32, u32, u32));
// Target us-80041af8: clear the 0x100/0x100000/0x1000000 unk68 flag bits,
// ask the NAND state check (func_8023F860) with (unkFC, func_80043564
// callback) whether the reset path applies, then switch the move-hook ptmf
// to pool lbl_eu_80525688 (bit 0x100 set) or lbl_eu_80525694 (bit 0x2000
// set).
void func_800414F4(CTaskGame* self) {
    self->unk68 &= 0xFFEFFEFF;
    if (func_8023F860(self->unkFC, &func_80043564) != 0) {
        self->unk68 |= 0x100;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525688);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        self->unk68 |= 0x2000;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525694);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Target us-80041ba0: error-window / move-hook setup (func_80041448 variant).
// When bit 0x100 of unk68 is clear: if bit 0x100000 is clear, set bit 0x2000,
// create the error-message window (factory ctor __ct__802B4DF4 with the scene
// pointer + 0x56/3), mark save/load reset-invalid, and switch the move-hook
// ptmf to pool lbl_eu_805256A0; otherwise just switch the move-hook ptmf to
// pool lbl_eu_805256AC (the big block is the fall-through, the short swap is
// sunk out-of-line at the end).
void func_800415AC(CTaskGame* self) {
    if ((self->unk68 & 0x100) != 0) {
        return;
    }
    if ((self->unk68 & 0x100000) == 0) {
        u32 scene = static_cast<u32>(reinterpret_cast<uintptr_t>(self->unk74));
        // Retail reloads unk68 fresh before the |=.
        volatile u32* f68 = &self->unk68;
        *f68 = *f68 | 0x2000;
        __ct__802B4DF4(self, scene, 0x56, 3);
        CWorkSystem::setSaveLoadInvalidReset(false);
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_805256A0);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_805256AC);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Target us-80041c3c: clear the 0x60300 unk68 flag bits (0x100/0x200/
// 0x20000/0x40000), ask the NAND state check (func_8023FBA0) with the
// func_80043564 callback whether the reset path applies, then switch the
// move-hook ptmf to pool lbl_eu_805256B8 (bit 0x100 set) or lbl_eu_805256C4
// (bit 0x2000 set).
void func_80041658(CTaskGame* self) {
    self->unk68 &= 0xFFF9FCFF;
    if (func_8023FBA0(&func_80043564) != 0) {
        self->unk68 |= 0x100;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_805256B8);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        self->unk68 |= 0x2000;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_805256C4);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Retail func_800416FC: error-window / move-hook setup gated on the 0x100 /
// 0x200 unk68 bits. Bit 0x100 set: no-op. Bit 0x200 set: switch the
// move-hook ptmf to pool lbl_eu_805256DC (retail sinks this short block
// out-of-line at the end, so the else comes first). Otherwise set bit
// 0x2000, create the error-message window (factory ctor __ct__802B4DF4 with
// the scene pointer + 0x54 or 0x55/3 depending on the 0x20000 bit), mark
// save/load reset-invalid, and switch the move-hook ptmf to pool
// lbl_eu_805256D0.
void func_800416FC(CTaskGame* self) {
    if ((self->unk68 & 0x100) != 0) {
        return;
    }
    if ((self->unk68 & 0x200) == 0) {
        // Retail reloads unk68 fresh before the |=.
        volatile u32* f68 = &self->unk68;
        *f68 = *f68 | 0x2000;
        if ((self->unk68 & 0x20000) != 0) {
            __ct__802B4DF4(self, static_cast<u32>(reinterpret_cast<uintptr_t>(self->unk74)), 0x54, 3);
            CWorkSystem::setSaveLoadInvalidReset(false);
        } else {
            __ct__802B4DF4(self, static_cast<u32>(reinterpret_cast<uintptr_t>(self->unk74)), 0x55, 3);
            CWorkSystem::setSaveLoadInvalidReset(false);
        }
        u32 v1;
        u32* pool2 = reinterpret_cast<u32*>(lbl_eu_805256D0);
        v1 = pool2[0];
        CTaskGamePtmfWords* words2 = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words2->field_0x40 = pool2[1];
        words2->field_0x3C = v1;
        words2->field_0x44 = pool2[2];
    } else {
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_805256DC);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Target us-80041da0: clear the 0x901 unk68 flag bits (0x1/0x100/0x800), ask
// the NAND state check (func_8023F690) with the func_80043564 callback
// whether the reset path applies, then switch the move-hook ptmf to pool
// lbl_eu_805256E8 (bit 0x100 set) or lbl_eu_805256F4 (bit 0x2000 set).
void func_800417AC(CTaskGame* self) {
    self->unk68 &= 0xFFFFF6FF;
    if (func_8023F690(&func_80043564) != 0) {
        self->unk68 |= 0x100;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_805256E8);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        self->unk68 |= 0x2000;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_805256F4);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Target us-80041e40: error-window / move-hook setup (func_80041448 variant
// gated on bit 0x800). When bit 0x100 of unk68 is clear: if bit 0x800 is
// clear, set bit 0x2000, create the error-message window (factory ctor
// __ct__802B4DF4 with the scene pointer + 0x56/3), mark save/load
// reset-invalid, and switch the move-hook ptmf to pool lbl_eu_80525700;
// otherwise just switch the move-hook ptmf to pool lbl_eu_8052570C (big block
// fall-through, short swap sunk out-of-line at the end).
void func_8004185C(CTaskGame* self) {
    if ((self->unk68 & 0x100) != 0) {
        return;
    }
    if ((self->unk68 & 0x800) == 0) {
        u32 scene = static_cast<u32>(reinterpret_cast<uintptr_t>(self->unk74));
        // Retail reloads unk68 fresh before the |=.
        volatile u32* f68 = &self->unk68;
        *f68 = *f68 | 0x2000;
        __ct__802B4DF4(self, scene, 0x56, 3);
        CWorkSystem::setSaveLoadInvalidReset(false);
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525700);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_8052570C);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Target us-80041edc: clear the 0x800100 unk68 flag bits (0x100/0x800000),
// ask the NAND state check (func_8023FCCC) with the func_80043564 callback
// whether the reset path applies, then switch the move-hook ptmf to pool
// lbl_eu_80525718 (bit 0x100 set) or lbl_eu_80525724 (bit 0x2000 set).
void func_80041908(CTaskGame* self) {
    self->unk68 &= 0xFF7FFEFF;
    if (func_8023FCCC(&func_80043564) != 0) {
        self->unk68 |= 0x100;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525718);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        self->unk68 |= 0x2000;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525724);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Target us-80041fd4: clear the 0x100/0x1000000 unk68 flag bits, ask the
// NAND state check (func_8023F5CC) with the func_80043564 callback whether
// the reset path applies, then switch the move-hook ptmf to pool
// lbl_eu_80525748 (bit 0x100 set) or lbl_eu_80525754 (bit 0x2000 set).
void func_80041A48(CTaskGame* self) {
    self->unk68 &= 0xFEFFFEFF;
    if (func_8023F5CC(&func_80043564) != 0) {
        self->unk68 |= 0x100;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525748);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        self->unk68 |= 0x2000;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525754);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Target us-800420d8: when no error-message window is active
// (lbl_eu_80664C28 == 0), mark save/load reset-invalid and switch the
// move-hook ptmf to pool lbl_eu_80525778.
extern "C" void func_80041B94(CTaskGame* self) {
    if (lbl_eu_80664C28 == 0) {
        CWorkSystem::setSaveLoadInvalidReset(true);
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525778);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Target us-80042130: title-menu / move-hook setup. Marks save/load
// reset-invalid, toggles the Hbm state off, sets the sound-state flag, clears
// unk68 bit 0x200000 and raises the lbl_eu_80663D2C flag. When the
// window-state gate (func_80042784) is clear: clear the +0x39 busy bytes and
// null the unkCC/unkD0 objects, run the func_80042874 reset, poke the unkF0
// object's +0x39 byte, create the title menu (__ct__CMenuTitle with the
// +0x60 ITitleMenu subobject via a null-checked adjusted cast), poke the
// unkD4 event object, reset unk78, and switch the move-hook ptmf to pool
// lbl_eu_80525784.
// optimize_for_size: retail saves r30-r31 with a single stmw (MWCC merges
// the callee-saved saves only under size optimization).
#pragma optimize_for_size on
void func_80041BC0(CTaskGame* self) {
    CWorkSystem::setSaveLoadInvalidReset(false);
    CLibHbm::func_8045D470(false);
    CLibHbm::func_8045D5C8(false);
    func_eu_804521BC(1);
    self->unk68 &= 0xFFDFFFFF;
    lbl_eu_80663D2C = 1;
    if (func_80042784(self) == 0) {
        if (self->unkCC != 0) {
            reinterpret_cast<CTaskGameFlag39*>(self->unkCC)->field_0x39 = 1;
            self->unkCC = 0;
        }
        if (self->unkD0 != 0) {
            reinterpret_cast<CTaskGameFlag39*>(self->unkD0)->field_0x39 = 1;
            self->unkD0 = 0;
        }
        func_80042874();
        if (self->unkF0 != 0) {
            reinterpret_cast<CTaskGameFlag39*>(self->unkF0)->field_0x39 = 1;
        }
        self->unkF0 = reinterpret_cast<u32>(__ct__CMenuTitle(self, self->unk74, static_cast<ITitleMenu*>(self)));
        if (self->unkD4 != 0) {
            func_802956A4(reinterpret_cast<void*>(self->unkD4));
        }
        self->unk78 = 0;
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525784);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
#pragma optimize_for_size off
// Target us-80042240: title-screen tick. Raises unk68 bit 0x400000, then
// when the window-state gate (func_80042784) is clear and the title
// captions are empty and the play-time gate (func_80043D68) is closed but
// the script-time gate (func_801684F4) opens: start streaming the
// lbl_eu_804FA890[0x6E] caption with the scene's alloc handle
// (func_80043738). Pokes the unkD4 event object; then when unk68 bit 0x1000
// is set: clear bit 0x800, and by unkF4 mode clear the unkF0 object's +0x39
// busy byte (0), run the func_80134C34 / func_80134E50 pair and store the
// scene's func_80496110 byte into unk188 (1/2), then push the float
// constants through the vec4 setter into the scene at framerate/2 and switch
// the move-hook ptmf to pool lbl_eu_80525790. Otherwise clear the unkF0
// object's +0xEA byte.
void func_80041CC8(CTaskGame* self) {
    self->unk68 |= 0x400000;
    if (func_80042784(self) == 0) {
        if (self->unk86 == 0 && func_80043D68() == 0 && func_801684F4() != 0) {
            func_80043738(0, &lbl_eu_804FA890[0x6E], func_80495FF0(self->unk74), 0, 1, 0, lbl_eu_80665D6C);
        }
        if (self->unkD4 != 0) {
            func_802956A4(reinterpret_cast<void*>(self->unkD4));
        }
        if ((self->unk68 & 0x1000) != 0) {
            // Retail loads unkF4 first and clears the tested 0x1000 bit
            // (rlwinm 20,18) with a FRESH unk68 read (retail re-loads r0,
            // 0x68 instead of reusing the test value); the volatile read
            // stops MWCC's value CSE (same shape as func_8004125C).
            s32 f4 = static_cast<s32>(self->unkF4);
            volatile u32* f68 = &self->unk68;
            *f68 = *f68 & ~0x1000;
            if (f4 == 0) {
                if (self->unkF0 != 0) {
                    reinterpret_cast<CTaskGameFlag39*>(self->unkF0)->field_0x39 = 1;
                    self->unkF0 = 0;
                }
            } else if (f4 == 1) {
                func_80134C34();
                self->unk188 = static_cast<u8>(func_80496110(self->unk74));
            } else if (f4 == 2) {
                func_80134E50(0);
                self->unk188 = static_cast<u8>(func_80496110(self->unk74));
            }
            func_800407C8_tmp buf;
            func_8049602C(self->unk74, getTargetFramerate__9CDeviceVIFv() >> 1,
                          func_800407C8(&buf, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D6C));
            u32 v0;
            u32* pool = reinterpret_cast<u32*>(lbl_eu_80525790);
            v0 = pool[0];
            CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
            words->field_0x40 = pool[1];
            words->field_0x3C = v0;
            words->field_0x44 = pool[2];
        } else {
            if (self->unkF0 != 0) {
                reinterpret_cast<CTaskGameFlag1000Object*>(self->unkF0)->flagEA = 0;
            }
        }
    }
}
// Target us-800423cc: scene gate then move-hook switch by unkF4 mode. When
// the scene current-process query (func_80496034) returns non-null: for
// unkF4==0 show the default loading caption via func_80040A3C, for
// unkF4==1/2 clear the unkF0 object's +0xE9 byte; each case copies a
// different move-hook ptmf pool (+0x234 / +0x240 / +0x24C), held in one
// base register.
// optimize_for_size: retail saves r30-r31 with a single stmw (MWCC merges
// the callee-saved saves only under size optimization).
#pragma optimize_for_size on
void func_80041E54(CTaskGame* self) {
    // Scene read first so the self spill precedes the base def (retail
    // allocation: self -> r31, pool base -> r30); the scheduler hoists the
    // base lis/addi above the load.
    CScnNw4r* scene = self->unk74;
    u32* base = lbl_eu_80525568;
    if (func_80496034(scene) != 0) {
        if (self->unkF4 == 0) {
            self->func_80040A3C(0, 0, &lbl_eu_804FA890[0x6D], 0);
            CTaskGamePtmfPool* pool = reinterpret_cast<CTaskGamePtmfPool*>(reinterpret_cast<char*>(base) + 0x234);
            u32 v0 = pool->w0;
            CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
            words->field_0x40 = pool->w1;
            words->field_0x3C = v0;
            words->field_0x44 = pool->w2;
        } else if (self->unkF4 == 1) {
            if (self->unkF0 != 0) {
                reinterpret_cast<CTaskGameFlagE9*>(self->unkF0)->flagE9 = 0;
            }
            CTaskGamePtmfPool* pool = reinterpret_cast<CTaskGamePtmfPool*>(reinterpret_cast<char*>(base) + 0x240);
            u32 v0 = pool->w0;
            CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
            words->field_0x40 = pool->w1;
            words->field_0x3C = v0;
            words->field_0x44 = pool->w2;
        } else if (self->unkF4 == 2) {
            if (self->unkF0 != 0) {
                reinterpret_cast<CTaskGameFlagE9*>(self->unkF0)->flagE9 = 0;
            }
            CTaskGamePtmfPool* pool = reinterpret_cast<CTaskGamePtmfPool*>(reinterpret_cast<char*>(base) + 0x24C);
            u32 v0 = pool->w0;
            CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
            words->field_0x40 = pool->w1;
            words->field_0x3C = v0;
            words->field_0x44 = pool->w2;
        }
    }
}
#pragma optimize_for_size off
void CTaskGame_stub_80041F54(){}
// Target us-800424cc: window/save-menu gates then move-hook switch. When the
// window-state gate (func_80042784) and the save-menu gate (func_8028E440)
// are both clear: switch the move-hook ptmf to pool lbl_eu_805257C0, then
// depending on the unk188 flag run the scene empty-per-frame stub with a vec4
// (trailing lbl_eu_80665D6C constant when unk188 is set, else all
// lbl_eu_80665D74) and arg1 0 or framerate/2, clear unk188, set the unkF0
// object's +0xE9 byte, and clear unk68 bit 0x1000.
void func_80041F54(CTaskGame* self) {
    if (func_80042784(self) != 0) {
        return;
    }
    if (func_8028E440() != 0) {
        return;
    }
    u32 v0;
    u32* pool = reinterpret_cast<u32*>(lbl_eu_805257C0);
    v0 = pool[0];
    CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
    words->field_0x40 = pool[1];
    words->field_0x3C = v0;
    words->field_0x44 = pool[2];
    if (self->unk188 != 0) {
        func_800407C8_tmp buf;
        func_8049602C(self->unk74, 0,
                      func_800407C8(&buf, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D6C));
    } else {
        func_800407C8_tmp buf2;
        func_8049602C(self->unk74, getTargetFramerate__9CDeviceVIFv() >> 1,
                      func_800407C8(&buf2, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74));
    }
    self->unk188 = 0;
    if (self->unkF0 != 0) {
        reinterpret_cast<CTaskGameFlagE9*>(self->unkF0)->flagE9 = 1;
    }
    self->unk68 &= ~0x1000;
}
// Target us-800425c0: window/option-menu gates then move-hook switch. Same
// shape as func_80041F54 but gated on func_8029BBA0 and pool
// lbl_eu_805257CC.
void func_80042048(CTaskGame* self) {
    if (func_80042784(self) != 0) {
        return;
    }
    if (func_8029BBA0() != 0) {
        return;
    }
    u32 v0;
    u32* pool = reinterpret_cast<u32*>(lbl_eu_805257CC);
    v0 = pool[0];
    CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
    words->field_0x40 = pool[1];
    words->field_0x3C = v0;
    words->field_0x44 = pool[2];
    if (self->unk188 != 0) {
        func_800407C8_tmp buf;
        func_8049602C(self->unk74, 0,
                      func_800407C8(&buf, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D6C));
    } else {
        func_800407C8_tmp buf2;
        func_8049602C(self->unk74, getTargetFramerate__9CDeviceVIFv() >> 1,
                      func_800407C8(&buf2, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74));
    }
    self->unk188 = 0;
    if (self->unkF0 != 0) {
        reinterpret_cast<CTaskGameFlagE9*>(self->unkF0)->flagE9 = 1;
    }
    self->unk68 &= ~0x1000;
}
// Target us-800426b4: clear the +0x39 busy bytes and null the unkCC/unkD0/
// unkF0 objects, run the func_804C8690 reset pair, then when the
// cf::CTaskGameCf singleton is live raise unk68 bit 0x8, request its exit and
// switch the move-hook ptmf to pool lbl_eu_805257D8; otherwise halve the
// unk78 budget, run the CRI ramp (func_80043CD8), push the float constants
// through the vec4 setter into the scene (func_8049602C, index framerate/2),
// and switch the move-hook ptmf to pool lbl_eu_805257E4.
void func_8004213C(CTaskGame* self) {
    if (self->unkCC != 0) {
        reinterpret_cast<CTaskGameFlag39*>(self->unkCC)->field_0x39 = 1;
        self->unkCC = 0;
    }
    if (self->unkD0 != 0) {
        reinterpret_cast<CTaskGameFlag39*>(self->unkD0)->field_0x39 = 1;
        self->unkD0 = 0;
    }
    if (self->unkF0 != 0) {
        reinterpret_cast<CTaskGameFlag39*>(self->unkF0)->field_0x39 = 1;
        self->unkF0 = 0;
    }
    func_804C8690(1, 0);
    if (cf::CTaskGameCf::getInstance() != nullptr) {
        self->unk68 |= 0x8;
        cf::CTaskGameCf::getInstance()->reqExit();
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_805257D8);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        self->unk78 = getTargetFramerate__9CDeviceVIFv() >> 1;
        func_80043CD8(getTargetFramerate__9CDeviceVIFv() >> 1, lbl_eu_80665D74);
        func_800407C8_tmp buf;
        func_8049602C(self->unk74,
                      getTargetFramerate__9CDeviceVIFv() >> 1,
                      func_800407C8(&buf, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D74, lbl_eu_80665D6C));
        u32 v1;
        u32* pool2 = reinterpret_cast<u32*>(lbl_eu_805257E4);
        v1 = pool2[0];
        CTaskGamePtmfWords* words2 = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words2->field_0x40 = pool2[1];
        words2->field_0x3C = v1;
        words2->field_0x44 = pool2[2];
    }
}
// Target us-800427ec: scene-handoff / mission-start setup. When the scene
// current-process query (func_80496034) returns non-null: run the shared
// reset (func_80043BC4), set unk68 bits (unk68 & ~3) | 4, and for the
// unk128 == 3 title path: copy the unk86/unk88 caption pair into
// unk80/unk82, strlen+strcpy the unkA4 caption into unkA0/unk90, copy the
// unk8A value into unk84, then build the +0x104 mission caption FixStr<32>
// from the default caption (lbl_eu_804FA890[0x6D]) plus the unkA4 caption
// (with the %s/%d suffix segments from lbl_eu_804FA890[0x83..0x87] appended
// via func_80044070 / the FixStr<32> format + func_800440C4), then reset the
// unk86/unk88/unkA4/unk8A title fields. After the title path: disable the
// Hbm state, create the cf::CTaskGameCf task, raise lbl_eu_80663E28 bit
// 0x10000000, run the per-mode cf::CTaskGameCf start (func_8004431C /
// func_8004433C / startMission by unk128), and switch the move-hook ptmf to
// pool lbl_eu_805257F0.
// optimize_for_size: retail saves r29-r31 with a single stmw.
#pragma optimize_for_size on
void func_80042274(CTaskGame* self) {
    if (func_80496034(self->unk74) == 0) {
        return;
    }
    func_80043BC4();
    // Retail: rlwinm r3,r3,0,31,29 (keep low 2 bits) | 0x4
    self->unk68 = (self->unk68 & 3u) | 4u;
    if ((int)self->unk128 == 3) {
        self->unk80 = self->unk86;
        self->unk82 = self->unk88;
        self->unkA0 = static_cast<u32>(strlen(self->unkA4.mString));
        strcpy(reinterpret_cast<char*>(&self->unk90), self->unkA4.mString);
        self->unk84 = self->unk8A;
        const char* s = &lbl_eu_804FA890[0x6D];
        ml::FixStr<32> fmt;
        CTaskGameCaptionStr* cap = reinterpret_cast<CTaskGameCaptionStr*>(self);
        cap->str.mLength = static_cast<int>(strlen(s));
        strcpy(cap->str.mString, s);
        if (self->unkA4.mLength != 0) {
            cap->str.mLength = static_cast<int>(strlen(self->unkA4.mString));
            strcpy(cap->str.mString, self->unkA4.mString);
            func_80044070(&cap->str, &lbl_eu_804FA890[0x83]);
            if (self->unk80 < 10) {
                func_80044070(&cap->str, &lbl_eu_804FA890[0x85]);
            }
            fmt.format(&lbl_eu_804FA890[0x87], self->unk8A);
            func_800440C4(&cap->str, fmt.mString);
        } else {
            if (self->unk80 < 10) {
                func_80044070(&cap->str, &lbl_eu_804FA890[0x85]);
            }
            fmt.format(&lbl_eu_804FA890[0x87], self->unk80);
            func_800440C4(&cap->str, fmt.mString);
            if (self->unk82 < 10) {
                func_80044070(&cap->str, &lbl_eu_804FA890[0x85]);
            }
            fmt.format(&lbl_eu_804FA890[0x87], self->unk82);
            func_800440C4(&cap->str, fmt.mString);
        }
        self->unk86 = 0;
        self->unk88 = 0;
        self->unkA4.mLength = static_cast<int>(strlen(s));
        strcpy(self->unkA4.mString, s);
        self->unk8A = 0;
    }
    CLibHbm::func_8045D5C8(false);
    create__Q22cf11CTaskGameCfFv(self, 0);
    lbl_eu_80663E28 |= 0x10000000;
    if ((int)self->unk128 == 1) {
        cf::CTaskGameCf::getInstance()->func_8004431C();
    } else if ((int)self->unk128 == 2) {
        cf::CTaskGameCf::getInstance()->func_8004433C();
    } else if ((int)self->unk128 == 3) {
        cf::CTaskGameCf::getInstance()->startMission(self->unk80, self->unk82,
                                                     *reinterpret_cast<ml::FixStr<32>*>(&self->unk90), self->unk84);
    }
    u32 v0;
    u32* pool = reinterpret_cast<u32*>(lbl_eu_805257F0);
    v0 = pool[0];
    CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
    words->field_0x40 = pool[1];
    words->field_0x3C = v0;
    words->field_0x44 = pool[2];
}
#pragma optimize_for_size off
// Target us-80042a58: when the game instance is absent / its flag bit 0x1 is
// set (func_800426F0) or this task's bit 0x2 is set, raise bit 0x8, request
// cf::CTaskGameCf exit, set its unk_54 bit 0x4 when bit 0x2 is still set, and
// switch the move-hook ptmf to pool lbl_eu_805257FC.
void func_800424E0(CTaskGame* self) {
    if (CTaskGame::func_800426F0() || (self->unk68 & 0x2) != 0) {
        self->unk68 |= 0x8;
        cf::CTaskGameCf::getInstance()->reqExit();
        if ((self->unk68 & 0x2) != 0) {
            cf::CTaskGameCf* inst = cf::CTaskGameCf::getInstance();
            inst->unk_54 |= 0x4;
        }
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_805257FC);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    }
}
// Retail func_8004256C: when the cf::CTaskGameCf singleton is live and its
// unk_54 bit 0x2 is set, set the byte flag at +0x39 (three separate
// getInstance() calls mirror the retail re-fetches; a clear bit 0x2 skips
// the caption copy entirely); then copy the default caption into the
// FixStr<32> at +0x104, clear the 0x4/0x8 unk68 bits, and switch the
// move-hook ptmf to pool lbl_eu_80525808 (bit 0x2 set) or lbl_eu_80525814.
// optimize_for_size: retail saves r30-r31 with a single stmw.
#pragma optimize_for_size on
void func_8004256C(CTaskGame* self) {
    if (cf::CTaskGameCf::getInstance() != nullptr) {
        if ((cf::CTaskGameCf::getInstance()->unk_54 & 0x2) == 0) {
            return;
        }
        cf::CTaskGameCf* inst = cf::CTaskGameCf::getInstance();
        reinterpret_cast<CTaskGameCfFlag39*>(inst)->field_0x39 = 1;
    }
    const char* s = &lbl_eu_804FA890[0x6D];
    reinterpret_cast<CTaskGameCaptionStr*>(self)->str = s;
    u32 f = self->unk68;
    self->unk68 = f & ~0xCu;
    if ((f & 0x2u) != 0) {
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525808);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
    } else {
        u32 v1;
        u32* pool2 = reinterpret_cast<u32*>(lbl_eu_80525814);
        v1 = pool2[0];
        CTaskGamePtmfWords* words2 = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words2->field_0x40 = pool2[1];
        words2->field_0x3C = v1;
        words2->field_0x44 = pool2[2];
    }
}
#pragma optimize_for_size off
// Retail func_80042630: if the effect-singleton accessor is non-null, run
// the per-frame effect update pair; then reset the object-selector singleton
// (fetch + dtor-style reset), free via the CfObj singleton teardown, set the
// move-hook ptmf from pool lbl_eu_80525820, and raise bit 0x10 of unk68.
void func_80042630(CTaskGame* self) {
    if (func_80044DF4() != 0) {
        func_80044DF4();
        func_800450C8();
    }
    __dt__800FDEF8(func_800FE68C());
    func_800B15A4(func_800B07E8());
    u32 v0;
    u32* pool = reinterpret_cast<u32*>(lbl_eu_80525820);
    v0 = pool[0];
    CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
    words->field_0x40 = pool[1];
    words->field_0x3C = v0;
    words->field_0x44 = pool[2];
    self->unk68 |= 0x10;
}
extern "C" void func_800426A8() {
    extern int func_8029183C();
    if (func_8029183C() == 2) {
        CTaskGame* self = CTaskGame::getInstance();
        self->func_80040A3C(0, 0, &lbl_eu_804FA890[0x8A], 1);
    }
}
void CTaskGame::stub_80042720() {}
void CTaskGame_stub_80042784(){}
// Target us-80042cfc: window-state gate (retail func_80040DE4 / func_80040C2C
// call it with the task pointer in r3 and branch on the result). When the
// game-instance reset gate (func_800426F0) is open: clear the unkD4 object's
// +0x60 bit 0x2, request cf::CTaskGameCf exit, switch the move-hook ptmf to
// pool lbl_eu_80525838 and return 1. Otherwise, when unk68 bit 0x2 is set:
// clear the unkD4 object's +0x60 bit 0x2, switch the move-hook ptmf to pool
// lbl_eu_80525844 and return 1. Returns 0 otherwise.
u32 func_80042784(CTaskGame* self) {
    if (CTaskGame::func_800426F0() != 0) {
        if (self->unkD4 != 0) {
            func_802956A8(reinterpret_cast<void*>(self->unkD4));
            reinterpret_cast<CTaskGameUnkD4Obj*>(self->unkD4)->field_0x60 &= ~2;
        }
        if (cf::CTaskGameCf::getInstance() != 0) {
            cf::CTaskGameCf::getInstance()->reqExit();
        }
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525838);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
        return 1;
    }
    if ((self->unk68 & 0x2) != 0) {
        if (self->unkD4 != 0) {
            func_802956A8(reinterpret_cast<void*>(self->unkD4));
            reinterpret_cast<CTaskGameUnkD4Obj*>(self->unkD4)->field_0x60 &= ~2;
        }
        u32 v0;
        u32* pool = reinterpret_cast<u32*>(lbl_eu_80525844);
        v0 = pool[0];
        CTaskGamePtmfWords* words = reinterpret_cast<CTaskGamePtmfWords*>(self);
        words->field_0x40 = pool[1];
        words->field_0x3C = v0;
        words->field_0x44 = pool[2];
        return 1;
    }
    return 0;
}
// Retail func_80042874: tear down the loading screen. Resets the phase
// counter, runs the loader shutdown helper, then deletes the object through
// its virtual deleting-dtor slot and clears the global pointer.
void func_80042874() {
    if (lbl_eu_80663D1C != nullptr) {
        lbl_eu_80663D24 = 0;
        func_802AE62C(lbl_eu_80663D1C);
        // Virtual deleting-dtor dispatch (vt+8, flag 1); the delete expansion
        // supplies the redundant pointer test (two beq).
        if (lbl_eu_80663D1C != nullptr) {
            delete reinterpret_cast<CLoadVtView*>(lbl_eu_80663D1C);
            lbl_eu_80663D1C = nullptr;
        }
    }
}
void CTaskGame_stub_8004302C(){}
// Retail func_8004302C: loading-screen lifecycle switch. With the task live
// and a scene attached (and, when flag bit 0x80 is raised, only when b != 0):
// a != 0 lazily constructs the global CLoad (MEM2 plain / MEM1 tail-aligned)
// then starts the load and clears the state counter; a == 0 stops and deletes
// it.
void func_8004302C(int a, int b) {
    CTaskGame* game = lbl_eu_80663D18;
    if (game == nullptr || game->unk74 == nullptr) {
        return;
    }
    if ((lbl_eu_80663E28 & 0x80) != 0 && b == 0) {
        return;
    }
    if (a != 0) {
        // Create path: lazily construct the global CLoad, then start the
        // load. Single shared call site (retail keeps one bl func_802AE508;
        // on the already-created edge the argument register still holds the
        // raw `a`, so pass it verbatim).
        CLoad* target;
        if (lbl_eu_80663D1C == nullptr) {
            CLoad* load;
            if (b == 0) {
                mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
                load = static_cast<CLoad*>(mtl::MemManager::allocate(0x30, handle));
                if (load != nullptr) {
                    __ct__CLoad(load, 0);
                }
            } else {
                mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM1();
                load = static_cast<CLoad*>(mtl::MemManager::allocate_ex(0x30, handle, -0x20));
                if (load != nullptr) {
                    __ct__CLoad(load, 0);
                }
            }
            lbl_eu_80663D1C = load;
            target = load;
        } else {
            target = reinterpret_cast<CLoad*>(static_cast<uintptr_t>(static_cast<unsigned>(a)));
        }
        func_802AE508(target);
        lbl_eu_80663D24 = 0;
        return;
    }
    // Teardown path.
    if (lbl_eu_80663D1C != nullptr) {
        func_802AE62C(lbl_eu_80663D1C);
        if (lbl_eu_80663D1C != nullptr) {
            delete reinterpret_cast<CLoadVtView*>(lbl_eu_80663D1C);
        }
        lbl_eu_80663D1C = nullptr;
    }
}
// Forward declaration only - body kept in separate TU to prevent MWCC inlining
extern "C" void func_8004312C();
// Retail func_8004362C: pad-confirm gate. Reads the current pad's pressed
// button word; in co-op mode (classic controller) only the D-pad/A row bits
// (3..10) plus the low nibble count, otherwise mask 0x1eff; returns whether
// any extracted bit is set (MWCC subic/subfe booleanize).
// Retail func_8004362C: pad-confirm gate. Reads the current pad's pressed
// button word; in co-op mode (classic controller) only the D-pad/A row bits
// (3..10) plus the low nibble, otherwise mask 0x1eff; returns whether any
// extracted bit is set.
// optimize_for_size: retail booleanizes with the subic/subfe setnz shape
// (MWCC only emits it under size optimization, cf. CPartyStateWin).
#pragma optimize_for_size on
// __declspec(noinline): retail keeps every caller's bl out-of-line (the
// attribute only affects call sites; the body itself is untouched).
__declspec(noinline) int func_8004362C(CTaskGame*) {
    CPad* pad = cf::CfGameManager::getCurrentPad();
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        u32 pressed = pad->mPressedButtonFlags;
        u32 v = (pressed & 0x1FE00000) | (pressed & 0xF);
        return v != 0;
    }
    return (pad->mPressedButtonFlags & 0x1eff) != 0;
}
#pragma optimize_for_size off
// Retail func_800436A8: (self->mMoveFunc == lbl_eu_80525850) as 0/1. The ptmf
// pool entry is passed by value so MWCC copies it onto the stack and compares
// via __ptmf_cmpr(&self->mMoveFunc, &local).
extern "C" s32 func_800436A8(CTaskGame* self) {
    return self->isMoveFunc(lbl_eu_80525850);
}
// Target us-80043cb4: streaming-open helper for voice/presentation files.
// When the CTaskGame singleton is live and (with the play-time gate open)
// the unkDC budget is not exceeded: resolve the file extension; when the
// func_804DE010 pack check identifies an archive path (func_804DDD54) the
// size check is skipped; otherwise getFileSize(path, 1) must be >= 0. Then
// run the shared reset (func_80043BC4), pick the alloc handle (func_80495FF0
// on lbl_eu_80663E14 when the caller passed -1), copy the path into a local
// FixStr<256> (mLength + strcpy; func_eu_804520D0 path fix-up for the
// non-archive case), open the CRI stream (func_8045997C) into unkD8, set the
// unk68 0x40/0x80 flags from the a5/a6 args, and when the play-time gate
// re-opens: re-seed the +0x130 caption FixStr<64> with the path, store the
// unkDC budget, and behind the CfGameManager reset gate forward the stream
// active state (func_80459A84) / run the func_80189C70 sound reset and
// store the unk8E ticker. Finally ramp the CRI volume (func_80043B04).
void func_80043738(u32 a1, const char* path, u32 a3, u32 a4, u32 a5, u32 a6, float volume) {
    if (lbl_eu_80663D18 == 0) {
        return;
    }
    if (func_80043D68() != 0) {
        if ((s32)lbl_eu_80663D18->unkDC < (s32)a4) {
            return;
        }
    }
    int found = 0;
    const char* ext = ml::CPathUtil::getFileExtPtr(path);
    if (ext == 0) {
        return;
    }
    if (func_804DE010() != 0) {
        u32 v0, v1, v2, v3;
        if (func_804DDD54(ext, path, &v0, &v1, &v2, &v3) != 0) {
            found = 1;
        }
    }
    if (found == 0) {
        if (getFileSize__11CDeviceFileFPCc(path, 1) < 0) {
            return;
        }
    }
    func_80043BC4();
    if ((u32)(a3 + 0x10000) == 0xFFFF) {
        a3 = func_80495FF0(lbl_eu_80663E14);
    }
    ml::FixStr<256> filename(false);
    filename.mLength = static_cast<int>(strlen(path));
    strcpy(filename.mString, path);
    if (found == 0) {
        func_eu_804520D0(filename.mString);
    }
    lbl_eu_80663D18->unkD8 = func_8045997C__7CLibCriFPCcUli(filename.mString, a3, a1);
    if (a5 != 0) {
        lbl_eu_80663D18->unk68 |= 0x40;
    } else {
        lbl_eu_80663D18->unk68 &= ~0x40;
    }
    if (a6 != 0) {
        lbl_eu_80663D18->unk68 |= 0x80;
    } else {
        lbl_eu_80663D18->unk68 &= ~0x80;
    }
    if (func_80043D68() == 0) {
        func_80043BC4();
        return;
    }
    reinterpret_cast<CTaskGameCaption130*>(lbl_eu_80663D18)->str = path;
    lbl_eu_80663D18->unkDC = a4;
    if (cf::CfGameManager::func_8007E1B4() != 0) {
        if ((lbl_eu_80663D18->unk68 & 0x40) == 0) {
            func_80459A84__7CLibCriFv(lbl_eu_80663D18->unkD8, cf::CfGameManager::func_800829B8());
        }
        if ((lbl_eu_80663D18->unk68 & 0x80) != 0) {
            func_80189C70();
            lbl_eu_80663D18->unk8E = static_cast<s16>(a6);
        }
    }
    func_80043B04(volume);
}
// Target us-80043ec8: vision streaming-open helper. When the CTaskGame
// singleton is live: build the voice file path into a local FixStr<32>
// (func_802A0CB8, index/length args); if the requested id matches the
// current unkE4/unkEC stream, stop it (func_80043BC4 for the unkE4 case, or
// the CRI stop + handle reset for the unkEC case). Then when the play-time
// gate (func_80043D68) is open: with a live unkE8 handle re-seed the unk18C
// struct (func_8004041C); otherwise allocate a fresh CRI stream (MEM2/MEM1
// alloc-handle selection by max-alloc size, func_80459AA0 + 0x800 budget),
// open it via CLibCri and record the unkE8 handle / unkEC id. When the
// gate is closed: start streaming via func_80043738 and record the unkE4 id.
int func_8004392C(u32 a, u32 b, u32 c, u32 d, u32 e, float f) {
    if (lbl_eu_80663D18 == 0) {
        return 0;
    }
    ml::FixStr<32> filename;
    if (func_802A0CB8(filename.mString, a, b) == 0) {
        return 0;
    }
    if ((s32)lbl_eu_80663D18->unkE4 == (s32)a || (s32)lbl_eu_80663D18->unkEC == (s32)a) {
        if ((s32)lbl_eu_80663D18->unkE4 == (s32)a) {
            func_80043BC4();
        } else if ((u32)(lbl_eu_80663D18->unkE8 + 0x10000) != 0xFFFF) {
            func_80459A7C__7CLibCriFv(lbl_eu_80663D18->unkE8);
            lbl_eu_80663D18->unkE8 = -1;
            lbl_eu_80663D18->unkEC = 0;
        }
    }
    if (func_80043D68() != 0) {
        // Retail sinks the live-handle re-seed (func_8004041C) out-of-line
        // after the allocate block: the unkE8 == -1 case is the fall-through,
        // the != -1 case is the else.
        if ((u32)(lbl_eu_80663D18->unkE8 + 0x10000) == 0xFFFF) {
            if ((u32)(c + 0x10000) == 0xFFFF) {
                c = static_cast<u32>(mtl::MemManager::getHandleMEM2());
            }
            u32 budget = static_cast<u32>(func_80459AA0__7CLibCriFv(1)) + 0x800;
            if (mtl::MemManager::getMaxAllocSize(c) < budget) {
                c = static_cast<u32>(mtl::MemManager::getHandleMEM1());
            }
            lbl_eu_80663D18->unkE8 = func_8045997C__7CLibCriFPCcUli(filename.mString, c, 0);
            if ((u32)(lbl_eu_80663D18->unkE8 + 0x10000) != 0xFFFF) {
                lbl_eu_80663D18->unkEC = a;
                return 1;
            }
        } else {
            func_8004041C(reinterpret_cast<Fields*>(&lbl_eu_80663D18->unk18C), 0, f, static_cast<int>(c), d, e, a, b);
        }
        return 0;
    }
    func_80043738(0, filename.mString, c, d, e, 0, f);
    if (func_80043D68() != 0) {
        lbl_eu_80663D18->unkE4 = a;
        return 1;
    }
    return 0;
}
extern "C" int func_80043D68();
// CRI volume/seek helper: while a CTaskGame instance exists and the play-time
// gate (func_80043D68) is open, forward (unkD8 handle, volume) to the CRI
// streaming-play setter. The global instance is re-read after the call.
// __declspec(noinline): retail func_80043738 calls it out of line
// (fmr f1, f31; bl func_80043B04); without it MWCC inlines the 5-instruction
// gate body into the caller.
extern "C" __declspec(noinline) void func_80043B04(float f1) {
    if (lbl_eu_80663D18 != nullptr && func_80043D68() != 0) {
        func_80459A90__7CLibCriFv(lbl_eu_80663D18->unkD8, f1);
    }
}
// Play-time seconds getter: -1 when no instance / gate closed, else the CRI
// streaming-play query result for the instance's unkD8 handle.
extern "C" s32 func_80043B54() {
    if (lbl_eu_80663D18 == nullptr) {
        return -1;
    }
    if (func_80043D68() != 0) {
        return func_80459A98__7CLibCriFv(lbl_eu_80663D18->unkD8);
    }
    return -1;
}
// Target us-80044160: shared reset helper (func_80043C88 calls it first).
// When the CTaskGame singleton is live: stop the CRI stream while the unkD8
// handle is still active (retail tests (u32)(handle + 0x10000) != 0xFFFF and
// resets the handle to -1), run the CfGameManager reset gate gated on unk68
// bit 0x80, then reset unkDC/unk68 bits 0xC0/unkE4 and re-seed the unk130
// caption FixStr<64> with the default caption (strlen+strcpy, retail order).
// The global instance is re-read after each call (retail shape).
// optimize_for_size: retail saves r30-r31 with a single stmw (MWCC merges
// the callee-saved saves only under size optimization).
#pragma optimize_for_size on
// __declspec(noinline): retail callers (func_80043C88 / func_8004312C) call
// this out of line; without it MWCC inlines the 0xC4 body into them.
__declspec(noinline) void func_80043BC4() {
    if (lbl_eu_80663D18 != nullptr) {
        if ((u32)(lbl_eu_80663D18->unkD8 + 0x10000) != 0xFFFF) {
            func_80459A7C__7CLibCriFv(lbl_eu_80663D18->unkD8);
            lbl_eu_80663D18->unkD8 = -1;
        }
        if (cf::CfGameManager::func_8007E1B4() != 0) {
            if ((lbl_eu_80663D18->unk68 & 0x80) != 0) {
                func_80189C7C();
            }
        }
        lbl_eu_80663D18->unkDC = 2;
        lbl_eu_80663D18->unk68 &= ~0xC0;
        lbl_eu_80663D18->unkE4 = 0;
        const char* s = &lbl_eu_804FA890[0x6D];
        reinterpret_cast<CTaskGameCaption130*>(lbl_eu_80663D18)->str = s;
    }
}
#pragma optimize_for_size off
// Target us-80044224: after the shared reset (func_80043BC4), stop the CRI
// stream when the unkE8 handle is still active (!= -1) and reset the handle /
// length fields. The global instance is re-read after the call (retail shape).
extern "C" void func_80043C88() {
    func_80043BC4();
    int handle = lbl_eu_80663D18->unkE8;
    if ((u32)handle != 0xFFFFFFFFu) {
        func_80459A7C__7CLibCriFv(handle);
        lbl_eu_80663D18->unkE8 = -1;
        lbl_eu_80663D18->unkEC = 0;
    }
}
// Target us-80044274: CRI stream volume/rate ramp. While a CTaskGame instance
// exists with an active stream handle, convert the frame count to seconds
// ((f32)count * frame time) and forward (handle, volume, seconds, 2) to the
// CRI streaming-play setter. The global instance is re-read for the handle.
extern "C" __declspec(noinline) void func_80043CD8(int frames, float volume) {
    if (lbl_eu_80663D18 != nullptr && (u32)lbl_eu_80663D18->unkD8 != 0xFFFFFFFFu) {
        func_80459A88__7CLibCriFv(lbl_eu_80663D18->unkD8, volume,
                                  (float)frames * getSecPerFrame__9CDeviceVIFv(), 2);
    }
}
// Retail func_80043D90: enum-list holder ctor. Seeds +0x00 from the CTaskGame
// object registry (func_80043310); when no slot is free, allocates a fresh
// CfObjEnumList from the MEM2 region. Zeroes the list's count fields and
// returns the holder.
CfEnumListHolder* func_80043D90(CfEnumListHolder* self) {
    self->handle = -1;
    self->list = reinterpret_cast<cf::CfObjEnumList*>(func_80043310());
    if (self->list == nullptr) {
        self->handle = mtl::MemManager::getHandleMEM2();
        cf::CfObjEnumList* list = static_cast<cf::CfObjEnumList*>(
            mtl::MemManager::allocate(0x3040, self->handle));
        if (list != nullptr) {
            list = __ct__cf_CfObjEnumList(list);
        }
        self->list = list;
    }
    CfObjEnumListFields* list = reinterpret_cast<CfObjEnumListFields*>(self->list);
    list->mPtrCount = 0;
    list->mObjInfoCount = 0;
    return self;
}
// Retail __dt__80043E88: enum-list holder destructor. When the holder owns a
// heap list (handle != -1) it virtual-deletes the list (vtable slot +8, flag 1)
// and nulls the slot; when the list came from the CTaskGame object registry
// (handle == -1) it returns the slot via func_8004335C. Deletes the holder
// itself when flag > 0. The redundant second pointer test mirrors MWCC's
// `delete` expansion (retail emits two beq against one cmpwi).
#pragma optimize_for_size on
CfEnumListHolder* __dt__80043E88(CfEnumListHolder* self, int flag) {
    if (self != nullptr) {
        if ((u32)self->handle != 0xFFFFFFFFu) {
            if (self->list != nullptr) {
                // Retail `delete list` (virtual deleting-dtor dispatch at
                // vtable+8 with flag 1); the redundant pointer test mirrors
                // the delete expansion (retail emits two beq, one cmpwi).
                delete reinterpret_cast<CfObjEnumListVtView*>(self->list);
                self->list = nullptr;
            }
        } else {
            func_8004335C(self->list);
        }
        if (flag > 0) {
            operator delete(self);
        }
    }
    return self;
}
#pragma optimize_for_size off

void CTaskGame_stub_80043E08(){}
// Target us-800443a4: enum-list holder ctor with message fill. Seeds +0x00
// from the CTaskGame object registry (func_80043310); when no slot is free,
// allocates a fresh CfObjEnumList from the MEM2 region. Unlike func_80043D90
// the count fields are NOT zeroed here; the list is passed straight to
// func_800F4A98 with the caller's type/filter and the holder is returned.
#pragma optimize_for_size on
CfEnumListHolder* func_80043E08(CfEnumListHolder* self, u32 type, u32 filter) {
    self->handle = -1;
    self->list = reinterpret_cast<cf::CfObjEnumList*>(func_80043310());
    if (self->list == nullptr) {
        self->handle = mtl::MemManager::getHandleMEM2();
        cf::CfObjEnumList* list = static_cast<cf::CfObjEnumList*>(
            mtl::MemManager::allocate(0x3040, self->handle));
        if (list != nullptr) {
            list = __ct__cf_CfObjEnumList(list);
        }
        self->list = list;
    }
    func_800F4A98(self->list, type, filter);
    return self;
}
#pragma optimize_for_size off
// Target us-8004460c: append a C-string to a FixStr<32> (retail call sites
// pass CTaskGame+0x104): strlen+strcat into mString and bump mLength; return
// the buffer (mString at +0, so == self). Retail emits the identical body
// twice under two symbols (func_80044070 / func_800440C4). The retail frame
// is stmw r29 - MWCC merges the 3 callee-saved saves only under
// optimize_for_size (same pattern as the CTTask/reslist dtors in this TU).
#pragma optimize_for_size on
extern "C" __declspec(noinline) char* func_80044070(ml::FixStr<32>* str, const char* s) {
    str->operator+=(s);
    return str->mString;
}
#pragma optimize_for_size off

// Target us-800436a4: per-frame CRI / error-message maintenance. While the
// unkD8 stream handle is live: stop it via func_80459A78 when it reports
// done (else, behind the CfGameManager reset gate and unk68 bit 0x80, count
// down the unk8E ticker and reset the sound state when it expires). The
// unkE8 handle is similarly reset. Then, when the play-time gate
// (func_80043D68) and the CfGameManager reset gate are open and unk68 bit
// 0x40 is clear, forward (unkD8, bool) to the CRI active-setter with the OR
// of the func_8016C720 / func_800829B8 gates. Finally, when the unk18C
// struct's +0x18 word is non-zero and the func_8004392C vision helper
// succeeds, re-seed the unk18C struct (func_8004041C with the float
// constant); when the scene pointer is gone, disable the Hbm state.
// optimize_for_size: retail bool conversion of the gate OR uses the
// subic/subfe idiom (the -O4,s form; -O4,p emits neg/or/rlwinm).
#pragma optimize_for_size on
void func_8004312C(CTaskGame* self) {
    if ((u32)(self->unkD8 + 0x10000) != 0xFFFF) {
        if (func_80459A78__7CLibCriFv(self->unkD8) == 0) {
            // Retail keeps this small reset inline (the big gate chain is
            // sunk out-of-line at the end with a bne from the test).
            func_80043BC4();
        } else {
            if (cf::CfGameManager::func_8007E1B4() != 0) {
                if ((self->unk68 & 0x80) != 0) {
                    self->unk8E--;
                    if (self->unk8E <= 0) {
                        func_80189C7C();
                        self->unk68 &= ~0x80;
                    }
                }
            }
        }
    }
    if ((u32)(self->unkE8 + 0x10000) != 0xFFFF) {
        if (func_80459A78__7CLibCriFv(self->unkE8) == 0) {
            self->unkE8 = -1;
            self->unkEC = 0;
        }
    }
    if (func_80043D68() != 0) {
        if (cf::CfGameManager::func_8007E1B4() != 0) {
            if ((lbl_eu_80663D18->unk68 & 0x40) == 0) {
                // Right-to-left arg evaluation: write the CfGameManager gate
                // first so func_8016C720 runs first into r30 (retail
                // mr r30, r3), then the gate returns in r3 and the OR is
                // r3 | r30 (retail or r4, r3, r30) before the subic/subfe
                // bool conversion (size-opt idiom).
                func_80459A84__7CLibCriFv(self->unkD8,
                                          (cf::CfGameManager::func_800829B8() | func_8016C720()) != 0);
            }
        }
    }
    if (self->unk18C.unk18 != 0) {
        // Sequenced loads mirror the retail order (b, a, f1, c, d, e); the
        // volatile pins the float load at its statement position.
        // Both loads volatile-pinned so their statement order survives
        // scheduling (retail: a into r3, then the float into f1).
        u32 a = *(volatile u32*)&self->unk18C.unk14;
        const float f = *(volatile const float*)&self->unk18C.unk4;
        if (func_8004392C(a, self->unk18C.unk18,
                          static_cast<u32>(self->unk18C.unk8),
                          self->unk18C.unkC, self->unk18C.unk10,
                          f) != 0) {
            func_8004041C(reinterpret_cast<Fields*>(&self->unk18C), 0, lbl_eu_80665D6C, -1, 2, 0, 0, 0);
        }
    }
    if (self->unk74 == 0) {
        CLibHbm::func_8045D470(false);
    }
}
#pragma optimize_for_size off
// Retail Draw__9CTaskGameFv is a 4-byte tail call to func_8004312C; the
// no-arg extern "C" decl keeps that call on the flat retail symbol (the
// func_8004312C definition below is the same flat symbol).
extern "C" void func_8004312C(); void Draw__9CTaskGameFv() {
    func_8004312C();
}

extern "C" __declspec(noinline) void func_8004347C(CTaskGame* inst, u32 a, u32 b, u32 c) {
    inst->unk68 &= ~0x00000100;
    if (a != 0) {
        inst->unk68 |= 0x00080000;
    } else {
        inst->unk68 &= ~0x00080000;
    }
    inst->unkFC = c;
}
// __declspec(noinline): retail func_80043564 tail-calls these out of line;
// without it MWCC inlines the stub bodies into the dispatcher.
extern "C" __declspec(noinline) void func_800434AC(CTaskGame* inst, u32 a, u32 b, u32 c) {
    inst->unk68 &= ~0x00000100;
    if (a != 0) {
        inst->unk68 |= 0x00000800;
    } else {
        inst->unk68 &= ~0x00000800;
    }
    inst->unkFC = c;
}
// Target us-80044660: identical body to func_80044070 (second retail symbol).
#pragma optimize_for_size on
extern "C" __declspec(noinline) char* func_800440C4(ml::FixStr<32>* str, const char* s) {
    str->operator+=(s);
    return str->mString;
}
#pragma optimize_for_size off

// Target us-80040f64: _reslist_base<cf::CfObject*> destructor (retail flat
// name). Restores the base vtable, walks the sentinel list nulling each
// node's mNext, then frees the backing array when the owned-flag is clear
// and deletes self when the delete flag is positive. Direct member access
// reproduces retail's per-iteration reload of mStartNodePtr.
#pragma optimize_for_size on
// __declspec(noinline): retail callers (CfObjEnumList dtor / reslist dtor)
// call this out of line; without it MWCC inlines the 0xAC body into them.
extern "C" __declspec(noinline) void* __dt___reslist_base_cf_CfObject(void* self, int flag) {
    _reslist_base<cf::CfObject*>* base = static_cast<_reslist_base<cf::CfObject*>*>(self);
    if (base != nullptr) {
        *(void**)base = lbl_eu_8052585C;
        {
            _reslist_node<cf::CfObject*>* cur = base->mStartNodePtr->mNext;
            while (cur != base->mStartNodePtr) {
                _reslist_node<cf::CfObject*>* prev = cur;
                cur = cur->mNext;
                prev->mNext = nullptr;
            }
            base->mStartNodePtr->mNext = base->mStartNodePtr;
            base->mStartNodePtr->mPrev = base->mStartNodePtr;
        }
        if (base->unk1C == false && base->mList != nullptr) {
            __dla__FPv(base->mList);
            base->mList = nullptr;
        }
        if (flag > 0) {
            operator delete(base);
        }
    }
    return self;
}
#pragma optimize_for_size off

// Retail __dt__Q22cf13CfObjEnumListFv (0x54) lives in this TU (weak placement).
// The reslist base dtor is a separate retail symbol; call it explicitly.
// optimize_for_size gives the retail stmw r30 prologue (like CTTask's dtor).
#pragma optimize_for_size on
cf::CfObjEnumList::~CfObjEnumList() {
    if (this) {
        __dt___reslist_base_cf_CfObject(this, 0);
    }
}

// Retail __dt__24reslist<PQ22cf8CfObject>Fv: a thin deleting-dtor wrapper that
// calls the flat-mangled base dtor out of line. The primary reslist<T> template
// (reslist.hpp) defines an inline virtual dtor that MWCC would emit in the full
// inlined form (vtable store + base body, ~0xB0) - the retail symbol is the
// 0x50 wrapper, so provide a TU-local full class specialization: non-polymorphic
// with a declared-only dtor, so MWCC emits no vtable store and no implicit base
// call. The explicit extern "C" call pins the retail flat base symbol (MWCC
// leaves __-prefixed names unmangled). optimize_for_size gives the stmw r30
// prologue (like CTTask's dtor).
template<>
class reslist<cf::CfObject*> {
public:
    ~reslist();
};

#pragma optimize_for_size on
reslist<cf::CfObject*>::~reslist() {
    __dt___reslist_base_cf_CfObject(this, 0);
}
#pragma optimize_for_size off

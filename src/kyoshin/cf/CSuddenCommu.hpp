#pragma once

#include <types.h>
#include "kyoshin/cf/object/IObjectInfo.hpp"

namespace cf{
    //size: 0x30?
    class CSuddenCommu : public IObjectInfo {
    public:
        CSuddenCommu();
        virtual ~CSuddenCommu(){}
        virtual void IObjectInfo_UnkVirtualFunc1();
        void func_801BA1DC();

        //0x0: vtable
        //0x0-4: IObjectInfo?
        /* 0x04 */ s16 field_4;
        /* 0x06 */ s16 field_6;
        /* 0x08 */ s16 field_8;
        /* 0x0A */ s16 field_A;
        /* 0x0C */ s16 field_C;
        /* 0x0E */ s16 field_E;
        /* 0x10 */ u32 field_10; // voice id probed via func_802A3748
        /* 0x14 */ u32 field_14;
        /* 0x18 */ float field_18;
        /* 0x1C */ float field_1C;
        /* 0x20 */ u32 field_20;
        /* 0x24 */ volatile u32 field_24;
        /* 0x28 */ u32 unk28;
        /* 0x2C */ float unk2C;
    };
}

// Battle-actor base pointer: CfGameManager::getPlayer(i) returns the embedded
// CfObjectMove at base+0x3E9C; subtracting 0x3E9C recovers the base used by
// the sudden-commu functions. Only the touched fields are declared.
struct CSuddenCommuVoiceAct {
    u8 pad_00[0x8];
    u16 field_3388;   // 0x3388 relative to the actor base
};

// Sub-object at actor+0x04: virtual slot 0x30 returns the object-list head
// (its first word is read as u32 and handed to func_80174C98).
// Abstract - cast-only, never constructed.
struct CSuddenCommuSub4 {
    virtual void v00() = 0; virtual void v01() = 0; virtual void v02() = 0; virtual void v03() = 0;
    virtual void v04() = 0; virtual void v05() = 0; virtual void v06() = 0; virtual void v07() = 0;
    virtual void v08() = 0; virtual void v09() = 0;
    virtual u32* vf30() = 0;   // slot 0x30 (index 12, incl. MWCC vtable prefix)
};

// Object behind actor+0x3ED4: u32 at +0x36C (non-zero test in func_801BB464).
struct CSuddenCommuBody36C {
    u8 pad_00[0x36C];
    u32 field_36C;                        // 0x36C
};

struct CSuddenCommuActor {
    void** vtable;                  // 0x00 (slots 0x88/0xE0/0x128 used here)
    CSuddenCommuSub4* field_4;      // 0x04 (sub-object, vtable slot 0x30)
    u8 pad_08[0x15E0 - 0x08];
    void* field_15E0;               // 0x15E0 (battle-object handle probed via func_80260518)
    u8 pad_15E4[0x3380 - 0x15E4];
    CSuddenCommuVoiceAct voiceAct;  // 0x3380 (handed to func_801537E0)
    u8 pad_338A[0x3E9C - 0x338A];
    void* moveSpot;                 // 0x3E9C (getPlayer() points here)
    u8 pad_3EA0[0x3ED4 - 0x3EA0];
    CSuddenCommuBody36C* field_3ED4;  // 0x3ED4
    u8 pad_3ED8[0x3F00 - 0x3ED8];
    u32 flags3F00;                  // 0x3F00 (bit 0x2 = player-controlled)
    u8 pad_3F04[0x0C];
    void* field_3F10;               // 0x3F10
    u8 pad_3F14[0x14];
    u16 field_3F28;                 // 0x3F28
    u8 pad_3F2A[0x3F60 - 0x3F2A];
    void* subField3F60;             // 0x3F60
};

// Local battle-move flag view (retail E2A9C_BattleMoveData lives in
// CBattleManager.cpp; only the flag words at 0x74/0x78 are touched).
struct CSuddenCommuMoveData {
    u8 pad_00[0x74];
    u32 field_74;    // 0x74
    u32 field_78;    // 0x78
};

// Effect/cue object produced by func_800451D8 for the sudden-commu voice cue.
struct CSuddenCommuVoiceCue {
    void** vtable;          // 0x00 (slot 0x88 takes a float)
    u8 pad_04[0xB0 - 0x04];
    void* field_B0;         // 0xB0
};

// Vtable-shape for the voice-cue object: slot 0x88 (index 34) takes a float.
// Abstract - cast-only, never constructed; the virtual call emits the retail
// lwz r12 / lwz r12,0x88 / mtctr / bctrl sequence.
struct CSuddenCommuVoiceCueVt {
    virtual void v00() = 0; virtual void v01() = 0; virtual void v02() = 0; virtual void v03() = 0;
    virtual void v04() = 0; virtual void v05() = 0; virtual void v06() = 0; virtual void v07() = 0;
    virtual void v08() = 0; virtual void v09() = 0; virtual void v10() = 0; virtual void v11() = 0;
    virtual void v12() = 0; virtual void v13() = 0; virtual void v14() = 0; virtual void v15() = 0;
    virtual void v16() = 0; virtual void v17() = 0; virtual void v18() = 0; virtual void v19() = 0;
    virtual void v20() = 0; virtual void v21() = 0; virtual void v22() = 0; virtual void v23() = 0;
    virtual void v24() = 0; virtual void v25() = 0; virtual void v26() = 0; virtual void v27() = 0;
    virtual void v28() = 0; virtual void v29() = 0; virtual void v30() = 0; virtual void v31() = 0;
    virtual void vf88(float f) = 0;   // slot 0x88 (index 34, incl. MWCC vtable prefix)
};

// Vtable-shape for battle actors (CSuddenCommuActor base): slots 0xE0
// (index 56, int) and 0x128 (index 74, float) are used by func_801BAB94.
// Abstract - cast-only, never constructed.
struct CSuddenCommuActBody;
struct CSuddenCommuActorVt {
    virtual void v00() = 0; virtual void v01() = 0; virtual void v02() = 0; virtual void v03() = 0;
    virtual void v04() = 0; virtual void v05() = 0; virtual void v06() = 0; virtual void v07() = 0;
    virtual void v08() = 0; virtual void v09() = 0; virtual void v10() = 0; virtual void v11() = 0;
    virtual void v12() = 0; virtual void v13() = 0; virtual void v14() = 0; virtual void v15() = 0;
    virtual void v16() = 0; virtual void v17() = 0; virtual void v18() = 0; virtual void v19() = 0;
    virtual void v20() = 0; virtual void v21() = 0; virtual void v22() = 0; virtual void v23() = 0;
    virtual void v24() = 0; virtual void v25() = 0; virtual void v26() = 0; virtual void v27() = 0;
    virtual void v28() = 0; virtual void v29() = 0; virtual void v30() = 0; virtual void v31() = 0;
    virtual void v32() = 0; virtual void v33() = 0; virtual void v34() = 0; virtual void v35() = 0;
    virtual void v36() = 0; virtual void v37() = 0; virtual void v38() = 0; virtual void v39() = 0;
    virtual void v40() = 0; virtual void v41() = 0; virtual void v42() = 0; virtual void v43() = 0;
    virtual void v44() = 0; virtual void v45() = 0; virtual void v46() = 0; virtual void v47() = 0;
    virtual void v48() = 0; virtual void v49() = 0; virtual void v50() = 0; virtual void v51() = 0;
    virtual void v52() = 0; virtual void v53() = 0;
    virtual int vfE0() = 0;                          // slot 0xE0 (index 56, incl. MWCC vtable prefix)
    virtual void v54() = 0; virtual void v55() = 0; virtual void v56() = 0; virtual void v57() = 0;
    virtual void v58() = 0; virtual void v59() = 0; virtual void v60() = 0; virtual void v61() = 0;
    virtual void v62() = 0; virtual void v63() = 0; virtual void v64() = 0; virtual void v65() = 0;
    virtual void v66() = 0; virtual void v67() = 0;
    virtual void vf11C(float f) = 0;                 // slot 0x11C (index 69, incl. MWCC vtable prefix)
    virtual void v69() = 0;
    virtual void v70() = 0;
    virtual float vf128() = 0;                       // slot 0x128 (index 74, incl. MWCC vtable prefix)
    // Deep slots used by the sudden-commu triggers (func_801BA2DC / func_801BA978 /
    // func_801BB464): 0x298 (returns CSuddenCommuActBody), 0x304/0x308 (int),
    // 0x314 (void). Padding virtuals keep the slot offsets exact.
    virtual float vf12C() = 0;                       // slot 0x12C (index 73, incl. MWCC vtable prefix)
    virtual void v72() = 0; virtual void v73() = 0; virtual void v74() = 0;
    virtual void v75() = 0; virtual void v76() = 0; virtual void v77() = 0; virtual void v78() = 0;
    virtual void v79() = 0; virtual void v80() = 0; virtual void v81() = 0; virtual void v82() = 0;
    virtual void v83() = 0; virtual void v84() = 0; virtual void v85() = 0; virtual void v86() = 0;
    virtual void v87() = 0; virtual void v88() = 0; virtual void v89() = 0; virtual void v90() = 0;
    virtual void v91() = 0; virtual void v92() = 0; virtual void v93() = 0; virtual void v94() = 0;
    virtual void v95() = 0; virtual void v96() = 0; virtual void v97() = 0; virtual void v98() = 0;
    virtual void v99() = 0; virtual void v100() = 0; virtual void v101() = 0; virtual void v102() = 0;
    virtual void v103() = 0; virtual void v104() = 0; virtual void v105() = 0; virtual void v106() = 0;
    virtual void v107() = 0; virtual void v108() = 0; virtual void v109() = 0; virtual void v110() = 0;
    virtual void v111() = 0; virtual void v112() = 0; virtual void v113() = 0; virtual void v114() = 0;
    virtual void v115() = 0; virtual void v116() = 0; virtual void v117() = 0; virtual void v118() = 0;
    virtual void v119() = 0; virtual void v120() = 0; virtual void v121() = 0; virtual void v122() = 0;
    virtual void v123() = 0; virtual void v124() = 0; virtual void v125() = 0; virtual void v126() = 0;
    virtual void v127() = 0; virtual void v128() = 0; virtual void v129() = 0; virtual void v130() = 0;
    virtual void v131() = 0; virtual void v132() = 0; virtual void v133() = 0; virtual void v134() = 0;
    virtual void v135() = 0; virtual void v136() = 0; virtual void v137() = 0; virtual void v138() = 0;
    virtual void v139() = 0; virtual void v140() = 0; virtual void v141() = 0; virtual void v142() = 0;
    virtual void v143() = 0; virtual void v144() = 0; virtual void v145() = 0; virtual void v146() = 0;
    virtual void v147() = 0; virtual void v148() = 0; virtual void v149() = 0; virtual void v150() = 0;
    virtual void v151() = 0; virtual void v152() = 0; virtual void v153() = 0; virtual void v154() = 0;
    virtual void v155() = 0; virtual void v156() = 0; virtual void v157() = 0; virtual void v158() = 0;
    virtual void v159() = 0; virtual void v160() = 0; virtual void v161() = 0;
    virtual CSuddenCommuActBody* vf298() = 0;       // slot 0x298 (index 164, incl. MWCC vtable prefix)
    virtual void v162() = 0; virtual void v163() = 0; virtual void v164() = 0; virtual void v165() = 0;
    virtual void v166() = 0; virtual void v167() = 0; virtual void v168() = 0; virtual void v169() = 0;
    virtual int vf2BC() = 0;                         // slot 0x2BC (index 173, incl. MWCC vtable prefix)
    virtual void v171() = 0; virtual void v172() = 0; virtual void v173() = 0;
    virtual void v174() = 0; virtual void v175() = 0; virtual void v176() = 0; virtual void v177() = 0;
    virtual void v178() = 0; virtual void v179() = 0; virtual void v180() = 0; virtual void v181() = 0;
    virtual void v182() = 0; virtual void v183() = 0; virtual void v184() = 0; virtual void v185() = 0;
    virtual void v186() = 0; virtual void v187() = 0;
    virtual int vf304(int arg) = 0;                 // slot 0x304 (index 191, incl. MWCC vtable prefix)
    virtual int vf308() = 0;                        // slot 0x308 (index 192, incl. MWCC vtable prefix)
    virtual void v188() = 0; virtual void v189() = 0;
    virtual void vf314() = 0;                       // slot 0x314 (index 195, incl. MWCC vtable prefix)
};

// 8-byte holder around a CfObjEnumList* (func_80043D90 / __dt__80043E88).
struct CSuddenCommuEnumHolder {
    void* list;   // 0x0
    u32 handle;   // 0x4
};

// View of the CBattleManager actor list: the reslist sentinel pointer
// (mActorList3.mHead) sits at +0x48.
struct CSuddenCommuBmList {
    u8 pad_00[0x48];
    void* mHead;    // 0x48
};

// Intrusive list node (next link at +0x00).
struct CSuddenCommuListNode {
    CSuddenCommuListNode* next;   // 0x00
};

// CBattleManager field view (retail CBattleManager.hpp is not includable from
// this TU - CfObjectActor.hpp decl conflict): only 0x1AA/0x20C8 are touched.
struct CSuddenCommuBmView {
    u8 pad_00[0x1AA];
    u8 field_1AA;                   // 0x1AA
    u8 pad_1AB[0x20C8 - 0x1AB];
    s16 field_20C8;                 // 0x20C8
};

// Global settings object (retail CfGimmickGlobal): only 0x214 touched.
struct CSuddenCommuGlobal {
    u8 pad_00[0x214];
    u32 field_214;                  // 0x214
};

// Object returned by the actor vtable slot 0x298: int state at +0x48.
struct CSuddenCommuActBody {
    u8 pad_00[0x48];
    int field_48;                   // 0x48
};

// 0x34-byte command payload built for func_800EA9A8 (voice/battle cue); only
// the fields the retail writes are declared.
struct CSuddenCommuCmd {
    u8 pad_00[0xC];
    u16 field_C;                    // 0xC
    u8 pad_E[2];
    u32 field_10;                   // 0x10
    u8 pad_14[0x20 - 0x14];
    f32 field_20;                   // 0x20
    u8 pad_24[0x34 - 0x24];
};

// u32-word pair / f64 view used for MWCC's 0x43300000 int->float conversion
// (subtracts the retail .sdata2 magic double lbl_eu_80667E40 so the pool reloc
// matches the retail constant instead of an MWCC-synthesised @N entry).
union CSuddenCommuF64Conv {
    u32 w[2];
    f64 d;
};
// CBattleManager sub-object at +0x194 (party gauge), handed to func_8018C820.
struct CSuddenCommuBmGauge {
    u8 pad_00[0x194];
    u32 field_194;                  // 0x194
};

// C-linkage callees defined in CSuddenCommu.cpp (retail symbols are unmangled
// global func_* names - the declarations below give the definitions C linkage
// so the object emits the exact retail symbol names).
extern "C" {
    void func_801BA490(cf::CSuddenCommu* self);
    void func_801BA978(cf::CSuddenCommu* self);
    void func_801BC6A4(cf::CSuddenCommu* self, int val, int num);
    // Imported voice-node lookup (defined in voice/CCharVoiceMan.cpp): returns
    // 0 when no voice node is registered for the given voice id.
    int func_802A3748(u32 arg);
    // Same-TU siblings (defined in CSuddenCommu.cpp; C linkage so the call
    // relocs and definitions keep the unmangled retail names).
    int func_801BA2DC(cf::CSuddenCommu* self);
    void func_801BB464(cf::CSuddenCommu* self, int playerIdx, int mode, CSuddenCommuActor* player, int arg5);
    void func_801BC474(cf::CSuddenCommu* self);
    void func_801BC590(cf::CSuddenCommu* self);
    // Voice-manager imports (defined in voice/CCharVoiceMan.cpp): retire/next
    // voice id. C linkage keeps the call relocs at the unmangled retail names.
    void func_802A35B8(u32 arg);
    int func_802A3290();
    // Battle-voice dispatch (defined in voice/CCharVoiceMan.cpp): run the
    // voice node for `voiceId` with the target spot and the commu's result id.
    void* func_802A3680(u32 voiceId, void* spot, int result);
    // Battle-manager singleton (retail pre-mangled name; C linkage keeps the
    // symbol verbatim - CSysWinScenarioLog idiom). CBattleManager.hpp is not
    // included by CSuddenCommu.cpp (conflicting func_80149154 decl in
    // CfObjectActor.hpp), so the singleton is reached through this decl.
    void* getInstance__Q22cf14CBattleManagerFv();
    // Voice/help imports used by the sudden-commu triggers (unmangled retail
    // names - C linkage keeps the call relocs verbatim).
    u32 func_8009CF8C(u32 resourceId);
    int func_80174C98(void* actor, u32* outVal, u32 flags);
    int func_8017FD44(void* global);
    CSuddenCommuGlobal* getUnk80664658();
}

// Retail sdata2 float constants (values live in the retail binary). Non-const:
// MWCC must reload them after calls, so loop uses stay at the use site (retail
// shape) instead of being hoisted into a saved FPR.
extern float lbl_eu_80667E30;
extern float lbl_eu_80667E34;   // voice-count scale (held in a saved FPR across the sweep)
extern float lbl_eu_80667E38;
extern f64 lbl_eu_80667E40;    // .sdata2 double: 0x4330000080000000 u32->f32 magic
extern float lbl_eu_80667E48;
extern float lbl_eu_80667E4C;   // commu partner timer clamp (voice re-arm)
extern float lbl_eu_80667E50;
extern float lbl_eu_80667E60;   // camera/trigger frame value (func_80133F48 arg)
extern float lbl_eu_80667E64;   // field_18 lower clamp (idle commu path)
extern float lbl_eu_80667E54;
extern float lbl_eu_80667E58;

// Running voice-count seed (.sdata) accumulated across the player sweep.
extern u32 lbl_eu_806625E0;

// C-ABI imports used by the sudden-commu voice sweep (retail symbols are
// unmangled global func_* names - declared here rather than TU-locally).
extern "C" {
    // Battle-object stat / event-flag probes (code_8025FB10.cpp).
    int func_80260518(void* self, int id, u32* outVal, f32* outF);
    int func_80260264(void* self, int id, u32* outVal);
    // Battle-command dispatch and battle-move accessor (CBattleManager.cpp).
    void func_800EA9A8(void* bm, void* actor, CSuddenCommuCmd* cmd, int size, int flag);
    f32 func_800D81A8(int arg1, void* actor, int arg3);
    s32 func_8018C820(void* obj, s32 value);
    // Voice/battle-manager helpers.
    void func_80280BF0();
    void func_80082568__Q22cf13CfGameManagerFv(int a, int b, int c);
    // CfGameManager gate used by the commu re-arm (retail pre-mangled name;
    // the class member is declared in CfGameManager.hpp, this is the bare
    // Fv-form import so the call reloc keeps the retail symbol verbatim).
    bool func_8007F91C__Q22cf13CfGameManagerFv();
    bool func_8006EF04__Fi(s32 mask);
    // Commu camera/trigger helpers (retail pre-mangled names; the Fv suffix
    // does not reflect the actual argument counts - see func_80082568).
    bool func_80086F9C__Q22cf13CfGameManagerFv(int arg);
    void func_80080F44__Q22cf13CfGameManagerFv(void* obj);
    // Camera/trigger object accessors (lbl_eu_80663E14 is a global pointer).
    extern "C" void* func_8049603C(void* obj);
    f32 func_80496288(void* obj);
    int func_8017FD4C(void* obj);
    void func_8017FEF0(void* obj, int arg);
}

// Commu camera/trigger globals (.sdata).
extern volatile u32 lbl_eu_80663E24;
extern void* lbl_eu_80663E14;

// Pad button-flag view (retail CPad first word).
struct CSuddenCommuPadView {
    u32 field_00;   // +0 button flags
};

// Camera/trigger position view (func_8049603C result): f32 at +0xC.
struct CSuddenCommuCamView {
    u8 pad_00[0xC];
    f32 field_C;    // 0xC
};

// Voice-id / voice-act tables (@sda21 in func_801BC6A4 / func_801BB464).
extern int lbl_eu_806625E4;
extern u8 lbl_eu_806625E8[];
extern u8 lbl_eu_806625F0[];
extern u8 lbl_eu_806625F8[];
extern u8 lbl_eu_80662600[];
extern u16 lbl_eu_80662608[1];
extern const u8 lbl_eu_805050B0[];

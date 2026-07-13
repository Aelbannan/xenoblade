#include "kyoshin/CUIBattleManager.hpp"

#include "monolib/util/MemManager.hpp"

struct CUIBattleInitProcess {
    u8 unk00[0x10];
    void* vtable;
    u8 unk14[0x28];
    u32 callbacks[6];
};

extern "C" {
CProcess* lbl_eu_80664048;
char lbl_eu_8052E208[];
char lbl_eu_8052E1C0[];
u32 __ptmf_null[3];
void __ct__8CProcessFv(CProcess*);
}

void CUIBattleManager::Init() {
    CUIBattleInitProcess* process;
    u8* ptmfBase;
    char* vtFinal;
    u32 ptmfWord1;
    u32 ptmfWord0;
    u32 ptmfWord2;

    process = static_cast<CUIBattleInitProcess*>(
        mtl::MemManager::allocate(0x54, CWorkThreadSystem::getWorkMem()));
    if (process != NULL) {
        __ct__8CProcessFv(reinterpret_cast<CProcess*>(process));
        ptmfBase = (u8*)__ptmf_null;
        process->vtable = lbl_eu_8052E208;
        ptmfWord1 = *(u32*)(ptmfBase + 4);
        vtFinal = lbl_eu_8052E1C0;
        ptmfWord0 = *(u32*)(ptmfBase + 0);
        process->callbacks[0] = ptmfWord0;
        process->callbacks[1] = ptmfWord1;
        ptmfWord2 = *(u32*)(ptmfBase + 8);
        process->callbacks[2] = ptmfWord2;
        ptmfWord1 = *(u32*)(ptmfBase + 4);
        ptmfWord0 = *(u32*)(ptmfBase + 0);
        process->callbacks[3] = ptmfWord0;
        process->callbacks[4] = ptmfWord1;
        ptmfWord2 = *(u32*)(ptmfBase + 8);
        process->callbacks[5] = ptmfWord2;
        process->vtable = vtFinal;
    }
    unk7C = reinterpret_cast<CProcess*>(process);
    reinterpret_cast<CProcess*>(process)->Regist(lbl_eu_80664048, false);
}

#include "monolib/work.hpp"
#include "monolib/util.hpp"

// Retail .rodata string owned by this TU (monolibdata1 blob dissolve):
//   lbl_eu_805224C0 (.rodata 0x2) = " "
// dispTree's indent pad string. The sdata2_threshold 0 pragma keeps the
// 2-byte const string in .rodata — MWCC would otherwise route a <=8B const
// into .sdata2, and consumers address it via lis/@l (matching retail).
#ifdef __MWERKS__
#pragma push
#pragma sdata2_threshold 0
#endif
extern "C" const char lbl_eu_805224C0[] = " ";
#ifdef __MWERKS__
#pragma pop
#endif

UnkStruct_80438AF0_3* CWorkUtil::func_80438AF0(UnkStruct_80438AF0* arg0){
    return arg0->unk4->unk0;
}

CWorkThread* CWorkUtil::getWorkThread(WORK_ID wid){
    return CWorkThread::getWorkThread(wid);
}

void CWorkUtil::dispTree(const CWorkThread* pThread, int indent){
    if(pThread == nullptr){
        return;
    }

    ml::FixStr<64> text;

    // The local keeps MWCC's lis/@l address materialisation in a saved
    // register with mr copies at each use (retail shape); referencing the
    // extern inline merges the LO into the first use (addi rD,rS,0).
    const char* pPad = lbl_eu_805224C0;

    for(int i = 0; i < indent; i++){
        text += pPad;
    }

    text += pThread->mName.c_str();

    for(reslist<CWorkThread*>::iterator it = pThread->mChildren.begin(); it != pThread->mChildren.end(); it++){
        dispTree(*it, indent + 1);
    }
}

#pragma push
#pragma optimize_for_size on
void CWorkUtil::entryWork(CWorkThread* pChild, CWorkThread* pParent, bool prepend){
    if(pChild != nullptr){
        CWorkRoot::entryWork(pChild, pParent, prepend);

        if(pParent != nullptr && pParent->isNoEvent()){
            pChild->wkSetEvent(CWorkThread::EVT_NONE);
        }
    }
}
#pragma pop

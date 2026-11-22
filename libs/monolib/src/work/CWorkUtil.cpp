#include "monolib/work.hpp"
#include "monolib/util.hpp"

UnkStruct_80438AF0_3* CWorkUtil::getData(UnkStruct_80438AF0* arg0){
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

    const char* pPad;
    int i;

    i = 0;

    if(indent > 0){
        pPad = " ";

        for(; i < indent; i++){
            text += pPad;
        }
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

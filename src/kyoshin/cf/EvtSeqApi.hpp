#pragma once

#include <types.h>

// Defined in kyoshin/cf/CTaskREvtSequence.cpp. C++ linkage: these signatures
// produce the retail mangled symbol names.
void EvtSeqPublishIdHalfwords();
u32 EvtSeqGetStateBit10();
u32 EvtSeqGetEntryLimit();
int EvtSeqGetCounter104();
int EvtSeqGetWalkIndex();
void EvtSeqSetStateBit9();
void EvtSeqClearStateBit9();
u32 EvtSeqGetC4FlagBit1();
void EvtSeqSetStateBit18(bool flag);
u32 EvtSeqGetStateBit19();
int EvtSeqCheckWalkGate(u8* gate);
void EvtSeqDtorThunk54(void* self);
void EvtSeqRenderThunk58(void* self);

// Unlike the functions above, this one has C linkage in retail (unmangled symbol
// EvtSeqGetCounter100). Retail callers compare its result signed (cmpw), so it returns int.
extern "C" int EvtSeqGetCounter100();

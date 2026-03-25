#include "monolib/vm/yvm_debug.h"
#include "monolib/vm/yvm_opcode_data.h"
#include "monolib/vm/yvm2.h"
#include <stdio.h>

//All stubbed for release
//Debug only code comes from XCX

#if defined(DEBUG)
static char codeInfoBuffer[128];
#endif

void vmCodePut(VMThread* pThread, u8 code){
    //Commented out until yvm2 is matched since this requires vmDataGet to not be inlined
    /*
#if defined(DEBUG)
    VMCOpcode* opcode = &vmcOpcodes[code];
    u32 paramVal = vmDataGet(pThread, pThread->reg.pc + 1, vmcOpcodes[code].paramSize);
    int offset = sprintf(codeInfoBuffer,"\t%05X#%02X:%05X: %02X ", pThread, pThread->unk2C, pThread->reg.pc, code);
    char* bufferPtr = codeInfoBuffer + offset;

    if(vmcOpcodes[code].paramSize == 1){
        int argOffset = sprintf(bufferPtr, "%02X ", paramVal);
        sprintf(bufferPtr + argOffset, "%s ", opcode->name);
    }else if(vmcOpcodes[code].paramSize == 2){
        int argOffset = sprintf(bufferPtr, "%04X ", paramVal);
        sprintf(bufferPtr + argOffset, "%s ", opcode->name);
    }else{
        sprintf(bufferPtr, "%s ", opcode->name);
    }
#endif
    */
}

//XCX has some leftover code for this function, but only an empty loop
void vmStackDump(VMThread* pThread){
}

//Unfortuntely, both of these are also stubbed in XCX...
void vmPackageDump(){
}

void vmThreadDump(){
}

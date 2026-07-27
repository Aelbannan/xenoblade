#pragma once

#include <types.h>

namespace cf{

    class CfObjectSelectorObj{
    public:        
        virtual ~CfObjectSelectorObj();

        static void create();
        static void destroy();

    private:
        static CfObjectSelectorObj* spInstance;
        
        char _pad_00[0x608C - 0x0000]; // 0x0000-0x608B
        u32 mField608C;                  // 0x608C
        u32 _pad_6090[0x6094 - 0x6090];  // 0x6090
        u32 mField6094;                  // 0x6094
        u32 mField6098;                  // 0x6098
        char _pad_609C[0x90E8 - 0x609C]; // 0x609C-0x90E7
        u32 mField90E8;                  // 0x90E8
        u32 mField90EC;                  // 0x90EC
        char _pad_90F0[0x90F8 - 0x90F0]; // 0x90F0-0x90F7
        float mField90F8;                // 0x90F8
        char _pad_90FC[0xC164 - 0x90FC]; // 0x90FC-0xC163
        float mFieldC164;                // 0xC164
        char _pad_C168[0xC178 - 0xC168]; // 0xC168-0xC177
        void* mPtrC178;                  // 0xC178
        char _pad_C17C[0xC180 - 0xC17C]; // 0xC17C-0xC17F
        u32 mFieldC180;                  // 0xC180
        char _pad_C184[0xC188 - 0xC184]; // 0xC184-0xC187
    }; //size = 0xC188

} //namespace cf

#include "monolib/work.hpp"
#include "monolib/util.hpp"

// Static data members are owned by the retail data blob (monolibdata*.s);
// declare them extern under the retail symbol names so this TU defines no
// data. Addresses from symbols.txt:
//   lbl_eu_80663518 (.sdata) = scRegionName   (const char*)
//   lbl_eu_8066351C (.sdata) = sAllocHandle   (mtl::ALLOC_HANDLE)
//   lbl_eu_80665590 (.sbss)  = sMemAvailable  (BOOL)
//   lbl_eu_80665594 (.sbss)  = sAllocFlags    (u32*)
//   lbl_eu_80665598 (.sbss)  = sWorkThreads   (CWorkThread**)
extern "C" {
    extern const char* lbl_eu_80663518;
    extern u32 lbl_eu_8066351C;
    BOOL lbl_eu_80665590;              // sMemAvailable - defined here (blob monolibdata1d dissolve)
    extern u32* lbl_eu_80665594;       // sAllocFlags - defined in CWorkThread.cpp (owner)
    extern CWorkThread** lbl_eu_80665598[2];  // sWorkThreads - 8-byte sbss (word 0 in use), defined in CWorkThread.cpp (owner)
}

void CWorkThreadSystem::initialize(){
    lbl_eu_8066351C = mtl::MemManager::create(mtl::MemManager::getHandleMEM1(), REGION_SIZE, lbl_eu_80663518);
    lbl_eu_80665590 = true;

    lbl_eu_80665594 = new (lbl_eu_8066351C) u32[ALLOC_FLAGS_COUNT];
    lbl_eu_80665598[0] =
        static_cast<CWorkThread**>(mtl::MemManager::allocate_head(lbl_eu_8066351C, sizeof(CWorkThread*) * MAX_WORK_ID, 4));

    for(WORK_ID i = 0; i < ALLOC_FLAGS_COUNT; i++){
        lbl_eu_80665594[i] = 0;
    }
}

WORK_ID CWorkThreadSystem::allocWID(CWorkThread* thread){
    //Find the first set of alloc flags with a free bit
    for(u32 flag = 0; flag < ALLOC_FLAGS_COUNT; flag++){

        //All bits set means all IDs allocated
        if(lbl_eu_80665594[flag] != 0xFFFFFFFF){

            //Find the free bit in the flagset
            u32 mask = 1 << 0;
            u32 flags = lbl_eu_80665594[flag];

            for(u32 i = 0; i < 32; i++, mask <<= 1){
                //ID is already allocated
                if(flags & mask){
                    continue;
                }

                //Convert bitset & bit index to an absolute index
                WORK_ID wid = flag * 32 + i;

                //Save thread information
                lbl_eu_80665594[flag] |= mask;
                lbl_eu_80665598[0][wid] = thread;

                return wid;
            }

            break;
        }
    }

    return INVALID_WORK_ID;
}

void CWorkThreadSystem::destroy(){
    DELETE_ARRAY(lbl_eu_80665594);
    DELETE_OBJ(lbl_eu_80665598[0]);

    mtl::MemManager::erase(lbl_eu_8066351C);
    lbl_eu_8066351C = mtl::INVALID_HANDLE;

    lbl_eu_80665590 = false;
}

mtl::ALLOC_HANDLE CWorkThreadSystem::getWorkMem(){
    return lbl_eu_8066351C;
}

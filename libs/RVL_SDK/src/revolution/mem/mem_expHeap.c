#include <revolution/MEM.h>

#define MEM_EXP_HEAP_MAGIC FOURCC('E', 'X', 'P', 'H')
#define MEM_EXP_HEAP_MBLOCK_FREE TWOCC('F', 'R')
#define MEM_EXP_HEAP_MBLOCK_USED TWOCC('U', 'D')

// Size of base and expandable heap head
#define MEM_EXP_HEAP_HEAD_SIZE (sizeof(MEMiHeapHead) + sizeof(MEMiExpHeapHead))

typedef struct MemRegion {
    void* start;
    void* end;
} MemRegion;

static MEMiExpHeapHead*
GetExpHeapHeadPtrFromHeapHead_(const MEMiHeapHead* heap) {
    return AddU32ToPtr(heap, sizeof(MEMiHeapHead));
}

static MEMiHeapHead*
GetHeapHeadPtrFromExpHeapHead_(const MEMiExpHeapHead* exp) {
    return SubU32ToPtr(exp, sizeof(MEMiHeapHead));
}

static void SetAllocMode_(MEMiExpHeapHead* exp, u16 mode) {
    exp->allocMode = mode;
}

static u16 GetAllocMode_(const MEMiExpHeapHead* exp) {
    return exp->allocMode;
}

static MEMiExpHeapMBlock* GetMBlockHeadPtr_(void* memBlock) {
    return SubU32ToPtr(memBlock, sizeof(MEMiExpHeapMBlock));
}

static const MEMiExpHeapMBlock* GetMBlockHeadCPtr_(const void* memBlock) {
    return SubU32ToCPtr(memBlock, sizeof(MEMiExpHeapMBlock));
}

static void* GetMemPtrForMBlock_(MEMiExpHeapMBlock* mblock) {
    return AddU32ToPtr(mblock, sizeof(MEMiExpHeapMBlock));
}

static void* GetMBlockEndAddr_(MEMiExpHeapMBlock* mblock) {
    return AddU32ToPtr(GetMemPtrForMBlock_(mblock), mblock->size);
}

static s32 GetAlignmentForMBlock_(const MEMiExpHeapMBlock* mblock) {
    return mblock->align;
}

static inline MEMiExpHeapMBlock* RemoveMBlock_(MEMiExpHeapMBlockList* list,
                                               MEMiExpHeapMBlock* mblock) {
    MEMiExpHeapMBlock* const prev = mblock->prev;
    MEMiExpHeapMBlock* const next = mblock->next;

    if (prev != NULL) {
        prev->next = next;
    } else {
        list->head = next;
    }

    if (next != NULL) {
        next->prev = prev;
    } else {
        list->tail = prev;
    }

    return prev;
}

static inline MEMiExpHeapMBlock* InsertMBlock_(MEMiExpHeapMBlockList* list,
                                              MEMiExpHeapMBlock* target,
                                              MEMiExpHeapMBlock* prev) {
    MEMiExpHeapMBlock* next;

    target->prev = prev;
    if (prev != NULL) {
        next = prev->next;
        prev->next = target;
    } else {
        next = list->head;
        list->head = target;
    }
    target->next = next;
    if (next != NULL) {
        next->prev = target;
    } else {
        list->tail = target;
    }
    return target;
}

static inline void AppendMBlock_(MEMiExpHeapMBlockList* list,
                                 MEMiExpHeapMBlock* block) {
    (void)InsertMBlock_(list, block, list->tail);
}

static void GetRegionOfMBlock_(MemRegion* region, MEMiExpHeapMBlock* mblock) {
    region->start = SubU32ToPtr(mblock, GetAlignmentForMBlock_(mblock));
    region->end = GetMBlockEndAddr_(mblock);
}

static MEMiExpHeapMBlock* InitMBlock_(MemRegion* region, u16 state) {
    MEMiExpHeapMBlock* mblock = (MEMiExpHeapMBlock*)region->start;

    mblock->state = state;
    mblock->settings = 0;
    mblock->size = GetOffsetFromPtr(GetMemPtrForMBlock_(mblock), region->end);
    mblock->prev = NULL;
    mblock->next = NULL;

    return mblock;
}

static inline MEMiExpHeapMBlock* InitFreeMBlock_(MemRegion* region) {
    return InitMBlock_(region, MEM_EXP_HEAP_MBLOCK_FREE);
}

static MEMiHeapHead* InitExpHeap_(MEMiHeapHead* heap, void* end, u16 opt) {
    MEMiExpHeapMBlock* mblock;
    MEMiExpHeapHead* exp;
    MemRegion region;

    exp = GetExpHeapHeadPtrFromHeapHead_(heap);
    MEMiInitHeapHead(heap, MEM_EXP_HEAP_MAGIC,
                     AddU32ToPtr(heap, MEM_EXP_HEAP_HEAD_SIZE), end, opt);
    exp->group = 0;
    exp->SHORT_0x12 = 0;
    SetAllocMode_(exp, MEM_EXP_HEAP_ALLOC_FAST);

    region.start = heap->start;
    region.end = heap->end;

    mblock = InitFreeMBlock_(&region);
    exp->freeMBlocks.head = mblock;
    exp->freeMBlocks.tail = mblock;

    exp->usedMBlocks.head = NULL;
    exp->usedMBlocks.tail = NULL;

    return heap;
}

static void* AllocUsedBlockFromFreeBlock_(MEMiExpHeapHead* exp,
                                          MEMiExpHeapMBlock* freeBlock,
                                          void* memPtr, u32 size,
                                          u16 allocDir) {
    MemRegion regionL;
    MemRegion regionR;
    MemRegion usedRegion;
    MEMiExpHeapMBlock* prev;
    MEMiExpHeapMBlock* usedBlock;

    GetRegionOfMBlock_(&regionL, freeBlock);
    regionR.end = regionL.end;
    regionR.start = AddU32ToPtr(memPtr, size);
    regionL.end = SubU32ToPtr(memPtr, sizeof(MEMiExpHeapMBlock));

    prev = RemoveMBlock_(&exp->freeMBlocks, freeBlock);

    if ((GetOffsetFromPtr(regionL.start, regionL.end) <
         sizeof(MEMiExpHeapMBlock) + 4) ||
        (allocDir == 0 && !exp->useMarginOfAlign)) {
        regionL.end = regionL.start;
    } else {
        prev = InsertMBlock_(&exp->freeMBlocks, InitFreeMBlock_(&regionL), prev);
    }

    if ((GetOffsetFromPtr(regionR.start, regionR.end) <
         sizeof(MEMiExpHeapMBlock) + 4) ||
        (allocDir == 1 && !exp->useMarginOfAlign)) {
        regionR.start = regionR.end;
    } else {
        InsertMBlock_(&exp->freeMBlocks, InitFreeMBlock_(&regionR), prev);
    }

    FillAllocMemory(GetHeapHeadPtrFromExpHeapHead_(exp), regionL.end,
                    GetOffsetFromPtr(regionL.end, regionR.start));

    usedRegion.start = SubU32ToPtr(memPtr, sizeof(MEMiExpHeapMBlock));
    usedRegion.end = regionR.start;
    usedBlock = InitMBlock_(&usedRegion, MEM_EXP_HEAP_MBLOCK_USED);
    usedBlock->allocDir = allocDir;
    usedBlock->align =
        (u16)(GetUIntPtr(usedBlock) - GetUIntPtr(regionL.end));
    usedBlock->group = (u8)exp->group;
    AppendMBlock_(&exp->usedMBlocks, usedBlock);

    return memPtr;
}

static void* AllocFromHead_(MEMiHeapHead* heap, u32 size, s32 align) {
    MEMiExpHeapHead* exp = GetExpHeapHeadPtrFromHeapHead_(heap);
    const BOOL bAllocFirst = GetAllocMode_(exp) == MEM_EXP_HEAP_ALLOC_FAST;
    MEMiExpHeapMBlock* it = NULL;
    MEMiExpHeapMBlock* found = NULL;
    u32 foundSize = 0xFFFFFFFF;
    void* foundMem = NULL;

    for (it = exp->freeMBlocks.head; it; it = it->next) {
        void* const start = GetMemPtrForMBlock_(it);
        void* const alignStart = ROUND_UP_PTR(GetUIntPtr(start), align);
        const u32 padding = GetOffsetFromPtr(start, alignStart);

        if (it->size >= size + padding && foundSize > it->size) {
            found = it;
            foundSize = it->size;
            foundMem = alignStart;

            if (bAllocFirst || foundSize == size) {
                break;
            }
        }
    }

    if (!found) {
        return NULL;
    }

    return AllocUsedBlockFromFreeBlock_(exp, found, foundMem, size, 0);
}

static void* AllocFromTail_(MEMiHeapHead* heap, u32 size, s32 align) {
    MEMiExpHeapHead* exp = GetExpHeapHeadPtrFromHeapHead_(heap);
    const BOOL bAllocFirst = GetAllocMode_(exp) == MEM_EXP_HEAP_ALLOC_FAST;
    MEMiExpHeapMBlock* it = NULL;
    MEMiExpHeapMBlock* found = NULL;
    u32 foundSize = 0xFFFFFFFF;
    void* foundMem = NULL;

    for (it = exp->freeMBlocks.tail; it; it = it->prev) {
        void* const start = GetMemPtrForMBlock_(it);
        void* const end = AddU32ToPtr(start, it->size);
        void* const alignEnd =
            ROUND_DOWN_PTR(GetUIntPtr(SubU32ToPtr(end, size)), align);

        if (ComparePtr(alignEnd, start) >= 0 && foundSize > it->size) {
            found = it;
            foundSize = it->size;
            foundMem = alignEnd;

            if (bAllocFirst || foundSize == size) {
                break;
            }
        }
    }

    if (!found) {
        return NULL;
    }

    return AllocUsedBlockFromFreeBlock_(exp, found, foundMem, size, 1);
}

static BOOL RecycleRegion_(MEMiExpHeapHead* exp, const MemRegion* region) {
    MEMiExpHeapMBlock* blockFree = NULL;
    MemRegion freeRgn = *region;
    MEMiExpHeapMBlock* block;

    for (block = exp->freeMBlocks.head; block != NULL; block = block->next) {
        if (block < region->start) {
            blockFree = block;
            continue;
        }

        if (block == region->end) {
            freeRgn.end = GetMBlockEndAddr_(block);
            (void)RemoveMBlock_(&exp->freeMBlocks, block);
        }
        break;
    }

    if (blockFree != NULL && GetMBlockEndAddr_(blockFree) == region->start) {
        freeRgn.start = blockFree;
        blockFree = RemoveMBlock_(&exp->freeMBlocks, blockFree);
    }

    if (GetOffsetFromPtr(freeRgn.start, freeRgn.end) <
        sizeof(MEMiExpHeapMBlock)) {
        return FALSE;
    }

    (void)InsertMBlock_(&exp->freeMBlocks, InitFreeMBlock_(&freeRgn),
                        blockFree);
    return TRUE;
}

MEMiHeapHead* MEMCreateExpHeapEx(void* start, u32 size, u16 opt) {
    void* end = AddU32ToPtr(start, size);

    // Heap range
    end = ROUND_DOWN_PTR(end, 4);
    start = ROUND_UP_PTR(start, 4);

    // Ensure valid heap
    if (GetUIntPtr(start) > GetUIntPtr(end) ||
        GetOffsetFromPtr(start, end) < MEM_EXP_HEAP_MIN_SIZE) {
        return NULL;
    }

    return InitExpHeap_(start, end, opt);
}

MEMiHeapHead* MEMDestroyExpHeap(MEMiHeapHead* heap) {
    MEMiFinalizeHeap(heap);
    return heap;
}

void* MEMAllocFromExpHeapEx(MEMiHeapHead* heap, u32 size, s32 align) {
    void* memBlock;

    if (size == 0) {
        size = 1;
    }
    size = ROUND_UP(size, 4);

    LockHeap(heap);

    // Alignment sign determines alloc direction
    if (align >= 0) {
        memBlock = AllocFromHead_(heap, size, align);
    } else {
        memBlock = AllocFromTail_(heap, size, -align);
    }

    UnlockHeap(heap);

    return memBlock;
}

void MEMFreeToExpHeap(MEMiHeapHead* heap, void* memBlock) {
    MEMiExpHeapHead* exp;
    MEMiExpHeapMBlock* mblock;
    MemRegion region;

    if (memBlock == NULL) {
        return;
    }

    LockHeap(heap);

    exp = GetExpHeapHeadPtrFromHeapHead_(heap);
    mblock = GetMBlockHeadPtr_(memBlock);

    GetRegionOfMBlock_(&region, mblock);
    (void)RemoveMBlock_(&exp->usedMBlocks, mblock);
    (void)RecycleRegion_(exp, &region);

    UnlockHeap(heap);
}

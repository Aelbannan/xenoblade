#pragma once

#include <types.h>

// Canonical C-linkage declarations of the LOD tables owned by
// monolib/src/lod/CLODCacheManagerS.cpp.  The pointee layouts live with the
// owner; callers only need the opaque types and the retail symbol names.
namespace LOD {
struct CLODCacheManagerS;
struct LODDistEntry;
struct LODLevelEntry;
struct LODShortEntry;
struct LODCacheIndex;
}

extern "C" LOD::CLODCacheManagerS* sLodCacheRecords;  // cache records, stride 0xC
extern "C" LOD::LODDistEntry* sLodDistTable;          // distance table, stride 8
extern "C" u32* sLodPairOffsets;                      // index -> pair-table offset
extern "C" u16* sLodPairTable;                        // pair table, stride 2
extern "C" LOD::LODLevelEntry* sLodLevelTable;        // level table, stride 0x10
extern "C" LOD::LODShortEntry* sLodShortTable;        // short table, stride 8
extern "C" LOD::LODCacheIndex* sLodCacheIndexTable;   // distance-key index table

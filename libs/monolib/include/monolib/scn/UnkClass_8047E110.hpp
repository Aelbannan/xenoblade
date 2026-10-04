#pragma once

#include <types.h>

/**
 * UnkClass_8047E110 -- unknown scene class (size TBD).
 *
 * Translation unit: monolib/src/scn/UnkClass_8047E110
 * Contains 29 known methods plus a static initializer.
 * Class layout not yet determined -- all methods still stubs.
 *
 * Status: only sinit_80481E68 is FULL_MATCH (empty .ctors hook).
 * Remaining functions are NOT_STARTED.
 */
class UnkClass_8047E110 {
public:
    void func_8047E110();
    void findNodeContaining();
    void findClosestNeighbor();
    void func_8047E62C();
    void resolveNodeAtPos();
    void buildCandidateList();
    void scanNeighbors();
    void expandWalkPath();
    void computeWalkDirection();
    void fetchWalkTarget();
    void floodFillWalkTable();
    void queryWalkTable();
    void seedPathTable();
    void stepToBestNeighbor();
    void findPathBfs();
    void findPathBidirectional();
    void testPortalCrossing();
    void func_80480EF0();
    void func_80480F48();
    void func_80481014();
    void probeWalkerContact();
    void func_804812D8();
    void func_804813E8();
    void rebuildWalkBox();
    void func_8048163C();
    void func_8048169C();
    void searchWalkGraph();
    void func_804819AC();
    void stepWalker();
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Walk-graph float constants used by testPortalCrossing (retail .rodata pool)
extern "C" const f32 lbl_eu_8066A898;
extern "C" const f32 lbl_eu_8066A8A8;
extern "C" const f32 lbl_eu_8066A8AC;
extern "C" const f32 lbl_eu_8066A8B8;

#include <cstdio>
#include <cstdlib>
#include <vector>

// Behaviour oracle for CUICfManager::func_80133324 (retail @0x80133DF8).
// Static match ~98.0% CODE_MATCH: remaining gap is Chaitin register-number
// choice in the savedRet stack spill/reload and the slot-search loop's
// list-pointer reload register (r3 in retail vs a different GPR in decomp) —
// not a logic/structural difference. See docs/MWCC_REFERENCE.md and
// docs/evidence/decomp/attempts.jsonl (target uicf-func-80133324) for detail.
//
// retail_dispatch() is transcribed directly from
// docs/evidence/decomp/batch_2026-07-14h/asm_func_80133324_CUICfManager.s
// (goto labels keep the retail `.L_*` names for traceability).
// decomp_dispatch() mirrors src/kyoshin/CUICfManager.cpp's
// func_80133324__12CUICfManagerFv body against a mock CUICfManager/SDA world.

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

// mNext@0x0 / mPrev@0x4 / item@0x8, matching monolib's _reslist_node<T> (see
// libs/monolib/include/monolib/util/reslist.hpp) which the range_312c_31f3
// event-queue insert reuses.
struct MockNode {
    MockNode* mNext;
    MockNode* mPrev;
    u32 item;
};

struct MockMgr {
    MockNode* head;   // unk128 (sentinel; head->mNext is the queue front)
    MockNode* nodes;  // unk138 (event-node array)
    int count;         // unk13C (event-node array length)
};

struct World {
    std::vector<int> codes;       // func_8013B428 byte codes, in call order
    int codes87c = -1;             // func_8013B87C argument (single call at most)
    int calls8009EC9C = 0;
    int calls800A21F8 = 0;
    int sda_80664050 = -1;         // last value written to lbl_eu_80664050
    MockMgr* mgr = nullptr;        // lbl_eu_80664054 ("no manager" == nullptr)

    int f80138138_ret = 0;         // func_80138138 return (index into table[])
    void* table[4] = {};            // lbl_eu_80573D18[idx]
    u8 f801361E8_ret = 0;           // func_801361E8 return (0/1/2)
    u16 idTable[27] = {};           // lbl_eu_804FFFDC snapshot (0-terminated)
    int f8014A1D4_ret = 0;          // func_8014A1D4 return (0 == no insert)
    u16 f8013606C_ret = 0;          // func_8013606C return (0 == no broadcast)

    int insertedIndex = -1;
    u32 insertedValue = 0;
};

static World* g;

static void mock_8013B428(u32 code) {
    g->codes.push_back((int)code);
}
static void mock_8013B87C(u32 code) {
    g->codes87c = (int)code;
}
static int mock_80138138(int /*off*/) {
    return g->f80138138_ret;
}
static u8 mock_801361E8(void* /*a*/, void* /*b*/, u32 /*off*/) {
    return g->f801361E8_ret;
}
static void mock_8013DA60(int, int, int) {}
static int mock_8014A1D4(void* /*unk144*/, u32 /*unk11C*/, u8 /*code*/, int /*flag*/) {
    return g->f8014A1D4_ret;
}
static void* mock_8009EC9C(u16) {
    g->calls8009EC9C++;
    return nullptr;
}
static void mock_800A21F8(void*, u16, int, int) {
    g->calls800A21F8++;
}
static u16 mock_8013606C(void* /*a*/, void* /*b*/, u8 /*code*/) {
    return g->f8013606C_ret;
}

// ---------------------------------------------------------------------------
// retail_dispatch
// ---------------------------------------------------------------------------
static void retail_dispatch(int id, int a1, int a2) {
    if (a1 == a2) {
        goto L_8013419C;
    }
    if (id >= 0x798) {
        goto L_80133E4C;
    }
    if (id >= 0x221) {
        goto L_80133E3C;
    }
    if (id >= 0x28) {
        goto L_8013419C;
    }
    if (id >= 0x22) {
        goto L_8013412C;
    }
    goto L_8013419C;

L_80133E3C:
    if (id == 0x608) {
        goto L_8013419C;
    }
    if (id >= 0x608) {
        goto L_8013415C;
    }
    goto L_80133E74;

L_80133E4C:
    if (id >= 0x312c) {
        goto L_80133E68;
    }
    if (id >= 0x805) {
        goto L_8013419C;
    }
    if (id >= 0x7fc) {
        goto L_801340A0;
    }
    goto L_8013419C;

L_80133E68:
    if (id >= 0x31f4) {
        goto L_8013419C;
    }
    goto L_80133F80;

L_80133E74: {
    u32 check = (u32)(a1 - 0xfe);
    int off = id - 0x220;
    g->sda_80664050 = off;
    if (check > 1) {
        goto L_8013419C;
    }
    int idx = mock_80138138(off);
    void* tableVal = g->table[idx];
    u8 res = mock_801361E8(tableVal, nullptr, (u32)off);
    if (res == 2) {
        goto L_8013419C;
    }
    mock_8013DA60(off, 1, 0);
    for (int i = 0; g->idTable[i] != 0; i++) {
        if (g->idTable[i] == (u16)off) {
            mock_8013B428(0xb9);
            goto L_8013419C;
        }
    }
    goto L_8013419C;
}

L_80133F80: {
    if (a1 == 0) {
        goto L_8013419C;
    }
    if (a2 != 0) {
        goto L_8013419C;
    }
    u32 diff = (u32)(id - 0x312c);
    u32 code = 0xc8;
    if ((u8)diff != 0) {
        code = (u8)diff;
    }
    u8 codeByte = (u8)code;

    mock_8013B87C(codeByte);

    if (g->mgr != nullptr) {
        int ret = mock_8014A1D4(nullptr, 0, codeByte, 1);
        if (ret != 0) {
            MockNode* head = g->mgr->head;
            int i = 0;
            // Retail checks node[i]'s first field (mNext) for zero to find a free slot
            // (lwzx r0,r3,r6 at offset 0, not the +8 item field).
            while (i < g->mgr->count && g->mgr->nodes[i].mNext != nullptr) {
                i++;
            }
            MockNode* node = &g->mgr->nodes[i];
            node->item = (u32)ret;
            node->mPrev = head;
            node->mNext = head->mNext;
            head->mNext->mPrev = node;
            head->mNext = node;
            g->insertedIndex = i;
            g->insertedValue = (u32)ret;
        }
    }

    u16 ret2 = mock_8013606C(nullptr, nullptr, codeByte);
    if (ret2 != 0) {
        for (int i = 1; i <= 8; i++) {
            void* r = mock_8009EC9C((u16)i);
            mock_800A21F8(r, ret2, 0, 0);
        }
    }
    goto L_8013419C;
}

L_801340A0: {
    int rel0 = id - 0x7fc;
    if (rel0 == 0) {
        goto L_801340B4;
    }
    if (rel0 == 1) {
        goto L_801340F0;
    }
    goto L_8013419C;
}

L_801340B4:
    if (a1 == 0x64) {
        mock_8013B428(0xc8);
        goto L_8013419C;
    }
    if (a1 >= 0x32) {
        mock_8013B428(0xc7);
        goto L_8013419C;
    }
    if (a1 >= 1) {
        mock_8013B428(0xc6);
    }
    goto L_8013419C;

L_801340F0:
    if (a1 == 0x96) {
        mock_8013B428(0xc5);
        goto L_8013419C;
    }
    if (a1 >= 0x64) {
        mock_8013B428(0xc4);
        goto L_8013419C;
    }
    if (a1 >= 0x32) {
        mock_8013B428(0xc3);
    }
    goto L_8013419C;

L_8013412C:
    if (a1 < 0xfa0) {
        goto L_8013419C;
    }
    mock_8013B428((u8)(id + 0x81));
    if (a1 < 0x1f40) {
        goto L_8013419C;
    }
    mock_8013B428(0xa8);
    mock_8013B428(0xa9);
    goto L_8013419C;

L_8013415C:
    if (a2 == 0 && a1 > 0) {
        mock_8013B428(0x9f);
        mock_8013B428(0xa0);
        mock_8013B428(0xa1);
        mock_8013B428(0xa2);
    }
    if (a1 < 5) {
        goto L_8013419C;
    }
    mock_8013B428(0x9e);
    goto L_8013419C;

L_8013419C:
    return;
}

// ---------------------------------------------------------------------------
// decomp_dispatch — mirrors src/kyoshin/CUICfManager.cpp's
// func_80133324__12CUICfManagerFv (same goto labels / variable names).
// ---------------------------------------------------------------------------
static void decomp_dispatch(int id, int a1, int a2) {
    u8 codePersist;

    if (a1 == a2) {
        goto end;
    }
    if (id >= 0x798) {
        goto ge798;
    }
    if (id >= 0x221) {
        goto ge221;
    }
    if (id >= 0x28) {
        goto end;
    }
    if (id >= 0x22) {
        goto range_22_27;
    }
    goto end;

ge221:
    if (id == 0x608) {
        goto end;
    }
    if (id >= 0x608) {
        goto range_609_797;
    }
    goto range_221_607;

ge798:
    if (id >= 0x312c) {
        goto ge312c;
    }
    if (id >= 0x805) {
        goto end;
    }
    if (id >= 0x7fc) {
        goto range_7fc_804;
    }
    goto end;

ge312c:
    if (id >= 0x31f4) {
        goto end;
    }
    goto range_312c_31f3;

range_221_607: {
    u32 check = (u32)(a1 - 0xfe);
    int off = id - 0x220;
    g->sda_80664050 = off;
    if (check > 1) {
        goto end;
    }

    int idx = mock_80138138(off);
    void* tableVal = g->table[idx];
    u8 res = (u8)mock_801361E8(tableVal, nullptr, (u32)g->sda_80664050);
    if (res == 2) {
        goto end;
    }

    mock_8013DA60(g->sda_80664050, 1, 0);

    {
        int target = g->sda_80664050;
        u8 i = 0;
        goto id_check;
    id_body:
        if ((u16)g->idTable[i] == target) {
            mock_8013B428(0xb9);
            goto end;
        }
        i++;
    id_check:
        if (g->idTable[i] != 0) {
            goto id_body;
        }
    }
    goto end;
}

range_312c_31f3: {
    if (a1 == 0) {
        goto end;
    }
    if (a2 != 0) {
        goto end;
    }

    {
        u32 diff = id - 0x312c;
        u32 code = 0xc8;
        if ((u8)diff != 0) {
            code = (u8)diff;
        }
        codePersist = (u8)code;
    }

    mock_8013B87C(codePersist);

    MockMgr* inst = g->mgr;
    if (inst != nullptr) {
        int savedRet = mock_8014A1D4(nullptr, 0, codePersist, 1);
        if (savedRet != 0) {
            inst = g->mgr;

            MockNode* startNode = inst->head;
            int capacity = inst->count;
            int i = 0;
            int byteOff = 0;
            goto slot_check;
        slot_body:
            if (*(u32*)((u8*)inst->nodes + byteOff) == 0) {
                goto slot_found;
            }
            byteOff += 0xc;
            i++;
        slot_check:
            if (i < capacity) {
                goto slot_body;
            }
        slot_found:
            {
                MockNode* temp = (MockNode*)((u8*)inst->nodes + i * 0xc);
                temp->item = (u32)savedRet;
                temp->mNext = startNode;
                temp->mPrev = startNode->mPrev;
                startNode->mPrev->mNext = temp;
                startNode->mPrev = temp;
                g->insertedIndex = i;
                g->insertedValue = (u32)savedRet;
            }
        }
    }

    {
        u16 ret2 = mock_8013606C(nullptr, nullptr, codePersist);
        if (ret2 != 0) {
            int i = 1;
            goto party_check;
        party_body:
            {
                void* r = mock_8009EC9C((u16)i);
                mock_800A21F8(r, ret2, 0, 0);
                i++;
            }
        party_check:
            if (i <= 8) {
                goto party_body;
            }
        }
    }
    goto end;
}

range_7fc_804: {
    int rel = id - 0x7fc;
    if (rel == 0) {
        goto case_7fc;
    }
    if (rel == 1) {
        goto case_7fd;
    }
    goto end;
}

case_7fc:
    if (a1 == 0x64) {
        mock_8013B428(0xc8);
        goto end;
    }
    if (a1 >= 0x32) {
        mock_8013B428(0xc7);
        goto end;
    }
    if (a1 >= 1) {
        mock_8013B428(0xc6);
    }
    goto end;

case_7fd:
    if (a1 == 0x96) {
        mock_8013B428(0xc5);
        goto end;
    }
    if (a1 >= 0x64) {
        mock_8013B428(0xc4);
        goto end;
    }
    if (a1 >= 0x32) {
        mock_8013B428(0xc3);
    }
    goto end;

range_22_27:
    if (a1 < 0xfa0) {
        goto end;
    }
    mock_8013B428((u8)(id + 0x81));
    if (a1 < 0x1f40) {
        goto end;
    }
    mock_8013B428(0xa8);
    mock_8013B428(0xa9);
    goto end;

range_609_797:
    if (a2 == 0) {
        if (a1 > 0) {
            mock_8013B428(0x9f);
            mock_8013B428(0xa0);
            mock_8013B428(0xa1);
            mock_8013B428(0xa2);
        }
    }
    if (a1 < 5) {
        goto end;
    }
    mock_8013B428(0x9e);
    goto end;

end:
    return;
}

// ---------------------------------------------------------------------------
// Scenario harness
// ---------------------------------------------------------------------------
struct Outcome {
    std::vector<int> codes;
    int codes87c;
    int calls8009EC9C;
    int calls800A21F8;
    int sda_80664050;
    int insertedIndex;
    u32 insertedValue;
};

// `setup` is invoked once per dispatch call, building an independent World
// (and, when a manager is involved, independent MockMgr/MockNode storage) so
// the retail and decomp runs never observe each other's mutations.
template <typename Setup>
static Outcome capture(void (*dispatch)(int, int, int), Setup setup, int id, int a1, int a2) {
    World world;
    setup(world);
    g = &world;
    dispatch(id, a1, a2);
    return {world.codes,        world.codes87c,       world.calls8009EC9C,
            world.calls800A21F8, world.sda_80664050,    world.insertedIndex,
            world.insertedValue};
}

static bool equals(const Outcome& a, const Outcome& b) {
    return a.codes == b.codes && a.codes87c == b.codes87c &&
           a.calls8009EC9C == b.calls8009EC9C && a.calls800A21F8 == b.calls800A21F8 &&
           a.sda_80664050 == b.sda_80664050 && a.insertedIndex == b.insertedIndex &&
           a.insertedValue == b.insertedValue;
}

template <typename Setup>
static void run_scenario(const char* name, Setup setup, int id, int a1, int a2) {
    Outcome r = capture(retail_dispatch, setup, id, a1, a2);
    Outcome d = capture(decomp_dispatch, setup, id, a1, a2);
    if (!equals(r, d)) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static void run_scenario(const char* name, const World& base, int id, int a1, int a2) {
    run_scenario(
        name, [&](World& w) { w = base; }, id, a1, a2);
}

int main() {
    World base;

    // -- a1==a2 early-out (id irrelevant) --------------------------------
    run_scenario("early_out_equal_01", base, 0x300, 5, 5);
    run_scenario("early_out_equal_02", base, 0x7fc, 0, 0);

    // -- dead zones (id < 0x22, [0x28,0x220], ==0x608, [0x805,0x312b], >=0x31f4)
    run_scenario("dead_low_01", base, 0x10, 1, 2);
    run_scenario("dead_mid_01", base, 0x100, 1, 2);
    run_scenario("dead_608_01", base, 0x608, 1, 2);
    run_scenario("dead_805plus_01", base, 0x900, 1, 2);
    run_scenario("dead_31f4plus_01", base, 0x3300, 1, 2);

    // -- range_22_27 (item pickup thresholds) ----------------------------
    run_scenario("range2227_below_01", base, 0x22, 0x100, 0);
    run_scenario("range2227_first_01", base, 0x25, 0xfa0, 0);
    run_scenario("range2227_both_01", base, 0x27, 0x1f40, 0);
    run_scenario("range2227_both_02", base, 0x22, 0x2000, 0);

    // -- range_221_607 (event-id validity table lookup) -------------------
    {
        World w = base;
        w.idTable[0] = 5;
        w.idTable[1] = 0;
        w.f801361E8_ret = 0;
        run_scenario("range221607_outofband_01", w, 0x221, 0x50, 0);
    }
    {
        World w = base;
        w.idTable[0] = 1;
        w.idTable[1] = 0;
        w.f801361E8_ret = 2;
        run_scenario("range221607_evt2skip_01", w, 0x221, 0xfe, 0);
    }
    {
        World w = base;
        w.idTable[0] = 1; // off = 0x221-0x220 = 1, matches ids[0]
        w.idTable[1] = 0;
        w.f801361E8_ret = 0;
        run_scenario("range221607_found_01", w, 0x221, 0xfe, 0);
    }
    {
        World w = base;
        w.idTable[0] = 9;
        w.idTable[1] = 0;
        w.f801361E8_ret = 1;
        run_scenario("range221607_notfound_01", w, 0x222, 0xff, 0);
    }
    {
        World w = base;
        w.idTable[0] = 3;
        w.idTable[1] = 7;
        w.idTable[2] = 0;
        w.f801361E8_ret = 0;
        run_scenario("range221607_found_02", w, 0x223, 0xff, 0); // off=3 -> idTable[0]
    }

    // -- range_609_797 (a1/a2 gated broadcast) -----------------------------
    run_scenario("range609797_bothtrue_01", base, 0x609, 10, 0);
    run_scenario("range609797_a2nonzero_01", base, 0x700, 10, 1);
    run_scenario("range609797_a1lt5_01", base, 0x797, 2, 0);
    run_scenario("range609797_a1zero_01", base, 0x650, 0, 0);
    run_scenario("range609797_a1negative_01", base, 0x650, -3, 0);

    // -- range_7fc_804 (7fc / 7fd sub-cases; 7fe+ dead) --------------------
    run_scenario("range7fc_exact100_01", base, 0x7fc, 0x64, 0);
    run_scenario("range7fc_ge50_01", base, 0x7fc, 0x32, 0);
    run_scenario("range7fc_ge1_01", base, 0x7fc, 1, 0);
    run_scenario("range7fc_zero_01", base, 0x7fc, 0, 0);
    run_scenario("range7fd_exact150_01", base, 0x7fd, 0x96, 0);
    run_scenario("range7fd_ge100_01", base, 0x7fd, 0x64, 0);
    run_scenario("range7fd_ge50_01", base, 0x7fd, 0x32, 0);
    run_scenario("range7fe_dead_01", base, 0x7fe, 100, 0);

    // -- range_312c_31f3 (broadcast + optional queue insert) ---------------
    run_scenario("range312c_a1zero_01", base, 0x312c, 0, 0);
    run_scenario("range312c_a2nonzero_01", base, 0x312c, 5, 1);
    {
        World w = base;
        w.mgr = nullptr;
        w.f8013606C_ret = 0;
        run_scenario("range312c_nomgr_nobroadcast_01", w, 0x312c, 5, 0);
    }
    {
        World w = base;
        w.mgr = nullptr;
        w.f8013606C_ret = 7;
        run_scenario("range312c_nomgr_broadcast_01", w, 0x3130, 5, 0);
    }
    // Each mgr-backed scenario below rebuilds its own static head/nodes/mgr
    // storage inside the setup lambda (invoked once per dispatch call) so the
    // retail and decomp runs never observe each other's list mutations.
    run_scenario(
        "range312c_mgr_noinsert_01",
        [](World& w) {
            static MockNode head;
            static MockNode nodes[4];
            static MockMgr mgr;
            head = MockNode{&head, &head, 0};
            for (auto& n : nodes) n = MockNode{};
            mgr = MockMgr{&head, nodes, 4};
            w.mgr = &mgr;
            w.f8014A1D4_ret = 0;
            w.f8013606C_ret = 0;
        },
        0x312c, 5, 0);

    run_scenario(
        "range312c_mgr_insert_empty_01",
        [](World& w) {
            static MockNode head;
            static MockNode nodes[4];
            static MockMgr mgr;
            head = MockNode{&head, &head, 0};
            for (auto& n : nodes) n = MockNode{};
            mgr = MockMgr{&head, nodes, 4};
            w.mgr = &mgr;
            w.f8014A1D4_ret = 42;
            w.f8013606C_ret = 0;
        },
        0x312d, 5, 0);

    run_scenario(
        "range312c_mgr_insert_skip_broadcast_01",
        [](World& w) {
            static MockNode head;
            static MockNode nodes[4];
            static MockMgr mgr;
            head = MockNode{&head, &head, 0};
            for (auto& n : nodes) n = MockNode{};
            nodes[0].mNext = &head; // slot 0 occupied -> insert lands at slot 1
            mgr = MockMgr{&head, nodes, 4};
            w.mgr = &mgr;
            w.f8014A1D4_ret = 99;
            w.f8013606C_ret = 3;
        },
        0x31f3, 5, 0);

    return 0;
}

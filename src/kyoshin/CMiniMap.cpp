// Auto-scaffolded catalog TU for kyoshin/CMiniMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMiniMap.hpp"
void func_8011C434(void* self) { reinterpret_cast<CMenuMiniMap2*>((char*)self - 0x58)->~CMenuMiniMap2(); }

void func_80117734(){}

void CMMTex::OnFileEvent() {}

void __ct__CMiniMap(){}

CMiniMap::~CMiniMap() {}

void func_80117C30(){}

void func_80118058(){}

void CMiniMap::OnFileEvent() {}

// ============================================================================
// func_80118854 - CMiniMap marker/map-icon update (retail symbol unmangled).
// Reconstructed from retail ASM; high-level C++ only.
// ============================================================================

#include <nw4r/lyt.h>
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// ---- retail data symbols (small-data / absolute addressing per retail) ----
extern "C" s32 lbl_eu_80663FBC;          // frame counter (periodic cleanup gate)
extern "C" u32 lbl_eu_80664184;          // current BDAT row index
extern "C" void* lbl_eu_80663FB8;        // current BDAT table pointer
extern "C" f32 lbl_eu_80667090;          // zero constant
extern "C" f64 lbl_eu_80667098;          // int->float magic double
extern "C" f32 lbl_eu_806670A0;          // marker clamp scale
extern "C" f32 lbl_eu_806670B8;          // marker clamp scale (2nd)
extern "C" u32 lbl_eu_8052C7B8[];        // pane-name table (periodic cleanup)
extern "C" void* lbl_eu_80573D18[];      // per-row BDAT entry table
extern "C" char lbl_eu_804FE1FC[];       // string pool (columns/formats/names)
extern "C" char lbl_eu_80526324[];       // Warning file name
extern "C" char lbl_eu_80526300[];       // Warning format
extern "C" char lbl_eu_8052CB40[];       // Panic file name
extern "C" char lbl_eu_8052CB1C[];       // Panic format

// ---- cross-TU helpers (retail unmangled symbols) ----
extern "C" {
void func_80043D90(void* holder);
void* func_80043F18(void* holder);
u32 func_8009CF8C(u32 resourceId);
u8 func_801361E8(u32, const char*, u32);
u16 func_80136254(void*, const char*, u32);
s16 func_80136330(void*, const char*, u32);
u32 func_8003B1EC(void*);
void __dt__80043E88(void* holder, s16);
void func_800F4A98(void* list, u32 type, u32 filter);
void* __ct__800FB044(void* list, f32, void* obj, int);
void* func_800BFC68__FPQ22cf12CfObjectMove(void* obj);
void* func_800B6CF8(int);
void* func_800B6C58();
void* func_800B6BEC();
void* func_800B6CC4();
u32 func_800F6E98(void* list, u32 index);
void* func_80193804();
void func_800ABC5C(ml::CVec3* out, void* obj);
void func_8045F7E8__17UnkClass_8045F564Fv(void* self);
void __ct__14Class_8045F858FP17UnkClass_8045F564(void* self, void* sub);
void __dt__14Class_8045F858Fv(void* self, int);
u32 func_801380A0(u16 row);
void* func_80138234(void* bdat, u32 row);
void* func_8013902C(int type);
nw4r::lyt::ArcResourceAccessor* func_801355F4();
void func_80137C1C(void* pic, s32 arg);
void func_80116B40(void* self);
void* func_801167EC(void* self);
void func_8011628C(void* self, u32 row);
void* func_800BF324(void* obj);
void* createPicture__10CLibLayoutFv();
void SetName__Q34nw4r3lyt4PaneFPCc(void* pane, const char* name);
void RemoveChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(void* parent, void* child);
void AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(void* parent, void* child);
void PrependChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(void* parent, void* child);
void deleteTextboxOrPicture__10CLibLayoutFv(void* pane);
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
f32 FrSqrt__Q24nw4r4mathFf(f32);
int sprintf(char*, const char*, ...);
}
// C++-mangled retail helper: actor id -> action source object.
void* func_800B708C(int id);

// ---- local SI types for retail virtual calls (fake SI iface pattern) ----
struct MiniMapSelf;
struct MiniMapPaneMgr {
    virtual void v000(); virtual void v004(); virtual void v008(); virtual void v00C();
    virtual void v010(); virtual void v014(); virtual void v018(); virtual void v01C();
    virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02C();
    virtual void v030(); virtual void v034();
    virtual void* v03C(const char* name, int create);  // vtable+0x3C
};
struct MiniMapSelf {
    void* vtable;                    // 0x00
    u8 pad04[0x0C - 0x04];
    struct MiniMapMgr {
        u8 pad[0x10];
        MiniMapPaneMgr* m10;         // 0x10
    }* m0C;                          // 0x0C
    u8 pad10[0x20 - 0x10];
    f32 m20;                         // 0x20 (marker grid scale)
    u32 m24;                         // 0x24 (BDAT table)
    u8 pad28[0x3C - 0x28];
    void* m3C;                       // 0x3C
    u8 pad40[0x17C - 0x40];
    u8 m17C[0x6B8];                  // 0x17C..0x834
    u8 m834[0x80];                   // 0x834
};
struct MiniMapObj {
    virtual void v000(); virtual void v004(); virtual void* v008(int);
    virtual void v00C(); virtual void v010(); virtual void v014(); virtual void v018();
    virtual void v01C(); virtual void v020(); virtual void v024(); virtual void v028();
    virtual void v02C(); virtual void v030(); virtual void v034(); virtual void v038();
    virtual void v03C(); virtual void v040(); virtual void v044(); virtual void v048();
    virtual void v04C(); virtual void v050(); virtual void v054(); virtual void v058();
    virtual void v05C(); virtual void v060(); virtual void v064(); virtual void v068();
    virtual void v06C(); virtual void v070(); virtual void v074(); virtual void v078();
    virtual void v07C(); virtual void v080(); virtual void v084(); virtual void v088();
    virtual void v08C(); virtual void v090(); virtual void v094(); virtual void v098();
    virtual void v09C(); virtual void v0A0(); virtual void v0A4();
    virtual ml::CVec3* GetPos();     // vtable+0xAC
    virtual void v0B0(); virtual void v0B4(); virtual void v0B8(); virtual void v0BC();
    virtual void v0C0(); virtual void v0C4(); virtual void v0C8(); virtual void v0CC();
    virtual void v0D0(); virtual void v0D4(); virtual void v0D8(); virtual void v0DC();
    virtual void v0E0(); virtual void v0E4(); virtual void v0E8(); virtual void v0EC();
    virtual void v0F0(); virtual void v0F4(); virtual void v0F8(); virtual void v0FC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void v130(); virtual void v134(); virtual void v138(); virtual void v13C();
    virtual void v140(); virtual void v144(); virtual void v148(); virtual void v14C();
    virtual void v150(); virtual void v154(); virtual void v158(); virtual void v15C();
    virtual void* v160();            // vtable+0x160 (valid check)
    virtual void v164(); virtual void v168(); virtual void v16C(); virtual void v170();
    virtual void v174(); virtual void v178(); virtual void v17C(); virtual void v180();
    virtual void v184(); virtual void v188(); virtual void v18C(); virtual void v190();
    virtual void v194(); virtual void v198(); virtual void v19C(); virtual void v1A0();
    virtual void v1A4(); virtual void v1A8(); virtual void v1AC(); virtual void v1B0();
    virtual void v1B4(); virtual void v1B8(); virtual void v1BC(); virtual void v1C0();
    virtual void v1C4(); virtual void v1C8(); virtual void v1CC(); virtual void v1D0();
    virtual void v1D4(); virtual void v1D8(); virtual void v1DC(); virtual void v1E0();
    virtual void v1E4(); virtual void v1E8(); virtual void v1EC(); virtual void v1F0();
    virtual void v1F4(); virtual void v1F8(); virtual void v1FC(); virtual void v200();
    virtual void v204(); virtual void v208(); virtual void v20C(); virtual void v210();
    virtual void v214(); virtual void v218(); virtual void v21C(); virtual void v220();
    virtual void v224();
    virtual int v228();              // vtable+0x228 (mode)
    u8 pad[0x64 - 0x04];
    u32 m64;                         // 0x64 flags
    u8 pad68[0x74 - 0x68];
    u32 m74;                         // 0x74 id (lwz)
    u8 pad78[0x8C - 0x78];
    u16 m8C;                         // 0x8C id (lhz)
    u8 pad8E[0x91 - 0x8E];
    u8 m91;                          // 0x91 kind
    u8 pad92[0xBB - 0x92];
    u8 mBB;                          // 0xBB flag
};
struct MiniMapDtorIf {
    virtual void v008(int);          // vtable+0x8 deleting dtor
};
struct MiniMapEnumHolder {
    void* list;                      // 0x00
    u32 handle;                      // 0x04
};
struct MiniMapEnumList {
    u8 pad[0x620];
    u32 count;                       // 0x620
};
struct MiniMapListNode {
    MiniMapListNode* next;           // 0x00
    u8 pad04[0x08 - 0x04];
    void* object;                    // 0x08
};
struct MiniMapList {
    void* field_00;                  // 0x00
    MiniMapListNode* head;           // 0x04
};
struct MiniMapCleanupNode {
    MiniMapCleanupNode* next;        // 0x00
    u8 pad04[0xB8 - 0x04];
    char name[0x40];                 // 0xB8
};
struct MiniMapCleanupList {
    u8 pad[0x14];
    MiniMapCleanupNode* first;       // 0x14 (sentinel == &first)
};
struct MiniMapCase2Elem {
    f32 x;                           // 0x00
    f32 y;                           // 0x04
    f32 z;                           // 0x08
    u8 pad0C[0x1C - 0x0C];
    u16 m1C;                         // 0x1C id
    u8 pad1E[0x4C - 0x1E];           // stride 76
};

// Shared marker-placement tail (inlined twice by retail with distinct locals).
// copy #1: diff1 @ sp+636, name1 @ sp+872, pane parent "panemapmark".
// copy #2: diff2 @ sp+624, name2 @ sp+840.
extern "C" void func_80118854(MiniMapSelf* self) {
    MiniMapEnumHolder holder;
    MiniMapEnumList* list;
    MiniMapObj* obj;
    f32 clamp;
    u8 type;
    u16 mapId;
    void* gimmickView;
    ml::CVec3 diff1;
    ml::CVec3 diff2;
    char name1[32];
    char name2[32];
    char buf3[32];
    char buf4[32];
    char buf5[32];
    char buf6[32];
    char buf7[32];
    char buf8[32];

    if (!self->m0C) return;
    if (!cf::CfGameManager::getPlayer(0)) return;
    obj = (MiniMapObj*)func_800BFC68__FPQ22cf12CfObjectMove(cf::CfGameManager::getPlayer(0));
    func_80043D90(&holder);
    list = (MiniMapEnumList*)func_80043F18(&holder);
    func_800F4A98(list, 0xB00, 0);
    {
        void* playerPos = ((MiniMapObj*)cf::CfGameManager::getPlayer(0))->GetPos();
        __ct__800FB044(func_80043F18(&holder), lbl_eu_806670B8 * self->m20, playerPos, 0);
    }
    if (lbl_eu_80663FBC++ > 30) {
        lbl_eu_80663FBC = 0;
        for (u8 i = 0; lbl_eu_8052C7B8[i] != 0; i++) {
            MiniMapCleanupList* clist =
                (MiniMapCleanupList*)self->m0C->m10->v03C((const char*)lbl_eu_8052C7B8[i], 1);
            MiniMapCleanupNode* node = clist->first;
            while (node != (MiniMapCleanupNode*)&clist->first) {
                if (!node) {
                    Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 573,
                                               (const char*)lbl_eu_8052CB1C);
                }
                MiniMapCleanupNode* next = node->next;
                void* pane = self->m0C->m10->v03C(node->name, 1);
                if (pane && *(void**)((u8*)pane + 0x0C)) {
                    RemoveChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                        *(void**)((u8*)pane + 0x0C), pane);
                    ((MiniMapDtorIf*)pane)->v008(-1);
                    deleteTextboxOrPicture__10CLibLayoutFv(pane);
                }
                node = next;
            }
        }
        func_80116B40((u8*)self + 0x17C);
        func_8045F7E8__17UnkClass_8045F564Fv((u8*)self + 0x834);
    }
    {
        u8 guard[0x10];
        __ct__14Class_8045F858FP17UnkClass_8045F564(guard, (u8*)self + 0x834);
        {
            u16 row = (u16)func_8009CF8C(32);
            if (row != 0) {
                if ((u8)func_801361E8(self->m24, &lbl_eu_804FE1FC[649], row) ==
                    (u8)lbl_eu_80664184) {
                    clamp = lbl_eu_806670A0 * self->m20;
                    type = func_801361E8(self->m24, &lbl_eu_804FE1FC[64], row);
                    mapId = func_80136254((void*)(u32)self->m24, &lbl_eu_804FE1FC[69], row);
                    if (mapId == 0) {
                        diff1.x = lbl_eu_80667090;
                        name1[0] = 0;
                        diff1.y = lbl_eu_80667090;
                        diff1.z = lbl_eu_80667090;
                        if ((int)type == 1) {
                            MiniMapList* glist = (MiniMapList*)func_800B6CF8(1);
                            for (MiniMapListNode* node = glist->head->next;
                                 node != glist->head; node = node->next) {
                                MiniMapObj* g = (MiniMapObj*)node->object;
                                f32 objY = g->GetPos()->y;
                                u8 rowA = 0;
                                if (cf::CfObjectMove* player =
                                        cf::CfGameManager::getPlayer(0)) {
                                    ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                                    ml::CVec3 ppos;
                                    ppos.x = pposPtr->x;
                                    ppos.y = pposPtr->y;
                                    ppos.z = pposPtr->z;
                                    f32 playerY = ppos.y;
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 i = 1; i <= count; i++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], i);
                                        if ((f32)v > playerY) { rowA = i; break; }
                                    }
                                }
                                u8 found = 0;
                                {
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 j = 1; j <= count; j++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], j);
                                        if ((f32)v > objY) {
                                            if (j == rowA) found = 1;
                                            break;
                                        }
                                    }
                                }
                                if (found) {
                                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *g->GetPos();
                                    diff1 = diff;
                                    sprintf(name1, &lbl_eu_804FE1FC[653], g->m8C,
                                            diff.x, diff.y, diff.z);
                                }
                            }
                        }
                        if (name1[0] != 0) {
                            f32 len2 = diff1.x * diff1.x + diff1.y * diff1.y +
                                       diff1.z * diff1.z;
                            if (len2 > clamp * clamp) {
                                if (len2 < 0.0f) {
                                    Warning__Q24nw4r2dbFPCciPCce(
                                        (const char*)lbl_eu_80526324, 0x273,
                                        (const char*)lbl_eu_80526300);
                                }
                                f32 len = (len2 <= 0.0f)
                                              ? 0.0f
                                              : len2 * FrSqrt__Q24nw4r4mathFf(len2);
                                f32 k = clamp / len;
                                diff1.x *= k;
                                diff1.z *= k;
                            }
                            s32 gx = -(s32)(diff1.x / self->m20);
                            s32 gz = (s32)(diff1.z / self->m20);
                            void* pane = self->m0C->m10->v03C(name1, 1);
                            if (pane) {
                                *(f32*)((u8*)pane + 0x2C) = (f32)gx;
                                *(f32*)((u8*)pane + 0x30) = (f32)gz;
                                *(f32*)((u8*)pane + 0x34) = lbl_eu_80667090;
                            } else {
                                void* pic = 0;
                                void* texName = func_8013902C(7);
                                if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                    pic = createPicture__10CLibLayoutFv();
                                    SetName__Q34nw4r3lyt4PaneFPCc(pic, name1);
                                    *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                    *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                    *(f32*)((u8*)pic + 0x34) = lbl_eu_80667090;
                                    func_80137C1C(pic, -1);
                                    *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                                }
                                if (pic) {
                                    void* parent = self->m0C->m10->v03C(
                                        &lbl_eu_804FE1FC[105], 1);
                                    AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                        parent, pic);
                                }
                            }
                        }
                    } else {
                        name2[0] = 0;
                        diff2.x = lbl_eu_80667090;
                        diff2.y = lbl_eu_80667090;
                        diff2.z = lbl_eu_80667090;
                        switch (type) {
                        case 2: {
                            MiniMapCase2Elem* arr = (MiniMapCase2Elem*)func_80193804();
                            s32 count = *(s32*)((u8*)arr + 0x9800);
                            for (MiniMapCase2Elem* el = arr; el < arr + count; el++) {
                                if (el->m1C != mapId) continue;
                                f32 objY = el->y;
                                u8 rowA = 0;
                                if (cf::CfObjectMove* player =
                                        cf::CfGameManager::getPlayer(0)) {
                                    ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                                    ml::CVec3 ppos;
                                    ppos.x = pposPtr->x;
                                    ppos.y = pposPtr->y;
                                    ppos.z = pposPtr->z;
                                    f32 playerY = ppos.y;
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 i = 1; i <= count; i++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], i);
                                        if ((f32)v > playerY) { rowA = i; break; }
                                    }
                                }
                                u8 found = 0;
                                {
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 j = 1; j <= count; j++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], j);
                                        if ((f32)v > objY) {
                                            if (j == rowA) found = 1;
                                            break;
                                        }
                                    }
                                }
                                if (found) {
                                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() -
                                                     ml::CVec3(el->x, el->y, el->z);
                                    diff2 = diff;
                                    sprintf(name2, &lbl_eu_804FE1FC[137], el->m1C,
                                            diff.x, diff.y, diff.z);
                                    goto converge;
                                }
                            }
                            break;
                        }
                        case 3: {
                            MiniMapList* glist = (MiniMapList*)func_800B6C58();
                            for (MiniMapListNode* node = glist->head->next;
                                 node != glist->head; node = node->next) {
                                MiniMapObj* g = (MiniMapObj*)node->object;
                                if ((g->m64 & 0x4000) == 0 && (g->m64 & 0x8000) == 0)
                                    continue;
                                if (g->m8C != mapId) continue;
                                f32 objY = g->GetPos()->y;
                                u8 rowA = 0;
                                if (cf::CfObjectMove* player =
                                        cf::CfGameManager::getPlayer(0)) {
                                    ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                                    ml::CVec3 ppos;
                                    ppos.x = pposPtr->x;
                                    ppos.y = pposPtr->y;
                                    ppos.z = pposPtr->z;
                                    f32 playerY = ppos.y;
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 i = 1; i <= count; i++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], i);
                                        if ((f32)v > playerY) { rowA = i; break; }
                                    }
                                }
                                u8 found = 0;
                                {
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 j = 1; j <= count; j++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], j);
                                        if ((f32)v > objY) {
                                            if (j == rowA) found = 1;
                                            break;
                                        }
                                    }
                                }
                                if (found) {
                                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *g->GetPos();
                                    diff2 = diff;
                                    sprintf(name2, &lbl_eu_804FE1FC[125], g->m74,
                                            diff.x, diff.y, diff.z);
                                    goto converge;
                                }
                            }
                            break;
                        }
                        case 4: {
                            MiniMapList* glist = (MiniMapList*)func_800B6BEC();
                            for (MiniMapListNode* node = glist->head->next;
                                 node != glist->head; node = node->next) {
                                MiniMapObj* g = (MiniMapObj*)node->object;
                                if (g->m8C != mapId) continue;
                                f32 objY = g->GetPos()->y;
                                u8 rowA = 0;
                                if (cf::CfObjectMove* player =
                                        cf::CfGameManager::getPlayer(0)) {
                                    ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                                    ml::CVec3 ppos;
                                    ppos.x = pposPtr->x;
                                    ppos.y = pposPtr->y;
                                    ppos.z = pposPtr->z;
                                    f32 playerY = ppos.y;
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 i = 1; i <= count; i++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], i);
                                        if ((f32)v > playerY) { rowA = i; break; }
                                    }
                                }
                                u8 found = 0;
                                {
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 j = 1; j <= count; j++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], j);
                                        if ((f32)v > objY) {
                                            if (j == rowA) found = 1;
                                            break;
                                        }
                                    }
                                }
                                if (found) {
                                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *g->GetPos();
                                    diff2 = diff;
                                    sprintf(name2, &lbl_eu_804FE1FC[664], g->m74,
                                            diff.x, diff.y, diff.z);
                                    goto converge;
                                }
                            }
                            break;
                        }
                        case 5: {
                            MiniMapList* glist = (MiniMapList*)func_800B6BEC();
                            for (MiniMapListNode* node = glist->head->next;
                                 node != glist->head; node = node->next) {
                                MiniMapObj* g = (MiniMapObj*)node->object;
                                if (g->m8C != mapId) continue;
                                f32 objY = g->GetPos()->y;
                                u8 rowA = 0;
                                if (cf::CfObjectMove* player =
                                        cf::CfGameManager::getPlayer(0)) {
                                    ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                                    ml::CVec3 ppos;
                                    ppos.x = pposPtr->x;
                                    ppos.y = pposPtr->y;
                                    ppos.z = pposPtr->z;
                                    f32 playerY = ppos.y;
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 i = 1; i <= count; i++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], i);
                                        if ((f32)v > playerY) { rowA = i; break; }
                                    }
                                }
                                u8 found = 0;
                                {
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 j = 1; j <= count; j++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], j);
                                        if ((f32)v > objY) {
                                            if (j == rowA) found = 1;
                                            break;
                                        }
                                    }
                                }
                                if (found) {
                                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *g->GetPos();
                                    diff2 = diff;
                                    sprintf(name2, &lbl_eu_804FE1FC[677], g->m74,
                                            diff.x, diff.y, diff.z);
                                    goto converge;
                                }
                            }
                            break;
                        }
                        default:
                            break;
                        }
                    converge:
                        if (name2[0] != 0) {
                            f32 len2 = diff2.x * diff2.x + diff2.y * diff2.y +
                                       diff2.z * diff2.z;
                            if (len2 > clamp * clamp) {
                                if (len2 < 0.0f) {
                                    Warning__Q24nw4r2dbFPCciPCce(
                                        (const char*)lbl_eu_80526324, 0x273,
                                        (const char*)lbl_eu_80526300);
                                }
                                f32 len = (len2 <= 0.0f)
                                              ? 0.0f
                                              : len2 * FrSqrt__Q24nw4r4mathFf(len2);
                                f32 k = clamp / len;
                                diff2.x *= k;
                                diff2.z *= k;
                            }
                            s32 gx = -(s32)(diff2.x / self->m20);
                            s32 gz = (s32)(diff2.z / self->m20);
                            void* pane = self->m0C->m10->v03C(name2, 1);
                            if (pane) {
                                *(f32*)((u8*)pane + 0x2C) = (f32)gx;
                                *(f32*)((u8*)pane + 0x30) = (f32)gz;
                                *(f32*)((u8*)pane + 0x34) = lbl_eu_80667090;
                            } else {
                                void* pic = 0;
                                void* texName = func_8013902C(7);
                                if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                    pic = createPicture__10CLibLayoutFv();
                                    SetName__Q34nw4r3lyt4PaneFPCc(pic, name2);
                                    *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                    *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                    *(f32*)((u8*)pic + 0x34) = lbl_eu_80667090;
                                    func_80137C1C(pic, -1);
                                    *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                                }
                                if (pic) {
                                    void* parent = self->m0C->m10->v03C(
                                        &lbl_eu_804FE1FC[105], 1);
                                    AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                        parent, pic);
                                }
                            }
                        }
                    }
                }
            }
        }

        // ---- mode-32 section ----
        {
            gimmickView = func_801167EC((u8*)self + 0x17C);
            if (func_8009CF8C(0x3334) == 0) {
                for (u32 i = 0;
                     i < ((MiniMapEnumList*)func_80043F18(&holder))->count; i++) {
                    MiniMapObj* o = (MiniMapObj*)func_800B708C(
                        (s32)func_800F6E98(func_80043F18(&holder), i));
                    if (!o) continue;
                    if (!o->v160()) continue;
                    if ((o->m64 & 0x4000) == 0 && (o->m64 & 0x8000) == 0) continue;
                    f32 objY = o->GetPos()->y;
                    u8 rowA = 0;
                    if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                                ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                        ml::CVec3 ppos;
                        ppos.x = pposPtr->x;
                        ppos.y = pposPtr->y;
                        ppos.z = pposPtr->z;
                        f32 playerY = ppos.y;
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 i = 1; i <= count; i++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], i);
                            if ((f32)v > playerY) { rowA = i; break; }
                        }
                    }
                    u8 found = 0;
                    {
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 j = 1; j <= count; j++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], j);
                            if ((f32)v > objY) {
                                if (j == rowA) found = 1;
                                break;
                            }
                        }
                    }
                    if (!found) continue;
                    {
                        ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *o->GetPos();
                        sprintf(buf3, &lbl_eu_804FE1FC[125],
                                (u32)func_800F6E98(func_80043F18(&holder), i),
                                diff.x, diff.y, diff.z);
                        s32 gx = -(s32)(diff.x / self->m20);
                        s32 gz = (s32)(diff.z / self->m20);
                        void* pane = self->m0C->m10->v03C(buf3, 1);
                        if (pane) {
                            *(f32*)((u8*)pane + 0x2C) = (f32)gx;
                            *(f32*)((u8*)pane + 0x30) = (f32)gz;
                            *(f32*)((u8*)pane + 0x34) = lbl_eu_80667090;
                        } else if (o->m91 == 6) {
                            void* pic = 0;
                            void* texName = func_8013902C(15);
                            if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                pic = createPicture__10CLibLayoutFv();
                                SetName__Q34nw4r3lyt4PaneFPCc(pic, buf3);
                                *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                *(f32*)((u8*)pic + 0x34) = lbl_eu_80667090;
                                func_80137C1C(pic, -1);
                                *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                            }
                            if (pic) {
                                void* parent = self->m0C->m10->v03C(
                                    &lbl_eu_804FE1FC[692], 1);
                                AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                    parent, pic);
                            }
                        }
                    }
                }
            }
        }

        // ---- row-table section ----
        {
            u8 rowB = (u8)lbl_eu_80664184;
            void* bdat2 = (void*)func_801380A0(rowB);
            void* bdatEntry = lbl_eu_80573D18[rowB - 1];
            u16 rowCount = (u16)func_8003B1EC(bdatEntry);
            if (gimmickView != 0) {
                for (u32 i = 0;
                     i < ((MiniMapEnumList*)func_80043F18(&holder))->count; i++) {
                    MiniMapObj* o = (MiniMapObj*)func_800B708C(
                        (s32)func_800F6E98(func_80043F18(&holder), i));
                    if (!o) continue;
                    if ((o->m64 & 0x8) == 0) continue;
                    f32 objY = o->GetPos()->y;
                    u8 rowA = 0;
                    if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                                ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                        ml::CVec3 ppos;
                        ppos.x = pposPtr->x;
                        ppos.y = pposPtr->y;
                        ppos.z = pposPtr->z;
                        f32 playerY = ppos.y;
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 i = 1; i <= count; i++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], i);
                            if ((f32)v > playerY) { rowA = i; break; }
                        }
                    }
                    u8 found = 0;
                    {
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 j = 1; j <= count; j++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], j);
                            if ((f32)v > objY) {
                                if (j == rowA) found = 1;
                                break;
                            }
                        }
                    }
                    if (!found) continue;
                    for (u16 k = (u16)rowB;
                         (s32)(u16)k < (s32)((u16)rowB + rowCount); k++) {
                        if (func_80136254(bdat2, &lbl_eu_804FE1FC[702], (u16)k) !=
                            o->m8C)
                            continue;
                        if (!func_80138234(bdat2, (u16)k)) continue;
                        {
                            ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *o->GetPos();
                            sprintf(buf4, &lbl_eu_804FE1FC[115],
                                    (u32)func_800F6E98(func_80043F18(&holder), i),
                                    diff.x, diff.y, diff.z);
                            s32 gx = -(s32)(diff.x / self->m20);
                            s32 gz = (s32)(diff.z / self->m20);
                            void* pane = self->m0C->m10->v03C(buf4, 1);
                            if (pane) {
                                *(f32*)((u8*)pane + 0x2C) = (f32)gx;
                                *(f32*)((u8*)pane + 0x30) = (f32)gz;
                                *(f32*)((u8*)pane + 0x34) = lbl_eu_80667090;
                            } else {
                                void* pic = 0;
                                void* texName = func_8013902C(10);
                                if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                    pic = createPicture__10CLibLayoutFv();
                                    SetName__Q34nw4r3lyt4PaneFPCc(pic, buf4);
                                    *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                    *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                    *(f32*)((u8*)pic + 0x34) = lbl_eu_80667090;
                                    func_80137C1C(pic, -1);
                                    *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                                }
                                if (pic) {
                                    void* parent = self->m0C->m10->v03C(
                                        &lbl_eu_804FE1FC[709], 1);
                                    AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                        parent, pic);
                                }
                            }
                        }
                    }
                }
            }

            // ---- sub-actor section (vt228()==3) ----
            for (u32 i = 0;
                 i < ((MiniMapEnumList*)func_80043F18(&holder))->count; i++) {
                MiniMapObj* o = (MiniMapObj*)func_800B708C(
                    (s32)func_800F6E98(func_80043F18(&holder), i));
                if (!o) continue;
                if (!o->v160()) continue;
                if ((o->m64 & 0x8) == 0) continue;
                if (((MiniMapObj*)func_800BF324(o))->v228() != 3) continue;
                f32 objY = o->GetPos()->y;
                u8 rowA = 0;
                if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                        ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                    ml::CVec3 ppos;
                    ppos.x = pposPtr->x;
                    ppos.y = pposPtr->y;
                    ppos.z = pposPtr->z;
                    f32 playerY = ppos.y;
                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                    for (u8 i = 1; i <= count; i++) {
                        s16 v = func_80136330(lbl_eu_80663FB8,
                                              &lbl_eu_804FE1FC[48], i);
                        if ((f32)v > playerY) { rowA = i; break; }
                    }
                }
                u8 found = 0;
                {
                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                    for (u8 j = 1; j <= count; j++) {
                        s16 v = func_80136330(lbl_eu_80663FB8,
                                              &lbl_eu_804FE1FC[48], j);
                        if ((f32)v > objY) {
                            if (j == rowA) found = 1;
                            break;
                        }
                    }
                }
                if (!found) continue;
                {
                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *o->GetPos();
                    sprintf(buf5, &lbl_eu_804FE1FC[115],
                            (u32)func_800F6E98(func_80043F18(&holder), i),
                            diff.x, diff.y, diff.z);
                    s32 gx = -(s32)(diff.x / self->m20);
                    s32 gz = (s32)(diff.z / self->m20);
                    void* pane = self->m0C->m10->v03C(buf5, 1);
                    if (pane) {
                        *(f32*)((u8*)pane + 0x2C) = (f32)gx;
                        *(f32*)((u8*)pane + 0x30) = (f32)gz;
                        *(f32*)((u8*)pane + 0x34) = lbl_eu_80667090;
                    } else {
                        void* pic = 0;
                        void* texName = func_8013902C(12);
                        if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                            pic = createPicture__10CLibLayoutFv();
                            SetName__Q34nw4r3lyt4PaneFPCc(pic, buf5);
                            *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                            *(f32*)((u8*)pic + 0x30) = (f32)gz;
                            *(f32*)((u8*)pic + 0x34) = lbl_eu_80667090;
                            func_80137C1C(pic, -1);
                            *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                        }
                        if (pic) {
                            void* parent = self->m0C->m10->v03C(
                                &lbl_eu_804FE1FC[718], 1);
                            AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                parent, pic);
                        }
                    }
                }
            }

            // ---- m91==12 section ----
            for (u32 i = 0;
                 i < ((MiniMapEnumList*)func_80043F18(&holder))->count; i++) {
                MiniMapObj* o = (MiniMapObj*)func_800B708C(
                    (s32)func_800F6E98(func_80043F18(&holder), i));
                if (!o) continue;
                if (!o->v160()) continue;
                if ((o->m64 & 0x4000) == 0 && (o->m64 & 0x8000) == 0) continue;
                if (o->m91 != 12) continue;
                f32 objY = o->GetPos()->y;
                u8 rowA = 0;
                if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                        ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                    ml::CVec3 ppos;
                    ppos.x = pposPtr->x;
                    ppos.y = pposPtr->y;
                    ppos.z = pposPtr->z;
                    f32 playerY = ppos.y;
                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                    for (u8 i = 1; i <= count; i++) {
                        s16 v = func_80136330(lbl_eu_80663FB8,
                                              &lbl_eu_804FE1FC[48], i);
                        if ((f32)v > playerY) { rowA = i; break; }
                    }
                }
                u8 found = 0;
                {
                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                    for (u8 j = 1; j <= count; j++) {
                        s16 v = func_80136330(lbl_eu_80663FB8,
                                              &lbl_eu_804FE1FC[48], j);
                        if ((f32)v > objY) {
                            if (j == rowA) found = 1;
                            break;
                        }
                    }
                }
                if (!found) continue;
                {
                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *o->GetPos();
                    sprintf(buf6, &lbl_eu_804FE1FC[125],
                            (u32)func_800F6E98(func_80043F18(&holder), i),
                            diff.x, diff.y, diff.z);
                    s32 gx = -(s32)(diff.x / self->m20);
                    s32 gz = (s32)(diff.z / self->m20);
                    void* pane = self->m0C->m10->v03C(buf6, 1);
                    if (pane) {
                        *(f32*)((u8*)pane + 0x2C) = (f32)gx;
                        *(f32*)((u8*)pane + 0x30) = (f32)gz;
                        *(f32*)((u8*)pane + 0x34) = lbl_eu_80667090;
                    } else {
                        void* pic = 0;
                        void* texName = func_8013902C(16);
                        if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                            pic = createPicture__10CLibLayoutFv();
                            SetName__Q34nw4r3lyt4PaneFPCc(pic, buf6);
                            *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                            *(f32*)((u8*)pic + 0x30) = (f32)gz;
                            *(f32*)((u8*)pic + 0x34) = lbl_eu_80667090;
                            func_80137C1C(pic, -1);
                            *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                        }
                        if (pic) {
                            void* parent = self->m0C->m10->v03C(
                                &lbl_eu_804FE1FC[727], 1);
                            AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                parent, pic);
                        }
                    }
                }
            }

            // ---- type-dispatch section (17/18/20) ----
            if (gimmickView != 0) {
                for (u32 i = 0;
                     i < ((MiniMapEnumList*)func_80043F18(&holder))->count; i++) {
                    MiniMapObj* o = (MiniMapObj*)func_800B708C(
                        (s32)func_800F6E98(func_80043F18(&holder), i));
                    if (!o) continue;
                    if (!o->v160()) continue;
                    f32 objY = o->GetPos()->y;
                    u8 rowA = 0;
                    if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                                ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                        ml::CVec3 ppos;
                        ppos.x = pposPtr->x;
                        ppos.y = pposPtr->y;
                        ppos.z = pposPtr->z;
                        f32 playerY = ppos.y;
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 i = 1; i <= count; i++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], i);
                            if ((f32)v > playerY) { rowA = i; break; }
                        }
                    }
                    u8 found = 0;
                    {
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 j = 1; j <= count; j++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], j);
                            if ((f32)v > objY) {
                                if (j == rowA) found = 1;
                                break;
                            }
                        }
                    }
                    if (!found) continue;
                    {
                        ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *o->GetPos();
                        sprintf(buf7, &lbl_eu_804FE1FC[115],
                                (u32)func_800F6E98(func_80043F18(&holder), i),
                                diff.x, diff.y, diff.z);
                        s32 gx = -(s32)(diff.x / self->m20);
                        s32 gz = (s32)(diff.z / self->m20);
                        void* pane = self->m0C->m10->v03C(buf7, 1);
                        if (pane) {
                            *(f32*)((u8*)pane + 0x2C) = (f32)gx;
                            *(f32*)((u8*)pane + 0x30) = (f32)gz;
                            *(f32*)((u8*)pane + 0x34) = lbl_eu_80667090;
                        } else {
                            s32 type2 = 0;
                            u32 arg2 = 0xFFFFFFFF;
                            if (o->m64 & 0x80000000) {
                                type2 = 17;
                                arg2 = 0xFF8C1EFF;
                            } else if (o->m64 & 0x8) {
                                int mode = o->v228();
                                if (mode == 2) {
                                    type2 = 18;
                                    arg2 = 0x3232C8FF;
                                } else if (mode == 9 || mode == 13) {
                                    type2 = 17;
                                    arg2 = 0xFF8C1EFF;
                                }
                            } else if (o->m64 & 0x10) {
                                type2 = 20;
                                arg2 = 0xEE1100FF;
                            }
                            void* pic = 0;
                            if (type2 != 0) {
                                void* texName = func_8013902C(type2);
                                if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                    pic = createPicture__10CLibLayoutFv();
                                    SetName__Q34nw4r3lyt4PaneFPCc(pic, buf7);
                                    *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                    *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                    *(f32*)((u8*)pic + 0x34) = lbl_eu_80667090;
                                    func_80137C1C(pic, arg2);
                                    *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                                }
                            }
                            if (pic) {
                                void* parent = self->m0C->m10->v03C(
                                    &lbl_eu_804FE1FC[739], 1);
                                AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                    parent, pic);
                            }
                        }
                    }
                }
            }

            // ---- reslist section ----
            {
                MiniMapList* rlist = (MiniMapList*)func_800B6CC4();
                for (MiniMapListNode* node = rlist->head->next;
                     node != rlist->head; node = node->next) {
                    MiniMapObj* o = (MiniMapObj*)node->object;
                    f32 objY = o->GetPos()->y;
                    u8 rowA = 0;
                    if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                                ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                        ml::CVec3 ppos;
                        ppos.x = pposPtr->x;
                        ppos.y = pposPtr->y;
                        ppos.z = pposPtr->z;
                        f32 playerY = ppos.y;
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 i = 1; i <= count; i++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], i);
                            if ((f32)v > playerY) { rowA = i; break; }
                        }
                    }
                    u8 found = 0;
                    {
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 j = 1; j <= count; j++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], j);
                            if ((f32)v > objY) {
                                if (j == rowA) found = 1;
                                break;
                            }
                        }
                    }
                    if (!found) continue;
                    {
                        f32 radius = lbl_eu_806670B8 * self->m20;
                        ml::CVec3 pos;
                        func_800ABC5C(&pos, o);
                        ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - pos;
                        f32 len2 = diff.x * diff.x + diff.y * diff.y +
                                   diff.z * diff.z;
                        if (len2 > radius * radius) {
                            s32 gx = -(s32)(diff.x / self->m20);
                            s32 gz = (s32)(diff.z / self->m20);
                            sprintf(buf8, &lbl_eu_804FE1FC[747], o->m74,
                                    diff.x, diff.y, diff.z);
                            void* pane = self->m0C->m10->v03C(buf8, 1);
                            if (pane) {
                                *(f32*)((u8*)pane + 0x2C) = (f32)gx;
                                *(f32*)((u8*)pane + 0x30) = (f32)gz;
                                *(f32*)((u8*)pane + 0x34) = lbl_eu_80667090;
                            } else {
                                void* pic = 0;
                                void* texName = func_8013902C(3);
                                if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                    pic = createPicture__10CLibLayoutFv();
                                    SetName__Q34nw4r3lyt4PaneFPCc(pic, buf8);
                                    *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                    *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                    *(f32*)((u8*)pic + 0x34) = lbl_eu_80667090;
                                    func_80137C1C(pic, -1);
                                    *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                                }
                                if (pic) {
                                    void* parent = self->m0C->m10->v03C(
                                        &lbl_eu_804FE1FC[762], 1);
                                    PrependChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                        parent, pic);
                                }
                            }
                        }
                    }
                }
            }
        }

        func_8011628C((u8*)self + 0x3C, lbl_eu_80664184);
        __dt__14Class_8045F858Fv(guard, -1);
        __dt__80043E88(&holder, -1);
    }
}

void func_8011B05C(){}


void CMenuMiniMap2::Init() {}

void CMenuMiniMap2::Term() {}

void CMenuMiniMap2::Move() {}

void CMenuMiniMap2::cbRenderBefore() {}

void __ct__8011C1B8(){}

extern u32 lbl_eu_80663F20;
extern u32 lbl_eu_80663FB0;
bool func_8011C2E8() {
    u32 v = lbl_eu_80663FB0;
    return ((-v) | v) >> 31;
}

// Cast-only SI for the minimap sub-object virtual calls (slots 0x2C/0x38)
struct MiniMapIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void vf2C(void* a, u32 b);
    virtual void _v030(); virtual void _v034(); virtual void vf38(void* a);
};
extern "C" void func_8011C2FC(void) {
    void* g = (void*)lbl_eu_80663FB0;
    if (!g) return;
    ((u8*)g)[0x8d4] = 3;
    ((u8*)g)[0x7c] = 3;
    ((u8*)g)[0x7d] = 0;
    if (*(void**)((u8*)g + 0x70)) {
        ((MiniMapIf*)*(void**)((u8*)g + 0x70))->vf2C(*(void**)((u8*)g + 0x78), 0);
        ((MiniMapIf*)*(void**)((u8*)g + 0x70))->vf2C(*(void**)((u8*)g + 0x74), 1);
        ((MiniMapIf*)*(void**)((u8*)g + 0x70))->vf38(0);
    }
    ((u8*)g)[0xa8] = 3;
    ((u8*)g)[0xa9] = 0;
    if (*(void**)((u8*)g + 0x9c)) {
        ((MiniMapIf*)*(void**)((u8*)g + 0x9c))->vf2C(*(void**)((u8*)g + 0xa4), 0);
        ((MiniMapIf*)*(void**)((u8*)g + 0x9c))->vf2C(*(void**)((u8*)g + 0xa0), 1);
        ((MiniMapIf*)*(void**)((u8*)g + 0x9c))->vf38(0);
    }
}

void func_8011C400()
{
    if (lbl_eu_80663FB0 != 0)
    {
        *((unsigned char*)lbl_eu_80663FB0 + 0x54) = 1;
    }
}

extern "C" void func_8011C43C(void* self) { reinterpret_cast<CMenuMiniMap2*>((char*)self - 0x5c)->cbRenderBefore(); }

extern "C" void func_8011C444(void* self) { reinterpret_cast<CMenuMiniMap2*>((char*)self - 0x5c)->~CMenuMiniMap2(); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8011C418() {
    lbl_eu_80663FB4 = lbl_eu_806670CC * (lbl_eu_806670A0 * lbl_eu_80661E48);
}

extern "C" void func_80115FD0() {}
extern "C" void func_801160A8() {}
#pragma dont_inline on
extern "C" void func_8011628C(void* self, u32 row) {}
extern "C" void func_801165EC() {}
extern "C" void func_80116670() {}
extern "C" void* func_801167EC(void* self) { return 0; }
extern "C" void func_801168A0() {}
extern "C" void func_80116B40(void* self) {}
#pragma dont_inline off

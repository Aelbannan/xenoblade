// Auto-scaffolded catalog TU for kyoshin/code_800B06A4
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/code_800B06A4.hpp"

void UnkClass_800B0AD8::clearCounters() {
    *(unsigned long*)((char*)this + 0xb00) = 0;
    *(unsigned long*)((char*)this + 0xafc) = 0;
}
// Forward declarations for functions called from wrappers
// func_800B07E8 is declared as UnkClass_805764CC::func_800B07E8() in the header.
// Provide an alias for unqualified calls (sub_mainReset, sub_dispatchInit_*).
UnkClass_805764CC* func_800B07E8();
u32 func_800AA2BC(u32 a, u32 b);

void gfloat_initStore(float a){
    extern float lbl_eu_80661CCC;
    extern float lbl_eu_80661CD0;
    extern float lbl_eu_80663EC8;
    extern float lbl_eu_80663ECC;
    extern float lbl_eu_80663ED0;
    extern float lbl_eu_80663ED4;

    double ad = a;
    float sq = (float)ad;
    float c = lbl_eu_80661CD0;
    lbl_eu_80661CCC = a;
    sq = sq * sq;
    lbl_eu_80663EC8 = a;
    lbl_eu_80663ECC = sq;
    lbl_eu_80663ED0 = c;
    lbl_eu_80663ED4 = sq;
}






void init_0A90(void){}




// clearCounters defined inline in header

void __ct__reslist_cf_TboxInfo(){}


void __ct__reslist_cf_IFactoryEvent(){}


void init_0FA0(){}

u32 UnkClass_805764CC::get_u32_18() { return *(u32*)((u8*)this + 0x18); }

void init_0FF4(){}

void init_dispatchTarget_1(){}

void __ct__cf_CfValueItemManager(){}

void gflag_setBits(unsigned long flags) {
    extern unsigned long lbl_eu_80663EE0;
    lbl_eu_80663EE0 |= flags;
}

void FactoryEvent3__Q22cf13IFactoryEventFv() {}

void init_137C(){}

void node_copyNextPtr(void* dst, void* src) {
    void* ptr = *(void**)((char*)src + 4);
    *(unsigned long*)dst = *(unsigned long*)ptr;
}

void* node_getDataPtr(void* self) {
    return (char*)(*(void**)self) + 8;
}

void init_14E0() {}

void node_copyNextU32(void* dst, void* src) {
    *(unsigned long*)dst = *(unsigned long*)((char*)src + 4);
}

void init_14FC() {}

void init_1518(void){}

void __dt__800B151C(){}

void init_dispatchTarget_2(){}

extern u32 lbl_eu_80663F24;
void gvar_clearF24(){ lbl_eu_80663F24 = 0; }

void FactoryEvent4__Q22cf13IFactoryEventFv() {}

void UnkClass_805764CC::set_u32_00(u32 val) { *(u32*)((u8*)this + 0x0) = val; }

void copy_int_ptr(int* dst, int* src) {
    *dst = *src;
}

void init_182C() {}

void __dt__800B183C(){}

void __dt__800B18CC(){}

void init_dispatchTarget_5(){}

void init_1A5C(){}

void init_1A8C() {}

void* node_getDataPtr_alt(void* self) {
    return (char*)(*(void**)self) + 8;
}

void init_1AA8() {}

void copy_int_ptr_alt(int* dst, int* src) {
    *dst = *src;
}

void node_copyNextU32_alt(void* dst, void* src) {
    *(unsigned long*)dst = *(unsigned long*)((char*)src + 4);
}

void init_1AD8() {}

void init_1AF4(){}

void init_dispatchTarget_6(){}

void init_1BBC(){}

unsigned long gflag_getBit25(){
    extern unsigned long lbl_eu_80663EE0;
    return (lbl_eu_80663EE0 >> 25) & 1;
}

void init_1C0C() {}

void init_1C24() {}

void init_1C40(){}

void init_1C78(){}

void init_1CDC(){}

void init_1E18(){}

void init_1E2C(){}

void init_1EB8() {}

void init_1EC8(){}

void init_1F2C(){}

void init_1F40(){}

void init_1F54(){}

void init_1F6C(){}

void init_1FD8(){}

void init_2034(){}

void init_2048(){}

void init_205C(){}

void init_20A0(){}

void init_20B4(){}

u32 UnkClass_805764CC::get_u32_04() { return *(u32*)((u8*)this + 0x4); }

void __dt__800B2D30(){}

void FactoryEvent1__Q22cf13IFactoryEventFv() {}

void UnkClass_805764CC::fwdTo_2DB0(int param) {
    extern void fwd_2DB0_body(void*, void*);
    fwd_2DB0_body(this, &param);
}

void fwd_2DB0_body(){}

void init_2E38(){}

void init_2ED0(){}

int obj_testBit_64_v0(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int obj_testBit_64_v1(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int obj_testBit_64_v2(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int obj_testBit_64_v3(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int obj_testBit_64_v4(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int obj_testBit_64_v5(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int obj_testBit_64_v6(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

u32 shift_u32_hi8(u32 val) { return (val >> 16) & 0xFF; }

void init_3210(){}

u32 UnkClass_805764CC::get_u32_74() { return *(u32*)((u8*)this + 0x74); }

void init_39C8(){}

void copy_int_ptr_alt2(int* dst, int* src) {
    *dst = *src;
}

void UnkClass_805764CC::fwdTo_99EC(int param) {
    extern void fwd_99EC_body(void*, void*);
    fwd_99EC_body(this, &param);
}

void init_3A88(){}

u32 shift_u32_byte3(u32 val) { return val >> 24; }

u32 shift_u32_hi8_alt1(u32 val) { return (val >> 16) & 0xFF; }

void init_3D4C() {}

void init_3D64(){}

void init_4120(){}

void init_4278(){}

void init_42E8(){}

void init_4368(){}

void init_4400(){}

void init_44A0(){}

void init_4554(){}

void init_4588(u32 val) {}

u32 shift_u32_hi8_alt2(u32 val) { return (val >> 16) & 0xFF; }

void init_45A0(){}

void init_47A8(){}

void init_4A24(){}

int CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv(void* self) { return 0; }

void init_4B0C(){}

void init_4B74(){}

void init_4B88(){}

void init_4CA0(){}

void init_4D84(){}

u32 shift_u32_hi8_alt3(u32 val) { return (val >> 16) & 0xFF; }

extern u16 lbl_eu_8066408C;
unsigned short gvar_get408C(){
    return lbl_eu_8066408C;
}

void init_4F6C() {}

void init_4F80() {}

void init_4F90() {}

u32 UnkClass_805764CC::get_u32_70() { return *(u32*)((u8*)this + 0x70); }

void init_4FAC(){}

void init_535C(){}

void init_570C(){}

void init_587C(){}

void init_5944() {}

void init_5948() {}

void UnkClass_805764CC::clear_380() { *(u32*)((u8*)this + 896) = 0; }

void* UnkClass_805764CC::getNull() { return 0; }

u32 UnkClass_805764CC::get_u32_380() { return *(u32*)((u8*)this + 0x380); }

void init_5978(){}

void init_5994(){}

void init_6484() {}

void init_6494() {}

u32 shift_u32_hi8_alt4(u32 val) { return (val >> 16) & 0xFF; }

void init_64B8() {}

u32 shift_u32_and(u32 a, u32 b) { return a & b; }

u32 UnkClass_805764CC::get_u32_98() { return *(u32*)((u8*)this + 0x98); }

u16 UnkClass_805764CC::get_u16_45C0() { return *(u16*)((u8*)this + 0x45C0); }

u32 shift_u32_hi8_alt5(u32 val) { return (val >> 16) & 0xFF; }

void init_64F8() {}

void init_6508() {}

void UnkClass_805764CC::appendToBuffer(void* other) {
    unsigned int count = *(unsigned int*)((unsigned char*)this + 0x700);
    unsigned int val = *(unsigned int*)other;
    unsigned int newCount = count + 1;
    *(unsigned int*)((unsigned char*)this + 0x700) = newCount;
    float fval = *(float*)((unsigned char*)other + 4);
    void* dest = (unsigned char*)this + count * 8;
    *(unsigned int*)dest = val;
    *(float*)((unsigned char*)dest + 4) = fval;
}

void init_6544() {}

void init_655C(){}

void init_66AC() {}

void init_66BC(){}

void init_67CC(){}

void* UnkClass_805764CC::getPtr_1A8() { return (void*)((u8*)this + 0x1a8); }

void UnkClass_805764CC::clear_700() { *(u32*)((u8*)this + 1792) = 0; }

void init_6800(){}

void init_68A8(){}

void init_6AF4(){}

void init_6BA0(void){}

void* sub_getReslist_B28() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xB28;
}

void* sub_getReslist_B48() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xB48;
}

void* sub_getReslist_B68() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xB68;
}

void* sub_getReslist_B88() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xB88;
}

void* sub_getReslist_BE8() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xBE8;
}

void* sub_getReslist_BC8() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xBC8;
}

void* sub_getReslist_C08() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xC08;
}

void* sub_getReslist_C48() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xC48;
}

void init_6CC4(){}

void init_6CF8(){}

void* func_800B6D3C(void*);
void fwd_6DD0_body(){}

void init_6EC0(){}

u32 UnkClass_800B0AD8::getCount() { return *(u32*)((u8*)this + 0xB00); }

u32 UnkClass_800B0AD8::getSize() { return *(u32*)((u8*)this + 0xB04); }

void* UnkClass_800B0AD8::getRingElem(u32 index) {
    u32 start = *(u32*)((u8*)this + 0xAFC);
    u32 count = *(u32*)((u8*)this + 0xB04);
    u32* base = *(u32**)((u8*)this + 0xAF8);
    return &base[(start + index) % count];
}

void sub_mainReset() {
    func_800B6D3C(func_800B07E8());
}

void init_7058(){}

void init_708C(){}

void init_70FC(){}

u32 gvar_get40F4(void) {
    extern u32 lbl_eu_806640F4;
    return lbl_eu_806640F4;
}

void init_71CC(){}

void* UnkClass_805764CC::getPtr_720() { return (void*)((u8*)this + 0x720); }

void init_7214(){}

void init_72DC(){}

void init_7320(){}

void list_removeNode(void* out_prev, void* unused, void* list_ptr) {
    (void)unused;
    void* entry = *(void**)list_ptr;
    void* prev = *(void**)entry;
    void* next = *(void**)((char*)entry + 4);
    *(void**)next = prev;
    *(void**)((char*)prev + 4) = next;
    entry = *(void**)list_ptr;
    *(void**)entry = NULL;
    *(void**)out_prev = prev;
}

void init_7410(){}

unsigned short gvar_get408C_alt1(){
    return lbl_eu_8066408C;
}

unsigned short gvar_get408C_alt2(){
    return lbl_eu_8066408C;
}

void UnkClass_805764CC::maskField_6C(u32 mask, int enable) {
    u32* field = (u32*)((u8*)this + 0x6C);
    if (enable) {
        *field |= mask;
    } else {
        *field &= ~mask;
    }
}

void init_75EC(){}

int list_countNodes(void* self){
    void* head = *(void**)((char*)self + 4);
    void* cur = *(void**)head;
    int count = 0;
    while (cur != head) {
        cur = *(void**)cur;
        count++;
    }
    return count;
}

void sub_resetReslist_B28(void* self) {
    func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB28);
}

void sub_resetReslist_B48(void* self) {
    func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB48);
}

void sub_resetReslist_B68(void* self) {
    func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB68);
}

void* sub_resetReslist_B88(void* self) {
    return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB88);
}

void* sub_resetReslist_BE8(void* self) {
    return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBE8);
}

void* sub_resetReslist_BC8(void* self) {
    return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBC8);
}

void* sub_resetReslist_C08(void* self) {
    return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xC08);
}

void* sub_resetReslist_BA8(void* self) {
    return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBA8);
}

void init_77E4(){}

void init_781C(){}

void init_7854(){}

void sub_pushToReslist_B88(cf::CfObject* param_1) { extern void fwd_6DD0_body(void*, void*); fwd_6DD0_body(&UnkClass_805764CC::func_800B07E8()->field_0xB88, param_1); }

void init_78C4(){}

void init_78FC(){}

void init_7934(){}

void init_796C(){}

void init_79A4(){}

void init_7A18(){}

void init_7AF0(){}

void init_83AC(){}

void init_8524(){}

void init_dispatchTarget_3() {}

void init_dispatchTarget_4() {}

void init_8804(){}

void init_88E0(){}

void init_8920(){}

void init_89CC(){}

void init_8A64(){}

void init_8AFC(){}

void init_8B94(){}

void init_8C78(){}

void init_8D5C(){}

void init_8FC4(){}

void sub_dispatchInit_1() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_1)();
}

void sub_dispatchInit_2() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_2)();
}

void init_92FC(){}

void sub_dispatchInit_3() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_3)();
}

void sub_dispatchInit_4() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_4)();
}

void sub_dispatchInit_5() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_5)();
}

void sub_dispatchInit_6() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_6)();
}

void init_93D0(){}

void init_9404(){}

void init_9438(){}

void init_946C(){}

void init_94A0(){}

void init_dispatchTarget_7(){}

void sub_dispatchWithArgs(u32 a, u32 b) {
    func_800AA2BC(a, b);
    ((void(*)())init_dispatchTarget_7)();
}

void init_9548(){}

void init_957C(){}

void UnkClass_805764CC::set_u32_734(u32 val) { *(u32*)((u8*)this + 0x734) = val; }

void UnkClass_805764CC::set_u8_73A(u8 a, u8 b) { this->field_0x20.unk0[0x71A] = a; this->field_0x20.unk0[0x71B] = b; }

void UnkClass_805764CC::set_u32_720(u32 val) { *(u32*)((u8*)this + 0x720) = val; }

void UnkClass_805764CC::set_u16_738(u16 val) { *(u16*)((u8*)this + 0x738) = val; }

void init_96E8() {}

void UnkClass_805764CC::set_float_730(float val) { *(float*)((u8*)this + 0x730) = val; }

void __ct__800B970C(){}

void init_97A0(){}

void init_985C(){}

void init_98C8(){}

u32 UnkClass_805764CC::get_u32_620() { return *(u32*)((u8*)this + 0x620); }

void init_998C(){}

void init_99BC(){}

void fwd_99EC_body(){}

void init_9A30() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Definitions provided by include/kyoshin/code_800B06A4.hpp
template <> _reslist_base<cf::TboxInfo>::~_reslist_base() {}
template <> reslist<cf::TboxInfo>::~reslist() {}
template <> _reslist_base<cf::IFactoryEvent*>::~_reslist_base() {}
template <> reslist<cf::IFactoryEvent*>::~reslist() {}
void sinit_800B9A40(){}

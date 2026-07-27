// Auto-scaffolded catalog TU for kyoshin/code_800B06A4
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_800B06A4.hpp"
// Forward declarations for functions called from wrappers
void* func_800B07E8();
u32 func_800AA2BC(u32 a, u32 b);

void func_800B06A4(float a){
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






void func_800B0A90(void){}




void func_800B0B40(void* self) {
    *(unsigned long*)((char*)self + 0xb00) = 0;
    *(unsigned long*)((char*)self + 0xafc) = 0;
}

void __ct__reslist_cf_TboxInfo(){}


void __ct__reslist_cf_IFactoryEvent(){}

void UnkClass_805764CC::~UnkClass_805764CC() const {}

void func_800B0FA0(){}

u32 func_800B0FEC(void* self) { return *(u32*)((u8*)self + 0x18); }

void func_800B0FF4(){}

void func_800B1120(){}

void __ct__cf_CfValueItemManager(){}

void func_800B1368(unsigned long flags) {
    extern unsigned long lbl_eu_80663EE0;
    lbl_eu_80663EE0 |= flags;
}

void FactoryEvent3__Q22cf13IFactoryEventFv() {}

void func_800B137C(){}

void func_800B14C4(void* dst, void* src) {
    void* ptr = *(void**)((char*)src + 4);
    *(unsigned long*)dst = *(unsigned long*)ptr;
}

void* func_800B14D4(void* self) {
    return (char*)(*(void**)self) + 8;
}

void func_800B14E0() {}

void func_800B14F0(void* dst, void* src) {
    *(unsigned long*)dst = *(unsigned long*)((char*)src + 4);
}

void func_800B14FC() {}

void func_800B1518(void){}

void __dt__800B151C(){}

void func_800B15A4(){}

extern u32 lbl_eu_80663F24;
void func_800B1808(){ lbl_eu_80663F24 = 0; }

void FactoryEvent4__Q22cf13IFactoryEventFv() {}

void func_800B1818(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }

void func_800B1820(int* dst, int* src) {
    *dst = *src;
}

void func_800B182C() {}

void __dt__800B183C(){}

void __dt__800B18CC(){}

void func_800B1954(){}

void func_800B1A5C(){}

void func_800B1A8C() {}

void* func_800B1A9C(void* self) {
    return (char*)(*(void**)self) + 8;
}

void func_800B1AA8() {}

void func_800B1AC0(int* dst, int* src) {
    *dst = *src;
}

void func_800B1ACC(void* dst, void* src) {
    *(unsigned long*)dst = *(unsigned long*)((char*)src + 4);
}

void func_800B1AD8() {}

void func_800B1AF4(){}

void func_800B1B2C(){}

void func_800B1BBC(){}

unsigned long func_800B1C00(){
    extern unsigned long lbl_eu_80663EE0;
    return (lbl_eu_80663EE0 >> 25) & 1;
}

void func_800B1C0C() {}

void func_800B1C24() {}

void func_800B1C40(){}

void func_800B1C78(){}

void func_800B1CDC(){}

void func_800B1E18(){}

void func_800B1E2C(){}

void func_800B1EB8() {}

void func_800B1EC8(){}

void func_800B1F2C(){}

void func_800B1F40(){}

void func_800B1F54(){}

void func_800B1F6C(){}

void func_800B1FD8(){}

void func_800B2034(){}

void func_800B2048(){}

void func_800B205C(){}

void func_800B20A0(){}

void func_800B20B4(){}

u32 func_800B2D28(void* self) { return *(u32*)((u8*)self + 0x4); }

void __dt__800B2D30(){}

void FactoryEvent1__Q22cf13IFactoryEventFv() {}

void func_800B2D88(void* self, int param) {
    extern void func_800B2DB0(void*, void*);
    func_800B2DB0(self, &param);
}

void func_800B2DB0(){}

void func_800B2E38(){}

void func_800B2ED0(){}

int func_800B31B0(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int func_800B31BC(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int func_800B31C8(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int func_800B31D4(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int func_800B31E0(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int func_800B31EC(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

int func_800B31F8(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

u32 func_800B3204(u32 val) { return (val >> 16) & 0xFF; }

void func_800B3210(){}

u32 func_800B39C0(void* self) { return *(u32*)((u8*)self + 0x74); }

void func_800B39C8(){}

void func_800B3A54(int* dst, int* src) {
    *dst = *src;
}

void func_800B3A60(void* self, int param) {
    extern void func_800B99EC(void*, void*);
    func_800B99EC(self, &param);
}

void func_800B3A88(){}

u32 func_800B3D34(u32 val) { return val >> 24; }

u32 func_800B3D40(u32 val) { return (val >> 16) & 0xFF; }

void func_800B3D4C() {}

void func_800B3D64(){}

void func_800B4120(){}

void func_800B4278(){}

void func_800B42E8(){}

void func_800B4368(){}

void func_800B4400(){}

void func_800B44A0(){}

void func_800B4554(){}

void func_800B4588(u32 val) {}

u32 func_800B4594(u32 val) { return (val >> 16) & 0xFF; }

void func_800B45A0(){}

void func_800B47A8(){}

void func_800B4A24(){}

int CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv(void* self) { return 0; }

void func_800B4B0C(){}

void func_800B4B74(){}

void func_800B4B88(){}

void func_800B4CA0(){}

void func_800B4D84(){}

u32 func_800B4F58(u32 val) { return (val >> 16) & 0xFF; }

extern u16 lbl_eu_8066408C;
unsigned short func_800B4F64(){
    return lbl_eu_8066408C;
}

void func_800B4F6C() {}

void func_800B4F80() {}

void func_800B4F90() {}

u32 func_800B4FA4(void* self) { return *(u32*)((u8*)self + 0x70); }

void func_800B4FAC(){}

void func_800B535C(){}

void func_800B570C(){}

void func_800B587C(){}

void func_800B5944() {}

void func_800B5948() {}

void func_800B5958(void* self) { *(u32*)((u8*)self + 896) = 0; }

void* func_800B5964(void* self) { return 0; }

u32 func_800B5970(void* self) { return *(u32*)((u8*)self + 0x380); }

void func_800B5978(){}

void func_800B5994(){}

void func_800B6484() {}

void func_800B6494() {}

u32 func_800B64AC(u32 val) { return (val >> 16) & 0xFF; }

void func_800B64B8() {}

u32 func_800B64D0(u32 a, u32 b) { return a & b; }

u32 func_800B64DC(void* self) { return *(u32*)((u8*)self + 0x98); }

u16 func_800B64E4(void* self) { return *(u16*)((u8*)self + 0x45C0); }

u32 func_800B64EC(u32 val) { return (val >> 16) & 0xFF; }

void func_800B64F8() {}

void func_800B6508() {}

void func_800B6520(void* self, void* other) {
    unsigned int count = *(unsigned int*)((unsigned char*)self + 0x700);
    unsigned int val = *(unsigned int*)other;
    unsigned int newCount = count + 1;
    *(unsigned int*)((unsigned char*)self + 0x700) = newCount;
    float fval = *(float*)((unsigned char*)other + 4);
    void* dest = (unsigned char*)self + count * 8;
    *(unsigned int*)dest = val;
    *(float*)((unsigned char*)dest + 4) = fval;
}

void func_800B6544() {}

void func_800B655C(){}

void func_800B66AC() {}

void func_800B66BC(){}

void func_800B67CC(){}

void* func_800B67EC(void* self) { return (void*)((u8*)self + 0x1a8); }

void func_800B67F4(void* self) { *(u32*)((u8*)self + 1792) = 0; }

void func_800B6800(){}

void func_800B68A8(){}

void func_800B6AF4(){}

void func_800B6BA0(void){}

void* func_800B6BA4__Fv() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB28;
}

void* func_800B6BC8() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB48;
}

void* func_800B6BEC() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB68;
}

void* func_800B6C10() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB88;
}

void* func_800B6C34() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xBE8;
}

void* func_800B6C58() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xBC8;
}

void* func_800B6C7C() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xC08;
}

void* func_800B6CA0() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xC48;
}

void func_800B6CC4(){}

void func_800B6CF8(){}

void* func_800B6D3C(void*);
void func_800B6DD0(){}

void func_800B6EC0(){}

u32 func_800B6FFC(void* self) { return *(u32*)((u8*)self + 0xB00); }

u32 func_800B7004(void* self) { return *(u32*)((u8*)self + 0xB04); }

void* func_800B700C(void* self, u32 index) {
    u32 start = *(u32*)((u8*)self + 0xAFC);
    u32 count = *(u32*)((u8*)self + 0xB04);
    u32* base = *(u32**)((u8*)self + 0xAF8);
    return &base[(start + index) % count];
}

void func_800B7034() {
    func_800B6D3C(func_800B07E8());
}

void func_800B7058(){}

void func_800B708C__Fi(){}

void func_800B70FC(){}

u32 func_800B71C4(void) {
    extern u32 lbl_eu_806640F4;
    return lbl_eu_806640F4;
}

void func_800B71CC(){}

void* func_800B720C(void* self) { return (void*)((u8*)self + 0x720); }

void func_800B7214(){}

void func_800B72DC(){}

void func_800B7320(){}

void func_800B73E8(void* out_prev, void* unused, void* list_ptr) {
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

void func_800B7410(){}

unsigned short func_800B75B4(){
    return lbl_eu_8066408C;
}

unsigned short func_800B75BC(){
    return lbl_eu_8066408C;
}

void func_800B75C4(void* self, u32 mask, int enable) {
    u32* field = (u32*)((u8*)self + 0x6C);
    if (enable) {
        *field |= mask;
    } else {
        *field &= ~mask;
    }
}

void func_800B75EC(){}

int func_800B7680(void* self){
    void* head = *(void**)((char*)self + 4);
    void* cur = *(void**)head;
    int count = 0;
    while (cur != head) {
        cur = *(void**)cur;
        count++;
    }
    return count;
}

void func_800B76A4(void* self) {
    void* ptr = func_800B07E8();
    func_800B6D3C((void*)((u8*)ptr + 0xB28));
}

void func_800B76CC(void* self) {
    void* ptr = func_800B07E8();
    func_800B6D3C((void*)((u8*)ptr + 0xB48));
}

void func_800B76F4(void* self) {
    void* ptr = func_800B07E8();
    func_800B6D3C((void*)((u8*)ptr + 0xB68));
}

void* func_800B771C(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xB88);
}

void* func_800B7744(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xBE8);
}

void* func_800B776C(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xBC8);
}

void* func_800B7794(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xC08);
}

void* func_800B77BC(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xBA8);
}

void func_800B77E4(){}

void func_800B781C(){}

void func_800B7854(){}

void func_800B788C(void* param_1) { extern void* func_800B07E8(); extern void func_800B6DD0(void*, void*); void* ptr = func_800B07E8(); func_800B6DD0((char*)ptr + 0xb88, param_1); }

void func_800B78C4(){}

void func_800B78FC(){}

void func_800B7934(){}

void func_800B796C(){}

void func_800B79A4(){}

void func_800B7A18(){}

void func_800B7AF0(){}

void func_800B83AC(){}

void func_800B8524(){}

void func_800B87FC() {}

void func_800B8800() {}

void func_800B8804__FPvPQ22cf13IFactoryEvent(){}

void func_800B88E0(){}

void func_800B8920(){}

void func_800B89CC(){}

void func_800B8A64(){}

void func_800B8AFC(){}

void func_800B8B94(){}

void func_800B8C78(){}

void func_800B8D5C(){}

void func_800B8FC4(){}

void func_800B92B4() {
    func_800B07E8();
    ((void(*)())func_800B1120)();
}

void func_800B92D8() {
    func_800B07E8();
    ((void(*)())func_800B15A4)();
}

void func_800B92FC(){}

void func_800B9340() {
    func_800B07E8();
    ((void(*)())func_800B87FC)();
}

void func_800B9364() {
    func_800B07E8();
    ((void(*)())func_800B8800)();
}

void func_800B9388() {
    func_800B07E8();
    ((void(*)())func_800B1954)();
}

void func_800B93AC() {
    func_800B07E8();
    ((void(*)())func_800B1B2C)();
}

void func_800B93D0(){}

void func_800B9404(){}

void func_800B9438(){}

void func_800B946C(){}

void func_800B94A0(){}

void func_800B94D4(){}

void func_800B9524(u32 a, u32 b) {
    func_800AA2BC(a, b);
    ((void(*)())func_800B94D4)();
}

void func_800B9548(){}

void func_800B957C(){}

void func_800B96C4(void* self, u32 val) { *(u32*)((u8*)self + 0x734) = val; }

void func_800B96CC(void* self, u8 a, u8 b) { ((u8*)self)[1850] = a; ((u8*)self)[1851] = b; }

void func_800B96D8(void* self, u32 val) { *(u32*)((u8*)self + 0x720) = val; }

void func_800B96E0(void* self, u16 val) { *(u16*)((u8*)self + 0x738) = val; }

void func_800B96E8() {}

void func_800B9704(void* self, float val) { *(float*)((u8*)self + 0x730) = val; }

void __ct__800B970C(){}

void func_800B97A0(){}

void func_800B985C(){}

void func_800B98C8(){}

u32 func_800B9984(void* self) { return *(u32*)((u8*)self + 0x620); }

void func_800B998C(){}

void func_800B99BC(){}

void func_800B99EC(){}

void func_800B9A30() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
namespace cf { struct TboxInfo { char _pad; }; }
namespace cf { struct IFactoryEvent { char _pad; }; }
template <typename T> class _reslist_base { public: ~_reslist_base(); };
template <typename T> class reslist { public: ~reslist(); };
template <> _reslist_base<cf::TboxInfo>::~_reslist_base() {}
template <> reslist<cf::TboxInfo>::~reslist() {}
template <> _reslist_base<cf::IFactoryEvent*>::~_reslist_base() {}
template <> reslist<cf::IFactoryEvent*>::~reslist() {}
void sinit_800B9A40(){}

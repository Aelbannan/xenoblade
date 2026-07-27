// Auto-scaffolded catalog TU for kyoshin/cf/CfRes
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void func_80061870(){}

struct CfResBuffer {
    u8 buffer[0x400];
    u32 field_400;
    u32 field_404;
};

int func_80061A80(struct CfResBuffer* self, unsigned char byte1, unsigned short halfword, unsigned int dataVal, unsigned int* src, int count, unsigned int headerBits) {
    unsigned int usedSize = self->field_404;
    unsigned int total = usedSize + count + 2;
    if (total >= 0x100) {
        return 0;
    }

    unsigned int writeIdx = self->field_400;
    unsigned int newIdx = (writeIdx - count - 2) & 0xFF;
    self->field_400 = newIdx;

    unsigned int h = headerBits & 0xFF0FFFFF;
    h = (h & 0x00FFFFFF) | ((unsigned int)byte1 << 24);
    h = (h & 0xFFFF0000) | (unsigned int)halfword;
    h &= 0xFFF7FFFF;
    h = (h & 0xFFF0FFFF) | (((unsigned int)count & 0xF) << 20);

    unsigned int* buf = (unsigned int*)self->buffer;
    buf[newIdx] = h;
    buf[(newIdx + 1) & 0xFF] = dataVal;

    if (count > 0) {
        unsigned int i = 0;
        unsigned int off = 2;
        while (i < (unsigned int)count) {
            buf[(newIdx + off) & 0xFF] = src[i];
            i++;
            off++;
        }
    }

    self->field_404 = usedSize + count + 2;
    return 1;
}

void func_80061C5C(){}

void func_80061D2C(){}

void func_80061E8C(){}

extern u32 lbl_eu_80663D80;
extern "C" int func_80061FD8() { return lbl_eu_80663D80; }

extern u32 lbl_eu_80663D80;
extern "C" u32 func_80061FE0(u32 val) { lbl_eu_80663D80 = val; return val; }

void func_80061FE8(){
    extern u32 lbl_eu_80663D80;
    extern void func_80495FF0();
    extern void* getHandleMEM1__Q23mtl10MemManagerFv();
    if (lbl_eu_80663D80) {
        func_80495FF0();
    }
    getHandleMEM1__Q23mtl10MemManagerFv();
}

void func_80061FFC(){
    extern u32 lbl_eu_80663D80;
    extern void func_80496004();
    extern void* getHandleMEM1__Q23mtl10MemManagerFv();
    if (lbl_eu_80663D80) {
        func_80496004();
    }
    getHandleMEM1__Q23mtl10MemManagerFv();
}

// Use explicit default return to match retail pattern
extern u32 lbl_eu_80663D7C;
extern "C" int func_80062010() {
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    return ret;
}

extern u32 lbl_eu_80663D7C;
extern "C" int CfRes_getInstance() { return lbl_eu_80663D7C; }

extern "C" {
extern u32 lbl_eu_80663D7C;
extern int func_80067E78(int, int);
}
extern "C" int func_80062030() {
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    if (ret) {
        return func_80067E78(ret, 1);
    }
    return ret;
}

extern "C" {
extern u32 lbl_eu_80663D7C;
extern int func_80067F10(int, int);
}
extern "C" int func_80062058() {
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    if (ret) {
        return func_80067F10(ret, 1);
    }
    return ret;
}

extern u32 lbl_eu_80663D7C;
extern "C" int func_80068078(int);
extern "C" int func_80062080() {
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    if (ret) {
        return func_80068078(ret);
    }
    return ret;
}

extern "C" {
extern u32 lbl_eu_80663D7C;
extern int func_80068110(int, void*);
}
extern "C" int func_800620A4(void* self) {
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    if (ret) {
        return func_80068110(ret, self);
    }
    return ret;
}

extern u32 lbl_eu_80663D7C;
extern "C" int func_80068254(int);
extern "C" int func_800620CC() {
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    if (ret) {
        return func_80068254(ret);
    }
    return ret;
}

u32 func_800620F0(){
    extern u32 lbl_eu_80663D7C;
    extern u32 func_80067FE0(u32);
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    if (ret == 0) {
        return 0;
    }
    return func_80067FE0(ret);
}

void func_80062114(){}

void func_800621A0(){}

void func_800621F4(){}

extern "C" void func_800623D0(void* a, void* b, void* c) {
    extern void func_8009CE14(void*, void*, unsigned long, void*);
    func_8009CE14(a, b, 2, c);
}

void func_800623DC(){}

void func_80062430(){}

void func_800624A8(){}

u32 func_8006251C(void* self){ return ((u32)(uintptr_t)self >> 16) & 0x1F; }

u32 func_80062524(void* self){ return ((u32)(uintptr_t)self >> 16) & 0x1F; }

void func_8006252C(){}

void func_800625A0(){}

void func_80062600(){}

extern u32 lbl_eu_80663E24;
extern "C" int func_80062648() {
    return (lbl_eu_80663E24 & 0x00048000) != 0 ? 1 : 0;
}

extern u32 lbl_eu_80663E24;
extern "C" int func_80062664() {
    return (lbl_eu_80663E24 & 0x02000400) != 0 ? 1 : 0;
}

void func_80062680(){}

void func_800626F4(){}

void func_eu_80062E58(){}

void func_80062758(){}

void func_800627BC(){}

void func_800627FC(){}

void func_80062860(){}

void func_800628C4(){}

void func_80062928(){}

extern "C" u32 func_80062990(void* self) { return *(u32*)((u8*)self + 0x18); }

void func_80062998(){}

void func_eu_80063174(){}

void func_80062A00(){}

void func_80062A84(){}

void func_80062AD8(){}

void func_80062B3C(){}

extern "C" void func_80062BA0(void* a, void* b, void* c) {
    extern void func_800643F0(void*, void*, unsigned long, void*);
    func_800643F0(a, b, 1, c);
}

void func_80062BAC(){}

extern "C" void func_80062C10(void* a, void* b, void* c) {
    extern void func_800643F0(void*, void*, unsigned long, void*);
    func_800643F0(a, b, 0, c);
}

extern "C" void func_80062C1C(unsigned long a, const char* b, void* c) {
    extern void readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, char const*, void*, int, int);
    readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(a, b, c, 0, 0);
}

void func_80062C28(){}

bool func_80062C80(){ return false; }

void func_80062C88(){}

extern "C" void* func_80062CD0(void* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 12) * 60 + 8);
}

void func_80062CE4(){}

extern "C" void* func_80062D2C(void* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 21) * 60 + 8);
}

void func_80062D44(){}

extern "C" void* func_80062D8C(void* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 22) * 60 + 8);
}

void func_80062DA4(){}

extern "C" void* func_80062DEC(void* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 18) * 60 + 8);
}

void func_80062E04(){}

extern "C" void* func_80062E4C(void* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 19) * 60 + 8);
}

void func_80062E64(){}

extern "C" void* func_80062EAC(void* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 20) * 60 + 8);
}

void func_80062EC4(){}

bool func_80062F10(){ return false; }

void func_80062F18(){}

bool func_80062F50(){ return false; }

bool func_80062F58(){ return false; }

void func_80062F60(){}

bool func_80062F98(){ return false; }

bool func_80062FA0(){ return false; }

void func_80062FA8(){}

bool func_80062FE0(){ return false; }

bool func_80062FE8(){ return false; }

void func_80062FF0(){}

bool func_80063028(){ return false; }

bool func_80063030(){ return false; }

void func_80063038(){}

bool func_80063070(){ return false; }

bool func_80063078(){ return false; }

void func_80063080(){}

bool func_800630B8(){ return false; }

bool func_800630C0(){ return false; }

void func_800630C8(){}

bool func_80063100(){ return false; }

bool func_80063108(){ return false; }

bool func_80063110(){ return false; }

bool func_80063118(){ return false; }

void func_80063120(){}

bool func_80063158(){ return false; }

void func_80063160(){}

extern "C" void CfResEntry_init(void* self) {
    *(int*)((char*)self + 0) = 0;
    *(int*)((char*)self + 4) = 0;
    *(int*)((char*)self + 8) = 0;
}

extern "C" void* CfRes_getResEntry(void* self, u32 idx) { return (u8*)self + idx * 12; }

void func_800631FC(){}

extern "C" void* CfRes_getTblEntry(void* self, u32 idx) { return (u8*)self + idx * 12; }

extern "C" u32 CfResEntry_getHandle(void* self) { return *(u32*)((u8*)self + 0x0); }

void func_8006328C(){}

void func_80063310(){}

void func_80063394(){}

extern "C" u32 CfResEntry_getField4(void* self) { return *(u32*)((u8*)self + 0x4); }

extern "C" void CfResEntry_setHandle(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }

void func_8006349C(){}

extern "C" u32 CfResEntry_getField8(void* self) { return *(u32*)((u8*)self + 0x8); }

extern "C" void CfResEntry_setField4(void* self, u32 val) { *(u32*)((u8*)self + 0x4) = val; }

extern "C" void CfResEntry_clearField8(void* self) { *(u32*)((u8*)self + 8) = 0; }

void func_80063560(){}

extern "C" void func_800638A8(void* self) { *(u32*)((u8*)self + 4) = 0; }

void func_800638B4(){}

void func_80063900(){}

void func_8006398C(){}

extern "C" void func_80063990() {}

void func_80063994(){}

int func_80063A34(void* self){
    extern int getFileSize__11CDeviceFileFPCc(void*, int);
    int sz = getFileSize__11CDeviceFileFPCc(self, 1);
    return (sz < 0) ? -1 : sz;
}

void func_80063A60(){}

extern "C" void func_80063ACC() {}

void func_80063AD0(){}

extern "C" void func_80063BDC(void* self, int a, int b, int c, int d) {
    *(int*)((char*)self + 4) = a;
    *(int*)((char*)self + 0x28) = b;
    *(int*)((char*)self + 8) = c;
    *(int*)((char*)self + 0x18) = d;
}

extern "C" void func_80063BF0(void* self) {
    int val = *(int*)self;
    *(int*)self = (val & ~2) | 1;
}

// RTTI on: omit _v000/_v004 so _v034 lands at retail 0x34 (offset 52).
struct CfResSub_63C04 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010();
    virtual void _v014(); virtual void _v018(); virtual void _v01C();
    virtual void _v020(); virtual void _v024(); virtual void _v028();
    virtual void _v02C(); virtual void _v030(); virtual void* _v034(void* outer);
};
struct CfResObj_63C04 { u8 _00[0x2C]; CfResSub_63C04* sub; };
extern "C" void* func_80063C04(void* self) {
    CfResSub_63C04* sub = ((CfResObj_63C04*)self)->sub;
    if (!sub) return self;
    return sub->_v034(self);
}

extern "C" void func_80063C2C() {}

extern "C" void func_80063C30(void* self) {
    u32 v = *(u32*)self;
    u32 z = 0;
    u32 m = 0xFFFFFFAE;
    u32 r = (v & m) | 2;
    *(u32*)((char*)self + 0x28) = z;
    *(u32*)self = r;
}

// RTTI on: omit _v000/_v004 so _v038 lands at retail 0x38 (offset 56).
struct CfResSub_63C50 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010();
    virtual void _v014(); virtual void _v018(); virtual void _v01C();
    virtual void _v020(); virtual void _v024(); virtual void _v028();
    virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void* _v038(void* outer);
};
struct CfResObj_63C50 { u8 _00[0x2C]; CfResSub_63C50* sub; };
extern "C" void* func_80063C50(void* self) {
    CfResSub_63C50* sub = ((CfResObj_63C50*)self)->sub;
    if (!sub) return self;
    return sub->_v038(self);
}

extern "C" void func_80063C78() {}

void func_80063C7C(){}

extern "C" int func_80063DB0(const char* s1, const char* s2) {
    return strcmp(s1, s2) == 0 ? 1 : 0;
}

extern "C" unsigned long func_80063DD8() {
    return (lbl_eu_80663E24 >> 22) & 1;
}

extern "C" int func_80063DE4() {
    extern u32 lbl_eu_80663E24;
    return (lbl_eu_80663E24 >> 18) & 1;
}

// C++ virtual thunk with struct access (14 dummies + RTTI = offset 64)
struct CfResObj_63DF0 { u8 _00[0x2C]; struct CfResSub_63DF0* sub; };
struct CfResSub_63DF0 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14(void* self); };
extern "C" void func_80063DF0(void* self) {
    ((CfResObj_63DF0*)self)->sub->m14(self);
}

struct CfResSub_63E08 { virtual void m02(void* self, void* arg); };
struct CfResObj_63E08 { u8 _00[0x2C]; CfResSub_63E08* sub; };
extern "C" void func_80063E08(void* self, void* arg) {
    ((CfResObj_63E08*)self)->sub->m02(self, arg);
}

extern "C" u32 func_80063E28(void* self) { return ((u32)(uintptr_t)self >> 27) & 0x1F; }

void func_80063E30(){}

void func_80063F1C(){}

void func_80063FA8(){}

void func_80064014(){}

void func_800640F4(){}

void func_8006414C(){}

extern "C" u32 func_800641C4(void* self) { return (u32)(uintptr_t)self & 0x3FF; }

void func_800641CC(){}

extern "C" unsigned long func_80064360(unsigned long a, unsigned long b) {
    return (a & 0x7FFFFFF) | (b << 27);
}

// C++ virtual thunk with struct access (4 dummies + RTTI = offset 24)
struct CfResObj_64370 { u8 _00[0x2C]; struct CfResSub_64370* sub; };
struct CfResSub_64370 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(void* self); };
extern "C" void func_80064370(void* self) {
    ((CfResObj_64370*)self)->sub->m04(self);
}

extern "C" bool func_80064388() { return false; }

// C++ virtual thunk with struct access (5 dummies + RTTI = offset 28)
struct CfResObj_64390 { u8 _00[0x2C]; struct CfResSub_64390* sub; };
struct CfResSub_64390 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(void* self); };
extern "C" void func_80064390(void* self) {
    ((CfResObj_64390*)self)->sub->m05(self);
}

extern "C" bool func_800643A8() { return false; }

// C++ virtual thunk with struct access (8 dummies + RTTI = offset 40)
struct CfResObj_643B0 { u8 _00[0x2C]; struct CfResSub_643B0* sub; };
struct CfResSub_643B0 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(void* self); };
extern "C" void func_800643B0(void* self) {
    ((CfResObj_643B0*)self)->sub->m08(self);
}

extern "C" bool func_800643C8() { return false; }

// C++ virtual thunk with struct access (7 dummies + RTTI = offset 36)
struct CfResObj_643D0 { u8 _00[0x2C]; struct CfResSub_643D0* sub; };
struct CfResSub_643D0 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(void* self); };
extern "C" void func_800643D0(void* self) {
    ((CfResObj_643D0*)self)->sub->m07(self);
}

extern "C" bool func_800643E8() { return false; }

void func_800643F0(){}

extern "C" void func_80064968(unsigned long mask) {
    extern unsigned long lbl_eu_80663E28;
    lbl_eu_80663E28 &= ~mask;
}

extern "C" void func_80064978(void* self) {
    extern void func_80065CA4(void* a, void* b);
    func_80065CA4(*(void**)((u32)self + 0x2C), self);
}

extern "C" unsigned long func_80064984(void* self) {
    unsigned long v = *(unsigned long*)((char*)self + 4);
    return v == 0 ? 1 : 0;
}

struct CfRes_64994 {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u8 _0C[0x14];
    u32 field_20;
    u32 field_24;
    u32 field_28;
};
extern "C" void func_80064994(CfRes_64994* self) {
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_28 = 0;
    self->field_00 = 0;
    self->field_24 = 0;
    self->field_20 = 0;
}

extern "C" void func_800649B4(void* self, u32 bits) {
    *(u32*)self |= bits;
}

extern u32 lbl_eu_80663E14;
extern "C" int func_800649C4() { return lbl_eu_80663E14; }

extern "C" void func_800649CC(void* self, u32 bits) {
    *(u32*)self |= bits;
}

extern u32 lbl_eu_80663E30;
extern "C" int func_800649DC() { return lbl_eu_80663E30; }

extern "C" void func_800649E4(u32 bits) {
    extern u32 lbl_eu_80663E28;
    lbl_eu_80663E28 |= bits;
}

void func_800649F4(void* self){
    void* obj = *(void**)((char*)self + 0x2C);
    void (*func)(void*) = *(void (**)(void*))((char*)(*(void**)obj) + 0xC);
    func(obj);
}

extern "C" int func_80064A08(void* self, u32 mask) {
    u32 val = *(u32*)self;
    return (val & mask) != 0 ? 1 : 0;
}

void CfRes::OnFileEvent() {}

extern "C" u32 func_80064A6C(void* self) { return *(u32*)((u8*)self + 0x4); }

void func_80064A74(){}

extern "C" u32 func_80064B70(void* self) { return (u32)(uintptr_t)self & 0x3FF; }

void func_80064B78(){}

// 15 dummies (m00-m14) span indices 2-16 with MWCC overhead; m17 at index 17 = offset 68
struct CfResSub_64CB8 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11();
    virtual void m12(); virtual void m13(); virtual void m14();
    virtual void m17(void* self, void* arg);
};
struct CfResObj_64CB8 { u8 _00[0x2C]; CfResSub_64CB8* sub; };
extern "C" void func_80064CB8(void* self, void* arg) {
    ((CfResObj_64CB8*)self)->sub->m17(self, arg);
}

void func_80064CD8(){}

void func_eu_80065590(){}

extern "C" u32 func_eu_80065640(u32 a, u32 b, u32 c, u32 d) {
    u32 t0 = (c << 10) & 0xFFFFFC00;
    u32 t1 = (b << 20) & 0xFFF00000;
    u32 t2 = (a << 27) & 0xF8000000;
    u32 t3 = d | t0;
    u32 t4 = t2 | t1;
    return t3 | t4;
}

void func_80064DC4(){}

void func_80064EB0(){}

extern "C" u32 func_80064F58(void* self) { return *(u32*)((u8*)self + 0x18); }

extern "C" void func_80064F60(void* self) {
    u32 val = *(u32*)self;
    *(u32*)self = (val & ~0x42) | 0x11;
}

void func_80064F78(){}

void func_80065050(){}

void func_80065158(){}

void func_80065254(){}

extern "C" unsigned long func_800652FC(unsigned long a, unsigned long b, unsigned long c) {
    return ((a & 0x1F) << 27) | ((b & 0xFFF) << 20) | ((c & 0x3FFFFF) << 10);
}

void func_80065314(){}

void func_eu_80065C7C(){}

bool func_eu_80065D60(){ return false; }

void func_800653E4(){}

void func_800654B4(){}

void func_800655C4(){}

void func_80065694(){}

extern "C" int CfResEntry_incRefCount(void* self) {
    return ++*(int*)((char*)self + 4);
}

struct CfRes_65818 {
    u32 field_00;
    int field_04;
};
extern "C" int CfResEntry_decRefCount(CfRes_65818* self) {
    if (self->field_04 > 0) {
        self->field_04--;
    }
    return self->field_04;
}

extern "C" int func_80065834(void* self) {
    return ++*(int*)((char*)self + 8);
}

void cf::CfRes::~CfRes() {}

void cf::CfResTask::~CfResTask() {}

extern "C" void Init__Q22cf9CfResTaskFv() {}

void cf::CfResTask::Term() {}

extern "C" void Move__Q22cf9CfResTaskFv() {}

cf::CfRes::CfRes() {}

extern "C" void Draw__Q22cf9CfResTaskFv() {}

extern "C" char* func_80065BE4(char* buffer, const char* suffix) {
    unsigned int length = (unsigned int)strlen(suffix);
    strcat(buffer, suffix);
    *(unsigned int*)(buffer + 0x40) += length;
    return buffer;
}

extern "C" int func_80065C48(int dummy, void* self) {
    return *(int*)((char*)self + 40) != 0 ? 1 : 0;
}

extern "C" int func_80065C5C(void* self, void* obj) {
    int ret = 0;
    if (*(u32*)((u8*)obj + 4)) {
        if (*(u32*)((u8*)obj + 0x28) == 0) {
            ret = 1;
        }
    }
    return ret;
}

extern "C" int func_80065C80(void* unused, const void* obj, u32 val) {
    u32 field = *(const u32*)((const u8*)obj + 4);
    if (field != 0 && field == val) return 1;
    return 0;
}

void func_80065CA4(){}

void func_80065D00(){}

void func_80065D04(){}

void func_80065D08(){}

void func_80065D0C(){}

// C++ virtual call forces MWCC to use r12 for vtable dispatch
// 14 dummies + RTTI overhead = offset 64 at vtable+0x40
struct CfResVtabClass { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11(); virtual void m13(); virtual void m14(); virtual void m16(); };
extern "C" void func_80065D64(void* self) {
    ((CfResVtabClass*)self)->m16();
}

void func_80065D74(){}

extern "C" void func_80065D78() {}

extern "C" void func_80065D7C() {}

void func_80065D80(){}

void func_80065D84(){}

void func_80065D88(){}

extern "C" void func_80065D8C() {}

void func_80065D90(){}

extern "C" u32 func_80065E00(u32 unused, void* obj) { return *(u32*)((char*)obj + 24); }

extern "C" void* func_80065E08(void* unused, const void* obj) {
    u8 val = *(const u8*)((const u8*)obj + 0x32);
    if (val == 10) {
        extern void* func_800A9B50();
        return func_800A9B50();
    }
    if (val == 7) {
        extern void* func_800A9C10();
        return func_800A9C10();
    }
    return 0;
}

extern "C" int func_801BFA64(int);
extern "C" int func_80065E2C(void* self, void* param) {
    u8 val = *(u8*)((char*)param + 0x32);
    if (val == 10) {
        return func_801BFA64(0);
    }
    if (val == 7) {
        return func_801BFA64(1);
    }
    return (int)self;
}

void func_80065E54(){}

void func_80065F18(){}

void func_80065F1C(){}

extern "C" void func_80065F20() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

namespace cf { class CfResTask; }
template<> CTTask<cf::CfResTask>::~CTTask() {}
template<> void CTTask<cf::CfResTask>::Move() {}
template<> void CTTask<cf::CfResTask>::Draw() {}

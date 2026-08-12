// Auto-scaffolded catalog TU for kyoshin/cf/CfRes
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CfRes.hpp"
extern "C" char* getEntryPtr(char* base, int a, int b);
extern "C" char* getEntryPtrGrid(char* self, int a, int b);
extern "C" void* func_80495FF0(void* scene);
extern "C" mtl::ALLOC_HANDLE func_80496004(void* src);
extern "C" void* getHandleMEM1__Q23mtl10MemManagerFv();
void func_80061870(){}

int CfResBuffer::func_80061A80(unsigned char byte1, unsigned short halfword, unsigned int dataVal, unsigned int* src, int count, unsigned int headerBits) {
    unsigned int usedSize = field_404;
    unsigned int total = usedSize + count + 2;
    if (total >= 0x100) {
        return 0;
    }

    unsigned int writeIdx = field_400;
    unsigned int newIdx = (writeIdx - count - 2) & 0xFF;
    field_400 = newIdx;

    unsigned int h = headerBits & 0xFF0FFFFF;
    h = (h & 0x00FFFFFF) | ((unsigned int)byte1 << 24);
    h = (h & 0xFFFF0000) | (unsigned int)halfword;
    h &= 0xFFF7FFFF;
    h = (h & 0xFFF0FFFF) | (((unsigned int)count & 0xF) << 20);

    unsigned int* buf = (unsigned int*)buffer;
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

    field_404 = usedSize + count + 2;
    return 1;
}

void func_80061C5C(){}

void func_80061D2C(){}

void func_80061E8C(){}

extern u32 lbl_eu_80663D80;
int CfRes_getD80Flag() { return lbl_eu_80663D80; }

extern u32 lbl_eu_80663D80;
u32 CfRes_setD80Flag(u32 val) { lbl_eu_80663D80 = val; return val; }

// retail: lwz r3,lbl; cmpwi; beq; b func_80495FF0 / b getHandleMEM1 (tail calls)
extern "C" void* func_80061FE8() {
    if (lbl_eu_80663D80 != 0) { return func_80495FF0((void*)lbl_eu_80663D80); }
    return (void*)getHandleMEM1__Q23mtl10MemManagerFv();
}

extern "C" mtl::ALLOC_HANDLE func_80061FFC() {
    if (lbl_eu_80663D80 != 0) { return func_80496004((void*)lbl_eu_80663D80); }
    return (mtl::ALLOC_HANDLE)(uintptr_t)getHandleMEM1__Q23mtl10MemManagerFv();
}

// Use explicit default return to match retail pattern
extern u32 lbl_eu_80663D7C;
int CfRes_getInstanceField() {
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    return ret;
}

extern u32 lbl_eu_80663D7C;
#pragma push
#pragma auto_inline off
extern "C" int CfRes_getInstance() { return lbl_eu_80663D7C; }
#pragma pop

extern u32 lbl_eu_80663D7C;
extern int func_80067E78(int, int);
extern "C" __declspec(noinline) int CfRes_callFunc_67E78() {
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

extern u32 lbl_eu_80663D7C;
extern int func_80067F10(int, int);
extern "C" int CfRes_callFunc_67F10() {
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
extern "C" int CfRes_callFunc_68078() {
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
extern "C" int CfRes_callFunc_68110(u8* self) {
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
extern "C" int CfRes_callFunc_68254() {
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

extern "C" __declspec(noinline) void func_800620F0(){
    extern u32 lbl_eu_80663D7C;
    extern void func_80067FE0(u32);
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    if (ret != 0) {
        func_80067FE0(ret);
    }
}

void func_80062114(){}

// func_800621A0: return the +4 count of a string-keyed record when its
// inline key matches the global name key (lbl_eu_80661A24), else 0.
int func_800621A0(CfResNameRec* rec) {
    if (rec != 0) {
        if (strcmp(lbl_eu_80661A24, rec->mKey) == 0) {
            return rec->mCount;
        }
    }
    return 0;
}

void func_800621F4(){}

extern "C" void CfRes_delegateOp2(void* a, void* b, void* c) {
    extern void func_8009CE14(void*, void*, unsigned long, void*);
    func_8009CE14(a, b, 2, c);
}

// func_800623DC: resolve a resource via the CfRes manager; returns the
// resolved size/state or -1 when the manager is unavailable.
int func_800623DC(u8* res) {
    int ret = -1;
    if (CfRes_getInstance() != 0) {
        CfRes_getInstance();
        ret = func_80063A60(res);
    }
    return ret;
}

void func_80062430(){}

void func_800624A8(){}

u32 func_8006251C(void* self){ return ((u32)(uintptr_t)self >> 20) & 0x7F; }

u32 func_80062524(void* self){ return ((u32)(uintptr_t)self >> 10) & 0x3FF; }

void func_8006252C(){}

void func_800625A0(){}

// func_80062600: run the update pipeline only while neither busy flag is set
extern "C" int CfRes_checkFlags_48000();
extern "C" int CfRes_checkFlags_2000400();
void func_80062600() {
    if (CfRes_checkFlags_48000() == 0 && CfRes_checkFlags_2000400() == 0) {
        func_800A9068();
        CfRes_callFunc_67E78();
        func_800620F0();
        func_800A9134();
    }
}

extern u32 lbl_eu_80663E24;
extern "C" __declspec(noinline) int CfRes_checkFlags_48000() {
    return (lbl_eu_80663E24 & 0x00048000) != 0 ? 1 : 0;
}

extern u32 lbl_eu_80663E24;
extern "C" __declspec(noinline) int CfRes_checkFlags_2000400() {
    return (lbl_eu_80663E24 & 0x02000400) != 0 ? 1 : 0;
}

void func_80062680(){}

void func_800626F4(){}

void func_eu_80062E58(){}

void func_80062758(){}

// func_800627BC: register the resource with the CfRes manager if it exists
extern "C" void func_80065254(int inst, u8* arg);
void func_800627BC(u8* arg) {
    if (CfRes_getInstance() != 0) {
        func_80065254(CfRes_getInstance(), arg);
    }
}

void func_800627FC(){}

void func_80062860(){}

void func_800628C4(){}

void func_80062928(){}

extern "C" u32 CfRes_getField18(u8* self) { return *(u32*)((u8*)self + 0x18); }

void func_80062998(){}

// func_eu_80063174: forward a (index, ptr) pair into the manager's resolver
// when the CfRes manager exists.
void func_eu_80063174(int index, u8* ptr) {
    if (CfRes_getInstance() != 0) {
        func_eu_80065590(CfRes_getInstance(), index, ptr);
    }
}

void func_80062A00(){}

void func_80062A84(){}

void func_80062AD8(){}

void func_80062B3C(){}

extern "C" void CfRes_delegateOp1(void* a, void* b, void* c) {
    extern void func_800643F0(void*, void*, unsigned long, void*);
    func_800643F0(a, b, 1, c);
}

void func_80062BAC(){}

extern "C" void CfRes_delegateOp0(void* a, void* b, void* c) {
    extern void func_800643F0(void*, void*, unsigned long, void*);
    func_800643F0(a, b, 0, c);
}

extern "C" void CfRes_readCommonArchive(unsigned long a, const char* b, void* c) {
    extern void readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, char const*, void*, int, int);
    readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(a, b, c, 0, 0);
}

void func_80062C28(){}

// retail: addi r3,r3,4; b getEntryPtrGrid (3-arg tail call)
extern "C" char* func_80062C80(char* self, int a, int b) {
    return getEntryPtrGrid(self + 4, a, b);
}

// func_80062C88: return the manager's array-elem-12 slot if the manager exists
extern "C" void* CfRes_getArrayElem12(u8* self, int idx);
void* func_80062C88(int arg) {
    if (CfRes_getInstance() != 0) {
        return CfRes_getArrayElem12((u8*)CfRes_getInstance(), arg);
    }
    return 0;
}

#pragma push
#pragma auto_inline off
extern "C" void* CfRes_getArrayElem12(u8* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 12) * 60 + 8);
}
#pragma pop

// func_80062CE4: return the manager's array-elem-21 slot if the manager exists
extern "C" void* CfRes_getArrayElem21(u8* self, int idx);
void* func_80062CE4(int arg) {
    if (CfRes_getInstance() != 0) {
        return CfRes_getArrayElem21((u8*)CfRes_getInstance(), arg);
    }
    return 0;
}

#pragma push
#pragma auto_inline off
extern "C" void* CfRes_getArrayElem21(u8* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 21) * 60 + 8);
}
#pragma pop

// func_80062D44: return the manager's array-elem-22 slot if the manager exists
extern "C" void* CfRes_getArrayElem22(u8* self, int idx);
void* func_80062D44(int arg) {
    if (CfRes_getInstance() != 0) {
        return CfRes_getArrayElem22((u8*)CfRes_getInstance(), arg);
    }
    return 0;
}

#pragma push
#pragma auto_inline off
extern "C" void* CfRes_getArrayElem22(u8* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 22) * 60 + 8);
}
#pragma pop

// func_80062DA4: return the manager's array-elem-18 slot if the manager exists
extern "C" void* CfRes_getArrayElem18(u8* self, int idx);
void* func_80062DA4(int arg) {
    if (CfRes_getInstance() != 0) {
        return CfRes_getArrayElem18((u8*)CfRes_getInstance(), arg);
    }
    return 0;
}

#pragma push
#pragma auto_inline off
extern "C" void* CfRes_getArrayElem18(u8* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 18) * 60 + 8);
}
#pragma pop

// func_80062E04: return the manager's array-elem-19 slot if the manager exists
extern "C" void* CfRes_getArrayElem19(u8* self, int idx);
void* func_80062E04(int arg) {
    if (CfRes_getInstance() != 0) {
        return CfRes_getArrayElem19((u8*)CfRes_getInstance(), arg);
    }
    return 0;
}

#pragma push
#pragma auto_inline off
extern "C" void* CfRes_getArrayElem19(u8* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 19) * 60 + 8);
}
#pragma pop

// func_80062E64: return the manager's array-elem-20 slot if the manager exists
extern "C" void* CfRes_getArrayElem20(u8* self, int idx);
void* func_80062E64(int arg) {
    if (CfRes_getInstance() != 0) {
        return CfRes_getArrayElem20((u8*)CfRes_getInstance(), arg);
    }
    return 0;
}

#pragma push
#pragma auto_inline off
extern "C" void* CfRes_getArrayElem20(u8* self, int idx) {
    return (void*)((u8*)self + (idx * 11 + 20) * 60 + 8);
}
#pragma pop

// func_80062EC4: return func_80062F10(instance, arg, 0) if the manager exists
extern "C" char* func_80062F10(char* self, int a, int b);
char* func_80062EC4(int arg) {
    if (CfRes_getInstance() != 0) {
        return func_80062F10((char*)CfRes_getInstance(), arg, 0);
    }
    return 0;
}

// retail: addi r3,r3,4; b getEntryPtr (3-arg tail call)
#pragma push
#pragma auto_inline off
extern "C" char* func_80062F10(char* self, int a, int b) {
    return getEntryPtr(self + 4, a, b);
}
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void* func_80062F50(void* self) { return (char*)self + 0x80; }
#pragma pop

// func_80062F18: return the manager's +0x80 region if the CfRes manager exists
char* func_80062F18() {
    if (CfRes_getInstance() != 0) {
        return (char*)func_80062F50((char*)CfRes_getInstance());
    }
    return 0;
}

extern "C" void* func_80062F58(void* self) { return (char*)self + 0x7c; }

#pragma push
#pragma auto_inline off
extern "C" void* func_80062F98(void* self) { return (char*)self + 0xbc; }
#pragma pop

// func_80062F60: return the manager's +0xbc region if the CfRes manager exists
char* func_80062F60() {
    if (CfRes_getInstance() != 0) {
        return (char*)func_80062F98((char*)CfRes_getInstance());
    }
    return 0;
}

extern "C" void* func_80062FA0(void* self) { return (char*)self + 0xb8; }

#pragma push
#pragma auto_inline off
extern "C" void* func_80062FE0(void* self) { return (char*)self + 0xf8; }
#pragma pop

// func_80062FA8: return the manager's +0xf8 region if the CfRes manager exists
char* func_80062FA8() {
    if (CfRes_getInstance() != 0) {
        return (char*)func_80062FE0((char*)CfRes_getInstance());
    }
    return 0;
}

extern "C" void* func_80062FE8(void* self) { return (char*)self + 0xf4; }

#pragma push
#pragma auto_inline off
extern "C" void* func_80063028(void* self) { return (char*)self + 0x134; }
#pragma pop

// func_80062FF0: return the manager's +0x134 region if the CfRes manager exists
char* func_80062FF0() {
    if (CfRes_getInstance() != 0) {
        return (char*)func_80063028((char*)CfRes_getInstance());
    }
    return 0;
}

extern "C" void* func_80063030(void* self) { return (char*)self + 0x130; }

#pragma push
#pragma auto_inline off
extern "C" void* func_80063070(void* self) { return (char*)self + 0x170; }
#pragma pop

// func_80063038: return the manager's +0x170 region if the CfRes manager exists
char* func_80063038() {
    if (CfRes_getInstance() != 0) {
        return (char*)func_80063070((char*)CfRes_getInstance());
    }
    return 0;
}

extern "C" void* func_80063078(void* self) { return (char*)self + 0x16c; }

// func_80063080: return the manager's +0x29c region if the CfRes manager exists
extern "C" void* func_800630B8(void* self);
char* func_80063080() {
    if (CfRes_getInstance() != 0) {
        return (char*)func_800630B8((char*)CfRes_getInstance());
    }
    return 0;
}

extern "C" __declspec(noinline) void* func_800630B8(void* self) { return (char*)self + 0x29c; }

extern "C" void* func_800630C0(void* self) { return (char*)self + 0x298; }

// func_800630C8: return the manager's +0x224 region if the CfRes manager exists
extern "C" void* func_80063100(void* self);
char* func_800630C8() {
    if (CfRes_getInstance() != 0) {
        return (char*)func_80063100((char*)CfRes_getInstance());
    }
    return 0;
}

extern "C" __declspec(noinline) void* func_80063100(void* self) { return (char*)self + 0x224; }

extern "C" void* func_80063108(void* self) { return (char*)self + 0x220; }

extern "C" void* func_80063110(void* self) { return (char*)self + 0x25c; }

extern "C" void* func_80063118(void* self) { return (char*)self + 0x1a8; }

// func_80063120: stash the archive id, init the embedded subobject, and
// register it into both lookup tables with a -1 id
extern "C" void func_80063160(int arg);
extern "C" void func_800631FC(int arg);
void func_80063120(u8* self, int arg) {
    lbl_eu_80663D80 = arg;
    func_800676F8(self + 4);
    func_80063160(-1);
    func_800631FC(-1);
}

extern "C" void __dt__80067670(u8* self);
#pragma push
#pragma auto_inline off
extern "C" void func_80063158(u8* self) { __dt__80067670(self + 4); }
#pragma pop

extern "C" __declspec(noinline) void func_80063160(int arg){}

extern "C" void CfResEntry_init(u8* self) {
    *(int*)((char*)self + 0) = 0;
    *(int*)((char*)self + 4) = 0;
    *(int*)((char*)self + 8) = 0;
}

extern "C" void* CfRes_getResEntry(u8* self, u32 idx) { return (u8*)self + idx * 12; }

extern "C" __declspec(noinline) void func_800631FC(int arg){}

extern "C" __declspec(noinline) void* CfRes_getTblEntry(u8* self, u32 idx) { return (u8*)self + idx * 12; }

extern "C" u32 CfResEntry_getHandle(u8* self) { return *(u32*)((u8*)self + 0x0); }

void func_8006328C(){}

void func_80063310(){}

void func_80063394(){}

extern "C" u32 CfResEntry_getField4(u8* self) { return *(u32*)((u8*)self + 0x4); }

extern "C" void CfResEntry_setHandle(u8* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }

void func_8006349C(){}

extern "C" u32 CfResEntry_getField8(u8* self) { return *(u32*)((u8*)self + 0x8); }

extern "C" void CfResEntry_setField4(u8* self, u32 val) { *(u32*)((u8*)self + 0x4) = val; }

extern "C" void CfResEntry_clearField8(u8* self) { *(u32*)((u8*)self + 8) = 0; }

void func_80063560(){}

extern "C" void CfRes_clearField4(u8* self) { *(u32*)((u8*)self + 4) = 0; }

// func_800638B4: decrement the refcount of the manager's table entry for a
// non-negative index (entry table lives at manager + 0x1ed8).
void func_800638B4(int idx) {
    int inst = CfRes_getInstance();
    if (inst != 0 && idx >= 0) {
        CfResEntry_decRefCount((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), idx));
    }
}

void func_80063900(){}

#pragma push
#pragma auto_inline off
int func_8006398C() { return CfRes_getInstance(); }
#pragma pop

extern "C" void CfRes_stub_63990() {}

void func_80063994(){}

int func_80063A34(void* self){
    extern int getFileSize__11CDeviceFileFPCc(void*, int);
    int sz = getFileSize__11CDeviceFileFPCc(self, 1);
    if (sz < 0) sz = -1;
    return sz;
}

int __declspec(noinline) func_80063A60(u8* res) { return 0; }

extern "C" void CfRes_stub_63ACC() {}

void func_80063AD0(){}

extern "C" void CfRes_initFields4(u8* self, int a, int b, int c, int d) {
    *(int*)((char*)self + 4) = a;
    *(int*)((char*)self + 0x28) = b;
    *(int*)((char*)self + 8) = c;
    *(int*)((char*)self + 0x18) = d;
}

extern "C" void CfRes_setBits1_2(u8* self) {
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
extern "C" void* CfRes_vcall34(u8* self) {
    CfResSub_63C04* sub = ((CfResObj_63C04*)self)->sub;
    if (!sub) return self;
    return sub->_v034(self);
}

extern "C" void CfRes_stub_63C2C() {}

extern "C" void CfRes_resetState2(u8* self) {
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
extern "C" void* CfRes_vcall38(u8* self) {
    CfResSub_63C50* sub = ((CfResObj_63C50*)self)->sub;
    if (!sub) return self;
    return sub->_v038(self);
}

extern "C" void CfRes_stub_63C78() {}

void func_80063C7C(){}

extern "C" int CfRes_streq(const char* s1, const char* s2) {
    return strcmp(s1, s2) == 0 ? 1 : 0;
}

extern "C" unsigned long CfRes_getE24Bit22() {
    return (lbl_eu_80663E24 >> 22) & 1;
}

extern "C" int CfRes_getE24Bit18() {
    extern u32 lbl_eu_80663E24;
    return (lbl_eu_80663E24 >> 18) & 1;
}

// C++ virtual thunk with struct access (14 dummies + RTTI = offset 64)
struct CfResObj_63DF0 { u8 _00[0x2C]; struct CfResSub_63DF0* sub; };
struct CfResSub_63DF0 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14(void* self); };
extern "C" void CfRes_vcall14(void* self) {
    ((CfResObj_63DF0*)self)->sub->m14(self);
}

struct CfResSub_63E08 { virtual void m02(void* self, void* arg); };
struct CfResObj_63E08 { u8 _00[0x2C]; CfResSub_63E08* sub; };
extern "C" void CfRes_vcall02(void* self, void* arg) {
    ((CfResObj_63E08*)self)->sub->m02(self, arg);
}

extern "C" u32 CfRes_extractBits27_5(void* self) { return ((u32)(uintptr_t)self >> 27) & 0x1F; }

void func_80063E30(){}

void func_80063F1C(){}

void func_80063FA8(){}

void __declspec(noinline) func_80064014(CfRes* self, CEventFile* evt, u32 field) {}

void func_800640F4(){}

void func_8006414C(){}

extern "C" u32 CfRes_getAddrLow10(void* self) { return (u32)(uintptr_t)self & 0x3FF; }

void func_800641CC(){}

extern "C" unsigned long CfRes_packShift27(unsigned long a, unsigned long b) {
    return (a & 0x7FFFFFF) | (b << 27);
}

// C++ virtual thunk with struct access (4 dummies + RTTI = offset 24)
struct CfResObj_64370 { u8 _00[0x2C]; struct CfResSub_64370* sub; };
struct CfResSub_64370 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(void* self); };
extern "C" void CfRes_vcall04(void* self) {
    ((CfResObj_64370*)self)->sub->m04(self);
}

extern "C" bool CfRes_stubFalse_64388() { return false; }

// C++ virtual thunk with struct access (5 dummies + RTTI = offset 28)
struct CfResObj_64390 { u8 _00[0x2C]; struct CfResSub_64390* sub; };
struct CfResSub_64390 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(void* self); };
extern "C" void CfRes_vcall05(void* self) {
    ((CfResObj_64390*)self)->sub->m05(self);
}

extern "C" bool CfRes_stubFalse_643A8() { return false; }

// C++ virtual thunk with struct access (8 dummies + RTTI = offset 40)
struct CfResObj_643B0 { u8 _00[0x2C]; struct CfResSub_643B0* sub; };
struct CfResSub_643B0 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(void* self); };
extern "C" void CfRes_vcall08(void* self) {
    ((CfResObj_643B0*)self)->sub->m08(self);
}

extern "C" bool CfRes_stubFalse_643C8() { return false; }

// C++ virtual thunk with struct access (7 dummies + RTTI = offset 36)
struct CfResObj_643D0 { u8 _00[0x2C]; struct CfResSub_643D0* sub; };
struct CfResSub_643D0 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(void* self); };
extern "C" void CfRes_vcall07(void* self) {
    ((CfResObj_643D0*)self)->sub->m07(self);
}

extern "C" bool CfRes_stubFalse_643E8() { return false; }

void func_800643F0(){}

extern "C" void CfRes_clearE28Mask(unsigned long mask) {
    extern unsigned long lbl_eu_80663E28;
    lbl_eu_80663E28 &= ~mask;
}

struct CfResData {
    u8 _pad00[0x2C];
    void* field_2C;
};

extern "C" void CfRes_delegateCleanup(void* self) {
    extern void func_80065CA4(void* a, void* b);
    func_80065CA4(static_cast<CfResData*>(self)->field_2C, self);
}

extern "C" unsigned long CfRes_isField4Zero(u8* self) {
    unsigned long v = *(unsigned long*)((char*)self + 4);
    return v == 0 ? 1 : 0;
}

void CfRes_64994::initStruct() {
    field_04 = 0;
    field_08 = 0;
    field_28 = 0;
    field_00 = 0;
    field_24 = 0;
    field_20 = 0;
}

extern "C" void CfRes_orBits_649B4(u8* self, u32 bits) {
    *(u32*)self |= bits;
}

extern u32 lbl_eu_80663E14;
extern "C" int CfRes_getE14() { return lbl_eu_80663E14; }

extern "C" void CfRes_orBits_649CC(u8* self, u32 bits) {
    *(u32*)self |= bits;
}

extern u32 lbl_eu_80663E30;
extern "C" int CfRes_getE30() { return lbl_eu_80663E30; }

extern "C" void CfRes_setE28Mask(u32 bits) {
    extern u32 lbl_eu_80663E28;
    lbl_eu_80663E28 |= bits;
}

// vtable+0xC dispatch on *(self+0x2C) (retail: lwz r3,0x2c; lwz r12,0; lwz r12,0xc; mtctr; bctr)
struct CfResVtC { virtual void m0(); virtual void m1(); };
extern "C" void func_800649F4(void* self) { ((CfResVtC*)(*(void**)((char*)self + 0x2C)))->m1(); }

extern "C" int CfRes_checkMask_64A08(u8* self, u32 mask) {
    u32 val = *(u32*)self;
    return (val & mask) != 0 ? 1 : 0;
}

// CfRes::OnFileEvent - async file-read completion for the resource manager:
// pass the event and its owning-handle field into the per-event handler.
extern "C" u32 CfRes_getField4_64A6C(u8* self);
void ::CfRes::OnFileEvent(CEventFile* ev) {
    func_80064014(this, ev, CfRes_getField4_64A6C((u8*)ev));
}

extern "C" __declspec(noinline) u32 CfRes_getField4_64A6C(u8* self) { return *(u32*)((u8*)self + 0x4); }

void func_80064A74(){}

extern "C" u32 CfRes_getAddrLow10_64B70(u8* self) { return (u32)(uintptr_t)self & 0x3FF; }

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
extern "C" void CfRes_vcall17(u8* self, void* arg) {
    ((CfResObj_64CB8*)self)->sub->m17(self, arg);
}

void func_80064CD8(){}

void __declspec(noinline) func_eu_80065590(int inst, int index, u8* ptr) {}

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

extern "C" u32 CfRes_getField18_64F58(u8* self) { return *(u32*)((u8*)self + 0x18); }

extern "C" void CfRes_setBits11_64F60(u8* self) {
    u32 val = *(u32*)self;
    *(u32*)self = (val & ~0x42) | 0x11;
}

void func_80064F78(){}

void func_80065050(){}

void func_80065158(){}

extern "C" __declspec(noinline) void func_80065254(int inst, u8* arg){}

extern "C" unsigned long CfRes_packThreeFields(unsigned long a, unsigned long b, unsigned long c) {
    return ((a & 0x1F) << 27) | ((b & 0xFFF) << 20) | ((c & 0x3FFFFF) << 10);
}

void func_80065314(){}

void func_eu_80065C7C(){}

extern "C" void* func_eu_80065D60(void* self) { return (char*)self + 0x1e4; }

void func_800653E4(){}

void func_800654B4(){}

void func_800655C4(){}

void func_80065694(){}

extern "C" int CfResEntry_incRefCount(u8* self) {
    return ++*(int*)((char*)self + 4);
}

int CfRes_65818::decRefCount() {
    if (field_04 > 0) {
        field_04--;
    }
    return field_04;
}

extern "C" int CfRes_incField8(u8* self) {
    return ++*(int*)((char*)self + 8);
}

cf::CfRes::~CfRes() {}

cf::CfResTask::~CfResTask() {}

void cf::CfResTask::Init() {}

void cf::CfResTask::Move() {}

void cf::CfResTask::Draw() {}

// Term: tear down the embedded resource storage and clear the global
// manager-singleton state.
void cf::CfResTask::Term() {
    func_80067D38(mEntries);
    func_80063158(mField54);
    lbl_eu_80663D80 = 0;
    lbl_eu_80663D78 = 0;
    lbl_eu_80663D7C = 0;
}


cf::CfRes::CfRes() {}


extern "C" char* CfRes_strcatAppend(char* buffer, const char* suffix) {
    unsigned int length = (unsigned int)strlen(suffix);
    strcat(buffer, suffix);
    *(unsigned int*)(buffer + 0x40) += length;
    return buffer;
}

extern "C" int CfRes_isField40NonZero(int dummy, void* self) {
    return *(int*)((char*)self + 40) != 0 ? 1 : 0;
}

extern "C" int CfRes_checkObjState(u8* self, void* obj) {
    int ret = 0;
    if (*(u32*)((u8*)obj + 4)) {
        if (*(u32*)((u8*)obj + 0x28) == 0) {
            ret = 1;
        }
    }
    return ret;
}

extern "C" int CfRes_cmpField4Eq(void* unused, const void* obj, u32 val) {
    u32 field = *(const u32*)((const u8*)obj + 4);
    if (field != 0 && field == val) return 1;
    return 0;
}

void func_80065CA4(){}

extern "C" int func_800A7EFC();
extern "C" int func_800A7FBC();
extern "C" int func_800A813C();
extern "C" int func_800A8CD4();
extern "C" int func_800A99D0();
extern "C" int func_800A9A90();
extern "C" int func_800A807C();
int func_80065D00() { return func_800A813C(); }
int func_80065D04() { return func_800A7EFC(); }
int func_80065D08() { return func_800A7EFC(); }

void func_80065D0C(){}

// C++ virtual call forces MWCC to use r12 for vtable dispatch
// 14 dummies + RTTI overhead = offset 64 at vtable+0x40
struct CfResVtabClass { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11(); virtual void m13(); virtual void m14(); virtual void m16(); };
extern "C" void CfRes_vcall16(u8* self) {
    ((CfResVtabClass*)self)->m16();
}

int func_80065D74() { return func_800A8CD4(); }

extern "C" void CfRes_stub_65D78() {}

extern "C" void CfRes_stub_65D7C() {}

// tail branches to lazily-initialised globals (retail: b func_800Axxxx)
int func_80065D80() { return func_800A813C(); }

int func_80065D84() { return func_800A7FBC(); }

int func_80065D88() { return func_800A99D0(); }

extern "C" void CfRes_stub_65D8C() {}

void func_80065D90(){}

extern "C" u32 CfRes_getField24(u32 unused, void* obj) { return *(u32*)((char*)obj + 24); }

extern "C" void* CfRes_dispatchTypeA(void* unused, const void* obj) {
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

extern "C" int CfRes_dispatchTypeB(u8* self, void* param) {
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

int func_80065F18() { return func_800A9A90(); }

int func_80065F1C() { return func_800A807C(); }

extern "C" void CfRes_stub_65F20() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CTTask<CfResTask> is declared in kyoshin/cf/CfRes.hpp; the out-of-line
// specializations below emit the retail Move/Draw/dtor symbols. Move/Draw
// dispatch the +0x3C/+0x48 ptmf hooks through __ptmf_test/__ptmf_scall.

// CTTask<cf::CfResTask>::Move - test PTMF at +0x3C, call if non-null
template<>
void CTTask<cf::CfResTask>::Move() {
    if (__ptmf_test(&mMoveFunc)) {
        (static_cast<cf::CfResTask*>(this)->*mMoveFunc)();
    }
}

// CTTask<cf::CfResTask>::Draw - test PTMF at +0x48, call if non-null
template<>
void CTTask<cf::CfResTask>::Draw() {
    if (__ptmf_test(&mDrawFunc)) {
        (static_cast<cf::CfResTask*>(this)->*mDrawFunc)();
    }
}

// CTTask<cf::CfResTask> destructor - base dtor body is empty (the deleting
// CfResTask dtor drives the CProcess teardown).
template<>
CTTask<cf::CfResTask>::~CTTask() {}

extern "C" void func_80062BA0() {}
extern "C" void func_80062CD0() {}

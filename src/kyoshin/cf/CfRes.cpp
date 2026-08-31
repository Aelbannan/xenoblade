// Auto-scaffolded catalog TU for kyoshin/cf/CfRes
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include <revolution/os/OSCache.h>  // DCFlushRange (C linkage)

#include "kyoshin/cf/CfRes.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "monolib/work/CEventFile.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
extern "C" char* getEntryPtrGrid(char* self, int a, int b);
extern "C" void* func_80495FF0(void* scene);
extern "C" mtl::ALLOC_HANDLE func_80496004(void* src);
extern "C" void* getHandleMEM1__Q23mtl10MemManagerFv();
// Two-word ring record built on the caller's frame by func_80061870; both
// members stay memory-resident in retail (header at 0x8(sp), data at 0xC(sp)).

// Append a header/data record to the ring buffer. Each stored word lands at
// ring offset (field_400 + field_404) words and advances field_404 by one.
// Returns 0 when the record would overflow the 0x100-word budget.
int func_80061870(CfResBuffer* buffer, unsigned char byte1, unsigned short halfword,
                  unsigned int dataVal, unsigned int* src, int count) {
    if (buffer->field_404 + count + 2 >= 0x100) {
        return 0;
    }

    // Stores index through a u32 view of the buffer; the potential aliasing
    // with the record pair below (and field_400/field_404) keeps everything
    // memory-resident in retail.
    u32* words = (u32*)buffer->buffer;

    // Header/data record (retail frame slots 0x8/0xC). The header's previous
    // contents are unspecified (retail reads the raw frame slot).
    u32 rec[2];
    rec[1] = dataVal;

    words[buffer->field_400 + buffer->field_404] =
        rec[0] = (((rec[0] & 0xFFFFF0FFu) | ((u32)byte1 << 24) | (u32)halfword) &
                  0xFFFFEFFFu) |
                 (((u32)count << 20) & 0xF00u); // key byte, halfword, count nibble
    buffer->field_404 = buffer->field_404 + 1;
    words[buffer->field_400 + buffer->field_404] = rec[1];
    buffer->field_404 = buffer->field_404 + 1;

    for (int j = 0; j < count; j++) {
        words[buffer->field_400 + buffer->field_404] = src[j];
        buffer->field_404 = buffer->field_404 + 1;
    }
    return 1;
}

// Backward-write variant of func_80061870: the record ends at the current
// field_400, so the write index walks backwards (field_400 - count - 2).
// Header layout: key byte at bits 24-31, payload halfword at bits 16-31,
// count nibble at bits 8-11; bits 12-19 come from the caller's headerBits
// and survive. Each ring store indexes through the u32* view, whose potential
// aliasing with field_400 forces retail to reload it for every word.
int CfResBuffer::func_80061A80(unsigned char byte1, unsigned short halfword,
                               unsigned int dataVal, unsigned int* src, int count,
                               unsigned int headerBits) {
    unsigned int total = field_404 + count + 2;
    if (total >= 0x100) {
        return 0;
    }

    u32* words = (u32*)buffer;
    u32 rec[2]; // header/data pair kept memory-resident (retail slots 0x8/0xC)

    field_400 = (field_400 - (count + 2)) & 0xFF;

    // Scratch header word is intentionally uninitialized (retail reads the
    // raw frame slot); overlay the key byte, payload halfword, count nibble.
    rec[0] &= 0xFFFFF0FFu;                     // clear count nibble (bits 8-11)
    rec[0] |= (u32)byte1 << 24;                // key byte
    rec[0] = (rec[0] & 0xFFFF0000u) | (u32)halfword; // payload halfword
    rec[0] &= 0xFFF00FFFu;                     // clear bits 12-19
    rec[0] |= (count << 20) & 0xF00u;          // count nibble -> bits 8-11
    rec[1] = dataVal;

    words[field_400] = rec[0];
    words[(field_400 + 1) & 0xFF] = rec[1];

    if (count != 0) {
        unsigned int i = 0;
        unsigned int off = 2;
        while (i < (unsigned int)count) {
            words[(field_400 + off) & 0xFF] = src[i];
            i++;
            off++;
        }
    }

    field_404 = field_404 + count + 2;
    return 1;
}

// func_80061C5C: pop a header word pair plus `count` data words off the
// CfResBuffer ring (mod-0x100 index); the popped header's bits 8-11 hold the
// data-word count. 1 when the ring is non-empty, 0 otherwise.
int func_80061C5C(CfResBuffer* buffer, u32* headerOut, u32* dataOut) {
    headerOut[1] = 0;
    headerOut[0] = headerOut[0] & 0x000F0000;
    if (buffer->field_404 == 0) {
        return 0;
    }
    u32 i = 0;
    u32* buf = (u32*)buffer->buffer;
    headerOut[0] = buf[buffer->field_400];
    buffer->field_400 = (buffer->field_400 + 1) & 0xFF;
    buffer->field_404 = buffer->field_404 - 1;
    headerOut[1] = buf[buffer->field_400];
    buffer->field_400 = (buffer->field_400 + 1) & 0xFF;
    buffer->field_404 = buffer->field_404 - 1;
    while (i < ((headerOut[0] >> 20) & 0xF)) {
        dataOut[i] = buf[buffer->field_400];
        buffer->field_400 = (buffer->field_400 + 1) & 0xFF;
        buffer->field_404 = buffer->field_404 - 1;
        i++;
    }
    return 1;
}

// Header/data word pair popped off the CfResBuffer ring by the inlined
// CfRes_popPair helper; keeping it a by-value struct return forces MWCC to
// materialize the pair on the stack (retail spills it to 8(sp)/0xC(sp) on
// every iteration).
struct CfResRingPair {
    u32 header;
    u32 data;
};

// Pops a header/data pair off the ring; by-value struct return keeps the pair
// memory-resident in the inlined callers (retail dead-stores it to the frame).
static CfResRingPair CfRes_popPair(CfResBuffer* buffer) {
    CfResRingPair pr;
    pr.header = ((u32*)buffer->buffer)[buffer->field_400];
    buffer->field_400 = (buffer->field_400 + 1) & 0xFF;
    buffer->field_404 = buffer->field_404 - 1;
    pr.data = ((u32*)buffer->buffer)[buffer->field_400];
    buffer->field_400 = (buffer->field_400 + 1) & 0xFF;
    buffer->field_404 = buffer->field_404 - 1;
    return pr;
}

// func_80061D2C: scan the CfResBuffer ring for records whose header key byte
// (bits 24-31) matches `mode`; each match marks that record's header word
// with bit 19 (0x00080000) in place and sets the result. Each visited record
// pops its header+data word pair, then skips its payload words (count nibble
// at header bits 16-19, extracted with the (h<<12)>>28 rotate so MWCC emits
// the extrwi-shaped rlwinm.). Ring indices are saved at entry and restored at
// exit, so the scan never consumes the ring.
bool func_80061D2C(CfResBuffer* buffer, u32 mode) {
    bool found = false;
    u32 saved400 = buffer->field_400;
    u32 saved404 = buffer->field_404;
    while (buffer->field_404 != 0) {
        u32 oldIdx = buffer->field_400;
        CfResRingPair pr = CfRes_popPair(buffer);
        if ((pr.header >> 24) == mode) {
            ((u32*)buffer->buffer)[oldIdx] |= 0x00080000;
            found = true;
        }
        // count nibble lives at header bits 16-19; the (h<<12)>>28 form makes
        // MWCC emit the same extrwi-shaped rlwinm as retail
        u32 count = (pr.header << 12) >> 28;
        for (u32 i = 0; i < count; i++) {
            buffer->field_400 = (buffer->field_400 + 1) & 0xFF;
            buffer->field_404 = buffer->field_404 - 1;
        }
    }
    buffer->field_400 = saved400;
    buffer->field_404 = saved404;
    return found;
}

// func_80061E8C: scan the CfResBuffer ring for a record whose header key
// byte (bits 24-31) matches `mode`; return 1 immediately on the first match
// (the remaining data words of the matched record are not popped). Each
// visited record consumes its header+data word pair (both advance the read
// index), then skips its payload words (count nibble at bits 16-19 of the
// header). Ring indices are saved at entry and restored at exit, so the scan
// never consumes the ring.
bool func_80061E8C(CfResBuffer* buffer, u32 mode) {
    bool found = false;
    u32 saved400 = buffer->field_400;
    u32 saved404 = buffer->field_404;
    while (buffer->field_404 != 0) {
        CfResRingPair pr = CfRes_popPair(buffer);
        if ((pr.header >> 24) == mode) {
            found = true;
            break;
        }
        // count nibble lives at header bits 16-19; the (h<<12)>>28 form
        // makes MWCC emit the same extrwi-shaped rlwinm (rotl 12, mask 28-31)
        // as retail
        u32 count = (pr.header << 12) >> 28;
        for (u32 i = 0; i < count; i++) {
            buffer->field_400 = (buffer->field_400 + 1) & 0xFF;
            buffer->field_404 = buffer->field_404 - 1;
        }
    }
    buffer->field_400 = saved400;
    buffer->field_404 = saved404;
    return found;
}

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
extern "C" __declspec(noinline) int CfRes_getInstanceField() {
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
extern "C" void CfRes_callFunc_67F10(s32 value) {
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    if (ret) {
        func_80067F10(ret, 1);
    }
}

extern u32 lbl_eu_80663D7C;
extern "C" void CfRes_callFunc_68078() {
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    if (ret) {
        func_80068078(ret);
    }
}

extern "C" {
extern u32 lbl_eu_80663D7C;
extern int func_80068110(int, void*);
}
extern "C" void CfRes_callFunc_68110(u32 self) {
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    if (ret) {
        func_80068110(ret, (void*)self);
    }
}

extern u32 lbl_eu_80663D7C;
extern "C" void CfRes_callFunc_68254() {
    u32 val = lbl_eu_80663D7C;
    int ret = 0;
    if (val) {
        ret = val + 4;
    }
    if (ret) {
        func_80068254(ret);
    }
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

// func_80062114: when the key string at `self` matches the global name key
// (lbl_eu_80661A20), resolve the indexed table record: store the packed id
// of its +0x18 string to *out and return the record's relative-offset target
// (0 otherwise).
void* func_80062114(char* self, int index, void** out) {
    void* result = 0;
    if (self != 0) {
        int same = strcmp(lbl_eu_80661A20, self) == 0;
        if (same) {
            CfResStrTableRec* rec = (CfResStrTableRec*)(self + index * 16);
            *out = (void*)(uintptr_t)func_800AA600(rec->str);
            result = self + rec->field_10;
        }
    }
    return result;
}

// func_800621A0: return the +4 count of a string-keyed record when its
// inline key matches the global name key (lbl_eu_80661A24), else 0.
int func_800621A0(CfResNameRec* rec) {
    if (rec != 0) {
        int same = strcmp(lbl_eu_80661A24, rec->mKey) == 0;
        if (same) {
            return rec->mCount;
        }
    }
    return 0;
}

// func_800621F4: resolve the extension class of a resource record path.
// The record lives at self + index*0x18 + 8; its +8 word is a relative path
// offset from self, +0xC and +0x10 are output words. The resolved path is
// matched against the shared extension table (lbl_eu_804FB214) and the
// matched class id is stored to *outType. Returns the resolved path pointer
// (null when self is missing or carries the manager key). Retail re-checks
// the key condition redundantly before selecting the record.
char* func_800621F4(char* self, int index, u32* outType, u32* outFieldC, u32* outField10) {
    u32* rec;
    char* result = 0;
    *outType = 0;
    *outFieldC = 0;
    *outField10 = 0;
    // Retail proceeds only when the key string equals self; the equality
    // result is materialized as an int before branching (cntlzw codegen).
    if (self != 0) {
        int same = strcmp(lbl_eu_80661A24, self) == 0;
        if (same) {
            rec = 0;
            if (self != 0) {
                int same2 = strcmp(lbl_eu_80661A24, self) == 0;
                if (same2) {
                    rec = (u32*)((int)self + index * 0x18 + 8);
                }
            }
            result = self + (int)rec[2];
            *outFieldC = rec[3];
            *outField10 = rec[4];
            // Extension table walk: offsets 0x00/0x04/0x08/0x0B/0x0B(dup)/0x0F/
            // 0x12/0x16 map to class ids 1/2/3/4/4/6/5/7.
            if (strncmp(result, lbl_eu_804FB214, 3) == 0) {
                *outType = 1;
            } else if (strncmp(result, lbl_eu_804FB214 + 4, 3) == 0) {
                *outType = 2;
            } else if (strncmp(result, lbl_eu_804FB214 + 8, 2) == 0) {
                *outType = 3;
            } else if (strncmp(result, lbl_eu_804FB214 + 0xB, 3) == 0) {
                *outType = 4;
            } else if (strncmp(result, lbl_eu_804FB214 + 0xB, 3) == 0) {
                *outType = 4;
            } else if (strncmp(result, lbl_eu_804FB214 + 0xF, 2) == 0) {
                *outType = 6;
            } else if (strncmp(result, lbl_eu_804FB214 + 0x12, 3) == 0) {
                *outType = 5;
            } else if (strncmp(result, lbl_eu_804FB214 + 0x16, 3) == 0) {
                *outType = 7;
            }
        }
    }
    return result;
}

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

// func_80062430: zero the output word, then resolve a resource through the
// manager's func_80065694 helper when the CfRes manager exists (0 otherwise).
int func_80062430(int a, int b, int* out) {
    int ret = 0;
    *out = 0;
    if (CfRes_getInstance() != 0) {
        CfRes_getInstance();
        ret = func_80065694(a, b, out);
    }
    return ret;
}

// func_800624A8: forward (a, b, c) into the manager's resource resolver
// (func_80064EB0) when the CfRes manager exists; 0 otherwise.
int func_800624A8(int a, int b, int c) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = func_80064EB0(CfRes_getInstance(), a, b, c);
    }
    return ret;
}

u32 __declspec(noinline) func_8006251C(void* self){ return ((u32)(uintptr_t)self >> 20) & 0x7F; }

u32 __declspec(noinline) func_80062524(void* self){ return ((u32)(uintptr_t)self >> 10) & 0x3FF; }

// func_8006252C: forward (a, b, c) into the manager's resource resolver
// (func_80064F78) when the CfRes manager exists; 0 otherwise.
int __declspec(noinline) func_8006252C(u16 a, u16 b, int c) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = func_80064F78(CfRes_getInstance(), a, b, c);
    }
    return ret;
}

// func_800625A0: pack the two index fields of `self` as u16 halves and route
// them (with arg2) into the resource-table walker func_8006252C. The raw
// results are held in locals so the u16 truncation happens at the call site.
void func_800625A0(void* self, int arg2) {
    u32 a = func_8006251C(self);
    u32 b = func_80062524(self);
    func_8006252C((u16)a, (u16)b, arg2);
}

// func_80062600: run the update pipeline only while neither busy flag is set
extern "C" int CfRes_checkFlags_48000();
extern "C" int CfRes_checkFlags_2000400();
void func_80062600() {
    if (CfRes_checkFlags_48000() == 0 && CfRes_checkFlags_2000400() == 0) goto run;
    return;
run:
    func_800A9068();
    CfRes_callFunc_67E78();
    func_800620F0();
    func_800A9134();
}

extern "C" __declspec(noinline) int CfRes_checkFlags_48000() {
    return (lbl_eu_80663E24 & 0x00048000) != 0 ? 1 : 0;
}

extern "C" __declspec(noinline) int CfRes_checkFlags_2000400() {
    return (lbl_eu_80663E24 & 0x02000400) != 0 ? 1 : 0;
}

// func_80062680: forward (a, b, c) into the manager's resource resolver
// (func_80065050) when the CfRes manager exists; 0 otherwise.
int func_80062680(int a, int b, int c) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = func_80065050(CfRes_getInstance(), a, b, c);
    }
    return ret;
}

// func_800626F4: forward (a, b, c, d) into the manager's resource resolver
// (func_80065158) when the CfRes manager exists; 0 otherwise.
int func_800626F4(int a, int b, int c, int d) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = func_80065158(CfRes_getInstance(), a, b, c, d);
    }
    return ret;
}

// func_eu_80062E58: forward (a, b, c) into the manager's resource resolver
// (func_eu_80065C7C) when the CfRes manager exists; 0 otherwise.
int func_eu_80062E58(int a, int b, int c) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = func_eu_80065C7C(CfRes_getInstance(), a, b, c);
    }
    return ret;
}

// func_80062758: forward (a, b) into the manager's resource resolver
// (func_80065314) when the CfRes manager exists; 0 otherwise.
int func_80062758(int a, int b) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = func_80065314(CfRes_getInstance(), a, b);
    }
    return ret;
}

// func_800627BC: register the resource with the CfRes manager if it exists
extern "C" void func_80065254(int inst, u8* arg);
void func_800627BC(u8* arg) {
    if (CfRes_getInstance() != 0) {
        func_80065254(CfRes_getInstance(), arg);
    }
}

// func_800627FC: forward (a, b) into the manager's resource resolver
// (func_800653E4) when the CfRes manager exists; 0 otherwise.
int func_800627FC(int a, int b) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = func_800653E4(CfRes_getInstance(), a, b);
    }
    return ret;
}

// func_80062860: forward (a, b) into the manager's resource resolver
// (func_800654B4) when the CfRes manager exists; 0 otherwise.
int func_80062860(int a, int b) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = func_800654B4(CfRes_getInstance(), a, b);
    }
    return ret;
}

// func_800628C4: forward (a, b) into the manager's resource resolver
// (func_800655C4) when the CfRes manager exists; 0 otherwise.
int func_800628C4(int a, int b) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = func_800655C4(CfRes_getInstance(), a, b);
    }
    return ret;
}

// func_80062928: forward (a, b, c) into the manager's resource resolver
// (func_80064A74) when the CfRes manager exists; `b` otherwise.
int func_80062928(int a, int b, int c) {
    int ret = b;
    if (CfRes_getInstance() != 0) {
        ret = func_80064A74(CfRes_getInstance(), a, b, c);
    }
    return ret;
}

extern "C" u32 CfRes_getField18(u8* self) { return *(u32*)((u8*)self + 0x18); }

// func_80062998: forward (a, b, c) into the manager's resource resolver
// (func_80064CD8) when the CfRes manager exists; `b` otherwise.
int func_80062998(int a, int b, int c) {
    int ret = b;
    if (CfRes_getInstance() != 0) {
        ret = func_80064CD8(CfRes_getInstance(), a, b, c);
    }
    return ret;
}

// func_eu_80063174: forward a (index, ptr) pair into the manager's resolver
// when the CfRes manager exists.
void func_eu_80063174(int index, u8* ptr) {
    if (CfRes_getInstance() != 0) {
        func_eu_80065590(CfRes_getInstance(), index, ptr);
    }
}

// func_80062A00: scan the 7x6 resource grid (rows x columns) for an in-flight
// load request (func_80062C28's +0x28 flag); 0 while any request is pending,
// 1 when the grid is idle or the manager is unavailable.
int func_80062A00() {
    if (CfRes_getInstance() != 0) {
        for (int row = 0; row < 7; row++) {
            for (int col = 1; col <= 6; col++) {
                CfResPcEntry28View* entry = func_80062C28(row, col);
                if (entry != 0 && entry->field_28 != 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// Resolve the local resource index: when the global instance is present,
// call the +0x140 table lookup with the instance and self.
extern "C" int func_800640F4(int, void*);
extern "C" int func_80062A84(void* self) {
    int r = -1;
    if (CfRes_getInstance()) {
        r = func_800640F4(CfRes_getInstance(), self);
    }
    return r;
}

// func_80062AD8: forward (a, b) into the manager's resource resolver
// (func_800641CC) when the CfRes manager exists; 0 otherwise.
__declspec(noinline) int func_80062AD8(int a, int b) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = func_800641CC(CfRes_getInstance(), a, b);
    }
    return ret;
}

// func_80062B3C: forward (a, b) into the manager's delegate-op handler
// (CfRes_delegateOp1) when the CfRes manager exists; 0 otherwise. The
// delegate is declared to return a value (retail callers capture r3) even
// though its body is a void call to func_800643F0 followed by blr.
int func_80062B3C(int a, int b) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = CfRes_delegateOp1((void*)(uintptr_t)CfRes_getInstance(),
                                (void*)(uintptr_t)a,
                                (void*)(uintptr_t)b);
    }
    return ret;
}

// Retail callers capture r3 after this call, so the delegate is declared with
// an int return even though the body is a void call to func_800643F0 followed
// by blr (the value left in r3 is func_800643F0's result). Omitting the
// return statement keeps the compiled bytes identical to the void form.
// noinline keeps callers (func_80062B3C) from inlining this body.
extern "C" int __declspec(noinline) CfRes_delegateOp1(void* a, void* b, void* c) {
    extern void func_800643F0(void*, void*, unsigned long, void*);
    func_800643F0(a, b, 1, c);
}

// func_80062BAC: forward (a, b) into the manager's delegate-op handler
// (CfRes_delegateOp0) when the CfRes manager exists; 0 otherwise.
int func_80062BAC(int a, int b) {
    int ret = 0;
    if (CfRes_getInstance() != 0) {
        ret = CfRes_delegateOp0((void*)(uintptr_t)CfRes_getInstance(),
                                (void*)(uintptr_t)a,
                                (void*)(uintptr_t)b);
    }
    return ret;
}

// Retail callers capture r3 after this call, so the delegate is declared with
// an int return even though the body is a void call to func_800643F0 followed
// by blr (the value left in r3 is func_800643F0's result).
extern "C" int __declspec(noinline) CfRes_delegateOp0(void* a, void* b, void* c) {
    extern void func_800643F0(void*, void*, unsigned long, void*);
    func_800643F0(a, b, 0, c);
}

extern "C" void CfRes_readCommonArchive(unsigned long a, const char* b, void* c) {
    extern void readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, char const*, void*, int, int);
    readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(a, b, c, 0, 0);
}

// Retail symbol is a resource-table accessor (see CfResPcImpl.hpp); the
// catalog stub keeps the call relocs resolvable until its own target is
// worked. noinline keeps callers (func_80062A00) from inlining the body.
extern "C" CfResPcEntry28View* func_80062C80(char*, int, int);
extern "C" __declspec(noinline) CfResPcEntry28View* func_80062C28(int id, int a) {
    if (!CfRes_getInstance())
        goto ret0;
    return func_80062C80((char*)CfRes_getInstance(), id, a);
ret0:
    return 0;
}

// retail: addi r3,r3,4; b getEntryPtrGrid (3-arg tail call)
extern "C" __declspec(noinline) CfResPcEntry28View* func_80062C80(char* self, int a, int b) {
    return (CfResPcEntry28View*)getEntryPtrGrid(self + 4, a, b);
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

extern "C" __declspec(noinline) void* func_80062F58(void* self) { return (char*)self + 0x7c; }

#pragma push
#pragma auto_inline off
extern "C" void* func_80062F98(void* self) { return (char*)self + 0xbc; }
#pragma pop

// func_80062F60: return the manager's +0xbc region if the CfRes manager exists
__declspec(noinline) char* func_80062F60() {
    if (CfRes_getInstance() != 0) {
        return (char*)func_80062F98((char*)CfRes_getInstance());
    }
    return 0;
}

extern "C" __declspec(noinline) void* func_80062FA0(void* self) { return (char*)self + 0xb8; }

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

extern "C" __declspec(noinline) void* func_80062FE8(void* self) { return (char*)self + 0xf4; }

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

extern "C" __declspec(noinline) void* func_80063030(void* self) { return (char*)self + 0x130; }

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

extern "C" __declspec(noinline) void* func_80063078(void* self) { return (char*)self + 0x16c; }

// func_80063080: return the manager's +0x29c region if the CfRes manager exists
extern "C" void* func_800630B8(void* self);
char* func_80063080() {
    if (CfRes_getInstance() != 0) {
        return (char*)func_800630B8((char*)CfRes_getInstance());
    }
    return 0;
}

extern "C" __declspec(noinline) void* func_800630B8(void* self) { return (char*)self + 0x29c; }

extern "C" __declspec(noinline) void* func_800630C0(void* self) { return (char*)self + 0x298; }

// func_800630C8: return the manager's +0x224 region if the CfRes manager exists
extern "C" void* func_80063100(void* self);
char* func_800630C8() {
    if (CfRes_getInstance() != 0) {
        return (char*)func_80063100((char*)CfRes_getInstance());
    }
    return 0;
}

extern "C" __declspec(noinline) void* func_80063100(void* self) { return (char*)self + 0x224; }

extern "C" __declspec(noinline) void* func_80063108(void* self) { return (char*)self + 0x220; }

extern "C" __declspec(noinline) void* func_80063110(void* self) { return (char*)self + 0x25c; }

extern "C" __declspec(noinline) void* func_80063118(void* self) { return (char*)self + 0x1a8; }

// func_80063120: stash the archive id, init the embedded subobject, and
// register it into both lookup tables with a -1 id
extern "C" void func_80063160(u32 arg);
extern "C" void func_800631FC(u32 arg);
// noinline: the retail ctor calls func_80063120 out-of-line (bl); without it
// MWCC inlines this body into __ct__Q22cf5CfResFv and the call relocs shift.
__declspec(noinline) void func_80063120(u8* self, int arg) {
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

// func_80063160: init the single manager entry table slot when the index is
// in range (<= 6), otherwise init all 7 slots.
extern "C" __declspec(noinline) void func_80063160(u32 arg) {
    int inst = CfRes_getInstance();
    if (inst != 0) {
        if (arg <= 6) {
            CfResEntry_init((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), arg));
        } else {
            for (u32 i = 0; i < 7; i++) {
                CfResEntry_init((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), i));
            }
        }
    }
}

extern "C" __declspec(noinline) void CfResEntry_init(u8* self) {
    *(int*)((char*)self + 0) = 0;
    *(int*)((char*)self + 4) = 0;
    *(int*)((char*)self + 8) = 0;
}

extern "C" __declspec(noinline) void* CfRes_getResEntry(u8* self, u32 idx) { return (u8*)self + idx * 12; }

// func_800631FC: init the single manager table entry slot when the index is
// in range (<= 2), otherwise init all 3 slots.
extern "C" __declspec(noinline) void func_800631FC(u32 arg) {
    int inst = CfRes_getInstance();
    if (inst != 0) {
        if (arg <= 2) {
            CfResEntry_init((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), arg));
        } else {
            for (u32 i = 0; i < 3; i++) {
                CfResEntry_init((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), i));
            }
        }
    }
}

extern "C" __declspec(noinline) void* CfRes_getTblEntry(u8* self, u32 idx) { return (u8*)self + idx * 12; }

extern "C" __declspec(noinline) u32 CfResEntry_getHandle(u8* self) { return *(u32*)((u8*)self + 0x0); }

// func_8006328C: return the entry-table index (0-6) whose handle matches,
// or -1 when the manager is missing, the handle is null, or nothing matched.
extern "C" __declspec(noinline) int func_8006328C(int handle) {
    int inst = CfRes_getInstance();
    if (inst != 0 && handle != 0) {
        for (u32 i = 0; i < 7; i++) {
            if ((u32)handle == CfResEntry_getHandle((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), i))) {
                return (int)i;
            }
        }
    }
    return -1;
}

// func_80063310: 1 when a table entry (index 0-2) whose handle matches
// exists, 0 otherwise.
int func_80063310(int handle) {
    int inst = CfRes_getInstance();
    if (inst != 0) {
        for (u32 i = 0; i < 3; i++) {
            if (handle != 0 && (u32)handle == CfResEntry_getHandle((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), i))) {
                return 1;
            }
        }
    }
    return 0;
}

// func_80063394: register `handle` in the manager's 3-slot table: return the
// index of the slot that already carries it (refcount bumped), else claim the
// first free slot (field_04 == 0) and bump its refcount; -1 when the manager
// is missing or no slot is free.
extern "C" int __declspec(noinline) func_80063394(int handle) {
    int result = -1;
    if (handle == 0) {
        return handle;
    }
    int inst = CfRes_getInstance();
    if (inst != 0) {
        for (u32 i = 0; i < 3; i++) {
            if ((u32)handle == CfResEntry_getHandle((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), i))) {
                CfResEntry_incRefCount((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), i));
                return (int)i;
            }
        }
        for (u32 i = 0; i < 3; i++) {
            if ((int)CfResEntry_getField4((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), i)) == 0) {
                CfResEntry_setHandle((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), i), (u32)handle);
                CfResEntry_incRefCount((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), i));
                result = (int)i;
                break;
            }
        }
    }
    return result;
}

extern "C" __declspec(noinline) u32 CfResEntry_getField4(u8* self) { return *(u32*)((u8*)self + 0x4); }

extern "C" __declspec(noinline) void CfResEntry_setHandle(u8* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }

extern "C" __declspec(noinline) u32 CfResEntry_getField8(u8* self) { return *(u32*)((u8*)self + 0x8); }

extern "C" __declspec(noinline) void CfResEntry_setField4(u8* self, u32 val) { *(u32*)((u8*)self + 0x4) = val; }

extern "C" __declspec(noinline) void CfResEntry_clearField8(u8* self) { *(u32*)((u8*)self + 8) = 0; }

// func_8006349C (0x80063C64): for each of the 7 resource-grid entries, move
// the leftover of (field_04 - field_08) into field_04 and clear field_08.
void func_8006349C() {
    int inst = CfRes_getInstance();
    if (inst != 0) {
        for (u32 i = 0; i < 7; i++) {
            u32 f4 = CfResEntry_getField4((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), i));
            u32 f8 = CfResEntry_getField8((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), i));
            int diff = (int)f4 - (int)f8;
            if (diff < 0) {
                diff = 0;
            }
            CfResEntry_setField4((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), i), (u32)diff);
            CfResEntry_clearField8((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), i));
        }
    }
}

// func_80063560: resolve a resource id to one of the 7 resource-table slots.
// If the handle lookup fails, special-case ids 3/8 re-check under the paired
// id (3<->8) after a game-mode query; otherwise fall back to preferred slots
// per id, then to a linear free-slot scan (field_04 == 0). Bumps refcounts
// when requested.
extern "C" int func_80063560(int id, int incRef, int incCount) {
    u32 uid = (u32)id;
    int idx = -1;

    if (id == 0) {
        return -1;
    }
    int inst = CfRes_getInstance();
    if (inst != 0) {
        idx = func_8006328C(id);
        if (idx < 0) {
            switch (uid) {
            case 3:
                if (initParticleSystem__Q22cf13CfGameManagerFv(8)) {
                    return -1;
                }
                idx = func_8006328C(8);
                if (idx >= 0) {
                    CfRes_clearField4((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), (u32)idx));
                    CfResEntry_clearField8((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), (u32)idx));
                    CfResEntry_setHandle((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), (u32)idx), uid);
                    func_8006398C(0);
                }
                break;
            case 8:
                if (initParticleSystem__Q22cf13CfGameManagerFv(3)) {
                    return -1;
                }
                idx = func_8006328C(3);
                if (idx >= 0) {
                    CfRes_clearField4((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), (u32)idx));
                    CfResEntry_clearField8((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), (u32)idx));
                    CfResEntry_setHandle((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), (u32)idx), uid);
                    func_8006398C(0);
                }
                break;
            default:
                break;
            }
        }

        if (idx >= 0) {
            if (incRef != 0) {
                CfResEntry_incRefCount((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), (u32)idx));
            }
            if (incCount != 0) {
                CfRes_incField8((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), (u32)idx));
            }
            return idx;
        }

        // Preferred-slot fallback: ids 1-8 prefer slot id-1 (slot 0 -> 2);
        // ids 9-13 have per-id preferred slots; then a linear free-slot scan.
        if (uid <= 8) {
            u32 slot = uid - 1;
            if (slot == 0) {
                slot = 2;
            }
            if (CfResEntry_getField4((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot)) == 0) {
                CfResEntry_setHandle((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot), uid);
                if (incRef != 0) {
                    CfResEntry_incRefCount((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot));
                }
                if (incCount != 0) {
                    CfRes_incField8((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot));
                }
                return (int)slot;
            }
        } else {
            u32 slot = 6;
            switch (uid) {
            case 9:
                slot = 5;
                break;
            case 10:
                slot = 6;
                break;
            case 11:
                slot = 5;
                break;
            case 12:
                slot = 3;
                break;
            case 13:
                slot = 3;
                break;
            default:
                break;
            }
            if (CfResEntry_getField4((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot)) == 0) {
                CfResEntry_setHandle((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot), uid);
                if (incRef != 0) {
                    CfResEntry_incRefCount((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot));
                }
                if (incCount != 0) {
                    CfRes_incField8((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot));
                }
                return (int)slot;
            }
        }
        for (u32 slot = 0; slot < 7; slot++) {
            if (CfResEntry_getField4((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot)) == 0) {
                CfResEntry_setHandle((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot), uid);
                if (incRef != 0) {
                    CfResEntry_incRefCount((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot));
                }
                if (incCount != 0) {
                    CfRes_incField8((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), slot));
                }
                return (int)slot;
            }
        }
    }
    return idx;
}

extern "C" __declspec(noinline) void CfRes_clearField4(u8* self) { *(u32*)((u8*)self + 4) = 0; }

// func_800638B4: decrement the refcount of the manager's table entry for a
// non-negative index (entry table lives at manager + 0x1ed8).
void func_800638B4(int idx) {
    int inst = CfRes_getInstance();
    if (inst != 0 && idx >= 0) {
        CfResEntry_decRefCount((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), idx));
    }
}

// func_80063900: find the entry-table record whose handle matches and
// decrement its reference count (no-op when the manager is missing).
void func_80063900(int handle) {
    if (handle > 0) {
        int inst = CfRes_getInstance();
        if (inst != 0) {
            for (u32 i = 0; i < 7; i++) {
                if ((u32)handle == CfResEntry_getHandle((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), i))) {
                    // Re-resolve the entry (retail re-calls getResEntry here).
                    CfResEntry_decRefCount((u8*)CfRes_getResEntry((u8*)(inst + 0x1efc), i));
                    break;
                }
            }
        }
    }
}

#pragma push
#pragma auto_inline off
// func_8006398C: retail signature takes a value (called with 0 by the
// acquire path); body owned by its own matching target.
extern "C" __declspec(noinline) void func_8006398C(u32 value) { (void)value; }
#pragma pop

extern "C" void CfRes_stub_63990() {}

int __declspec(noinline) func_80063A34(void* self){
    extern int getFileSize__11CDeviceFileFPCc(void*, int);
    int sz = getFileSize__11CDeviceFileFPCc(self, 1);
    if (sz < 0) sz = -1;
    return sz;
}

// The FixStr<64> default ctor is defined out-of-line (weak) in the
// CfGameManager TU; declare the specialization here so MWCC emits a call
// instead of inlining the trivial clear() body (retail calls it out-of-line).
namespace ml {
template <> FixStr<64>::FixStr();
}

// func_80063A60: format the resource id into a path string and return the
// file size (-1 when the path cannot be built or the file is missing).
int __declspec(noinline) func_80063A60(u8* res) {
    int ret = -1;
    ml::FixStr<64> str;
    if (func_800AA33C(str, (u32)(uintptr_t)res, 1, 1) != 0) {
        ret = func_80063A34(CfRes_stub_63ACC(&str));
    }
    return ret;
}

extern "C" __declspec(noinline) ml::FixStr<64>* CfRes_stub_63ACC(ml::FixStr<64>* str) { return str; }

// func_80063AD0 (0x8006429C): pre-open entry reservation/validation step in
// the archive-read pipeline. Scans the work-cache for the entry whose name
// matches the file portion of `d`; on a hit the cached data is copied into
// `b` (then flushed to memory), the entry `e` is initialized and 1 is
// returned. 0 when the cache is empty or no entry matches.
extern "C" int __declspec(noinline) func_80063AD0(void* a, void* b, u32 c, void* d, int size, void* e) {
    int count = func_804D8FB4();
    if (count > 0) {
        const char* filePtr = ml::CPathUtil::getFilePtrFromPath((const char*)d);
        ml::FixStr<64> str;
        func_80063C7C(str, filePtr);
        for (int i = 0; i < count; i++) {
            u32 field40;
            unsigned long dataPtr;
            char* namePtr;
            if (func_804D8FDC(i, &dataPtr, &namePtr, &field40) != 0 &&
                CfRes_streq(str.mString, namePtr) != 0) {
                CfRes_initFields4((u8*)e, (int)c, 0, (int)(uintptr_t)b, size);
                CfRes_setBits1_2((u8*)e);
                CfRes_vcall34((u8*)e);
                memcpy(b, (void*)dataPtr, (size_t)size);
                DCFlushRange(b, (u32)size);
                CfRes_resetState2((u8*)e);
                CfRes_vcall38((u8*)e);
                return 1;
            }
        }
    }
    return 0;
}

extern "C" __declspec(noinline) void CfRes_initFields4(u8* self, int a, int b, int c, int d) {
    *(int*)((char*)self + 4) = a;
    *(int*)((char*)self + 0x28) = b;
    *(int*)((char*)self + 8) = c;
    *(int*)((char*)self + 0x18) = d;
}

extern "C" __declspec(noinline) void CfRes_setBits1_2(u8* self) {
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
extern "C" __declspec(noinline) void* CfRes_vcall34(u8* self) {
    CfResSub_63C04* sub = ((CfResObj_63C04*)self)->sub;
    if (!sub) return self;
    return sub->_v034(self);
}

extern "C" void CfRes_stub_63C2C() {}

extern "C" __declspec(noinline) void CfRes_resetState2(u8* self) {
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
extern "C" __declspec(noinline) void* CfRes_vcall38(u8* self) {
    CfResSub_63C50* sub = ((CfResObj_63C50*)self)->sub;
    if (!sub) return self;
    return sub->_v038(self);
}

extern "C" void CfRes_stub_63C78() {}

// func_80063C7C: copy src into dest, then strip the file extension: scan
// backward from the end of the string for the last '.' (the lbl_eu_80661A40
// key) and cut everything from it onward so only the bare name remains.
// Returns 1 when an extension was removed, 0 when the string is empty, has
// no '.', or the '.' sits at index 0.
extern "C" int __declspec(noinline) func_80063C7C(ml::FixStr<64>& dest, const char* src) {
    dest.mLength = strlen(src);
    strcpy(dest.mString, src);

    int extPos;
    if (dest.mLength == 0) {
        extPos = -1;
    } else {
        u32 sepLen = strlen(&lbl_eu_80661A40);
        char* searchEnd = dest.mString - 1;
        char* searchPos = (dest.mString - 1) + dest.mLength;
        while (searchPos > searchEnd) {
            if (strncmp(searchPos, &lbl_eu_80661A40, sepLen) == 0) {
                extPos = (int)(searchPos - dest.mString);
                goto found_ext;
            }
            searchPos--;
        }
        extPos = -1;
    }

found_ext:
    // Named u32 keeps MWCC's literal addi/cmplwi guard pair (retail shape).
    u32 posCheck = extPos + 1;
    if (posCheck > 1u) {
        // Strip the extension: rebuild dest from its first extPos characters
        // (inlined FixStr range copy-ctor + assignment).
        ml::FixStr<64> stripped(dest, 0, extPos);
        dest = stripped;
        return 1;
    }
    return 0;
}

extern "C" __declspec(noinline) int CfRes_streq(const char* s1, const char* s2) {
    return strcmp(s1, s2) == 0 ? 1 : 0;
}

extern "C" unsigned long CfRes_getE24Bit22() {
    return (lbl_eu_80663E24 >> 22) & 1;
}

extern "C" __declspec(noinline) int CfRes_getE24Bit18() {
    return (lbl_eu_80663E24 >> 18) & 1;
}

// C++ virtual thunk with struct access (14 dummies + RTTI = offset 64)
struct CfResObj_63DF0 { u8 _00[0x2C]; struct CfResSub_63DF0* sub; };
struct CfResSub_63DF0 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual int m14(void* self); };
extern "C" __declspec(noinline) int CfRes_vcall14(void* self) {
    return ((CfResObj_63DF0*)self)->sub->m14(self);
}

struct CfResSub_63E08 { virtual void* m02(void* self, void* arg); };
struct CfResObj_63E08 { u8 _00[0x2C]; CfResSub_63E08* sub; };
__declspec(noinline) void* CfRes_vcall02(void* self, void* arg) {
    return ((CfResObj_63E08*)self)->sub->m02(self, arg);
}

extern "C" __declspec(noinline) u32 CfRes_extractBits27_5(void* self) { return ((u32)(uintptr_t)self >> 27) & 0x1F; }

// func_80063E30 (0x800645FC): async archive-read pipeline. Resolves the file
// size for the path, reserves the entry slot (func_80063AD0), then opens the
// common archive and initializes the entry's fields. Returns 1 once the read
// is dispatched, 0 on failure.
int __declspec(noinline) func_80063E30(void* a, void* b, u32 c, void* d, void* e, int f) {
    int result = 0;
    int size = func_80063A34(d);
    if (size < 0) {
        return 0;
    }
    if (func_80063AD0(a, b, c, d, size, e) != 0) {
        return 1;
    }
    CFileHandle* handle = CDeviceFile::readCommonArchiveFile(
        (mtl::ALLOC_HANDLE)(uintptr_t)b, (const char*)d, (IWorkEvent*)a, 0, 0);
    if (handle != 0) {
        if (f != 4) {
            CDeviceFile::func_8044F154(handle, f);
        }
        CfRes_initFields4((u8*)e, (int)c, (int)(uintptr_t)handle, (int)(uintptr_t)b, size);
        CfRes_setBits1_2((u8*)e);
        CfRes_vcall34((u8*)e);
        result = 1;
    }
    return result;
}

// func_80063F1C (0x800646E8): format the packed resource id into a path and
// run the archive-read pipeline (func_80063E30); 0 when the path cannot be
// built or the pipeline reports failure.
int __declspec(noinline) func_80063F1C(u8* a, u8* b, u32 c, u8* d, int e) {
    int ret = 0;
    ml::FixStr<64> str;
    if (func_800AA33C(str, c, 1, 1) != 0) {
        ret = func_80063E30(a, b, c, CfRes_stub_63ACC(&str), d, e);
    }
    return ret;
}

// 0x34-byte sub-record scanned by func_80063FA8: it starts at slot+0x08 and
// its +4 word is compared against the search value. Kept separate from the
// slot struct so &slot.rec yields the retail base + idx*0x3C + 8 address.
struct ResScanRec {
    u32 field_00;   // slot+0x08
    u32 field_04;   // slot+0x0C - id compared against the search value
    u8 _08[0x2C];
};

// 0x3C-byte table slot walked by func_80063FA8 (payload at +0x08).
struct ResScanSlot {
    u8 _00[8];
    ResScanRec rec; // +0x08
};

// func_80063FA8: scan the 0x3C-stride resource table for the first entry
// whose +0x0C id field matches `value` (0 = no match). `start`/`end` bound the
// number of scanned slots, `offset` is the first table index and `stride`
// advances the index each step. The record pointer is dereferenced without a
// null check when the index is out of range - retail does the same load from
// a null base.
// extern "C": retail exports this helper under its pre-mangled name; the
// callers here (func_800640F4 / func_800641CC) carry the unmangled reloc.
extern "C" ResInfoEntry* func_80063FA8(ResInfoEntry* base, int value, int start, int end, int stride, int offset) {
    ResScanSlot* slots = (ResScanSlot*)base;
    ResInfoEntry* result = 0;
    if (value != 0) {
        // entry declared before the accumulator: MWCC colors the pointer
        // first (r5), then cur (r6), and the materialized range flag lands
        // in the r0 scratch - matching retail.
        ResScanRec* entry;
        for (int count = end - start, cur = 0; count > 0; --count) {
            int inRange = 0;
            u32 tableIdx = (u32)(offset + cur);
            if (tableIdx <= 0x81) {
                inRange = 1;
            }
            if (inRange != 0) {
                entry = &slots[tableIdx].rec;
            } else {
                entry = 0;
            }
            if (entry->field_04 == value) {
                return (ResInfoEntry*)entry;
            }
            cur += stride;
        }
    }
    return result;
}

// func_80064014 (0x800647E0): file-event completion handler. Walks the
// 0x3C-stride resource-entry table from self+0xBC for the entry whose pending
// file handle matches the event; clears its in-flight state and, on a
// successful read, notifies the entry's lookup object. Returns 1 when an entry
// was updated, 0 otherwise.
extern "C" int __declspec(noinline) func_80064014(CfRes* self, CEventFile* evt, u32 field) {
    if (field != 0) {
        ResInfoEntry* table = (ResInfoEntry*)((u8*)self + 0xbc);
        int n = 0x7f;
        for (u32 idx = 3; n > 0; idx++, n--) {
            ResInfoEntry* entry = (idx < 0x82 && idx <= 0x81) ? table : 0;
            if ((u32)(uintptr_t)entry->field_0x28 == field) {
                if (evt->unk0 == 1) {
                    entry->field_0x00 = (entry->field_0x00 & 0xFFFFFFAEu) | 2;
                    entry->field_0x28 = 0;
                    if (entry->field_0x2C != 0) {
                        ((CfResLookupV38*)entry->field_0x2C)->_v038(entry);
                    }
                } else {
                    entry->field_0x00 = (entry->field_0x00 & 0xFFFFFFACu);
                    entry->field_0x28 = 0;
                }
                return 1;
            }
            table++;
        }
    }
    return 0;
}

extern "C" int func_800640F4(int a, void* b) {
    int result = -1;
    void* r = func_80063FA8((ResInfoEntry*)(uintptr_t)a, (int)(uintptr_t)b, 0, 130, 1, 0);
    if (r)
        result = (0 - *(u32*)((u8*)r + 40)) == 0;
    return result;
}

// func_8006414C: true when the packed resource tag has the marker shape
// (field-27 bits == 8 and both low index fields zero).
extern "C" int __declspec(noinline) func_8006414C(u32 self) {
    u32 bits = CfRes_extractBits27_5((void*)(uintptr_t)self);
    int mid = (int)func_80062524((void*)(uintptr_t)self);
    int low = (int)CfRes_getAddrLow10((void*)(uintptr_t)self);
    return (bits == 8 && mid == 0 && low == 0) ? 1 : 0;
}

extern "C" __declspec(noinline) u32 CfRes_getAddrLow10(void* self) { return (u32)(uintptr_t)self & 0x3FF; }

// func_800641CC: resolve a packed resource token `a` through the manager's
// resource table. Special bit-27 fields (7/8) are normalized before the
// scan; after a match, `out` receives whether the entry's load is complete
// and, for the special types, the entry's +0x2C lookup object is finalized
// through its vtable (vcall04/05/07/08).
// func_800641CC: resolve a packed resource token `packed` through the
// manager's resource table (func_80063FA8 scan of 0x82 slots). Bits 27-31 of
// the token select normalization (7 -> 4, 8 -> 3 unless already the pure
// (8,0,0) marker reported by func_8006414C). After a hit, *out receives
// whether the entry finished loading; finished special-class entries are
// finalized through their lookup-object vtable (vcall04/05/07/08), everything
// else through vcall02 with the token's bits 20-26.
int __declspec(noinline) func_800641CC(void* inst, u32 packed, u32* out) {
    int result = 0;
    *out = -1;
    if (packed == 0) {
        return 0;
    }
    u32 bits = CfRes_extractBits27_5((void*)(uintptr_t)packed);
    int vcflag = func_8006414C(packed);
    // special marks the bits classes handled by the finalize switch below;
    // every case assigns it so MWCC keeps the dispatch as one jump table.
    int special;
    switch (bits - 3) {
    case 4:   // bits 7 -> normalize to 4
        packed = CfRes_packShift27(packed, 4);
        special = 1;
        break;
    case 5:   // bits 8 -> normalize to 3 unless the pure form
        if (vcflag == 0) {
            packed = CfRes_packShift27(packed, 3);
        }
        special = 1;
        break;
    case 0:
        special = 1;
        break;
    case 1:
        special = 1;
        break;
    case 2:
        special = 1;
        break;
    case 3:
        special = 1;
        break;
    case 6:
        special = 1;
        break;
    case 7:
        special = 1;
        break;
    case 8:
        special = 1;
        break;
    case 9:
        special = 1;
        break;
    case 10:
        special = 1;
        break;
    case 11:
        special = 1;
        break;
    case 12:
        special = 1;
        break;
    case 13:
        special = 1;
        break;
    case 14:
        special = 1;
        break;
    case 15:
        special = 1;
        break;
    case 16:
        special = 1;
        break;
    case 17:
        special = 1;
        break;
    case 18:
        special = 1;
        break;
    case 19:
        special = 1;
        break;
    default:
        special = 0;
        break;
    }
    ResInfoEntry* entry = func_80063FA8((ResInfoEntry*)inst, (int)packed, 0, 0x82, 1, 0);
    if (entry != NULL) {
        int done = (entry->field_0x28 == 0);
        *out = done;
        if (special != 0 && done == 1) {
            if (bits - 3 <= 0x13) {
                switch (bits - 3) {
                case 1:   // bits 4
                    result = (int)(uintptr_t)CfRes_vcall04((void*)entry);
                    break;
                case 5:   // bits 8: pure form -> vcall02, else vcall05
                    if (vcflag != 0) {
                        result = (int)(uintptr_t)CfRes_vcall02((void*)entry, 0);
                    } else {
                        result = (int)(uintptr_t)CfRes_vcall05((void*)entry);
                    }
                    break;
                case 0xF:   // bits 0x12
                    result = (int)(uintptr_t)CfRes_vcall08((void*)entry);
                    break;
                case 0x10:  // bits 0x13
                    result = (int)(uintptr_t)CfRes_vcall07((void*)entry);
                    break;
                default:
                    result = (int)(uintptr_t)CfRes_vcall02(
                        (void*)entry, (void*)(uintptr_t)func_8006251C((void*)(uintptr_t)packed));
                    break;
                }
            }
        } else {
            result = (int)(uintptr_t)CfRes_vcall02(
                (void*)entry, (void*)(uintptr_t)func_8006251C((void*)(uintptr_t)packed));
        }
    }
    return result;
}

extern "C" unsigned long __declspec(noinline) CfRes_packShift27(unsigned long a, unsigned long b) {
    return (a & 0x7FFFFFF) | (b << 27);
}

// C++ virtual thunk with struct access (4 dummies + RTTI = offset 24).
// Declared int-returning so callers can capture the r3 result (retail
// callers do `mr rX, r3` after the call); the body is a void virtual call
// followed by blr, so the bytes are identical to the void form.
struct CfResObj_64370 { u8 _00[0x2C]; struct CfResSub_64370* sub; };
struct CfResSub_64370 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(void* self); };
extern "C" int CfRes_vcall04(void* self) {
    ((CfResObj_64370*)self)->sub->m04(self);
}

extern "C" bool CfRes_stubFalse_64388() { return false; }

// C++ virtual thunk with struct access (5 dummies + RTTI = offset 28).
// int-returning so callers can capture r3 (see CfRes_vcall04).
struct CfResObj_64390 { u8 _00[0x2C]; struct CfResSub_64390* sub; };
struct CfResSub_64390 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(void* self); };
extern "C" int CfRes_vcall05(void* self) {
    ((CfResObj_64390*)self)->sub->m05(self);
}

extern "C" bool CfRes_stubFalse_643A8() { return false; }

// C++ virtual thunk with struct access (8 dummies + RTTI = offset 40).
// int-returning so callers can capture r3 (see CfRes_vcall04).
struct CfResObj_643B0 { u8 _00[0x2C]; struct CfResSub_643B0* sub; };
struct CfResSub_643B0 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(void* self); };
extern "C" int CfRes_vcall08(void* self) {
    ((CfResObj_643B0*)self)->sub->m08(self);
}

extern "C" bool CfRes_stubFalse_643C8() { return false; }

// C++ virtual thunk with struct access (7 dummies + RTTI = offset 36).
// int-returning so callers can capture r3 (see CfRes_vcall04).
struct CfResObj_643D0 { u8 _00[0x2C]; struct CfResSub_643D0* sub; };
struct CfResSub_643D0 { virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(void* self); };
extern "C" int CfRes_vcall07(void* self) {
    ((CfResObj_643D0*)self)->sub->m07(self);
}

extern "C" bool CfRes_stubFalse_643E8() { return false; }

// func_800643F0: resource-load dispatcher. `packed` is the packed resource
// token; a jump table keyed on its bits-27-31 field selects a pre-check
// (already-loaded probe, pending-read poll, or token repack). Then the entry
// is located/cleaned up, a buffer is allocated (archive cache first, then the
// MEM2/MEM1/scene memory-manager fallback chain) and handed to the loader.
int func_800643F0(void* self, u32 packed, int flag, int kind) {
    // Declaration order feeds MWCC's callee-saved register coloring (retail:
    // low=r25, check=r26, inst=r27, result=r28, queued=r29, bits=r30, e24=r31).
    int result = 0;
    u32 low;
    int check;
    int inst;
    u32 bits;
    int queued;
    u32 e24bit;

    if (packed == 0) {
        return 0;
    }
    if (isSceneLoading__Q22cf13CfGameManagerFv(self)) {
        return 0;
    }
    CfRes_clearE28Mask(0x40);
    inst = CfRes_getInstanceField();
    bits = CfRes_extractBits27_5((void*)(uintptr_t)packed);
    func_80062524((void*)(uintptr_t)packed);
    low = CfRes_getAddrLow10((void*)(uintptr_t)packed);
    // `check` starts as the pure-marker query and is later reused for the
    // memory-probe result (retail keeps both in one register).
    queued = 0;
    check = func_8006414C(packed);
    e24bit = CfRes_getE24Bit18();

    // Jump table over `bits` (0x00-0x16); cases >= 6 share the fallthrough.
    switch (bits) {
    case 0:
        // Probe whether an equivalent resource id is already resident.
        if (low != 9) {
            result = func_80063560(func_8006251C((void*)(uintptr_t)packed), 1, flag);
        }
        if (low == 9 || result < 0) {
            queued = 1;
        } else {
            result = func_80062928(packed, flag != 0 ? 4 : 5) != 0;
        }
        break;
    case 1: {
        // Pending async read: done when it already completed (-1 = failed).
        u32 pending;
        func_80062AD8(packed, &pending);
        if (pending != (u32)-1) {
            return 1;
        }
        queued = 1;
        break;
    }
    case 2:
        queued = 1;
        break;
    case 3:
        packed = CfRes_packShift27(packed, 4);
        queued = 1;
        break;
    case 4:
        if (check == 0) {
            packed = CfRes_packShift27(packed, 3);
            queued = 1;
        }
        break;
    case 5:
        if (flag != 0) {
            check = 1;
        }
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    default:
        break;
    }

    u32 out10;
    u32 outC;
    if (flag != 0 && findResEntry(inst, packed, &out10, &outC)) {
        return 1;
    }
    if (queued == 0 && check == 0) {
        return result;
    }

    int size = func_800623DC(packed);
    if (flag != 0) {
        u8* mgr = (u8*)func_80062F60();
        CfRes_delegateCleanup(mgr);
        if (!CfRes_isField4Zero(mgr)) {
            CfRes_initStruct_64994(mgr);
        }
        if (!func_8006861C(inst, packed, &out10, &outC)) {
            // Memory probe; kinds 3/4/0x16 carry a 0x3800 header allowance.
            if (bits == 4 || bits == 3 || bits == 0x16) {
                size += 0x3800;
            }
            check = func_800A8E6C(-((size + 0x7ff) & ~0x7FF), 1) == NULL;
        }
    }

    // Fetch or create the resource record.
    ResInfoEntry* entry = (ResInfoEntry*)(flag != 0 ? func_800685BC(inst, packed, &out10, &outC)
                                                    : func_80068564(inst, packed, &out10, &outC));
    if (entry == NULL) {
        if (flag == 0) {
            CfRes_setE28Mask(0x40);
        }
        return result;
    }
    if (CfRes_vcall02(entry, NULL) != NULL) {
        // Already resident.
        if (flag != 0) {
            entry->field_0x24 = CfRes_getE30();
        } else if (func_800649F4(entry) == 4 && !CfRes_checkMask_64A08((u8*)entry, 0x800)) {
            CfRes_vcall14(entry);
        }
        return 1;
    }
    if (size <= 0) {
        return result;
    }

    // Allocation: archive cache first, then the memory-manager chain. The
    // rounded request size doubles as func_80066C74's third argument.
    void* buf;
    u32 rounded;
    if (flag != 0) {
        if (bits == 4 || bits == 3 || bits == 0x16) {
            size += 0x3800;
        }
        rounded = (size + 0x7ff) & ~0x7FF;
        if (check != 0) {
            buf = func_800A8B98(rounded);
            if (buf == NULL) {
                // Cache full: walk the MEM2 / MEM1 / scene-heap fallback chain.
                CfRes_orBits_649B4((u8*)(uintptr_t)inst, 2);
                if (e24bit == 0) {
                    return 0;
                }
                for (int h = 0; h < 3; h++) {
                    // 0 = MEM2, 1 = MEM1, 2 = the current scene heap.
                    u32 handle;
                    if (h == 0) {
                        handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
                    } else if (h == 1) {
                        handle = (u32)getHandleMEM1__Q23mtl10MemManagerFv();
                    } else {
                        handle = (u32)func_80495FF0((void*)CfRes_getE14());
                    }
                    if (rounded <= getMaxAllocSize__Q23mtl10MemManagerFUl(handle)) {
                        buf = h == 0 ? allocate_tail__Q23mtl10MemManagerFUlUli(handle, rounded, 0x20)
                                     : allocate_head__Q23mtl10MemManagerFUlUli(handle, rounded, 0x20);
                        if (buf != NULL) {
                            CfRes_orBits_649CC((u8*)entry, 0x1000);
                            break;
                        }
                    }
                }
                if (buf == NULL) {
                    return 0;
                }
            }
            CfRes_orBits_649CC((u8*)entry, e24bit != 0 ? 0x400 : 0x200);
            entry->field_0x24 = CfRes_getE30();
            kind = 4;
        } else {
            buf = func_800A8E6C(-(int)rounded, 0);
            kind = 4;
        }
    } else {
        // Round the request up to a whole 0xBD000 archive granule; `kind`
        // stays at the caller's value on this path.
        int gran = size / 0xBD000;
        if (size % 0xBD000 != 0) {
            gran++;
        }
        rounded = gran * 0xBD000;
        buf = func_800A8B98(rounded);
        if (buf == NULL) {
            CfRes_setE28Mask(0x40);
        }
    }

    if (buf == NULL) {
        return 0;
    }
    func_80066C74((u8*)entry, buf, rounded);
    if (func_800A8BD8(buf) != 0) {
        func_800A8C1C(buf, 0, packed);
    } else if (func_800A9024(buf) != 0) {
        func_800A92F8(buf, 0, packed);
    }
    result = func_80062998(outC, packed, kind) != 0;
    return result;
}

extern "C" __declspec(noinline) void CfRes_clearE28Mask(unsigned long mask) {
    extern unsigned long lbl_eu_80663E28;
    lbl_eu_80663E28 &= ~mask;
}

struct CfResData {
    u8 _pad00[0x2C];
    void* field_2C;
};

extern "C" __declspec(noinline) void CfRes_delegateCleanup(void* self) {
    return func_80065CA4((CfResCleanupEntry*)(static_cast<CfResData*>(self)->field_2C), (CfResCleanupEntry*)self);
}

extern "C" __declspec(noinline) unsigned long CfRes_isField4Zero(u8* self) {
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

extern "C" __declspec(noinline) void CfRes_orBits_649B4(u8* self, u32 bits) {
    *(u32*)self |= bits;
}

// lbl_eu_80663E14 is declared by the shared headers (CScn*); cast at use.
extern "C" __declspec(noinline) int CfRes_getE14() { return (int)(uintptr_t)lbl_eu_80663E14; }

extern "C" __declspec(noinline) void CfRes_orBits_649CC(u8* self, u32 bits) {
    *(u32*)self |= bits;
}

extern u32 lbl_eu_80663E30;
extern "C" __declspec(noinline) int CfRes_getE30() { return lbl_eu_80663E30; }

extern "C" __declspec(noinline) void CfRes_setE28Mask(u32 bits) {
    extern u32 lbl_eu_80663E28;
    lbl_eu_80663E28 |= bits;
}

// vtable+0xC dispatch on *(self+0x2C) (retail: lwz r3,0x2c; lwz r12,0; lwz r12,0xc; mtctr; bctr)
struct CfResVtC { virtual void m0(); virtual void m1(); };
extern "C" __declspec(noinline) int func_800649F4(void* self) { ((CfResVtC*)(*(void**)((char*)self + 0x2C)))->m1(); }

extern "C" __declspec(noinline) int CfRes_checkMask_64A08(u8* self, u32 mask) {
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

extern "C" u32 __declspec(noinline) CfRes_getAddrLow10_64B70(u8* self) { return (u32)(uintptr_t)self & 0x3FF; }

// func_80064B78 (0x8006533C): entry-state resolve helper. Looks up the grid
// entry for (a, d) and checks whether it still tracks the token `b`; on a hit
// the vtable cleanup path runs for the special types (d == 9/10) and `b` is
// returned. On a miss, the entry is detached, its pending handle cleared and
// the archive read is dispatched (func_80063F1C), zeroing the token on
// failure (type 7 failures return 0 immediately).
extern "C" int __declspec(noinline) func_80064B78(int inst, int a, int b, int d, int e) {
    if (b == 0) {
        return b;
    }
    if (a < 0) {
        return 0;
    }
    ResInfoEntry* entry = (ResInfoEntry*)getEntryPtrGrid((char*)(inst + 4), a, d);
    if (CfRes_vcall17((u8*)entry, (void*)(uintptr_t)b) != 0) {
        if (CfRes_vcall14(entry) != 0) {
            if ((u32)(d - 9) <= 1) {
                CfRes_vcall38((u8*)entry);
            }
        }
        return b;
    }
    if (entry->field_0x04 != 0) {
        func_80066714(entry, true);
    }
    CfRes_delegateCleanup(entry);
    void* vc = CfRes_vcall02(entry, (void*)(uintptr_t)func_8006251C((void*)(uintptr_t)b));
    if (vc == 0) {
        ml::FixStr<64> str;
        func_800AA33C(str, (u32)(uintptr_t)b, 0, 0);
        return 0;
    }
    int result = func_80063F1C((u8*)(uintptr_t)inst, (u8*)vc, (u32)(uintptr_t)b, (u8*)entry, e);
    if (d == 7 && result == 0) {
        return 0;
    }
    if (result == 0) {
        b = 0;
    }
    return b;
}

// func_80064A74: resolve a packed resource token by its bits-27 field: pick a
// payload value (special-casing bits 2/7 to index fields of the token) and
// hand off to func_80064B78. `b` is returned unchanged when zero.
extern "C" int __declspec(noinline) func_80064A74(int inst, int a, int b, int c) {
    if (b == 0) {
        return b;
    }
    u32 bits = CfRes_extractBits27_5((void*)(uintptr_t)b);
    u32 low = CfRes_getAddrLow10((void*)(uintptr_t)b);
    int d;
    if (bits == 2) {
        d = (int)CfRes_getAddrLow10_64B70((u8*)(uintptr_t)b);
    } else if (bits == 7) {
        if (low == 0xa) {
            d = 8;
        } else {
            d = 0;
        }
    } else if (bits == 8) {
        return 0;
    } else if (bits == 5) {
        d = 6;
    } else if (bits == 0xb) {
        d = 7;
    } else if (bits == 0xf) {
        d = 0xa;
    } else if (bits == 0x14) {
        d = 9;
    } else {
        return 0;
    }
    return func_80064B78(inst, a, b, d, c);
}

// 15 dummies (m00-m14) span indices 2-16 with MWCC overhead; m17 at index 17 = offset 68
struct CfResSub_64CB8 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11();
    virtual void m12(); virtual void m13(); virtual void m14();
    virtual int m17(void* self, void* arg);
};
struct CfResObj_64CB8 { u8 _00[0x2C]; CfResSub_64CB8* sub; };
extern "C" __declspec(noinline) int CfRes_vcall17(u8* self, void* arg) {
    return ((CfResObj_64CB8*)self)->sub->m17(self, arg);
}

// func_80064CD8: resolve a packed resource token when its bits-27 field is
// one of the supported types (2-8 or 11); otherwise 0. `b` is returned
// unchanged when zero, and negative `a` fails.
extern "C" int __declspec(noinline) func_80064CD8(int inst, int a, int b, int c) {
    if (b == 0) {
        return b;
    }
    if (a < 0) {
        return 0;
    }
    u32 bits = CfRes_extractBits27_5((void*)(uintptr_t)b);
    int d;
    if (bits == 3 || bits == 4) {
        d = 0;
    } else if (bits == 7 || bits == 8) {
        d = 0;
    } else if (bits == 5 || bits == 6) {
        d = 0;
    } else if (bits == 2) {
        d = 0;
    } else if (bits == 0xb) {
        d = 0;
    } else {
        return 0;
    }
    return func_80064DC4(inst, a, b, d, c);
}

// func_eu_80065590: re-register the resource-table entry at `index` for the
// packed token `ptr`: detach the old registration, then repack (entry id,
// params) from the token and store the packed result into the entry's
// field_04.
void __declspec(noinline) func_eu_80065590(int inst, int index, u8* ptr) {
    if (ptr == 0) {
        return;
    }
    ResInfoEntry* entry = (ResInfoEntry*)getEntryPtr((char*)inst + 4, index, 0);
    if (entry == 0) {
        return;
    }
    if (CfRes_vcall17((u8*)entry, ptr) == 0) {
        return;
    }
    if (entry->field_0x04 != 0) {
        func_80066714(entry, true);
    }
    CfRes_delegateCleanup(entry);
    u32 out0, out1, out2, out3;
    func_800AA318((u32)(uintptr_t)ptr, &out0, &out1, &out2, &out3);
    entry->field_0x04 = func_eu_80065640(out0, out1, out2, 0x63);
}

extern "C" __declspec(noinline) u32 func_eu_80065640(u32 a, u32 b, u32 c, u32 d) {
    u32 t0 = (c << 10) & 0xFFFFFC00;
    u32 t1 = (b << 20) & 0xFFF00000;
    u32 t2 = (a << 27) & 0xF8000000;
    u32 t3 = d | t0;
    u32 t4 = t2 | t1;
    return t3 | t4;
}

// func_80064DC4: resolve a packed resource token through the table entry at
// (inst+4, a, d): when the entry still tracks the token, run its cleanup
// probe and return the token; otherwise detach it, clear its pending handle
// and dispatch the archive read (func_80063F1C), zeroing the token on
// failure. `b` is returned unchanged when zero.
extern "C" int __declspec(noinline) func_80064DC4(int inst, int a, int b, int d, int e) {
    if (b == 0) {
        return b;
    }
    ResInfoEntry* entry = (ResInfoEntry*)getEntryPtr((char*)inst + 4, a, d);
    if (CfRes_vcall17((u8*)entry, (void*)(uintptr_t)b) != 0) {
        if (CfRes_vcall14((u8*)entry) != 0) {
            CfRes_vcall38((u8*)entry);
        }
        return b;
    }
    if ((int)entry->field_0x04 != 0) {
        func_80066714(entry, true);
    }
    CfRes_delegateCleanup(entry);
    void* vc = CfRes_vcall02(entry, 0);
    if (func_80063F1C((u8*)(uintptr_t)inst, (u8*)vc, (u32)(uintptr_t)b, (u8*)entry, e) == 0) {
        b = 0;
    }
    return b;
}

extern "C" __declspec(noinline) u32 CfRes_getField18_64F58(u8* self) { return *(u32*)((u8*)self + 0x18); }

extern "C" __declspec(noinline) void CfRes_setBits11_64F60(u8* self) {
    u32 val = *(u32*)self;
    *(u32*)self = (val & ~0x42) | 0x11;
}

// func_80064EB0 (0x80065748): resolve the packed id via func_80064F78 and,
// when the target grid entries are valid, link them into the resolved record.
int __declspec(noinline) func_80064EB0(int inst, int a, int b, int c) {
    u8* p2;
    int v;
    u8* p1;
    v = func_80064F78(inst, a, b, c);
    p1 = (u8*)func_80062F58((u8*)(inst + 4));
    p2 = (u8*)func_80062FA0((u8*)(inst + 4));
    if (v != 0 && CfRes_getField18_64F58(p2) != 0) {
        u32 f18 = CfRes_getField18_64F58(p2);
        int vc = (int)(uintptr_t)CfRes_vcall02(p1, 0);
        CfRes_initFields4(p1, v, 0, vc, (int)f18);
        CfRes_setBits11_64F60(p1);
    }
    return v;
}

// func_80064F78 (0x80065810): resolve a packed resource token through the
// +0xB8 slot. Packs (a, b); when the slot already tracks the token, return it.
// Otherwise clean the slot, obtain the lookup object (falling back to
// func_800A8CD4) and dispatch the archive read (func_80063F1C), zeroing the
// token on failure.
extern "C" int __declspec(noinline) func_80064F78(int inst, int a, int b, int c) {
    int result = (int)func_800AA2BC((u32)a, (u32)b);
    u8* slot = (u8*)func_80062FA0((u8*)(inst + 4));
    if (result == 0) {
        return result;
    }
    if (CfRes_vcall17(slot, (void*)(uintptr_t)result) != 0) {
        return result;
    }
    CfRes_delegateCleanup(slot);
    void* vc = CfRes_vcall02(slot, 0);
    if (vc == 0) {
        vc = (void*)(uintptr_t)func_800A8CD4();
        ((ResInfoEntry*)slot)->data = (u32*)vc;
    }
    if (func_80063F1C((u8*)(uintptr_t)inst, (u8*)vc, (u32)(uintptr_t)result, slot, c) == 0) {
        result = 0;
    }
    return result;
}

// func_80065050: resolve a packed resource token through the +0xf8 slot.
// When the slot does not already track the token, clean it, format the token
// into a path (packed-name + rodata suffix) and run the archive-read
// pipeline (func_80063E30), zeroing the token on failure.
extern "C" int __declspec(noinline) func_80065050(int inst, int a, int b, int c) {
    u32 packed = func_800AA2BC((u32)a, (u32)b);
    u8* slot = (u8*)func_80062FE8((u8*)(inst + 4));
    if (CfRes_vcall17(slot, (void*)(uintptr_t)packed) != 0) {
        return (int)packed;
    }
    CfRes_delegateCleanup(slot);
    void* vc = CfRes_vcall02(slot, 0);
    ml::FixStr<64> str;
    if (func_800AA33C(str, packed, 1, 1) != 0) {
        ml::FixStr<64> path;
        func_80063C7C(path, CfRes_stub_63ACC(&str)->mString);
        CfRes_strcatAppend((char*)&path, lbl_eu_804FB214 + 0x1a);
        if (func_80063E30((void*)(uintptr_t)inst, vc, packed, CfRes_stub_63ACC(&path), slot, c) == 0) {
            packed = 0;
        }
    } else {
        packed = 0;
    }
    return (int)packed;
}

// func_80065158: resolve a packed resource token through the +0x130 slot.
// When the slot does not already track the token, clean it, format the token
// into a path (packed-name + rodata suffix) and run the archive-read
// pipeline (func_80063E30), zeroing the token on failure.
extern "C" int __declspec(noinline) func_80065158(int inst, int a, int b, int c, int d) {
    u32 packed = func_800AA2BC((u32)b, (u32)c);
    u8* slot = (u8*)func_80063030((u8*)(inst + 4));
    if (CfRes_vcall17(slot, (void*)(uintptr_t)packed) != 0) {
        return (int)packed;
    }
    CfRes_delegateCleanup(slot);
    ml::FixStr<64> str;
    if (func_800AA33C(str, packed, 1, 1) != 0) {
        ml::FixStr<64> path;
        func_80063C7C(path, CfRes_stub_63ACC(&str)->mString);
        CfRes_strcatAppend((char*)&path, lbl_eu_804FB214 + 0x1e);
        if (func_80063E30((void*)(uintptr_t)inst, (void*)(uintptr_t)a, packed,
                          CfRes_stub_63ACC(&path), slot, d) == 0) {
            packed = 0;
        }
    } else {
        packed = 0;
    }
    return (int)packed;
}

extern "C" __declspec(noinline) unsigned long CfRes_packThreeFields(unsigned long a, unsigned long b, unsigned long c) {
    return ((a & 0x1F) << 27) | ((b & 0xFFF) << 20) | ((c & 0x3FFFFF) << 10);
}

// func_80065254 (0x80065AEC): run the archive-update step for the manager's
// +0x16c slot; when the slot's vtable probe returns an object, pack the
// game-manager fields and store them into the slot.
void __declspec(noinline) func_80065254(int inst, u8* arg) {
    u8* p = (u8*)func_80063078((u8*)(inst + 4));
    func_80066788(p, 0, 0, 0);
    void* vc = CfRes_vcall02(p, 0);
    if (vc != 0) {
        u16 first;
        u16 second;
        getEventHalfwordPair__Q22cf13CfGameManagerFv(&first, &second);
        int packed = (int)CfRes_packThreeFields(0x1d, first, second);
        CfRes_initFields4(p, packed, 0, (int)(uintptr_t)vc, (int)(uintptr_t)arg);
        CfRes_resetState2(p);
    }
}

// func_80065314: resolve/refresh a resource slot. When the lookup object
// reports the entry still in use, run its vtable cleanup path and return the
// request id unchanged; otherwise detach it, rebuild the path and dispatch
// the archive read (func_80063F1C), zeroing the request id on failure.
int __declspec(noinline) func_80065314(int inst, int a, int b) {
    u8* slot = (u8*)func_80063078((u8*)(inst + 4));
    if (a == 0) {
        return a;
    }
    if (CfRes_vcall17(slot, (void*)(uintptr_t)a) != 0) {
        if (CfRes_vcall14(slot) != 0) {
            CfRes_vcall38(slot);
        }
        return a;
    }
    CfRes_delegateCleanup(slot);
    void* vc = CfRes_vcall02(slot, 0);
    if (func_80063F1C((u8*)(uintptr_t)inst, (u8*)vc, (u32)(uintptr_t)a, slot, b) == 0) {
        a = 0;
    }
    return a;
}

// func_eu_80065C7C (0x80065C7C): EU-style resource load dispatch. Packs the
// (a, b) indices into a token; if the +0x1E4 slot already tracks it, raise the
// 0x2000 mask and return the token. Otherwise clean the slot, build the path
// string (rodata base +0x23 formatted with the two indices) and run the
// archive-read pipeline (func_80063E30), zeroing the token on failure.
extern "C" int __declspec(noinline) func_eu_80065C7C(int inst, int a, int b, int c) {
    int result = (int)func_800AA2BC((u32)a, (u32)b);
    u8* slot = (u8*)func_eu_80065D60((u8*)(inst + 4));
    if (CfRes_vcall17(slot, (void*)(uintptr_t)result) != 0) {
        CfRes_setE28Mask(0x2000);
        return result;
    }
    CfRes_delegateCleanup(slot);
    ml::FixStr<64> str;
    str.format(lbl_eu_804FB214 + 0x23, a, b);
    void* vc = CfRes_vcall02(slot, 0);
    if (func_80063E30((void*)(uintptr_t)inst, vc, (u32)(uintptr_t)result, CfRes_stub_63ACC(&str), slot, c) == 0) {
        result = 0;
    }
    return result;
}

extern "C" __declspec(noinline) void* func_eu_80065D60(void* self) { return (char*)self + 0x1e4; }

// func_800653E4: twin of func_80065314 over the +0x220 slot (see there).
extern "C" int __declspec(noinline) func_800653E4(int inst, int a, int b) {
    u8* slot = (u8*)func_80063108((u8*)(inst + 4));
    if (a == 0) {
        return a;
    }
    if (CfRes_vcall17(slot, (void*)(uintptr_t)a) != 0) {
        if (CfRes_vcall14(slot) != 0) {
            CfRes_vcall38(slot);
        }
        return a;
    }
    CfRes_delegateCleanup(slot);
    void* vc = CfRes_vcall02(slot, 0);
    if (func_80063F1C((u8*)(uintptr_t)inst, (u8*)vc, (u32)(uintptr_t)a, slot, b) == 0) {
        a = 0;
    }
    return a;
}

// func_800654B4: resolve a packed resource token through one of two slots
// chosen by the token's bits-27 field (0x12 -> +0x25c slot, 0x13 -> +0x1a8
// slot). When the slot does not already track the token, detach it, clean it
// and dispatch the archive read (func_80063F1C), zeroing the token on failure.
extern "C" int __declspec(noinline) func_800654B4(int inst, int a, int b) {
    u32 bits = CfRes_extractBits27_5((void*)(uintptr_t)a);
    u8* slot = 0;
    if (bits == 0x12) {
        slot = (u8*)func_80063110((void*)(inst + 4));
    } else if (bits == 0x13) {
        slot = (u8*)func_80063118((void*)(inst + 4));
    }
    if (slot == 0 || a == 0) {
        return a;
    }
    if (CfRes_vcall17(slot, (void*)(uintptr_t)a) != 0) {
        if (CfRes_vcall14(slot) != 0) {
            CfRes_vcall38(slot);
        }
        return a;
    }
    CfRes_vcall34(slot);
    CfRes_delegateCleanup(slot);
    void* vc = CfRes_vcall02(slot, 0);
    if (func_80063F1C((u8*)(uintptr_t)inst, (u8*)vc, (u32)(uintptr_t)a, slot, b) == 0) {
        a = 0;
    }
    return a;
}

// func_800655C4 (0x80065F48): resolve a packed resource token through the
// +0x298 slot (twin of func_80065314 over the +0x16C slot).
extern "C" int __declspec(noinline) func_800655C4(int inst, int a, int b) {
    u8* slot = (u8*)func_800630C0((u8*)(inst + 4));
    if (a == 0) {
        return a;
    }
    if (CfRes_vcall17(slot, (void*)(uintptr_t)a) != 0) {
        if (CfRes_vcall14(slot) != 0) {
            CfRes_vcall38(slot);
        }
        return a;
    }
    CfRes_delegateCleanup(slot);
    void* vc = CfRes_vcall02(slot, 0);
    if (func_80063F1C((u8*)(uintptr_t)inst, (u8*)vc, (u32)(uintptr_t)a, slot, b) == 0) {
        a = 0;
    }
    return a;
}

int __declspec(noinline) func_80065694(int a, int type, int* out) {
    CfResNameTable* table = (CfResNameTable*)(uintptr_t)a;
    CfResNameEntry* baseEntry = table->entries;
    CfResNameEntry* e = baseEntry;
    int result = 0;
    *out = 0;
    for (u32 idx = 0; idx < table->field_08; e++, idx++) {
        char name[0x20];
        strncpy(name, e->name, 8);
        name[8] = '\0';
        u32 bits = CfRes_extractBits27_5((void*)(uintptr_t)func_800AA714(name));
        if (type == 0) {
            if (bits >= 2 && bits <= 6) {
                u32 rel = baseEntry[idx].field_00;
                u32 val = baseEntry[idx].field_04;
                *out = (int)val;
                result = (int)((uintptr_t)a + rel);
                goto ret;
            }
        } else if (type == 1) {
            if (bits >= 7 && bits <= 11) {
                u32 rel = baseEntry[idx].field_00;
                u32 val = baseEntry[idx].field_04;
                *out = (int)val;
                result = (int)((uintptr_t)a + rel);
                goto ret;
            }
        } else if (type == 2) {
            if (bits >= 0xc && bits <= 0x11) {
                u32 rel = baseEntry[idx].field_00;
                u32 val = baseEntry[idx].field_04;
                *out = (int)val;
                result = (int)((uintptr_t)a + rel);
                goto ret;
            }
        } else if (type == 4) {
            if (bits >= 0x12 && bits <= 0x16) {
                u32 rel = baseEntry[idx].field_00;
                u32 val = baseEntry[idx].field_04;
                *out = (int)val;
                result = (int)((uintptr_t)a + rel);
                goto ret;
            }
        } else if (type == 3) {
            if (bits == 0x1e) {
                u32 rel = baseEntry[idx].field_00;
                u32 val = baseEntry[idx].field_04;
                *out = (int)val;
                result = (int)((uintptr_t)a + rel);
                goto ret;
            }
        }
    }
ret:
    return result;
}

extern "C" __declspec(noinline) int CfResEntry_incRefCount(u8* self) {
    return ++*(int*)((char*)self + 4);
}

// func_80063994 (0x8006415C): register `b` as the handle of the entry-table
// record at index `a`; when the handle already matches, bump its refcount
// instead of re-registering.
void func_80063994(int a, int b) {
    int inst = CfRes_getInstance();
    if (inst != 0) {
        if ((u32)b == CfResEntry_getHandle((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), (u32)a))) {
            CfResEntry_incRefCount((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), (u32)a));
        } else {
            CfResEntry_setHandle((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), (u32)a), (u32)b);
            CfResEntry_setField4((u8*)CfRes_getTblEntry((u8*)(inst + 0x1ed8), (u32)a), 1);
        }
    }
}

int CfRes_65818::decRefCount() {
    if (field_04 > 0) {
        field_04--;
    }
    return field_04;
}

extern "C" __declspec(noinline) int CfRes_incField8(u8* self) {
    return ++*(int*)((char*)self + 8);
}

cf::CfRes::~CfRes() {
    __dt__8006754C((u8*)&mStorage, -1);
}

// CTTask<cf::CfResTask> destructor - base dtor body is empty (the deleting
// CfResTask dtor drives the CProcess teardown). Defined BEFORE the CfResTask
// dtor so MWCC inlines the base teardown (CProcess dtor + delete flag) into
// it, reproducing the retail shape (the retail standalone symbol exists too).
template<>
CTTask<cf::CfResTask>::~CTTask() {}

// Destroy the embedded resource storage at +0x58 (guard checks the +0x54
// header word), then the CProcess base; the delete-flag path is auto-emitted.
cf::CfResTask::~CfResTask() {
    u8* storage = (u8*)this + 0x54;
    if (storage != 0) {
        __dt__8006754C(storage + 4, -1);
    }
}

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


// cf::CfRes::CfRes() — retail factory ctor. The retail symbol is the
// pre-mangled name __ct__Q22cf5CfResFv (a global function, not a member: a
// member ctor would return `this`, but retail returns the freshly allocated
// manager object). Allocates a 0x1FA4-byte manager, constructs it as a
// CProcess sub-process of `parent` (base ctor, interim CTTask vtable, null
// PTMF slots, CfRes vtable, storage subobject, entry tables), publishes the
// globals and registers the process.
// Zero a run of entry-table slots; kept as a count-parameterized helper so
// each inline expansion keeps the generic guarded counted-loop form.
static void CfRes_zeroSlots(CfResSlot* slot, CfResSlot* end) {
    while (slot < end) {
        slot->field_00 = 0;
        slot->field_04 = 0;
        slot->field_08 = 0;
        slot++;
    }
}

__declspec(noinline) CfResManager* __ct__Q22cf5CfResFv(CProcess* parent, int archiveId) {
    u32 handle = getWorkMem__17CWorkThreadSystemFv();
    CfResManager* mgr = (CfResManager*)allocate__Q23mtl10MemManagerFUlUl(0x1fa4, handle);
    if (mgr != NULL) {
        __ct__8CProcessFv((CProcess*)mgr);
        // Hand-built construction: interim CTTask vtable, null PTMF slots for
        // Move/Draw, then the real CfRes vtable.
        u32* base54 = &mgr->storageVtable;
        mgr->vtable = (u32)lbl_eu_805267EC;
        // Copy the null member-function pointer into both callback slots
        // (12-byte PTMF struct assignment reproduces the retail
        // load-[1]/load-[0] pair then store order).
        struct PTMF12 { u32 w[3]; };
        *(PTMF12*)&mgr->mMoveFunc[0] = *(PTMF12*)(uintptr_t)__ptmf_null;
        *(PTMF12*)&mgr->mDrawFunc[0] = *(PTMF12*)(uintptr_t)__ptmf_null;
        mgr->vtable = (u32)lbl_eu_805267A4;
        *base54 = (u32)lbl_eu_80526830;
        __ct__80066F9C((u8*)&base54[1]);
        // Entry tables live past the storage blob; deriving their addresses
        // from the +0x54 pointer keeps the retail r30-relative offsets.
        u32* tbl1 = base54 + 0x7B6;   // abs 0x1F2C
        tbl1[0] = 0;
        CfResSlot* s1 = (CfResSlot*)(tbl1 + 3);
        tbl1[1] = 0;
        tbl1[2] = 0;
        CfRes_zeroSlots(s1, s1 + 2);
        u32* tbl2 = base54 + 0x7BF;   // abs 0x1F50
        tbl2[0] = 0;
        CfResSlot* s2 = (CfResSlot*)(tbl2 + 3);
        tbl2[1] = 0;
        tbl2[2] = 0;
        CfRes_zeroSlots(s2, s2 + 6);
        lbl_eu_80663D7C = (u32)base54;
        func_80063120((u8*)&base54[1], archiveId);
    }
    lbl_eu_80663D78 = (u32)mgr;
    ((CProcess*)mgr)->Regist(parent, false);
    return mgr;
}


extern "C" __declspec(noinline) char* CfRes_strcatAppend(char* buffer, const char* suffix) {
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

// func_80065CA4: cancel the parent entry's pending device-file handle (if
// any), clear its state fields, then reset the load-progress field. The
// `child` resource object is passed through by callers but not used here.
void __declspec(noinline) func_80065CA4(CfResCleanupEntry* child, CfResCleanupEntry* parent) {
    if (parent->field_28 != 0) {
        cancel__11CDeviceFileFP11CFileHandle(parent->field_28);
        parent->field_04 = 0;
        parent->field_08 = 0;
        parent->field_28 = 0;
        parent->field_00 = 0;
        parent->field_24 = 0;
        parent->field_20 = 0;
    }
    parent->field_14 = 0;
}

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

// Bit-4/bit-3/bit-6 mask gates on the +0 flags word, then the +4 word must
// be non-zero and the +0x28 word zero.
int func_80065D0C(void* a1, CfResCleanupEntry* self) {
    u32 flags = self->field_00;
    if (flags & 0x10)
        return 0;
    if (flags & 0x8)
        return 0;
    if (flags & 0x40)
        return 0;
    // Retail hoists the zero result into r3 before the compares (beqlr/bnelr
    // tail returns), which is the shape MWCC emits for a result variable that
    // starts at 0 and is conditionally set to 1.
    int result = 0;
    if (self->field_04 != 0 && self->field_28 == 0)
        result = 1;
    return result;
}

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

// func_80065D90: lazily resolve the resource base of a ResInfoEntry through
// its +0x2C lookup object, cache it in field_0x10, and register it with the
// effect-singleton list (func_804CC1BC). The retail first arg (r3) is
// unused (same convention as func_8006660C in IResInfo.cpp).
void func_80065D90(int unused, ResInfoEntry* self) {
    if (self->field_0x10 == 0) {
        void* r = self->field_0x2C->getResourceBase(self, 0);
        if (r != 0) {
            u32* fc18 = lbl_eu_8065FC18;
            if (fc18 != 0) {
                self->field_0x10 = (u32)r;
                func_804CC1BC(fc18, r);
            }
        }
    }
}

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

// func_80065E54: lazily resolve the resource base of an entry through its
// +0x2C lookup object, cache it in field_0x14, and publish it to the sound
// manager (subtype 10 -> slot 0, subtype 7 -> slot 1).
void func_80065E54(int unused, ResInfoEntry* self) {
    if (*(u32*)self->field_0x14 == 0) {
        void* result = self->field_0x2C->getResourceBase(self, 0);
        if (result != 0) {
            u8 type = self->field_0x32;
            *(u32*)self->field_0x14 = (u32)(uintptr_t)result;
            if (type == 10) {
                func_801BFA08(0, result, self->field_0x18, 0x106000);
                func_801BFA88(0, 7, 0, 0);
            } else if (type == 7) {
                func_801BFA08(1, result, self->field_0x18, 0x1A0000);
                func_801BFA88(1, 8, 0, 0);
            }
        }
    }
}

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

extern "C" void func_80062BA0() {}
extern "C" void func_80062CD0() {}

// absorb: split1 retail data sections
// generated from retail build/us/asm via absorb
__declspec(section ".data") __attribute__((aligned(8), used)) const volatile unsigned char __absorb_kyoshin_cf_CfRes_cpp_data[0x220] = {
    0x80, 0x06, 0x4A, 0xB4, 0x80, 0x06, 0x4A, 0xB4, 0x80, 0x06, 0x4B, 0x14,
    0x80, 0x06, 0x4B, 0x14, 0x80, 0x06, 0x4B, 0x14, 0x80, 0x06, 0x4A, 0xC0,
    0x80, 0x06, 0x4A, 0xC0, 0x80, 0x06, 0x4B, 0x14, 0x80, 0x06, 0x4B, 0x14,
    0x80, 0x06, 0x4B, 0x14, 0x80, 0x06, 0x4B, 0x14, 0x80, 0x06, 0x4B, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
__declspec(section ".rodata") __attribute__((aligned(8), used)) const unsigned char __absorb_kyoshin_cf_CfRes_cpp_rodata[0x1A0] = {
    0x63, 0x66, 0x3A, 0x3A, 0x43, 0x66, 0x52, 0x65, 0x73, 0x54, 0x61, 0x73,
    0x6B, 0x00, 0x00, 0x00, 0x43, 0x54, 0x54, 0x61, 0x73, 0x6B, 0x3C, 0x63,
    0x66, 0x3A, 0x3A, 0x43, 0x66, 0x52, 0x65, 0x73, 0x54, 0x61, 0x73, 0x6B,
    0x3E, 0x00, 0x00, 0x00, 0x63, 0x66, 0x3A, 0x3A, 0x43, 0x66, 0x52, 0x65,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
int __absorb_kyoshin_cf_CfRes_cpp_sbss_0;
int __absorb_kyoshin_cf_CfRes_cpp_sbss_1;
int __absorb_kyoshin_cf_CfRes_cpp_sbss_2;
int __absorb_kyoshin_cf_CfRes_cpp_sbss_3;
__declspec(section ".sdata") __attribute__((aligned(8), used)) const volatile unsigned char __absorb_kyoshin_cf_CfRes_cpp_sdata[0xA0] = {
    0x80, 0x66, 0x61, 0xF0, 0x80, 0x66, 0x61, 0xF4, 0x80, 0x4F, 0xB1, 0xE0,
    0x80, 0x52, 0x67, 0xC8, 0x80, 0x4F, 0xB1, 0xF0, 0x80, 0x52, 0x68, 0x10,
    0x80, 0x4F, 0xB2, 0x08, 0x80, 0x52, 0x68, 0xB8, 0x2E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x4F, 0xB2, 0x58, 0x80, 0x52, 0x69, 0x2C,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
__declspec(section ".sdata2") __attribute__((aligned(8), used)) const unsigned char __absorb_kyoshin_cf_CfRes_cpp_sdata2[0x10] = {
    0x4B, 0x59, 0x50, 0x00, 0x44, 0x41, 0x50, 0x31, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

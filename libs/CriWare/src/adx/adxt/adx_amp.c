// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_amp
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// ADXAMP object: an ADX audio object with a virtual method table.
// ADXAMP_Start dispatches at vtable offsets 0x14 (stop), 0x18 (query into
// buf), 0x1c (apply buf) and 0x24 (query length).
struct ADXAMP_Obj;

struct ADXAMP_ObjVtbl {
    void (*f_0x00)(struct ADXAMP_Obj*);  // 0x00
    void (*f_0x04)(struct ADXAMP_Obj*);  // 0x04
    void (*f_0x08)(struct ADXAMP_Obj*);  // 0x08
    void (*f_0x0c)(struct ADXAMP_Obj*);  // 0x0c
    void (*f_0x10)(struct ADXAMP_Obj*);  // 0x10
    void (*f_0x14)(struct ADXAMP_Obj*);  // 0x14 - stop
    void (*f_0x18)(struct ADXAMP_Obj*, s32, s32, u32*);  // 0x18 - query data into buf
    void (*f_0x1c)(struct ADXAMP_Obj*, s32, u32*);       // 0x1c - apply data from buf
    void (*f_0x20)(struct ADXAMP_Obj*);  // 0x20
    s32 (*f_0x24)(struct ADXAMP_Obj*, s32);  // 0x24 - query length
};

struct ADXAMP_Obj {
    struct ADXAMP_ObjVtbl* vtbl;  // 0x00
};

struct ADXAMP {
    u8 field_0x00;              // 0x00
    u8 status;                  // 0x01
    s8 count;                   // 0x02
    u8 field_0x03;              // 0x03
    struct ADXAMP_Obj* listA[2];  // 0x04
    struct ADXAMP_Obj* listB[2];  // 0x0c
    u32 field_0x14[6];          // 0x14
    u32 field_0x2c;             // 0x2c
};

void ADXAMP_Destroy(void* p) {
    if (!p) return;
    ADXCRS_Lock();
    memset(p, 0, 0x30);
    ADXCRS_Unlock();
}

void ADXAMP_Start(struct ADXAMP* self) {
    s32 i;
    struct ADXAMP_Obj* obj;
    u32 buf[2];

    // Clear the per-object state array, then the shared state word.
    for (i = 0; i < self->count; i++) {
        self->field_0x14[i] = 0;
    }
    self->field_0x2c = 0;

    // Stop and reset every object in the primary list.
    for (i = 0; i < self->count; i++) {
        s32 size;
        obj = self->listA[i];
        obj->vtbl->f_0x14(obj);
        size = obj->vtbl->f_0x24(obj, 0);
        obj->vtbl->f_0x18(obj, 0, size, buf);
        memset((void*)buf[0], 0, buf[1]);
        obj->vtbl->f_0x1c(obj, 0, buf);
    }

    // Same reset pass over the secondary list.
    for (i = 0; i < self->count; i++) {
        s32 size;
        obj = self->listB[i];
        obj->vtbl->f_0x14(obj);
        size = obj->vtbl->f_0x24(obj, 0);
        obj->vtbl->f_0x18(obj, 0, size, buf);
        memset((void*)buf[0], 0, buf[1]);
        obj->vtbl->f_0x1c(obj, 0, buf);
    }

    self->status = 2;
}

void ADXAMP_Stop(void *self) { *(u8*)((u8*)self + 1) = 0; }

void ADXAMP_SetSfreq(void* self, u32 val) { *(u32*)((u8*)self + 0x20) = val; }

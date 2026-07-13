#include "cview_mock.h"

void (*rb_jumptable_eu_8056B5C0[8])(void);
void (*dc_jumptable_eu_8056B5C0[8])(void);

void cview_init_jumptables(void) {
    int i;
    for (i = 0; i < 8; ++i) {
        rb_jumptable_eu_8056B5C0[i] = 0;
        dc_jumptable_eu_8056B5C0[i] = 0;
    }
}

void* rb_lbl_eu_806655C8;
void* dc_lbl_eu_806655C8;

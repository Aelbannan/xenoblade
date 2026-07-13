#include <types.h>

#include "cviewroot_mock.h"

CViewRootMockRoot* rb_lbl_eu_806655D0;
CViewRootMockRoot* dc_lbl_eu_806655D0;

void* rb_getWorkThread__9CWorkUtilFUl(u32 id) {
    return cviewroot_mock_get_work_thread(id, 0);
}

void* dc_getWorkThread__9CWorkUtilFUl(u32 id) {
    return cviewroot_mock_get_work_thread(id, 1);
}

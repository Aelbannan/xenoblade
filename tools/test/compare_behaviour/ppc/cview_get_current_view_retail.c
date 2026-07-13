extern void* rb_lbl_eu_806655C8;

asm void* ppc_rb_getCurrentView(void) {
    nofralloc
    lis r3, rb_lbl_eu_806655C8@ha
    lwz r3, rb_lbl_eu_806655C8@l(r3)
    blr
}

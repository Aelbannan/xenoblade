extern void* dc_lbl_eu_806655C8;

asm void* ppc_dc_getCurrentView(void) {
    nofralloc
    lis r3, dc_lbl_eu_806655C8@ha
    lwz r3, dc_lbl_eu_806655C8@l(r3)
    blr
}

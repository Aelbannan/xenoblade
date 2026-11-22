#include <harness_catalog.h>

/* Real CriWare rodata object (JP map: cft_ver_str, global linkage).
 * One aligned string-table object: two NUL-terminated build stamps plus the
 * retail 2-byte link tail padding (0x58 slot). */
const char cft_ver_str[] =
    "\nCRI CFT/WII Ver.2.15 Build:Oct 28 2009 11:37:48\n\0"
    "Append: MW4302 WII30Jul2008Patch04\n";

/* Real CriWare BSS version-pointer slot (JP map: cri_verstr_ptr, local
 * linkage). */
static void* cri_verstr_ptr;

void CFT_Ycc420plnToArgb8888Init(void);
void CFT_Init(void) {
    cri_verstr_ptr = (void*)cft_ver_str;
    CFT_Ycc420plnToArgb8888Init();
}

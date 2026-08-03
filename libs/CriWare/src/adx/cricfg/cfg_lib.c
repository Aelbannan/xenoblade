#include <harness_catalog.h>
#include <string.h>

extern s32 lbl_eu_805E66E0;
extern s32 lbl_eu_805E66E4;

int CRICFG_Read(const char *key, int *value) {
    int i, count;
    char *p;
    
    p = (char *)lbl_eu_805E66E0;
    if (p == NULL)
        return -1;
    
    if (*key == '\0') {
        p = NULL;
    } else {
        count = *(s32 *)&lbl_eu_805E66E4;
        for (i = 0; i < count; i++, p += 16) {
            if (strncmp(p, key, 12) != 0)
                continue;
            goto found;
        }
        p = NULL;
    }
found:
    if (p == NULL)
        return -3;
    
    *value = *(s32 *)(p + 12);
    return 0;
}

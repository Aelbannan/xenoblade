#include <harness_catalog.h>
#include <string.h>

extern s32 lbl_eu_805E66E0;
extern s32 lbl_eu_805E66E4;

/* Search the 16-byte config entries; returns the matching entry or NULL. */
static inline char *cfg_find_key(int count, const char *key, char *p) {
    int i;
    for (i = 0; i < count; i++, p += 16) {
        if (strncmp(p, key, 12) == 0)
            return p;
    }
    return NULL;
}

int CRICFG_Read(const char *key, int *value) {
    int count;
    char *p;
    
    p = (char *)lbl_eu_805E66E0;
    if (p == NULL)
        return -1;
    
    if (*key == '\0') {
        p = NULL;
    } else {
        count = *(s32 *)&lbl_eu_805E66E4;
        p = cfg_find_key(count, key, p);
    }
    if (p == NULL)
        return -3;
    
    *value = *(s32 *)(p + 12);
    return 0;
}

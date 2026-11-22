#include <harness_catalog.h>
#include <string.h>

// 0x805E66E0 / 0x805E66E4 are cfglib_cfginfo / cfglib_nelem in the JP map.
// name-guess: CriCfgInfo is the 16-byte key/value record CRICFG_Read walks.
typedef struct {
    char key[12];
    s32 value;
} CriCfgInfo;

CriCfgInfo *cfglib_cfginfo;
s32 cfglib_nelem;

/* Search the 16-byte config entries; returns the matching entry or NULL. */
static inline CriCfgInfo *cfg_find_key(int count, const char *key, CriCfgInfo *p) {
    int i;
    for (i = 0; i < count; i++, p++) {
        if (strncmp(p->key, key, 12) == 0)
            return p;
    }
    return NULL;
}

int CRICFG_Read(const char *key, int *value) {
    int count;
    CriCfgInfo *p;

    p = cfglib_cfginfo;
    if (p == NULL)
        return -1;

    if (*key == '\0') {
        p = NULL;
    } else {
        count = cfglib_nelem;
        p = cfg_find_key(count, key, p);
    }
    if (p == NULL)
        return -3;

    *value = p->value;
    return 0;
}

#include <revolution/OS.h>

// unused in Xenoblade retail: OSNotifyLink, OSNotifyUnlink, OSNotifyPreLink,
// OSNotifyPostLink, OSSetStringTable, Relocate, Link, OSLink, OSLinkFixed,
// Undo, OSUnlink, OSSearchModule

void __OSModuleInit(void) {
    OS_FIRST_REL = OS_LAST_REL = NULL;
    OS_REL_NAME_TABLE = NULL;
}

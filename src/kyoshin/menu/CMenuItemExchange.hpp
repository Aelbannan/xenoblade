#pragma once

#include <types.h>

/**
 * Forward declaration for the OC thunk interface embedded at offset 0x6c
 * within CMenuItemExchange. Thunks such as func_801BF75C / func_801BF764
 * receive a CMenuItemExchangeOC* (the OC sub-object pointer) and adjust
 * back to the containing CMenuItemExchange before delegating to the real
 * implementation.
 */
struct CMenuItemExchangeOC;

/**
 * Menu item exchange / equipment screen controller.
 *
 * Full class layout TBD - members will be filled in during decomp.
 */
class CMenuItemExchange {
    // ...
};

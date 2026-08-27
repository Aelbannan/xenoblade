#pragma once

#include <types.h>

namespace mpfsys {

// Unofficial name
class MPFDrawDisplayList {
public:
    static MPFDrawDisplayList* getInstance();

    // Empty stubs (address-based names - semantics unknown)
    void updateSlotChain();
    void collectActiveSlots();
    void allocateDrawSlot();
    void createSlotFromPool();
    void func_804753B4();
    void func_80475C78();
    void func_80475E64();
    void func_80476104();
    void func_80476344();
    void func_80476E50();
    void func_80477F80();
    void func_804782C4();
    void func_804783D0();
    void checkProbeRange();
    void func_80478C94();
    void setPlainDrawNode();
    void setColorDrawNode();
    void func_804795BC();
    void drawColoredList();
    void destroy();
    void deallocate();
};

} // namespace mpfsys

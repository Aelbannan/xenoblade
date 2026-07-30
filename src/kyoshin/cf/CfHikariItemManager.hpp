#pragma once

#include <types.h>

namespace cf {
class CfHikariItemManager {
public:
    virtual ~CfHikariItemManager();
    void cbRenderBefore();
};
} // namespace cf

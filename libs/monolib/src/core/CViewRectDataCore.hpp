#pragma once

#include <types.h>
#include "monolib/core/CViewRectData.hpp"

class CViewRectDataCore {
public:
    void updateScrollForSize();

    // TODO: add fields
    void initViewRect();
    void setBoundsSize(const ml::CPnt16& maxSize);
};


#pragma once

#include <types.h>
#include "libs/monolib/src/scn/CVirtualLightObj.hpp"

class __declspec(novtable) CVirtualLightAmb : public CVirtualLightObj {
public:
    CVirtualLightAmb();
    virtual ~CVirtualLightAmb();
};

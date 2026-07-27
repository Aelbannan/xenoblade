#pragma once

#include <types.h>

class CSLCur {
public:
    CSLCur();
    void func_8028EA74();

    // TODO: add fields
};

class CSaveLoad {
public:
    CSaveLoad();
    virtual ~CSaveLoad();
    void func_8028F23C();
    void OnFileEvent();

    // TODO: add fields
};

namespace cf {

class CfAward {
public:
    virtual ~CfAward();
    void func_80291B18();

    // TODO: add fields
};
} // namespace cf


#pragma once

#include <types.h>
namespace cf {

class CHelp_EtherMakeTIPS {
public:
    void func_80295D8C();

    // TODO: add fields
};
} // namespace cf
namespace cf {

class CHelp_KizunaTIPS {
public:
    void func_80295D9C();

    // TODO: add fields
};
} // namespace cf
namespace cf {

class CHelp_AlwaysTIPS {
public:
    void func_80295DAC();

    // TODO: add fields
};
} // namespace cf


namespace cf {
class CHelpManager {
public:
    CHelpManager();
    virtual ~CHelpManager();
};
} // namespace cf

namespace cf {
class CHelp_TalkTIPS { public: void func_80295D98(); };
class CHelp_EndEventTIPS { public: void func_80295DA0(); };
} // namespace cf

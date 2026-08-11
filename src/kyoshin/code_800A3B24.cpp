// Auto-scaffolded catalog TU for kyoshin/code_800A3B24
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

namespace cf {
class CfDebugDrawManager {
public:
    void renderSphere();
    void renderCylinder();
    void renderCapsule();
    // TODO: add fields
};
} // namespace cf

void func_800A3B24(){}

void func_800A3C48(){}

void func_800A3D34(){}

void func_800A3DF8(){}

void func_800A3EF4(){}

void func_800A3F8C(){}

void func_800A4050(){}

void func_800A40E8(){}

void func_800A41BC(){}

void func_800A44CC(){}

void func_800A47C8(){}

void func_800A49E4(){}

void func_800A4B5C(){}

void func_800A4C48(){}

void func_800A5038(){}

void func_800A50AC(){}

void func_800A5488(){}

void func_800A5738(){}

void func_800A5B18(){}

void func_800A5F54(){}

void func_800A5FE8(){}

void cf::CfDebugDrawManager::renderSphere() {}

extern "C" void renderCylinder__Q22cf18CfDebugDrawManagerFb(cf::CfDebugDrawManager* self, const void* vec, const void* arg3, int mode);

void cf::CfDebugDrawManager::renderCylinder() {
    const void* vec;   // forwarded dangling r4 (retail: li r6,0; b Fb)
    const void* arg3;  // forwarded dangling r5
    renderCylinder__Q22cf18CfDebugDrawManagerFb(this, vec, arg3, 0);
}

void func_800A7094(){}

void func_800A72E0(){}

#pragma push
#pragma auto_inline off
extern "C" void renderCylinder__Q22cf18CfDebugDrawManagerFb(cf::CfDebugDrawManager* self, const void* vec, const void* arg3, int mode) {}
#pragma pop

void cf::CfDebugDrawManager::renderCapsule() {
    const void* vec;   // forwarded dangling r4 (retail: li r6,1; b Fb)
    const void* arg3;  // forwarded dangling r5
    renderCylinder__Q22cf18CfDebugDrawManagerFb(this, vec, arg3, 1);
}

#pragma once

#include <types.h>
#include <monolib/scn/CScnItemCamera.hpp>

class CScnItemCameraNw4r : public CScnItemCamera {
public:
    virtual ~CScnItemCameraNw4r();
};

void ScnCamNw4r_RefreshFrustumA(CScnItemCameraNw4r* self);
void ScnCamNw4r_RefreshFrustumB(CScnItemCameraNw4r* self);
void ScnCamNw4r_EmptyHook48();
void* ScnCamNw4r_GetField10Ptr(u8* self);

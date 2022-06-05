#pragma once

#include <stddef.h>
#include <stdint.h>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include "mrdr_math.h"

namespace mrdr {

class ICamera;
class IScene;

class IContext {
public:
    // Camera Manipulation
    virtual ICamera* CreateCamera() = 0;
    virtual void DestroyCamera(ICamera* camera) = 0;

    // Scene Manipulation
    virtual IScene* CreateScene() = 0;
    virtual void DestroyScene(IScene* scene) = 0;
};

class ICamera {
public:
    virtual void SetDepthRange(float znear, float zfar) = 0;
    virtual void SetOrthographicsProjection(float width, float height) = 0;
    virtual void SetPerspectiveProjection(float fovy) = 0;

    virtual void SetPosition(vec3& pos) = 0;
    virtual void SetOrientation(quat& rot) = 0;

    virtual void SetClearColor(vec4& color) = 0;

    virtual void RenderScene(IScene* scene) = 0;
};

class IScene {
public:
    virtual void DrawLine(const vec3& pt0, const vec3& pt1, const vec4& color) = 0;
};

}

// You should implement the functions below
extern size_t mrdr_userdata_size();
extern void mrdr_initialize(mrdr::IContext* ctx, void* udmem);
extern void mrdr_update(mrdr::IContext* ctx, void* ud);
extern void mrdr_shutdown(mrdr::IContext* ctx, void* ud);

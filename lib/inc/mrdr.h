#pragma once

#include <stddef.h>
#include <stdint.h>
#include <spdlog/spdlog.h>
#include <imgui.h>

namespace mrdr {


class IObject;
class IBuffer;
class ITexture;
class IMaterial;
class IMesh;
class IEntity;
class ICamera;
class IScene;
class IContext;

struct BufferCreateInfo;
struct TextureCreateInfo;
struct MaterialCreateInfo;
struct MeshCreateInfo;
struct EntityCreateInfo;
struct SceneCreateInfo;
struct CameraCreateInfo;

extern void mrdr_main(IContext* ctx);

class IContext {
public:
    virtual ~IContext() = 0;

    // Create Objects
    virtual IBuffer* CreateBuffer(const BufferCreateInfo* info) = 0;
    virtual ITexture* CreateTexture(const TextureCreateInfo* info) = 0;
    virtual IMaterial* CreateMaterial(const MaterialCreateInfo* info) = 0;
    virtual IMesh* CreateMesh(const MeshCreateInfo* info) = 0;
    virtual IEntity* CreateEntity(const EntityCreateInfo* info) = 0;
    virtual IScene* CreateScene(const SceneCreateInfo* info) = 0;
    virtual ICamera* CreateCamera(const CameraCreateInfo* info) = 0;

    // Main Loop
    virtual void ProcessEvents() = 0;
    virtual void BeginUpdate(IScene* scene) = 0;
    virtual void EndUpdate() = 0;
    virtual void Render(ICamera* camera) = 0;
};

class IObject {
public:
    virtual void Retain() = 0;
    virtual void Release() = 0;
};

class IBuffer : IObject {
public:
    virtual void UploadData(size_t dstOffset, const void* srcData, size_t size) = 0;
};

class ITexture : IObject {
public:
    virtual void UploadPixels(uint8_t level, const void* data, size_t size) = 0;
};

class IMaterial : IObject {
};

class IMesh : IObject {
};

class IEntity : IObject {
};

class IScene : IObject {
public:
    // TODO: add entity
};

class ICamera : IObject {
public:
    // TODO: lookat, projection
};

}

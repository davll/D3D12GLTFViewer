#pragma once

#include <stddef.h>
#include <stdint.h>
#include <spdlog/spdlog.h>
#include <imgui.h>

namespace mrdr {

class IObject;
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

class IObject {
public:
    virtual void AddRef() = 0;
    virtual void Release() = 0;
};

class IContext {
public:
    // Create Objects
    // virtual ITexture* CreateTexture(const TextureCreateInfo* info) = 0;
    // virtual IMaterial* CreateMaterial(const MaterialCreateInfo* info) = 0;
    // virtual IMesh* CreateMesh(const MeshCreateInfo* info) = 0;
    // virtual IEntity* CreateEntity(const EntityCreateInfo* info) = 0;
    // virtual IScene* CreateScene(const SceneCreateInfo* info) = 0;
    // virtual ICamera* CreateCamera(const CameraCreateInfo* info) = 0;

    // Main Loop Control
    virtual void Quit() = 0;
};

class ITexture : public IObject {
public:
    virtual void UploadPixels(uint8_t level, const void* data, size_t size) = 0;
};

class IMaterial : public IObject {
};

class IMesh : public IObject {
};

class IEntity : public IObject {
};

class IScene : IObject {
public:
    // TODO: add entity
};

class ICamera : public IObject {
public:
    // TODO: lookat, projection
};

}

// You should implement the functions below
extern size_t mrdr_userdata_size();
extern void mrdr_initialize(mrdr::IContext* ctx, void* udmem);
extern void mrdr_update(mrdr::IContext* ctx, void* ud);
extern void mrdr_shutdown(mrdr::IContext* ctx, void* ud);

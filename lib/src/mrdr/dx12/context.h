#pragma once

#include <mrdr.h>
#include "common.h"
#include <SDL.h>

namespace mrdr {
namespace dx12 {

class Device;
class CommandFence;
class CommandPool;
class CommandQueue;
class GpuDescriptorHeap;
class SwapChain;

class Context : public IContext {
public:
    Context(SDL_Window* window);
    ~Context();

    //
    // IContext
    //
    virtual ICamera* CreateCamera() override;
    virtual void DestroyCamera(ICamera* camera) override;
    virtual IScene* CreateScene() override;
    virtual void DestroyScene(IScene* scene) override;

    //
    // Internal
    //
    bool ProcessEvent(const SDL_Event& event);
    bool BeginFrame();
    void EndFrame();

private:
    static IDXGIFactory6* CreateFactory();
    static IDXGIAdapter4* ChooseAdapter(IDXGIFactory6* factory);

private:
    SDL_Window* m_Window;
    Device* m_Device;
    SwapChain* m_SwapChain;
    GpuDescriptorHeap* m_ResourceDescriptorHeap;
    GpuDescriptorHeap* m_SamplerDescriptorHeap;

    //
    // Current Frame
    //
    ID3D12Resource* m_FinalTexture;
    D3D12_CPU_DESCRIPTOR_HANDLE m_FinalRenderTargetView;
    ID3D12GraphicsCommandList* m_CommandList;
};

}
}

#pragma once

#include "common.h"

namespace mrdr {

class CommandPool;
class CommandQueue;
class ShaderDescriptorHeap;
class SwapChain;

class Renderer {
public:
    struct CreateInfo {
        UINT NumFrames;
        SDL_Window* Window;
    };

    explicit Renderer(const CreateInfo& info);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void StartFrame();

    // TODO: SetCamera
    // TODO: DrawScene
    // TODO: DrawText
    // TODO: DrawBox

    void EndFrame();

    void ProcessEvent(const SDL_Event* event);

private:
    static IDXGIFactory6* CreateFactory();
    static IDXGIAdapter4* ChooseAdapter(IDXGIFactory6* factory);
    void InitDevice(IDXGIAdapter* adapter);
    void InitCommandQueue();

private:
    enum class StaticCbvSrvUav : UINT {
        IMGUI_FONT_SRV = 0,
    };

    const UINT m_NumFrames;
    UINT m_FrameIdx;
    ID3D12Device* m_Device;
    CommandQueue* m_CommandQueue;
    std::vector<std::unique_ptr<CommandPool>> m_CommandPools;
    ShaderDescriptorHeap* m_CbvSrvUavHeap;
    ShaderDescriptorHeap* m_SamplerHeap;
    SwapChain* m_SwapChain;
};

} // namespace mrdr

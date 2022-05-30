#pragma once

#include "common.h"

namespace mrdr {

class CommandPool;
class CommandQueue;
class ShaderDescriptorHeap;
struct StaticDescriptors;
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
    void InitDescriptors();
    void InitCommandPools();
    void InitImGui(SDL_Window* window);

private:
    const UINT m_NumFrames;
    UINT m_FrameIdx;
    ID3D12Device* m_Device;
    CommandQueue* m_CommandQueue;
    SwapChain* m_SwapChain;
    ShaderDescriptorHeap* m_ResourceDescriptorHeap;
    ShaderDescriptorHeap* m_SamplerDescriptorHeap;
    StaticDescriptors* m_StaticDescriptors;
    std::vector<std::unique_ptr<CommandPool>> m_CommandPools;
};

} // namespace mrdr

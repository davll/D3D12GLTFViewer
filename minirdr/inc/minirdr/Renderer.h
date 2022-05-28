#pragma once

#include "common.h"

namespace minirdr {

class CommandManager;
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

    void Resize();

private:
    static IDXGIFactory6* CreateFactory();
    static IDXGIAdapter4* ChooseAdapter(IDXGIFactory6* factory);
    void InitDevice(IDXGIAdapter* adapter);

private:
    enum class StaticCbvSrvUav : UINT {
        IMGUI_FONT_SRV = 0,
    };

    const UINT m_NumFrames;
    UINT m_FrameIdx;
    ID3D12Device* m_Device;
    CommandManager* m_CommandManager;
    ShaderDescriptorHeap* m_CbvSrvUavHeap;
    ShaderDescriptorHeap* m_SamplerHeap;
    SwapChain* m_SwapChain;
};

} // namespace minirdr
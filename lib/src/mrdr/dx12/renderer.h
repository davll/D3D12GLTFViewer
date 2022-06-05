#pragma once

#include "common.h"
#include "device.h"
#include "gpu_descriptor_heap.h"
#include "swap_chain.h"

struct SDL_Window;

namespace mrdr {

class Renderer {
public:
    Renderer(SDL_Window* window);
    ~Renderer();

    void BeginFrame();
    void EndFrame();
    void Resize();

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

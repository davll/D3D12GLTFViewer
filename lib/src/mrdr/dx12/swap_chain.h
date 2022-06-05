#pragma once

#include "common.h"
#include "command_queue.h"
#include "command_fence.h"

struct SDL_Window;

namespace mrdr {
namespace dx12 {

class SwapChain {
public:
    struct CreateInfo {
        UINT NumFrames;
        IDXGIFactory6* Factory;
        CommandQueue* CommandQueue;
        SDL_Window* Window;
    };

    explicit SwapChain(const CreateInfo& info);
    ~SwapChain();

    inline IDXGISwapChain3* GetSwapChain() const;
    inline ID3D12Resource* GetBackBuffer() const;
    inline D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const;

    void WaitAll();
    void Present();
    void Resize();

private:
    void Init(const CreateInfo& info);
    void InitDescriptorHeap();
    void WriteDescriptors();
    void SetupBuffers();
    void ClearBuffers();
    void Wait(UINT64 workId);

private:
    const UINT m_NumFrames;
    ID3D12Device* m_Device;
    CommandQueue* m_CommandQueue;
    IDXGISwapChain3* m_SwapChain;

    CommandFence* m_Fence;
    std::vector<UINT64> m_WorkIds;

    std::vector<ID3D12Resource*> m_BackBuffers;
    ID3D12DescriptorHeap* m_RenderTargetViewHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_RenderTargetViewBase;
    UINT m_RenderTargetViewDescriptorSize;
};

inline IDXGISwapChain3* SwapChain::GetSwapChain() const
{
    return m_SwapChain;
}

inline ID3D12Resource* SwapChain::GetBackBuffer() const
{
    const UINT frameIdx = m_SwapChain->GetCurrentBackBufferIndex();
    return m_BackBuffers[frameIdx];
}

inline D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::GetRenderTargetView() const
{
    const UINT frameIdx = m_SwapChain->GetCurrentBackBufferIndex();
    D3D12_CPU_DESCRIPTOR_HANDLE handle = m_RenderTargetViewBase;
    handle.ptr += m_RenderTargetViewDescriptorSize * frameIdx;
    return handle;
}

}
}

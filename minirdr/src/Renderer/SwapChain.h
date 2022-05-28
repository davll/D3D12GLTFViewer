#pragma once

#include "common.h"

namespace minirdr {

class SwapChain {
public:
    struct CreateInfo {
        UINT NumFrames;
        IDXGIFactory6* Factory;
        ID3D12Device* Device;
        ID3D12CommandQueue* CommandQueue;
        SDL_Window* Window;
    };

    explicit SwapChain(const CreateInfo& info);
    ~SwapChain();

    inline IDXGISwapChain3* GetSwapChain() const;
    inline ID3D12Resource* GetBackBuffer() const;
    inline D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const;
    inline void WaitAll();

    void Present();
    void Resize();

private:
    void Init(const CreateInfo& info);
    void InitDescriptorHeap();
    void WriteDescriptors();
    void SetupBuffers();
    void ClearBuffers();
    void Wait(UINT64 frameId);

private:
    const UINT m_NumFrames;
    ID3D12Device* m_Device;
    ID3D12CommandQueue* m_CommandQueue;

    IDXGISwapChain3* m_SwapChain;

    UINT64 m_FrameCount;
    std::vector<UINT64> m_FrameIds;
    ID3D12Fence* m_Fence;
    HANDLE m_FenceEvent;

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

inline void SwapChain::WaitAll()
{
    Wait(m_FrameCount);
}

}

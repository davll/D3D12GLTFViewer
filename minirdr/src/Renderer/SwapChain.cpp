#include "SwapChain.h"
#include <SDL_syswm.h>

namespace minirdr {

SwapChain::SwapChain(const CreateInfo& info)
: m_NumFrames(info.NumFrames)
, m_FrameCount(0)
, m_FrameIds((size_t)info.NumFrames, 0)
, m_Device(info.Device)
, m_CommandQueue(info.CommandQueue)
, m_BackBuffers((size_t)info.NumFrames)
{
    m_Device->AddRef();
    m_CommandQueue->AddRef();
    Init(info);
}

SwapChain::~SwapChain()
{
    WaitAll();

    m_Fence->Release();
    CloseHandle(m_FenceEvent);

    m_RenderTargetViewHeap->Release();
    ClearBuffers();
    m_SwapChain->Release();
    m_CommandQueue->Release();
    m_Device->Release();
}

void SwapChain::Present()
{
    UINT64 frameId = ++m_FrameCount;
    m_FrameIds[m_SwapChain->GetCurrentBackBufferIndex()] = frameId;

    MINIRDR_CHKHR(
        m_SwapChain->Present(1, 0)
    );

    MINIRDR_CHKHR(
        m_CommandQueue->Signal(m_Fence, frameId)
    );

    frameId = m_FrameIds[m_SwapChain->GetCurrentBackBufferIndex()];
    Wait(frameId);
}

void SwapChain::Resize()
{
    WaitAll();
    ClearBuffers();
    m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
    SetupBuffers();
    WriteDescriptors();
}

void SwapChain::Wait(UINT64 frameId)
{
    if (m_Fence->GetCompletedValue() < frameId) {
        MINIRDR_CHKHR(
            m_Fence->SetEventOnCompletion(frameId, m_FenceEvent)
        );
        WaitForSingleObject(m_FenceEvent, INFINITE);
    }
}

void SwapChain::Init(const CreateInfo& info)
{
    int width, height;
    SDL_GetWindowSize(info.Window, &width, &height);

    SDL_SysWMinfo wminfo;
    SDL_VERSION(&wminfo.version);
    if (SDL_GetWindowWMInfo(info.Window, &wminfo) == 0) {
        MINIRDR_FAIL("Unable to get SDL_WMInfo");
    }
    HWND hwnd = wminfo.info.win.window;

    DXGI_SWAP_CHAIN_DESC1 desc;
    desc.Width = width;
    desc.Height = height;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Stereo = FALSE;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = m_NumFrames;
    desc.Scaling = DXGI_SCALING_NONE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    desc.Flags = 0;

    IDXGISwapChain1* swapChain1;
    MINIRDR_CHKHR(
        info.Factory->CreateSwapChainForHwnd(info.CommandQueue, hwnd, &desc, NULL, NULL, &swapChain1)
    );

    IDXGISwapChain3* swapChain3;
    MINIRDR_CHKHR(
        swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain3))
    );
    swapChain1->Release();
    m_SwapChain = swapChain3;

    MINIRDR_CHKHR(
        info.Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))
    );

    m_FenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (!m_FenceEvent) {
        abort();
    }

    InitDescriptorHeap();
    SetupBuffers();
    WriteDescriptors();
}

void SwapChain::InitDescriptorHeap()
{
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = m_NumFrames;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 0;

        MINIRDR_CHKHR(
            m_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_RenderTargetViewHeap))
        );
    }

    m_RenderTargetViewDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_RenderTargetViewBase = m_RenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
}

void SwapChain::WriteDescriptors()
{
    D3D12_RENDER_TARGET_VIEW_DESC desc;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice = 0;
    desc.Texture2D.PlaneSlice = 0;

    D3D12_CPU_DESCRIPTOR_HANDLE handle = m_RenderTargetViewBase;
    UINT size = m_RenderTargetViewDescriptorSize;
    for (UINT i = 0; i < m_NumFrames; ++i, handle.ptr += size) {
        m_Device->CreateRenderTargetView(m_BackBuffers[i], &desc, handle);
    }
}

void SwapChain::SetupBuffers()
{
    for (UINT i = 0; i < m_NumFrames; ++i) {
        ID3D12Resource* buffer;
        MINIRDR_CHKHR(
            m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&buffer))
        );
        m_BackBuffers[i] = buffer;
    }
}

void SwapChain::ClearBuffers()
{
    for (UINT i = 0; i < m_NumFrames; ++i) {
        m_BackBuffers[i]->Release();
        m_BackBuffers[i] = NULL;
    }
}

}

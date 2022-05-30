#include "Renderer.h"
#include "Renderer/CommandPool.h"
#include "Renderer/CommandQueue.h"
#include "Renderer/ShaderDescriptorHeap.h"
#include "Renderer/StaticDescriptors.h"
#include "Renderer/SwapChain.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_dx12.h"

namespace mrdr {

Renderer::Renderer(const CreateInfo& info)
: m_NumFrames(info.NumFrames)
, m_FrameIdx(0)
{
    IDXGIFactory6* factory = CreateFactory();
    IDXGIAdapter4* adapter = ChooseAdapter(factory);
    InitDevice(adapter);
    adapter->Release();
    InitCommandQueue();
    {
        SwapChain::CreateInfo cinfo;
        cinfo.NumFrames = info.NumFrames;
        cinfo.Device = m_Device;
        cinfo.CommandQueue = m_CommandQueue;
        cinfo.Factory = factory;
        cinfo.Window = info.Window;
        m_SwapChain = new SwapChain(cinfo);
    }
    factory->Release();
    InitDescriptors();
    InitCommandPools();
    InitImGui(info.Window);
}

Renderer::~Renderer()
{
    // Wait for GPU
    m_SwapChain->WaitAll();

    // ImGui
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Cleanup
    m_CommandPools.clear();
    delete m_StaticDescriptors;
    delete m_ResourceDescriptorHeap;
    delete m_SamplerDescriptorHeap;
    delete m_SwapChain;
    delete m_CommandQueue;
    m_Device->Release();
}

void Renderer::StartFrame()
{
    // ImGui
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void Renderer::EndFrame()
{
    const UINT frameIdx = m_FrameIdx;

    auto& commandPool = m_CommandPools[m_FrameIdx];
    commandPool->Reset();
    ID3D12CommandAllocator* commandAllocator = commandPool->NewCommandAllocator();
    ID3D12GraphicsCommandList* commandList = commandPool->NewCommandList(commandAllocator);

    {
        ID3D12DescriptorHeap* dheaps[2] = {
            m_ResourceDescriptorHeap->GetDescriptorHeap(),
            m_SamplerDescriptorHeap->GetDescriptorHeap(),
        };
        commandList->SetDescriptorHeaps(2, dheaps);
    }

    ID3D12Resource* backbufTexture = m_SwapChain->GetBackBuffer();
    D3D12_CPU_DESCRIPTOR_HANDLE backbufrRTV = m_SwapChain->GetRenderTargetView();

    {
        D3D12_RESOURCE_BARRIER rbdesc;
        rbdesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        rbdesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        rbdesc.Transition.pResource = backbufTexture;
        rbdesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        rbdesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
        rbdesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        commandList->ResourceBarrier(1, &rbdesc);
    }

    {
        FLOAT clearColor[4] = { 1.0f, 0.5f, 0.5f, 1.0f };
        commandList->ClearRenderTargetView(backbufrRTV, clearColor, 0, NULL);
    }

    {
        ImGui::Render();
        commandList->OMSetRenderTargets(1, &backbufrRTV, FALSE, NULL);
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
    }

    {
        D3D12_RESOURCE_BARRIER rbdesc;
        rbdesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        rbdesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        rbdesc.Transition.pResource = backbufTexture;
        rbdesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        rbdesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        rbdesc.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
        commandList->ResourceBarrier(1, &rbdesc);
    }

    MRDR_CHKHR(
        commandList->Close()
    );

    m_CommandQueue->SubmitCommands(1, (ID3D12CommandList**)&commandList);

    m_SwapChain->Present();

    m_FrameIdx = (m_FrameIdx + 1) % m_NumFrames;
}

void Renderer::InitDevice(IDXGIAdapter* adapter)
{
    HRESULT hr;

    hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));
    if (FAILED(hr)) {
        MRDR_FAIL("Failed to create D3D12 device");
    }

#if defined(_DEBUG) && 0
    ID3D12InfoQueue* infoQueue;
    hr = m_Device->QueryInterface(IID_PPV_ARGS(&infoQueue));
    if (FAILED(hr)) {
        MRDR_FAIL("Failed to get info queue");
    }
    infoQueue->Release();
#endif
}

void Renderer::InitCommandQueue()
{
    CommandQueue::CreateInfo info;
    info.Type = CommandQueue::Type::GRAPHICS;
    info.Device = m_Device;

    m_CommandQueue = new CommandQueue(info);
}

void Renderer::InitDescriptors()
{
    ShaderDescriptorHeap::CreateInfo info;
    info.Device = m_Device;

    info.Type = ShaderDescriptorHeap::Type::RESOURCE;
    info.MaxNumStaticDescriptors = (UINT)StaticResourceDescriptorIds::_COUNT;
    info.MaxNumDynamicDescriptors = 4096;
    m_ResourceDescriptorHeap = new ShaderDescriptorHeap(info);

    info.Type = ShaderDescriptorHeap::Type::SAMPLER;
    info.MaxNumStaticDescriptors = (UINT)StaticSamplerDescriptorIds::_COUNT;
    info.MaxNumDynamicDescriptors = 512;
    m_SamplerDescriptorHeap = new ShaderDescriptorHeap(info);

    m_StaticDescriptors = new StaticDescriptors();
    if (m_ResourceDescriptorHeap->GetMaxNumStaticDescriptors() > 0) {
        UINT size = m_ResourceDescriptorHeap->GetDescriptorSize();
        ShaderDescriptorHandle handle = m_ResourceDescriptorHeap->GetBaseStaticDescriptorHandle();
        for (UINT i = 0; i < (UINT)StaticResourceDescriptorIds::_COUNT; ++i, handle = handle + size) {
            m_StaticDescriptors->Resources.Handles[i] = handle;
        }
    }
    if (m_SamplerDescriptorHeap->GetMaxNumStaticDescriptors() > 0) {
        UINT size = m_SamplerDescriptorHeap->GetDescriptorSize();
        ShaderDescriptorHandle handle = m_SamplerDescriptorHeap->GetBaseStaticDescriptorHandle();
        for (UINT i = 0; i < (UINT)StaticSamplerDescriptorIds::_COUNT; ++i, handle = handle + size) {
            m_StaticDescriptors->Samplers.Handles[i] = handle;
        }
    }
}

void Renderer::InitCommandPools()
{
    m_CommandPools.reserve(m_NumFrames);
    for (UINT i = 0; i < m_NumFrames; ++i) {
        CommandPool::CreateInfo info;
        info.Type = CommandPool::Type::GRAPHICS;
        info.Device = m_Device;
        m_CommandPools.emplace_back(std::make_unique<CommandPool>(info));
    }
}

void Renderer::InitImGui(SDL_Window* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForD3D(window);
    ImGui_ImplDX12_Init(
        m_Device,
        m_NumFrames,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        m_ResourceDescriptorHeap->GetDescriptorHeap(),
        m_StaticDescriptors->Resources.ImGuiFontSRV.Cpu,
        m_StaticDescriptors->Resources.ImGuiFontSRV.Gpu
    );
}

IDXGIFactory6* Renderer::CreateFactory()
{
    HRESULT hr;
#if defined(_DEBUG)
    UINT flags = 0;
    ID3D12Debug* debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
        flags |= DXGI_CREATE_FACTORY_DEBUG;
    }
    debugController->Release();
#else
    UINT flags = 0;
#endif

    IDXGIFactory6* factory;
    hr = CreateDXGIFactory2(flags, IID_PPV_ARGS(&factory));
    if (FAILED(hr)) {
        MRDR_FAIL("Failed to create DXGIFactory");
    }

    return factory;
}

IDXGIAdapter4* Renderer::ChooseAdapter(IDXGIFactory6* factory)
{
    HRESULT hr;
    IDXGIAdapter4* adapter;
    for (UINT adapterIdx = 0; true; ++adapterIdx) {
        hr = factory->EnumAdapterByGpuPreference(
            adapterIdx,
            DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
            IID_PPV_ARGS(&adapter));

        if (SUCCEEDED(hr)) {
            DXGI_ADAPTER_DESC3 desc;
            hr = adapter->GetDesc3(&desc);
            if (FAILED(hr)) {
                MRDR_FAIL("Failed to get adapter desc");
            }

            if (desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
                continue;

            hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), NULL);            
            if (SUCCEEDED(hr)) {
                break;
            }
        } else {
            MRDR_FAIL("Failed to enumerate adapter");
        }
    }
    return adapter;
}

void Renderer::ProcessEvent(const SDL_Event* event)
{
    ImGui_ImplSDL2_ProcessEvent(event);
    switch (event->type) {
    case SDL_WINDOWEVENT:
        switch (event->window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            m_SwapChain->Resize();
            break;
        }
        break;
    default:
        break;
    }
}

}

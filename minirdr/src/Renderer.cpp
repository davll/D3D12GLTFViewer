#include "Renderer.h"
#include "Renderer/CommandManager.h"
#include "Renderer/ShaderDescriptorHeap.h"
#include "Renderer/SwapChain.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_dx12.h"

namespace minirdr {

Renderer::Renderer(const CreateInfo& info)
: m_NumFrames(info.NumFrames)
, m_FrameIdx(0)
{
    IDXGIFactory6* factory = CreateFactory();
    IDXGIAdapter4* adapter = ChooseAdapter(factory);
    InitDevice(adapter);
    adapter->Release();
    {
        ShaderDescriptorHeap::CreateInfo info;
        info.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        info.Device = m_Device;
        info.MaxNumStaticDescriptors = 16;
        info.MaxNumDynamicDescriptors = 4096;
        m_CbvSrvUavHeap = new ShaderDescriptorHeap(info);
    }
    {
        ShaderDescriptorHeap::CreateInfo info;
        info.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        info.Device = m_Device;
        info.MaxNumStaticDescriptors = 4;
        info.MaxNumDynamicDescriptors = 512;
        m_SamplerHeap = new ShaderDescriptorHeap(info);
    }
    {
        m_CommandManager = new CommandManager(m_Device, info.NumFrames);
    }
    {
        SwapChain::CreateInfo cinfo;
        cinfo.NumFrames = info.NumFrames;
        cinfo.Device = m_Device;
        cinfo.CommandQueue = m_CommandManager->GetCommandQueue(CommandType::GRAPHICS);
        cinfo.Factory = factory;
        cinfo.Window = info.Window;
        m_SwapChain = new SwapChain(cinfo);
    }
    factory->Release();

    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForD3D(info.Window);
        ImGui_ImplDX12_Init(
            m_Device,
            info.NumFrames,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            m_CbvSrvUavHeap->GetDescriptorHeap(),
            m_CbvSrvUavHeap->GetStaticDescriptorCpuHandle((UINT)StaticCbvSrvUav::IMGUI_FONT_SRV),
            m_CbvSrvUavHeap->GetStaticDescriptorGpuHandle((UINT)StaticCbvSrvUav::IMGUI_FONT_SRV));
    }
}

Renderer::~Renderer()
{
    m_SwapChain->WaitAll();

    ImGui_ImplDX12_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    delete m_CommandManager;
    delete m_CbvSrvUavHeap;
    delete m_SamplerHeap;
    delete m_SwapChain;
    m_Device->Release();
}

void Renderer::StartFrame()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void Renderer::EndFrame()
{
    const UINT frameIdx = m_FrameIdx;

    m_CommandManager->ResetCommands(frameIdx);
    ID3D12CommandAllocator* commandAllocator = m_CommandManager->NewCommandAllocator(frameIdx, CommandType::GRAPHICS);
    ID3D12GraphicsCommandList* commandList = m_CommandManager->NewCommandList(frameIdx, CommandType::GRAPHICS, commandAllocator);

    {
        ID3D12DescriptorHeap* dheaps[2] = {
            m_CbvSrvUavHeap->GetDescriptorHeap(),
            m_SamplerHeap->GetDescriptorHeap(),
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

    MINIRDR_CHKHR(
        commandList->Close()
    );

    Workload workload = { 1, (ID3D12CommandList**)&commandList };
    m_CommandManager->SubmitCommands(frameIdx, CommandType::GRAPHICS, 1, &workload);

    m_SwapChain->Present();

    m_FrameIdx = (m_FrameIdx + 1) % m_NumFrames;
}

void Renderer::Resize()
{
    m_SwapChain->Resize();
}

void Renderer::InitDevice(IDXGIAdapter* adapter)
{
    HRESULT hr;
    ID3D12Device4* device;
    hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
    if (FAILED(hr)) {
        MINIRDR_FAIL("Failed to create D3D12 device");
    }

#if defined(_DEBUG) && 0
    ID3D12InfoQueue* infoQueue;
    hr = device->QueryInterface(IID_PPV_ARGS(&infoQueue));
    if (FAILED(hr)) {
        MINIRDR_FAIL("Failed to get info queue");
    }
    infoQueue->Release();
#endif

    m_Device = device;
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
        MINIRDR_FAIL("Failed to create DXGIFactory");
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
                MINIRDR_FAIL("Failed to get adapter desc");
            }

            if (desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
                continue;

            hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), NULL);            
            if (SUCCEEDED(hr)) {
                break;
            }
        } else {
            MINIRDR_FAIL("Failed to enumerate adapter");
        }
    }
    return adapter;
}

}
#include "context.h"
#include "device.h"
#include "gpu_descriptor_heap.h"
#include "swap_chain.h"
#include <assert.h>
#include <stdlib.h>
#include <spdlog/spdlog.h>
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_dx12.h>

namespace mrdr {
namespace dx12 {

Context::Context(SDL_Window* window)
: m_Window(window)
{
    IDXGIFactory6* factory = CreateFactory();
    IDXGIAdapter4* adapter = ChooseAdapter(factory);
    m_Device = new Device({
        2, adapter
    });
    adapter->Release();
    m_SwapChain = new SwapChain({
        5, factory, m_Device->GetCommandQueue(), window,
    });
    factory->Release();

    m_ResourceDescriptorHeap = new GpuDescriptorHeap({
        GpuDescriptorHeap::Type::RESOURCE,
        16, 1024,
        m_Device->GetDevice(),
    });

    m_SamplerDescriptorHeap = new GpuDescriptorHeap({
        GpuDescriptorHeap::Type::SAMPLER,
        0, 256,
        m_Device->GetDevice(),
    });

    {
        auto staticCpuSpan = m_ResourceDescriptorHeap->GetStaticCpuSpan();
        auto staticGpuSpan = m_ResourceDescriptorHeap->GetStaticGpuSpan();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForD3D(m_Window);
        ImGui_ImplDX12_Init(
            m_Device->GetDevice(),
            2,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            m_ResourceDescriptorHeap->GetDescriptorHeap(),
            staticCpuSpan[0],
            staticGpuSpan[0]
        );
    }
}

Context::~Context()
{
    m_SwapChain->WaitAll();

    ImGui_ImplDX12_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    delete m_ResourceDescriptorHeap;
    delete m_SamplerDescriptorHeap;
    delete m_SwapChain;
    delete m_Device;
}

//
// IContext Implementation
//

ICamera* Context::CreateCamera()
{
    return NULL;
}

void Context::DestroyCamera(ICamera* camera)
{
}

IScene* Context::CreateScene()
{
    return NULL;
}

void Context::DestroyScene(IScene* scene)
{
}

//
// Internal Implementation
//

bool Context::ProcessEvent(const SDL_Event& event)
{
    if (event.type == SDL_QUIT)
        return false;

    auto& io = ImGui::GetIO();
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (io.WantCaptureKeyboard) {
        switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        case SDL_TEXTEDITING:
        case SDL_TEXTINPUT:
            return true;
            break;
        }
    }
    if (io.WantCaptureMouse) {
        switch (event.type) {
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
            return true;
            break;
        }
    }

    switch (event.type) {
    case SDL_WINDOWEVENT:
        switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            m_SwapChain->Resize();
            break;
        }
        break;
    default:
        break;
    }
    return true;
}

bool Context::BeginFrame()
{
    m_CommandList = m_Device->BeginFrame();
    m_FinalTexture = m_SwapChain->GetBackBuffer();
    m_FinalRenderTargetView = m_SwapChain->GetRenderTargetView();

    {
        ID3D12DescriptorHeap* descriptorHeaps[2] = {
            m_ResourceDescriptorHeap->GetDescriptorHeap(),
            m_SamplerDescriptorHeap->GetDescriptorHeap(),
        };
        m_CommandList->SetDescriptorHeaps(2, descriptorHeaps);
    }

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    return true;
}

void Context::EndFrame()
{
    // TODO: Render Scene

    {
        D3D12_RESOURCE_BARRIER rbdesc;
        rbdesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        rbdesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        rbdesc.Transition.pResource = m_FinalTexture;
        rbdesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        rbdesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
        rbdesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        m_CommandList->ResourceBarrier(1, &rbdesc);
    }

    {
        FLOAT clearColor[4] = { 0.5f, 1.0f, 0.8f, 1.0f };
        m_CommandList->ClearRenderTargetView(m_FinalRenderTargetView, clearColor, 0, NULL);
    }

    {
        ImGui::Render();
        m_CommandList->OMSetRenderTargets(1, &m_FinalRenderTargetView, FALSE, NULL);
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_CommandList);
    }

    {
        D3D12_RESOURCE_BARRIER rbdesc;
        rbdesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        rbdesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        rbdesc.Transition.pResource = m_FinalTexture;
        rbdesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        rbdesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        rbdesc.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
        m_CommandList->ResourceBarrier(1, &rbdesc);
    }

    m_Device->EndFrame();
    m_SwapChain->Present();
}

IDXGIFactory6* Context::CreateFactory()
{
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
    MRDR_CHKHR(
        CreateDXGIFactory2(flags, IID_PPV_ARGS(&factory))
    );

    return factory;
}

IDXGIAdapter4* Context::ChooseAdapter(IDXGIFactory6* factory)
{
    IDXGIAdapter4* adapter;
    for (UINT adapterIdx = 0; true; ++adapterIdx) {
        MRDR_CHKHR(
            factory->EnumAdapterByGpuPreference(
                adapterIdx,
                DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                IID_PPV_ARGS(&adapter))
        );

        {
            DXGI_ADAPTER_DESC3 desc;
            MRDR_CHKHR(
                adapter->GetDesc3(&desc)
            );

            if (desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
                continue;

            HRESULT hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), NULL);
            if (SUCCEEDED(hr))
                break;
        }
    }
    return adapter;
}

}
}

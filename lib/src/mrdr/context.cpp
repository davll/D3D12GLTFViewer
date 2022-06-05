#include "context.h"
#include <assert.h>
#include <stdlib.h>
#include <spdlog/spdlog.h>
#include "imgui_impl_sdl.h"
#include "dx12/renderer.h"

namespace mrdr {

Context::Context()
: m_ShouldQuit(false)
{
    InitWindow();
    m_Renderer = new Renderer(m_Window);
}

Context::~Context()
{
    delete m_Renderer;
    SDL_DestroyWindow(m_Window);
}

//
// IContext Implementation
//

void Context::Quit()
{
    m_ShouldQuit.store(true, std::memory_order_relaxed);
}

//
// Internal Implementation
//

bool Context::ShouldQuit() const
{
    return m_ShouldQuit.load(std::memory_order_acquire);
}

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
            m_Renderer->Resize();
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
    m_Renderer->BeginFrame();

    return true;
}

void Context::EndFrame()
{
    m_Renderer->EndFrame();
}

//
//
//

void Context::InitWindow()
{
    SDL_Window* window = SDL_CreateWindow("D3D12 GLTF Viewer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_RESIZABLE);
    if (!window) {
        SPDLOG_ERROR("Unable to create SDL window: {}", SDL_GetError());
        abort();
    }
    m_Window = window;
}

}

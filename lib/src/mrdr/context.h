#pragma once

#include <mrdr.h>
#include <atomic>
#include <SDL.h>

namespace mrdr {
namespace dx12 { class Renderer; }

class Context : public IContext {
public:
    Context();
    ~Context();

    //
    // IContext
    //
    virtual void Quit() override;

    //
    // Internal
    //
    bool ShouldQuit() const;
    bool ProcessEvent(const SDL_Event& event);
    bool BeginFrame();
    void EndFrame();

private:
    void InitWindow();

private:
    std::atomic_bool m_ShouldQuit;
    SDL_Window* m_Window;
    dx12::Renderer* m_Renderer;
};

}

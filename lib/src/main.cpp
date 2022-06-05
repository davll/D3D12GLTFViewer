#include <mrdr.h>
#include "mrdr/dx12/context.h"
#include <spdlog/spdlog.h>
#include <SDL.h>
#include <SDL_main.h>

static SDL_Window* InitWindow()
{
    SDL_Window* window = SDL_CreateWindow("D3D12 GLTF Viewer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_RESIZABLE);

    if (!window) {
        SPDLOG_ERROR("Unable to create SDL window: {}", SDL_GetError());
        abort();
    }

    return window;
}

int main(int argc, char* argv[])
{
    // TODO: Load Config
    // Perhaps JSON format

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SPDLOG_ERROR("Unable to initialize SDL: {}", SDL_GetError());
        return 1;
    }

    //
    // Initialization
    //
    void* userdata_ptr = NULL;
    size_t userdata_siz = 0;
    {
        userdata_siz = mrdr_userdata_size();
        if (userdata_siz > 0) {
#ifdef _MSC_VER
            userdata_ptr = _aligned_malloc(userdata_siz, 16);
#else
            userdata_ptr = malloc(userdata_siz);
#endif
            if (!userdata_ptr) {
                SPDLOG_ERROR("Unable to allocate memory space for user data");
                SDL_Quit();
                abort();
            }
        }
    }
    SDL_Window* window = InitWindow();
    mrdr::dx12::Context* ctx = new mrdr::dx12::Context(window); // TODO: pass config
    mrdr_initialize(ctx, userdata_ptr);

    //
    // Update
    //
    for (bool running = true; running; ) {
        // Process OS events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (!ctx->ProcessEvent(event)) {
                running = false;
            }
        }
        if (!running)
            break;

        // Process a frame
        if (ctx->BeginFrame()) {
            mrdr_update(ctx, userdata_ptr);
            ctx->EndFrame();
        }
    }

    //
    // Shutdown
    //
    mrdr_shutdown(ctx, userdata_ptr);
    if (userdata_ptr) {
#ifdef _MSC_VER
        _aligned_free(userdata_ptr);
#else
        free(userdata_ptr);
#endif
    }
    delete ctx;
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#include <mrdr.h>
#include "mrdr/context.h"
#include <spdlog/spdlog.h>
#include <SDL.h>
#include <SDL_main.h>

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
    mrdr::Context* ctx = new mrdr::Context(); // TODO: pass config
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

        running = !ctx->ShouldQuit();
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
    SDL_Quit();

    return 0;
}

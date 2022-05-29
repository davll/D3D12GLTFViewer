#include <mrdr/common.h>
#include <mrdr/Renderer.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/msvc_sink.h>
#include <SDL.h>
#include <SDL_main.h>
#include <imgui_impl_sdl.h>
#include <memory>
#include "font.h"

static void SetupImGUIStyle();

int main(int argc, char* argv[])
{
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto debugger_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        //console_sink->set_level(spdlog::level::debug);
        //debugger_sink->set_level(spdlog::level::debug);
        std::vector<spdlog::sink_ptr> sinks { console_sink, debugger_sink };
        auto logger = std::make_shared<spdlog::logger>("my_logger", sinks.begin(), sinks.end());
        //logger->set_level(spdlog::level::debug);
        spdlog::set_default_logger(logger);
    }

    // SPDLOG_INFO("TEST LOGGING");
    // MRDR_FAIL("TEST FAIL");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        MRDR_FAIL("Unable to initialize SDL: {}", SDL_GetError());
    }

    {
        SDL_Window* _window = SDL_CreateWindow(argv[0],
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            800, 600,
            SDL_WINDOW_RESIZABLE);
        if (!_window) {
            MRDR_FAIL("Unable to create SDL window: {}", SDL_GetError());
        }
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window(_window, &SDL_DestroyWindow);

        mrdr::Renderer renderer(mrdr::Renderer::CreateInfo {
            3,
            window.get(),
        });

        SetupImGUIStyle();

        bool show_demo_window = true;

        for (bool running = true; running; ) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        renderer.Resize();
                        break;
                    }
                    break;
                default:
                    break;
                }
            }

            renderer.StartFrame();

            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            renderer.EndFrame();
        }
    }

    SPDLOG_INFO("Quitting");
    SDL_Quit();
    return 0;
}

static void AddChicagoFont(int size_in_pixels)
{
    ImGuiIO& io = ImGui::GetIO();

    ImFontConfig font_cfg = {};
    sprintf_s(font_cfg.Name, "Chicago, %dpx", size_in_pixels);
    size_t font_size = chicago_size();
    void* font_data = malloc(font_size);
    memcpy(font_data, chicago_data(), font_size);
    io.Fonts->AddFontFromMemoryTTF(font_data, (int)font_size, size_in_pixels, &font_cfg);
}

static void AddIBMPlexMonoFont(int size_in_pixels)
{
    ImGuiIO& io = ImGui::GetIO();

    ImFontConfig font_cfg = {};
    sprintf_s(font_cfg.Name, "IBMPlex-Mono, %dpx", size_in_pixels);
    size_t font_size = ibmplexmono_size();
    void* font_data = malloc(font_size);
    memcpy(font_data, ibmplexmono_data(), font_size);
    io.Fonts->AddFontFromMemoryTTF(font_data, (int)font_size, size_in_pixels, &font_cfg);
}

static void SetupImGUIStyle()
{
    AddIBMPlexMonoFont(24);
    AddChicagoFont(24);

    ImGuiStyle& style = ImGui::GetStyle();

    //style.ScaleAllSizes(2.0f);
    // SetupImGuiStyle2();
}

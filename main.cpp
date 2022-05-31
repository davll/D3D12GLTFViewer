#include <mrdr/common.h>
#include <mrdr/Renderer.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/msvc_sink.h>
#include <SDL.h>
#include <SDL_main.h>
#include <SDL_syswm.h>
#include <memory>
#include "guifont.h"
#include "guistyle.h"

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

        SDL_SysWMinfo wminfo;
        SDL_VERSION(&wminfo.version);
        if (SDL_GetWindowWMInfo(window.get(), &wminfo) == 0) {
            MRDR_FAIL("Unable to get SDL_WMInfo");
        }
        HWND hwnd = wminfo.info.win.window;

        mrdr::Renderer renderer(mrdr::Renderer::CreateInfo {
            3,
            window.get(),
        });

        guifont_add_ibmplexmono_font(24);
        guifont_add_chicago_font(24);

        bool show_app_metrics = false;
        bool show_app_style_editor = false;
        bool show_demo_window = false;
        GuiStyle huestyle;
        guistyle_init(huestyle);
        guistyle_apply(huestyle);

        for (bool running = true; running; ) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                renderer.ProcessEvent(&event);
                switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                default:
                    break;
                }
            }

            renderer.StartFrame();

            if (show_app_metrics)
                ImGui::ShowMetricsWindow(&show_app_metrics);
            if (show_app_style_editor)
                guistyle_edit(huestyle, &show_app_style_editor);
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("Open", "Ctrl+O")) {
                        std::vector<wchar_t> filePath(1024, 0);
                        OPENFILENAMEW data = {};
                        data.lStructSize = sizeof(OPENFILENAMEW);
                        data.hwndOwner = hwnd;
                        data.lpstrFilter = L"GLTF Files (*.gltf, *.glb)\0*.gltf;*.glb\0All Files (*.*)\0*.*\0\0";
                        data.lpstrFile = filePath.data();
                        data.nMaxFile = filePath.size();
                        data.lpstrTitle = L"Open File";
                        data.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                        if (GetOpenFileNameW(&data)) {
                            wprintf_s(L"%s\n", filePath.data());
                        }
                    }
                    ImGui::Separator();
                    if (ImGui::MenuItem("Quit", "Ctrl+Q")) {
                        running = false;
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Tools")) {
                    ImGui::MenuItem("Metrics/Debugger", NULL, &show_app_metrics);
                    ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
                    ImGui::MenuItem("Demo Window", NULL, &show_demo_window);
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

            renderer.EndFrame();
        }
    }

    SPDLOG_INFO("Quitting");
    SDL_Quit();
    return 0;
}

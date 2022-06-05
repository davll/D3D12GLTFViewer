#include "guifont.h"
#include <imgui.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void guifont_add_chicago_font(int size_in_pixels)
{
    ImGuiIO& io = ImGui::GetIO();

    ImFontConfig font_cfg = {};
    sprintf_s(font_cfg.Name, "Chicago, %dpx", size_in_pixels);
    size_t font_size = guifont_chicago_size();
    void* font_data = malloc(font_size);
    memcpy(font_data, guifont_chicago_data(), font_size);
    io.Fonts->AddFontFromMemoryTTF(font_data, (int)font_size, size_in_pixels, &font_cfg);
}

void guifont_add_ibmplexmono_font(int size_in_pixels)
{
    ImGuiIO& io = ImGui::GetIO();

    ImFontConfig font_cfg = {};
    sprintf_s(font_cfg.Name, "IBMPlex-Mono, %dpx", size_in_pixels);
    size_t font_size = guifont_ibmplexmono_size();
    void* font_data = malloc(font_size);
    memcpy(font_data, guifont_ibmplexmono_data(), font_size);
    io.Fonts->AddFontFromMemoryTTF(font_data, (int)font_size, size_in_pixels, &font_cfg);
}

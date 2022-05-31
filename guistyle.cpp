#include "guistyle.h"
#include <imgui.h>

void guistyle_init(GuiStyle& huestyle)
{
    huestyle.hue = 148.f/255.f;
    huestyle.col_main_sat = 180.f/255.f;
    huestyle.col_main_val = 161.f/255.f;
    huestyle.col_area_sat = 124.f/255.f;
    huestyle.col_area_val = 100.f/255.f;
    huestyle.col_back_sat = 59.f/255.f;
    huestyle.col_back_val = 40.f/255.f;
}

void guistyle_apply(const GuiStyle& huestyle)
{
    ImGuiStyle& style = ImGui::GetStyle();

    ImVec4 col_text = ImColor::HSV(
        huestyle.hue,  20.f/255.f, 235.f/255.f);
    ImVec4 col_main = ImColor::HSV(
        huestyle.hue, huestyle.col_main_sat, huestyle.col_main_val);
    ImVec4 col_back = ImColor::HSV(
        huestyle.hue, huestyle.col_back_sat, huestyle.col_back_val);
    ImVec4 col_area = ImColor::HSV(
        huestyle.hue, huestyle.col_area_sat, huestyle.col_area_val);

    style.Colors[ImGuiCol_Text]                  = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);
    style.Colors[ImGuiCol_ChildBg]               = ImVec4(col_area.x, col_area.y, col_area.z, 0.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(col_area.x, col_area.y, col_area.z, 0.90f);
    style.Colors[ImGuiCol_Border]                = ImVec4(col_text.x, col_text.y, col_text.z, 0.30f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(col_main.x, col_main.y, col_main.z, 0.68f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(col_main.x, col_main.y, col_main.z, 0.45f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(col_main.x, col_main.y, col_main.z, 0.35f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(col_area.x, col_area.y, col_area.z, 0.57f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(col_main.x, col_main.y, col_main.z, 0.31f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(col_main.x, col_main.y, col_main.z, 0.80f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(col_main.x, col_main.y, col_main.z, 0.24f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_Separator]             = ImVec4(col_text.x, col_text.y, col_text.z, 0.80f);
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(col_text.x, col_text.y, col_text.z, 0.90f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_Tab]                   = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
    style.Colors[ImGuiCol_TabHovered]            = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
    style.Colors[ImGuiCol_TabActive]             = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(col_area.x, col_area.y, col_area.z, 0.44f);
    style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
    style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(col_text.x, col_text.y, col_text.z, 0.80f);
    style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(col_text.x, col_text.y, col_text.z, 0.80f);
    style.Colors[ImGuiCol_TableRowBg]            = ImVec4(col_area.x, col_area.y, col_area.z, 0.30f);
    style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(col_area.x, col_area.y, col_area.z, 0.60f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
    style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
    style.Colors[ImGuiCol_NavHighlight]          = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
    style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void guistyle_edit(GuiStyle& huestyle, bool* p_open)
{
    ImGui::Begin("GUI Style Editor", p_open);

    if (ImGui::CollapsingHeader("Hue Style")) {
        int hue = (int)(huestyle.hue * 255.f);
        hue = std::min(hue, 255);
        hue = std::max(hue, 0);
        ImGui::SliderInt("master hue", &hue, 0, 255);
        huestyle.hue = hue / 255.f;

        float dummy;
        ImVec4 rgb;

        ImGui::ColorConvertHSVtoRGB(hue/255.f, huestyle.col_main_sat, huestyle.col_main_val, rgb.x, rgb.y, rgb.z);
        ImGui::ColorEdit3("main", &rgb.x);
        ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, huestyle.col_main_sat, huestyle.col_main_val);

        ImGui::ColorConvertHSVtoRGB(hue/255.f, huestyle.col_area_sat, huestyle.col_area_val, rgb.x, rgb.y, rgb.z);
        ImGui::ColorEdit3("area", &rgb.x);
        ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, huestyle.col_area_sat, huestyle.col_area_val);

        ImGui::ColorConvertHSVtoRGB(hue/255.f, huestyle.col_back_sat, huestyle.col_back_val, rgb.x, rgb.y, rgb.z);
        ImGui::ColorEdit3("back", &rgb.x);
        ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, huestyle.col_back_sat, huestyle.col_back_val);

        if (ImGui::Button("Reset")) {
            guistyle_init(huestyle);
        }

        guistyle_apply(huestyle);
    }

    if (ImGui::CollapsingHeader("Style Editor")) {
        ImGui::ShowStyleEditor();
    }

    ImGui::End();
}

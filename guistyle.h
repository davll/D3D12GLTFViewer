#pragma once

struct GuiStyle {
    float hue;
    float col_main_sat;
    float col_main_val;
    float col_area_sat;
    float col_area_val;
    float col_back_sat;
    float col_back_val;
};

void guistyle_init(GuiStyle& guistyle);
void guistyle_apply(const GuiStyle& guistyle);
void guistyle_edit(GuiStyle& guistyle, bool* p_open);

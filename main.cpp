#include <mrdr.h>
#include "guistyle.h"
#include "guifont.h"

struct UserData {
    bool ShowImGuiDemoWindow;
};

size_t mrdr_userdata_size()
{
    return sizeof(UserData);
}

void mrdr_initialize(mrdr::IContext* ctx, void* udmem)
{
    UserData* userdata = ::new(udmem) UserData();
    userdata->ShowImGuiDemoWindow = true;

    guifont_add_ibmplexmono_font(20);
    guifont_add_chicago_font(20);

    GuiStyle huestyle;
    guistyle_init(huestyle);
    guistyle_apply(huestyle);
}

void mrdr_update(mrdr::IContext* ctx, void* ud)
{
    UserData* userdata = (UserData*)ud;

    if (userdata->ShowImGuiDemoWindow)
        ImGui::ShowDemoWindow(&userdata->ShowImGuiDemoWindow);
}

void mrdr_shutdown(mrdr::IContext* ctx, void* ud)
{
    UserData* userdata = (UserData*)ud;
    userdata->~UserData();
}

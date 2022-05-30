#pragma once

#include "common.h"

namespace mrdr {

enum class StaticResourceDescriptorIds {
    IMGUI_FONT_SRV = 0,
    _COUNT,
};

enum class StaticSamplerDescriptorIds {
    _COUNT = 0,
};

struct StaticDescriptors {
    struct {
        union {
            ShaderDescriptorHandle Handles[(UINT)StaticResourceDescriptorIds::_COUNT];
            struct {
                ShaderDescriptorHandle ImGuiFontSRV;
            };
        };
    } Resources;
    struct {
        union {
            ShaderDescriptorHandle Handles[(UINT)StaticSamplerDescriptorIds::_COUNT];
            struct {
            };
        };
    } Samplers;
};

}

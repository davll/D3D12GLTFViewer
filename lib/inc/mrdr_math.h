#pragma once

#include <stddef.h>
#include <stdint.h>
#include <float.h>

namespace mrdr {

struct vec3 {
    float x, y, z;
};

struct vec4 {
    float x, y, z, w;
};

struct quat {
    float x, y, z, w;
};

struct mat4 {
    vec4  x, y, z, w;
};

}

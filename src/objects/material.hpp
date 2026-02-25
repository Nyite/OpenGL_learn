#pragma once
#include "../textures/texture.hpp"

struct TMaterial {
    TTexture diffuse_map;
    TTexture specular_map;
    TTexture emmit_map;

    float shine;
    float emitStrenght;
};

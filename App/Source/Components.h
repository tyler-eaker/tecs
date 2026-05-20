#pragma once

#include <raylib.h>

#include <string>

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Sprite {
    std::string textureId;
    uint32_t width;
    uint32_t height;
    Color tint;
};
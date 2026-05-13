#pragma once
#include <cstdint>
#include <raylib.h>

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Sprite {
    uint32_t width;
    uint32_t height;
    Color color;
};
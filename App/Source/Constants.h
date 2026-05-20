#pragma once

#include <raylib.h>

#include <cstdint>

#define SPDLOG_ASSERT(condition, ...) \
    if (!(condition)) { \
        spdlog::critical("Assertion Failed: " __VA_ARGS__); \
        std::abort(); \
    }

constexpr uint32_t SCREEN_WIDTH = 1920;
constexpr uint32_t SCREEN_HEIGHT= 1080;
constexpr uint32_t FPS_LIMIT = 120;

constexpr uint32_t MAX_COLORS_COUNT = 21;
constexpr Color CLEAR_BACKGROUND_COLOR = BLACK;
constexpr Color COLORS[MAX_COLORS_COUNT] = {
        DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
        GREEN, SKYBLUE, PURPLE, BEIGE };
#pragma once
#include <raylib.h>
#include <raymath.h>

struct Sprite {
    Texture2D texture;
    Vector2 source_anchor = {}, source_size = {};
    int horizontal_frames = 1, vertical_frames = 1;
    Vector2 rotation_origin = {};

    Rectangle source(const int &frame) const {
        return Rectangle{
            .x = source_anchor.x + source_size.x * (frame % horizontal_frames),
            .y = source_anchor.y +
                 source_size.y * static_cast<int>(frame / horizontal_frames),
            .width = source_size.x,
            .height = source_size.y,
        };
    }

    Rectangle dest(const Vector2 &position, const Vector2 &scale) const {
        return Rectangle{
            .x = position.x,
            .y = position.y,
            .width = source_size.x * scale.x,
            .height = source_size.y * scale.y,
        };
    }

    Vector2 offset(const Vector2 &scale) const {
        return rotation_origin * scale;
    }
};

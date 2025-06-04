#pragma once
#include <raylib.h>
#include <raymath.h>

#include "./sprite.h"

inline void draw_sprite_pro(const Sprite &sprite, const int frame,
                            const Vector2 &position, const Vector2 &scale,
                            const float rotation, const bool flip_h,
                            const bool flip_v, const Color &tint) {
    DrawTexturePro(sprite.texture, sprite.source(frame, flip_h, flip_v),
                   sprite.dest(position, scale), sprite.offset(scale), rotation,
                   tint);
}

inline void draw_sprite(const Sprite &sprite, const Vector2 &position,
                        const Vector2 &scale = {1, 1}) {
    draw_sprite_pro(sprite, 0, position, scale, 0, false, false, WHITE);
}

inline void draw_billboard_pro(const Camera &camera, const Sprite &sprite,
                               const int frame, const Vector3 &position,
                               const Vector2 &scale, const float rotation,
                               const bool flip_h, const bool flip_v,
                               const Color &tint) {
    DrawBillboardPro(camera, sprite.texture,
                     sprite.source(frame, flip_h, flip_v), position, {0, 1, 0},
                     scale, sprite.billboard_offset(scale), rotation, tint);
}

inline void draw_circle_arc_pro(const Vector2 &p0, const Vector2 &p1,
                                const float t, const float radius,
                                const float line_size, const Color &color) {
    const Vector2 p01 = p1 - p0;
    const Vector2 p2 = Vector2Lerp(p0, p1, .5);
    const Vector2 n = Vector2Normalize({p01.y, -p01.x}) * (p01.x > 0 ? -1 : 1);
    const Vector2 c =
        p2 + n * sqrtf(radius * radius - Vector2DistanceSqr(p0, p1) / 4);
    const float angle_from = fmodf(
        fmodf(2 * PI - Vector2Angle(p0 - c, {1, 0}), 2 * PI) + 6 * PI, 2 * PI);
    const float angle_to = fmodf(
        fmodf(2 * PI - Vector2Angle(p1 - c, {1, 0}), 2 * PI) + 6 * PI, 2 * PI);
    const float angle_min = fminf(angle_from, angle_to);
    const float angle_max = fmaxf(angle_from, angle_to);
    const float angle_dist = angle_max - angle_min;
    const float caf = angle_dist > PI ? angle_max : angle_min;
    const float cat =
        Lerp(caf, angle_dist > PI ? (angle_min + 2 * PI) : angle_max, t);
    const int segments = 64;
    for (int i = 0; i < segments; ++i) {
        const float theta_from = Lerp(caf, cat, float(i) / float(segments));
        const float theta_to = Lerp(caf, cat, float(i + 1) / float(segments));
        DrawLineEx(c + Vector2{cosf(theta_from), sinf(theta_from)} * radius,
                   c + Vector2{cosf(theta_to), sinf(theta_to)} * radius,
                   line_size, color);
    }
}

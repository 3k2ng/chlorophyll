#pragma once
#include <raylib.h>
#include <raymath.h>

#include <vector>

#include "./sprite.h"

inline void draw_sprite(const Sprite &sprite, const int &frame,
                        const Vector2 &position, const Vector2 &scale,
                        const float &rotation, const Color &tint = WHITE) {
    DrawTexturePro(sprite.texture, sprite.source(frame),
                   sprite.dest(position, scale), sprite.offset(scale), rotation,
                   tint);
}

inline void draw_billboard(const Camera &camera, const Sprite &sprite,
                           const int &frame, const Vector3 &position,
                           const Vector2 &scale, const float &rotation,
                           const Color &tint = WHITE) {
    DrawBillboardPro(camera, sprite.texture, sprite.source(frame), position,
                     {0, 1, 0}, scale, sprite.billboard_offset(scale), rotation,
                     tint);
}

class RenderGuy {
  public:
    RenderGuy() {}
    ~RenderGuy() {
        for (const Texture2D &texture : textures_) {
            UnloadTexture(texture);
        }
    }

    Texture add_texture(const char *file) {
        textures_.push_back(LoadTexture(file));
        return textures_.back();
    }

  private:
    std::vector<Texture> textures_;
};

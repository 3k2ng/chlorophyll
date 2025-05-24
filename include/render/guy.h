#pragma once
#include <raylib.h>
#include <raymath.h>

#include <vector>

#include "./sprite.h"

class RenderGuy {
  public:
    RenderGuy() {}
    ~RenderGuy() {
        for (const Texture2D &texture : textures_) {
            UnloadTexture(texture);
        }
    }

    Texture2D add_texture(const char *file) {
        textures_.push_back(LoadTexture(file));
        return textures_.back();
    }

    void draw_sprite(const Sprite &sprite, const int &frame,
                     const Vector2 &position, const Vector2 &scale,
                     const float &rotation, const Color &tint = WHITE) const {
        DrawTexturePro(sprite.texture, sprite.source(frame),
                       sprite.dest(position, scale), sprite.offset(scale),
                       rotation, tint);
    }

  private:
    std::vector<Texture2D> textures_;
};

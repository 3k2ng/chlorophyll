#include <raylib.h>

#include "render/guy.h"

class Timer {
  public:
    Timer(const float &cooldown, const bool &reset = true)
        : cooldown_{cooldown}, timer_{reset ? cooldown : 0} {}

    void update(const float delta) { timer_ -= delta; }
    void reset() { timer_ = cooldown_; }
    bool finished() const { return timer_ <= 0; }

  private:
    float cooldown_, timer_;
};

int main(int argc, char *argv[]) {
    InitWindow(640, 480, "chlorophyll");

    RenderGuy render;
    Texture2D bridget_texture =
        render.add_texture("./data/textures/bridget.jpg");

    Sprite bridget_sprite;
    bridget_sprite.texture = bridget_texture;
    bridget_sprite.source_anchor = {};
    bridget_sprite.source_size = {512, 512};
    bridget_sprite.horizontal_frames = 4;
    bridget_sprite.vertical_frames = 4;
    bridget_sprite.rotation_origin = {256, 256};

    int frame = 0;
    float rotation = 0;

    Timer frame_timer(1);

    while (!WindowShouldClose()) {
        const float delta = GetFrameTime();

        rotation += 30 * delta;
        if (frame_timer.finished()) {
            frame = (frame + 1) % 16;
            frame_timer.reset();
        }

        frame_timer.update(delta);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        render.draw_sprite(bridget_sprite, frame, {256, 256}, {0.5, 0.5},
                           rotation);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

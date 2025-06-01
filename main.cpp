#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include "./render/guy.h"

int main(int argc, char *argv[]) {
    InitWindow(1280, 720, "chlorophyll");

    Camera3D camera;
    camera.position = {20.0f, 4.0f, 0};
    camera.target = {0, 1, 0};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 30.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    RenderGuy rg;

    auto bridget = rg.add_texture("./data/textures/bridget.jpg");
    Sprite bgs;
    bgs.texture = bridget;
    bgs.source_anchor = {};
    bgs.source_size = {1024, 1024};
    bgs.origin = {512, 0};
    bgs.horizontal_frames = 2;
    bgs.vertical_frames = 2;

    while (!WindowShouldClose()) {
        const float delta = GetFrameTime();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        std::vector<Vector3> character_positions = {
            {-4, 0, +2}, {-4, 0, +6}, {+4, 0, +2}, {+4, 0, +6},
            {-4, 0, -2}, {-4, 0, -6}, {+4, 0, -2}, {+4, 0, -6},
        };

        {
            BeginMode3D(camera);
            DrawGrid(64, 1.0f);

            for (const auto &cp : character_positions) {
                draw_billboard(camera, bgs, 0, cp, {2, 2}, 0);
            }

            EndMode3D();
        }

        for (const auto cp : character_positions) {
            const Vector2 center =
                GetWorldToScreen(cp + Vector3{0, 2, 0}, camera) +
                Vector2{0, -40};
            DrawRectangleRec(Rectangle{center.x - 20, center.y - 20, 40, 40},
                             GREEN);
        }

        {
            for (const auto [i, j] : std::vector<std::pair<int, int>>{
                     {0, 5},
                     {1, 5},
                     {2, 5},
                     {3, 5},
                 }) {
                const Vector2 from =
                    GetWorldToScreen(character_positions[i] + Vector3{0, 2, 0},
                                     camera) +
                    Vector2{0, -40};
                const Vector2 to =
                    GetWorldToScreen(character_positions[j] + Vector3{0, 2, 0},
                                     camera) +
                    Vector2{0, -40};
                const Vector2 tmf = to - from;
                const Vector2 middle = Vector2Lerp(from, to, .5) +
                                       Vector2{tmf.y, -fabsf(tmf.x)} * .3;
                const Vector2 tmm = to - middle;
                const Vector2 arrow_up = Vector2Normalize({tmm.y, -tmm.x});
                const Vector2 arrow_right = Vector2Normalize(tmm);
                const float arrow_size = 20;
                const std::vector<Vector2> controls = {
                    from, middle, to - arrow_right * arrow_size * .5};
                DrawSplineBezierQuadratic(controls.data(), controls.size(), 12,
                                          ORANGE);
                DrawTriangle(to + arrow_right * 4,
                             to - arrow_right * (arrow_size + 2) +
                                 arrow_up * (arrow_size + 8) * .5,
                             to - arrow_right * (arrow_size + 2) -
                                 arrow_up * (arrow_size + 8) * .5,
                             ORANGE);
                DrawSplineBezierQuadratic(controls.data(), controls.size(), 8,
                                          YELLOW);
                DrawTriangle(
                    to,
                    to - arrow_right * arrow_size + arrow_up * arrow_size * .5,
                    to - arrow_right * arrow_size - arrow_up * arrow_size * .5,
                    YELLOW);
            }
        }

        {
            DrawRectangleRec(
                Rectangle{0, static_cast<float>(GetScreenHeight() - 160),
                          static_cast<float>(GetScreenWidth()), 80},
                BLUE);
            for (int i = 0; i < 12; ++i) {
                DrawRectangleRec(
                    Rectangle{50 + static_cast<float>((80 + 20) * i),
                              static_cast<float>(GetScreenHeight() - 160), 80,
                              80},
                    GREEN);
            }
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

#include <raylib.h>

int main(int argc, char *argv[]) {
    InitWindow(1280, 720, "chlorophyll");
    while (!WindowShouldClose()) {
        const float delta = GetFrameTime();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        {
            DrawRectangleRec(Rectangle{0, 0, 560, 240}, BLUE);
            DrawRectangleRec(
                Rectangle{static_cast<float>(GetScreenWidth() - 560), 0, 560,
                          240},
                BLUE);
        }

        {
            DrawRectangleRec(
                Rectangle{0, static_cast<float>(GetScreenHeight() - 80),
                          static_cast<float>(GetScreenWidth()), 80},
                BLUE);
            for (int i = 0; i < 12; ++i) {
                DrawRectangleRec(
                    Rectangle{50 + static_cast<float>((80 + 20) * i),
                              static_cast<float>(GetScreenHeight() - 80), 80,
                              80},
                    GREEN);
            }
        }

        {
            DrawRectangleRec(Rectangle{0, 240, 560, 400}, LIGHTGRAY);
            DrawRectangleRec(Rectangle{200, 320, 80, 80}, BLUE);
            DrawRectangleRec(Rectangle{200, 480, 80, 80}, BLUE);
            DrawRectangleRec(Rectangle{400, 320, 80, 80}, BLUE);
            DrawRectangleRec(Rectangle{400, 480, 80, 80}, BLUE);
            DrawRectangleRec(
                Rectangle{static_cast<float>(GetScreenWidth() - 560), 240, 560,
                          400},
                LIGHTGRAY);
            DrawRectangleRec(
                Rectangle{static_cast<float>(GetScreenWidth() - 80 - 200), 320,
                          80, 80},
                BLUE);
            DrawRectangleRec(
                Rectangle{static_cast<float>(GetScreenWidth() - 80 - 200), 480,
                          80, 80},
                BLUE);
            DrawRectangleRec(
                Rectangle{static_cast<float>(GetScreenWidth() - 80 - 400), 320,
                          80, 80},
                BLUE);
            DrawRectangleRec(
                Rectangle{static_cast<float>(GetScreenWidth() - 80 - 400), 480,
                          80, 80},
                BLUE);
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

#include <raylib.h>

enum class Navigation : unsigned char {
    None,
    Up,
    Down,
    Left,
    Right,
};

Navigation get_navigation() {
    int updown = 0; // -1 is up, 1 is down
    if (IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP)) {
        --updown;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN)) {
        ++updown;
    }
    if (updown < 0) {
        return Navigation::Up;
    }
    if (updown > 0) {
        return Navigation::Down;
    }
    int leftright = 0; // -1 is left, 1 is right
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
        --leftright;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
        ++leftright;
    }
    if (leftright < 0) {
        return Navigation::Left;
    }
    if (leftright > 0) {
        return Navigation::Right;
    }
    return Navigation::None;
}

int main(int argc, char *argv[]) {
    InitWindow(640, 480, "chlorophyll");
    while (!WindowShouldClose()) {
        const float delta = GetFrameTime();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

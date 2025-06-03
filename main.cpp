#include <optional>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include "./render/guy.h"

bool left_click(const Rectangle &rect) {
    return IsMouseButtonReleased(MOUSE_BUTTON_LEFT) &&
           CheckCollisionPointRec(GetMousePosition(), rect);
}

void draw_arc(const Vector2 &p0, const Vector2 &p1, const float radius,
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
    const float cat = angle_dist > PI ? (angle_min + 2 * PI) : angle_max;
    const int segments = 64;
    for (int i = 0; i < segments; ++i) {
        const float theta_from = Lerp(caf, cat, float(i) / float(segments));
        const float theta_to = Lerp(caf, cat, float(i + 1) / float(segments));
        DrawLineEx(c + Vector2{cosf(theta_from), sinf(theta_from)} * radius,
                   c + Vector2{cosf(theta_to), sinf(theta_to)} * radius,
                   line_size, color);
    }
}

int main(int argc, char *argv[]) {
    InitWindow(1280, 720, "chlorophyll");

    Camera3D camera;
    camera.position = {10.0f, 2.0f, 0};
    camera.target = {0, .5f, 0};
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

    const float ui_slot_size = 80;
    const float ui_slot_spacing = 20;
    const float field_slot_size = 40;
    const float field_slot_spacing = 10;

    const std::vector<Vector3> player_character_position = {
        {-2, 0, +1},
        {-2, 0, +3},
        {+2, 0, +1},
        {+2, 0, +3},
    };
    const std::vector<Vector3> enemy_character_position = {
        {-2, 0, -1},
        {-2, 0, -3},
        {+2, 0, -1},
        {+2, 0, -3},
    };
    const std::vector<Color> player_character_slot_color = {
        RED,
        BLUE,
        GREEN,
        MAGENTA,
    };

    const std::vector<int> player_speed_roll = {1, 2, 2, 3};
    const std::vector<int> player_slot_count = {2, 1, 3, 2};

    const std::vector<int> enemy_speed_roll = {1, 2, 2, 3};
    const std::vector<int> enemy_slot_count = {2, 2, 2, 2};

    struct SlotDetails {
        bool is_player;
        int i, j;
    };
    std::optional<SlotDetails> current_selection;
    std::vector<std::pair<SlotDetails, SlotDetails>> target_list;

    auto draw_arrow = [](const Vector2 &from_position,
                         const Vector2 &to_position) {
        const Color border_color = ORANGE;
        const Color line_color = YELLOW;
        const float line_size = 4;
        const float point_size = 12;
        const float border_size = 2;
        const float arc_radius = GetScreenWidth() * .5;
        draw_arc(from_position, to_position, arc_radius,
                 line_size + border_size * 2, border_color);
        DrawRectangleRec(
            Rectangle{
                to_position.x - point_size / 2 - border_size,
                to_position.y - point_size / 2 - border_size,
                point_size + border_size * 2,
                point_size + border_size * 2,
            },
            border_color);
        draw_arc(from_position, to_position, arc_radius, line_size, line_color);
        DrawRectangleRec(
            Rectangle{
                to_position.x - point_size / 2,
                to_position.y - point_size / 2,
                point_size,
                point_size,
            },
            line_color);
    };

    while (!WindowShouldClose()) {
        // pre input calculation
        std::vector<std::vector<Rectangle>> ui_slot_rect(
            player_slot_count.size());

        std::vector<std::vector<Rectangle>> player_field_slot_rect(
            player_slot_count.size());
        std::vector<std::vector<Rectangle>> enemy_field_slot_rect(
            enemy_slot_count.size());

        {
            int ui_slot_count = 0;
            for (const int sc : player_slot_count) {
                ui_slot_count += sc;
            }
            const float slot_bar_width =
                ui_slot_count * (ui_slot_size + ui_slot_spacing) -
                ui_slot_spacing;
            int i = 0;
            for (int j = 0; j < player_slot_count.size(); ++j) {
                ui_slot_rect[j] = std::vector<Rectangle>(
                    player_slot_count[j], {0, 0, ui_slot_size, ui_slot_size});
                for (int k = 0; k < player_slot_count[j]; ++k) {
                    ui_slot_rect[j][k].x =
                        (GetScreenWidth() - slot_bar_width) / 2 +
                        (ui_slot_size + ui_slot_spacing) * i;
                    ui_slot_rect[j][k].y = GetScreenHeight() - ui_slot_size * 2;
                    ++i;
                }
            }
        }

        for (int i = 0; i < player_slot_count.size(); ++i) {
            const float player_slot_bar_width =
                player_slot_count[i] * (field_slot_size + field_slot_spacing) -
                field_slot_spacing;
            player_field_slot_rect[i] = std::vector<Rectangle>(
                player_slot_count[i], {0, 0, field_slot_size, field_slot_size});
            const Vector2 center =
                GetWorldToScreen(
                    player_character_position[i] + Vector3{0, 1, 0}, camera) +
                Vector2{0, -1} * (field_slot_size / 2 + field_slot_spacing);
            for (int j = 0; j < player_slot_count[i]; ++j) {
                player_field_slot_rect[i][j].x =
                    center.x - player_slot_bar_width / 2 +
                    (field_slot_size + field_slot_spacing) * j;
                player_field_slot_rect[i][j].y = center.y - field_slot_size / 2;
            }
        }

        for (int i = 0; i < enemy_slot_count.size(); ++i) {
            const float enemy_slot_bar_width =
                enemy_slot_count[i] * (field_slot_size + field_slot_spacing) -
                field_slot_spacing;
            enemy_field_slot_rect[i] = std::vector<Rectangle>(
                enemy_slot_count[i], {0, 0, field_slot_size, field_slot_size});
            const Vector2 center =
                GetWorldToScreen(enemy_character_position[i] + Vector3{0, 1, 0},
                                 camera) +
                Vector2{0, -1} * (field_slot_size / 2 + field_slot_spacing);
            for (int j = 0; j < enemy_slot_count[i]; ++j) {
                enemy_field_slot_rect[i][j].x =
                    center.x - enemy_slot_bar_width / 2 +
                    (field_slot_size + field_slot_spacing) * j;
                enemy_field_slot_rect[i][j].y = center.y - field_slot_size / 2;
            }
        }

        auto field_slot_position = [&](const SlotDetails slot) -> Vector2 {
            if (slot.is_player) {
                const Rectangle pfsr = player_field_slot_rect[slot.i][slot.j];
                return Vector2{
                    pfsr.x + pfsr.width / 2,
                    pfsr.y + pfsr.height / 2,
                };
            } else {
                const Rectangle efsr = enemy_field_slot_rect[slot.i][slot.j];
                return Vector2{
                    efsr.x + efsr.width / 2,
                    efsr.y + efsr.height / 2,
                };
            }
        };

        // input phase
        const float delta = GetFrameTime();
        const Vector2 mpos = GetMousePosition();

        // render phase
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (current_selection.has_value()) {
            std::optional<SlotDetails> target_slot = std::nullopt;
            // player
            for (int i = 0; i < player_slot_count.size(); ++i) {
                for (int j = 0; j < player_slot_count[i]; ++j) {
                    if (left_click(player_field_slot_rect[i][j]) &&
                        current_selection.value().i != i) {
                        target_slot = SlotDetails{true, i, j};
                    }
                }
            }
            // enemy
            for (int i = 0; i < enemy_slot_count.size(); ++i) {
                for (int j = 0; j < enemy_slot_count[i]; ++j) {
                    if (left_click(enemy_field_slot_rect[i][j])) {
                        target_slot = SlotDetails{false, i, j};
                    }
                }
            }
            if (target_slot.has_value()) {
                target_list.push_back({
                    current_selection.value(),
                    target_slot.value(),
                });
                current_selection = std::nullopt;
            }
        } else {
            for (int i = 0; i < player_slot_count.size(); ++i) {
                for (int j = 0; j < player_slot_count[i]; ++j) {
                    if (left_click(ui_slot_rect[i][j])) {
                        current_selection = SlotDetails{true, i, j};
                    }
                }
            }
        }

        // field character
        {
            BeginMode3D(camera);

            DrawGrid(32, 1.0f);

            for (const auto &cp : player_character_position) {
                draw_billboard(camera, bgs, 0, cp, {1, 1}, 0);
            }

            for (const auto &cp : enemy_character_position) {
                draw_billboard(camera, bgs, 0, cp, {1, 1}, 0);
            }

            EndMode3D();
        }

        // field slot
        {
            // player
            for (int i = 0; i < player_slot_count.size(); ++i) {
                for (const auto &pfsr : player_field_slot_rect[i]) {
                    DrawRectangleRec(pfsr, player_character_slot_color[i]);
                }
            }
            // enemy
            for (int i = 0; i < enemy_slot_count.size(); ++i) {
                for (const auto &efsr : enemy_field_slot_rect[i]) {
                    DrawRectangleRec(efsr, GRAY);
                }
            }
        }

        for (const auto &t : target_list) {
            const Vector2 from_position = field_slot_position(t.first);
            const Vector2 to_position = field_slot_position(t.second);
            draw_arrow(from_position, to_position);
        }

        // ui slot bar
        {
            for (int i = 0; i < player_slot_count.size(); ++i) {
                for (const auto &usr : ui_slot_rect[i]) {
                    DrawRectangleRec(usr, player_character_slot_color[i]);
                }
            }
        }

        if (current_selection.has_value()) {
            const Vector2 from_position =
                field_slot_position(current_selection.value());
            const Vector2 to_position = mpos;
            draw_arrow(from_position, to_position);
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

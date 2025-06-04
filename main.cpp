#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <vector>

#include "./gameplay/skill.h"

std::vector<Skill> skill_pool = {
    Skill{
        .name = "Test Source",
        .icon = "./data/textures/test_texture/attack.png",
        .cost = 1,
        .content =
            Source{
                .hit_list =
                    {
                        Hit{
                            .range_min = 1,
                            .range_max = 2,
                            .hit_effect = std::nullopt,
                        },
                        Hit{
                            .range_min = 1,
                            .range_max = 3,
                            .hit_effect = CombatEffect{},
                        },
                    },
            },
        .combat_start_effect = CombatEffect{},
        .use_effect = CombatEffect{},
    },
    Skill{
        .name = "Test Block",
        .icon = "./data/textures/test_texture/block.png",
        .cost = 1,
        .content =
            Modifier{
                .range_min = 2,
                .range_max = 3,
                .type = Modifier::Type::Block,
                .modifier_effect = std::nullopt,
            },
        .combat_start_effect = std::nullopt,
        .use_effect = std::nullopt,
    },
    Skill{
        .name = "Test Evade",
        .icon = "./data/textures/test_texture/evade.png",
        .cost = 1,
        .content =
            Modifier{
                .range_min = 1,
                .range_max = 3,
                .type = Modifier::Type::Evade,
                .modifier_effect = ModifierEffect{},
            },
        .combat_start_effect = std::nullopt,
        .use_effect = std::nullopt,
    },
};

void draw_card_icon(const Skill &skill, const Texture &icon_texture,
                    const Vector2 &position) {
    DrawTexturePro(icon_texture, {0, 0, 400, 400},
                   {position.x, position.y, 160, 160}, {}, 0, WHITE);
    DrawRectangleLinesEx({position.x, position.y, 160, 160}, 4, BLUE);
    DrawRectangle(position.x, position.y, 44, 44, BLUE);
    const char *skill_cost = TextFormat("%d", skill.cost);
    const float offset = float(40 - MeasureText(skill_cost, 40)) / 2;
    DrawText(skill_cost, position.x + offset + 2, position.y + 2, 40, WHITE);
}

void draw_card_details(const Skill &skill, const Vector2 &position) {
    const Vector2 details_position = position + Vector2{160, 0};
    const char *skill_type =
        std::holds_alternative<Source>(skill.content)
            ? "Source"
            : (std::get<Modifier>(skill.content).type == Modifier::Type::Block
                   ? "Block"
                   : "Evade");
    const char *skill_title =
        TextFormat("[%s] %s", skill_type, skill.name.c_str());
    DrawRectangle(details_position.x, details_position.y,
                  MeasureText(skill_title, 20) + 16, 22, BLUE);
    Vector2 draw_position = details_position;
    draw_position.x += 2;
    draw_position.y += 2;
    DrawText(skill_title, draw_position.x, draw_position.y, 20, WHITE);
    draw_position.x += 2;
    draw_position.y += 24;
    if (skill.combat_start_effect.has_value()) {
        const char *effect_text =
            TextFormat("Cycle start: %s",
                       skill.combat_start_effect.value().effect_text().c_str());
        DrawText(effect_text, draw_position.x, draw_position.y, 20, RED);
        draw_position.y += 24;
    }
    if (skill.use_effect.has_value()) {
        const char *effect_text = TextFormat(
            "Use: %s", skill.combat_start_effect.value().effect_text().c_str());
        DrawText(effect_text, draw_position.x, draw_position.y, 20, RED);
        draw_position.y += 24;
    }
    if (std::holds_alternative<Source>(skill.content)) {
        const auto &source = std::get<Source>(skill.content);
        for (const auto hit : source.hit_list) {
            const char *hit_range =
                TextFormat("%d - %d", hit.range_min, hit.range_max);
            DrawRectangleLinesEx({draw_position.x, draw_position.y,
                                  float(MeasureText(hit_range, 20) + 12), 26},
                                 2, BLUE);
            draw_position.x += 6;
            draw_position.y += 4;
            DrawText(hit_range, draw_position.x, draw_position.y, 20, RED);
            if (hit.hit_effect.has_value()) {
                const char *effect_text = TextFormat(
                    "Hit: %s", hit.hit_effect.value().effect_text().c_str());
                draw_position.x += MeasureText(hit_range, 20) + 12;
                DrawText(effect_text, draw_position.x, draw_position.y, 20,
                         RED);
                draw_position.x -= MeasureText(hit_range, 20) + 12;
            }
            draw_position.x -= 6;
            draw_position.y += 26;
        }
    } else {
        const auto &modifier = std::get<Modifier>(skill.content);
        const char *value_range =
            TextFormat("%d - %d", modifier.range_min, modifier.range_max);
        DrawRectangleLinesEx({draw_position.x, draw_position.y,
                              float(MeasureText(value_range, 20) + 12), 26},
                             2, BLUE);
        draw_position.x += 6;
        draw_position.y += 4;
        DrawText(value_range, draw_position.x, draw_position.y, 20,
                 modifier.type == Modifier::Type::Block ? GREEN : BLUE);
        draw_position.x -= 6;
        draw_position.y += 26;
        if (modifier.modifier_effect.has_value()) {
            const char *effect_text = TextFormat(
                "%s: %s",
                modifier.type == Modifier::Type::Block ? "Block" : "Evade",
                modifier.modifier_effect.value().effect_text().c_str());
            DrawText(effect_text, draw_position.x, draw_position.y, 20, RED);
            draw_position.y += 26;
        }
    }
}

int main(int argc, char *argv[]) {
    InitWindow(1280, 720, "chlorophyll");

    std::vector<Texture> skill_icon_texture(skill_pool.size());
    for (int i = 0; i < skill_pool.size(); ++i) {
        skill_icon_texture[i] = LoadTexture(skill_pool[i].icon.c_str());
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        const int i = 2;
        draw_card_icon(skill_pool[i], skill_icon_texture[i], {});
        draw_card_details(skill_pool[i], {});
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

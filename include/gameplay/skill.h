#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "./effect.h"

struct Hit {
    int range_min, range_max;
    std::optional<CombatEffect> hit_effect;
};

struct Source {
    std::vector<Hit> hit_list;
};

struct Modifier {
    enum class Type {
        Block,
        Evade,
    };
    int range_min, range_max;
    Type type;
    std::optional<ModifierEffect>
        modifier_effect; // on block if block, and on evade if evade
};

struct Skill {
    std::string name;
    std::string icon; // path to icon
    int cost;
    std::variant<Source, Modifier> content;
    std::optional<CombatEffect> combat_start_effect; // on combat start
    std::optional<CombatEffect> use_effect;          // on use
};

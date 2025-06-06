#pragma once

#include <vector>

#include "effect.h"

struct SourceUnit {
    int health;
    int attack;
    Ability unit_ability;
};

struct ModifierUnit {
    int health;
    Ability first_ability;
    Ability second_ability;
};

struct SummonUnit {
    int health;
    std::optional<int> attack;
};

using Unit = std::variant<SourceUnit, ModifierUnit, SummonUnit>;

inline Unit attack_unit(const Unit &unit, const int value) {
    if (std::holds_alternative<SourceUnit>(unit)) {
        const SourceUnit &source = std::get<SourceUnit>(unit);
        return SourceUnit{
            source.health - value,
            source.attack,
            source.unit_ability,
        };
    } else if (std::holds_alternative<ModifierUnit>(unit)) {
        const ModifierUnit &modifier = std::get<ModifierUnit>(unit);
        return ModifierUnit{
            modifier.health - value,
            modifier.first_ability,
            modifier.second_ability,
        };
    } else {
        const SummonUnit &summon = std::get<SummonUnit>(unit);
        return SummonUnit{
            summon.health - value,
            summon.attack,
        };
    }
}

inline Unit give_unit(const Unit &unit, const int attack, const int health) {
    if (std::holds_alternative<SourceUnit>(unit)) {
        const SourceUnit &source = std::get<SourceUnit>(unit);
        return SourceUnit{
            source.health + health,
            source.attack + attack,
            source.unit_ability,
        };
    } else if (std::holds_alternative<ModifierUnit>(unit)) {
        const ModifierUnit &modifier = std::get<ModifierUnit>(unit);
        return ModifierUnit{
            modifier.health + health,
            modifier.first_ability,
            modifier.second_ability,
        };
    } else {
        const SummonUnit &summon = std::get<SummonUnit>(unit);
        return SummonUnit{
            summon.health - health,
            summon.attack.has_value()
                ? std::optional<int>{summon.attack.value() + attack}
                : std::nullopt,
        };
    }
}

inline int unit_health(const Unit &unit) {
    if (std::holds_alternative<SourceUnit>(unit)) {
        return std::get<SourceUnit>(unit).health;
    } else if (std::holds_alternative<ModifierUnit>(unit)) {
        return std::get<ModifierUnit>(unit).health;
    } else {
        return std::get<SummonUnit>(unit).health;
    }
}

inline int unit_attack(const Unit &unit) {
    if (std::holds_alternative<SourceUnit>(unit)) {
        return std::get<SourceUnit>(unit).attack;
    } else if (std::holds_alternative<ModifierUnit>(unit)) {
        // this is intentional, to throw an error
        return std::get<SourceUnit>(unit).attack;
    } else {
        return std::get<SummonUnit>(unit).attack.value();
    }
}

inline std::vector<Ability> unit_ability(const Unit &unit) {
    if (std::holds_alternative<SourceUnit>(unit)) {
        return {
            std::get<SourceUnit>(unit).unit_ability,
        };
    } else if (std::holds_alternative<ModifierUnit>(unit)) {
        return {
            std::get<ModifierUnit>(unit).first_ability,
            std::get<ModifierUnit>(unit).second_ability,
        };
    } else {
        return {};
    }
}

#pragma once

#include <string>

namespace effect {};

struct CombatEffect {
    std::string effect_text() const { return "place holder for combat effect"; }
}; // used for hit and triggers, does not have input
struct ModifierEffect {
    std::string effect_text() const {
        return "place holder for modifier effect";
    }
}; // used for erm... modifiers

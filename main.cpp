#include <iostream>
#include <vector>

#include <raylib.h>

#include "battle.h"
#include "unit.h"

void print_unit(const Unit &unit) {
    if (std::holds_alternative<SourceUnit>(unit)) {
        const SourceUnit &source = std::get<SourceUnit>(unit);
        std::cout << "([" << source.attack << ", " << source.health << "])";
    } else if (std::holds_alternative<ModifierUnit>(unit)) {
        const ModifierUnit &modifier = std::get<ModifierUnit>(unit);
        std::cout << "([" << modifier.health << "])";
    } else {
        const SummonUnit &summon = std::get<SummonUnit>(unit);
        std::cout << "<[";
        if (summon.attack.has_value()) {
            std::cout << summon.attack.value() << ", ";
        }
        std::cout << summon.health << "]>";
    }
}

void print_battle(const Battle &battle) {
    for (int i = battle.player_team.size() - 1; i >= 0; --i) {
        print_unit(battle.player_team[i]);
        if (i > 0) {
            std::cout << ", ";
        }
    }
    std::cout << " >><< ";
    for (int i = 0; i < battle.enemy_team.size(); ++i) {
        print_unit(battle.enemy_team[i]);
        std::cout << ", ";
    }
    std::cout << "\n";
}

int main(int argc, char *argv[]) { return 0; }

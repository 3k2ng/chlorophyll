#include <iostream>
#include <queue>
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

std::pair<Battle, std::vector<std::pair<Event, Battle>>>
simulate_battle(const Battle &battle) {
    std::vector<std::pair<Event, Battle>> event_record;

    std::queue<Event> event_queue;

    auto add_event_and_process_queue = [&](const Event &event) {
        event_queue.push(event);
        while (!event_queue.empty()) {
            const Event ce = event_queue.front();
            if (event_record.empty()) {
                const auto lb = battle;
                const Battle cb = apply_event(lb, ce);
                event_record.push_back({ce, cb});
                for (const TriggerRecord &tr : find_trigger(cb, ce)) {
                    const auto [effect, source, target] = tr;
                    for (const Event &e :
                         apply_effect(cb, effect, source, target)) {
                        // per event
                        event_queue.push(e);
                    }
                }
            } else {
                const auto [_, lb] = event_record.back();
                const Battle cb = apply_event(lb, ce);
                event_record.push_back({ce, cb});
                for (const TriggerRecord &tr : find_trigger(cb, ce)) {
                    const auto [effect, source, target] = tr;
                    for (const Event &e :
                         apply_effect(cb, effect, source, target)) {
                        // per event
                        event_queue.push(e);
                    }
                }
            }
            event_queue.pop();
        }
    };

    auto clear_faint = [&]() {
        while (true) {
            const std::vector<event::Faint> fes =
                find_faint(event_record.back().second);
            if (fes.empty()) {
                break;
            } else {
                const event::Faint fe = fes.back();
                add_event_and_process_queue(fe);
            }
        }
    };

    // battle start
    add_event_and_process_queue(event::BattleStart{});
    clear_faint();

    while (true) {
        auto [player_source, enemy_source] =
            get_source_location(event_record.back().second);
        if (player_source.has_value() && enemy_source.has_value()) {
            add_event_and_process_queue(
                event::TurnStart{player_source.value()});
            add_event_and_process_queue(event::TurnStart{enemy_source.value()});
            clear_faint();

            add_event_and_process_queue(event::Hurt{
                player_source.value(),
                player_source.value() - 1,
                unit_attack(get_unit(battle, player_source.value())),
            });
            add_event_and_process_queue(event::Hurt{
                enemy_source.value(),
                enemy_source.value() + 1,
                unit_attack(get_unit(battle, enemy_source.value())),
            });
            clear_faint();

            add_event_and_process_queue(event::TurnEnd{player_source.value()});
            add_event_and_process_queue(event::TurnEnd{enemy_source.value()});
            clear_faint();
        } else {
            break;
        }
    }

    return {battle, event_record};
}

int main(int argc, char *argv[]) {
    Battle battle = {
        {
            SourceUnit{
                4,
                1,
                {
                    TriggerType::BattleStart,
                    effect::Deal{TargetingType::ForwardUnit, 1},
                },
            },
        },
        {
            SourceUnit{
                3,
                1,
                {
                    TriggerType::BattleStart,
                    effect::Deal{TargetingType::ForwardUnit, 1},
                },
            },
        },
    };
    print_battle(battle);
    const auto record = simulate_battle(battle);
    for (const auto r : record.second) {
        print_battle(r.second);
    }
    return 0;
}

#pragma once

#include <vector>

#include <raylib.h>

#include "unit.h"

namespace event {
struct BattleStart {};
struct BattleEnd {};
struct TurnStart {
    // location of the source whose turn it is
    int target;
};
struct TurnEnd {
    // location of the source whose turn it is
    int target;
};
struct Hurt {
    // location of the originator of the attack
    int source;
    // location of the unit getting hurt, before any relocation happen
    int target;
    // attack value
    int attack;
};
struct Faint {
    // location of the unit faining, before any relocation happen
    int target;
};
struct Summon {
    // location of the summon, after all relocation
    int target;
    // unit to summon
    SummonUnit unit;
};
struct Give {
    // location of the originator of the buff
    int source;
    // location of the unit buffed, before any relocation happen
    int target;
    // resource given
    int attack, health;
};
struct ShopStart {};
struct ShopEnd {};
struct LevelUp {
    // location of the unit leveling up
    int target;
};
struct Buy {
    // location of the unit after being bought
    int target;
};
struct Sell {
    // location of the unit befor being sold
    int target;
};
}; // namespace event
using Event =
    std::variant<event::BattleStart, event::BattleEnd, event::TurnStart,
                 event::TurnEnd, event::Hurt, event::Faint, event::Summon,
                 event::Give, event::ShopStart, event::ShopEnd, event::LevelUp,
                 event::Buy, event::Sell>;

struct Battle {
    std::vector<Unit> player_team, enemy_team;
};

inline Unit get_unit(const Battle &battle, const int location) {
    if (location < 0) {
        return battle.enemy_team[-1 - location];
    }
    return battle.player_team[location];
}

inline Battle set_unit(const Battle &battle, const int location,
                       const Unit &unit) {
    Battle updated_battle = battle;
    if (location < 0) {
        updated_battle.enemy_team[-1 - location] = unit;
    } else {
        updated_battle.player_team[location] = unit;
    }
    return updated_battle;
}

inline std::pair<std::optional<int>, std::optional<int>>
get_source_location(const Battle &battle) {
    int player_source = -1, enemy_source = -1;
    for (int i = 0; i < battle.player_team.size(); ++i) {
        if (std::holds_alternative<SourceUnit>(battle.player_team[i]) ||
            std::holds_alternative<SummonUnit>(battle.player_team[i])) {
            player_source = i;
            break;
        }
    }
    for (int i = 0; i < battle.enemy_team.size(); ++i) {
        if (std::holds_alternative<SourceUnit>(battle.enemy_team[i]) ||
            std::holds_alternative<SummonUnit>(battle.enemy_team[i])) {
            enemy_source = i;
            break;
        }
    }
    std::pair<std::optional<int>, std::optional<int>> source_location;
    if (player_source >= 0) {
        source_location.first = player_source;
    }
    if (enemy_source >= 0) {
        source_location.second = -1 - enemy_source;
    }
    return source_location;
}

inline std::vector<int> find_target(const Battle &battle,
                                    const TargetingType tt, const int source,
                                    const int target) {
    const bool is_player = source >= 0;
    const int all_max = battle.player_team.size() - 1;
    const int all_min = -battle.enemy_team.size();
    const int ally_min = is_player ? 0 : all_min;
    const int ally_max = is_player ? all_max : -1;
    const int oppo_min = is_player ? all_min : 0;
    const int oppo_max = is_player ? -1 : all_max;
    const int player_back = all_max;
    const int enemy_back = all_min;
    switch (tt) {
    case TargetingType::Self: {
        return {source};
    }
    case TargetingType::TriggerTarget: {
        return {target};
    }
    case TargetingType::AllUnit: {
        std::vector<int> target_list;
        for (int i = all_min; i <= all_max; ++i) {
            target_list.push_back(i);
        }
        return target_list;
    }
    case TargetingType::AllAlly: {
        std::vector<int> target_list;
        for (int i = ally_min; i <= ally_max; ++i) {
            target_list.push_back(i);
        }
        return target_list;
    }
    case TargetingType::AllEnemy: {
        std::vector<int> target_list;
        for (int i = oppo_min; i <= oppo_max; ++i) {
            target_list.push_back(i);
        }
        return target_list;
    }
    case TargetingType::RandomUnit: {
        return {GetRandomValue(all_min, all_max)};
    }
    case TargetingType::RandomAlly: {
        return {GetRandomValue(ally_min, ally_max)};
    }
    case TargetingType::RandomEnemy: {
        return {GetRandomValue(oppo_min, oppo_max)};
    }
    case TargetingType::AdjacentUnit: {
        std::vector<int> target_list;
        for (const int i : {source - 1, source, source + 1}) {
            if (all_min <= i && i <= all_max) {
                target_list.push_back(i);
            }
        }
        return target_list;
    }
    case TargetingType::AllyBack: {
        if (is_player) {
            return {player_back};
        }
        return {enemy_back};
    }
    case TargetingType::EnemyBack: {
        if (is_player) {
            return {enemy_back};
        }
        return {player_back};
    }
    case TargetingType::ForwardUnit: {
        if (is_player) {
            return {source - 1};
        }
        return {source + 1};
    }
    case TargetingType::BackwardUnit: {
        if (is_player) {
            return {source + 1};
        }
        return {source - 1};
    }
    case TargetingType::TwoRandomUnit: {
        return {
            GetRandomValue(all_min, all_max),
            GetRandomValue(all_min, all_max),
        };
    }
    case TargetingType::TwoRandomAlly: {
        return {
            GetRandomValue(ally_min, ally_max),
            GetRandomValue(ally_min, ally_max),
        };
    }
    case TargetingType::TwoRandomEnemy: {
        return {
            GetRandomValue(oppo_min, oppo_max),
            GetRandomValue(oppo_min, oppo_max),
        };
    }
    case TargetingType::TwoForwardUnit: {
        std::vector<int> target_list;
        for (const int i :
             {source - (is_player ? 1 : -1), source - (is_player ? 2 : -2)}) {
            if (all_min <= i && i <= all_max) {
                target_list.push_back(i);
            }
        }
        return target_list;
    }
    case TargetingType::TwoBackwardUnit: {
        std::vector<int> target_list;
        for (const int i :
             {source + (is_player ? 1 : -1), source + (is_player ? 2 : -2)}) {
            if (all_min <= i && i <= all_max) {
                target_list.push_back(i);
            }
        }
        return target_list;
    }
    }
}

inline std::vector<event::Faint> find_faint(const Battle &battle) {
    std::vector<event::Faint> faint_event;
    for (int i = 0; i < battle.player_team.size(); ++i) {
        if (unit_health(battle.player_team[i]) <= 0) {
            faint_event.push_back({i});
        }
    }
    for (int i = 0; i < battle.enemy_team.size(); ++i) {
        if (unit_health(battle.enemy_team[i]) <= 0) {
            faint_event.push_back({-1 - i});
        }
    }
    return faint_event;
}

inline Battle insert_unit(const Battle &battle, const int location,
                          const Unit unit) {
    std::vector<Unit> updated_player_team, updated_enemy_team;
    for (int i = 0; i < battle.player_team.size(); ++i) {
        if (i == location) {
            updated_player_team.push_back(unit);
        }
        updated_player_team.push_back(get_unit(battle, i));
    }
    for (int i = -1; i >= -battle.enemy_team.size(); --i) {
        if (i == location) {
            updated_enemy_team.push_back(unit);
        }
        updated_enemy_team.push_back(get_unit(battle, i));
    }
    return {updated_player_team, updated_enemy_team};
}

inline Battle remove_unit(const Battle &battle, const int location) {
    std::vector<Unit> updated_player_team, updated_enemy_team;
    for (int i = 0; i < battle.player_team.size(); ++i) {
        if (i != location) {
            updated_player_team.push_back(get_unit(battle, i));
        }
    }
    for (int i = -1; i >= -battle.enemy_team.size(); --i) {
        if (i != location) {
            updated_enemy_team.push_back(get_unit(battle, i));
        }
    }
    return {updated_player_team, updated_enemy_team};
}

inline std::vector<event::Hurt> turn_action(const Battle &battle) {
    auto [player_source, enemy_source] = get_source_location(battle);
    if (player_source.has_value() && enemy_source.has_value()) {
        std::vector<event::Hurt> hurt_event;
        const int player_target = player_source.value() - 1;
        const int enemy_target = enemy_source.value() + 1;
        return {
            event::Hurt{player_source.value(), player_target,
                        unit_attack(get_unit(battle, player_source.value()))},
            event::Hurt{enemy_source.value(), enemy_target,
                        unit_attack(get_unit(battle, enemy_source.value()))},
        };
    }
    return {};
}

using TriggerRecord = std::tuple<Effect, int, int>;

inline std::vector<TriggerRecord> find_trigger(const Battle &battle,
                                               const Event &event) {
    std::vector<std::pair<Ability, int>> to_verify;
    for (int i = -battle.enemy_team.size(); i < battle.player_team.size();
         ++i) {
        const Unit &unit = get_unit(battle, i);
        if (std::holds_alternative<SourceUnit>(unit)) {
            const SourceUnit &source = std::get<SourceUnit>(unit);
            to_verify.emplace_back(source.unit_ability, i);
        } else if (std::holds_alternative<ModifierUnit>(unit)) {
            const ModifierUnit &modifier = std::get<ModifierUnit>(unit);
            to_verify.emplace_back(modifier.first_ability, i);
            to_verify.emplace_back(modifier.second_ability, i);
        }
    }
    std::vector<TriggerRecord> trigger_record;
    for (const auto [skill, i] : to_verify) {
        switch (skill.first) {
        case TriggerType::BattleStart:
            if (std::holds_alternative<event::BattleStart>(event)) {
                trigger_record.emplace_back(skill.second, i, i);
            }
            break;
        case TriggerType::BattleEnd:
            if (std::holds_alternative<event::BattleEnd>(event)) {
                trigger_record.emplace_back(skill.second, i, i);
            }
            break;
        case TriggerType::SelfTurnStart:
            if (std::holds_alternative<event::TurnStart>(event) &&
                std::get<event::TurnStart>(event).target == i) {
                trigger_record.emplace_back(skill.second, i, i);
            }
            break;
        case TriggerType::SelfTurnEnd:
            if (std::holds_alternative<event::TurnEnd>(event) &&
                std::get<event::TurnEnd>(event).target == i) {
                trigger_record.emplace_back(skill.second, i, i);
            }
            break;
        case TriggerType::AllyTurnStart:
            if (std::holds_alternative<event::TurnStart>(event)) {
                int j = std::get<event::TurnStart>(event).target;
                if ((i >= 0 && j >= 0) || (i < 0 && j < 0)) {
                    trigger_record.emplace_back(skill.second, i, j);
                }
            }
            break;
        case TriggerType::AllyTurnEnd:
            if (std::holds_alternative<event::TurnEnd>(event)) {
                int j = std::get<event::TurnEnd>(event).target;
                if ((i >= 0 && j >= 0) || (i < 0 && j < 0)) {
                    trigger_record.emplace_back(skill.second, i, j);
                }
            }
            break;
        case TriggerType::EnemyTurnStart:
            if (std::holds_alternative<event::TurnStart>(event)) {
                int j = std::get<event::TurnStart>(event).target;
                if ((i >= 0 && j < 0) || (i < 0 && j >= 0)) {
                    trigger_record.emplace_back(skill.second, i, j);
                }
            }
            break;
        case TriggerType::EnemyTurnEnd:
            if (std::holds_alternative<event::TurnEnd>(event)) {
                int j = std::get<event::TurnEnd>(event).target;
                if ((i >= 0 && j < 0) || (i < 0 && j >= 0)) {
                    trigger_record.emplace_back(skill.second, i, j);
                }
            }
            break;
        case TriggerType::SelfHurt:
            if (std::holds_alternative<event::Hurt>(event) &&
                std::get<event::Hurt>(event).target == i) {
                trigger_record.emplace_back(skill.second, i, i);
            }
            break;
        case TriggerType::SelfFaint:
            if (std::holds_alternative<event::Faint>(event) &&
                std::get<event::Faint>(event).target == i) {
                trigger_record.emplace_back(skill.second, i, i);
            }
            break;
        case TriggerType::AllyHurt:
            if (std::holds_alternative<event::Hurt>(event)) {
                int j = std::get<event::Hurt>(event).target;
                if ((i >= 0 && j >= 0) || (i < 0 && j < 0)) {
                    trigger_record.emplace_back(skill.second, i, j);
                }
            }
            break;
        case TriggerType::AllyFaint:
            if (std::holds_alternative<event::Faint>(event)) {
                int j = std::get<event::Faint>(event).target;
                if ((i >= 0 && j >= 0) || (i < 0 && j < 0)) {
                    trigger_record.emplace_back(skill.second, i, j);
                }
            }
            break;
        case TriggerType::EnemyHurt:
            if (std::holds_alternative<event::Hurt>(event)) {
                int j = std::get<event::Hurt>(event).target;
                if ((i >= 0 && j < 0) || (i < 0 && j >= 0)) {
                    trigger_record.emplace_back(skill.second, i, j);
                }
            }
            break;
        case TriggerType::EnemyFaint:
            if (std::holds_alternative<event::Faint>(event)) {
                int j = std::get<event::Faint>(event).target;
                if ((i >= 0 && j < 0) || (i < 0 && j >= 0)) {
                    trigger_record.emplace_back(skill.second, i, j);
                }
            }
            break;
        case TriggerType::AllySummon:
            if (std::holds_alternative<event::Summon>(event)) {
                int j = std::get<event::Summon>(event).target;
                if ((i >= 0 && j >= 0) || (i < 0 && j < 0)) {
                    trigger_record.emplace_back(skill.second, i, j);
                }
            }
            break;
        case TriggerType::EnemySummon:
            if (std::holds_alternative<event::Summon>(event)) {
                int j = std::get<event::Summon>(event).target;
                if ((i >= 0 && j < 0) || (i < 0 && j >= 0)) {
                    trigger_record.emplace_back(skill.second, i, j);
                }
            }
            break;

            // not battle event
        case TriggerType::ShopStart:
        case TriggerType::ShopEnd:
        case TriggerType::LevelUp:
        case TriggerType::Buy:
        case TriggerType::Sell:
            break;
        }
    }
    return trigger_record;
}

inline Battle apply_event(const Battle &battle, const Event &event) {
    if (std::holds_alternative<event::BattleStart>(event)) {
        // nothing happens
    } else if (std::holds_alternative<event::BattleEnd>(event)) {
        // nothing happens
    } else if (std::holds_alternative<event::TurnStart>(event)) {
        // nothing happens
    } else if (std::holds_alternative<event::TurnEnd>(event)) {
        // nothing happens
    } else if (std::holds_alternative<event::Hurt>(event)) {
        // apply damage to the unit
        const int source = std::get<event::Hurt>(event).source;
        const int target = std::get<event::Hurt>(event).target;
        const int attack = std::get<event::Hurt>(event).attack;
        return set_unit(battle, target,
                        attack_unit(get_unit(battle, target), attack));
    } else if (std::holds_alternative<event::Faint>(event)) {
        // remove unit from the battle
        const int target = std::get<event::Faint>(event).target;
        return remove_unit(battle, target);
    } else if (std::holds_alternative<event::Summon>(event)) {
        // insert unit into the battle
        const int target = std::get<event::Summon>(event).target;
        const SummonUnit unit = std::get<event::Summon>(event).unit;
        return insert_unit(battle, target, unit);
    } else if (std::holds_alternative<event::Give>(event)) {
        // add to unit stats
        const int source = std::get<event::Give>(event).source;
        const int target = std::get<event::Give>(event).target;
        const int attack = std::get<event::Give>(event).attack;
        const int health = std::get<event::Give>(event).health;
        return set_unit(battle, target,
                        give_unit(get_unit(battle, target), attack, health));
    }
    // not battle event
    else if (std::holds_alternative<event::ShopStart>(event)) {
    } else if (std::holds_alternative<event::ShopEnd>(event)) {
    } else if (std::holds_alternative<event::LevelUp>(event)) {
    } else if (std::holds_alternative<event::Buy>(event)) {
    } else if (std::holds_alternative<event::Sell>(event)) {
    }
    return battle;
}

inline int infer_value(const Battle &battle, const Value &value,
                       const int source, const int target) {
    if (std::holds_alternative<value::Variable>(value)) {
        switch (std::get<value::Variable>(value)) {
        case value::Variable::SelfAttack:
            return unit_attack(get_unit(battle, source));
        case value::Variable::HalfSelfAttack:
            return unit_attack(get_unit(battle, source)) / 2;
        case value::Variable::SelfHealth:
            return unit_health(get_unit(battle, source));
        case value::Variable::HalfSelfHealth:
            return unit_health(get_unit(battle, source)) / 2;
        case value::Variable::TargetHealth:
            return unit_health(get_unit(battle, target));
        case value::Variable::HalfTargetHealth:
            return unit_health(get_unit(battle, target)) / 2;
        }
    }
    return std::get<int>(value);
}

inline std::vector<Event> apply_effect(const Battle &battle,
                                       const Effect &effect, const int source,
                                       const int target) {
    if (std::holds_alternative<effect::Deal>(effect)) {
        const auto &deal = std::get<effect::Deal>(effect);
        std::vector<Event> e;
        for (const int i : find_target(battle, deal.tt, source, target)) {
            e.push_back(event::Hurt{
                source, i, infer_value(battle, deal.damage_deal, source, i)});
        }
        return e;
    } else if (std::holds_alternative<effect::Give>(effect)) {
        const auto &give = std::get<effect::Give>(effect);
        std::vector<Event> e;
        for (const int i : find_target(battle, give.tt, source, target)) {
            e.push_back(event::Give{
                source,
                i,
                infer_value(battle, give.health_give, source, i),
                infer_value(battle, give.attack_give, source, i),
            });
        }
        return e;
    } else if (std::holds_alternative<effect::Summon>(effect)) {
        const auto &summon = std::get<effect::Summon>(effect);
        return {
            event::Summon{
                source,
                SummonUnit{
                    infer_value(battle, summon.summon_health, source, target),
                    summon.summon_attack.has_value()
                        ? std::optional<int>{infer_value(
                              battle, summon.summon_attack.value(), source,
                              target)}
                        : std::nullopt,
                }},
        };
    } else if (std::holds_alternative<effect::Trigger>(effect)) {
        const auto &trigger = std::get<effect::Trigger>(effect);
        std::vector<Event> e;
        for (const int i : find_target(battle, trigger.tt, source, target)) {
            for (const auto &skill : unit_ability(get_unit(battle, i))) {
                for (const auto &te :
                     apply_effect(battle, skill.second, i, source)) {
                    e.push_back(te);
                }
            }
        }
        return e;
    }
    return {};
}

#pragma once

#include <optional>
#include <utility>
#include <variant>

enum class TargetingType : char {
    Self,
    TriggerTarget,
    AllUnit,
    AllAlly,
    AllEnemy,
    RandomUnit,
    RandomAlly,
    RandomEnemy,
    AdjacentUnit,
    AllyBack,
    EnemyBack,
    ForwardUnit,
    BackwardUnit,
    TwoRandomUnit,
    TwoRandomAlly,
    TwoRandomEnemy,
    TwoForwardUnit,
    TwoBackwardUnit,
};

namespace value {
enum class Variable : char {
    SelfAttack,
    HalfSelfAttack,
    SelfHealth,
    HalfSelfHealth,
    TargetHealth,
    HalfTargetHealth,
};
using Number = int;
}; // namespace value
using Value = std::variant<value::Variable, value::Number>;

namespace effect {
struct Deal {
    TargetingType tt;
    Value damage_deal;
};
struct Give {
    TargetingType tt;
    Value health_give, attack_give;
};
struct Summon {
    std::optional<Value> summon_attack;
    Value summon_health;
};
struct Trigger {
    TargetingType tt;
};
struct Cash {
    Value amount;
};
} // namespace effect
using Effect = std::variant<effect::Deal, effect::Give, effect::Summon,
                            effect::Trigger, effect::Cash>;

enum class TriggerType {
    BattleStart,
    SelfTurnStart,
    SelfTurnEnd,
    AllyTurnStart,
    AllyTurnEnd,
    EnemyTurnStart,
    EnemyTurnEnd,
    SelfHurt,
    SelfFaint,
    AllyHurt,
    AllyFaint,
    EnemyHurt,
    EnemyFaint,
    AllySummon,
    EnemySummon,
    ShopStart,
    LevelUp,
    Buy,
    Sell,
};

using Ability = std::pair<TriggerType, Effect>;

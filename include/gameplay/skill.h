#pragma once

#include "tool/range.h"
#include <memory>

class Effect {
  public:
  private:
};

// small action with a range
struct Action {
    Range value_range;
    std::unique_ptr<Effect> on_hit;
};

class Skill {
  public:
  private:
};

class Source {
  public:
  private:
};

class Modifier {
  public:
  private:
};

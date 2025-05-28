#pragma once
#include <raylib.h>

struct Range {
    int min, max;

    int roll() const { return GetRandomValue(min, max); }
};

#pragma once

#include <stdint.h>
#include <random>

#include "Position.h"
#include "Direction.h"

namespace AntBattle {

namespace Math {

uint32_t random(uint32_t min, uint32_t max);

#ifndef WIN32
std::mt19937& randGenerator();
#else
class SimpleRandomClass
{
public:
  typedef size_t result_type;
  static size_t min() { return 0; }
  static size_t max() { return RAND_MAX; }
  size_t operator()() { return random(min(), max()); }
};

SimpleRandomClass& randGenerator();
#endif

Direction randDirection();

Direction normalizeDirection(int val);

Direction directionTo(const Position& posFrom, const Position& posTo);

Position randomPositionByDirection(const Direction& dir, uint32_t len);

Position positionOffset(const Direction& dir);

std::vector<Direction> createDirectionArray(const Direction& dir);
Direction probabilisticDirection(const Direction& dir);
Direction inverseDirection(const Direction& dir);
std::vector<Position> visibleCells(const Position& pos, uint32_t visibility);

std::string descriptionDirection(const Direction& dir, bool shortname = false);

};

};

#pragma once

#include <stdint.h>
#include <random>

#include "Position.h"
#include "Direction.h"

namespace AntBattle {

namespace Math {

std::random_device& randDevice();
std::mt19937& randGenerator();

Direction randDirection();
Direction normalizeDirection(int val);
Direction directionTo(const Position& posFrom, const Position& posTo);
Position positionOffset(const Direction& dir);
std::vector<Direction> createDirectionArray(const Direction& dir);
Direction probabilisticDirection(const Direction& dir);
Direction inverseDirection(const Direction& dir);
std::vector<Position> visibleCells(const Position& pos, uint32_t visibility);

};

};

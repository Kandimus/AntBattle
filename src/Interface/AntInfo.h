#pragma once

#include <stdint.h>
#include <string>

#include "ApiVersion.h"
#include "Direction.h"

namespace AntBattle {

struct AntInfo
{
	uint32_t iteration = 0;
	uint32_t countOfWorker = 0;
	uint32_t countOfSolder = 0;
	uint32_t countOfFood = 0;

	double healthPrecent = 0.0;
	double satietyPrecent = 0.0;
	uint32_t cargo = 0;
	uint32_t distanceToQueen = 0;
	Direction directionToLabel = Direction::Nord;
	uint32_t distanceToLabel = 0;
	uint32_t countOfVisibleAlly = 0;
	uint32_t countOfVisibleEnemies = 0;
	uint32_t countOfVisibleFood = 0;
	Direction directionToNearEnemy = Direction::Nord;
	Direction directionToNearFood = Direction::Nord;

	//TODO lastCommand;
	//TODO lastCmdStatus;
};

};

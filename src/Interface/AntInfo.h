#pragma once

#include <stdint.h>
#include <string>

namespace AntBattle {

enum AntClass {
	Worker = 0,
	Solder,
	Queen,
};

struct AntInfo
{
	uint32_t iteration;
	uint32_t countOfWorker;
	uint32_t countOfSolder;
	uint32_t countOfFood;

	AntClass antClass;
	uint32_t satiety;
	uint32_t health;
	uint32_t satietyPrecent;
	uint32_t cargo;
	uint32_t distanceToQueen;
	uint32_t countOfVisibleAlly;
	uint32_t countOfVisibleEnemies;
	uint32_t countOfVisibleFood;
	uint32_t directionToNearEnemy;
	uint32_t directionToNearFood;
};

};

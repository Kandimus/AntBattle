#pragma once

#include <stdint.h>
#include "Direction.h"

namespace AntBattle {


enum CommandType
{
	Idle = 0,
	Explore,
	Move,
	Eat,
	MoveToFood,
	Attack,
	MoveAndAttack,
	Feed,
	FeedQueen,
	Defense,
	CreateOfWorker,
	CreateOfSolder,
};

struct Command
{
	CommandType type;
	Direction direction;
	uint32_t count;
};

};

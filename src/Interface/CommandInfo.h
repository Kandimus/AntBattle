#pragma once

#include "Direction.h"

namespace AntBattle {


enum CommandType
{
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
}

struct Command
{
	CommandType type;
	Direction direction;
	uint32_t count;
};

};

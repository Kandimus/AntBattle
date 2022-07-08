#pragma once

#include <stdint.h>
#include <string>

namespace AntBattle {

typedef std::function<void(int)> AntProcess;

enum AntType {
	Black = 0,
	Red,
};

struct PlayerInfo
{
	AntProcess processWorker;
	AntProcess processSolder;
	AntProcess processQueen;

	AntType type;
	std::string TeamName = "";
};

};

#pragma once

#include <stdint.h>
#include <string>
#include <functional>

#include "ApiVersion.h"

namespace AntBattle {

enum AntType {
	Black = 0,
	Red,
};

struct PlayerInfo
{
	AntType type;
	std::string teamName = "";
	std::string version = "";
	ApiVersion apiVersion = 0;
};

typedef void(*AntInit)(PlayerInfo*);
typedef void(*AntFinalize)(void);
typedef void(*AntProcess)(int);
//typedef std::function<void(PlayerInfo*)> AntInit;
//typedef std::function<void()> AntFinalize;
//typedef std::function<void(int)> AntProcess;

};

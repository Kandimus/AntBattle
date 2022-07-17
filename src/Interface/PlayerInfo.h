#pragma once

#include <stdint.h>
#include <string>
#include <functional>

#include "ApiVersion.h"
#include "Command.h"
#include "AntInfo.h"

namespace AntBattle {

struct PlayerInfo
{
	AntType type;
	std::string teamName = "";
	std::string version = "";
	ApiVersion apiVersion = 0;
};

typedef void(*AntInit)(PlayerInfo*);
typedef void(*AntFinalize)(void);
typedef void(*AntProcess)(AntInfo*, Command*);

};

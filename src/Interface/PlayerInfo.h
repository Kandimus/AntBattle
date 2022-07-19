#pragma once

#include <stdint.h>
#include <string>

#include "ApiVersion.h"
#include "AntInfo.h"

namespace AntBattle {

struct PlayerInfo
{
	AntType type;
	std::string teamName = "";
	std::string version = "";
	ApiVersion apiVersion = 0;
};

};

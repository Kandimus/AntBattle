#pragma once

#include <stdint.h>

#include "Command.h"
#include "AntInfo.h"
#include "PlayerInfo.h"

namespace AntBattle {

typedef void(*AntInit)(PlayerInfo*);
typedef void(*AntFinalize)(void);
typedef void(*AntProcess)(AntInfo*, Command*);

};

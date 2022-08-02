#pragma once

#include <stdint.h>

namespace AntBattle {

typedef uint32_t ApiVersion;

const ApiVersion ANTBATTLE_API_VERSION = 0x00010000;

enum AntType {
	Black = 0,
	Red,
};

enum AntClass {
	Worker = 0,
	Solder,
	Queen,
};


};

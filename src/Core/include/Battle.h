#pragma once

#include <stdint.h>
#include <memory>

namespace AntBattle {

class Map;

class Battle
{
public:
	Battle(uint32_t width, uint32_t height);
	virtual ~Battle() = default;

protected:
	std::unique_ptr<Map> m_map;
};

};

#pragma once

#include <stdint.h>
#include <memory>
#include "Map.h"

namespace AntBattle {

//class Map;
class Config;


class Battle
{
public:
	Battle(const std::string& confname);
	virtual ~Battle() = default;

protected:
	uint64_t m_uid;
	std::shared_ptr<Config> m_conf;
	std::unique_ptr<Map> m_map;
};

};

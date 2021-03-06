#pragma once

#include <string>
#include "json.hpp"

namespace AntBattle {

class Config
{
public:
	Config(const std::string& filename);
	virtual ~Config() = default;

	// map
	uint32_t width();
	uint32_t height();
	double stoneOnMap();
	double foodOnMap();
	bool   isBordered();

	uint32_t workerCountOfStart();
	uint32_t solderCountOfStart();

protected:
	std::string m_filename;
	nlohmann::json m_conf;
};

};

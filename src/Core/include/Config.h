#pragma once

#include <string>
#include "json.hpp"

namespace AntBattle {

class Config
{
public:
	Config(const std::string& filename);
	virtual ~Config() = default;

	uint32_t width();
	uint32_t height();
	double stoneOnMap();
	double foodOnMap();

protected:
	std::string m_filename;
	nlohmann::json m_conf;
};

};

#pragma once

#include <string>

namespace AntBattle {

class Config
{
public:
	Config(const std::string& filename);
	virtual ~Config() = default;

	uint32_t stonePercent();
	uint32_t foodPercent();

protected:
	std::string m_filename;
};

};

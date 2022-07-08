#include "Config.h"
#include <fstream>

using namespace AntBattle;

Config::Config(const std::string& filename) : m_filename(filename)
{
	std::ifstream file(filename);

	if (!file.is_open()) {
		return;
	}

	file >> m_conf;
}

uint32_t Config::width()
{
	return m_conf["map"]["width"];
}

uint32_t Config::height()
{
	return m_conf["map"]["height"];
}

double Config::stoneOnMap()
{
	return m_conf["map"]["stone"];
}

double Config::foodOnMap()
{
	return m_conf["map"]["food"];
}

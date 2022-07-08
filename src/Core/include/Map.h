#pragma once

#include <stdint.h>
#include <memory>
#include <vector>

#include "Position.h"

namespace AntBattle {

class Cell;
class Config;

class Map
{
public:
	Map(const std::weak_ptr<Config>& conf);
	virtual ~Map() = default;

	std::weak_ptr<Cell> cell(Position pos) const;
	std::weak_ptr<Cell> cell(uint32_t x, uint32_t y) const;

	const Position& size() const;
	uint32_t sizeX() const;
	uint32_t sizeY() const;

protected:
	void incPosition(Position& pos, uint32_t x = 1) const;
	uint32_t absPosition(Position& pos) const;
	void generate();

protected:
	Position m_size;
	std::vector<std::shared_ptr<Cell>> m_map;
	std::weak_ptr<Config> m_conf;
};

};

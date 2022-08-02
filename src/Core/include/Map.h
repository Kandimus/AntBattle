#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <list>

#include "Position.h"
#include "ApiVersion.h"

namespace AntBattle {

class Ant;
class Cell;
class Config;
class Player;

using ListSharedAnt = std::list<std::shared_ptr<Ant>>;

class Map
{
public:
	Map(const std::weak_ptr<Config>& conf);
	virtual ~Map() = default;

	std::weak_ptr<Cell> cell(Position pos) const;
	std::weak_ptr<Cell> cell(uint32_t x, uint32_t y) const;
	std::weak_ptr<Cell> cell(uint32_t offset) const;

	const Position& size() const;
	uint32_t sizeX() const;
	uint32_t sizeY() const;

	ListSharedAnt generate(const std::vector<std::shared_ptr<Player>>& players);

	bool isCellEmpty(const Position& pos) const;
	bool isCellEmpty(int32_t x, int32_t y) const;
	void clearChanged();

	Position nearAvaliblePosition(const Position& pos) const;

	void moveAnt(const std::weak_ptr<Ant>& ant, const Position& pos);
	void removeAnt(const Position& pos);
	std::shared_ptr<Ant> createAnt(std::weak_ptr<Player> player, AntClass ant_class, const Position& pos, uint32_t r);

protected:
	void incPosition(Position& pos, uint32_t x = 1) const;
	int32_t absPosition(const Position& pos) const;

protected:
	Position m_size;
	std::vector<std::shared_ptr<Cell>> m_map;
	std::weak_ptr<Config> m_conf;

	const uint32_t m_startingSquare = 10;
	const uint32_t m_minWidth = 50;
	const uint32_t m_minHeight = 50;
};

};

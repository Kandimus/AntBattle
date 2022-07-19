#include "Map.h"

#include "Log.h"
#include "Math.h"
#include "Cell.h"
#include "Player.h"
#include "Config.h"

#include "AntProvider.h"

using namespace AntBattle;

using SharedAnt = std::shared_ptr<Ant>;
using VectorSharedAnt = std::vector<std::shared_ptr<Ant>>;

Map::Map(const std::weak_ptr<Config>& conf)
	: m_conf(conf)
{
	uint32_t w = 0;
	uint32_t h = 0;

	{
		auto cfg = m_conf.lock();
		w   = cfg->width();
		h   = cfg->height();
	}

	if (w < m_minWidth) {
		w = m_minWidth;
		Log::instance().put(format("Set width = %i", w));
	}

	if (h < m_minHeight) {
		h = m_minHeight;
		Log::instance().put(format("Set height = %i", h));
	}

	m_size.init(w, h);
	m_map.resize(m_size.x() * m_size.y());

	Position pos;
	for (auto& cell : m_map) {
		cell = std::make_shared<Cell>(pos);
		incPosition(pos);
	}

	Log::instance().put(format("Create map [%i x %i]", m_size.x(), m_size.y()));
}

VectorSharedAnt Map::generate(const std::vector<std::shared_ptr<Player>>& players)
{
	VectorSharedAnt ants;
	auto conf = m_conf.lock();

	// countCell = m_map.size();

	//TODO generate stone

	//TODO generate food

	//TODO generate ants
	for (auto& player : players) {
		Position posQueen(Math::random(0, m_size.x()), Math::random(0, m_size.y()));

		posQueen = nearAvaliblePosition(posQueen);

		// do place Queen
		auto queen = createAnt(player, AntClass::Queen, posQueen, 1);
		ants.push_back(queen);

		// do place workers
		for (int ii = 0; ii < conf->workerCountOfStart(); ++ii) {
			auto worker = createAnt(player, AntClass::Worker, posQueen, m_startingSquare);
			ants.push_back(worker);
		}

		// do place workers
		for (int ii = 0; ii < conf->solderCountOfStart(); ++ii) {
			auto solder = createAnt(player, AntClass::Solder, posQueen, m_startingSquare);
			ants.push_back(solder);
		}
	}

	return ants;
}

std::shared_ptr<Ant> Map::createAnt(std::weak_ptr<Player> player, AntClass ant_class, const Position& pos, uint32_t r)
{
	Position calc_pos(Math::random(0, r * 2), Math::random(0, r * 2));
	calc_pos -= r;
	calc_pos += pos;
	calc_pos = nearAvaliblePosition(calc_pos);

	auto ant = Provide::newAnt(player, ant_class);

	ant->setPosition(calc_pos);
	ant->reset();

	m_map[absPosition(calc_pos)]->setAnt(ant);

	return ant;
}

bool Map::isCellEmpty(const Position& pos) const
{
	int32_t idx = absPosition(pos);

	//TODO need support on unboarding map
	return idx >= 0 && idx < m_map.size() ? m_map[idx]->isEmpty() : false;
}

bool Map::isCellEmpty(int32_t x, int32_t y) const
{
	return isCellEmpty(Position(x, y));
}

void Map::clearChanged()
{
	for (auto& cell : m_map) {
		cell->clearChanged();
	}
}

Position Map::nearAvaliblePosition(const Position& pos) const
{
	Position curPos = pos;
	uint32_t len = 1;
	uint32_t count = 0;
	Direction dir = Math::randDirection();
	Position addPos = Math::positionOffset(dir);

	if (isCellEmpty(curPos)) {
		return pos;
	}

	curPos += addPos;
	++count;

	// to rotate at clockwise to angle 90
	if (static_cast<int>(dir) % 2) {
		dir = Math::normalizeDirection(static_cast<int>(dir) + 1);
	}

	// to rotate at clockwise to next angle 90 and get new offset
	dir = Math::normalizeDirection(static_cast<int>(dir) + 2);
	addPos = Math::positionOffset(dir);

	while(!isCellEmpty(curPos)) {
		if (count >= 8 * len) {
			++len;
			// to rotate at counter-clockwise
			Direction cc_dir = Math::normalizeDirection(static_cast<int>(dir) - 2);
			curPos += Math::positionOffset(cc_dir);
			count = 0;
		}

		if (std::abs(curPos.x() + addPos.x()) > len || std::abs(curPos.y() + addPos.y()) > len) {
			dir = Math::normalizeDirection(static_cast<int>(dir) + 2);
			addPos = Math::positionOffset(dir);
		}

		++count;
		curPos += addPos;
	}

	return curPos;
}

void Map::moveAnt(const std::weak_ptr<Ant>& ant, const Position& pos)
{
	SharedAnt pAnt = ant.lock();
	int32_t old_idx = absPosition(pAnt->position());
	int32_t new_idx = absPosition(pos);

	m_map[new_idx]->setAnt(ant);
	m_map[old_idx]->removeAnt();
	pAnt->setPosition(pos);
}

void Map::incPosition(Position& pos, uint32_t x) const
{
	pos.addX(x);

	if (pos.x() >= m_size.x()) {
		pos.addY(pos.x() - m_size.x() + 1);
		pos.setX(m_size.x() - 1);
	}
}

int32_t Map::absPosition(const Position& pos) const
{
	return pos.x() + pos.y() * m_size.x();
}

std::weak_ptr<Cell> Map::cell(Position pos) const
{
	uint32_t idx = absPosition(pos);

	return (idx < m_map.size()) ? m_map[idx] : nullptr;
}

std::weak_ptr<Cell> Map::cell(uint32_t x, uint32_t y) const
{
	return cell(Position(x, y));
}

std::weak_ptr<Cell> Map::cell(uint32_t offset) const
{
	return (offset < m_map.size()) ? m_map[offset] : nullptr;
}

const Position& Map::size() const
{
	return m_size;
}

uint32_t Map::sizeX() const
{
	return m_size.x();
}

uint32_t Map::sizeY() const
{
	return m_size.y();
}

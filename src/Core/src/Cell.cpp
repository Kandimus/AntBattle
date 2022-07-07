#include "Cell.h"

using namespace AntBattle;

Cell::Cell(const Position& pos) : m_pos(pos)
{

}

Cell::Cell(uint32_t x, uint32_t y) : m_pos(x, y)
{

}

void Cell::setFood(uint32_t count)
{
	if (m_ant.expired() && !m_isStone) {
		m_food += count;
	} else {
		//TODO Log
	}
}

void Cell::setAnt(const std::weak_ptr<Ant>& ant)
{
	if (!m_food && !m_isStone) {
		m_ant = ant;
	} else {
		//TODO Log
	}
}

void Cell::removeAnt()
{
	if (!m_ant.expired()) {
		m_ant.reset();
	}
}

void Cell::setStone(bool isstone)
{
	if (!m_food && m_ant.expired()) {
		m_isStone = isstone;
	} else {
		//TODO Log
	}
}

#include "Position.h"

using namespace AntBattle;

Position::Position(const Position& pos)
{
	m_x = pos.x();
	m_y = pos.y();
}

Position::Position(uint32_t x, uint32_t y)
{
	m_x = x;
	m_y = y;
}

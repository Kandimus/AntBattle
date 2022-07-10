#include "Position.h"

using namespace AntBattle;

Position::Position(const Position& pos)
{
	m_x = pos.x();
	m_y = pos.y();
}

Position::Position(int32_t x, int32_t y)
{
	m_x = x;
	m_y = y;
}

bool Position::operator==(const Position& p) const
{
	return m_x == p.x() && m_y == p.y();
}

Position Position::operator+(const Position& p) const
{
	Position result(*this);

	result.addX(p.x());
	result.addY(p.y());

	return result;
}

Position Position::operator-(const Position& p) const
{
	Position result(*this);

	result.addX(-p.x());
	result.addY(-p.y());

	return result;
}

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

Position Position::operator*(uint32_t len) const
{
	Position result(*this);

	result.setX(result.x() * len);
	result.setY(result.y() * len);

	return result;
}

void Position::operator+=(const Position& p)
{
	m_x += p.x();
	m_y += p.y();
}

void Position::operator-=(const Position& p)
{
	m_x -= p.x();
	m_y -= p.y();
}

void Position::operator+=(uint32_t val)
{
	m_x += val;
	m_y += val;
}

void Position::operator-=(uint32_t val)
{
	m_x -= val;
	m_y -= val;
}

std::string Position::toString() const
{
	char buf[64] = {0};

	sprintf(buf, "[%i, %i]", m_x, m_y);

	return std::string(buf);
}

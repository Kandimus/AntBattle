#pragma once

#include <stdint.h>

namespace AntBattle {

class Position
{
public:
	Position() = default;
	Position(const Position& pos);
	Position(uint32_t x, uint32_t y);
	virtual ~Position() = default;

	uint32_t x() const { return m_x; }
	uint32_t y() const { return m_y; }

	void init(uint32_t x, uint32_t y) { m_x = x; m_y = y; }
	void setX(uint32_t x) { m_x = x; }
	void setY(uint32_t y) { m_y = y; }
	void addX(uint32_t x) { m_x += x; }
	void addY(uint32_t y) { m_y += y; }

private:
	uint32_t m_x = 0;
	uint32_t m_y = 0;
};

};

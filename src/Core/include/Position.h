#pragma once

#include <stdint.h>

#include <string>

namespace AntBattle {

class Position
{
public:
	Position() = default;
	Position(const Position& pos);
	Position(int32_t x, int32_t y);
	virtual ~Position() = default;

	constexpr int32_t x() const { return m_x; }
	constexpr int32_t y() const { return m_y; }

	void init(int32_t x, int32_t y) { m_x = x; m_y = y; }
	void setX(int32_t x) { m_x = x; }
	void setY(int32_t y) { m_y = y; }
	void addX(int32_t x) { m_x += x; }
	void addY(int32_t y) { m_y += y; }

	bool operator==(const Position& p) const;
	Position operator+(const Position& p) const;
	Position operator-(const Position& p) const;
	Position operator*(uint32_t len) const;

	void operator+=(const Position& p);
	void operator-=(const Position& p);
	void operator+=(uint32_t val);
	void operator-=(uint32_t val);

	std::string toString() const;

private:
	int32_t m_x = 0;
	int32_t m_y = 0;
};

};

#include "Math.h"

#include <vector>

namespace AntBattle {

namespace Math {

std::random_device g_randDev;
std::mt19937 g_randGenerator(g_randDev());

std::random_device& randDevice()
{
	return g_randDev;
}


std::mt19937& randGenerator()
{
	return g_randGenerator;
}

/// return random direction
Direction randDirection()
{
	std::uniform_int_distribution<std::mt19937::result_type> dist0_7(0, 7);

	return static_cast<Direction>(dist0_7(randGenerator()));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief return position offset to direction
Position positionOffset(const Direction& dir)
{
	Direction d = dir;

	while(1) {
		switch(d) {
			case Direction::Nord:      return Position(0, -1);
			case Direction::NordEast:  return Position(1, -1);
			case Direction::East:      return Position(1, 0);
			case Direction::SouthEast: return Position(1, 1);
			case Direction::South:     return Position(0, 1);
			case Direction::SouthWest: return Position(-1, 1);
			case Direction::West:      return Position(-1, 0);
			case Direction::NordWest:  return Position(-1, -1);
			default:
				d = randDirection();
				break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \brief normalize value of direction
///
/// Normalize any value to the direction enum
Direction normalizeDirection(int val)
{
	while (val < static_cast<int>(Direction::Nord)) {
		val += 8;
	}

	while (val > static_cast<int>(Direction::NordWest)) {
		val -= 8;
	}

	return static_cast<Direction>(val);
}

Direction probabilisticDirection(const Direction& dir)
{
	std::vector<Direction> array;
	std::uniform_int_distribution<std::mt19937::result_type> dist0_1(0, 1);
	int clockwise = dist0_1(g_randGenerator);
	int val = static_cast<int>(dir);

	array.push_back(dir);
	array.push_back(dir);
	array.push_back(dir);
	array.push_back(dir);
	array.push_back(dir);
	array.push_back(dir);

	array.push_back(normalizeDirection(clockwise ? val + 1 : val - 1));
	array.push_back(normalizeDirection(clockwise ? val + 1 : val - 1));
	array.push_back(normalizeDirection(clockwise ? val + 1 : val - 1));
	array.push_back(normalizeDirection(clockwise ? val - 1 : val + 1));
	array.push_back(normalizeDirection(clockwise ? val - 1 : val + 1));
	array.push_back(normalizeDirection(clockwise ? val - 1 : val + 1));

	array.push_back(normalizeDirection(clockwise ? val + 2 : val - 2));
	array.push_back(normalizeDirection(clockwise ? val + 2 : val - 2));
	array.push_back(normalizeDirection(clockwise ? val - 2 : val + 2));
	array.push_back(normalizeDirection(clockwise ? val - 2 : val + 2));

	array.push_back(normalizeDirection(clockwise ? val + 3 : val - 3));
	array.push_back(normalizeDirection(clockwise ? val - 3 : val + 3));

	array.push_back(normalizeDirection(val + 4));

	std::uniform_int_distribution<std::mt19937::result_type> dist0_n(0, array.size());

	return array[dist0_n(g_randGenerator)];
}

Direction directionTo(const Position& posFrom, const Position& posTo)
{
	static const double pi_1_8 = (1 / 8) * 3.14159;
	static const double pi_3_8 = (3 / 8) * 3.14159;
	static const double pi_5_8 = (5 / 8) * 3.14159;
	static const double pi_7_8 = (7 / 8) * 3.14159;

	// convert the pointTo to coordinate system the pointFrom
	Position pos = posTo - posFrom;

	if (posFrom == posTo) {
		return randDirection();
	}

	double angle = std::atan2(pos.x(), pos.y());

	if (angle >= 0) {
		if (angle < pi_1_8) return Direction::East;
		if (angle < pi_3_8) return Direction::NordEast;
		if (angle < pi_5_8) return Direction::Nord;
		if (angle < pi_7_8) return Direction::NordWest;
		return Direction::West;
	} else {
		if (angle < -pi_7_8) return Direction::West;
		if (angle < -pi_5_8) return Direction::SouthWest;
		if (angle < -pi_3_8) return Direction::South;
		if (angle < -pi_1_8) return Direction::SouthEast;
		return Direction::East;
	}
}

/// \brief Create the priority array of direction.
///
/// The array will sorted by priority of select direction, from selected to reverse direction.
/// Middle values will added by random order (clockwise or counter-clockwise)
std::vector<Direction> createDirectionArray(const Direction& dir)
{
	std::vector<Direction> array;
	std::uniform_int_distribution<std::mt19937::result_type> dist0_1(0, 1);
	int clockwise = dist0_1(g_randGenerator);
	int val = static_cast<int>(dir);

	array.push_back(dir);
	array.push_back(normalizeDirection(clockwise ? val + 1 : val - 1));
	array.push_back(normalizeDirection(clockwise ? val - 1 : val + 1));
	array.push_back(normalizeDirection(clockwise ? val + 2 : val - 2));
	array.push_back(normalizeDirection(clockwise ? val - 2 : val + 2));
	array.push_back(normalizeDirection(clockwise ? val + 3 : val - 3));
	array.push_back(normalizeDirection(clockwise ? val - 3 : val + 3));
	array.push_back(normalizeDirection(val + 4));

	return array;
}

Direction inverseDirection(const Direction& dir)
{
	switch(dir) {
		case Direction::Nord:      return Direction::South;
		case Direction::NordEast:  return Direction::SouthWest;
		case Direction::East:      return Direction::West;
		case Direction::SouthEast: return Direction::NordWest;
		case Direction::South:     return Direction::Nord;
		case Direction::SouthWest: return Direction::NordEast;
		case Direction::West:      return Direction::East;
		case Direction::NordWest:  return Direction::SouthEast;
		default: return randDirection();
	}
}

}; // namespace Math

}; // namespace AntBattle

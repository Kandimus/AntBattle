#include <iostream>
#include <chrono>
//#include "../../catchtest/catch.hpp"

#include "Math.h"
#include "Map.h"
//#include "Config.h"

//TEST_CASE("Test visibleCells function", "[Math]")
//{
//	SECTION("check count of cell") {
//		int count_1 = 7;
//		int count_2 = 50;
//		std::vector<AntBattle::Position> array_1 = AntBattle::Math::visibleCells(AntBattle::Position(0, 0), count_1);
//		std::vector<AntBattle::Position> array_2 = AntBattle::Math::visibleCells(AntBattle::Position(0, 0), count_2);

//		std::cout << "count of element: " << array_2.size() << std::endl;

//		for (auto& itm : array_2) {
//			std::cout << "(" << itm.x() << ", " << itm.y() << "), ";
//		}

//		std::cout << std::endl;

//		//REQUIRE(ss("var.test_dens.value"));
//		CHECK(array_1.size() == 177);
//	}
//}

//void checkNearAvaliblePosition()
//{
//	auto conf = std::make_shared<AntBattle::Config>("test_nearavalibleposition.conf");
//	AntBattle::Map map(conf);

//	map.nearAvaliblePosition(AntBattle::Position(25, 25));
//}

bool checkDuble(const std::vector<AntBattle::Position>& array)
{
	for (int ii = 0; ii < array.size() - 1; ++ii) {
		for (int jj = ii + 1; jj < array.size(); ++jj) {
			if (array[ii] == array[jj]) {
				std::cout << "Error: duplicate (" << array[ii].x() << ", " << array[ii].y() << ")" << std::endl;
				return false;
			}
		}
	}
	return true;
}

void printVisibleArray(const std::vector<AntBattle::Position>& array, int count)
{
	for (int y = -count; y <= count; ++y) {
		std::cout << '|';

		for (int x = -count; x <= count; ++x) {
			AntBattle::Position pos(x, y);
			char ch = ' ';

			for (auto& cell : array) {
				if (cell == pos) {
					ch = 'X';
					break;
				}
			}

			std::cout << ch;
		}
		std::cout << '|' << std::endl;
	}
}

int main()
{
	int count_1 = 7;
	int count_2 = 50;
	std::vector<AntBattle::Position> array_1 = AntBattle::Math::visibleCells(AntBattle::Position(0, 0), count_1);
	std::vector<AntBattle::Position> array_2 = AntBattle::Math::visibleCells(AntBattle::Position(0, 0), count_2);

	std::cout << "count of element_1: " << array_1.size() << std::endl;
	std::cout << "count of element_2: " << array_2.size() << std::endl;

	if (checkDuble(array_1)) {
		std::cout << "All values in array_1 is unique" << std::endl;
	}

	if (checkDuble(array_2)) {
		std::cout << "All values in array_2 is unique" << std::endl;
	}

	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_t> m_beg(clock_t::now());
	int timer_count = 10000;

	for (int ii = 0; ii < timer_count; ++ii) {
		volatile std::vector<AntBattle::Position> timer_array = AntBattle::Math::visibleCells(AntBattle::Position(0, 0), count_2);
	}

	double elapsed = std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();

	std::cout << "Call AntBattle::Math::visibleCells(R = " <<  count_2 << ") = " << elapsed / timer_count << " sec" << std::endl;
}

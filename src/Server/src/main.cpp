#include <iostream>
#include "Battle.h"
#include "Math.h"

int main()
{
	for (int ii = 0; ii < 20; ++ii) {
		std::cout << AntBattle::Math::random(0, RAND_MAX) << std::endl;
	}

	std::vector<std::string> plr_list = {"red.dll", "black.dll"};
	AntBattle::Battle battle("test.conf", plr_list);

	battle.run();

	std::cout << "Hello World!" << std::endl;
	return 0;
}

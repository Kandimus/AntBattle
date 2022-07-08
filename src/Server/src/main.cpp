#include <iostream>
#include "Battle.h"

int main()
{
	std::vector<std::string> plr_list = {"red.dll", "black.dll"};
	AntBattle::Battle battle("test.conf", plr_list);

	std::cout << "Hello World!" << std::endl;
	return 0;
}

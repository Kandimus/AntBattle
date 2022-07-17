#include <iostream>
#include "Battle.h"
#include "Math.h"
#include "simpleargs.h"

namespace Arg {
	const std::string Config = "config";
	const char Config_short = 'c';
}



int main(int argc, const char **argv)
{
	rSimpleArgs::instance()
		.addOption(Arg::Config, Arg::Config_short, "");

	rSimpleArgs::instance().parse(argc, argv);

	if (rSimpleArgs::instance().getOption(Arg::Config).empty()) {
		std::cout << "Do not selected configuration file!" << std::endl;
		return 1;
	}

	std::vector<std::string> plr_list;

	for (int ii = 0; ii < rSimpleArgs::instance().getCountArgument(); ++ii) {
		plr_list.push_back(rSimpleArgs::instance().getArgument(ii));
	}

	AntBattle::Battle battle(rSimpleArgs::instance().getOption(Arg::Config), plr_list);

	battle.run();

	std::cout << "Hello World!" << std::endl;
	return 0;
}

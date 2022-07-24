#include <iostream>
#include "Battle.h"
#include "Math.h"
#include "Log.h"
#include "simpleargs.h"

namespace Arg {

using ArgName = const std::pair<const std::string, const char>;

ArgName Config = {"config", 'c'};
ArgName Log    = {"log", 'l'};

}



int main(int argc, const char **argv)
{
	rSimpleArgs::instance()
		.addOption(Arg::Config.first, Arg::Config.second, "")
		.addOption(Arg::Log.first   , Arg::Log.second , "");

	rSimpleArgs::instance().parse(argc, argv);

	AntBattle::Log::instance().setLevel(AntBattle::Log::Level::Debug);

	if (rSimpleArgs::instance().getOption(Arg::Config.first).empty()) {
		std::cout << "Do not selected configuration file!" << std::endl;
		return 1;
	}

	std::vector<std::string> plr_list;

	for (int ii = 0; ii < rSimpleArgs::instance().getCountArgument(); ++ii) {
		plr_list.push_back(rSimpleArgs::instance().getArgument(ii));
	}

	AntBattle::Battle battle(rSimpleArgs::instance().getOption(Arg::Config.first), plr_list);

	battle.run();

	std::cout << "Hello World!" << std::endl;
	return 0;
}

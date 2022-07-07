#pragma once

#include <string>

namespace BattleLog {

class IBattleLog
{
public:
	IBattleLog() = default;
	virtual ~Battle() = default;


	void toSave(const std::string& json) = 0;
};

};

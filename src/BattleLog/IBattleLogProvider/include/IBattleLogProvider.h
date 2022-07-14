#pragma once

#include <memory>

namespace AntBattle {

class Map;

class IBattleLogProvider
{
public:
	IBattleLogProvider() = default;
	virtual ~IBattleLogProvider() = default;


	virtual void toSave(const std::weak_ptr<Map>& map) = 0;
};

};

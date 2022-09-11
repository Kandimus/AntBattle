#pragma once

#include <memory>

namespace AntBattle {

class Player;
class Map;

class IBattleLogProvider
{
public:
	IBattleLogProvider() = default;
	virtual ~IBattleLogProvider() = default;

//	virtual void saveBattle(const std::weak_ptr<Map>& map) = 0;
	virtual void saveMapInfo(const std::weak_ptr<Map>& map) = 0;
	virtual void savePlayer(const std::weak_ptr<Player>& player) = 0;
	virtual void saveNewTurn(uint32_t iteration) = 0;
	virtual void saveMap(const std::weak_ptr<Map>& map) = 0;
//	virtual void saveFinal(const std::weak_ptr<Map>& map) = 0;
};

};

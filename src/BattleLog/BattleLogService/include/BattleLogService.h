#pragma once

#include <memory>
#include <vector>

#include "IBattleLogProvider.h"

namespace AntBattle {

class BattleLogService : public IBattleLogProvider
{
public:
	BattleLogService() = default;
	virtual ~BattleLogService() = default;

public:
	void add(const std::shared_ptr<IBattleLogProvider>& provider);

	void saveMapInfo(const std::weak_ptr<Map>& map) override;
	void savePlayer(const std::weak_ptr<Player>& player) override;
	void saveNewTurn(uint32_t iteration) override;
	void saveMap(const std::weak_ptr<Map>& map) override;

protected:
	std::vector<std::shared_ptr<IBattleLogProvider>> m_providers;
};

};

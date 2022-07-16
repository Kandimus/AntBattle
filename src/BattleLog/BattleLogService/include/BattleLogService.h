#pragma once

#include <memory>
#include <vector>

#include "IBattleLogProvider.h"

namespace AntBattle {

class BattleLogService : public IBattleLogProvider
{
//// Singleton
//public:
//	virtual ~BattleLogService() = default;
//	BattleLogService(const BattleLogService&) = delete;
//	BattleLogService(const BattleLogService&&) = delete;

//	static BattleLogService& instance();

//private:
//	BattleLogService() = default;
//	BattleLogService& operator=(BattleLogService&);
public:
	BattleLogService() = default;
	virtual ~BattleLogService() = default;

public:
	void add(const std::shared_ptr<IBattleLogProvider>& provider);

	void savePlayer(const std::weak_ptr<Player>& player) override;
	void saveMap(const std::weak_ptr<Map>& map) override;

protected:
	std::vector<std::shared_ptr<IBattleLogProvider>> m_providers;
};

};

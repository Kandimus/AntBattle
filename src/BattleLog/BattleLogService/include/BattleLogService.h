#pragma once

#include <memory>
#include <vector>

namespace AntBattle {

class IBattleLogProvider;

class BattleLogService
{
// Singleton
public:
	virtual ~BattleLogService() = default;
	BattleLogService(const BattleLogService&) = delete;
	BattleLogService(const BattleLogService&&) = delete;

	static BattleLogService& instance();

private:
	BattleLogService() = default;
	BattleLogService& operator=(BattleLogService&);


public:
	void add(const std::shared_ptr<IBattleLogProvider>& provider);


protected:
	std::vector<std::shared_ptr<IBattleLogProvider>> m_providers;
};

};

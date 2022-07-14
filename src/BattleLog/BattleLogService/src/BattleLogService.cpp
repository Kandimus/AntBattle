#include "BattleLogService.h"
#include "IBattleLogProvider.h"

namespace AntBattle {

BattleLogService& BattleLogService::instance()
{
	static BattleLogService Singleton;
	return Singleton;
}

void BattleLogService::add(const std::shared_ptr<IBattleLogProvider>& provider)
{
	m_providers.push_back(provider);
}

};

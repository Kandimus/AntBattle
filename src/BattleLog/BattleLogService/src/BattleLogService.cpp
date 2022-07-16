#include "BattleLogService.h"
#include "IBattleLogProvider.h"

namespace AntBattle {

//BattleLogService& BattleLogService::instance()
//{
//	static BattleLogService Singleton;
//	return Singleton;
//}

void BattleLogService::add(const std::shared_ptr<IBattleLogProvider>& provider)
{
	for (auto& item : m_providers) {
		if (item == provider) {
			return;
		}
	}

	m_providers.push_back(provider);
}

void BattleLogService::savePlayer(const std::weak_ptr<Player>& player)
{
	for (auto& provider : m_providers) {
		provider->savePlayer(player);
	}
}

void BattleLogService::saveMap(const std::weak_ptr<Map>& map)
{
	for (auto& provider : m_providers) {
		provider->saveMap(map);
	}
}

};

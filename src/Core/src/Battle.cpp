#include "Map.h"
#include "Config.h"
#include "Battle.h"
#include "Log.h"

using namespace AntBattle;

Battle::Battle(const std::string& confname)
{
	m_conf = std::make_shared<Config>(confname);
	m_map  = std::unique_ptr<Map>(new Map(m_conf));

	Log::instance().put(format("create new battle. map is [%i x %i]", m_conf->width(), m_conf->width()));
}


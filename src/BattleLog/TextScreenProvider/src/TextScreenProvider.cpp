#include "TextScreenProvider.h"

#include <fstream>

#include "Log.h"
#include "Map.h"
#include "Cell.h"
#include "Ant.h"
#include "Player.h"

using namespace AntBattle;

class Map;

TextScreenProvider::TextScreenProvider(const std::string& filename)
	: m_filename(filename)
{
	std::ofstream file(m_filename);

	if (!file.is_open()) {
		if (!m_cantOpen) {
			Log::instance().put(Log::Level::Error, format("Error: Can create the '%s' log file!", m_filename.c_str()));
			m_cantOpen = true;
		}
		return;
	}
}

bool TextScreenProvider::isFileOpen(std::ofstream& of)
{
	if (!of.is_open()) {
		if (!m_cantOpen) {
			Log::instance().put(Log::Level::Error, format("Error: Can not open the '%s' log file!", m_filename.c_str()));
			m_cantOpen = true;
		}
		return false;
	}

	return true;
}

void TextScreenProvider::saveMapInfo(const std::weak_ptr<Map>& map)
{
	if (!m_map.size()) {
		auto pMap = map.lock();

		m_w = pMap->size().x();
		m_h = pMap->size().y();
		m_map.resize(m_w * m_h);

		for (auto& item : m_map) {
			item = cEmpty;
		}
	}
}

void TextScreenProvider::savePlayer(const std::weak_ptr<Player>& player)
{
//	auto pPlayer = player.lock();
//	std::ofstream file(m_filename, std::ios::app);

//	if (!isFileOpen(file)) {
//		return;
//	}

//	nlohmann::json json;

//	json["player"]["index"] = pPlayer->index();
//	json["player"]["team"] = pPlayer->teamName();
//	json["player"]["type"] = pPlayer->antType();
//	json["player"]["library"]["filename"] = pPlayer->library();
//	json["player"]["library"]["hash"] = pPlayer->libHash();
//	json["player"]["library"]["version"] = pPlayer->libVersion();

//	file << json << std::endl;
//	file.close();
}

void TextScreenProvider::saveNewTurn(uint32_t iteration)
{
//	std::ofstream file(m_filename, std::ios::app);

//	if (!isFileOpen(file)) {
//		return;
//	}

//	nlohmann::json json;

//	json["turn"] = iteration;

//	file << json << std::endl;

//	file.close();
}

void TextScreenProvider::saveMap(const std::weak_ptr<Map>& map)
{
	auto pMap = map.lock();
	std::ofstream file(m_filename, std::ios::app);

	if (!isFileOpen(file)) {
		return;
	}

	uint32_t size = pMap->size().x() * pMap->size().y();

	for (int ii = 0; ii < size; ++ii) {
		auto pCell = pMap->cell(ii).lock();

		if (!pCell->isChanged()) {
			continue;
		}

		if (pCell->isEmpty()) {
			m_map[ii] = cEmpty;
		} else if (pCell->isStone()) {
			m_map[ii] = cStone;
		} else if (pCell->food()) {
			m_map[ii] = cFood;
		} else {
			auto pAnt = pCell->ant().lock();
			auto pPlayer = pAnt->player().lock();

			if (pPlayer->index() >= 4) {
				m_map[ii] = '@';
			} else {
				switch (pAnt->type()) {
					case Ant::Type::Queen:  m_map[ii] = cQueen [pPlayer->index()]; break;
					case Ant::Type::Solder: m_map[ii] = cSolder[pPlayer->index()]; break;
					case Ant::Type::Worker: m_map[ii] = cWorker[pPlayer->index()]; break;
					default: m_map[ii] = '?';
				}
			}
		}
	}

	file << '+';
	for (uint32_t x = 0; x < m_w; ++x) {
		file << '-';
	}
	file << '+' << std::endl;

	for (uint32_t y = 0; y < m_h; ++y) {
		file << '|';
		for (uint32_t x = 0; x < m_w; ++x) {
			file << m_map[x + y * m_w];
		}
		file << '|' << std::endl;
	}

	file << '+';
	for (uint32_t x = 0; x < m_w; ++x) {
		file << '-';
	}
	file << '+';

	file << std::endl;
	file << std::endl;

	file.close();
}


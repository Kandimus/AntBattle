#include "FileProvider.h"

#include <fstream>
#include "json.hpp"

#include "Log.h"
#include "Map.h"
#include "Cell.h"
#include "Ant.h"
#include "Player.h"

using namespace AntBattle;

class Map;

FileProvider::FileProvider(const std::string& filename)
	: m_filename(filename)
{
	std::ofstream file(m_filename);

	if (!file.is_open()) {
		if (!m_cantOpen) {
			Log::instance().put(format("Error: Can create the '%s' log file!", m_filename.c_str()));
			m_cantOpen = true;
		}
		return;
	}
}

bool FileProvider::isFileOpen(std::ofstream& of)
{
	if (!of.is_open()) {
		if (!m_cantOpen) {
			Log::instance().put(format("Error: Can not open the '%s' log file!", m_filename.c_str()));
			m_cantOpen = true;
		}
		return false;
	}

	return true;
}

void FileProvider::savePlayer(const std::weak_ptr<Player>& player)
{
	auto pPlayer = player.lock();
	std::ofstream file(m_filename, std::ios::app);

	if (!isFileOpen(file)) {
		return;
	}

	nlohmann::json json;

	json["player"]["index"] = pPlayer->index();
	json["player"]["team"] = pPlayer->teamName();
	json["player"]["type"] = pPlayer->antType();
	json["player"]["library"]["filename"] = pPlayer->library();
	json["player"]["library"]["hash"] = pPlayer->libHash();
	json["player"]["library"]["version"] = pPlayer->libVersion();

	file << json << std::endl;
	file.close();
}

void FileProvider::saveNewTurn(uint32_t iteration)
{
	std::ofstream file(m_filename, std::ios::app);

	if (!isFileOpen(file)) {
		return;
	}
}

void FileProvider::saveMap(const std::weak_ptr<Map>& map)
{
	auto pMap = map.lock();
	std::ofstream file(m_filename, std::ios::app);

	if (!isFileOpen(file)) {
		return;
	}

	uint32_t size = pMap->size().x() * pMap->size().y();
	for (int ii = 0; ii < size; ++ii) {
		auto pCell = pMap->cell(ii).lock();
		nlohmann::json json;

		if (!pCell->isChanged()) {
			continue;
		}

		json["cell"]["x"] = ii % pMap->size().y();
		json["cell"]["y"] = ii / pMap->size().y();

		if (pCell->isEmpty()) {
			json["cell"]["type"] = "empty";
		} else if (pCell->isStone()) {
			json["cell"]["type"] = "stone";
		} else {
			auto pAnt = pCell->ant().lock();
			auto pPlayer = pAnt->player().lock();

			json["cell"]["type"] = "ant";
			json["cell"]["ant"]["type"] = pAnt->strType();
			json["cell"]["ant"]["player"] = pPlayer->index();
			json["cell"]["ant"]["health"] = pAnt->healthPercent();
		}

		file << json << std::endl;
	}

	file.close();
}


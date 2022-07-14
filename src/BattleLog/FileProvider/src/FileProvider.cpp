#include "FileProvider.h"

#include <fstream>

#include "Map.h"
#include "Cell.h"
#include "Ant.h"
#include "Player.h"

#include "json.hpp"

using namespace AntBattle;

class Map;

FileProvider::FileProvider(const std::string& filename)
{

}


void FileProvider::toSave(const std::weak_ptr<Map>& map)
{
	nlohmann::json json;
	auto pMap = map.lock();


	//TODO check for first iteration

	uint32_t size = pMap->size().x() * pMap->size().y();
	for (int ii = 0; ii < size; ++ii) {
		auto pCell = pMap->cell(ii).lock();

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

			json["cell"]["type"]["ant"]["type"] = pAnt->strType();
			json["cell"]["type"]["ant"]["x"] = pAnt->position().x();
			json["cell"]["type"]["ant"]["x"] = pAnt->position().y();
			json["cell"]["type"]["ant"]["player"] = pPlayer->teamName();
			json["cell"]["type"]["ant"]["health"] = pAnt->healthPercent();
		}
	}

	std::ofstream file(m_filename);

	if (file.is_open()) {
		file << json;
		file.close();
	}
}


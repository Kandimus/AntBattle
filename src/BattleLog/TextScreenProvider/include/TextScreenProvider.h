#pragma once

#include "IBattleLogProvider.h"
#include <vector>

namespace AntBattle {

class TextScreenProvider : public IBattleLogProvider
{
public:
	TextScreenProvider(const std::string& filename);
	virtual ~TextScreenProvider() = default;

	void saveMapInfo(const std::weak_ptr<Map>& map) override;
	void savePlayer(const std::weak_ptr<Player>& player) override;
	void saveNewTurn(uint32_t iteration) override;
	void saveMap(const std::weak_ptr<Map>& map) override;

protected:
	bool isFileOpen(std::ofstream& of);

protected:
	std::string m_filename = "";
	bool m_cantOpen = false;

	uint32_t m_w = 0;
	uint32_t m_h = 0;
	std::vector<char> m_map;

	const char cEmpty = ' ';
	const char cStone = '#';
	const char cFood  = '*';
	const char cQueen[4]  = {'q', 'w', 'e', 'r'};
	const char cSolder[4] = {'a', 's', 'd', 'f'};
	const char cWorker[4] = {'z', 'x', 'c', 'v'};
};

};

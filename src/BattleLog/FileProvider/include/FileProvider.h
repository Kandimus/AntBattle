#pragma once

#include "IBattleLogProvider.h"

namespace AntBattle {

class FileProvider : public IBattleLogProvider
{
public:
	FileProvider(const std::string& filename);
	virtual ~FileProvider() = default;


	void savePlayer(const std::weak_ptr<Player>& player) override;
	void saveMap(const std::weak_ptr<Map>& map) override;

protected:
	std::string m_filename = "";
	bool m_cantOpen = false;
};

};

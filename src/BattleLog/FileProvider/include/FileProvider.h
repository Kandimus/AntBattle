#pragma once

#include "IBattleLogProvider.h"

namespace AntBattle {

class Map;

class FileProvider : public IBattleLogProvider
{
public:
	FileProvider(const std::string& filename);
	virtual ~FileProvider() = default;


	void toSave(const std::weak_ptr<Map>& map) override;

protected:
	std::string m_filename = "";

};

};

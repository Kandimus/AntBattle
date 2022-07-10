#pragma once

#include <stdint.h>
#include <string>
#include <memory>

#include "../../Interface/PlayerInfo.h"

namespace AntBattle {

class Ant;

class Player
{
public:
	Player(const std::string& libname);
	virtual ~Player() = default;

//	void/*Command*/ doWorkerProcess(/*PlayerInfo*/);
//	void/*Command*/ doSolderProcess(/*PlayerInfo*/);
//	void/*Command*/ doQueenProcess(/*PlayerInfo*/);

	void antQueen(const std::weak_ptr<Ant>& queen);
	std::weak_ptr<Ant> antQueen();

protected:
	std::string m_libName = "";
	uint32_t m_libHash = 0;
	PlayerInfo m_info;

	std::weak_ptr<Ant> m_antQueen;
};

};

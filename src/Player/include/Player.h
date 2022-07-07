#pragma once

#include <stdint.h>
#include <string>

namespace AntBattle {

class Player
{
public:
	Player(const std::string& libname);
	virtual ~Player() = default;

	void/*Command*/ doWorkerProcess(/*PlayerInfo*/);
	void/*Command*/ doSolderProcess(/*PlayerInfo*/);
	void/*Command*/ doQueenProcess(/*PlayerInfo*/);

protected:
	std::string m_libName = "";
	uint32_t m_libHash = 0;
};

};

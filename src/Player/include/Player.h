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
	virtual ~Player();

//	void/*Command*/ doWorkerProcess(/*PlayerInfo*/);
//	void/*Command*/ doSolderProcess(/*PlayerInfo*/);
//	void/*Command*/ doQueenProcess(/*PlayerInfo*/);

	bool isInit() const;

	void antQueen(const std::weak_ptr<Ant>& queen);
	std::weak_ptr<Ant> antQueen();

protected:
	bool loadLibrary();

protected:
	std::string m_libName = "";
	uint32_t m_libHash = 0;
	PlayerInfo m_info;
	bool m_isInit = false;

	void* m_handleLib = nullptr;
	AntInit    m_fnInit;
	AntFinalize m_fnFinalize;
	AntProcess m_fnWorkerProcess;
	AntProcess m_fnSolderProcess;
	AntProcess m_fnQueenProcess;

	std::weak_ptr<Ant> m_antQueen;
};

};

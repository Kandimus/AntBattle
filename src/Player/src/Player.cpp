#include "Player.h"

using namespace AntBattle;

Player::Player(const std::string& libname) : m_libName(libname)
{
	//TODO open lib

	//TODO call GetPlayerInfo

	//TODO calc m_libHash as CRC32(file libname)

	//TODO get pointers to Do*Process

	m_libHash = 0;
}

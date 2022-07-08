#include "Ant.h"

using namespace AntBattle;

Ant::Ant()
	: m_pos()
	, m_status(Status::Undef)
{

}

std::weak_ptr<Player> Ant::player() const
{
	return m_player;
}

void Ant::setPlayer(std::weak_ptr<Player> player)
{
	m_player = player;
}

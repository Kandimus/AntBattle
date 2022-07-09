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

void Ant::setCommand(const Command& cmd)
{
	m_command = cmd;
}

Command& Ant::command()
{
	return m_command;
}

void Ant::clearCommand()
{
	m_command.type = CommandType::Idle;
	m_command.count = 0;
}

void Ant::setPosition(const Position& pos)
{
	m_pos = pos;
}

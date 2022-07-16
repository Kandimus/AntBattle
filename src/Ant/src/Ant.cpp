#include "Ant.h"

using namespace AntBattle;

Ant::Ant()
	: m_pos(0, 0)
	, m_status(Status::Undef)
{
}

void Ant::reset()
{
	m_status  = Status::Idle;
	m_satiety = maxSatiety();
	m_attack  = maxAttack();
	m_health  = maxHealth();
	m_visibility = maxVisibility();

	clearCommand();

	m_isFight = false;
}

double Ant::satietyPercent()
{
	return maxSatiety() / m_satiety * 100.0;
}

double Ant::healthPercent()
{
	return maxHealth() / m_health * 100.0;
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

#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include "Position.h"
#include "Direction.h"
#include "PlayerInfo.h"
#include "Command.h"

namespace AntBattle {

class Player;

class Ant
{
public:
	Ant();
	virtual ~Ant() = default;

	enum class Status {
		Undef = 0,
		Idle,
		Moving,
	};

public:
	virtual void reset();

	virtual uint32_t maxSatiety() const = 0;
	virtual uint32_t maxHealth() const = 0;
	virtual uint32_t maxAttack() const = 0;
	virtual uint32_t maxVisibility() const = 0;
	virtual std::string strType() const = 0;

	double satietyPercent();
	double healthPercent();

	bool hasCommand() const { return m_command.type != CommandType::Idle; }
	void setCommand(const Command& cmd);
	Command& command();
	void clearCommand();

	const Position& position() const { return m_pos; }
	void setPosition(const Position& pos);

	std::weak_ptr<Player> player() const;
	void setPlayer(std::weak_ptr<Player> player);

protected:
	Position m_pos;
	Status m_status;
	bool m_isFight;
	uint32_t m_satiety = 0;
	uint32_t m_health = 0;
	uint32_t m_attack = 0;
	uint32_t m_visibility = 0;

	Command m_command;
	std::weak_ptr<Player> m_player;
	AntProcess m_fnProcess;
};

};

#pragma once

#include <memory>
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
//	virtual bool toSearch(uint32_t count_iter) = 0;
//	virtual bool toMove(Direction dir, uint32_t count_iter) = 0;
//	virtual bool toAttack(Direction dir, uint32_t count_iter) = 0;
//	virtual bool toEeat(Direction dir) = 0;

	bool hasCommand() const { return m_command.type != CommandType::Idle; }
	void setCommand(const Command& cmd);
	Command& command();
	void clearCommand();

	constexpr const Position& position() const { return m_pos; }
	void setPosition(const Position& pos);

	std::weak_ptr<Player> player() const;
	void setPlayer(std::weak_ptr<Player> player);

protected:
	Position m_pos;
	Status m_status;
	uint32_t m_satiety = 0;
	uint32_t m_healty = 0;
	uint32_t m_attack = 0;
	uint32_t m_visibility = 0;

	Command m_command;
	std::weak_ptr<Player> m_player;
	AntProcess m_fnProcess;
	bool m_isFight;
};

};

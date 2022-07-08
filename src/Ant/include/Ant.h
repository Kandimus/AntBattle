#pragma once

#include <memory>
#include "Position.h"
#include "Direction.h"
#include "../../Interface/PlayerInfo.h"

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
	virtual bool toSearch(uint32_t count_iter) = 0;
	virtual bool toMove(Direction dir, uint32_t count_iter) = 0;
	virtual bool toAttack(Direction dir, uint32_t count_iter) = 0;
	virtual bool toEeat(Direction dir) = 0;

	std::weak_ptr<Player> player() const;
	void setPlayer(std::weak_ptr<Player> player);

protected:
	Position m_pos;
	Status m_status;
	uint32_t m_satiety = 0;
	uint32_t m_healty = 0;
	uint32_t m_attack = 0;
	uint32_t m_visibility = 0;

	Position m_moveTo;
	uint32_t m_iterationCount;
	std::weak_ptr<Player> m_player;
	AntProcess m_fnProcess;
};

};

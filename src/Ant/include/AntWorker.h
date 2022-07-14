#pragma once

#include "Ant.h"

namespace AntBattle {

class AntWorker : public Ant
{
public:
	AntWorker() = default;
	virtual ~AntWorker() = default;

	virtual void reset() override;
//	virtual void takeFood();

	virtual uint32_t maxCargoFood() const = 0;

protected:
	uint32_t m_cargoFood = 0;
};

};

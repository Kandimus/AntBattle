#pragma once

#include "Ant.h"

namespace AntBattle {

class AntWorker : public Ant
{
public:
	AntWorker();
	virtual ~AntWorker() = default;

	virtual void takeFood();

protected:
	virtual uint32_t getMaxCargo() const = 0;

protected:
	uint32_t m_cargoFood = 0;
};

};

#pragma once

#include "Ant.h"

namespace AntBattle {

class AntQueen : public Ant
{
public:
	AntQueen();
	virtual ~AntQueen() = default;

	virtual uint32_t maxTurnForWorker() const = 0;
	virtual uint32_t maxTurnForSolver() const = 0;
};

};

#pragma once

#include "Ant.h"

namespace AntBattle {

class AntQueen : public Ant
{
public:
	AntQueen() = default;
	virtual ~AntQueen() = default;

	virtual std::string strType() const override { return "queen"; }
	virtual uint32_t maxTurnForWorker() const = 0;
	virtual uint32_t maxTurnForSolver() const = 0;
};

};

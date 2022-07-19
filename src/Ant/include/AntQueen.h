#pragma once

#include "Ant.h"

namespace AntBattle {

class AntQueen : public Ant
{
public:
	AntQueen(std::weak_ptr<Player> player);
	virtual ~AntQueen() = default;

	virtual uint32_t maxTurnForWorker() const = 0;
	virtual uint32_t maxTurnForSolver() const = 0;

	virtual std::string strType() const override { return "queen"; }
	virtual uint32_t cargo() const override { return m_cargo; }
	virtual void reset() override;

protected:
	uint32_t m_cargo = 0;
};

};

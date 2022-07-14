#pragma once

#include "AntQueen.h"

namespace AntBattle {

class BlackAntQueen : public AntQueen
{
public:
	BlackAntQueen() = default;
	virtual ~BlackAntQueen() = default;

	virtual uint32_t maxSatiety()       const override { return 25; }
	virtual uint32_t maxHealth()        const override { return 15; }
	virtual uint32_t maxAttack()        const override { return 2;  }
	virtual uint32_t maxVisibility()    const override { return 50; }
	virtual uint32_t maxTurnForWorker() const override { return 1;  }
	virtual uint32_t maxTurnForSolver() const override { return 1;  }
};

};

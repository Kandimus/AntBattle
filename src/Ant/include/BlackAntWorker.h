#pragma once

#include "AntWorker.h"

namespace AntBattle {

class BlackAntWorker : public AntWorker
{
public:
	BlackAntWorker();

	virtual uint32_t maxSatiety()    const override { return 13; }
	virtual uint32_t maxHealth()     const override { return 3;  }
	virtual uint32_t maxAttack()     const override { return 1;  }
	virtual uint32_t maxVisibility() const override { return 50; }
	virtual uint32_t maxCargoFood()  const override { return 16; }
};

};

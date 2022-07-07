#pragma once

#include "AntWorker.h"

namespace AntBattle {

class BlackAntWorker : public AntWorker
{
public:
	BlackAntWorker();

protected:
	virtual uint32_t getMaxCargo() const override { return 16; }
};

};

#pragma once

#include "AntSolder.h"

namespace AntBattle {

class BlackAntSolder : public AntSolder
{
public:
	BlackAntSolder(std::weak_ptr<Player> player);
	virtual ~BlackAntSolder() = default;

	virtual uint32_t maxSatiety()    const override { return 13; }
	virtual uint32_t maxHealth()     const override { return 3;  }
	virtual uint32_t maxAttack()     const override { return 2;  }
	virtual uint32_t maxVisibility() const override { return 40; }
};

};

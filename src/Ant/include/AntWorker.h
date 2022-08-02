#pragma once

#include "Ant.h"

namespace AntBattle {

class AntWorker : public Ant
{
public:
	AntWorker(std::weak_ptr<Player> player);
	virtual ~AntWorker() = default;

	virtual uint32_t maxCargoFood() const = 0;

	virtual std::string strType() const override { return "worker"; }
	virtual uint32_t cargo() const override { return m_cargo; }

	virtual void reset() override;

protected:
	uint32_t m_cargo = 0;
};

};

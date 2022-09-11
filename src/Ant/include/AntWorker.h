#pragma once

#include "Ant.h"

namespace AntBattle {

class AntWorker : public Ant
{
public:
	AntWorker(std::weak_ptr<Player> player);
	virtual ~AntWorker() = default;

	virtual uint32_t maxCargoFood() const = 0;

	virtual bool isWorker() const override { return true; }
	virtual Type type() const override { return Type::Worker; }
	virtual std::string strType() const override { return "worker"; }
	virtual uint32_t cargo() const override { return m_cargo; }

	virtual int32_t setCargo(int32_t value);
	virtual int32_t modifyCargo(int32_t value);
	virtual bool isFullCargo() const { return m_cargo >= maxCargoFood(); }

	virtual void reset() override;

protected:
	uint32_t m_cargo = 0;
};

};

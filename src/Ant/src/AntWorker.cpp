#include "AntWorker.h"

using namespace AntBattle;

AntWorker::AntWorker()
{

}

void AntWorker::reset()
{
	m_cargoFood = maxCargoFood();

	Ant::reset();
}

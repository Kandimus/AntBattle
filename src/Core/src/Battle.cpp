#include "Log.h"
#include "Config.h"

#include "Math.h"
#include "Map.h"
#include "Player.h"
#include "Battle.h"

using namespace AntBattle;

Battle::Battle(const std::string& confname, const std::vector<std::string>& players)
{
	m_conf = std::make_shared<Config>(confname);
	m_map  = std::unique_ptr<Map>(new Map(m_conf));

	for (auto& libname : players) {
		m_players.push_back(std::make_shared<Player>(libname));
	}

	Log::instance().put(format("create new battle. map is [%i x %i]", m_conf->width(), m_conf->height()));
}

void Battle::run()
{
	//TODO check all players is init
	for (auto& player : m_players) {
		if (!player->isInit()) {
			Log::instance().put("Can't start the battle. One player is not ready");
			return;
		}
	}

	//TODO create UID of battle

	// create list of all ant
	m_ants = m_map->generate(m_players);

	// main loop
	while(true) {
		// randomize the list of ants
		std::shuffle(m_ants.begin(), m_ants.end(), Math::randGenerator());

		for (auto& ant : m_ants) {
			std::weak_ptr<Player> plr = ant->player();
			Command cmd;

			if(!ant->hasCommand()) {
				//TODO generate AntInfo for current ant
				//ant->process(&ai, &cmd);
				//ant->setCommand(cmd);
			}

			doAntCommand(ant);
		}
	}
}

void Battle::doAntCommand(AntSharedPtr& ant)
{
	Command& cmd = ant->command();

	switch(cmd.type) {
		case CommandType::Idle: cmd.type = CommandType::Explore; cmd.count = 1;
		case CommandType::Explore: commandAntExplore(ant); break;
		case CommandType::Move: commandAntMove(ant); break;
//		case CommandType::Eat: doAntEat(ant);break;
//		case CommandType::MoveToFood: doAntMoveToFood(ant); break;
//		case CommandType::Attack: doAntAttack(ant); break;
//		case CommandType::MoveAndAttack: doAntMoveAndAttack(ant); break;
//		case CommandType::Feed: doAntFeed(ant); break;
//		case CommandType::FeedQueen: doAntFeedQueen(ant); break;
//		case CommandType::Defense: doAntDefence(ant); break;
//		case CommandType::CreateOfWorker: doAntCreateWorker(ant); break;
//		case CommandType::CreateOfSolder: doAntCreateSolder(ant); break;
		default:
			cmd.type = CommandType::Explore;
			cmd.count = 1;
			commandAntExplore(ant);
			break;
	}

	if (!cmd.count) {
		ant->clearCommand();
	}
}

void Battle::commandAntExplore(AntSharedPtr& ant)
{
	std::shared_ptr<Player> player = ant->player().lock();
	std::shared_ptr<Ant> queen = player->antQueen().lock();

	Direction dirToQueen = Math::directionTo(ant->position(), queen->position());
	Direction dirFromQueen =Math::inverseDirection(dirToQueen);
	Direction dir = Math::probabilisticDirection(dirFromQueen);

	moveAnt(ant, dir);

	--ant->command().count;
}

void Battle::commandAntMove(AntSharedPtr& ant)
{
	Direction dir = Math::probabilisticDirection(ant->command().direction);

	moveAnt(ant, dir);

	--ant->command().count;
}

/// \brief Move the ant to selected direction
///
/// If we can not move the ant to selected direction then we move ant to direction near to selected
void Battle::moveAnt(AntSharedPtr& ant, const Direction& dir)
{
	// get odered directions
	std::vector<Direction> array_dir = Math::createDirectionArray(ant->command().direction);

	for (auto& dir : array_dir) {
		// get the new position as the ant position and the direction
		Position newpos = ant->position() + Math::positionOffset(dir);

		// checking reachability of the new position
		if (m_map->isCellEmpty(ant->position())) {
			// if the cell is empty then move the ant to it
			m_map->moveAnt(ant, newpos);
			return;
		}

		// cell is not empty, go to next the direction and to check next
	}

	ant->clearCommand();
}


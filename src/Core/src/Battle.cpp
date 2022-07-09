#include "Log.h"
#include "Config.h"

#include "Map.h"
#include "Player.h"
#include "Battle.h"

using namespace AntBattle;

Battle::Battle(const std::string& confname, const std::vector<std::string>& players)
	: m_randDev()
	, m_randGenerator(m_randDev)
{
	m_conf = std::make_shared<Config>(confname);
	m_map  = std::unique_ptr<Map>(new Map(m_conf));

	Log::instance().put(format("create new battle. map is [%i x %i]", m_conf->width(), m_conf->height()));

	for (auto& item : players) {
		m_players.push_back(std::make_shared<Player>(item));
	}

	m_ants = m_map->generate(m_players);
}

void Battle::run()
{
	//TODO check all players is init

	//TODO create UID of battle

	//TODO create temp list of all ant and randomize it

	// main loop
	while(true) {
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
		case CommandType::Move: doAndMove(ant); break;
		case CommandType::Eat: doAntEat(ant);break;
		case CommandType::MoveToFood: doAntMoveToFood(ant); break;
		case CommandType::Attack: doAntAttack(ant); break;
		case CommandType::MoveAndAttack: doAntMoveAndAttack(ant); break;
		case CommandType::Feed: doAntFeed(ant); break;
		case CommandType::FeedQueen: doAntFeedQueen(ant); break;
		case CommandType::Defense: doAntDefence(ant); break;
		case CommandType::CreateOfWorker: doAntCreateWorker(ant); break;
		case CommandType::CreateOfSolder: doAntCreateSolder(ant); break;
		default:
			cmd.type = CommandType::Explore;
			cmd.count = 1;
			doAntExplore(ant);
			break;
	}

	if (!cmd.count) {
		ant->clearCommand();
	}
}

void Battle::commandAntExplore(AntSharedPtr& ant)
{
	Command& cmd = ant->command();

	//TODO crate vector then have 4-5 values direction from queen, 2-3 values near first values, and 1 value for other directions. Tatal ~20 values
	//TODO crate rand value from 0 to 19 and get direction for move on current iteration

	cmd.direction = randDirection();

	moveAnt(ant, cmd.direction);

	--cmd.count;
}

/// \brief Move the ant to selected direction
///
/// If we can not move the ant to selected direction then we move ant to direction near to selected
void Battle::moveAnt(AntSharedPtr& ant, const Direction& dir)
{
	// get odered directions
	std::vector<Direction> array_dir = createDirectionArray(ant->command().direction);

	while(array_dir.size()) {
		// get the new position as ant position and direction
		Position newpos = ant->position() + positionOffset(array_dir.back());

		// checking reachability of the new position
		if (m_map->isCellEmpty(ant->position())) {
			// if the cell is empty then move the ant to it
			m_map->moveAnt(ant, newpos);
			return;
		}

		// cell is not empty, delete this direction from the array and to check next
		array_dir.pop_back();
	}

	ant->clearCommand();
}

/// return random direction
Direction Battle::randDirection()
{
	std::uniform_int_distribution<std::mt19937::result_type> dist0_7(0, 7);

	return static_cast<Direction>(dist0_7(m_randGenerator));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief return position offset to direction
Position Battle::positionOffset(const Direction& dir)
{
	Direction d = dir;

	while(1) {
		switch(d) {
			case Direction::Nord:      return Position(0, -1);
			case Direction::NordEast:  return Position(1, -1);
			case Direction::East:      return Position(1, 0);
			case Direction::SouthEast: return Position(1, 1);
			case Direction::South:     return Position(0, 1);
			case Direction::SouthWest: return Position(-1, 1);
			case Direction::West:      return Position(-1, 0);
			case Direction::NordWest:  return Position(-1, -1);
			default:
				d = randDirection();
				break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \brief normalize value of direction
///
/// Normalize any value to the direction enum
Direction Battle::normalizeDirection(int val)
{
	while (val < static_cast<int>(Direction::Nord)) {
		val += 8;
	}

	while (val > static_cast<int>(Direction::NordWest)) {
		val -= 8;
	}

	return static_cast<Direction>(val);
}


/// \brief Create the priority array of direction.
///
/// The array will sorted by priority of select direction, from selected to reverse direction.
/// Middle values will added by random order (clockwise or counter-clockwise)
std::vector<Direction> Battle::createDirectionArray(Direction dir)
{
	std::vector<Direction> array;
	std::uniform_int_distribution<std::mt19937::result_type> dist0_1(0, 1);
	int clockwise = dist0_1(m_randGenerator);
	int val = static_cast<int>(dir);

	array.push_back(dir);
	array.push_back(normalizeDirection(clockwise ? val + 1 : val - 1));
	array.push_back(normalizeDirection(clockwise ? val - 1 : val + 1));
	array.push_back(normalizeDirection(clockwise ? val + 2 : val - 2));
	array.push_back(normalizeDirection(clockwise ? val - 2 : val + 2));
	array.push_back(normalizeDirection(clockwise ? val + 3 : val - 3));
	array.push_back(normalizeDirection(clockwise ? val - 3 : val + 3));
	array.push_back(normalizeDirection(val + 4));

	return array;
}

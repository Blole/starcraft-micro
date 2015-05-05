#include "search/units/unit.hpp"
#include "search/gamestate.hpp"
#include "search/actions/attack.hpp"
#include "search/units/marine.hpp"

using namespace Bot::Search;

Unit::Unit(BWAPI::Unit bwapiUnit, id_t id)
	: id(id)
	, hp(bwapiUnit->getHitPoints())
	, isMoving(false)
	, isAttackFrame(false)
	, groundWeaponCooldown(false)
{
}

Unit* Unit::create(GameState* state, BWAPI::Unit bwapiUnit, id_t id)
{
	Unit* unit;
	switch (bwapiUnit->getType().getID())
	{
	case BWAPI::UnitTypes::Enum::Terran_Marine:
		unit = new Terran_Marine(bwapiUnit, id);
	default:
		throw std::runtime_error("only marines supported for MCTS so far.");
	}

	unit->firstFrameInitToAddAlreadyActiveEffects(state);

	return unit;
}

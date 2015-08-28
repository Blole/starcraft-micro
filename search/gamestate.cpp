#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/effects/effect.hpp"

using namespace Bot::Search;

GameState::GameState(vector<BWAPI::Unit> playerBwapiUnits, vector<BWAPI::Unit> enemyBwapiUnits)
	: frame(0)
	, playerUnitCount(playerBwapiUnits.size())
{
	id_t id = 0;

	for (BWAPI::Unit playerUnit : playerBwapiUnits)
		units.push_back(Unit::create(*this, playerUnit, id++));

	for (BWAPI::Unit enemyUnit : enemyBwapiUnits)
		units.push_back(Unit::create(*this, enemyUnit, id++));
}

GameState::GameState(const GameState& o)
	: frame(o.frame)
	, playerUnitCount(o.playerUnitCount)
{
	for (auto& unit : o.units)
		units.emplace_back(unit->clone());
	for (const vector<shared_ptr<Effect>>& o_frameEffects : o.pendingEffects)
	{
		pendingEffects.emplace_back();
		for (const shared_ptr<Effect>& effect : o_frameEffects)
			pendingEffects.back().emplace_back(effect);
	}
}

void GameState::advanceFrames(unsigned int framesToAdvance)
{
	for (unsigned int i=0; i<framesToAdvance; i++)
	{
		frame++;
		vector<shared_ptr<Effect>> effects = pendingEffects.front();
		pendingEffects.pop_front();

		for (auto& effect : effects)
			effect->applyTo(*this);
	}
}

void GameState::queueEffect(unsigned int frameOffset, shared_ptr<Effect> effect)
{
	if (frameOffset == 0)
		effect->applyTo(*this);
	else
	{
		if (pendingEffects.size() < frameOffset)
			pendingEffects.resize(frameOffset);
		pendingEffects[frameOffset-1].push_back(effect);
	}
}

bool GameState::isTerminal()
{
	static auto isAlive = [](const unique_ptr<Unit>& u) {return u->isAlive();};
	return
		getFrame() > 100 ||
		std::none_of(playerUnits().begin(), playerUnits().end(), isAlive) ||
		std::none_of(enemyUnits().begin(),  enemyUnits().end(),  isAlive);
}

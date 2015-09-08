#pragma once
#include "common/common.hpp"
#include "common/units/unit.hpp"

namespace Bot { namespace Search
{
	class Effect;

	class GameState final
	{
	private:
		struct GameStateUnitContainer
		{
			unordered_map<BWAPI::Unit, Unit*> unitMap;
			vector<unique_ptr<Unit>> units;
			vector<Unit*> playerUnits;
			vector<Unit*> enemyUnits;
			GameStateUnitContainer(const vector<Unit*>& playerUnits, const vector<Unit*>& enemyUnits);
			GameStateUnitContainer(GameStateUnitContainer&& other)
				: unitMap(other.unitMap)
				, units(std::move(other.units))
				, playerUnits(other.playerUnits)
				, enemyUnits(other.enemyUnits)
			{}
		};
	private:
		unsigned int frame_;
		deque<vector<shared_ptr<Effect>>> pendingEffects;

	private:
		const unordered_map<BWAPI::Unit, Unit*> unitMap;
	public:
		const vector<unique_ptr<Unit>> units;
		const vector<Unit*> playerUnits;
		const vector<Unit*> enemyUnits;

		Unit& get(BWAPI::Unit bwapiUnit)
		{
			return *unitMap.at(bwapiUnit);
		}

	private:
		GameState(GameStateUnitContainer u, unsigned int frame_);
	public:
		GameState(const vector<Unit*>& playerUnits, const vector<Unit*>& enemyUnits);
		GameState(const GameState& other);
		~GameState() {}
		
	public:
		const vector<Unit*>& teamunits(bool player) const
		{
			if (player)
				return playerUnits;
			else
				return enemyUnits;
		}

	public:
		unsigned int frame() const { return frame_; }

		void advanceFrames(unsigned int framesToAdvance);
		void queueEffect(unsigned int frameOffset, shared_ptr<Effect> effect);
	};
}}

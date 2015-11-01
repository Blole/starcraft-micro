#pragma once
#include "common/common.hpp"

namespace Bot
{
	struct Squad;
	class Effect;
	class GameState;

	class Unit
	{
	private:
		int hp_;

	public:
		const int id;
		const BWAPI::Unit bwapiUnit;
		const bool isPlayer;
		
		Squad* squad;
		BWAPI::Position pos;
		bool isMoving;
		int moveCooldown;
		int groundWeaponCooldown;

	protected:
		Unit(BWAPI::Unit bwapiUnit)
			: id(bwapiUnit->getID())
			, bwapiUnit(bwapiUnit)
			, isPlayer(bwapiUnit->getPlayer() == Broodwar->self())
			, squad(nullptr)
			, hp_(-1000)
		{}
	public:
		virtual void onFrame();
		virtual ~Unit() {}

	public:
		bool isAlive() const { return hp_ > 0; }
		unsigned int hp() const { return hp_; }
		void takeDamage(int damage)
		{
			hp_ = std::max(hp_ - damage, 0);
		}

		virtual shared_ptr<Effect> getActualOrders() = 0;
		virtual const BWAPI::UnitType& unitType() = 0;
		virtual vector<shared_ptr<Effect>> possibleActions(const GameState& state) const = 0;
		virtual Unit* clone() const = 0;
		void simulateOneFrameForward(GameState& state);

	public:
		static Unit& get(const BWAPI::Unit bwapiUnit)
		{
			static const int key = 87073;
			void* ptr = bwapiUnit->getClientInfo(key);
			if (!ptr)
			{
				ptr = Unit::create(bwapiUnit);
				bwapiUnit->setClientInfo(ptr, key);
			}
			return *static_cast<Unit*>(ptr);
		}
	private:
		static Unit* create(BWAPI::Unit bwapiUnit);
	};
}

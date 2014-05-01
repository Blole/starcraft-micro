#include "behaviors/AttackClosest.hpp"
#include "common/GameUnit.hpp"
#include "common/PUnit.hpp"

using namespace BWAPI;

AttackClosest::AttackClosest()
{
}

void AttackClosest::init(void* agent)
{
	first = true;
	PUnit* pUnit = (PUnit*)agent;
	pUnit->attackTarget(pUnit->getClosestEnemy(),false);
}

BEHAVIOR_STATUS AttackClosest::execute(void* agent)
{
	PUnit* pUnit = (PUnit*)agent;
	BWAPI::Unit unit = pUnit->unit;

	static bool hasStartAttack = false;

	if(hasStartAttack)
	{
		Broodwar->drawLineMap(pUnit->getPosition(),pUnit->target.getUnit()->getPosition(),Color(255,0,0));
		//if (!pUnit->target.getUnit()->exists())
		//	return BT_SUCCESS;
		if(!pUnit->target.getUnit()->exists() || !pUnit->isAttacking())
			return BT_SUCCESS;
		
		return BT_RUNNING;
	}
	else
	{
		if(pUnit->isAttacking())
		{
			hasStartAttack = true;
		}

		if(!hasStartAttack)
		{
			Unit target = pUnit->getClosestEnemy();
			if(pUnit->target.getUnit()!=target)
				pUnit->attackTarget(target,false);
		}
		Broodwar->drawLineMap(pUnit->getPosition(),pUnit->target.getUnit()->getPosition(),Color(255,0,0));
		return BT_RUNNING;
	}
}

#include "CombatSyetem.h"
#include "Entity.h"

int CombatSyetem::CalculateDmg(Entity* attacker, Entity* defender)
{
	return max(attacker->GetAttDmg() - defender->GetDefense(), 0);
}

void CombatSyetem::ProcessAttack(Entity* attacker, Entity* defender)
{
	if (!attacker || !defender)	return;
	if (attacker->GetType() == defender->GetType())	return;

	int dmg = CalculateDmg(attacker, defender);
	defender->TakeDamage(dmg);
	if (defender->GetType() == EntityType::MONSTER && !defender->IsAlive())
		attacker->TakeExp(defender->GetExp());
}

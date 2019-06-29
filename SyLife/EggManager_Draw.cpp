#include "EggManager.h"
#include "Egg.h"
#include "Cell.h"

void EggManager::Draw()
{
	for (const auto& e : g_eggManagerPtr->GetEggStates())
		e->Draw();
}
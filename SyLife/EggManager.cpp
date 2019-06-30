#include "EggManager.h"
#include "Egg.h"

unique_ptr<EggManager> g_eggManagerPtr;

const shared_ptr<EggState>& EggManager::AddEggState(const shared_ptr<CellModel>& cellModel)
{
	const auto& e = GetEggStates().emplace_back(make_shared<EggState>());
	e->SetCellModel(cellModel);
	e->SetMass(1.0);

	m_indexer.AddParticle(e);

	return e;
}

void EggManager::Update()
{
	for (const auto& e : GetEggStates())
	{
		if (!e->IsDestroyed())
		{
			e->UpdateEgg();
			e->UpdateParticle();
		}
	}

	m_indexer.Update();
}

void EggManager::Draw()
{
	for (const auto& e : GetEggStates())
		if (!e->IsDestroyed()) e->Draw();
}

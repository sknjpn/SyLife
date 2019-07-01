#include "Molecule.h"
#include "MoleculeManager.h"
#include "AssetManager.h"

void MoleculeState::UpdateMolecule()
{
	// Amino Acid‚Ì•ª‰ð
	if (m_model == g_assetManagerPtr->GetModel<MoleculeModel>("Amino Acid") && rand() % 10 == 0)
	{
		g_moleculeManagerPtr->AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Nitrogen"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));
		g_moleculeManagerPtr->AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Carbon"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));
		g_moleculeManagerPtr->AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Oxygen"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));

		Destroy();
	}
}

void MoleculeState::Draw()
{
	Circle(GetPosition().x, GetPosition().y, GetRadius())
		.draw(ColorF(m_model->GetColor(), 0.5))
		.drawFrame(1.0, ColorF(1.0, 0.5));
}
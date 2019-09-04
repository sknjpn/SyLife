#include "Molecule.h"

void MoleculeState::UpdateMolecule()
{
	// Amino Acid‚Ì•ª‰ð
	if (m_model->GetName() == "Amino Acid" && RandomBool(0.1))
	{
		g_moleculeManagerPtr->AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Nitrogen"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));
		g_moleculeManagerPtr->AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Carbon"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));
		g_moleculeManagerPtr->AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Oxygen"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));

		Destroy();
		return;
	}

	// if (RandomBool(0.01)) Destroy();
}

void MoleculeState::Draw()
{
	static Texture particle(U"assets/image/particle.png", TextureDesc::Mipped);

	particle.resized(m_model->GetRadius() * 2.0, m_model->GetRadius() * 2.0).drawAt(GetPosition(), m_model->GetColor());
	/*
	Circle(GetPosition().x, GetPosition().y, GetRadius())
		.draw(ColorF(m_model->GetColor(), 0.5));
		//.drawFrame(1.0, ColorF(1.0, 0.5));
		*/
}
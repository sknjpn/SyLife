#include "SystemManager.h"
#include "FieldManager.h"
#include "CellManager.h"
#include "MoleculeManager.h"
#include "AssetManager.h"

void SystemManager::Draw()
{

	g_fieldManagerPtr->Draw();

	g_moleculeManagerPtr->Draw();

	g_cellManagerPtr->Draw();

	// UI‚Ì•`‰æ
	/*{
		static s3d::Font printFont(16);
		s3d::Rect(192, 80).draw(s3d::ColorF(s3d::Palette::Gray, 0.75));

		for (int i = 0; i < g_assetManagerPtr->GetModels<MoleculeModel>().size(); ++i)
		{
			const auto& m = g_assetManagerPtr->GetModels<MoleculeModel>()[i];

			printFont(s3d::Unicode::Widen(m->m_name + " size=" + to_string(g_moleculeManagerPtr->NumMolecule(m)))).draw(0 + 1, i * 16 + 1, s3d::Palette::White);
			printFont(s3d::Unicode::Widen(m->m_name + " size=" + to_string(g_moleculeManagerPtr->NumMolecule(m)))).draw(0, i * 16, m->m_color);
		}
	}*/
}
#include "AssetViewer.h"
#include "AssetManager.h"
#include "CellModel.h"
#include "MoleculeModel.h"
#include "Part.h"
#include "Body.h"
#include "Equipment.h"
#include "Module.h"

void AssetViewer::Update()
{
	m_drawPos = s3d::Vec2::Zero();

	DrawModels<CellModel>("CellModel");
	DrawModels<MoleculeModel>("MoleculeModel");
	DrawModels<BodyModel>("BodyModel");
	DrawModels<EquipmentModel>("EquipmentModel");
	DrawModels<ModuleModel>("ModuleModel");

	// Save
	if (s3d::KeyControl.pressed() && s3d::KeyS.down() && m_selectedModel != nullptr)
		write_json(m_selectedModel->GetFilepath(), m_selectedModel->Save());
}

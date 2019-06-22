#include "PartPaletteViewer.h"

#include "BodyModel.h"
#include "EquipmentModel.h"
#include "ModuleModel.h"

shared_ptr<PartModel>	PartPaletteViewer::m_selectedPart;

void PartPaletteViewer::Update()
{
	m_drawPos = s3d::Vec2::Zero();

	DrawModels<BodyModel>("BodyModel");
	DrawModels<EquipmentModel>("EquipmentModel");
	DrawModels<ModuleModel>("ModuleModel");
}
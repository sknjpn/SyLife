#include "PartPaletteViewer.h"

#include "Body.h"
#include "Equipment.h"
#include "Module.h"

shared_ptr<PartModel>	PartPaletteViewer::m_selectedPart;

void PartPaletteViewer::Update()
{
	m_drawPos = Vec2::Zero();

	DrawModels<BodyModel>("BodyModel");
	DrawModels<EquipmentModel>("EquipmentModel");
	DrawModels<ModuleModel>("ModuleModel");
}
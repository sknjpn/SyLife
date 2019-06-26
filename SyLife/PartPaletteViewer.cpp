#include "PartPaletteViewer.h"

#include "Body.h"
#include "Equipment.h"
#include "Module.h"

shared_ptr<PartModel>	PartPaletteViewer::m_selectedPart;

void PartPaletteViewer::Update()
{
	s3d::Rect(m_drawRect.size.asPoint()).draw(s3d::Color(11, 22, 33, 192));

	m_drawPos = s3d::Vec2::Zero();

	DrawModels<BodyModel>("BodyModel");
	DrawModels<EquipmentModel>("EquipmentModel");
	DrawModels<ModuleModel>("ModuleModel");
}
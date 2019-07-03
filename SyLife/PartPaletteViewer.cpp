#include "PartPaletteViewer.h"
#include "Part.h"
#include "Body.h"
#include "Equipment.h"
#include "Module.h"

void	PartPaletteViewer::Update()
{
	DrawModels<BodyModel>("BodyModel");
	DrawModels<EquipmentModel>("EquipmentModel");
	DrawModels<ModuleModel>("ModuleModel");
}
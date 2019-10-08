#include "AddModelViewer.h"

#include "Molecule.h"
#include "CellModel.h"
#include "PartModel.h"
#include "BodyModel.h"
#include "EquipmentModel.h"
#include "ModuleModel.h"
#include "WingModel.h"

void AddModelViewer::update()
{
	DrawModels<MoleculeModel>("MoleculeModel");
	DrawModels<CellModel>("CellModel");
	DrawModels<PartModel>("PartModel");
	DrawModels<BodyModel>("BodyModel");
	DrawModels<EquipmentModel>("EquipmentModel");
	DrawModels<ModuleModel>("ModuleModel");
	DrawModels<EyeModel>("EyeModel");
	DrawModels<ChloroplastModel>("ChloroplastModel");
	DrawModels<WingModel>("WingModel");
}

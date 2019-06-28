#include "AddModelViewer.h"

#include "Model.h"
#include "Molecule.h"
#include "Cell.h"
#include "Part.h"
#include "Body.h"
#include "Equipment.h"
#include "Module.h"
#include "Eye.h"
#include "Chloroplast.h"
#include "Wing.h"

void AddModelViewer::Update()
{
	m_drawPos = s3d::Vec2::Zero();

	DrawModels<Model>("Model");
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

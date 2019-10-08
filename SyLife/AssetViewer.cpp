#include "AssetViewer.h"
#include "AddModelViewer.h"
#include "CellModel.h"
#include "Molecule.h"
#include "PartModel.h"
#include "BodyModel.h"
#include "EquipmentModel.h"
#include "ModuleModel.h"

#include "ViewerManager.h"

void AssetViewer::update()
{
	DrawModels<CellModel>("CellModel");
	DrawModels<MoleculeModel>("MoleculeModel");
	DrawModels<BodyModel>("BodyModel");
	DrawModels<EquipmentModel>("EquipmentModel");
	DrawModels<ModuleModel>("ModuleModel");

	// AddNewModel
	{
		static Font font(15, Typeface::Bold);

		auto f = font(U"新しいModelの追加");

		f.region().draw(ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
		f.draw();

		if (f.region().leftClicked() && !g_viewerManagerPtr->HasViewer<AddModelViewer>()) g_viewerManagerPtr->makeViewer<AddModelViewer>();

		MoveDrawPos(0, 16);
	}

	// save
	if (KeyControl.pressed() && KeyS.down() && m_selectedModel != nullptr)
	{
		ptree pt;
		m_selectedModel->save(pt);
		write_json(m_selectedModel->getFilepath(), pt);
	}
}

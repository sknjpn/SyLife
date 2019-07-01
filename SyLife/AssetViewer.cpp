#include "AssetViewer.h"
#include "AssetManager.h"
#include "AddModelViewer.h"
#include "Cell.h"
#include "Molecule.h"
#include "Part.h"
#include "Body.h"
#include "Equipment.h"
#include "Module.h"
#include "ViewerManager.h"

void AssetViewer::Update(bool isMouseOver)
{
	m_drawPos = Vec2::Zero();

	DrawModels<CellModel>("CellModel");
	DrawModels<MoleculeModel>("MoleculeModel");
	DrawModels<BodyModel>("BodyModel");
	DrawModels<EquipmentModel>("EquipmentModel");
	DrawModels<ModuleModel>("ModuleModel");

	// AddNewModel
	{
		static Font font(15, Typeface::Bold);

		auto t = Transformer2D(Mat3x2::Translate(m_drawPos), true);
		auto f = font(U"V‚µ‚¢Model‚Ì’Ç‰Á");

		f.region().draw(ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
		f.draw();

		if (f.region().leftClicked() && !g_viewerManagerPtr->HasViewer<AddModelViewer>()) g_viewerManagerPtr->MakeViewer<AddModelViewer>();

		m_drawPos.moveBy(0.0, 16.0);
	}

	// Save
	if (KeyControl.pressed() && KeyS.down() && m_selectedModel != nullptr)
	{
		ptree pt;
		m_selectedModel->Save(pt);
		write_json(m_selectedModel->GetFilepath(), pt);
	}
}

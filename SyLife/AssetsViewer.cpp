#include "AssetsViewer.h"
#include "AddModelViewer.h"
#include "CellAsset.h"
#include "Element.h"
#include "PartAsset.h"
#include "BodyAsset.h"
#include "EquipmentAsset.h"
#include "ModuleAsset.h"

#include "ViewerManager.h"

void AssetsViewer::update()
{
	drawModels<CellAsset>("CellAsset");
	drawModels<ElementAsset>("ElementAsset");
	drawModels<BodyAsset>("BodyAsset");
	drawModels<EquipmentAsset>("EquipmentAsset");
	drawModels<ModuleAsset>("ModuleAsset");

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

#include "AssetsViewer.h"

#include "AddModelViewer.h"
#include "AssetEditor.h"

#include "CellAsset.h"
#include "ElementAsset.h"
#include "PartAsset.h"
#include "BodyAsset.h"
#include "EquipmentAsset.h"
#include "ModuleAsset.h"

void AssetsViewer::update()
{
	drawAssets<CellAsset>("CellAsset");
	drawAssets<ElementAsset>("ElementAsset");
	drawAssets<BodyAsset>("BodyAsset");
	drawAssets<EquipmentAsset>("EquipmentAsset");
	drawAssets<ModuleAsset>("ModuleAsset");

	// AddNewModel
	{
		static Font font(15, Typeface::Bold);

		auto f = font(U"新しいModelの追加");

		f.region().draw(ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
		f.draw();

		//if (f.region().leftClicked() && !g_viewerManagerPtr->hasViewer<AddModelViewer>()) g_viewerManagerPtr->makeViewer<AddModelViewer>();

		moveDrawPos(0, 16);
	}

	// save
	if (KeyControl.pressed() && KeyS.down() && m_selectedAsset != nullptr)
	{
		ptree pt;
		m_selectedAsset->save(pt);
		write_json(m_selectedAsset->getFilepath(), pt);
	}
}

void AssetsViewer::setSelectedAsset(const shared_ptr<Asset>& asset)
{
	// ModelEditorを消す
	//g_viewerManagerPtr->deleteViewer<AssetEditor>();

	// 対象のViewを追加
	asset->makeViewer();

	// 対象モデルの設定
	m_selectedAsset = asset;
}
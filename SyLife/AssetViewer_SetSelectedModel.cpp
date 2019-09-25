#include "AssetViewer.h"

#include "PartPaletteViewer.h"
#include "Cell.h"
#include "Molecule.h"
#include "AssemblyViewer.h"
#include "Body.h"
#include "Equipment.h"
#include "Module.h"
#include "PartShapeViewer.h"

void AssetViewer::SetSelectedModel(const shared_ptr<Model>& model)
{
	// ModelViewerを消す
	g_viewerManagerPtr->DeleteViewer<ModelViewer>();

	// 対象のViewを追加
	model->MakeViewer();

	// 対象モデルの設定
	m_selectedModel = model;
}
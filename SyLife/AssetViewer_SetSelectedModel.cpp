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
	// 自分以外のViewを殺す
	g_viewerManagerPtr->DeleteViewer<PartPaletteViewer>();
	g_viewerManagerPtr->DeleteViewer<PartViewer>();
	g_viewerManagerPtr->DeleteViewer<CellViewer>();
	g_viewerManagerPtr->DeleteViewer<MoleculeViewer>();
	g_viewerManagerPtr->DeleteViewer<PartShapeViewer>();

	// 対象のViewを追加
	if (dynamic_pointer_cast<CellModel>(model) != nullptr) g_viewerManagerPtr->MakeViewer<PartPaletteViewer>(dynamic_pointer_cast<CellModel>(model));
	if (dynamic_pointer_cast<CellModel>(model) != nullptr) g_viewerManagerPtr->MakeViewer<AssemblyViewer>(dynamic_pointer_cast<CellModel>(model));
	if (dynamic_pointer_cast<CellModel>(model) != nullptr) g_viewerManagerPtr->MakeViewer<CellViewer>(dynamic_pointer_cast<CellModel>(model));
	if (dynamic_pointer_cast<MoleculeModel>(model) != nullptr) g_viewerManagerPtr->MakeViewer<MoleculeViewer>(dynamic_pointer_cast<MoleculeModel>(model));
	if (dynamic_pointer_cast<BodyModel>(model) != nullptr) g_viewerManagerPtr->MakeViewer<BodyViewer>(dynamic_pointer_cast<BodyModel>(model));
	if (dynamic_pointer_cast<EquipmentModel>(model) != nullptr) g_viewerManagerPtr->MakeViewer<EquipmentViewer>(dynamic_pointer_cast<EquipmentModel>(model));
	if (dynamic_pointer_cast<ModuleModel>(model) != nullptr) g_viewerManagerPtr->MakeViewer<ModuleViewer>(dynamic_pointer_cast<ModuleModel>(model));
	if (dynamic_pointer_cast<PartModel>(model) != nullptr) g_viewerManagerPtr->MakeViewer<PartShapeViewer>(dynamic_pointer_cast<PartModel>(model));

	// 対象モデルの設定
	m_selectedModel = model;
}
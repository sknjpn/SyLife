#include "AssetViewer.h"

#include "ViewerManager.h"

#include "PartPaletteViewer.h"
#include "CellPropertyViewer.h"
#include "MoleculePropertyViewer.h"
#include "AssemblyViewer.h"
#include "BodyModel.h"
#include "BodyPropertyViewer.h"
#include "EquipmentModel.h"
#include "EquipmentPropertyViewer.h"
#include "ModuleModel.h"
#include "ModulePropertyViewer.h"
#include "PartShapeViewer.h"

void AssetViewer::SetSelectedModel(const shared_ptr<Model>& model)
{
	// é©ï™à»äOÇÃViewÇéEÇ∑
	while (g_viewerManagerPtr->m_viewers.size() > 1) g_viewerManagerPtr->m_viewers.pop_back();

	// ëŒè€ÇÃViewÇí«â¡
	if (dynamic_pointer_cast<CellModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<PartPaletteViewer>(dynamic_pointer_cast<CellModel>(model));
	if (dynamic_pointer_cast<CellModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<AssemblyViewer>(dynamic_pointer_cast<CellModel>(model));
	if (dynamic_pointer_cast<CellModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<CellPropertyViewer>(dynamic_pointer_cast<CellModel>(model));
	if (dynamic_pointer_cast<MoleculeModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<MoleculePropertyViewer>(dynamic_pointer_cast<MoleculeModel>(model));
	if (dynamic_pointer_cast<BodyModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<BodyPropertyViewer>(dynamic_pointer_cast<BodyModel>(model));
	if (dynamic_pointer_cast<EquipmentModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<EquipmentPropertyViewer>(dynamic_pointer_cast<EquipmentModel>(model));
	if (dynamic_pointer_cast<ModuleModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<ModulePropertyViewer>(dynamic_pointer_cast<ModuleModel>(model));
	if (dynamic_pointer_cast<PartModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<PartShapeViewer>(dynamic_pointer_cast<PartModel>(model));

	// ëŒè€ÉÇÉfÉãÇÃê›íË
	m_selectedModel = model;
}
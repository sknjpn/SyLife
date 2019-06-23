#include "AssetViewer.h"

#include "ViewerManager.h"

#include "PartPaletteViewer.h"
#include "Cell.h"
#include "MoleculeViewer.h"
#include "AssemblyViewer.h"
#include "Body.h"
#include "Equipment.h"
#include "Module.h"
#include "PartShapeViewer.h"

void AssetViewer::SetSelectedModel(const shared_ptr<Model>& model)
{
	// ©•ªˆÈŠO‚ÌView‚ğE‚·
	while (g_viewerManagerPtr->m_viewers.size() > 1) g_viewerManagerPtr->m_viewers.pop_back();

	// ‘ÎÛ‚ÌView‚ğ’Ç‰Á
	if (dynamic_pointer_cast<CellModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<PartPaletteViewer>(dynamic_pointer_cast<CellModel>(model));
	if (dynamic_pointer_cast<CellModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<AssemblyViewer>(dynamic_pointer_cast<CellModel>(model));
	if (dynamic_pointer_cast<CellModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<CellViewer>(dynamic_pointer_cast<CellModel>(model));
	if (dynamic_pointer_cast<MoleculeModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<MoleculeViewer>(dynamic_pointer_cast<MoleculeModel>(model));
	if (dynamic_pointer_cast<BodyModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<BodyViewer>(dynamic_pointer_cast<BodyModel>(model));
	if (dynamic_pointer_cast<EquipmentModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<EquipmentViewer>(dynamic_pointer_cast<EquipmentModel>(model));
	if (dynamic_pointer_cast<ModuleModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<ModuleViewer>(dynamic_pointer_cast<ModuleModel>(model));
	if (dynamic_pointer_cast<PartModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<PartShapeViewer>(dynamic_pointer_cast<PartModel>(model));

	// ‘ÎÛƒ‚ƒfƒ‹‚Ìİ’è
	m_selectedModel = model;
}
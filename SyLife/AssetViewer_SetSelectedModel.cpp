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
	// ModelViewer‚ğÁ‚·
	g_viewerManagerPtr->DeleteViewer<ModelViewer>();

	// ‘ÎÛ‚ÌView‚ğ’Ç‰Á
	model->MakeViewer();

	// ‘ÎÛƒ‚ƒfƒ‹‚Ìİ’è
	m_selectedModel = model;
}
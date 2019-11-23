#include "SetupViewer.h"

#include "WorldManager.h"
#include "AssetManager.h"
#include "CellManager.h"
#include "EggManager.h"
#include "ChipManager.h"
#include "ElementManager.h"

void Main()
{
	Window::SetTitle(U"SyLife");

	g_worldManagerPtr = make_unique<WorldManager>();
	g_assetManagerPtr = make_unique<AssetManager>();
	g_cellManagerPtr = make_unique<CellManager>();
	g_eggManagerPtr = make_unique<EggManager>();
	g_chipManagerPtr = make_unique<ChipManager>();
	g_elementManagerPtr = make_unique<ElementManager>();

	g_assetManagerPtr->init();
	g_chipManagerPtr->init();

	Viewer::GetRootViewer()->addChildViewer<SetupViewer>();
	Viewer::Run();
}

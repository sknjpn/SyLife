#include "SetupViewer.h"

#include "AssetManager.h"
#include "CellManager.h"
#include "FieldManager.h"
#include "EggManager.h"
#include "WaveManager.h"
#include "ChipManager.h"
#include "ElementManager.h"

void Main()
{
	Window::SetTitle(U"SyLife");

	g_assetManagerPtr = make_unique<AssetManager>();
	g_cellManagerPtr = make_unique<CellManager>();
	g_fieldManagerPtr = make_unique<FieldManager>();
	g_eggManagerPtr = make_unique<EggManager>();
	g_waveManagerPtr = make_unique<WaveManager>();
	g_chipManagerPtr = make_unique<ChipManager>();
	g_elementManagerPtr = make_unique<ElementManager>();

	g_assetManagerPtr->init();
	g_fieldManagerPtr->init();
	g_chipManagerPtr->init();

	Viewer::GetRootViewer()->addChildViewer<SetupViewer>();
	Viewer::Run();
}

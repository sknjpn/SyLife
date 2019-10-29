#include "TitleViewer.h"

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

	// フルスクリーンの利用
	/*
	{
		// 現在のモニタで使用可能なフルスクリーン解像度を取得
		const Array<Size> resolutions = Graphics::GetFullscreenResolutions();

		if (!resolutions) throw Error(U"フルスクリーンモードを利用できません。");

		// 最大のフルスクリーン解像度にする
		size_t index = resolutions.size() - 1;
		if (!Window::SetFullscreen(true, resolutions[index])) throw Error(U"フルスクリーンモードへの切り替えに失敗しました。");
	}
	*/

	// Windowの利用
	{
		Window::Resize(1920, 1080);
		Window::SetStyle(WindowStyle::Sizable);
		Window::Maximize();
	}

	// ESCキーによる終了の防止
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);

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

	Viewer::GetRootViewer()->addChildViewer<TitleViewer>();
	Viewer::Run();
}

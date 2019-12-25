#include "SetupViewer.h"
#include "GUIButton.h"
#include "GUIChecker.h"
#include "WorldGenerateViewer.h"

void SetupViewer::startGame()
{
	// それまでの描画が残らないように修正
	for (int i = 0; i < 2; ++i)
		if (!System::Update()) break;

	// Window設定
	if (getChildViewer<GUIChecker>(U"フルスクリーン")->getValue())
	{
		// 現在のモニタで使用可能なフルスクリーン解像度を取得
		const Array<Size> resolutions = Graphics::GetFullscreenResolutions();

		if (!resolutions) throw Error(U"フルスクリーンモードを利用できません。");

		// 最大のフルスクリーン解像度にする
		size_t index = resolutions.size() - 1;
		if (!Window::SetFullscreen(true, resolutions[index])) throw Error(U"フルスクリーンモードへの切り替えに失敗しました。");
	}
	else
	{
		// 通常のWindow
		Window::Resize(1920, 1080);
		Window::SetStyle(WindowStyle::Sizable);
		//Window::Maximize();
	}

	getParentViewer()->addChildViewer<WorldGenerateViewer>();
	destroy();
}

void SetupViewer::init()
{
	Window::Resize(240, 320);
	Window::SetStyle(WindowStyle::Fixed);

	setBackgroundColor(Palette::Gray);
	setViewerSize(Scene::Size());

	addChildViewer<GUIButton>(U"始める！", [this]() { startGame(); })
		->setViewerRectInLocal(40, 260, 160, 40);

	addChildViewer<GUIChecker>()
		->setName(U"フルスクリーン")
		->setViewerRectInLocal(5, 5, 30, 30);
}

void SetupViewer::update()
{
	moveDrawPos(5, 5);

	{
		static Font font(20, Typeface::Bold);

		font(U"フルスクリーン").draw(Vec2(35, 0), Palette::Black);
		moveDrawPos(0, 30);
	}
}

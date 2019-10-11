﻿#include "TitleViewer.h"

#include "SystemManager.h"
#include "ViewerManager.h"

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
		//Window::Maximize();
	}

	// ESCキーによる終了の防止
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);

	g_systemManagerPtr = make_unique<SystemManager>();

	g_viewerManagerPtr->makeViewer<TitleViewer>();

	while (System::Update())
	{
		g_systemManagerPtr->update();

		if (KeyEscape.down()) break;
	}
}

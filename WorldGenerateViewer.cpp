#include "WorldGenerateViewer.h"
#include "WorldManager.h"
#include "ChipManager.h"
#include "GUIText.h"
#include "GUITextBox.h"
#include "GUIButton.h"

void WorldGenerateViewer::generateWorld()
{
}

void WorldGenerateViewer::init()
{
	// Chip
	{
		addChildViewer<GUIText>(U"波の設定")
			->setViewerRectInLocal(200, 100, 200, 50);

		addChildViewer<GUIButton>(U"再構築", [this]() { generateWorld(); })
		->setViewerRectInLocal(200, 150, 200, 40);
	}

	// World
	{
		addChildViewer<GUIText>(U"World名")
			->setViewerRectInLocal(200, 100, 200, 50);

		addChildViewer<GUITextBox>([this](const auto& s) { g_worldManagerPtr->setName(s); })
			->setViewerRectInLocal(100, 500, 400, 50);

		addChildViewer<GUIButton>(U"生成", [this]() { generateWorld(); })
		->setViewerRectInLocal(200, 550, 200, 40);
	}
}

void WorldGenerateViewer::update()
{
}

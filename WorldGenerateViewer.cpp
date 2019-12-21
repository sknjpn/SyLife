#include "WorldGenerateViewer.h"
#include "GUIText.h"
#include "GUITextBox.h"
#include "GUIButton.h"
#include "MainViewer.h"
#include "World.h"

void WorldGenerateViewer::generateWorld()
{
	World::Make();
	World::GetInstance()->setName(U"Hello World");

	getParentViewer()->addChildViewer<MainViewer>();

	destroy();
}

void WorldGenerateViewer::init()
{
	setBackgroundColor(Palette::White);

	// Chip
	{
		/*addChildViewer<GUIText>(U"波の設定")
			->setViewerRectInLocal(200, 100, 200, 50);

		addChildViewer<GUIButton>(U"再構築", [this]() { generateWorld(); })
		->setViewerRectInLocal(200, 150, 200, 40);*/
	}

	// World
	{
		addChildViewer<GUIText>(U"World名")
			->setViewerRectInLocal(200, 100, 200, 50);

		addChildViewer<GUITextBox>([this](const auto& s) { m_name = s; })
			->setViewerRectInLocal(100, 500, 400, 50);

		addChildViewer<GUIButton>(U"生成", [this]() { generateWorld(); })
		->setViewerRectInLocal(200, 550, 200, 40);
	}
}

void WorldGenerateViewer::update()
{
}

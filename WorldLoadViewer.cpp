#include "WorldLoadViewer.h"
#include "GUIText.h"
#include "GUITextBox.h"
#include "GUIButton.h"
#include "MainViewer.h"
#include "World.h"

void WorldLoadViewer::generateWorld()
{
	// Pathの設定
	for (int i = 1; ; ++i)
	{
		String path = Format(U"worlds/world_", i, U"/");

		if (!FileSystem::Exists(path)) break;

		World::Load(path);
	}

	if (!World::GetInstance())
	{
		terminate();
		System::Exit();
	}

	getParentViewer()->addChildViewer<MainViewer>();

	destroy();
}

void WorldLoadViewer::init()
{
	generateWorld();
}

void WorldLoadViewer::update()
{
}

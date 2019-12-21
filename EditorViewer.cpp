#include "EditorViewer.h"
#include "Assets.h"

void EditorViewer::init()
{
	Assets::Load(U"resources/assets/");

	addChildViewer<AssetList>();

	setBackgroundColor(Palette::White);
}

void EditorViewer::update()
{
}

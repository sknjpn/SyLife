#include "EditorViewer.h"

void EditorViewer::init()
{
	addChildViewer<AssetList>();

	setBackgroundColor(Palette::White);
}

void EditorViewer::update()
{
}

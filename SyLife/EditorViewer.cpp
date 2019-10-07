#include "EditorViewer.h"

#include "ViewerManager.h"

#include "AssetViewer.h"

EditorViewer::EditorViewer()
{

}

void EditorViewer::Init()
{
	g_viewerManagerPtr->makeViewer<AssetViewer>();
}

void EditorViewer::update()
{

}

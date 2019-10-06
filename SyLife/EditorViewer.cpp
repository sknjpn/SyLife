#include "EditorViewer.h"

#include "ViewerManager.h"

#include "AssetViewer.h"

EditorViewer::EditorViewer()
{

}

void EditorViewer::Init()
{
	g_viewerManagerPtr->MakeViewer<AssetViewer>();
}

void EditorViewer::update()
{

}

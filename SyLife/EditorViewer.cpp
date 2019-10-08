#include "EditorViewer.h"

#include "ViewerManager.h"

#include "AssetsViewer.h"

EditorViewer::EditorViewer()
{

}

void EditorViewer::Init()
{
	g_viewerManagerPtr->makeViewer<AssetsViewer>();
}

void EditorViewer::update()
{

}

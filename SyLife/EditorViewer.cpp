#include "EditorViewer.h"

#include "ViewerManager.h"

#include "AssetsViewer.h"

EditorViewer::EditorViewer()
{

}

void EditorViewer::init()
{
	g_viewerManagerPtr->makeViewer<AssetsViewer>();
}

void EditorViewer::update()
{

}

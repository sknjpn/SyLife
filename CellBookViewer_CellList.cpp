#include "CellBookViewer.h"
#include "GUISlider.h"

void CellBookViewer::CellList::init()
{
	addChildViewer<GUISlider>(800);
}

void CellBookViewer::CellList::update()
{
	
}
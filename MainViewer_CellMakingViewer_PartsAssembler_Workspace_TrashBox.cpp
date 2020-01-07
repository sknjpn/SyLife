#include "MainViewer.h"

void MainViewer::CellMakingViewer::PartsAssembler::Workspace::TrashBox::init()
{
	setViewerRectInLocal(800 - 110, 800 - 110, 100, 100);
}

void MainViewer::CellMakingViewer::PartsAssembler::Workspace::TrashBox::update()
{
	RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

	m_textureTrashBox.drawAt(getViewerSize() / 2.0,m_isSelected ? ColorF(Palette::Red) : ColorF(0.5));
}

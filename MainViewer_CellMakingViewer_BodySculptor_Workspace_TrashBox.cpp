#include "MainViewer.h"

void MainViewer::CellMakingViewer::BodySculptor::Workspace::TrashBox::init()
{
	setViewerRectInLocal(800 - 110, 800 - 110, 100, 100);
}

void MainViewer::CellMakingViewer::BodySculptor::Workspace::TrashBox::update()
{
	RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

	m_textureTrashBox.drawAt(getViewerSize() / 2.0, ColorF(0.5));
}

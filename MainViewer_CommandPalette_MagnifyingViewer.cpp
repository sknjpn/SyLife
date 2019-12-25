#include "MainViewer.h"

void MainViewer::CommandPalette::MagnifyingViewer::init()
{
	setViewerSize(80, 40);
}

void MainViewer::CommandPalette::MagnifyingViewer::update()
{
	Rect r(0, 0, 40, 40);
	Rect l(40, 0, 40, 40);

	r.draw(ColorF(Palette::Red, 0.5)).drawFrame(2.0, Palette::Red);
	l.draw(ColorF(Palette::Blue, 0.5)).drawFrame(2.0, Palette::Blue);

	static Font font(30);
	font(U"+").drawAt(20, 20);
	font(U"-").drawAt(60, 20);

	if (r.leftPressed())
	{
		getParentViewer()->getParentViewer()->getChildViewer<FieldViewer>()->getCamera().zoomIn();
	}
	if (l.leftPressed())
	{
		getParentViewer()->getParentViewer()->getChildViewer<FieldViewer>()->getCamera().zoomOut();
	}
}

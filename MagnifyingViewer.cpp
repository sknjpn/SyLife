#include "MagnifyingViewer.h"
#include "FieldViewer.h"

void MagnifyingViewer::init()
{
	setViewerSize(80, 40);
}

void MagnifyingViewer::update()
{
	Rect r(0, 0, 40, 40);
	Rect l(40, 0, 40, 40);

	r.draw(ColorF(Palette::Red, 0.5)).drawFrame(2.0, Palette::Red);
	l.draw(ColorF(Palette::Blue, 0.5)).drawFrame(2.0, Palette::Blue);

	static Font font(30);
	font(U"+").drawAt(20, 20);
	font(U"-").drawAt(60, 20);

	if (r.leftClicked())
	{
		getParentViewer<FieldViewer>()->getCamera().zoomIn();
	}
	if (l.leftClicked())
	{
		getParentViewer<FieldViewer>()->getCamera().zoomOut();
	}
}

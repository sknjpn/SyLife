#include "ScalingViewer.h"

void ScalingViewer::init()
{
	setBackgroundColor(Color(0, 0));
	setViewerSize(80, 320);
}

void ScalingViewer::update()
{
	Rect(80, 320).rounded(10)
		.draw(Palette::White)
		.drawFrame(2.0, 0.0, Palette::Black);
}

#include "MainViewer.h"

void MainViewer::CommandPalette::SpeedControllerViewer::init()
{
	setViewerSize(200, 40);
	setViewerPosInLocal(getParentViewer()->getViewerSize().x - 220, 20);
}

void MainViewer::CommandPalette::SpeedControllerViewer::update()
{
	auto region = RectF(200, 40);

	if (isMouseover() && region.leftClicked())
	{
		m_isHighSpeed = !m_isHighSpeed;
		getParentViewer()->getParentViewer()->getChildViewer<FieldViewer>()->m_isHighSpeed = m_isHighSpeed;
	}
	region.rounded(8).draw(ColorF(0.25));

	{
		static Font font(32, Typeface::Bold);
		font(U"加速 x", getParentViewer()->getParentViewer()->getChildViewer<FieldViewer>()->m_frameCount).draw(Vec2(5, -3), m_isHighSpeed ? ColorF(Palette::Red) : ColorF(0.5));
	}
}

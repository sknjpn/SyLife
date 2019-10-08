#include "ReleaseViewer.h"

#include "CellAsset.h"
#include "PartConfig.h"

ReleaseViewer::ReleaseViewer()
{
	SetDrawRect(Scene::Width() - 400, 320, 300, 100);
}

void ReleaseViewer::update()
{
	Circle circle(50, 50, 45);

	circle
		.draw(circle.mouseOver() ? Palette::Orange : Palette::Skyblue)
		.drawFrame(4.0, Palette::Black);

	if (m_isDragged) circle.draw(Palette::Red);

	if (circle.leftClicked()) m_isDragged = true;
	if (!MouseL.pressed()) m_isDragged = false;

	// part
	{
		auto t1 = Transformer2D(Mat3x2::Scale(45.0 / m_cellAsset->getRadius() / 2.0).translated(circle.center));

		for (const auto& p : m_cellAsset->getPartConfigs())
		{
			auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
				.translated(p->getPosition().x, p->getPosition().y));

			for (const auto& s : p->getModel()->getShapes())
				s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
		}
	}
}

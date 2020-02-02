#include "MainViewer.h"
#include "Shape.h"
#include "PartAsset.h"
#include "PartAsset_Body.h"

void MainViewer::CellMakingViewer::BodySculptor::ColorSelector::init()
{
	setViewerRectInLocal(1010, 20, 180, 980);

	m_selectedColor = m_colors.choice();
}

void MainViewer::CellMakingViewer::BodySculptor::ColorSelector::update()
{
	m_timer = Math::Lerp(m_timer, 1.0, 0.1);

	moveDrawPos(60, 0);
	for (auto it = m_colors.begin(); it != m_colors.end(); ++it)
	{
		if (m_selectedColor == *it && !m_eraseMode) moveDrawPos(m_timer * -60, 0);

		Polygon polygon({ { 0, 15 }, { 45, 0 }, { 200,0 },{ 200, 30 }, { 45, 30 } });
		Triangle(0, 15, 45, 0, 45, 30).draw(Palette::White);
		Triangle(0, 15, 15, 10, 15, 20).draw(*it);
		Rect(45, 0, 155, 30).draw(*it);
		Rect(45, 0, 155, 10).draw(ColorF(1.0, 0.5));
		Rect(45, 20, 155, 10).draw(ColorF(0.0, 0.5));

		if (m_selectedColor == *it && !m_eraseMode) moveDrawPos(m_timer * 60, 0);
		else if (polygon.leftClicked() && isMouseover())
		{
			m_timer = 0.0;
			m_selectedColor = *it;
			m_eraseMode = false;
		}

		moveDrawPos(0, 30);
	}

	moveDrawPos(0, 10);

	// 消しゴム
	{
		if (m_eraseMode) moveDrawPos(m_timer * -60, 0);

		Circle(30, 30, 30).draw(Palette::White);
		Rect(30, 0, 15, 60).draw(Palette::White);
		Rect(45, 0, 155, 60).draw(Palette::Blue);
		Rect(45, 0, 155, 20).draw(ColorF(1.0, 0.25));
		Rect(45, 40, 155, 20).draw(ColorF(0.0, 0.25));

		if (m_eraseMode) moveDrawPos(m_timer * 60, 0);
		else if (Rect(200, 60).leftClicked() && isMouseover())
		{
			m_timer = 0.0;
			m_eraseMode = true;
		}
	}
}

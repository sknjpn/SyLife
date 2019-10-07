#include "AssemblyViewer.h"

#include "ViewerManager.h"

#include "PartPaletteViewer.h"
#include "Cell.h"
#include "PartModel.h"

AssemblyViewer::AssemblyViewer()
{
	SetDrawRect(Scene::Width() - 400, 20, 300, 300);
	m_camera.setScreen(Rect(300, 300));
	m_camera.setRestrictedRect(RectF(m_width, m_height).setCenter(Vec2::Zero()));
	m_camera.setCenter(Vec2::Zero());
	m_camera.setTargetCenter(Vec2::Zero());
	m_camera.setMaxScale(4.0);
}

void AssemblyViewer::Init()
{
}

void AssemblyViewer::update()
{
	//Rect(GetDrawRect().size.asPoint()).draw(Color(11, 22, 33, 192));

	m_camera.update();

	const auto t1 = m_camera.createTransformer();
	const int scale = (int)log10(m_camera.getScale());
	const double thickness = 2.0 / m_camera.getScale();
	const double interval = pow(10.0, -scale + 1);
	const auto cursor = (Cursor::Pos() / interval).asPoint() * interval;
	const auto boxSize = Vec2::One() * 5.0 / m_camera.getScale();
	const auto pointer = RectF(Arg::center(cursor), boxSize);

	// 縦線
	{
		const auto color = ColorF(Palette::White, 0.25);

		for (double x = -m_width / 2.0; x <= m_width / 2.0; x += interval)
			Line(x, -m_height / 2.0, x, m_height / 2.0).draw(thickness, color);

		for (double y = -m_height / 2.0; y <= m_height / 2.0; y += interval)
			Line(-m_width / 2.0, y, m_width / 2.0, y).draw(thickness, color);
	}

	// XY軸
	{
		Line(-m_width / 2.0, 0, m_width / 2.0, 0).draw(thickness, Palette::Red);
		Line(0, -m_height / 2.0, 0, m_height / 2.0).draw(thickness, Palette::Red);
	}

	// part
	{
		for (const auto& p : getModel<CellModel>()->GetPartConfigs())
		{
			auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
				.translated(p->getPosition().x, p->getPosition().y));

			for (const auto& s : p->getModel()->getShapes())
				s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
		}
	}

	// selectedPart
	if (g_viewerManagerPtr->GetViewer<PartPaletteViewer>()->m_selectedPart != nullptr)
	{
		if (IsMouseOver())
		{
			{
				auto t = Transformer2D(Mat3x2::Translate(Cursor::PosF()));

				for (const auto& s : g_viewerManagerPtr->GetViewer<PartPaletteViewer>()->m_selectedPart->getShapes())
				{
					s.draw(0.5);
					s.GetPolygon().drawFrame(2.0, Palette::White);
				}
			}

			if (MouseL.up())
			{
				const auto& partConfig = getModel<CellModel>()->AddPartConfig();

				partConfig->setModel(g_viewerManagerPtr->GetViewer<PartPaletteViewer>()->m_selectedPart);
				partConfig->setPosition(Vec2(Cursor::PosF().x, Cursor::PosF().y));
				partConfig->setRotation(0.0);
			}
		}

		if (!MouseL.pressed()) g_viewerManagerPtr->GetViewer<PartPaletteViewer>()->m_selectedPart = nullptr;
	}

	getModel<CellModel>()->UpdateProperties();
}

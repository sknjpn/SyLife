#pragma once

#include "Viewer.h"
#include "Cell.h"
#include "Part.h"
#include "AssemblyViewer.h"
#include "ModelViewer.h"
#include "TinyCamera.h"
#include "PartPaletteViewer.h"

#include "Body.h"
#include "Equipment.h"
#include "Module.h"

class AssemblyViewer
	: public ModelViewer
{
public:
	TinyCamera	m_camera;

	double	m_mass;
	double	m_inertia;
	double	m_radius;
	double	m_width = 300;
	double	m_height = 300;

public:
	AssemblyViewer()
	{
		SetDrawRect(Scene::Width() - 400, 20, 300, 300);
		m_camera.setScreen(Rect(300, 300));
		m_camera.setRestrictedRect(RectF(m_width, m_height).setCenter(Vec2::Zero()));
		m_camera.setCenter(Vec2::Zero());
		m_camera.setTargetCenter(Vec2::Zero());
		m_camera.setMaxScale(4.0);
	}

	void	Init() override
	{

	}

	void	Update() override
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

		// ècê¸
		{
			const auto color = ColorF(Palette::White, 0.25);

			for (double x = -m_width / 2.0; x <= m_width / 2.0; x += interval)
				Line(x, -m_height / 2.0, x, m_height / 2.0).draw(thickness, color);

			for (double y = -m_height / 2.0; y <= m_height / 2.0; y += interval)
				Line(-m_width / 2.0, y, m_width / 2.0, y).draw(thickness, color);
		}

		// XYé≤
		{
			Line(-m_width / 2.0, 0, m_width / 2.0, 0).draw(thickness, Palette::Red);
			Line(0, -m_height / 2.0, 0, m_height / 2.0).draw(thickness, Palette::Red);
		}

		// part
		{
			for (const auto& p : GetModel<CellModel>()->GetPartConfigs())
			{
				auto t2 = Transformer2D(Mat3x2::Rotate(p->GetRotation())
					.translated(p->GetPosition().x, p->GetPosition().y));

				for (const auto& s : p->GetModel()->GetShapes())
					s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
			}
		}

		// selectedPart
		if (g_viewerManagerPtr->GetViewer<PartPaletteViewer>()->m_selectedPart != nullptr)
		{
			if (IsMouseOver())
			{
				for (const auto& s : g_viewerManagerPtr->GetViewer<PartPaletteViewer>()->m_selectedPart->GetShapes())
					s.m_polygon.drawTransformed(0.0, 1.0, Cursor::Pos(), ColorF(s.m_color, 0.5));

				if (MouseL.up())
				{
					const auto& partConfig = GetModel<CellModel>()->AddPartConfig();

					partConfig->SetModel(g_viewerManagerPtr->GetViewer<PartPaletteViewer>()->m_selectedPart);
					partConfig->SetPosition(Vec2(Cursor::PosF().x, Cursor::PosF().y));
					partConfig->SetRotation(0.0);
				}
			}

			if (!MouseL.pressed()) g_viewerManagerPtr->GetViewer<PartPaletteViewer>()->m_selectedPart = nullptr;
		}

		GetModel<CellModel>()->UpdateProperties();
	}
};


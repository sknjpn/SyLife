#pragma once

#include "Viewer.h"
#include "Cell.h"
#include "TinyCamera.h"
#include "Part.h"
#include "AssemblyViewer.h"
#include "PartPaletteViewer.h"

#include "Body.h"
#include "Equipment.h"
#include "Module.h"
class AssemblyViewer
	: public Viewer
{
public:
	shared_ptr<CellModel>	m_model;

	CursorCamera2D	m_camera;

	double	m_mass;
	double	m_inertia;
	double	m_radius;

public:
	AssemblyViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{
		m_drawRect = RectF(Window::Width() - 400, 20, 300, 300);

		m_camera.setTargetCenter(Window::BaseSize() / 2 - m_drawRect.size / 2);
		m_camera.setCenter(Window::BaseSize() / 2 - m_drawRect.size / 2);
	}

	void	Init() override
	{
		m_camera.setCenter(m_drawRect.center());
	}

	void	Update(bool isMouseOver) override
	{
		Rect(m_drawRect.size.asPoint()).draw(Color(11, 22, 33, 192));

		if (isMouseOver) m_camera.update();

		const auto t1 = m_camera.createTransformer();
		const int scale = (int)log10(m_camera.getMagnification());
		const double thickness = 2.0 / m_camera.getMagnification();
		const double interval = pow(10.0, -scale + 1);
		const auto cursor = (Cursor::Pos() / interval).asPoint() * interval;
		const auto boxSize = Vec2::One() * 5.0 / m_camera.getMagnification();
		const auto pointer = RectF(Arg::center(cursor), boxSize);

		// cü
		{
			const auto color = ColorF(Palette::White, 0.25);

			for (double x = 0; x >= m_camera.getCameraRect().x; x -= interval)
				Line(x, m_camera.getCameraRect().y, x, m_camera.getCameraRect().br().y).draw(thickness, color);

			for (double x = 0; x <= m_camera.getCameraRect().br().x; x += interval)
				Line(x, m_camera.getCameraRect().y, x, m_camera.getCameraRect().br().y).draw(thickness, color);

			for (double y = 0; y >= m_camera.getCameraRect().y; y -= interval)
				Line(m_camera.getCameraRect().x, y, m_camera.getCameraRect().br().x, y).draw(thickness, color);

			for (double y = 0; y <= m_camera.getCameraRect().br().y; y += interval)
				Line(m_camera.getCameraRect().x, y, m_camera.getCameraRect().br().x, y).draw(thickness, color);
		}

		// XYŽ²
		{
			Line(m_camera.getCameraRect().x, 0, m_camera.getCameraRect().br().x, 0).draw(thickness, Palette::Red);
			Line(0, m_camera.getCameraRect().y, 0, m_camera.getCameraRect().br().y).draw(thickness, Palette::Red);
		}

		// disk
		{
			Circle(m_radius).draw(ColorF(Palette::Green, 0.5)).drawFrame(2.0, Palette::Black);
		}

		// part
		{
			for (const auto& p : m_model->m_partConfigs)
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
			if (isMouseOver)
			{
				for (const auto& s : g_viewerManagerPtr->GetViewer<PartPaletteViewer>()->m_selectedPart->GetShapes())
					s.m_polygon.drawTransformed(0.0, 1.0, Cursor::Pos(), ColorF(s.m_color, 0.5));

				if (MouseL.up())
				{
					const auto& partConfig = m_model->m_partConfigs.emplace_back(make_shared<PartConfig>());

					partConfig->SetModel(g_viewerManagerPtr->GetViewer<PartPaletteViewer>()->m_selectedPart);
					partConfig->SetPosition(Vec2(Cursor::PosF().x, Cursor::PosF().y));
					partConfig->SetRotation(0.0);
				}
			}

			if (!MouseL.pressed()) g_viewerManagerPtr->GetViewer<PartPaletteViewer>()->m_selectedPart = nullptr;
		}

		m_model->UpdateProperties();
	}
};


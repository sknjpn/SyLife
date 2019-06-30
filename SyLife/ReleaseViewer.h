#pragma once

#include "Viewer.h"
#include "Part.h"
#include "Cell.h"

class ReleaseViewer
	: public Viewer
{
public:
	shared_ptr<CellModel>	m_model;
	bool	m_isDragged;

public:
	ReleaseViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(s3d::Window::Width() - 400, 320, 300, 100);
	}

	void	Update(bool	isMouseOver) override
	{
		s3d::Circle circle(50, 50, 45);

		circle
			.draw(circle.mouseOver() ? s3d::Palette::Orange : s3d::Palette::Skyblue)
			.drawFrame(4.0, s3d::Palette::Black);

		if (m_isDragged) circle.draw(s3d::Palette::Red);

		if (circle.leftClicked()) m_isDragged = true;
		if (!s3d::MouseL.pressed()) m_isDragged = false;

		// part
		{
			auto t1 = s3d::Transformer2D(s3d::Mat3x2::Scale(45.0 / m_model->m_radius / 2.0).translated(circle.center));

			for (const auto& p : m_model->m_partConfigs)
			{
				auto t2 = s3d::Transformer2D(s3d::Mat3x2::Rotate(p->GetRotation())
					.translated(p->GetPosition().m_x, p->GetPosition().m_y));

				for (const auto& s : p->GetModel()->GetShapes())
					s.m_polygon.draw(s3d::ColorF(s.m_color, 0.5)).drawFrame(1.0, s3d::Palette::Black);
			}
		}
	}
};


#pragma once

#include "Viewer.h"
#include "Part.h"
#include "Cell.h"
#include "ModelViewer.h"

class ReleaseViewer
	: public ModelViewer
{
public:
	bool	m_isDragged;

public:
	ReleaseViewer()
	{
		SetDrawRect(Window::Width() - 400, 320, 300, 100);
	}

	void	Update() override
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
			auto t1 = Transformer2D(Mat3x2::Scale(45.0 / GetModel<CellModel>()->m_radius / 2.0).translated(circle.center));

			for (const auto& p : GetModel<CellModel>()->m_partConfigs)
			{
				auto t2 = Transformer2D(Mat3x2::Rotate(p->GetRotation())
					.translated(p->GetPosition().x, p->GetPosition().y));

				for (const auto& s : p->GetModel()->GetShapes())
					s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
			}
		}
	}
};


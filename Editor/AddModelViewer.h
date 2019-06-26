#pragma once

#include "Viewer.h"

class AddModelViewer
	: public Viewer
{
	s3d::Vec2	m_drawPos;
public:
	AddModelViewer()
	{
		m_drawRect = s3d::RectF(600, 0, 200, 600);
	}

	template <typename T>
	void	DrawModels(const string& name)
	{
		static s3d::Font font10(10, s3d::Typeface::Bold);
		auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(m_drawPos), true);
		auto f = font10(s3d::Unicode::Widen(name));

		f.region().draw(s3d::ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
		f.draw();

		//if (f.region().leftClicked()) m_selectedPart = *it;

		m_drawPos.moveBy(0.0, 15.0);
	}

	void	Update() override;
};


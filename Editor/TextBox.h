#pragma once

class TextBox
{
public:
	const s3d::Font		m_font;
	const s3d::Point	m_pos;
	s3d::String			m_text;
	s3d::RectF			m_rect;

	size_t	m_cursorIndex = 0;

	s3d::Stopwatch m_leftPress, m_rightPress, m_upPress, m_downPress;

public:
	TextBox(const s3d::RectF& rect)
		: m_font(16)
		, m_pos(10, 10)
		, m_rect(rect)
	{}

	void	Update();
};


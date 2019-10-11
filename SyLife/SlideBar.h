#pragma once

class SlideBar
{
	double	m_value;
	double	m_size;
	double	m_length;
	bool	m_isDragged;

	double	getBarPositionY() const { return (m_size - m_length) * m_value; }

public:
	SlideBar(double size, double length)
		: m_size(size)
		, m_length(length)
		, m_value(0.0)
		, m_isDragged(false)
	{}

	void	update();
	double	getValue() const { return m_value; }
};


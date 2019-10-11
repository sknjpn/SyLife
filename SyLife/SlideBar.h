#pragma once

class SlideBar
{
	double	m_value;
	double	m_size;

public:

	void	update();
	double	getValue() const { return m_value; }
};


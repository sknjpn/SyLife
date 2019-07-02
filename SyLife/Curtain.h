#pragma once

class Curtain
{
public:
	Stopwatch	m_st;
	Color		m_color;
	double			m_duration;

public:
	Curtain(Color color, double duration)
		: m_color(color)
		, m_duration(duration)
		, m_st(true)
	{}

	void	OpenUpdate()
	{
		const auto a = Max(1.0 - m_st.sF() / m_duration, 0.0);

		Window::ClientRect().draw(ColorF(m_color, a));
	}
	void	CloseUpdate()
	{
		const auto a = Min(m_st.sF() / m_duration, 1.0);

		Window::ClientRect().draw(ColorF(m_color, a));
	}
};


#pragma once

class Curtain
{
	s3d::Stopwatch	m_st;
	s3d::Color		m_color;
	double			m_duration;

public:
	Curtain(s3d::Color color, double duration)
		: m_color(color)
		, m_duration(duration)
	{}

	void	Update()
	{
		const auto a = s3d::Max(m_st.sF() / m_duration, 0.0);

		s3d::Window::ClientRect().draw(s3d::ColorF(m_color, a));
	}

};


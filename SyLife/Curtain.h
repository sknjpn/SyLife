#pragma once

class Curtain
{
	Stopwatch	m_st;
	Color		m_beginColor;
	Color		m_endColor;
	double		m_duration;

public:
	Curtain(Color beginColor, Color endColor, double duration, bool start = false)
		: m_beginColor(beginColor)
		, m_endColor(endColor)
		, m_duration(duration)
		, m_st(start)
	{}

	// Get
	bool	isRunning() const { return m_st.isRunning(); }
	double	getSecond() const { return m_st.sF(); }
	double	getDuration() const { return m_duration; }
	double	getProgress() const { return Min(m_st.sF() / m_duration, 1.0); }

	void	start() { m_st.start(); }

	// true : is end
	bool	update()
	{
		const ColorF color = Math::Lerp(m_beginColor, m_endColor, Min(m_st.sF() / m_duration, 1.0));

		Scene::Rect().draw(color);

		if (m_st.sF() > m_duration)
		{
			m_st.reset();

			return true;
		}

		return false;
	}
};


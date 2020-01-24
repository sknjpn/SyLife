#pragma once

#include "EasyViewer.h"

class CurtainViewer
	: public EasyViewer
{
	Stopwatch	m_st;
	Color		m_beginColor;
	Color		m_endColor;
	double		m_duration;
	function<void(void)>	m_functionOnEnd;

	Color	getColor() const { return  m_beginColor.lerp(m_endColor, Min(m_st.sF() / m_duration, 1.0)); }

public:
	CurtainViewer(Color beginColor, Color endColor, double duration, function<void(void)> functionOnEnd)
		: m_beginColor(beginColor)
		, m_endColor(endColor)
		, m_duration(duration)
		, m_st(true)
		, m_functionOnEnd(functionOnEnd)
	{}
	CurtainViewer(Color beginColor, Color endColor, double duration)
		: m_beginColor(beginColor)
		, m_endColor(endColor)
		, m_duration(duration)
		, m_st(true)
	{}

	void	update() override
	{
		RectF(getViewerSize()).draw(getColor());

		if (m_st.sF() > m_duration)
		{
			if (m_functionOnEnd) m_functionOnEnd();

			destroy();
		}
	}
};

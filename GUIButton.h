#pragma once

#include "EasyViewer.h"

class GUIButton :
	public EasyViewer
{
	bool	m_isGrabbed = false;
	bool	m_isSelected = false;
	bool	m_isEnabled = true;
	Color	m_color;
	std::function<void(void)>	m_functionOnSelected;

	Color	getTargetColor() const
	{
		return !m_isEnabled ? Color(64) : m_isGrabbed ? Color(isMouseover() ? 128 : 192) : isMouseover() ? Palette::Skyblue : Palette::White;
	}

public:
	GUIButton(bool isEnabled = true)
		: m_isEnabled(isEnabled)
	{}
	GUIButton(std::function<void(void)> functionOnSelected, bool isEnabled = true)
		: m_functionOnSelected(functionOnSelected)
		, m_isEnabled(isEnabled)
	{}

	void	init() override { m_color = getTargetColor(); }
	void	update() override
	{
		static const Font font(128, Typeface::Bold);

		const RectF rect(getViewerSize());
		const double d = rect.h / 3.0;
		const double frameThickness = rect.h / 12;

		if (m_isEnabled)
		{
			m_isSelected = m_isGrabbed && isMouseover() && MouseL.up();
			if (m_isSelected && m_functionOnSelected) m_functionOnSelected();

			if (isMouseover() && MouseL.down()) m_isGrabbed = true;
			if (MouseL.up()) m_isGrabbed = false;
		}

		// ゆっくりの色変化
		m_color = m_color.lerp(getTargetColor(), 0.25);

		// 背景
		rect.rounded(d).draw(m_color).drawFrame(frameThickness, 0.0, Palette::Black);
	}
	
	void	setIsEnabled(bool isEnabled)
	{
		m_isEnabled = isEnabled;

		if (!m_isEnabled)
		{
			m_isGrabbed = false;
			m_isSelected = false;
		}
	}

	bool	isGrabbed() const { return m_isGrabbed; }
	bool	isEnabled() const { return m_isEnabled; }
};


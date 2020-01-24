#pragma once

#include "EasyViewer.h"

class GUIButtonIcon :
	public EasyViewer
{
	bool	m_isGrabbed = false;
	bool	m_isSelected = false;
	bool	m_isEnabled = true;
	Color	m_color;
	uint32	m_code;
	Texture	m_texture;
	function<void(void)>	m_functionOnSelected;

	Color	getTargetColor() const
	{
		return !m_isEnabled ? Color(64) : m_isGrabbed ? Color(isMouseover() ? 128 : 192) : isMouseover() ? Palette::Skyblue : Palette::White;
	}

public:
	GUIButtonIcon(uint32 code, bool isEnabled = true)
		: m_code(code)
		, m_isEnabled(isEnabled)
	{}
	GUIButtonIcon(uint32 code, function<void(void)> functionOnSelected, bool isEnabled = true)
		: m_code(code)
		, m_functionOnSelected(functionOnSelected)
		, m_isEnabled(isEnabled)
	{}

	void	setIcon(uint32 code) { m_code = code; m_texture = Texture(Icon(m_code, int(getViewerSize().x * 0.75))); }

	void	init() override { m_color = getTargetColor(); setIcon(m_code); }
	void	update() override
	{
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
		rect.rounded(5).draw(m_color).drawFrame(1.0, 0.0, Palette::Black);

		// Icon描画
		m_texture.drawAt(getViewerSize() / 2.0, Palette::Black);
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


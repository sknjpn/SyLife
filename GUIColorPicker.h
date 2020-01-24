#pragma once

#include "EasyViewer.h"

class GUIColorPicker :
	public EasyViewer
{
	HSV		m_color;
	bool	m_isGrabbed = false;
	function<void(const Color&)>	m_functionOnChanged;

public:
	GUIColorPicker(const Color& color = Palette::White)
		: m_color(color)
	{}
	GUIColorPicker(function<void(const Color&)> functionOnChanged, const Color& color = Palette::White)
		: m_functionOnChanged(functionOnChanged)
		, m_color(color)
	{}

	void	update() override;

	Color	getColor() const { return m_color; }
};

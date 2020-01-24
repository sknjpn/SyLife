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

	void	update() override
	{
		// size 160, 116

		RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

		if (MouseL.up()) m_isGrabbed = false;
		if (SimpleGUI::ColorPickerRegionAt(getViewerSize() / 2.0).leftClicked()) m_isGrabbed = true;

		if (SimpleGUI::ColorPickerAt(m_color, getViewerSize() / 2.0, m_isGrabbed))
			m_functionOnChanged(m_color);
	}

	Color	getColor() const { return m_color; }
};

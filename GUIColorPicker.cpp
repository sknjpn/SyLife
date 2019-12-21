#include "stdafx.h"
#include "GUIColorPicker.h"

void GUIColorPicker::update()
{
	// size 160, 116

	RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

	if (MouseL.up()) m_isGrabbed = false;
	if (SimpleGUI::ColorPickerRegionAt(getViewerSize() / 2.0).leftClicked()) m_isGrabbed = true;

	if (SimpleGUI::ColorPickerAt(m_color, getViewerSize() / 2.0, m_isGrabbed))
		m_functionOnChanged(m_color);
}

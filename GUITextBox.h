#pragma once

#include "EasyViewer.h"

class GUITextBox :
	public EasyViewer
{
	TextEditState	m_textEditState;
	function<void(const String&)>	m_functionOnChanged;

public:
	GUITextBox(const String& text = U"")
		: m_textEditState(text)
	{}
	GUITextBox(function<void(const String&)> functionOnChanged, const String& text = U"")
		: m_functionOnChanged(functionOnChanged)
		, m_textEditState(text)
	{}

	void	update() override
	{
		RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

		if (SimpleGUI::TextBoxAt(m_textEditState, getViewerSize() / 2.0, getViewerSize().x - 10.0))
			m_functionOnChanged(m_textEditState.text);
	}
};


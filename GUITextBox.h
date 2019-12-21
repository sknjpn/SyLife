#pragma once

#include "Viewer.h"

class GUITextBox :
	public Viewer
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

	void	update() override;
};


#pragma once

#include "Viewer.h"

class GUIText :
	public Viewer
{
	String	m_text;

public:
	GUIText(const String& text)
		: m_text(text)
	{}

	void	update() override;
};

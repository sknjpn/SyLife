#pragma once

#include "EasyViewer.h"
#include "GUIText.h"

class GUITextBox :
	public EasyViewer
{
	size_t	m_cursorPos;
	bool	m_isSelected;
	std::function<void(const String&)>	m_functionOnChanged;

	const String& getText() const { return getChildViewer<GUIText>()->m_text; }
	String& getText() { return getChildViewer<GUIText>()->m_text; }

public:
	GUITextBox() = default;
	GUITextBox(std::function<void(const String&)> functionOnChanged)
		: m_functionOnChanged(functionOnChanged)
	{}

	const String& getValue() const { return getChildViewer<GUIText>()->m_text; }

	void	setFont(const Font& font) { getChildViewer<GUIText>()->m_font = font; }

	void	init() override
	{
		addChildViewer<GUIText>(U"", Font(16), GUIText::Mode::DrawLeftCenter);
	}

	void	update() override
	{
		RectF(getViewerSize()).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

		// 枠が押されたら有効化
		if (MouseL.down()) m_isSelected = isMouseover();

		// 入力
		if (m_isSelected)
		{
			auto newCursorPos = TextInput::UpdateText(getText(), m_cursorPos, TextInputMode::AllowBackSpace);

			if (newCursorPos != m_cursorPos)
			{
				m_functionOnChanged(getText());

				m_cursorPos = newCursorPos;
			}
		}

		// 位置調整
		getChildViewer<GUIText>()->setViewerRectInLocal(RectF(getViewerSize()).stretched(-2));
	}
};


#include "GUITextBox.h"

void GUITextBox::update()
{
	RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

	if (SimpleGUI::TextBoxAt(m_textEditState, getViewerSize() / 2.0, getViewerSize().x - 10.0))
		m_functionOnChanged(m_textEditState.text);
}

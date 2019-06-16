#include "TextBox.h"

void TextBox::Update()
{
	m_cursorIndex = s3d::TextInput::UpdateText(m_text, m_cursorIndex);
	const s3d::String markedText = s3d::TextInput::GetEditingText();
	const std::u32string m_textUTF32 = m_text.toUTF32();
	const s3d::Array<int> xAdvances = m_font(m_text).getXAdvances();
	int requestUpDown = 0;

	if (!markedText)
	{
		if (0 < m_cursorIndex && (s3d::KeyLeft.down() || (s3d::KeyLeft.pressedDuration() > 0.33s && m_leftPress > 0.06s)))
		{
			--m_cursorIndex;
			m_leftPress.restart();
		}

		if (m_cursorIndex < m_textUTF32.size() && (s3d::KeyRight.down() || (s3d::KeyRight.pressedDuration() > 0.33s && m_rightPress > 0.06s)))
		{
			++m_cursorIndex;
			m_rightPress.restart();
		}

		if (s3d::KeyUp.down() || (s3d::KeyUp.pressedDuration() > 0.33s && m_upPress > 0.06s))
		{
			requestUpDown = -1;
			m_upPress.restart();
		}

		if (s3d::KeyDown.down() || (s3d::KeyDown.pressedDuration() > 0.33s && m_downPress > 0.06s))
		{
			requestUpDown = 1;
			m_downPress.restart();
		}
	}

	s3d::Point cursorPos(0, 0);
	int xOffset = 0;

	if (0 < m_cursorIndex)
	{
		s3d::Point charPos(0, 0);
		size_t index = 0;

		for (const auto ch : m_textUTF32)
		{
			if (ch == U'\n')
			{
				charPos.set(0, charPos.y + 1);
				xOffset = 0;
			}
			else
			{
				++charPos.x;
				xOffset += xAdvances[index];
			}

			if (++index == m_cursorIndex)
			{
				cursorPos = charPos;
				break;
			}
		}
	}

	if (requestUpDown)
	{
		s3d::Point charPos(0, 0);
		size_t index = 0;
		bool onTargetLine = false;

		for (const auto ch : m_textUTF32)
		{
			if (!onTargetLine && charPos.y == cursorPos.y + requestUpDown)
			{
				if (charPos.x == cursorPos.x)
				{
					m_cursorIndex = index;
					break;
				}

				onTargetLine = true;
			}

			if (ch == U'\n')
			{
				charPos.set(0, charPos.y + 1);

				if (onTargetLine)
				{
					m_cursorIndex = index;
					onTargetLine = false;
					break;
				}
			}
			else
			{
				++charPos.x;
			}

			++index;

			if (onTargetLine && charPos.x == cursorPos.x)
			{
				m_cursorIndex = index;
				onTargetLine = false;
				break;
			}
		}

		if (onTargetLine)
		{
			m_cursorIndex = index;
		}
	}

	const int yOffset = cursorPos.y * m_font.height();
	m_rect.draw(s3d::Palette::White).drawFrame(2.0, s3d::Palette::Black);
	m_font(m_text).draw(m_pos, s3d::Palette::Black);
	m_font(markedText).draw(s3d::Point(xOffset, yOffset).movedBy(m_pos), s3d::Palette::Gray)
		.bottom().movedBy(1, -2.0).draw(2, s3d::Palette::Gray);

	for (const int xAdvance : m_font(markedText).getXAdvances())
	{
		xOffset += xAdvance;
	}

	s3d::Rect(xOffset - 1, yOffset, 1, m_font.height()).movedBy(m_pos).draw(s3d::Palette::Black);

}
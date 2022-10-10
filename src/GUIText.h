#pragma once

#include "EasyViewer.h"

class GUIText : public EasyViewer {
public:
  String m_text;
  Font   m_font;
  Color  m_color;

  enum class Mode {
    DrawAtCenter,
    DrawInBox,
    DrawLeftCenter,
  } m_mode;

public:
  GUIText(const String& text, const Font& font, Mode mode = Mode::DrawAtCenter, Color color = Palette::Black)
      : m_text(text)
      , m_font(font)
      , m_mode(mode)
      , m_color(color) { }

  void init() override {
    setIsPenetrated(true);
  }

  void update() override {
    const RectF rect(getViewerSize());

    // 文字列描画
    switch (m_mode) {
    case GUIText::Mode::DrawAtCenter: {
      m_font(m_text).drawAt(rect.center(), m_color);
    } break;
    case GUIText::Mode::DrawInBox: {
      m_font(m_text).draw(rect, m_color);
    } break;
    case GUIText::Mode::DrawLeftCenter: {
      m_font(m_text).draw(Arg::leftCenter(0.0, getViewerSize().y / 2.0), m_color);
    } break;
    default:
      break;
    }
  }
};

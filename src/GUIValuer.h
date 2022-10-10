#pragma once

#include "EasyViewer.h"

class GUIValuer
    : public EasyViewer {
  bool   m_isGrabbed = false;
  double m_grabbedPos = 0.0;
  double m_value = 0.0;
  Color  m_color = Color(255);

  Circle getButton() const {
    const double w = getViewerSize().x;
    const double h = getViewerSize().y;

    return Circle(h / 2 + (w - h) * m_value, h / 2, h / 2);
  }

  Color getTargetColor() const { return m_isGrabbed ? Color(192) : (isMouseover() ? Palette::Skyblue : Palette::White); }

public:
  GUIValuer(double value = 0.0)
      : m_value(value) { }

  void init() override {
    m_color = getTargetColor();
  }

  void update() override {
    const double w = getViewerSize().x;
    const double h = getViewerSize().y;

    const double d = h / 2.0;
    const double frameThickness = h / 12;

    if (isMouseover() && MouseL.down()) m_isGrabbed = true;
    if (MouseL.up()) m_isGrabbed = false;

    if (m_isGrabbed) m_value = Clamp<double>((Cursor::PosF().x - h / 2) / (w - h), 0.0, 1.0);

    // ゆっくりの色変化
    m_color = m_color.lerp(getTargetColor(), 0.25);

    // 背景
    RectF(getViewerSize()).rounded(d).draw(Color(64)).drawFrame(frameThickness, 0.0, Palette::Black);
    getButton().draw(m_color).drawFrame(frameThickness, 0.0, Palette::Black);
  }

  bool isGrabbed() const { return m_isGrabbed; }

  void   setValue(double value) { m_value = value; }
  double getValue() const { return m_value; }
};

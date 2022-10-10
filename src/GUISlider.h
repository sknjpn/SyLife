#pragma once

#include "EasyViewer.h"

class GUISlider
  : public EasyViewer
{
  bool   m_isGrabbed = false;
  double m_grabbedPos;
  double m_height;
  double m_value;
  Color  m_color;

  Color  getTargetColor() const
  {
    return m_isGrabbed ? Color(192) : getButton().mouseOver() ? Palette::Skyblue :
      Palette::White;
  }
  RectF  getButton() const { return RectF(getViewerSize().x, getButtonHeight()).setPos(0, m_value * (getViewerSize().y - getButtonHeight())); }
  double getButtonHeight() const { return Min(getViewerSize().y * (getViewerSize().y / m_height), getViewerSize().y); }

public:
  GUISlider(double height)
    : m_height(height)
    , m_value(0.0)
  {
  }

  void init() override { m_color = getTargetColor(); }
  void update() override
  {
    const double w = getViewerSize().x;
    const double vh = getViewerSize().y;
    const double bh = Min(vh * (vh / m_height), vh);

    const double d = w / 3.0;
    const double frameThickness = w / 12;

    if (isMouseover() && MouseL.down())
    {
      m_isGrabbed = true;

      if (getButton().mouseOver())
        m_grabbedPos = m_value;
      else
        m_grabbedPos = (Cursor::PosF().y - bh / 2.0) / (vh - bh);
    }

    if (MouseL.up()) m_isGrabbed = false;

    if (m_isGrabbed && vh < m_height)
    {
      m_grabbedPos += Cursor::DeltaF().y / (getViewerSize().y - Min(vh * (vh / m_height), vh));
      m_value = Clamp<double>(m_grabbedPos, 0.0, 1.0);
    }

    // ゆっくりの色変化
    m_color = m_color.lerp(getTargetColor(), 0.25);

    // 背景
    RectF(getViewerSize()).rounded(d).draw(Color(64)).drawFrame(frameThickness, 0.0, Palette::Black);
    getButton().rounded(d).draw(m_color).drawFrame(frameThickness, 0.0, Palette::Black);
  }

  void setHeight(double height) { m_height = height; }

  bool isGrabbed() const { return m_isGrabbed; }

  void   setValue(double value) { m_value = value; }
  double getValue() const { return m_value; }

  double getDelta() const { return m_value * (m_height - getViewerSize().y); }
};

﻿#pragma once

#include "EasyViewer.h"

class DraggableViewer : public EasyViewer {
  bool m_isGrabbed = false;
  Vec2 m_grabbedPos;
  Vec2 m_deltaPos;

public:
  void update() override {
    if (isMouseover() && MouseL.down()) {
      m_isGrabbed = true;
      m_grabbedPos = Cursor::PosF();
      m_deltaPos = Vec2::Zero();

      moveToFront();
    }

    if (MouseL.up()) m_isGrabbed = false;

    if (m_isGrabbed) {
      setViewerPosInLocal(getViewerPosInLocal() + Cursor::DeltaF());

      if (getViewerPosInLocal().x < 0) setViewerPosInLocal(0, getViewerPosInLocal().y);
      if (getViewerPosInLocal().y < 0) setViewerPosInLocal(getViewerPosInLocal().x, 0);

      auto d = getViewerPosInLocal() + getViewerSize() - getParentViewer()->getViewerSize();
      if (d.x > 0) setViewerPosInLocal(getViewerPosInLocal().x - d.x, getViewerPosInLocal().y);
      if (d.y > 0) setViewerPosInLocal(getViewerPosInLocal().x, getViewerPosInLocal().y - d.y);
    }
  }
};

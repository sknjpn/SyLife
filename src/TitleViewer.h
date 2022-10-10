#pragma once

#include "EasyViewer.h"

class TitleViewer : public EasyViewer
{
  class WorldGenerator : public EasyViewer
  {
    Texture m_fieldTexture;
    uint32  m_noiseSeedX;
    uint32  m_noiseSeedY;

    void generate();

  public:
    void init() override;
    void update() override;

    void onStart();
    void onContinue();
  };

  class Bubble
  {
  public:
    Vec3   m_position;
    double m_timer = 0.0;
  };

  Array<Bubble> m_bubbles;

public:
  void updateBubbles();
  void drawBubbles();

  void runNew();
  void runContinue();

  void init() override;
  void update() override;
};

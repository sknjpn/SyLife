#pragma once

#include "EasyViewer.h"
#include "GUIButton.h"
#include "GUIIcon.h"
#include "GUIValuer.h"
#include "HiddenViewer.h"

class GUIMusicBox : public HiddenViewer {
  bool   m_loadComplate = false;
  String m_assetName;
  double m_volume;
  bool   m_isEnabled = true;
  bool   m_isLoop = true;

  void onClicked() {
    if (m_isEnabled) {
      getChildViewer<GUIButton>()->getChildViewer<GUIIcon>()->setIcon(0xf6a9);
      getChildViewer<GUIValuer>()->setValue(0.0);
      AudioAsset(m_assetName).setVolume(0.0);
    } else {
      getChildViewer<GUIButton>()->getChildViewer<GUIIcon>()->setIcon(0xf028);
      getChildViewer<GUIValuer>()->setValue(m_volume);
      AudioAsset(m_assetName).setVolume(m_volume);
    }

    m_isEnabled = !m_isEnabled;
  }

public:
  GUIMusicBox(const String& assetName, bool isLoop = true)
      : m_assetName(assetName)
      , m_isLoop(isLoop) {
    m_volume = GeneralSetting::GetInstance().m_musicVolume;
  }

  void init() override {
    setFirstPosInLocal(20, 20);
    setSecondPosInLocal(-60, 20);
    setViewerSize(60, 75);

    addChildViewer<GUIButton>([this]() { onClicked(); })
        ->setViewerRectInLocal(5, 5, 50, 50)
        ->addChildViewer<GUIIcon>(0xf028);

    addChildViewer<GUIValuer>(m_volume)
        ->setViewerRectInLocal(5, 60, 50, 10);
  }

  bool isPlaying() const { return AudioAsset(m_assetName).isPlaying(); }
  void setMusic(const String& assetName) {
    m_assetName = assetName;
    m_loadComplate = false;
  }

  void update() override {
    HiddenViewer::update();

    if (!m_loadComplate && AudioAsset::IsReady(m_assetName)) {
      m_loadComplate = true;
      AudioAsset(m_assetName).setVolume(m_volume);
      AudioAsset(m_assetName).setLoop(m_isLoop);
      AudioAsset(m_assetName).play();
    }

    RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(1.0, 0.0, Palette::Black);

    if (m_isEnabled) {
      AudioAsset(m_assetName).setVolume(m_volume);

      if (m_volume != getChildViewer<GUIValuer>()->getValue()) {
        m_volume = getChildViewer<GUIValuer>()->getValue();

        GeneralSetting::GetInstance().m_musicVolume = m_volume;

        if (m_volume > 0.5)
          getChildViewer<GUIButton>()->getChildViewer<GUIIcon>()->setIcon(0xf028);
        else
          getChildViewer<GUIButton>()->getChildViewer<GUIIcon>()->setIcon(0xf027);
      }
    } else {
      AudioAsset(m_assetName).setVolume(0.0);
      getChildViewer<GUIValuer>()->setValue(0.0);
    }
  }

  void onDestroy() override {
    AudioAsset(m_assetName).stop();
  }
};

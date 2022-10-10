﻿#include "CellState.h"
#include "CellAsset.h"
#include "EggState.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "PartState.h"
#include "Part_EyeAsset.h"
#include "Part_EyeState.h"
#include "ProteinAsset.h"
#include "TileState.h"
#include "World.h"

CellState::CellState(const std::shared_ptr<CellAsset>& cellAsset)
    : m_cellAsset(cellAsset)
    , m_startTimer(0.0)
    , m_deathTimer(cellAsset->getLifespanTime())
    , m_yieldTimer(0.0)
    , m_bioaccumulation(0.0) {
  setMass(m_cellAsset->getMass());
  setRadius(m_cellAsset->getRadius());
  setInertia(m_cellAsset->getInertia());

  // parts
  for (const auto& partConfig : m_cellAsset->getPartConfigs())
    m_partStates.emplace_back(
        partConfig->getPartAsset()->makePartState(partConfig));
}

void CellState::updateCell() {
  // 衝突処理
  {
    auto result = World::GetInstance()->getCellStateKDTree().knnSearch(2, getPosition());
    if (result.size() == 2) {
      auto& t = World::GetInstance()->getCellStates()[result[1]];

      if (t->getPosition() != getPosition() && (getRadius() + t->getRadius() - (t->getPosition() - getPosition()).length()) > 0) {
        auto f = -1000.0 * (t->getPosition() - getPosition()).normalized() * (getRadius() + t->getRadius() - (t->getPosition() - getPosition()).length());
        addForceInWorld(f, getPosition());
        t->addForceInWorld(-f, t->getPosition());
      }
    }
  }

  // Timer
  m_startTimer += DeltaTime;

  // parts
  for (const auto& p : m_partStates)
    p->update(*this);

  // Elementの取り込み
  takeElement();

  // 分裂処理
  if (m_yieldTimer > 0) {
    if ((m_yieldTimer += DeltaTime) >= m_cellAsset->getYieldTime()) {
      m_yieldTimer = 0.0;
      m_storage -= m_cellAsset->getMaterial();

      const auto& e = World::GetInstance()->addEggState(m_cellAsset);
      e->setPosition(getPosition());
      e->setRotation(Random(Math::TwoPi));
      e->setVelocity(Vec2(1.0, 0.0).rotated(rand() / 360.0));
    }
  } else if (m_storage >= m_cellAsset->getMaterial()) {
    m_yieldTimer += DeltaTime;
  }

  // 余剰分の吐き出し
  {
    for (const auto& protein : m_storage.getProteinList()) {
      int max = m_cellAsset->getMaxStorage().numProtein(protein.first);
      if (max < protein.second) {
        m_storage.addElement(
            protein.first->getMaterial().getElementRecursive() * (protein.second - max));
        m_storage.pullProtein(protein.first, protein.second - max);
      }
    }
    if (m_storage.getElement() > m_cellAsset->getMaxStorage().getElement()) {
      double delta = m_storage.getElement() - m_cellAsset->getMaxStorage().getElement();

      // Elementの吐き出し
      World::GetInstance()->getTile(getPosition()).addElement(delta);
      m_storage.pullElement(delta);
    }
  }

  // 毒の影響
  {
    auto get = 0.1 * World::GetInstance()->getTile(getPosition()).getPoison() * DeltaTime;
    auto k = 100.0;

    World::GetInstance()->getTile(getPosition()).pullPoison(get);

    m_bioaccumulation += get;

    // 死亡
    if (m_bioaccumulation > k) {
      // Elementの吐き出し
      World::GetInstance()
          ->getTile(getPosition())
          .addElement(m_storage.getElementRecursive() + m_cellAsset->getMaterial().getElementRecursive());

      // Poisonの掃き出し
      World::GetInstance()->getTile(getPosition()).addPoison(m_bioaccumulation);

      destroy();
    }
  }

  // 死亡処理
  if ((m_deathTimer -= DeltaTime) <= 0.0) {
    // Elementの吐き出し
    World::GetInstance()
        ->getTile(getPosition())
        .addElement(m_storage.getElementRecursive() + m_cellAsset->getMaterial().getElementRecursive());

    // Poisonの掃き出し
    World::GetInstance()->getTile(getPosition()).addPoison(m_bioaccumulation);

    destroy();
  }
}

void CellState::draw() {
  const double stage = Min(1.0, m_startTimer / m_cellAsset->getLifespanTime() * 10.0);

  {
    auto t1 = Transformer2D(getMat3x2());
    auto t2 = Transformer2D(Mat3x2::Scale(Math::Lerp(0.25, 1.0, stage)));

    m_cellAsset->getCellStateTexture()
        .scaled(1.0 / GeneralSetting::GetInstance().m_textureScale)
        .drawAt(Vec2::Zero(), ColorF(1.0, 0.5));

    // parts
    for (const auto& partState : m_partStates)
      if (partState->getPartConfig()->getPartAsset()->isDrawOnStateEnabled())
        partState->draw(*this);
  }

  // Eye
  /*if (m_cellAsset->m_isInViewer)
  {
          for (const auto& partState : m_partStates)
          {
                  if (auto eye =
  std::dynamic_pointer_cast<Part_EyeState>(partState))
                  {
                          const auto position =
  getWorldPosition(eye->getPartConfig()->getPosition());

                          Circle(position,
  eye->getPart_EyeAsset()->getMaxDistance()) .draw(ColorF(Palette::Red, 0.1))
                                  .drawFrame(2.0, Palette::Black);

                          if (auto target = eye->getTargetCellState())
                          {
                                  Line(getPosition(), target->getPosition())
                                          .drawArrow(5.0, Vec2(25.0, 25.0),
  ColorF(1.0, 0.5));
                          }
                  }
          }
  }*/
}

void CellState::takeElement() {
  const double space = m_cellAsset->getMaxStorage().getElement() - m_storage.getElement();
  const double amount = World::GetInstance()->getTile(getPosition()).getElement();
  const double value = Min(space, amount);

  World::GetInstance()->getTile(getPosition()).pullElement(value);
  m_storage.addElement(value);
}

bool CellState::isNeedNutrition() const {
  return !m_storage.contain(m_cellAsset->getMaterial());
}

void CellState::load(Deserializer<BinaryReader>& reader) {
  Rigidbody::load(reader);

  reader >> m_startTimer;
  reader >> m_deathTimer;
  reader >> m_yieldTimer;

  reader >> m_bioaccumulation;

  m_storage.load(reader);

  {
    String cellAssetName;
    reader >> cellAssetName;
    m_cellAsset = World::GetAsset<CellAsset>(cellAssetName);
  }

  // parts
  for (const auto& partConfig : m_cellAsset->getPartConfigs()) {
    const auto partState = m_partStates.emplace_back(
        partConfig->getPartAsset()->makePartState(partConfig));
    partState->load(reader);
  }

  // 設定
  setMass(m_cellAsset->getMass());
  setRadius(m_cellAsset->getRadius());
  setInertia(m_cellAsset->getInertia());
}

void CellState::save(Serializer<MemoryWriter>& writer) const {
  Rigidbody::save(writer);

  writer << m_startTimer;
  writer << m_deathTimer;
  writer << m_yieldTimer;

  writer << m_bioaccumulation;

  m_storage.save(writer);

  writer << m_cellAsset->getName();

  for (const auto& partState : m_partStates)
    partState->save(writer);
}

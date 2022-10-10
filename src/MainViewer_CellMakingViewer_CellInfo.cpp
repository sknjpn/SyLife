﻿#include "CellAsset.h"
#include "MainViewer.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "Part_NeedleAsset.h"
#include "Part_NucleusAsset.h"
#include "Part_SynthesizerAsset.h"
#include "ProteinAsset.h"

void MainViewer::CellMakingViewer::CellInfo::init()
{
  m_textEditState_name.text = getParentViewer<CellMakingViewer>()->getCellAsset()->getNameJP();
}

void MainViewer::CellMakingViewer::CellInfo::update()
{
  RectF(getViewerSize())
    .rounded(5)
    .draw(Palette::White)
    .drawFrame(2.0, 0.0, Palette::Black);

  const auto& cellAsset = getParentViewer<CellMakingViewer>()->getCellAsset();

  moveDrawPos(5, 5);

  // Name
  {
    SimpleGUI::TextBox(m_textEditState_name, Vec2::Zero(), 280);

    moveDrawPos(0, 40);
  }

  // material
  {
    static Font font(16, Typeface::Regular);

    // Proteins
    font(U"この生き物を作るのに必要なもの").draw(Vec2::Zero(), Palette::Black);
    moveDrawPos(0, 20);
    {
      moveDrawPos(8, 0);

      font(U"エレメント(緑色で広がっているもの)")
        .draw(Vec2::Zero(), Palette::Black);
      moveDrawPos(0, 20);
      {
        moveDrawPos(8, 0);
        font(ToString(int(cellAsset->getMaterial().getElement())) + U"elm")
          .draw(Vec2::Zero(), Palette::Black);
        moveDrawPos(0, 30);
        moveDrawPos(-8, 0);
      }

      font(U"プロテイン(合成器官で出来るもの)")
        .draw(Vec2::Zero(), Palette::Black);
      moveDrawPos(0, 20);
      {
        moveDrawPos(8, 0);
        for (const auto& protein : cellAsset->getMaterial().getProteinList())
        {
          bool canMakeSelf = false;
          for (const auto& partConfig : cellAsset->getPartConfigs())
            if (auto synthesizer = std::dynamic_pointer_cast<Part_SynthesizerAsset>(
              partConfig->getPartAsset()))
              if (synthesizer->getExport() == protein.first)
              {
                canMakeSelf = true;
                break;
              }

          if (canMakeSelf)
            font(protein.first->getNameJP() + U": " + ToString(protein.second) + U"個" + U"(自分で作れます)")
            .draw(Vec2::Zero(), Palette::Black);
          else
            font(protein.first->getNameJP() + U": " + ToString(protein.second) + U"個" + U"(自分で作れません)")
            .draw(Vec2::Zero(), Palette::Red);

          moveDrawPos(0, 20);
        }
        moveDrawPos(0, 10);
        moveDrawPos(-8, 0);
      }

      moveDrawPos(-8, 0);
    }

    font(U"この生き物が作れるプロテイン").draw(Vec2::Zero(), Palette::Black);
    moveDrawPos(0, 20);
    {
      moveDrawPos(8, 0);

      for (const auto& partConfig : cellAsset->getPartConfigs())
      {
        if (auto synthesizer = std::dynamic_pointer_cast<Part_SynthesizerAsset>(
          partConfig->getPartAsset()))
        {
          font(synthesizer->getExport()->getNameJP(),
              int(synthesizer->getProductTime()), U"秒ごとに")
            .draw(Vec2::Zero(), Palette::Black);
          moveDrawPos(0, 20);
        }
      }
      moveDrawPos(0, 10);

      moveDrawPos(-8, 0);
    }

    font(U"この生き物の特性").draw(Vec2::Zero(), Palette::Black);
    moveDrawPos(0, 20);
    {
      moveDrawPos(8, 0);

      font(U"孵化までの時間", int(cellAsset->getNucleusAsset()->getBornTime()),
          U"秒")
        .draw(Vec2::Zero(), Palette::Black);
      moveDrawPos(0, 20);

      font(U"産卵までの時間", int(cellAsset->getNucleusAsset()->getYieldTime()),
          U"秒")
        .draw(Vec2::Zero(), Palette::Black);
      moveDrawPos(0, 20);

      font(U"寿命:", int(cellAsset->getNucleusAsset()->getLifespanTime()),
          U"秒")
        .draw(Vec2::Zero(), Palette::Black);
      moveDrawPos(0, 20);

      font(U"硬さ:", int(cellAsset->getNucleusAsset()->getArmor()))
        .draw(Vec2::Zero(), Palette::Black);
      moveDrawPos(0, 20);

      {
        int penetrating = 0;

        for (const auto& partConfig : cellAsset->getPartConfigs())
          if (auto needle = std::dynamic_pointer_cast<Part_NeedleAsset>(
            partConfig->getPartAsset()))
            penetrating = Max(penetrating, needle->getPenetrating());

        font(U"トゲの貫通力:", penetrating).draw(Vec2::Zero(), Palette::Black);
        moveDrawPos(0, 20);
      }

      moveDrawPos(-8, 0);
    }
  }
}

﻿#include "MainViewer.h"
#include "CellAsset.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "PartAsset_Body.h"
#include "PartAsset_Nucleus.h"
#include "GUIButton.h"

void MainViewer::CellMakingViewer::PartsAssembler::Workspace::init()
{
	setBackgroundColor(Palette::Black);

	setViewerRectInLocal(200, 0, 800, 800);

	addChildViewer<TrashBox>();
}

void MainViewer::CellMakingViewer::PartsAssembler::Workspace::update()
{
	auto t = Transformer2D(Mat3x2::Scale(4).translated(400, 400), true);

	// 描画
	for (const auto& partConfig : m_cellAsset->getPartConfigs())
	{
		{
			auto t2 = Transformer2D(partConfig->getMat3x2());

			partConfig->getPartAsset()->getShape().draw(0.5);
			partConfig->getPartAsset()->getShape().getPolygon().drawFrame(1.0, Palette::Black);
		}

		// 回転の中心
		if (m_state == State::RotateMode && !dynamic_pointer_cast<PartAsset_Body>(partConfig->getPartAsset())) Circle(partConfig->getPosition(), 2.0).draw(Palette::Yellow).drawFrame(1.0, Palette::Black);
	}

	// 描画
	if (m_selectedPartConfig)
	{
		auto t2 = Transformer2D(Mat3x2::Rotate(m_selectedPartConfig->getRotation() + m_deltaRotation).translated(m_selectedPartConfig->getPosition() + m_deltaPosition));

		if (m_state == State::MoveMode && getChildViewer<TrashBox>()->isMouseover()) getChildViewer<TrashBox>()->select();
		else
		{
			bool canSetPart = m_cellAsset->getBodyAsset()->getShape().getPolygon().contains(m_selectedPartConfig->getPosition() + m_deltaPosition);

			auto color = canSetPart ? Palette::Green : Palette::Red;

			m_selectedPartConfig->getPartAsset()->getShape().draw(0.5);
			m_selectedPartConfig->getPartAsset()->getShape().getPolygon().draw(ColorF(color, 0.5));
		}
	}

	// 適用
	if (MouseL.up() && m_selectedPartConfig)
	{
		if (m_state == State::MoveMode)
		{

			if (getChildViewer<TrashBox>()->isMouseover())
			{
				m_cellAsset->removePartConfig(m_selectedPartConfig);
			}
			else
			{
				bool canSetPart = m_cellAsset->getBodyAsset()->getShape().getPolygon().contains(m_selectedPartConfig->getPosition() + m_deltaPosition);

				if (canSetPart) m_selectedPartConfig->setPosition(m_selectedPartConfig->getPosition() + m_deltaPosition);
			}
		}

		if (m_state == State::RotateMode) m_selectedPartConfig->setRotation(m_selectedPartConfig->getRotation() + m_deltaRotation);

		m_selectedPartConfig = nullptr;
	}

	// 移動
	if (m_selectedPartConfig)
	{
		if (m_state == State::MoveMode) m_deltaPosition += Cursor::DeltaF();

		if (m_state == State::RotateMode)
		{
			const auto t2 = Transformer2D(Mat3x2::Translate(m_selectedPartConfig->getPosition()), true);

			if (!Cursor::PreviousPosF().isZero() && !Cursor::PosF().isZero()) m_deltaRotation += Cursor::PreviousPosF().getAngle(Cursor::PosF());
		}
	}

	// 選択
	if (!m_selectedPartConfig && MouseL.down() && isMouseover())
	{
		for (const auto& partConfig : m_cellAsset->getPartConfigs())
		{
			if (dynamic_pointer_cast<PartAsset_Body>(partConfig->getPartAsset())) continue;
			if (dynamic_pointer_cast<PartAsset_Nucleus>(partConfig->getPartAsset())) continue;

			auto t2 = Transformer2D(partConfig->getMat3x2(), true);

			if (partConfig->getPartAsset()->getShape().getPolygon().mouseOver())
			{
				m_selectedPartConfig = partConfig;
				m_deltaPosition = Vec2::Zero();
				m_deltaRotation = 0.0;
			}
		}
	}

	// selectedPart
	if (auto& selectedPart = getParentViewer()->getChildViewer<PartList>()->getSelectedPart())
	{
		bool canSetPart = m_cellAsset->getBodyAsset()->getShape().getPolygon().contains(Cursor::PosF());

		if (isMouseover())
		{
			{
				auto t2 = Transformer2D(Mat3x2::Translate(Cursor::PosF()));

				selectedPart->getShape().draw(0.5);
			}

			if (MouseL.up() && canSetPart)
			{
				const auto& partConfig = m_cellAsset->addPartConfig();

				partConfig->setPartAsset(selectedPart);
				partConfig->setPosition(Vec2(Cursor::PosF().x, Cursor::PosF().y));
				partConfig->setRotation(0.0);
			}

			selectedPart->getShape().getPolygon().movedBy(Cursor::PosF()).draw(ColorF(canSetPart ? Palette::Green : Palette::Red, 0.5));
		}

		if (!MouseL.pressed()) getParentViewer()->getChildViewer<PartList>()->clearSelectedPart();
	}
}

void	MainViewer::CellMakingViewer::PartsAssembler::Workspace::setMoveMode()
{
	m_state = State::MoveMode;
	getParentViewer()->getChildViewer<GUIButton>(U"移動モード")->setIsEnabled(false);
	getParentViewer()->getChildViewer<GUIButton>(U"回転モード")->setIsEnabled(true);
}

void	MainViewer::CellMakingViewer::PartsAssembler::Workspace::setRotateMode()
{
	m_state = State::RotateMode;
	getParentViewer()->getChildViewer<GUIButton>(U"移動モード")->setIsEnabled(true);
	getParentViewer()->getChildViewer<GUIButton>(U"回転モード")->setIsEnabled(false);
}

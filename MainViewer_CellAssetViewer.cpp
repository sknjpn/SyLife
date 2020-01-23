#include "MainViewer.h"
#include "CellAsset.h"
#include "ProteinAsset.h"
#include "PartAsset_Synthesizer.h"
#include "PartAsset_Nucleus.h"
#include "PartAsset_Needle.h"
#include "PartConfig.h"
#include "GUIText.h"
#include "GUITextBox.h"

MainViewer::CellAssetViewer::CellAssetViewer(const shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
{
}

void MainViewer::CellAssetViewer::init()
{
	setViewerSize(800, 600);

	// name
	addChildViewer<GUIText>(m_cellAsset->getNameJP(), Font(24, Typeface::Heavy), GUIText::Mode::DrawLeftCenter)
		->setViewerRectInLocal(5, 5, 400, 40);
}

void MainViewer::CellAssetViewer::update()
{
	RectF(getViewerSize()).rounded(5.0).draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

	// 数の推移
	{
		setDrawPos(5, 50);

		RectF(790, 80).draw(Color(11, 22, 33)).drawFrame(1.0, 0.0, Palette::Black);
	}

	// アイコン
	{
		setDrawPos(5, 140);
		const double size = 200;

		Circle(size / 2.0, size / 2.0, size / 2.0)
			.draw(ColorF(0.2))
			.drawFrame(2.0, 0.0, Palette::Black);

		auto t = Transformer2D(Mat3x2::Scale(0.75 * size / 2.0 / m_cellAsset->getRadius()).translated(size / 2.0, size / 2.0));
		m_cellAsset->draw();
	}

	// 性能
	{
		setDrawPos(210, 140);

		static Font font(16, Typeface::Default);

		// Proteins
		font(U"この生き物を作るのに必要なもの").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, 20);
		{
			moveDrawPos(8, 0);

			font(U"エレメント(緑色で広がっているもの)").draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);
			{
				moveDrawPos(8, 0);
				font(ToString(int(m_cellAsset->getMaterial().getElement())) + U"mL").draw(Vec2::Zero(), Palette::Black);
				moveDrawPos(0, 30);
				moveDrawPos(-8, 0);
			}

			font(U"プロテイン(合成器官で出来るもの)").draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);
			{
				moveDrawPos(8, 0);
				for (const auto& protein : m_cellAsset->getMaterial().getProteinList())
				{
					font(protein.first->getNameJP() + U": " + ToString(protein.second) + U"個").draw(Vec2::Zero(), Palette::Black);
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

			for (const auto& partConfig : m_cellAsset->getPartConfigs())
			{
				if (auto synthesizer = dynamic_pointer_cast<PartAsset_Synthesizer>(partConfig->getPartAsset()))
				{
					font(synthesizer->getExport()->getNameJP()).draw(Vec2::Zero(), Palette::Black);
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

			font(U"孵化までの時間", int(m_cellAsset->getNucleusAsset()->getBornTime()), U"秒").draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);

			font(U"産卵までの時間", int(m_cellAsset->getNucleusAsset()->getYieldTime()), U"秒").draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);

			font(U"寿命:", int(m_cellAsset->getNucleusAsset()->getLifespanTime()), U"秒").draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);

			font(U"硬さ:", int(m_cellAsset->getNucleusAsset()->getArmor())).draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);

			{
				int penetrating = 0;

				for (const auto& partConfig : m_cellAsset->getPartConfigs())
					if (auto needle = dynamic_pointer_cast<PartAsset_Needle>(partConfig->getPartAsset()))
						penetrating = Max(penetrating, needle->getPenetrating());

				font(U"トゲの貫通力:", penetrating).draw(Vec2::Zero(), Palette::Black);
				moveDrawPos(0, 20);
			}

			moveDrawPos(-8, 0);
		}
	}
}

#include "MainViewer.h"
#include "CellAsset.h"
#include "PartConfig.h"
#include "PartAsset.h"
#include "ProteinAsset.h"

void MainViewer::CellMakingViewer::CellInfo::init()
{
	m_textEditState_name.text = getParentViewer<CellMakingViewer>()->getCellAsset()->getName();
}

void MainViewer::CellMakingViewer::CellInfo::update()
{
	const auto& cellAsset = getParentViewer<CellMakingViewer>()->getCellAsset();

	moveDrawPos(5, 5);

	// Name
	{
		SimpleGUI::TextBox(m_textEditState_name, Vec2::Zero(), 290);

		moveDrawPos(0, 40);
	}

	// Release
	{
		const RectF rect = Rect(300, 300).stretched(-5);
		const double r = rect.size.x / 2.0;

		Circle(rect.size / 2.0, r)
			.draw(Palette::Skyblue)
			.drawFrame(2.0, 0.0, Palette::Black);

		// part
		{
			auto t1 = Transformer2D(Mat3x2::Translate(-cellAsset->getCentroid()).scaled(r / cellAsset->getRadius() / 2.0).translated(rect.center()));

			for (const auto& partConfig : cellAsset->getPartConfigs())
			{
				auto t2 = Transformer2D(partConfig->getMat3x2());

				partConfig->getPartAsset()->getShape().draw(0.5);
				partConfig->getPartAsset()->getShape().getPolygon().drawFrame(2.0, Palette::Black);
			}
		}

		moveDrawPos(0, 300);
	}

	// material
	{
		static Font font(16, Typeface::Bold);

		// Proteins
		font(U"この生き物を作るのに必要なもの").draw();
		moveDrawPos(0, 20);
		{
			moveDrawPos(8, 0);

			font(U"エレメント(緑色で広がっているもの)").draw();
			{
				moveDrawPos(8, 0);
				moveDrawPos(0, 20);
				font(ToString(int(cellAsset->getMaterial().getElement())) + U"個").draw();
				moveDrawPos(0, 30);
				moveDrawPos(-8, 0);
			}

			font(U"プロテイン(合成器官で出来るもの)").draw();
			{
				moveDrawPos(8, 0);
				for (const auto& protein : cellAsset->getMaterial().getProteinList())
				{
					font(protein.first->getName() + U": " + ToString(protein.second) + U"個").draw(Vec2::Zero(), Palette::Black);
					moveDrawPos(0, 20);
				}
				moveDrawPos(0, 10);
				moveDrawPos(-8, 0);
			}

			moveDrawPos(-8, 0);
		}

		font(U"この生き物が作れるプロテイン").draw();

		font(U"この生き物の特性(核で決まる)").draw();
		font(U"寿命").draw();
		font(U"増殖までの時間").draw();
		font(U"硬さ").draw();
		font(U"攻撃力").draw();
		font(U"移動力").draw();
	}
}

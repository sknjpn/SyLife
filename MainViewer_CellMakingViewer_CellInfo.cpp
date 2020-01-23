#include "MainViewer.h"
#include "CellAsset.h"
#include "PartConfig.h"
#include "PartAsset.h"
#include "ProteinAsset.h"

void MainViewer::CellMakingViewer::CellInfo::init()
{
	m_textEditState_name.text = getParentViewer<CellMakingViewer>()->getCellAsset()->getNameJP();
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

	// material
	{
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
				font(ToString(int(cellAsset->getMaterial().getElement())) + U"個").draw(Vec2::Zero(), Palette::Black);
				moveDrawPos(0, 30);
				moveDrawPos(-8, 0);
			}

			font(U"プロテイン(合成器官で出来るもの)").draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);
			{
				moveDrawPos(8, 0);
				for (const auto& protein : cellAsset->getMaterial().getProteinList())
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

		font(U"この生き物の特性(核で決まる)").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, 20);
		font(U"寿命").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, 20);
		font(U"増殖までの時間").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, 20);
		font(U"硬さ").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, 20);
		font(U"攻撃力").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, 20);
		font(U"移動力").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, 20);
	}
}

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
		static Font font(24);

		//font(cellAsset->getName()).drawAt(Vec2(95, 20));

		SimpleGUI::TextBox(m_textEditState_name, Vec2::Zero(), 190);

		moveDrawPos(0, 40);
	}

	// Release
	{
		const RectF rect = Rect(200, 200).stretched(-5);
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

		moveDrawPos(0, 200);
	}

	// material
	{
		static Font font(16, Typeface::Bold);

		// Nutrition
		font(U"Nutrition: " + ToString(cellAsset->getMaterial().getNutrition())).draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, 20);

		// Proteins
		for (const auto& protein : cellAsset->getMaterial().getProteinList())
		{
			font(protein.first->getName() + U": " + ToString(protein.second) + U"U").draw(Vec2::Zero(), Palette::Black);

			moveDrawPos(0, 20);
		}
	}
}

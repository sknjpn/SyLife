#include "CellMakingViewer.h"
#include "CellAsset.h"
#include "PartConfig.h"
#include "PartAsset.h"
#include "ElementAsset.h"

void CellMakingViewer::CellInfo::init()
{
	m_textEditState_name.text = getParentViewer<CellMakingViewer>()->getCellAsset()->getName();
}

void CellMakingViewer::CellInfo::update()
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
			.drawFrame(4.0, Palette::Black);

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

		setDrawPos(Vec2(0, 330));
	}

	// material
	{
		static Font font(13, Typeface::Bold);

		// Nutrition
		font(U"Nutrition: " + ToString(cellAsset->getMaterial().getNutrition())).draw();
		moveDrawPos(0, 20);

		// Elements
		for (const auto& element : cellAsset->getMaterial().getElementList())
		{
			font(element.first->getName() + U": " + ToString(element.second) + U"U").draw();

			moveDrawPos(0, 16);
		}
	}
}

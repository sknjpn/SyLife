#include "CellMakingViewer.h"

#include "AssetManager.h"

#include "PartConfig.h"
#include "PartAsset.h"
#include "BodyAsset.h"
#include "ElementAsset.h"
#include "CellAsset.h"
#include "ReleaseViewer.h"

#include "AssemblyViewer.h"
#include "ReleaseViewer.h"
#include "PartPaletteViewer.h"
#include "ShapeAssemblyViewer.h"
#include "ShapeLayerViewer.h"

void CellMakingViewer::update()
{
	// Release
	{
		const RectF rect = Rect(200, 200).stretched(-5);
		const double r = rect.size.x / 2.0;
		setDrawPos(rect.pos);

		Circle(rect.size / 2.0, r)
			.draw(Palette::Skyblue)
			.drawFrame(4.0, Palette::Black);

		// part
		{
			auto t1 = Transformer2D(Mat3x2::Translate(-m_cellAsset->getCentroid()).scaled(r / m_cellAsset->getRadius() / 2.0).translated(rect.center()));

			for (const auto& p : m_cellAsset->getPartConfigs())
			{
				auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
					.translated(p->getPosition().x, p->getPosition().y));

				p->getPartAsset()->getShape().draw(0.5);
				p->getPartAsset()->getShape().getPolygon().drawFrame(2.0, Palette::Black);
			}
		}
	}

	// Release Button
	{
		setDrawPos(5, 210);

		if (SimpleGUI::Button(U"Release", Vec2(0, 0), 180))
		{
			addChildViewer<ReleaseViewer>(m_cellAsset);
			destroy();
			return;
		}
	}

	// Close Button
	{
		setDrawPos(5, 250);

		if (SimpleGUI::Button(U"Close", Vec2(0, 0), 180))
		{
			destroy();
			return;
		}
	}

	// material
	{
		setDrawPos(Vec2(0, 330));

		static Font font(13, Typeface::Bold);

		// Nutrition
		font(U"Nutrition: " + ToString(m_cellAsset->getMaterial().getNutrition())).draw();
		moveDrawPos(0, 20);

		// Elements
		for (const auto& e : m_cellAsset->getMaterial().getElementList())
		{
			font(Unicode::Widen(e.first->getName()) + U": " + ToString(e.second) + U"U").draw();

			moveDrawPos(0, 16);
		}
	}

	switch (m_mode)
	{
	case CellMakingViewer::Mode::EditParts:

		// Mode Change Button
	{
		setDrawPos(5, 290);

		if (SimpleGUI::Button(U"Shape Mode", Vec2(0, 0), 180))
		{
			setMode(Mode::EditBodyShapes);

			return;
		}
	}
	break;

	case CellMakingViewer::Mode::EditBodyShapes:


		// Mode Change Button
	{
		setDrawPos(5, 290);

		if (SimpleGUI::Button(U"Part Mode", Vec2(0, 0), 180))
		{
			setMode(Mode::EditParts);

			return;
		}
	}
	break;
	}
}

void CellMakingViewer::init()
{
	addChildViewer<AssemblyViewer>();
	addChildViewer<PartPaletteViewer>();
	addChildViewer<ShapeLayerViewer>();
	addChildViewer<ShapeAssemblyViewer>();
	
	// 新しいモデルの登録
	makeAsset();

	setMode(Mode::EditParts);
}

void CellMakingViewer::setMode(Mode mode)
{
	m_mode = mode;

	switch (mode)
	{
	case CellMakingViewer::Mode::EditParts:
		// BackgroundColorの設定
		setBackgroundColor(Color(11, 22, 33));

		// DrawRectの設定
		setViewerRect(RectF(1200, 900).setCenter(Scene::CenterF()));

		break;

	case CellMakingViewer::Mode::EditBodyShapes:
		// BackgroundColorの設定
		setBackgroundColor(Color(11, 22, 33));

		// DrawRectの設定
		setViewerRect(RectF(1200, 900).setCenter(Scene::CenterF()));
		break;
	}
}

void CellMakingViewer::makeAsset()
{
	m_bodyAsset = g_assetManagerPtr->makeAsset<BodyAsset>();
	m_cellAsset = g_assetManagerPtr->makeAsset<CellAsset>();
	getChildViewer<AssemblyViewer>()->m_cellAsset = m_cellAsset;

	{
		m_bodyAsset->m_mass = 1.0;
		m_bodyAsset->m_material.setNutrition(1.0);
		auto& l = m_bodyAsset->m_shape.emplace_back();
		l.m_color = Palette::White;
		l.m_polygon = Circle(10.0).asPolygon();
	}

	m_cellAsset->addPartConfig()->setPartAsset(m_bodyAsset);
	m_cellAsset->updateProperties();

	getChildViewer<ShapeAssemblyViewer>()->setPartAsset(m_bodyAsset);
	getChildViewer<ShapeLayerViewer>()->setPartAsset(m_bodyAsset);
}

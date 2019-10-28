#include "CellMakingViewer.h"

#include "ViewerManager.h"
#include "AssetManager.h"

#include "PartConfig.h"
#include "PartAsset.h"
#include "BodyAsset.h"
#include "ElementAsset.h"
#include "CellAsset.h"
#include "AssemblyViewer.h"
#include "ReleaseViewer.h"
#include "PartPaletteViewer.h"
#include "ShapeAssemblyViewer.h"
#include "ShapeLayerViewer.h"

void CellMakingViewer::init()
{
	setPriority(1);

	// サブViewer生成
	//m_assemblyViewer = g_viewerManagerPtr->makeViewer<AssemblyViewer>();
	//m_partPaletteViewer = g_viewerManagerPtr->makeViewer<PartPaletteViewer>();
	//m_releaseViewer = g_viewerManagerPtr->makeViewer<ReleaseViewer>();
	//m_shapeAssemblyViewer = g_viewerManagerPtr->makeViewer<ShapeAssemblyViewer>();
	//m_shapeLayerViewer = g_viewerManagerPtr->makeViewer<ShapeLayerViewer>();

	// 新しいモデルの登録
	makeAsset();

	setMode(Mode::Close);
}

void CellMakingViewer::update()
{
	switch (m_mode)
	{
	case CellMakingViewer::Mode::Close:
	{
		static bool isClicked = false;


		if (isMouseOver())
		{
			if (MouseL.down()) isClicked = true;

			if (MouseL.up())
			{
				isClicked = false;

				setMode(Mode::EditBodyShapes);

				return;
			}

			if (isClicked && MouseL.pressed()) setBackgroundColor(ColorF(1.0, 0.3));
			else setBackgroundColor(ColorF(1.0, 0.5));
		}
		else
		{
			if (MouseL.up()) isClicked = false;

			setBackgroundColor(ColorF(1.0, 0.1));
		}
	}

	// Text
	{
		static Font font(32, Typeface::Bold);

		font(U"Create Cell").drawAt(getViewerSize() / 2.0);
	}
	break;
	case CellMakingViewer::Mode::EditParts:

		// 閉じる
		if (KeyEscape.down())
		{
			setMode(Mode::Close);

			return;
		}

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
				setMode(Mode::Release);

				return;
			}
		}

		// Close Button
		{
			setDrawPos(5, 250);

			if (SimpleGUI::Button(U"Close", Vec2(0, 0), 180))
			{
				setMode(Mode::Close);

				return;
			}
		}

		// Mode Change Button
		{
			setDrawPos(5, 290);

			if (SimpleGUI::Button(U"Shape Mode", Vec2(0, 0), 180))
			{
				setMode(Mode::EditBodyShapes);

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
		break;

	case CellMakingViewer::Mode::EditBodyShapes:

		// 閉じる
		if (KeyEscape.down())
		{
			setMode(Mode::Close);

			return;
		}

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
				setMode(Mode::Release);

				return;
			}
		}

		// Close Button
		{
			setDrawPos(5, 250);

			if (SimpleGUI::Button(U"Close", Vec2(0, 0), 180))
			{
				setMode(Mode::Close);

				return;
			}
		}

		// Mode Change Button
		{
			setDrawPos(5, 290);

			if (SimpleGUI::Button(U"Part Mode", Vec2(0, 0), 180))
			{
				setMode(Mode::EditParts);

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
		break;
	case CellMakingViewer::Mode::Release:
		break;
	default:
		break;
	}
}

void CellMakingViewer::setMode(Mode mode)
{
	m_mode = mode;

	switch (mode)
	{
	case CellMakingViewer::Mode::Close:
		setInvisible(false);
		m_assemblyViewer->setInvisible(true);
		m_partPaletteViewer->setInvisible(true);
		m_releaseViewer->setInvisible(true);
		m_shapeAssemblyViewer->setInvisible(true);
		m_shapeLayerViewer->setInvisible(true);

		// DrawRectの設定
		setViewerRect(RectF(200, 50).setCenter(Scene::CenterF().x, 50));
		break;

	case CellMakingViewer::Mode::EditParts:
		setInvisible(false);
		m_assemblyViewer->setInvisible(false);
		m_partPaletteViewer->setInvisible(false);
		m_releaseViewer->setInvisible(true);
		m_shapeAssemblyViewer->setInvisible(true);
		m_shapeLayerViewer->setInvisible(true);

		// BackgroundColorの設定
		setBackgroundColor(Color(11, 22, 33));

		// DrawRectの設定
		setViewerRect(RectF(1200, 900).setCenter(Scene::CenterF()));

		break;

	case CellMakingViewer::Mode::EditBodyShapes:
		setInvisible(false);
		m_assemblyViewer->setInvisible(true);
		m_partPaletteViewer->setInvisible(true);
		m_releaseViewer->setInvisible(true);
		m_shapeAssemblyViewer->setInvisible(false);
		m_shapeLayerViewer->setInvisible(false);

		// BackgroundColorの設定
		setBackgroundColor(Color(11, 22, 33));

		// DrawRectの設定
		setViewerRect(RectF(1200, 900).setCenter(Scene::CenterF()));
		break;

	case CellMakingViewer::Mode::Release:
		setInvisible(true);
		m_assemblyViewer->setInvisible(true);
		m_partPaletteViewer->setInvisible(true);
		m_releaseViewer->setInvisible(false);
		m_shapeAssemblyViewer->setInvisible(true);
		m_shapeLayerViewer->setInvisible(true);
		break;

	default:
		break;
	}
}

void CellMakingViewer::makeAsset()
{
	m_bodyAsset = g_assetManagerPtr->makeAsset<BodyAsset>();
	m_cellAsset = g_assetManagerPtr->makeAsset<CellAsset>();

	{
		m_bodyAsset->m_mass = 1.0;
		m_bodyAsset->m_material.setNutrition(1.0);
		auto& l = m_bodyAsset->m_shape.emplace_back();
		l.m_color = Palette::White;
		l.m_polygon = Circle(10.0).asPolygon();
	}

	m_cellAsset->addPartConfig()->setPartAsset(m_bodyAsset);
	m_cellAsset->updateProperties();

	m_shapeAssemblyViewer->setPartAsset(m_bodyAsset);
	m_shapeLayerViewer->setPartAsset(m_bodyAsset);
}

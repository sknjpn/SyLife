#include "MainViewer.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "CellAsset.h"
#include "GUIChecker.h"
#include "PartAsset_Body.h"

Polygon MainViewer::CellMakingViewer::BodySculptor::Workspace::getReversed(const Polygon& polygon) const
{
	auto outer = polygon.outer();
	for (auto& point : outer) point.x *= -1;

	outer.reverse();

	auto inners = polygon.inners();
	for (auto& hole : inners)
	{
		for (auto& point : hole)
			point.x *= -1;

		hole.reverse();
	}

	return Polygon(outer, inners);
}

Layer& MainViewer::CellMakingViewer::BodySculptor::Workspace::getSelectedLayer()
{
	const auto slv = getParentViewer()->getChildViewer<LayerLists>();

	return m_bodyAsset->getShape()[slv->getSelectedIndex()];
}

void MainViewer::CellMakingViewer::BodySculptor::Workspace::init()
{
	setBackgroundColor(Palette::Black);

	setViewerRectInLocal(200, 0, 800, 800);
}

void MainViewer::CellMakingViewer::BodySculptor::Workspace::update()
{
	auto t = Transformer2D(Mat3x2::Scale(4).translated(400, 400), true);

	m_bodyAsset->getShape().updateProperties();

	// Part
	m_bodyAsset->getShape().draw(0.5);
	m_bodyAsset->getShape().getPolygon().drawFrame(1.0, Palette::Black);

	// タッチパネル用に押し下げた瞬間は処理しない
	if (MouseL.pressed() && !MouseL.down())
	{
		auto p1 = Cursor::PreviousPosF();
		auto p2 = Cursor::PosF();
		auto r = getParentViewer<BodySculptor>()->getStampRadius();

		auto stamp = (p1 == p2 ? Polygon() : Polygon({ p1 + (p1 - p2).setLength(r).rotated(-Math::HalfPi), p1 + (p1 - p2).setLength(r).rotated(Math::HalfPi), p2 + (p1 - p2).setLength(r).rotated(Math::HalfPi), p2 + (p1 - p2).setLength(r).rotated(-Math::HalfPi) }));
		stamp.append(Circle(getParentViewer<BodySculptor>()->getStampRadius()).asPolygon().movedBy(Cursor::PosF()));
		stamp.append(Circle(getParentViewer<BodySculptor>()->getStampRadius()).asPolygon().movedBy(Cursor::PreviousPosF()));

		// Mouse
		{
			if (getParentViewer<BodySculptor>()->getChildViewer<GUIChecker>(U"左右対称")->getValue())
			{
				stamp.draw(ColorF(getSelectedLayer().m_color, 0.5));
				getReversed(stamp).draw(ColorF(getSelectedLayer().m_color, 0.5));
			}
			else
			{
				stamp.draw(ColorF(getSelectedLayer().m_color, 0.5));
			}
		}

		if (isMouseover())
		{
			if (MouseL.pressed())
			{
				switch (getParentViewer<BodySculptor>()->getState())
				{
				case BodySculptor::State::Put:
					attach(stamp);
					break;

				case BodySculptor::State::Shave:
				{
					detach(stamp);

					if (getParentViewer<BodySculptor>()->getChildViewer<GUIChecker>(U"左右対称")->getValue())
						detach(getReversed(stamp));

				}
				break;
				}
			}

			double k = 0.01;
			m_bodyAsset->setMass(m_bodyAsset->getShape()[0].m_polygon.area() * k);
			m_bodyAsset->getMaterial().setElement(m_bodyAsset->getMass());
		}
	}

	// 左右対称
	if (getParentViewer<BodySculptor>()->getChildViewer<GUIChecker>(U"左右対称")->getValue())
		getSelectedLayer().m_polygon.append(getReversed(getSelectedLayer().m_polygon));

	// パーツの描画
	{
		const auto cellAsset = getParentViewer<BodySculptor>()->getParentViewer<MainViewer::CellMakingViewer>()->getCellAsset();

		for (const auto& p : cellAsset->getPartConfigs())
		{
			// 二度も同じものを描画しない
			if (p->getPartAsset() == m_bodyAsset) continue;

			auto t2 = Transformer2D(p->getMat3x2());

			p->getPartAsset()->getShape().draw(0.25);
			p->getPartAsset()->getShape().getPolygon().drawFrame(1.0, Palette::Black);
		}
	}
}

void MainViewer::CellMakingViewer::BodySculptor::Workspace::attach(const Polygon& polygon)
{
	getSelectedLayer().m_polygon.append(polygon);
}

void MainViewer::CellMakingViewer::BodySculptor::Workspace::detach(const Polygon& polygon)
{
	const auto polygons = Geometry2D::Subtract(getSelectedLayer().m_polygon, polygon);

	if (polygons.empty()) getSelectedLayer().m_polygon = Polygon();
	else
	{
		int maxIndex = 0;
		for (int i = 1; i < polygons.size(); ++i)
			if (polygons[maxIndex].area() < polygons[i].area()) maxIndex = i;

		getSelectedLayer().m_polygon = polygons[maxIndex];
	}
}


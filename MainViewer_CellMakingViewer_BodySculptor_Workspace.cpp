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
	const auto color = getParentViewer()->getChildViewer<ColorSelector>()->getSelectedColor();

	// スタンプと既存の形が重なるかどうか
	const auto stamp = getStamp();
	for (auto& layer : m_bodyAsset->getShape())
		if (layer.m_color == color && layer.m_polygon.intersects(stamp)) return layer;

	{
		auto& layer = m_bodyAsset->getShape().emplace_back();

		layer.m_color = color;

		return layer;
	}
}

Polygon MainViewer::CellMakingViewer::BodySculptor::Workspace::getStamp() const
{
	const auto p1 = Cursor::PreviousPosF();
	const auto p2 = Cursor::PosF();
	const auto r = getParentViewer<BodySculptor>()->getStampRadius();

	auto stamp = (p1 == p2 ? Polygon() : Polygon({ p1 + (p1 - p2).setLength(r).rotated(-Math::HalfPi), p1 + (p1 - p2).setLength(r).rotated(Math::HalfPi), p2 + (p1 - p2).setLength(r).rotated(Math::HalfPi), p2 + (p1 - p2).setLength(r).rotated(-Math::HalfPi) }));
	stamp.append(Circle(getParentViewer<BodySculptor>()->getStampRadius()).asPolygon().movedBy(Cursor::PosF()));
	stamp.append(Circle(getParentViewer<BodySculptor>()->getStampRadius()).asPolygon().movedBy(Cursor::PreviousPosF()));

	return stamp;
}

void MainViewer::CellMakingViewer::BodySculptor::Workspace::init()
{
	setBackgroundColor(Palette::Black);

	setViewerRectInLocal(200, 0, 800, 800);
}

void MainViewer::CellMakingViewer::BodySculptor::Workspace::update()
{
	auto t = Transformer2D(Mat3x2::Scale(4).translated(400, 400), true);

	// タッチパネル用に押し下げた瞬間は処理しない
	if (MouseL.pressed() && !MouseL.down())
	{
		const auto stamp = getStamp();

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

	// 合成
	{
		auto& shape = m_bodyAsset->getShape();

		// 空のLayerの削除
		shape.remove_if([](const auto& layer) { return layer.m_polygon.isEmpty(); });

		// くっついているLayerの結合
		for (;;)
		{
			bool flag = true;

			for (auto it1 = shape.begin(); it1 != shape.end(); ++it1)
			{
				for (auto it2 = it1 + 1; it2 != shape.end(); ++it2)
				{
					if (it1->m_color == it2->m_color && it1->m_polygon.intersects(it2->m_polygon))
					{
						Polygon newPolygon;
						newPolygon.append(it1->m_polygon);
						newPolygon.append(it2->m_polygon);
						const Color newColor = it1->m_color;

						shape.erase(it2);
						shape.erase(it1);

						auto& layer = shape.emplace_back();
						layer.m_color = newColor;
						layer.m_polygon = newPolygon;

						flag = false;
						break;
					}
				}
				if (!flag) break;
			}

			if (flag) break;
		}

		// 分離しているLayerの削除
		{
			Array<Polygon> polygons;
			for (const auto& layer : m_bodyAsset->getShape()) polygons.emplace_back(layer.m_polygon);

			for (;;)
			{
				bool flag = true;

				for (auto it1 = polygons.begin(); it1 != polygons.end(); ++it1)
				{
					for (auto it2 = it1 + 1; it2 != polygons.end(); ++it2)
					{
						if (it1->intersects(*it2))
						{
							it1->append(*it2);

							polygons.erase(it2);

							flag = false;

							break;
						}
					}

					if (!flag) break;
				}

				if (flag) break;
			}

			polygons.sort_by([](const auto& p1, const auto& p2) { return p1.area() > p2.area(); });

			if (!polygons.isEmpty()) m_bodyAsset->getShape().remove_if([&polygons](const auto& layer) { return !layer.m_polygon.intersects(polygons.front()); });
		}
	}

	// 描画
	{
		m_bodyAsset->getShape().updateProperties();

		// Part
		m_bodyAsset->getShape().draw(0.5);
		m_bodyAsset->getShape().getPolygon().drawFrame(1.0, Palette::Black);
	}

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
	detach(polygon.calculateBuffer(-1));
	getSelectedLayer().m_polygon.append(polygon);
}

void MainViewer::CellMakingViewer::BodySculptor::Workspace::detach(const Polygon& polygon)
{
	auto& shape = m_bodyAsset->getShape();
	for (int i = 0; i < shape.size(); ++i)
	{
		auto& layer = shape[i];
		const auto polygons = Geometry2D::Subtract(layer.m_polygon, polygon);

		if (polygons.empty()) layer.m_polygon = Polygon();
		else
		{
			layer.m_polygon = polygons[0];
			for (int j = 1; j < polygons.size(); ++j)
			{
				auto& newLayer = shape.emplace_back();
				newLayer.m_color = layer.m_color;
				newLayer.m_polygon = polygons[j];
			}
		}
	}
}


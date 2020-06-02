#include "MainViewer.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "CellAsset.h"
#include "GUIChecker.h"
#include "Part_BodyAsset.h"

Polygon MainViewer::CellMakingViewer::BodySculptor::Workspace::getStamp() const
{
	const auto p1 = Cursor::PreviousPosF();
	const auto p2 = Cursor::PosF();
	const auto r = getParentViewer<BodySculptor>()->getStampRadius();

	if (p1 == p2) return Circle(p1, r).asPolygon();

	const auto v = (p2 - p1).normalized();

	Array<Vec2> outers;
	double delta = 1.0 / r;
	for (double ang = delta; ang < Math::Pi; ang += delta)
	{
		outers.emplace_back(p1 + (v * r).rotated(Math::HalfPi + ang));
	}

	for (double ang = delta; ang < Math::Pi; ang += delta)
	{
		outers.emplace_back(p2 + (v * r).rotated(-Math::HalfPi + ang));
	}

	return Polygon(outers);
}

Polygon MainViewer::CellMakingViewer::BodySculptor::Workspace::getStampOnImage() const
{
	return getStamp().scaled(GeneralSetting::GetInstance().m_textureScale).movedBy(getViewerSize() / 2.0);
}

Polygon MainViewer::CellMakingViewer::BodySculptor::Workspace::getStampReversed() const
{
	const auto polygon = getStamp();

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

Polygon MainViewer::CellMakingViewer::BodySculptor::Workspace::getStampOnImageReversed() const
{
	return getStampReversed().scaled(GeneralSetting::GetInstance().m_textureScale).movedBy(getViewerSize() / 2.0);
}

const Color& MainViewer::CellMakingViewer::BodySculptor::Workspace::getColor() const
{
	return getParentViewer()->getChildViewer<ColorSelector>()->getSelectedColor();
}

void MainViewer::CellMakingViewer::BodySculptor::Workspace::onDestroy()
{
	m_bodyAsset->m_texture = Texture(m_bodyAsset->m_image);
}

void MainViewer::CellMakingViewer::BodySculptor::Workspace::init()
{
	setBackgroundColor(Color(11, 22, 33));

	setViewerRectInLocal(200, 0, 800, 800);

	m_bodyAsset->m_image.resize(getViewerSize().asPoint());

	m_texture.fill(m_bodyAsset->m_image);
}

void MainViewer::CellMakingViewer::BodySculptor::Workspace::update()
{
	const bool reverseEnabled = getParentViewer<BodySculptor>()->getChildViewer<GUIChecker>(U"左右対称")->getValue();

	// タッチパネル用に押し下げた瞬間は処理しない
	if (MouseL.pressed() && !MouseL.down())
	{
		auto t = Transformer2D(Mat3x2::Scale(GeneralSetting::GetInstance().m_textureScale).translated(getViewerSize() / 2), true);

		// 適用
		if (isMouseover())
		{
			if (MouseL.pressed())
			{
				if (!getParentViewer()->getChildViewer<ColorSelector>()->isEraseMode())
				{
					m_bodyAsset->getShape().front().m_polygon.append(getStamp());
					getStampOnImage().overwrite(m_bodyAsset->m_image, getColor());

					if (reverseEnabled)
					{
						m_bodyAsset->getShape().front().m_polygon.append(getStampReversed());
						getStampOnImageReversed().overwrite(m_bodyAsset->m_image, getColor());
					}
				}
				else
				{
					detach(getStamp());
					getStampOnImage().overwrite(m_bodyAsset->m_image, Color(0, 0));

					if (reverseEnabled)
					{
						detach(getStampReversed());
						getStampOnImageReversed().overwrite(m_bodyAsset->m_image, Color(0, 0));
					}
				}
			}

			double k = 0.01;
			m_bodyAsset->setMass(m_bodyAsset->getShape()[0].m_polygon.area() * k);
			m_bodyAsset->getMaterial().setElement(m_bodyAsset->getMass());
		}

	}
	// 左右対称
	//if (reverseEnabled)
		//getSelectedLayer().m_polygon.append(getReversed(getSelectedLayer().m_polygon));

	// 調整
	{
		for (const auto& polygon : Geometry2D::Subtract(RectF(getViewerSize()).asPolygon(), m_bodyAsset->getShape().front().m_polygon.scaled(GeneralSetting::GetInstance().m_textureScale).movedBy(getViewerSize() / 2.0)))
			polygon.overwrite(m_bodyAsset->m_image, Color(0, 0));
	}

	// 描画
	{
		m_texture.fill(m_bodyAsset->m_image);
		m_texture.draw(ColorF(1.0, 0.5));
		/*
		m_bodyAsset->getShape().updateProperties();

		// Part
		m_bodyAsset->getShape().draw(0.5);
		m_bodyAsset->getShape().getPolygon().drawFrame(1.0, Palette::Black);
		*/
	}

	{
		auto t = Transformer2D(Mat3x2::Scale(GeneralSetting::GetInstance().m_textureScale).translated(getViewerSize() / 2), true);

		//m_bodyAsset->getShape().front().m_polygon.draw(ColorF(1.0, 0.5));
	}

	// Mouse
	if (isMouseover() && !GeneralSetting::GetInstance().m_touchPanelModeEnabled || MouseL.pressed())
	{
		auto t = Transformer2D(Mat3x2::Scale(GeneralSetting::GetInstance().m_textureScale).translated(getViewerSize() / 2), true);

		// Mouse
		{
			getStamp().draw(ColorF(getColor(), 0.5));

			if (reverseEnabled) getStampReversed().draw(ColorF(getColor(), 0.5));
		}
	}

	// パーツの描画
	{
		auto t = Transformer2D(Mat3x2::Scale(GeneralSetting::GetInstance().m_textureScale).translated(getViewerSize() / 2), true);

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

void MainViewer::CellMakingViewer::BodySculptor::Workspace::detach(const Polygon& polygon)
{
	const auto polygons = Geometry2D::Subtract(m_bodyAsset->getShape().front().m_polygon, polygon);

	if (polygons.isEmpty()) m_bodyAsset->getShape().front().m_polygon = Polygon();
	else m_bodyAsset->getShape().front().m_polygon = polygons
		.sorted_by([](const auto& p1, const auto& p2) { return p1.area() > p2.area(); })
		.front();
}

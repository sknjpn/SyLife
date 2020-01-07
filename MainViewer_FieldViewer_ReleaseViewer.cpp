#include "MainViewer.h"
#include "World.h"
#include "CellAsset.h"
#include "CellState.h"
#include "PartConfig.h"
#include "PartAsset.h"

MainViewer::FieldViewer::ReleaseViewer::ReleaseViewer(const shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
{
	setBackgroundColor(Color(0, 0));
}

void MainViewer::FieldViewer::ReleaseViewer::update()
{
	auto fv = getParentViewer<FieldViewer>();

	auto t = fv->getCamera().createTransformer();
	Circle circle(Cursor::PosF(), m_cellAsset->getRadius() * 2.0);

	circle
		.draw(circle.mouseOver() ? Palette::Orange : Palette::Skyblue)
		.drawFrame(4.0, Palette::Black);

	// part
	{
		auto t1 = Transformer2D(Mat3x2::Translate(Cursor::PosF()));

		for (const auto& p : m_cellAsset->getPartConfigs())
		{
			auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
				.translated(p->getPosition().x, p->getPosition().y));

			p->getPartAsset()->getShape().draw(0.5);
		}
	}

	// Release
	if (MouseL.up())
	{
		// CellAssetのリセット
		m_cellAsset->setCentroidAsOrigin();

		// 新規Cell
		const auto& c = World::GetInstance()->addCellState(m_cellAsset);
		c->setPosition(Cursor::PosF());
		c->setVelocity(Vec2::Zero());
		c->init();

		destroy();
	}
}

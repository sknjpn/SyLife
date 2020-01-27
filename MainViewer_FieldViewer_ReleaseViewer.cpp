#include "MainViewer.h"
#include "World.h"
#include "CellAsset.h"
#include "CellState.h"
#include "PartConfig.h"
#include "PartAsset.h"

MainViewer::FieldViewer::ReleaseViewer::ReleaseViewer(const std::shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
{

}

void MainViewer::FieldViewer::ReleaseViewer::init()
{
	mouseoverDisable();
	setBackgroundColor(Color(0, 0));
}

void MainViewer::FieldViewer::ReleaseViewer::update()
{
	auto fv = getParentViewer<FieldViewer>();

	auto t = fv->getCamera().createTransformer();

	Circle(Cursor::PosF(), m_cellAsset->getRadius() * 2.0)
		.draw(ColorF(Palette::Orange, 0.5))
		.drawFrame(3.0, Palette::Black);

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
		if (getParentViewer()->isMouseover())
		{
			// CellAssetのリセット
			m_cellAsset->setCentroidAsOrigin();

			// 新規Cell
			const auto& c = World::GetInstance()->addCellState(m_cellAsset);
			c->setPosition(Cursor::PosF());
			c->setVelocity(Vec2::Zero());
			c->init();
		}

		destroy();
	}
}

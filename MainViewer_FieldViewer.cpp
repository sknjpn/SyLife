#include "MainViewer.h"
#include "World.h"
#include "Rigidbody.h"
#include "CellAsset.h"
#include "CellState.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "EggState.h"
#include "TileState.h"
#include "ProteinAsset.h"

void MainViewer::FieldViewer::openCellMakingViewer()
{
	if (!hasChildViewer<CellMakingViewer>()) addChildViewer<CellMakingViewer>();
}

void MainViewer::FieldViewer::init()
{
	m_camera.setRestrictedRect(RectF(World::GetInstance()->getFieldSize()));
	m_camera.setMaxScale(4);
	m_camera.setMinScale(0.1);
	m_camera.setCenter(m_camera.getRestrictedRect()->center());
	m_camera.setTargetCenter(m_camera.getRestrictedRect()->center());
}

void MainViewer::FieldViewer::update()
{
	// エッジスクロール (タッチパネル使用時はMouseL.pressed()を要求)
	if (!GeneralSetting::GetInstance().m_touchPanelModeEnabled || MouseL.pressed())
	{
		if (Cursor::Pos().x < 50) { Rect(50, Scene::Height()).draw(ColorF(0.5)); m_camera.moveL(); }
		if (Cursor::Pos().y < 50) { Rect(Scene::Width(), 50).draw(ColorF(0.5)); m_camera.moveU(); }
		if (Cursor::Pos().x > Scene::Width() - 50) { Rect(Scene::Width() - 50, 0, 50, Scene::Height()).draw(ColorF(0.5)); m_camera.moveR(); }
		if (Cursor::Pos().y > Scene::Height() - 50) { Rect(0, Scene::Height() - 50, Scene::Width(), 50).draw(ColorF(0.5)); m_camera.moveD(); }
	}

	{
		// camera
		m_camera.update();
		auto t = m_camera.createTransformer();

		// update
		int numUpdate = 0;
		{
			auto maxConut = m_isHighSpeed ? 100 : 1;
			Stopwatch sw(StartImmediately::Yes);
			int i = 0;
			for (; i < maxConut; ++i)
			{
				World::GetInstance()->update();

				if (auto sv = getParentViewer()->getChildViewer<StatisticsViewer>())
					sv->takeLog();

				numUpdate++;

				// 60FPSを保つ動作
				if (sw.ms() > 15) break;
			}
			m_frameCount = i;
		}

		// draw
		World::GetInstance()->draw();

		// Wave
		if (m_drawWaveEnabled)
		{
			for (auto p : step(World::GetInstance()->getTiles().size()))
			{
				const auto& tile = World::GetInstance()->getTiles()[p];
				const auto w = 3.0 * tile.getWaveVelocity() * TileLength / 2.0;

				Line(Vec2::Zero(), w)
					.movedBy(p * TileLength + Vec2(0.5, 0.5) * TileLength)
					.drawArrow(TileLength * 0.1, Vec2(TileLength * 0.2, TileLength * 0.2), ColorF(1.0, tile.getWaveVelocity().length()));
			}
		}

		// 選択中の生き物
		for (const auto& cellAssetViewer : getParentViewer<MainViewer>()->getChildViewers<CellAssetViewer>())
		{
			if (auto cellStateViewer = cellAssetViewer->getChildViewer<CellAssetViewer::CellStateViewer>())
			{
				const auto cellState = cellStateViewer->getCellState();
				const auto t2 = Transformer2D(cellState->getMat3x2());

				Circle(cellState->getRadius() * 1.5)
					.draw(ColorF(Palette::Red, 0.25))
					.drawFrame(2.0, Palette::Black);
			}
		}

		switch (m_handAction)
		{
		case MainViewer::FieldViewer::HandAction::None:
			if (isMouseover() && MouseL.down() && !World::GetInstance()->getCellStates().isEmpty())
			{
				auto index = World::GetInstance()->getCellStateKDTree().knnSearch(1, Cursor::PosF()).front();
				auto cellState = World::GetInstance()->getCellStates()[index];
				if (cellState->getRadius() > (cellState->getPosition() - Cursor::PosF()).length())
				{
					addChildViewer<CellStateCaptureViewer>(cellState);
					getParentViewer<MainViewer>()->addCellAssetViewer(cellState);
				}
			}
			break;
		case MainViewer::FieldViewer::HandAction::AddElement:
			if (isMouseover() && MouseL.pressed())
			{
				Circle circle(Cursor::PosF(), 256.0);
				circle.draw(ColorF(Palette::Green, 0.75));

				for (auto p : step(World::GetInstance()->getTiles().size()))
				{
					auto distance = (p * TileLength).distanceFrom(Cursor::PosF());
					if (distance < 256.0)
					{
						World::GetInstance()->getTile(p).addElement(Math::Lerp(0.0, 10.0 * numUpdate, 1.0 - distance / 256.0));
					}
				}
			}
			break;
		case MainViewer::FieldViewer::HandAction::Poison:
			if (isMouseover() && MouseL.pressed())
			{
				Circle circle(Cursor::PosF(), 256.0);
				circle.draw(ColorF(Palette::Purple, 0.75));

				for (auto p : step(World::GetInstance()->getTiles().size()))
				{
					auto distance = (p * TileLength).distanceFrom(Cursor::PosF());
					if (distance < 256.0)
					{
						World::GetInstance()->getTile(p).addPoison(Math::Lerp(0.0, 10.0 * numUpdate, 1.0 - distance / 256.0));
					}
				}
			}
			break;
		case MainViewer::FieldViewer::HandAction::Trash:
			if (isMouseover() && MouseL.pressed())
			{
				for (auto p : step(World::GetInstance()->getTiles().size()))
				{
					auto distance = (p * TileLength).distanceFrom(Cursor::PosF());
					if (distance < 256.0)
					{
						World::GetInstance()->getTile(p).setElement(0.0);
						World::GetInstance()->getTile(p).setPoison(0.0);
					}
				}

				Circle circle(Cursor::PosF(), 256.0);
				circle.draw(ColorF(Palette::Red, 0.75));

				for (const auto& c : World::GetInstance()->getCellStates())
					if (Circle(c->getPosition(), c->getRadius()).intersects(circle)) c->m_deathTimer = 0.0;

				for (const auto& e : World::GetInstance()->getEggStates())
					if (Circle(e->getPosition(), e->getRadius()).intersects(circle)) e->destroy();
			}
			break;
		default:
			break;
		}
	}

	// HighSpeed
	if (m_isHighSpeed)
		RectF(getViewerSize()).draw(ColorF(Palette::Red, 0.1)).drawFrame(8.0, 0.0, ColorF(Palette::Red, 0.5));
}

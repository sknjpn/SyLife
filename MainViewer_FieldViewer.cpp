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

#include "GUIButton.h"

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
	// エッジスクロール
	if (MouseL.pressed() && Cursor::Pos().x < 32) { Rect(32, Scene::Size().y).draw(ColorF(0.5)); m_camera.moveL(); }
	if (MouseL.pressed() && Cursor::Pos().y < 32) { Rect(Scene::Size().x, 32).draw(ColorF(0.5)); m_camera.moveU(); }
	if (MouseL.pressed() && Cursor::Pos().x > Scene::Size().x - 32) { Rect(Scene::Size().x - 32, 0, 32, Scene::Size().y).draw(ColorF(0.5)); m_camera.moveR(); }
	if (MouseL.pressed() && Cursor::Pos().y > Scene::Size().y - 32) { Rect(0, Scene::Size().y - 32, Scene::Size().x, 32).draw(ColorF(0.5)); m_camera.moveD(); }

	{
		// camera
		m_camera.update();
		auto t = m_camera.createTransformer();

		// update
		{
			auto maxConut = m_isHighSpeed ? 100 : 1;
			Stopwatch sw(true);
			int i = 0;
			for (; i < maxConut; ++i)
			{
				World::GetInstance()->update();

				getParentViewer()->
					getChildViewer<StatisticsViewer>()->takeLog();

				// 60FPSを保つ動作
				if (sw.ms() > 15) break;
			}
			m_frameCount = i;
		}

		// draw
		World::GetInstance()->draw();

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

					if (!getParentViewer<MainViewer>()->hasChildViewer(cellState->getCellAsset()->getName()))
						getParentViewer<MainViewer>()->addChildViewer<CellAssetViewer>(cellState->getCellAsset());
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
					auto distance = (p * World::GetInstance()->getTileLength()).distanceFrom(Cursor::PosF());
					if (distance < 256.0)
					{
						World::GetInstance()->getTile(p).addElement(Math::Lerp(0.0, 10.0, 1.0 - distance / 256.0));
					}
				}
			}
			break;
		case MainViewer::FieldViewer::HandAction::Poison:
			if (isMouseover() && MouseL.pressed())
			{
				Circle circle(Cursor::PosF(), 256.0);
				circle.draw(ColorF(Palette::Purple, 0.75));

				for (const auto& c : World::GetInstance()->getCellStates())
					if (Circle(c->getPosition(), c->getRadius()).intersects(circle)) c->m_deathTimer = 0.0;

				for (const auto& e : World::GetInstance()->getEggStates())
				{
					if (Circle(e->getPosition(), e->getRadius()).intersects(circle))
					{
						e->destroy();

						// Elementの吐き出し
						World::GetInstance()->getTile(e->getPosition()).addElement(e->getCellAsset()->getMaterial().getElementRecursive());
					}
				}
			}
			break;
		case MainViewer::FieldViewer::HandAction::Trash:
			if (isMouseover() && MouseL.pressed())
			{
				for (auto p : step(World::GetInstance()->getTiles().size()))
				{
					auto distance = (p * World::GetInstance()->getTileLength()).distanceFrom(Cursor::PosF());
					if (distance < 256.0)
					{
						World::GetInstance()->getTile(p).setElement(0.0);
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

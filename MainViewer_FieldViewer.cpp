#include "MainViewer.h"
#include "World.h"

#include "Rigidbody.h"
#include "CellAsset.h"
#include "CellState.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "EggState.h"
#include "TileState.h"
#include "ProteinState.h"
#include "ProteinAsset.h"

#include "GUIButton.h"

void MainViewer::FieldViewer::openCellMakingViewer()
{
	if (!hasChildViewer<CellMakingViewer>()) addChildViewer<CellMakingViewer>();
}

void MainViewer::FieldViewer::init()
{
	m_camera.setRestrictedRect(Rect(World::GetInstance()->getTileSize()).scaledAt(Vec2::Zero(), World::GetInstance()->getTileLength()));
	m_camera.setMaxScale(4);
	m_camera.setMinScale(0.1);
	m_camera.setCenter(m_camera.getRestrictedRect()->center());
	m_camera.setTargetCenter(m_camera.getRestrictedRect()->center());
}

void MainViewer::FieldViewer::update()
{
	Window::SetTitle(Cursor::PosF());

	// エッジスクロール
	if (MouseL.pressed() && Cursor::Pos().x < 32) { Rect(32, Scene::Size().y).draw(ColorF(0.5)); m_camera.moveL(); }
	if (MouseL.pressed() && Cursor::Pos().y < 32) { Rect(Scene::Size().x, 32).draw(ColorF(0.5)); m_camera.moveU(); }
	if (MouseL.pressed() && Cursor::Pos().x > Scene::Size().x -32) { Rect(Scene::Size().x - 32, 0, 32, Scene::Size().y).draw(ColorF(0.5)); m_camera.moveR(); }
	if (MouseL.pressed() && Cursor::Pos().y > Scene::Size().y -32) { Rect(0, Scene::Size().y - 32, Scene::Size().x, 32).draw(ColorF(0.5)); m_camera.moveD(); }

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

		// CellState Capture
		if (isMouseover() && MouseL.down() && !World::GetInstance()->getCellStates().isEmpty())
		{
			auto index = World::GetInstance()->getCellStateKDTree().knnSearch(1, Cursor::PosF()).front();
			auto cellState = World::GetInstance()->getCellStates()[index];
			if (cellState->getRadius() > (cellState->getPosition() - Cursor::PosF()).length())
			{
				addChildViewer<CellStateCaptureViewer>(cellState);
			}
		}

		// draw
		World::GetInstance()->draw();

		// delete
		if (isMouseover() && MouseR.pressed())
		{
			Circle circle(Cursor::PosF(), 256.0);
			circle.draw(ColorF(Palette::Red, 0.5));

			for (const auto& c : World::GetInstance()->getCellStates())
				if (Circle(c->getPosition(), c->getRadius()).intersects(circle)) c->m_deathTimer = 0.0;

			for (const auto& e : World::GetInstance()->getEggStates())
			{
				if (Circle(e->getPosition(), e->getRadius()).intersects(circle))
				{
					e->destroy();

					// Elementの吐き出し
					World::GetInstance()->getTile(e->getPosition())->addElement(e->getCellAsset()->getMaterial().getElement());

					// ProteinStateの吐き出し
					auto s = e->getCellAsset()->getMaterial();
					for (const auto& m : s.getProteinList())
					{
						for (int i = 0; i < m.second; i++)
						{
							// 吐き出す方向
							auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

							// 吐き出されたProteinState
							const auto& ms = World::GetInstance()->addProteinState(m.first);
							ms->setPosition(e->getPosition() + v * (e->getRadius() + m.first->getRadius()) * Random(1.0));
							ms->setVelocity(v * 0.1);
						}
					}
				}
			}
		}
	}

	// HighSpeed
	if (m_isHighSpeed)
		RectF(getViewerSize()).draw(ColorF(Palette::Red, 0.1)).drawFrame(8.0, 0.0, ColorF(Palette::Red, 0.5));
}

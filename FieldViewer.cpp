#include "FieldViewer.h"

#include "AssetManager.h"
#include "EggManager.h"
#include "ChipManager.h"
#include "CellManager.h"
#include "ElementManager.h"

#include "Rigidbody.h"
#include "CellAsset.h"
#include "CellState.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "EggState.h"
#include "ElementState.h"
#include "ElementAsset.h"

#include "CellAssetViewer.h"
#include "CellMakingViewer.h"
#include "CellStateViewer.h"
#include "CellStateCaptureViewer.h"
#include "StatisticsViewer.h"
#include "CellBookViewer.h"

#include "GUIButton.h"
#include "CurtainViewer.h"

void FieldViewer::openCellMakingViewer()
{
	if (!hasChildViewer<CellMakingViewer>()) addChildViewer<CellMakingViewer>();
}

void FieldViewer::init()
{
	m_audio = Audio(U"assets/music/シアン.mp3");

	m_camera.setRestrictedRect(Rect(g_chipManagerPtr->getChipSize()).scaledAt(Vec2::Zero(), g_chipManagerPtr->getChipLength()));
	m_camera.setMaxScale(4);
	m_camera.setMinScale(0.1);
	m_camera.setCenter(m_camera.getRestrictedRect()->center());
	m_camera.setTargetCenter(m_camera.getRestrictedRect()->center());

	m_audio.setLoop(true);
	m_audio.play();

	addChildViewer<CellStateViewer>();

	addChildViewer<GUIButton>(U"Cell作成", [this]() { openCellMakingViewer(); })->setViewerRectInLocal(100, 50, 200, 50);

	addChildViewer<StatisticsViewer>();

	// OpenCurtain
	addChildViewer<CurtainViewer>(Color(11, 22, 33), Color(0, 0), 0.5);
}

void FieldViewer::update()
{
	static int speed = 1;

	{
		// camera
		if (isMouseover()) m_camera.update();
		auto t = m_camera.createTransformer();

		// speed
		if (KeyF1.down()) speed = 1;
		if (KeyF2.down() && speed != 1) speed /= 2;
		if (KeyF3.down() && speed != 128) speed *= 2;
		if (KeyF4.down()) speed = 128;

		// update
		for (int i = 0; i < speed; ++i)
		{
			g_cellManagerPtr->updateCellStates();
			g_eggManagerPtr->updateEggStates();
			g_chipManagerPtr->updateChips();
			g_elementManagerPtr->updateElementStates();

			getChildViewer<StatisticsViewer>()->takeLog();
		}

		// CellState Capture
		if (isMouseover() && MouseL.down() && !g_cellManagerPtr->getCellStates().isEmpty())
		{
			auto index = g_cellManagerPtr->getCellStateKDTree().knnSearch(1, Cursor::PosF()).front();
			auto cellState = g_cellManagerPtr->getCellStates()[index];
			if (cellState->getRadius() > (cellState->getPosition() - Cursor::PosF()).length())
			{
				addChildViewer<CellStateCaptureViewer>(cellState);

				getChildViewer<CellStateViewer>()->m_cellState = cellState;

				// CellAssetViewerの構築
				//if (auto cv = getChildViewer<CellAssetViewer>()) cv->destroy();
				//addChildViewer<CellAssetViewer>(cellState->getCellAsset());
			}
		}

		// draw
		g_chipManagerPtr->drawChips();
		g_elementManagerPtr->drawElementStates();
		g_eggManagerPtr->drawEggStates();
		g_cellManagerPtr->drawCellStates();

		// delete
		if (isMouseover() && MouseR.pressed())
		{
			Circle circle(Cursor::PosF(), 256.0);
			circle.draw(ColorF(Palette::Red, 0.5));

			for (const auto& c : g_cellManagerPtr->getCellStates())
				if (Circle(c->getPosition(), c->getRadius()).intersects(circle)) c->m_deathTimer = 0.0;

			for (const auto& e : g_eggManagerPtr->getEggStates())
			{
				if (Circle(e->getPosition(), e->getRadius()).intersects(circle))
				{
					e->destroy();

					// Nutritionの吐き出し
					g_chipManagerPtr->addNutrition(e->getPosition(), e->getCellAsset()->getMaterial().getNutrition());

					// ElementStateの吐き出し
					auto s = e->getCellAsset()->getMaterial();
					for (const auto& m : s.getElementList())
					{
						for (int i = 0; i < m.second; i++)
						{
							// 吐き出す方向
							auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

							// 吐き出されたElementState
							const auto& ms = g_elementManagerPtr->addElementState(m.first);
							ms->setPosition(e->getPosition() + v * (e->getRadius() + m.first->getRadius()) * Random(1.0));
							ms->setVelocity(v * 0.1);
						}
					}
				}
			}
		}

		{
			const auto& cs = getChildViewer<CellStateViewer>()->m_cellState;
			if (cs != nullptr)
			{
				Circle(cs->getPosition(), cs->getRadius() * 1.5)
					.draw(ColorF(1.0, 0.25))
					.drawFrame(4.0, Palette::Black);
			}
		}
	}

	{
		static Font font(64, Typeface::Bold);

		font(U"x", speed).draw(Scene::Size().x - 200, 25);
	}
}

#include "FieldViewer.h"

#include "AssetManager.h"
#include "WaveManager.h"
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

#include "CellMakingButton.h"
#include "CellMakingViewer.h"
#include "CellStateViewer.h"
#include "StatisticsViewer.h"

FieldViewer::FieldViewer()
	: m_audio(U"assets/music/シアン.mp3")
	, m_openCurtain(Color(11, 22, 33), Color(0, 0), 0.5, true)
{
	m_camera.setRestrictedRect(g_chipManagerPtr->getRect().scaledAt(Vec2::Zero(), g_chipManagerPtr->getLength()));
	m_camera.setMaxScale(4);
	m_camera.setMinScale(0.1);
	m_camera.setCenter(m_camera.getRestrictedRect()->center());
	m_camera.setTargetCenter(m_camera.getRestrictedRect()->center());

	setViewerRect(Scene::Size());
	m_audio.setLoop(true);
	//m_audio.play();

	m_cellMakingButton = MakeUnique<CellMakingButton>();
	m_cellStateViewer = MakeUnique<CellStateViewer>();
	m_statisticsViewer = MakeUnique<StatisticsViewer>();
	m_releaseViewer = MakeUnique<ReleaseViewer>();
}

void FieldViewer::update()
{
	{
		// camera
		if (isMouseOver()) m_camera.update();
		auto t = m_camera.createTransformer();

		// speed
		static int speed = 1;
		if (KeyF1.down()) speed = 1;
		if (KeyF2.down() && speed != 1) speed /= 2;
		if (KeyF3.down() && speed != 128) speed *= 2;
		if (KeyF4.down()) speed = 128;

		// update
		for (int i = 0; i < speed; ++i)
		{
			g_waveManagerPtr->updateWave();
			g_cellManagerPtr->updateCellStates();
			g_eggManagerPtr->updateEggStates();
			g_chipManagerPtr->updateChips();
			g_elementManagerPtr->updateElementStates();
		}

		// Rigidbody Capture
		{
			static shared_ptr<CellState> selectedRigidbody = nullptr;

			if (MouseL.down())
			{
				for (auto i : g_cellManagerPtr->getCellStateKDTree().knnSearch(1, Cursor::PosF()))
				{
					auto& cellState = g_cellManagerPtr->getCellStates()[i];

					if (cellState->getRadius() > (cellState->getPosition() - Cursor::PosF()).length())
					{
						selectedRigidbody = cellState;
						m_cellStateViewer->m_cellState = dynamic_pointer_cast<CellState>(cellState);
					}
				}
			}

			if (MouseL.pressed() && isMouseOver())
			{
				if (selectedRigidbody != nullptr) selectedRigidbody->setPosition(Vec2(Cursor::PosF().x, Cursor::PosF().y));
			}
			else selectedRigidbody = nullptr;
		}

		// draw
		g_chipManagerPtr->drawChips();
		g_waveManagerPtr->drawWave();
		g_eggManagerPtr->drawEggStates();
		g_cellManagerPtr->drawCellStates();
		g_elementManagerPtr->drawElementStates();

		// delete
		if (MouseR.pressed())
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
			const auto& cs = m_cellStateViewer->m_cellState;
			if (cs != nullptr)
			{
				Circle(cs->getPosition(), cs->getRadius() * 1.5)
					.draw(ColorF(1.0, 0.25))
					.drawFrame(4.0, Palette::Black);
			}
		}
	}

	// Cell Making
	if (m_cellMakingButton && m_cellMakingButton->isSelected()) 
	{
		m_cellMakingButton = nullptr;

		m_cellMakingViewer = MakeUnique<CellMakingViewer>();
	}

	// Open Curtain
	if (m_openCurtain.isRunning() && m_openCurtain.update()) m_audio.setVolume(m_openCurtain.getProgress());
	else m_audio.setVolume(1.0);
}

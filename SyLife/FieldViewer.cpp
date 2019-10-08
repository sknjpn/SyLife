#include "FieldViewer.h"

#include "AssetManager.h"
#include "ViewerManager.h"
#include "WaveManager.h"
#include "EggManager.h"
#include "ChipManager.h"
#include "CellManager.h"

#include "PartPaletteViewer.h"
#include "AssemblyViewer.h"
#include "ReleaseViewer.h"
#include "CellStateViewer.h"

#include "EggState.h"

FieldViewer::FieldViewer()
	: m_audio(U"assets/music/シアン.mp3")
	, m_openCurtain(Color(11, 22, 33), Color(0, 0), 0.5, true)
{
	m_camera.setRestrictedRect(RectF(12800, 12800));
	m_camera.setMaxScale(4);
	m_camera.setMinScale(0.1);
	m_camera.setCenter(Vec2::Zero());
	m_camera.setTargetCenter(Vec2::Zero());

	SetDrawRect(Scene::Size());
	m_audio.setLoop(true);
	m_audio.play();
}

void FieldViewer::Init()
{
	// 付属Viewerの初期化
	m_newModel = g_assetManagerPtr->makeModel<CellAsset>();
	g_viewerManagerPtr->makeViewer<PartPaletteViewer>()->setModel(m_newModel);
	g_viewerManagerPtr->makeViewer<AssemblyViewer>()->setModel(m_newModel);
	g_viewerManagerPtr->makeViewer<ReleaseViewer>()->setModel(m_newModel);
	g_viewerManagerPtr->makeViewer<CellStateViewer>();
}

void FieldViewer::update()
{
	{
		// camera
		if (IsMouseOver()) m_camera.update();
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
			g_terrainManagerPtr->UpdateTerrain();
			g_waveManagerPtr->UpdateWave();
			g_cellManagerPtr->UpdateCellStates();
			g_eggManagerPtr->UpdateEggStates();
			g_chipManagerPtr->updateChips();
		}

		// Rigidbody Capture
		{
			static shared_ptr<CellState> selectedRigidbody = nullptr;

			if (MouseL.down())
			{
				for (auto i : g_cellManagerPtr->GetCellStateKDTree().knnSearch(1, Cursor::PosF()))
				{
					auto& cell = g_cellManagerPtr->GetCellStates()[i];

					if (cell->getRadius() > (cell->getPosition() - Cursor::PosF()).length())
					{
						selectedRigidbody = cell;
						g_viewerManagerPtr->GetViewer<CellStateViewer>()->m_cellState = dynamic_pointer_cast<CellState>(cell);
					}
				}
			}

			if (MouseL.pressed() && IsMouseOver())
			{
				if (selectedRigidbody != nullptr) selectedRigidbody->setPosition(Vec2(Cursor::PosF().x, Cursor::PosF().y));
			}
			else selectedRigidbody = nullptr;
		}

		// draw
		g_terrainManagerPtr->DrawTerrain();
		g_waveManagerPtr->DrawWave();
		g_eggManagerPtr->DrawEggStates();
		g_cellManagerPtr->DrawCellStates();
		g_chipManagerPtr->drawChips();

		// delete
		if (MouseR.pressed())
		{
			Circle circle(Cursor::PosF(), 128.0);
			circle.draw(ColorF(Palette::Red, 0.5));

			for (const auto& c : g_cellManagerPtr->GetCellStates())
				if (Circle(c->getPosition(), c->getRadius()).intersects(circle)) c->m_deathTimer = 0.0;

			for (const auto& e : g_eggManagerPtr->GetEggStates())
			{
				if (Circle(e->getPosition(), e->getRadius()).intersects(circle))
				{
					e->Destroy();

					// MoleculeStateの吐き出し
					/*auto s = e->getCellAsset()->getMaterial();
					for (const auto& m : s.GetMolecules())
					{
						for (unsigned int i = 0; i < m.second; i++)
						{
							// 吐き出す方向
							auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

							// 吐き出されたMoleculeState
							const auto& ms = g_moleculeManagerPtr->AddMoleculeState(m.first);
							ms->setPosition(e->getPosition() + v * (e->getRadius() + m.first->getRadius()) * Random(1.0));
							ms->SetVelocity(v * 0.1);
						}
					}*/
				}
			}
		}

		auto rv = g_viewerManagerPtr->GetViewer<ReleaseViewer>();
		auto ppv = g_viewerManagerPtr->GetViewer<PartPaletteViewer>();
		auto av = g_viewerManagerPtr->GetViewer<AssemblyViewer>();

		if (rv->m_isDragged && IsMouseOver())
		{
			// part
			{
				auto t1 = Transformer2D(Mat3x2::Translate(Cursor::PosF()));

				for (const auto& p : rv->getModel<CellAsset>()->getPartConfigs())
				{
					auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
						.translated(p->getPosition()));

					for (const auto& s : p->getModel()->getShapes())
						s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
				}
			}

			if (MouseL.up())
			{
				const auto& c = g_cellManagerPtr->AddCellState(rv->getModel<CellAsset>());
				c->setPosition(Cursor::PosF());
				c->SetVelocity(Vec2::Zero());
				c->Init();

				m_newModel = g_assetManagerPtr->makeModel<CellAsset>();
				ppv->setModel(m_newModel);
				av->setModel(m_newModel);
				rv->setModel(m_newModel);
			}
		}

		{
			const auto& cs = g_viewerManagerPtr->GetViewer<CellStateViewer>()->m_cellState;

			if (cs != nullptr)
			{
				Circle(cs->getPosition(), cs->getRadius() * 1.5)
					.draw(ColorF(1.0, 0.25))
					.drawFrame(4.0, Palette::Black);
			}
		}
	}

	// Open Curtain
	if (m_openCurtain.IsRunning() && m_openCurtain.update()) m_audio.setVolume(m_openCurtain.GetProgress());
	else m_audio.setVolume(1.0);
}

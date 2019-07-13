#pragma once

#include "Viewer.h"
#include "FieldViewer.h"
#include "Curtain.h"
#include "ReleaseViewer.h"
#include "AssemblyViewer.h"
#include "PartPaletteViewer.h"
#include "CellStateViewer.h"
#include "Egg.h"

class CellModel;
class PartPaletteViewer;
class AssemblyViewer;
class ReleaseViewer;

class FieldViewer
	: public Viewer
{
	Audio	m_audio;
	Camera2D	m_cursorCamera2D;
	shared_ptr<CellModel>	m_newModel;

public:
	FieldViewer()
		: m_audio(U"assets/music/ƒVƒAƒ“.mp3")
	{
		//m_cursorCamera2D.setRestrictedRect(RectF(-4000, -4000, 8000, 8000));
		m_cursorCamera2D.setCenter(Vec2::Zero());
		m_cursorCamera2D.setTargetCenter(Vec2::Zero());
		//m_cursorCamera2D.setMinMagnification(0.01);

		SetDrawRect(Scene::Size());
		m_audio.setLoop(true);
		m_audio.play();
	}

	void	Init() override
	{
		// •t‘®Viewer‚Ì‰Šú‰»
		m_newModel = g_assetManagerPtr->MakeModel<CellModel>();
		g_viewerManagerPtr->MakeViewer<PartPaletteViewer>()->SetModel(m_newModel);
		g_viewerManagerPtr->MakeViewer<AssemblyViewer>()->SetModel(m_newModel);
		g_viewerManagerPtr->MakeViewer<ReleaseViewer>()->SetModel(m_newModel);
		g_viewerManagerPtr->MakeViewer<CellStateViewer>();
	}

	void	Update() override
	{
		{
			// camera
			if (IsMouseOver()) m_cursorCamera2D.update();
			auto t = m_cursorCamera2D.createTransformer();

			// speed
			static int speed = 1;
			if (KeyF1.down()) speed = 1;
			if (KeyF2.down() && speed != 1) speed /= 2;
			if (KeyF3.down() && speed != 128) speed *= 2;
			if (KeyF4.down()) speed = 128;

			// update
			for (int i = 0; i < speed; ++i)
			{
				g_terrainManagerPtr->Update();
				g_moleculeManagerPtr->Update();
				g_fieldManagerPtr->Update();
				g_waveManagerPtr->Update();
				g_cellManagerPtr->Update();
				g_eggManagerPtr->Update();
			}

			// Rigidbody Capture
			{
				static shared_ptr<CellState> selectedRigidbody = nullptr;

				if (MouseL.down())
				{
					for (auto i : g_cellManagerPtr->GetCellStateKDTree().knnSearch(1, Cursor::PosF()))
					{
						auto& cell = g_cellManagerPtr->GetCellStates()[i];

						if (cell->GetRadius() > (cell->GetPosition() - Cursor::PosF()).length())
						{
							selectedRigidbody = cell;
							g_viewerManagerPtr->GetViewer<CellStateViewer>()->m_cellState = dynamic_pointer_cast<CellState>(cell);
						}
					}
				}

				if (MouseL.pressed() && IsMouseOver())
				{
					if (selectedRigidbody != nullptr) selectedRigidbody->SetPosition(Vec2(Cursor::PosF().x, Cursor::PosF().y));
				}
				else selectedRigidbody = nullptr;
			}

			// draw
			g_terrainManagerPtr->Draw();
			g_waveManagerPtr->Draw();
			g_eggManagerPtr->Draw();
			g_moleculeManagerPtr->Draw();
			g_cellManagerPtr->Draw();

			// delete
			if (MouseR.pressed())
			{
				Circle circle(Cursor::PosF(), 128.0);
				circle.draw(ColorF(Palette::Red, 0.5));

				for (const auto& c : g_cellManagerPtr->GetCellStates())
					if (Circle(c->GetPosition(), c->GetRadius()).intersects(circle)) c->m_deathTimer = 0.0;

				for (const auto& e : g_eggManagerPtr->GetEggStates())
				{
					if (Circle(e->GetPosition(), e->GetRadius()).intersects(circle))
					{
						e->Destroy();

						// MoleculeState‚Ì“f‚«o‚µ
						auto s = e->GetCellModel()->GetMaterial();
						for (const auto& m : s.GetMolecules())
						{
							for (unsigned int i = 0; i < m.second; i++)
							{
								// “f‚«o‚·•ûŒü
								auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

								// “f‚«o‚³‚ê‚½MoleculeState
								const auto& ms = g_moleculeManagerPtr->AddMoleculeState(m.first);
								ms->SetPosition(e->GetPosition() + v * (e->GetRadius() + m.first->GetRadius()) * Random(1.0));
								ms->SetVelocity(v * 0.1);
							}
						}
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

					for (const auto& p : rv->GetModel<CellModel>()->GetPartConfigs())
					{
						auto t2 = Transformer2D(Mat3x2::Rotate(p->GetRotation())
							.translated(p->GetPosition()));

						for (const auto& s : p->GetModel()->GetShapes())
							s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
					}
				}

				if (MouseL.up())
				{
					const auto& c = g_cellManagerPtr->AddCellState(rv->GetModel<CellModel>());
					c->SetPosition(Cursor::PosF());
					c->SetVelocity(Vec2::Zero());
					c->Init();

					m_newModel = g_assetManagerPtr->MakeModel<CellModel>();
					ppv->SetModel(m_newModel);
					av->SetModel(m_newModel);
					rv->SetModel(m_newModel);
				}
			}

			{
				const auto& cs = g_viewerManagerPtr->GetViewer<CellStateViewer>()->m_cellState;

				if (cs != nullptr)
				{
					Circle(cs->GetPosition(), cs->GetRadius() * 1.5)
						.draw(ColorF(1.0, 0.25))
						.drawFrame(4.0, Palette::Black);
				}
			}
		}

		static Curtain curtain(Color(11, 22, 33), 0.5);
		curtain.OpenUpdate();
		m_audio.setVolume(Min(curtain.m_st.sF() / curtain.m_duration, 1.0));
	}
};

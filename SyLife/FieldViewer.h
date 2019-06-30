#pragma once

#include "Viewer.h"
#include "TinyCamera.h"
#include "FieldViewer.h"
#include "FieldManager.h"
#include "Curtain.h"
#include "EggManager.h"
#include "CellManager.h"
#include "MoleculeManager.h"
#include "ViewerManager.h"
#include "ReleaseViewer.h"
#include "AssemblyViewer.h"
#include "PartPaletteViewer.h"
#include "WaveManager.h"
#include "CellStateViewer.h"
#include "Egg.h"
#include "Random.h"

class CellModel;
class PartPaletteViewer;
class AssemblyViewer;
class ReleaseViewer;

class FieldViewer
	: public Viewer
{
	s3d::Audio	m_audio;
	RestrictedCamera2D	m_cursorCamera2D;
	shared_ptr<CellModel>	m_newModel;

public:
	FieldViewer()
		: m_audio(U"assets/music/AQUA.mp3")
	{
		m_cursorCamera2D.setRestrictedRect(s3d::RectF(-4000, -4000, 8000, 8000));
		m_cursorCamera2D.setCenter(s3d::Vec2::Zero());
		m_cursorCamera2D.setTargetCenter(s3d::Vec2::Zero());
		m_cursorCamera2D.setMinMagnification(0.01);

		m_drawRect = s3d::RectF(s3d::Window::Size());
		m_audio.setLoop(true);
		m_audio.play();
	}

	void	Init() override
	{
		m_newModel = g_assetManagerPtr->MakeModel<CellModel>();

		g_viewerManagerPtr->AddViewer<PartPaletteViewer>(m_newModel);
		g_viewerManagerPtr->AddViewer<AssemblyViewer>(m_newModel);
		g_viewerManagerPtr->AddViewer<ReleaseViewer>(m_newModel);
		g_viewerManagerPtr->AddViewer<CellStateViewer>();
	}

	void	Update(bool isMouseOver) override
	{
		if (isMouseOver) m_cursorCamera2D.update();
		auto t = m_cursorCamera2D.createTransformer();

		static int speed = 1;
		if (s3d::KeyF1.down()) speed = 1;
		if (s3d::KeyF2.down() && speed != 1) speed /= 2;
		if (s3d::KeyF3.down() && speed != 128) speed *= 2;
		if (s3d::KeyF4.down()) speed = 128;

		for (int i = 0; i < speed; ++i)
		{
			g_moleculeManagerPtr->Update();
			g_fieldManagerPtr->Update();
			g_waveManagerPtr->Update();
			g_cellManagerPtr->Update();
			g_eggManagerPtr->Update();
		}

		// Rigidbody Capture
		{
			static shared_ptr<Rigidbody> selectedRigidbody = nullptr;

			if (s3d::MouseL.down())
			{
				Vector2D cursorPos(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y);

				for (auto target : g_fieldManagerPtr->GetIndexer().GetNearParticles(cursorPos, 100))
				{
					if (target->GetRadius() > (target->GetPosition() - cursorPos).length())
					{
						selectedRigidbody = target;
						g_viewerManagerPtr->GetViewer<CellStateViewer>()->m_cellState = dynamic_pointer_cast<CellState>(target);
					}
				}
			}

			if (s3d::MouseL.pressed() && isMouseOver)
			{
				if (selectedRigidbody != nullptr) selectedRigidbody->SetPosition(Vector2D(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y));
			}
			else selectedRigidbody = nullptr;
		}

		g_eggManagerPtr->Draw();
		g_moleculeManagerPtr->Draw();
		g_cellManagerPtr->Draw();
		g_waveManagerPtr->Draw();

		if (s3d::MouseR.pressed())
		{
			s3d::Circle circle(s3d::Cursor::PosF(), 128.0);
			circle.draw(s3d::ColorF(s3d::Palette::Red, 0.5));

			for (const auto& c : g_cellManagerPtr->GetCellStates())
				if (s3d::Circle(c->GetPosition().m_x, c->GetPosition().m_y, c->GetRadius()).intersects(circle)) c->m_deathTimer = 0.0;

			for (const auto& e : g_eggManagerPtr->GetEggStates())
			{
				if (s3d::Circle(e->GetPosition().m_x, e->GetPosition().m_y, e->GetRadius()).intersects(circle))
				{
					e->Destroy();

					// MoleculeState‚Ì“f‚«o‚µ
					auto s = e->GetCellModel()->m_material;
					for (const auto& m : s.GetMolecules())
					{
						for (unsigned int i = 0; i < m.second; i++)
						{
							// “f‚«o‚·•ûŒü
							auto v = Vector2D(1.0, 0.0).rotated(rand() / 3600.0);

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

		if (rv->m_isDragged && isMouseOver)
		{
			// part
			{
				auto t1 = s3d::Transformer2D(s3d::Mat3x2::Translate(s3d::Cursor::PosF()));

				for (const auto& p : rv->m_model->m_partConfigs)
				{
					auto t2 = s3d::Transformer2D(s3d::Mat3x2::Rotate(p->GetRotation())
						.translated(p->GetPosition().m_x, p->GetPosition().m_y));

					for (const auto& s : p->GetModel()->GetShapes())
						s.m_polygon.draw(s3d::ColorF(s.m_color, 0.5)).drawFrame(1.0, s3d::Palette::Black);
				}
			}

			if (s3d::MouseL.up())
			{
				const auto& c = g_cellManagerPtr->AddCellState(rv->m_model);
				c->SetPosition(Vector2D(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y));
				c->SetVelocity(Vector2D::Zero());
				c->Init();

				m_newModel = g_assetManagerPtr->MakeModel<CellModel>();
				ppv->m_model = m_newModel;
				av->m_model = m_newModel;
				rv->m_model = m_newModel;
			}
		}

		{
			const auto& cs = g_viewerManagerPtr->GetViewer<CellStateViewer>()->m_cellState;

			if (cs != nullptr)
			{
				s3d::Circle(cs->GetPosition().m_x, cs->GetPosition().m_y, cs->GetRadius() * 1.5)
					.draw(s3d::ColorF(1.0, 0.25))
					.drawFrame(4.0, s3d::Palette::Black);
			}
		}

		static Curtain curtain(s3d::Color(11, 22, 33), 0.5);
		curtain.OpenUpdate();
		m_audio.setVolume(s3d::Min(curtain.m_st.sF() / curtain.m_duration, 1.0));
	}
};

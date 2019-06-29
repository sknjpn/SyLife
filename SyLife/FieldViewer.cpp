#include "FieldViewer.h"
#include "FieldManager.h"
#include "Curtain.h"
#include "CellManager.h"
#include "MoleculeManager.h"
#include "ViewerManager.h"
#include "ReleaseViewer.h"
#include "AssemblyViewer.h"
#include "PartPaletteViewer.h"

void FieldViewer::Init()
{
	m_newModel = make_shared<CellModel>();

	m_partPaletteViewer = g_viewerManagerPtr->AddViewer<PartPaletteViewer>(m_newModel);
	m_assemblyViewer = g_viewerManagerPtr->AddViewer<AssemblyViewer>(m_newModel);
	m_releaseViewer = g_viewerManagerPtr->AddViewer<ReleaseViewer>(m_newModel);
}

void FieldViewer::Update(bool isMouseOver)
{
	if (isMouseOver) m_cursorCamera2D.update();
	auto t = m_cursorCamera2D.createTransformer();

	auto f = s3d::KeySpace.pressed() ? 5 : 1;

	for (int i = 0; i < f; ++i)
		g_fieldManagerPtr->Update();

	// Rigidbody Capture
	{
		static shared_ptr<Rigidbody> selectedRigidbody = nullptr;

		if (s3d::MouseL.down())
		{
			Vector2D cursorPos(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y);

			for (auto target : g_fieldManagerPtr->GetIndexer().GetNearParticles(cursorPos, 100))
			{
				if (target->m_radius > (target->m_position - cursorPos).length()) selectedRigidbody = target;
			}
		}

		if (s3d::MouseL.pressed() && isMouseOver)
		{
			if (selectedRigidbody != nullptr) selectedRigidbody->m_position = Vector2D(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y);
		}
		else selectedRigidbody = nullptr;
	}

	g_moleculeManagerPtr->Draw();

	g_cellManagerPtr->Draw();

	if (s3d::MouseR.pressed())
	{
		s3d::Circle circle(s3d::Cursor::PosF(), 128.0);
		circle.draw(s3d::ColorF(s3d::Palette::Red, 0.5));

		for (const auto& c : g_cellManagerPtr->GetCellStates())
		{
			if (s3d::Circle(c->m_position.m_x, c->m_position.m_y, c->m_radius).intersects(circle)) c->m_deathTimer = 0.0;
		}
	}

	if (m_releaseViewer->m_isDragged && isMouseOver)
	{
		// part
		{
			auto t1 = s3d::Transformer2D(s3d::Mat3x2::Translate(s3d::Cursor::PosF()));

			for (const auto& p : m_releaseViewer->m_model->m_partConfigs)
			{
				auto t2 = s3d::Transformer2D(s3d::Mat3x2::Rotate(p->m_rotation).translated(p->m_position.m_x, p->m_position.m_y));

				for (const auto& s : p->m_model->GetShapes())
					s.m_polygon.draw(s3d::ColorF(s.m_color, 0.5)).drawFrame(1.0, s3d::Palette::Black);
			}
		}

		if (s3d::MouseL.up())
		{
			const auto& c = g_cellManagerPtr->AddCellState(m_releaseViewer->m_model);
			c->m_molecules.AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Amino Acid"), 5);
			c->m_molecules.AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Carbon"), 5);
			c->m_molecules.AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Oxygen"), 5);
			c->m_position.m_x = s3d::Cursor::PosF().x;
			c->m_position.m_y = s3d::Cursor::PosF().y;
			c->m_velocity = Vector2D::Zero();
			c->Init();

			m_newModel = make_shared<CellModel>();
			m_partPaletteViewer->m_model = m_newModel;
			m_assemblyViewer->m_model = m_newModel;
			m_releaseViewer->m_model = m_newModel;
		}
	}

	static Curtain curtain(s3d::Color(11, 22, 33), 0.5);
	curtain.OpenUpdate();
	m_audio.setVolume(s3d::Min(curtain.m_st.sF() / curtain.m_duration, 1.0));
}

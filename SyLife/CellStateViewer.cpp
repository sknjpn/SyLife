#include "CellStateViewer.h"
#include "AssetManager.h"
#include "Cell.h"

void CellStateViewer::Update(bool isMouseOver)
{
	s3d::Rect(m_drawRect.size.asPoint()).draw(s3d::Color(11, 22, 33, 192));

	if (!m_cellState) return;
	if (m_cellState->m_isDestroyed)
	{
		m_cellState = nullptr;

		return;
	}

	const auto& model = m_cellState->m_model;

	{
		auto t1 = s3d::Transformer2D(s3d::Mat3x2::Translate(100, 50));
		auto t2 = s3d::Transformer2D(s3d::Mat3x2::Scale(50.0 / m_cellState->m_model->m_radius));

		m_cellState->m_model->Draw(1.0);
	}

	s3d::Vec2 pos(0, 100);
	static s3d::Font font(13, s3d::Typeface::Bold);

	{
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos));
			font(U"Material").draw();
			pos.moveBy(0, 16);
		}

		pos.moveBy(16.0, 0.0);
		for (const auto& m : model->m_material.GetMolecules())
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos));
			font(s3d::Unicode::Widen(m.first->GetName()) + U":" + s3d::ToString(m.second) + U"U").draw();
			pos.moveBy(0, 15);
		}
		pos.moveBy(0, 32);
	}

	{
		pos.moveBy(-16.0, 0.0);
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos));
			font(U"Storage").draw();
			pos.moveBy(0, 16);
		}

		pos.moveBy(16.0, 0.0);
		for (const auto& m : m_cellState->m_storage.GetMolecules())
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos));
			font(s3d::Unicode::Widen(m.first->GetName()) + U":" + s3d::ToString(m.second) + U"U").draw();
			pos.moveBy(0, 15);
		}
		pos.moveBy(-16.0, 0.0);
	}
}

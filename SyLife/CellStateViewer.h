#pragma once

#include "Viewer.h"
#include "Cell.h"

class CellStateViewer
	: public Viewer
{
public:
	shared_ptr<CellState>	m_cellState;

public:
	CellStateViewer()
		: m_cellState(nullptr)
	{
		SetDrawRect(Window::Width() - 300, 450, 200, 400);

	}

	void	Update() override
	{
		Rect(GetDrawRect().size.asPoint()).draw(Color(11, 22, 33, 192));

		if (!m_cellState) return;
		if (m_cellState->IsDestroyed())
		{
			m_cellState = nullptr;

			return;
		}

		const auto& model = m_cellState->m_model;

		{
			auto t1 = Transformer2D(Mat3x2::Translate(100, 50));
			auto t2 = Transformer2D(Mat3x2::Scale(50.0 / m_cellState->m_model->GetRadius()));

			m_cellState->m_model->Draw(1.0);
		}

		Vec2 pos(0, 100);
		static Font font(13, Typeface::Bold);

		{
			{
				auto t = Transformer2D(Mat3x2::Translate(pos));
				font(U"Material").draw();
				pos.moveBy(0, 16);
			}

			pos.moveBy(16.0, 0.0);
			for (const auto& m : model->GetMaterial().GetMolecules())
			{
				auto t = Transformer2D(Mat3x2::Translate(pos));
				font(Unicode::Widen(m.first->GetName()) + U":" + ToString(m.second) + U"U").draw();
				pos.moveBy(0, 15);
			}
			pos.moveBy(0, 32);
		}

		{
			pos.moveBy(-16.0, 0.0);
			{
				auto t = Transformer2D(Mat3x2::Translate(pos));
				font(U"Storage").draw();
				pos.moveBy(0, 16);
			}

			pos.moveBy(16.0, 0.0);
			for (const auto& m : m_cellState->m_storage.GetMolecules())
			{
				auto t = Transformer2D(Mat3x2::Translate(pos));
				font(Unicode::Widen(m.first->GetName()) + U":" + ToString(m.second) + U"U").draw();
				pos.moveBy(0, 15);
			}
			pos.moveBy(-16.0, 0.0);
		}
	}

};


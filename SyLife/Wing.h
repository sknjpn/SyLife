#pragma once

#include "Equipment.h"
#include "Cell.h"

class WingModel
	: public EquipmentModel
{
public:
	bool	m_isRight = false;

public:
	shared_ptr<PartState>	MakeState() override;


	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class WingState
	: public EquipmentState
{
	int		m_timer = 0;
	double	m_v = 0.0;
	double	m_p = 0.0;
	int		m_counter = 0;

public:
	void	Draw(const CellState& cell) const
	{
		auto t = Transformer2D(Mat3x2::Rotate(dynamic_pointer_cast<WingModel>(GetPartConfig()->GetModel())->m_isRight ? m_p : -m_p));

		GetPartConfig()->GetModel()->Draw();
	}
	void	Update(CellState& cell) override
	{
		if ((m_timer++) == 60)
		{
			m_timer = 0;

			if (m_counter > 0) --m_counter;
			else
			{
				Flap(cell);
				m_v = 0.1;
			}
		}

		if ((rand() % 1000) == 0) m_counter = 3;

		m_p += m_v;
		m_p *= 0.95;
		m_v *= 0.95;
	}

	void	Flap(CellState& cell);
};

inline shared_ptr<PartState>	WingModel::MakeState() { return make_shared<WingState>(); }

inline void WingModel::SetFromJSON(const ptree& pt)
{
	m_isRight = pt.get<bool>("isRight");

	EquipmentModel::SetFromJSON(pt);
}

inline void WingState::Flap(CellState& cell)
{
	cell.AddImpulseInLocal(Vec2::Up()*10000.0, GetPartConfig()->GetPosition());
}

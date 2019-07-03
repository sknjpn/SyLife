#pragma once

#include "Equipment.h"
#include "Cell.h"

class WingModel
	: public EquipmentModel
{
public:
	bool	m_isRight = false;

public:
	void MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;


	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		pt.put("isRight", m_isRight);

		EquipmentModel::Save_this(pt);

		pt.put("type", "WingModel");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
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

class WingViewer
	: public EquipmentViewer
{
public:
	WingViewer()
	{}
};

inline void		WingModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<WingViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}
inline shared_ptr<PartState>	WingModel::MakeState() { return make_shared<WingState>(); }

inline void WingModel::Load_this(const ptree& pt)
{
	m_isRight = pt.get<bool>("isRight");

	EquipmentModel::Load_this(pt);
}

inline void WingState::Flap(CellState& cell)
{
	cell.AddImpulseInLocal(Vec2::Up()*10000.0, GetPartConfig()->GetPosition());
}

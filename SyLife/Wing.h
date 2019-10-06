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


	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		pt.put("isRight", m_isRight);

		EquipmentModel::save_this(pt);

		pt.put("type", "WingModel");
	}
	void	save(ptree& pt) const override { save_this(pt); }
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
	// Reload
	void reloadProperties_this()
	{
		EquipmentViewer::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		EquipmentViewer::Update_this();
	}
	void Update() override { Update_this(); }
};

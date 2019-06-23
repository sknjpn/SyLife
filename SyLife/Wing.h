#pragma once

#include "Equipment.h"

class WingModel
	: public EquipmentModel
{
public:
	bool	m_isRight = false;

public:
	shared_ptr<PartConfig>	MakeConfig() override;
	shared_ptr<PartState>	MakeState() override;


	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class WingConfig
	: public EquipmentConfig
{
public:

};

class WingState
	: public EquipmentState
{
	int		m_timer = 0;
	double	m_v = 0.0;
	double	m_p = 0.0;
	int		m_counter = 0;

public:
	void	Draw(const Cell& cell) const
	{
		auto t = s3d::Transformer2D(s3d::Mat3x2::Rotate(dynamic_pointer_cast<WingModel>(m_config->m_model)->m_isRight ? m_p : -m_p));

		m_config->m_model->Draw();
	}
	void	Update(Cell& cell) override
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

	void	Flap(Cell& cell);
};

inline shared_ptr<PartConfig>	WingModel::MakeConfig() { return make_shared<WingConfig>(); }
inline shared_ptr<PartState>	WingModel::MakeState() { return make_shared<WingState>(); }

inline void WingModel::SetFromJSON(const ptree & pt)
{
	m_isRight = pt.get<bool>("isRight");

	EquipmentModel::SetFromJSON(pt);
}
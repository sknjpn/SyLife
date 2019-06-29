#pragma once

#include "Equipment.h"
#include "FieldManager.h"
#include "Cell.h"

class NeedleModel
	: public EquipmentModel
{
public:
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class NeedleState
	: public EquipmentState
{
	double	m_heat = 0.0;

public:
	void	Draw(const CellState& cell) const
	{
		auto t = s3d::Transformer2D(s3d::Mat3x2::Scale(1.0, max(m_heat - 4.0, 0.0) * 1.0 + 1.0));

		m_config->m_model->Draw(max(m_heat - 4.0, 0.0) * 0.9 + 0.1);
	}
	void	Update(CellState& cell) override
	{
		m_heat -= g_fieldManagerPtr->GetDeltaTime();
		if (m_heat < 0)
		{
			m_heat = 5.0;

			auto p = cell.GetWorldPosition(m_config->m_position + Vector2D::Up() * 50.0);
			for (auto target : g_fieldManagerPtr->GetIndexer().GetNearParticles(p, 100.0))
			{
				if (target->m_radius > (target->m_position - p).length() && !target->m_isDestroyed && dynamic_pointer_cast<CellState>(target)->m_model != cell.m_model)
				{
					auto cs = dynamic_pointer_cast<CellState>(target);

					cs->m_isDestroyed = true;
					cell.m_storage += cs->m_storage;
					cell.m_storage += cs->m_model->m_material;
				}
			}
		}
	}
};

inline shared_ptr<PartState>	NeedleModel::MakeState() { return make_shared<NeedleState>(); }

inline void NeedleModel::SetFromJSON(const ptree& pt)
{
	EquipmentModel::SetFromJSON(pt);
}

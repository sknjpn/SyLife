#pragma once

#include "Equipment.h"
#include "Cell.h"

class NeedleModel
	: public EquipmentModel
{
public:
	void	MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const
	{
		EquipmentModel::AddToJSON(pt);

		// type
		pt.put("type", "NeedleModel");
	}
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

class NeedleState
	: public EquipmentState
{
	double	m_heat = 0.0;

public:
	void	Draw(const CellState& cell) const
	{
		auto t = Transformer2D(Mat3x2::Scale(1.0, max(m_heat - 4.0, 0.0) * 1.0 + 1.0));

		GetPartConfig()->GetModel()->Draw(max(m_heat - 4.0, 0.0) * 0.9 + 0.1);
	}
	void	Update(CellState& cell) override
	{
		m_heat -= g_fieldManagerPtr->GetDeltaTime();
		if (m_heat < 0)
		{
			m_heat = 5.0;

			auto p = cell.GetWorldPosition(GetPartConfig()->GetPosition() + Vec2::Up() * 50.0);
			for (auto target : g_fieldManagerPtr->GetIndexer().GetNearParticles(p, 100.0))
			{
				if (target->GetRadius() > (target->GetPosition() - p).length() && !target->IsDestroyed() && dynamic_pointer_cast<CellState>(target)->m_model != cell.m_model)
				{
					auto cs = dynamic_pointer_cast<CellState>(target);

					cs->Destroy();
					cell.m_storage += cs->m_storage;
					cell.m_storage += cs->m_model->m_material;
				}
			}
		}
	}
};

class NeedleViewer
	: public PartViewer
{
public:
	NeedleViewer()
	{
		SetDrawRect(0, 450, 600, 150);
	}
};

inline void		NeedleModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<NeedleViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}
inline shared_ptr<PartState>	NeedleModel::MakeState() { return make_shared<NeedleState>(); }

inline void NeedleModel::SetFromJSON(const ptree& pt)
{
	EquipmentModel::SetFromJSON(pt);
}

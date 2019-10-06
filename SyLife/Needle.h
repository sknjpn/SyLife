#pragma once

#include "Equipment.h"
#include "Cell.h"

class NeedleModel
	: public EquipmentModel
{
public:
	void	MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		EquipmentModel::Save_this(pt);

		// type
		pt.put("type", "NeedleModel");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
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
		m_heat -= g_systemManagerPtr->GetDeltaTime();
		if (m_heat < 0)
		{
			m_heat = 5.0;

			auto p = cell.GetWorldPosition(GetPartConfig()->GetPosition() + Vec2::Up() * 50.0);


			for (auto i : g_cellManagerPtr->GetCellStateKDTree().knnSearch(1, p))
			{
				auto& t = g_cellManagerPtr->GetCellStates()[i];

				if (!t->IsDestroyed() && t->GetRadius() > (t->GetPosition() - p).length() && t->m_model != cell.m_model)
				{
					t->Destroy();
					cell.m_storage += t->m_storage;
					cell.m_storage += t->m_model->GetMaterial();
				}
			}
		}
	}
};

class NeedleViewer
	: public EquipmentViewer
{
public:
	// Reload
	void ReloadProperties_this()
	{
		EquipmentViewer::ReloadProperties_this();
	}
	void ReloadProperties() override { ReloadProperties_this(); }

	// Update
	void Update_this()
	{
		EquipmentViewer::Update_this();
	}
	void Update() override { Update_this(); }
};

inline void		NeedleModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<NeedleViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}
inline shared_ptr<PartState>	NeedleModel::MakeState() { return make_shared<NeedleState>(); }

inline void NeedleModel::load_this(const ptree& pt)
{
	EquipmentModel::load_this(pt);
}

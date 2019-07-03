#pragma once

#include "Equipment.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class WingViewer;

class WingModel
	: public EquipmentModel
{
	bool	m_isRight;

public:
	void	MakeViewers() override;

	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const;
	void	Save(ptree& pt) const override { Save_this(pt); }
};

class WingViewer
	: public EquipmentViewer
{
public:
	WingViewer(const shared_ptr<PartModel>& model)
		: EquipmentViewer(model)
	{}
};

inline void WingModel::MakeViewers()
{
	g_viewerManagerPtr->MakeViewer<WingViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void WingModel::Load_this(const ptree& pt)
{
	m_isRight= pt.get<bool>("isRight");

	EquipmentModel::Load_this(pt);
}

inline void WingModel::Save_this(ptree& pt) const
{
	pt.put("isRight", m_isRight);

	EquipmentModel::Save_this(pt);

	pt.put("type", "WingModel");
}
#include "PartAsset.h"

#include "AssetManager.h"
#include "ViewerManager.h"

#include "PartState.h"
#include "PartEditor.h"
#include "PartShapeViewer.h"

void PartAsset::makeViewer()
{
	//g_viewerManagerPtr->makeViewer<PartEditor>()->setPartAsset(shared_from_this());
	//g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> PartAsset::makeState()
{
	return make_shared<PartState>();
}

void PartAsset::load(const ptree& pt)
{
	Asset::load(pt);

	// mass
	m_mass = pt.get<double>("mass");

	// layers
	m_shape.load(pt);

	// material
	m_material.load(pt.get_child("material"));
}

void PartAsset::save(ptree& pt) const
{
	Asset::save(pt);

	// mass
	pt.put<double>("mass", m_mass);

	// layers
	m_shape.save(pt);

	// material
	{
		ptree material;

		m_material.save(material);

		pt.add_child("material", material);
	}

	pt.put("type", "PartAsset");
}

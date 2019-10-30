#include "NucleusAsset.h"

#include "NucleusState.h"
#include "NucleusEditor.h"
#include "PartShapeViewer.h"

void NucleusAsset::makeViewer()
{
	//g_viewerManagerPtr->makeViewer<NucleusEditor>()->setPartAsset(shared_from_this());
	//g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> NucleusAsset::makeState()
{
	return make_shared<NucleusState>();
}

void NucleusAsset::load(const ptree& pt)
{
	PartAsset::load(pt);

	m_lifespanTime = pt.get<double>("lifespanTime");
	m_bornTime = pt.get<double>("bornTime");
	m_yieldTime = pt.get<double>("yieldTime");
}

void NucleusAsset::save(ptree& pt) const
{
	PartAsset::save(pt);

	pt.put("lifespanTime", m_lifespanTime);
	pt.put("bornTime", m_bornTime);
	pt.put("yieldTime", m_yieldTime);

	// type
	pt.put("type", "NucleusAsset");
}

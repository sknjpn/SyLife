#include "PartAsset.h"

#include "AssetManager.h"


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

void PartAsset::load(const JSONValue& json)
{
	Asset::load(json);

	// mass
	m_mass = json[U"mass"].get<double>();

	// layers
	m_shape.load(json);

	// material
	m_material.load(json[U"material"]);
}

void PartAsset::save(JSONWriter& json) const
{
	Asset::save(json);

	// mass
	json.key(U"mass").write(m_mass);

	// layers
	m_shape.save(json);

	// material
	{
		json.key(U"material").startObject();

		m_material.save(json);

		json.endObject();
	}

	json.key(U"type").write(U"PartAsset");
}

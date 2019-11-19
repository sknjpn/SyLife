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

void PartAsset::save(const JSONWriter& json) const
{
	Asset::save(json);

	// mass
	//pt.put<double>(U"mass", m_mass);

	// layers
	//m_shape.save(json);

	// material
	/*{
		ptree material;

		m_material.save(material);

		pt.add_child(U"material", material);
	}*/

	//pt.put(U"type", "PartAsset");
}

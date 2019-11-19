#include "ModuleAsset.h"



#include "ModuleState.h"
#include "ModuleEditor.h"
#include "PartShapeViewer.h"

void ModuleAsset::makeViewer()
{
	//g_viewerManagerPtr->makeViewer<ModuleEditor>()->setPartAsset(shared_from_this());
	//g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> ModuleAsset::makeState()
{
	return make_shared<ModuleState>();
}

void ModuleAsset::load(const JSONValue& json)
{
	PartAsset::load(json);
}

void ModuleAsset::save(JSONWriter& json) const
{
	PartAsset::save(json);

	// type
	json.key(U"type").write(U"ModuleAsset");
}

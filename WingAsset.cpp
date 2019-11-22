#include "WingAsset.h"
#include "WingState.h"
#include "PartShapeViewer.h"

shared_ptr<PartState> WingAsset::makeState()
{
	return make_shared<WingState>();
}

void WingAsset::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_isRight = json[U"isRight"].get<bool>();
}

void WingAsset::save(JSONWriter& json) const
{
	PartAsset::save(json);

	// is right
	json.key(U"isRight").write(m_isRight);

	// type
	json.key(U"type").write(U"WingAsset");
}

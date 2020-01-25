#include "World.h"

#include "CellAsset.h"
#include "ProteinAsset.h"
#include "PartAsset_Body.h"
#include "PartAsset_Synthesizer.h"
#include "PartAsset_Wing.h"
#include "PartAsset_Needle.h"
#include "PartAsset_Nucleus.h"
#include "PartAsset_Eye.h"

std::shared_ptr<Asset> World::makeAsset(const String& type)
{
	if (type == U"CellAsset")			return makeAsset<CellAsset>();

	if (type == U"ProteinAsset")		return makeAsset<ProteinAsset>();

	if (type == U"PartAsset_Body")			return makeAsset<PartAsset_Body>();
	if (type == U"PartAsset_Synthesizer")	return makeAsset<PartAsset_Synthesizer>();
	if (type == U"PartAsset_Wing")			return makeAsset<PartAsset_Wing>();
	if (type == U"PartAsset_Needle")		return makeAsset<PartAsset_Needle>();
	if (type == U"PartAsset_Nucleus")		return makeAsset<PartAsset_Nucleus>();
	if (type == U"PartAsset_Eye")			return makeAsset<PartAsset_Eye>();

	throw Error(U"存在しないタイプのAssetを生成しようとしました\r Type:" + type);
}

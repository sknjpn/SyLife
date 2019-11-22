#include "AssetManager.h"

#include "CellAsset.h"
#include "ElementAsset.h"
#include "BodyAsset.h"
#include "SynthesizerAsset.h"
#include "WingAsset.h"
#include "NeedleAsset.h"
#include "NucleusAsset.h"

shared_ptr<Asset> AssetManager::makeAsset(const String& type)
{
	if (type == U"CellAsset")			return makeAsset<CellAsset>();
	
	if (type == U"ElementAsset")		return makeAsset<ElementAsset>();

	if (type == U"BodyAsset")			return makeAsset<BodyAsset>();
	if (type == U"SynthesizerAsset")	return makeAsset<SynthesizerAsset>();
	if (type == U"WingAsset")			return makeAsset<WingAsset>();
	if (type == U"NeedleAsset")			return makeAsset<NeedleAsset>();
	if (type == U"NucleusAsset")		return makeAsset<NucleusAsset>();

	throw Error(U"存在しないタイプのAssetを生成しようとしました\r Type:" + type);
}

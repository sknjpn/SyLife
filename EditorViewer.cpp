#include "EditorViewer.h"
#include "World.h"
#include "PartAsset.h"
#include "ProteinAsset.h"

void EditorViewer::init()
{
	const auto assetList = addChildViewer<AssetList>();

	for (const auto& partAsset : World::GetAssets<PartAsset>())
		assetList->addAsset(partAsset);

	for (const auto& proteinAsset : World::GetAssets<ProteinAsset>())
		assetList->addAsset(proteinAsset);
}

void EditorViewer::update()
{
}

void EditorViewer::openAsset(const std::shared_ptr<Asset>& asset)
{

}

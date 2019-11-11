#include "Asset.h"
#include "AssetManager.h"

#include "CellAsset.h"

unique_ptr<AssetManager>	g_assetManagerPtr;

void AssetManager::init()
{
	const auto jsonFiles = FileSystem::DirectoryContents(U"assets/models", true)
		.removed_if([](const auto& dc) { return FileSystem::IsDirectory(dc) || FileSystem::Extension(dc) != U"json"; });

	for (const auto& jsonFile : jsonFiles)
	{
		JSONReader json(jsonFile);

		auto a = makeAsset(json[U"type"].getString());
		a->setName(json[U"name"].getString());
		a->setFilePath(jsonFile);
	}

	for (const auto& m : m_assets)
	{
		Logger << m->getName() + U"を読み込み中";

		JSONReader json(m->getFilePath());
		m->load(json);

		Logger << m->getName() + U"を読み込み成功";
	}

	// CellAssetの初期化
	auto cellAssets = getAssets<CellAsset>();
	for (const auto& ca : cellAssets) ca->updateProperties();
}

shared_ptr<Asset> AssetManager::getAsset(const String& name) const
{
	for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
		if ((*it)->getName() == name) return dynamic_pointer_cast<Asset>(*it);

	throw Error(U"存在しない名前のモデルを参照しました");
}

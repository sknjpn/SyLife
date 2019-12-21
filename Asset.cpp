#include "Asset.h"
#include "World.h"
#include "Assets.h"

Asset::Asset()
{
	// 一意な名前とパスの作成
	for (int i = 0;; ++i)
	{
		const String name = Format(U"Asset ", i);
		const FilePath filepath = Format(U"asset_", i, U".json");

		if (!World::GetInstance()->getAssets().hasAsset(name) &&
			!FileSystem::Exists(World::GetInstance()->getAssetsSaveDirectory() + filepath))
		{
			m_name = name;
			m_filepath = filepath;

			break;
		}
	}
}

void Asset::load(const JSONValue& json)
{
	Model::load(json);

	m_name = json[U"name"].getString();
}

void Asset::save(JSONWriter& json) const
{
	Model::save(json);

	// name
	json.key(U"name").write(m_name);
}

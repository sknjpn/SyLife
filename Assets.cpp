#include "Assets.h"
#include "Asset.h"

#include "CellAsset.h"

shared_ptr<Asset> Assets::getAsset(const String& name) const
{
	for (auto it = begin(); it != end(); ++it)
		if ((*it)->getName() == name) return dynamic_pointer_cast<Asset>(*it);

	throw Error(U"存在しない名前のモデルを参照しました name:" + name);
}

bool Assets::hasAsset(const String& name) const
{
	for (auto it = begin(); it != end(); ++it)
		if ((*it)->getName() == name) return true;

	return false;
}

void Assets::save(const FilePath& directory)
{
	for (const auto& asset : *this)
	{
		JSONWriter json;
		json.startObject();

		json.key(U"type").writeString(asset->getTypeName());

		asset->save(json);

		json.endObject();

		json.save(directory + asset->getName() + U".json");
	}
}

void Assets::load(const FilePath& directory)
{
	// JSONのパスを取得
	const auto jsonFiles = FileSystem::DirectoryContents(directory, true)
		.removed_if([](const auto& dc) { return FileSystem::IsDirectory(dc) || FileSystem::Extension(dc) != U"json"; });

	// 名前の読み込み(リンクがあるため、Loadの前に名前の登録を行う)
	for (const auto& jsonFile : jsonFiles)
	{
		JSONReader json(jsonFile);

		auto a = makeAsset(json[U"type"].getString());
		a->setName(json[U"name"].getString());
		a->setFilePath(FileSystem::RelativePath(jsonFile, directory));
	}

	// 読み込み
	for (const auto& m : *this)
	{
		Logger << m->getName() + U"を読み込み中";

		JSONReader json(directory + m->getFilePath());
		m->load(json);

		Logger << m->getName() + U"を読み込み成功";
	}

	// CellAssetの初期化
	{
		const auto cellAssets = getAssets<CellAsset>();

		for (const auto& ca : cellAssets) ca->updateProperties();
	}
}

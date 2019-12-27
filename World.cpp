#include "World.h"
#include "Asset.h"
#include "CellAsset.h"

unique_ptr<World>	World::g_instance;

void World::Make()
{
	g_instance = MakeUnique<World>();

	// FilePathの設定
	g_instance->m_filePath = U"world/";

	g_instance->make();

	g_instance->getField().init();

	// Field情報の保存
	g_instance->save();
}

void World::Load(const FilePath& filepath)
{
	g_instance = MakeUnique<World>();

	g_instance->m_filePath = filepath;

	g_instance->load();
}

void World::update()
{
	m_field.update();
}

void World::save()
{
	Logger << U"WORLD SAVE : " << m_filePath;

	// World
	{
		JSONWriter json;

		json.startObject();

		json.key(U"name").write(m_name);

		json.endObject();

		json.save(m_filePath + U"world.json");
	}

	// Assets
	{
		for (const auto& asset : m_assets)
		{
			JSONWriter json;
			json.startObject();

			json.key(U"type").writeString(asset->getTypeName());

			asset->save(json);

			json.endObject();

			json.save(m_filePath + U"assets/" + asset->getName() + U".json");
		}
	}

	// Field
	{
		m_field.save(m_filePath + U"field/");
	}
}

void World::loadAssets()
{
	// JSONのパスを取得
	const auto jsonFiles = FileSystem::DirectoryContents(m_filePath + U"assets/", true)
		.removed_if([](const auto& dc) { return FileSystem::IsDirectory(dc) || FileSystem::Extension(dc) != U"json"; });

	// 名前の読み込み(リンクがあるため、Loadの前に名前の登録を行う)
	for (const auto& jsonFile : jsonFiles)
	{
		JSONReader json(jsonFile);

		auto a = makeAsset(json[U"type"].getString());
		a->setName(json[U"name"].getString());
		a->setFilePath(FileSystem::RelativePath(jsonFile, m_filePath + U"assets/"));
	}

	// 読み込み
	for (const auto& m : m_assets)
	{
		Logger << m->getName() + U"を読み込み中";

		JSONReader json(m_filePath + U"assets/" + m->getFilePath());
		m->load(json);

		Logger << m->getName() + U"を読み込み成功";
	}

	// CellAssetの初期化
	{
		const auto cellAssets = getAssets<CellAsset>();

		for (const auto& ca : cellAssets) ca->updateProperties();
	}
}

void World::load()
{
	Logger << U"WORLD LOAD : " << m_filePath;

	// World
	{
		JSONReader json;

		m_name = json.getString();
	}

	// Assets
	loadAssets();

	// Field
	{
		m_field.load(m_filePath + U"field/");
	}
}

void World::make()
{
	// ディレクトリの作成
	FileSystem::CreateDirectories(m_filePath);
	FileSystem::CreateDirectories(m_filePath + U"field/");
	FileSystem::CreateDirectories(m_filePath + U"assets/");

	// アセットのコピー
	FileSystem::Copy(U"resources/assets/", m_filePath + U"assets/", CopyOption::OverwriteExisting);

	// Assetsのロード
	loadAssets();
}

shared_ptr<Asset> World::getAsset(const String& name) const
{
	for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
		if ((*it)->getName() == name) return dynamic_pointer_cast<Asset>(*it);

	throw Error(U"存在しない名前のモデルを参照しました name:" + name);
}

bool World::hasAsset(const String& name) const
{
	for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
		if ((*it)->getName() == name) return true;

	return false;
}

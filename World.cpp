#include "World.h"
#include "Assets.h"

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
		Assets::Save(m_filePath + U"assets/");
	}

	// Field
	{
		m_field.save(m_filePath + U"field/");
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
	{
		Assets::Load(m_filePath + U"assets/");
	}

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
	{
		Assets::Load(m_filePath + U"assets/");
	}
}

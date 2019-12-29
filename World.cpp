﻿#include "World.h"
#include "Asset.h"
#include "CellAsset.h"
#include "CellState.h"
#include "EggState.h"
#include "TileState.h"
#include "PartState.h"
#include "ProteinState.h"

unique_ptr<World>	World::g_instance;

void World::Make()
{
	g_instance = MakeUnique<World>();

	// FilePathの設定
	g_instance->m_filePath = U"world/";

	g_instance->make();

	g_instance->initField();

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
	// Cell
	{
		for (const auto& cellState : m_cellStates)
		{
			if (cellState->isDestroyed()) continue;

			cellState->updateParticle();
			cellState->updateRigidbody();
			cellState->updateCell();
		}

		m_cellStates.remove_if([](const auto& cellState) { return cellState->isDestroyed(); });
		m_cellStateKDTree.rebuildIndex();
	}

	// Egg
	{
		for (const auto& eggState : getEggStates())
		{
			if (eggState->isDestroyed()) continue;

			eggState->updateParticle();
			eggState->updateEgg();
		}

		m_eggStates.remove_if([](const auto& eggState) { return eggState->isDestroyed(); });
		m_eggStateKDTree.rebuildIndex();
	}

	// Protein
	{
		for (const auto& proteinState : getProteinStates())
		{
			if (proteinState->isDestroyed()) continue;

			proteinState->updateParticle();
			proteinState->updateProtein();
		}

		// 存在しないProteinの削除
		m_proteinStates.remove_if([](const auto& proteinState) { return proteinState->isDestroyed(); });

		m_proteinStateKDTree.rebuildIndex();
	}

	// Tile
	{
		for (const auto& tile : m_tiles)
		{
			tile->update();
		}
	}
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
		Serializer<MemoryWriter> writer;

		// Cells
		writer << int(m_cellStates.size());
		for (const auto& cellState : m_cellStates)
			cellState->save(writer);

		// Eggs
		writer << int(m_eggStates.size());
		for (const auto& eggState : m_eggStates)
			eggState->save(writer);

		// Proteins
		writer << int(m_proteinStates.size());
		for (const auto& proteinState : m_proteinStates)
			proteinState->save(writer);

		// Tiles
		writer << m_tileSize;
		for (const auto& tileState : m_tiles)
			tileState->save(writer);

		BinaryWriter binWriter(m_filePath + U"field");
		binWriter.write(writer.getWriter().data(), writer.getWriter().size());
	}
}

void World::initField()
{
	// Tiles
	{
		const Size size = m_tileSize;

		m_tileSize = size;
		m_tiles.resize(size);

		for (auto point : step(size))
		{
			m_tiles[point] = MakeShared<TileState>(point);
			m_tiles[point]->setElement(50.0);
		}
	}

	generateWave(Size(80, 45));
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
		Deserializer<ByteArray> reader(m_filePath + U"field");

		{
			int cellStateSize;
			reader >> cellStateSize;
			m_cellStates.resize(cellStateSize);

			for (auto& cellState : m_cellStates) cellState = MakeShared<CellState>(reader);
		}

		{
			int eggStateSize;
			reader >> eggStateSize;
			m_eggStates.resize(eggStateSize);

			for (auto& eggState : m_eggStates) eggState = MakeShared<EggState>(reader);
		}

		{
			int proteinStateSize;
			reader >> proteinStateSize;
			m_proteinStates.resize(proteinStateSize);

			for (auto& proteinState : m_proteinStates) proteinState = MakeShared<ProteinState>(reader);
		}

		{
			Size tileStateSize;
			reader >> tileStateSize;
			m_tiles.resize(tileStateSize);

			for (auto p : step(m_tiles.size()))
				m_tiles.at(p) = MakeShared<TileState>(p);

			for (const auto& tileState : m_tiles)
				tileState->load(reader);
		}
	}
}

void World::make()
{
	// ディレクトリの作成
	FileSystem::CreateDirectories(m_filePath);
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

World::World()
	: m_cellStateKDTree(m_cellStates)
	, m_eggStateKDTree(m_eggStates)
	, m_proteinStateKDTree(m_proteinStates)
	, m_tileSize(80, 45)
	, m_tileLength(100)
	, m_tiles(m_tileSize)
{
	m_fieldSize = m_tileSize * m_tileLength;
	m_cellStates.reserve(0xFFFF);
	m_proteinStates.reserve(0xFFFF);
	m_eggStates.reserve(0xFFFF);
}

void World::draw()
{
	static bool showWave = false;
	if (KeyP.down()) showWave = !showWave;

	for (const auto& tile : m_tiles)
		tile->draw();

	for (const auto& e : getProteinStates())
		if (!e->isDestroyed()) e->draw();

	for (const auto& e : getEggStates())
		if (!e->isDestroyed()) e->draw();

	for (const auto& c : m_cellStates)
		if (!c->isDestroyed()) c->draw();
}

const shared_ptr<CellState>& World::addCellState(const shared_ptr<CellAsset>& asset)
{
	return m_cellStates.emplace_back(make_shared<CellState>(asset));
}

const shared_ptr<EggState>& World::addEggState(const shared_ptr<CellAsset>& asset)
{
	return m_eggStates.emplace_back(make_shared<EggState>(asset));
}

const shared_ptr<ProteinState>& World::addProteinState(const shared_ptr<ProteinAsset>& asset)
{
	return m_proteinStates.emplace_back(make_shared<ProteinState>(asset));
}

shared_ptr<TileState> World::getTile(const Point& point) const
{
	Point ap = point;

	if (ap.x < 0) ap.x = 0;
	if (ap.y < 0) ap.y = 0;
	if (ap.x >= m_tileSize.x) ap.x = m_tileSize.x - 1;
	if (ap.y >= m_tileSize.y) ap.y = m_tileSize.y - 1;

	return m_tiles[ap];
}

void World::generateWave(const Size& tileSize)
{
	m_tileSize = tileSize;
	m_tiles.resize(tileSize);

	const PerlinNoise perlinNoiseX(Random(0xFFFFFFFF));
	const PerlinNoise perlinNoiseY(Random(0xFFFFFFFF));
	const auto interval = 500.0;

	for (auto& tile : m_tiles)
	{
		//外的圧力比
		const double rx = (tile->getPoint().x - m_tileSize.x / 2.0) / (m_tileSize.x / 2.0);
		const double ry = (tile->getPoint().y - m_tileSize.y / 2.0) / (m_tileSize.y / 2.0);

		const auto wx = perlinNoiseX.noise(tile->getCentroid().x / interval, tile->getCentroid().y / interval);
		const auto wy = perlinNoiseY.noise(tile->getCentroid().x / interval, tile->getCentroid().y / interval);

		// 最大の長さを1とする
		tile->setWaveVelocity(Vec2(Math::Lerp(wx, rx > 0 ? -1.0 : 1.0, EaseInExpo(Abs(rx))), Math::Lerp(wy, ry > 0 ? -1.0 : 1.0, EaseInExpo(Abs(ry)))) / Math::Sqrt2);
	}
}

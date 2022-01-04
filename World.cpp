#include "World.h"
#include "Asset.h"
#include "CellAsset.h"
#include "CellState.h"
#include "EggState.h"
#include "PartState.h"
#include "TileState.h"
#include <thread>

std::unique_ptr<World> World::g_instance;

void World::MakeForEditor() {
  g_instance = std::make_unique<World>();

  // Assetのロード
  g_instance->loadAssets(U"resources/assets/");
  for (const auto &asset : g_instance->m_assets)
    asset->setIsUserAsset(false);
}

void World::Make() {
  g_instance = std::make_unique<World>();

  // FilePathの設定
  g_instance->m_filePath = U"world/";

  g_instance->make();
}

void World::Load(const FilePath &filepath) {
  g_instance = std::make_unique<World>();

  g_instance->m_filePath = filepath;

  g_instance->load();
}

void World::update() {
  // Cell
  {
    for (const auto &cellState : m_cellStates) {
      if (cellState->isDestroyed())
        continue;

      cellState->updateParticle();
      cellState->updateRigidbody();
      cellState->updateCell();
    }

    m_cellStates.remove_if(
        [](const auto &cellState) { return cellState->isDestroyed(); });
    m_cellStateKDTree.rebuildIndex();

    World::GetAssets<CellAsset>().each(
        [](const auto &cellAsset) { cellAsset->m_log.update(cellAsset); });
  }

  // Egg
  {
    for (const auto &eggState : getEggStates()) {
      if (eggState->isDestroyed())
        continue;

      eggState->updateParticle();
      eggState->updateEgg();
    }

    m_eggStates.remove_if(
        [](const auto &eggState) { return eggState->isDestroyed(); });
    m_eggStateKDTree.rebuildIndex();
  }

  // Tile
  updateTiles();
}

void World::save() {
  Logger << U"WORLD SAVE : " << m_filePath;

  // World
  {
    JSON json;

    json.startObject();

    json[U"name"] = m_name;

    json.endObject();

    json.save(m_filePath + U"world.json");
  }

  // Assets
  {
    for (const auto &asset : m_assets.filter(
             [](const auto &asset) { return asset->getIsUserAsset(); })) {
      if (asset->getTypeName() != U"CellAsset" &&
          asset->getTypeName() != U"Part_BodyAsset")
        continue;

      JSON json;
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
    for (const auto &cellState : m_cellStates)
      cellState->save(writer);

    // Eggs
    writer << int(m_eggStates.size());
    for (const auto &eggState : m_eggStates)
      eggState->save(writer);

    // Tiles
    writer << m_tiles.size();
    for (const auto &tileState : m_tiles)
      tileState.save(writer);

    BinaryWriter binWriter(m_filePath + U"field");
    binWriter.write(writer.getWriter().data(), writer.getWriter().size());
  }
}

void World::initField() {
  // Tiles
  for (auto &tile : m_tiles)
    tile.m_element = m_elementPerTile;

  generateWave();

  initTiles();
}

void World::setTileSize(const Point &size) {
  m_fieldSize = size * TileLength;
  m_tiles.resize(size);
  m_tiles_swap.resize(size);
  m_tileGroups.clear();
  m_tileGroups.resize(Max(int(std::thread::hardware_concurrency()), 1));

  for (auto p : step(m_tiles.size()))
    m_tileGroups[(p.y * m_tiles.size().x + p.x) % m_tileGroups.size()]
        .emplace_back(p);
}

void World::loadAssets(const FilePath &directory) {
  Array<std::shared_ptr<Asset>> assets;

  // JSONのパスを取得
  auto jsonFiles = FileSystem::DirectoryContents(directory, true)
                       .removed_if([](const auto &dc) {
                         return FileSystem::IsDirectory(dc) ||
                                FileSystem::Extension(dc) != U"json";
                       });

  // 名前の読み込み(リンクがあるため、Loadの前に名前の登録を行う)
  for (auto &jsonFile : jsonFiles) {
    JSON json(jsonFile);

    const auto asset =
        assets.emplace_back(makeAsset(json[U"type"].getString()));
    asset->setName(json[U"name"].getString());
    asset->setFilePath(jsonFile);
  }

  // 読み込み
  for (const auto &m : assets) {
    Logger << m->getName() + U"を読み込み中";

    JSON json(m->getFilePath());
    m->load(json);

    Logger << m->getName() + U"を読み込み成功";
  }

  // CellAssetの初期化
  for (const auto &cellAsset : getAssets<CellAsset>()) {
    cellAsset->updateProperties();
    cellAsset->preRender();
  }
}

void World::init() {
  g_instance->initField();

  // Field情報の保存
  g_instance->save();
}

void World::load() {
  Logger << U"WORLD LOAD : " << m_filePath;

  // World
  {
    JSON json;

    m_name = json.getString();
  }

  // Assets
  loadAssets(U"resources/assets/");
  for (const auto &asset : m_assets)
    asset->setIsUserAsset(false);
  loadAssets(m_filePath + U"assets/");

  // Field
  {
    Deserializer<BinaryReader> reader(m_filePath + U"field");

    {
      int cellStateSize;
      reader >> cellStateSize;
      m_cellStates.resize(cellStateSize);

      for (auto &cellState : m_cellStates)
        cellState = std::make_shared<CellState>(reader);
    }

    {
      int eggStateSize;
      reader >> eggStateSize;
      m_eggStates.resize(eggStateSize);

      for (auto &eggState : m_eggStates)
        eggState = std::make_shared<EggState>(reader);
    }

    {
      Size tileStateSize;
      reader >> tileStateSize;
      setTileSize(tileStateSize);

      for (auto &tileState : m_tiles)
        tileState.load(reader);

      initTiles();
    }
  }
}

void World::make() {
  // ディレクトリの作成
  FileSystem::CreateDirectories(m_filePath);
  FileSystem::CreateDirectories(m_filePath + U"assets/");

  // Assetsのロード
  loadAssets(U"resources/assets/");
  for (const auto &asset : m_assets)
    asset->setIsUserAsset(false);
  loadAssets(m_filePath + U"assets/");
}

std::shared_ptr<Asset> World::getAsset(const String &name) const {
  for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
    if ((*it)->getName() == name)
      return std::dynamic_pointer_cast<Asset>(*it);

  throw Error(U"存在しない名前のモデルを参照しました name:" + name);
}

bool World::hasAsset(const String &name) const {
  for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
    if ((*it)->getName() == name)
      return true;

  return false;
}

void World::updateTiles() {
  // Tileの本体からswapにコピー
  std::memcpy(&m_tiles_swap[0][0], &m_tiles[0][0], m_tiles.size_bytes());

  // SwapのElementリセット
  for (auto &tile_swap : m_tiles_swap) {
    tile_swap.m_element = 0;
    tile_swap.m_poison = 0;
  }

  Array<ConcurrentTask<void>> tasks;

  for (int i = 0; i < m_tileGroups.size(); ++i)
    // World::updateTileGroup(i);
    tasks.emplace_back(&World::updateTileGroup, this, i);

  for (auto &t : tasks)
    while (!t.is_done())
      ;

  // Tileのswapから本体にコピー
  std::memcpy(&m_tiles[0][0], &m_tiles_swap[0][0], m_tiles.size_bytes());
}

void World::updateTileGroup(int groupIndex) {
  const int xMax = m_tiles.size().x - 1;
  const int yMax = m_tiles.size().y - 1;

  for (const auto &p : m_tileGroups[groupIndex]) {
    for (int x = 0; x < 3; ++x) {
      if (p.x == 0 && x == 0)
        continue;
      if (p.x == xMax && x == 2)
        continue;

      for (int y = 0; y < 3; ++y) {
        if (p.y == 0 && y == 0)
          continue;
        if (p.y == yMax && y == 2)
          continue;

        const auto &fromTile = m_tiles[p.y + y - 1][p.x + x - 1];
        const auto sendRate = fromTile.m_sendRate[2 - x][2 - y];

        m_tiles_swap[p].m_element += (fromTile.m_element * sendRate);
        m_tiles_swap[p].m_poison += (fromTile.m_poison * sendRate);
      }
    }
  }
}

void World::initTiles() {
  // SendRateの計算
  for (auto p : step(m_tiles.size())) {
    auto &tile = m_tiles[p];
    const Vec2 d = tile.m_waveVelocity * 0.015;
    const double l = 0.01;
    const double w = 1.0 + l * 2;
    const RectF rect = RectF(-l, -l, w, w).movedBy(d);
    const double area = rect.area();

    // 初期化
    for (auto point : step(Size(3, 3)))
      tile.m_sendRate[point.x][point.y] = 0.0;

    // 周囲
    if (rect.tl().x < 0.0)
      tile.m_sendRate[0][1] = (-rect.tl().x) *
                              (Min(rect.br().y, 1.0) - Max(rect.tl().y, 0.0)) /
                              area;
    if (rect.tl().y < 0.0)
      tile.m_sendRate[1][0] = (-rect.tl().y) *
                              (Min(rect.br().x, 1.0) - Max(rect.tl().x, 0.0)) /
                              area;
    if (rect.br().x > 1.0)
      tile.m_sendRate[2][1] = (rect.br().x - 1) *
                              (Min(rect.br().y, 1.0) - Max(rect.tl().y, 0.0)) /
                              area;
    if (rect.br().y > 1.0)
      tile.m_sendRate[1][2] = (rect.br().y - 1) *
                              (Min(rect.br().x, 1.0) - Max(rect.tl().x, 0.0)) /
                              area;
    if (rect.tl().x < 0.0 && rect.tl().y < 0.0)
      tile.m_sendRate[0][0] = (-rect.tl().x) * (-rect.tl().y) / area;
    if (rect.tl().x < 0.0 && rect.br().y > 1.0)
      tile.m_sendRate[0][2] = (-rect.tl().x) * (rect.br().y - 1.0) / area;
    if (rect.br().x > 1.0 && rect.tl().y < 0.0)
      tile.m_sendRate[2][0] = (rect.br().x - 1.0) * (-rect.tl().y) / area;
    if (rect.br().x > 1.0 && rect.br().y > 1.0)
      tile.m_sendRate[2][2] = (rect.br().x - 1.0) * (rect.br().y - 1.0) / area;

    // 中心
    tile.m_sendRate[1][1] =
        1.0 - tile.m_sendRate[0][0] - tile.m_sendRate[1][0] -
        tile.m_sendRate[2][0] - tile.m_sendRate[0][1] - tile.m_sendRate[2][1] -
        tile.m_sendRate[0][2] - tile.m_sendRate[1][2] - tile.m_sendRate[2][2];

    // 存在しないところの分を移動
    if (p.x == 0) {
      for (int i = 0; i < 3; ++i) {
        tile.m_sendRate[1][i] += tile.m_sendRate[0][i];
        tile.m_sendRate[0][i] = 0;
      }
    }
    if (p.y == 0) {
      for (int i = 0; i < 3; ++i) {
        tile.m_sendRate[i][1] += tile.m_sendRate[i][0];
        tile.m_sendRate[i][0] = 0;
      }
    }
    if (p.x == m_tiles.size().x - 1) {
      for (int i = 0; i < 3; ++i) {
        tile.m_sendRate[1][i] += tile.m_sendRate[2][i];
        tile.m_sendRate[2][i] = 0;
      }
    }
    if (p.y == m_tiles.size().y - 1) {
      for (int i = 0; i < 3; ++i) {
        tile.m_sendRate[i][1] += tile.m_sendRate[i][2];
        tile.m_sendRate[i][2] = 0;
      }
    }
  }
}

World::World()
    : m_cellStateKDTree(m_cellStates), m_eggStateKDTree(m_eggStates) {
  m_cellStates.reserve(0xFFFF);
  m_eggStates.reserve(0xFFFF);
}

void World::draw() {
  RectF(m_fieldSize).draw(Color(11, 22, 33));

  // Tiles
  {
	// TODO:Enable Shader
    //static const PixelShader ps(U"resources/tile" SIV3D_SELECT_SHADER(U".hlsl", U".frag"), {{U"PSConstants2D", 0}});
    //const ScopedRenderStates2D state(SamplerState::BorderLinear);
    //const ScopedCustomShader2D shader(ps);

    Image image(m_tiles.size());

    {
      for (auto p : step(m_tiles.size()))
        image[p] =
            ColorF(Palette::Palegreen, Min(0.8, m_tiles[p].m_element * 0.01));

      m_tileTextureElement.fill(image);

      m_tileTextureElement.scaled(TileLength).draw();
    }

    {
      for (auto p : step(m_tiles.size()))
        image[p] =
            ColorF(Palette::Purple, Min(0.8, m_tiles[p].m_poison * 0.01));

      m_tileTexturePoison.fill(image);

      m_tileTexturePoison.scaled(TileLength).draw();
    }
  }

  // Texture
  {
    const auto mat3x2 = Graphics2D::GetLocalTransform().inversed();
    const auto viewRect = RectF(mat3x2.transform(Scene::Rect().pos),
                                mat3x2.transform(Scene::Rect().size));

    for (const auto &eggState : m_eggStates)
      if (!eggState->isDestroyed() &&
          viewRect.intersects(
              Circle(eggState->getPosition(),
                     eggState->getCellAsset()->getDrawRadius())))
        eggState->draw1();

    for (const auto &eggState : m_eggStates)
      if (!eggState->isDestroyed() &&
          viewRect.intersects(
              Circle(eggState->getPosition(),
                     eggState->getCellAsset()->getDrawRadius())))
        eggState->draw2();

    for (const auto &cellState : m_cellStates)
      if (!cellState->isDestroyed() &&
          viewRect.intersects(Circle(cellState->getPosition(),
                                     cellState->m_cellAsset->getDrawRadius())))
        cellState->draw();
  }
}

const std::shared_ptr<CellState> &
World::addCellState(const std::shared_ptr<CellAsset> &asset) {
  return m_cellStates.emplace_back(std::make_shared<CellState>(asset));
}

const std::shared_ptr<EggState> &
World::addEggState(const std::shared_ptr<CellAsset> &asset) {
  return m_eggStates.emplace_back(std::make_shared<EggState>(asset));
}

void World::generateWave() {
  for (auto p : step(m_tiles.size())) {
    auto &tile = m_tiles[p];

    //外的圧力比
    const double rx = (p.x - m_tiles.size().x / 2.0) / (m_tiles.size().x / 2.0);
    const double ry = (p.y - m_tiles.size().y / 2.0) / (m_tiles.size().y / 2.0);

    const auto wx = m_perlinNoiseX.noise(
        TileLength * (Vec2(p) + Vec2(0.5, 0.5)).x / m_waveInterval,
        TileLength * (Vec2(p) + Vec2(0.5, 0.5)).y / m_waveInterval);
    const auto wy = m_perlinNoiseY.noise(
        TileLength * (Vec2(p) + Vec2(0.5, 0.5)).x / m_waveInterval,
        TileLength * (Vec2(p) + Vec2(0.5, 0.5)).y / m_waveInterval);

    // 最大の長さを1とする
    tile.m_waveVelocity =
        Vec2(Math::Lerp(wx, rx > 0 ? -1.0 : 1.0, EaseInExpo(Abs(rx))),
             Math::Lerp(wy, ry > 0 ? -1.0 : 1.0, EaseInExpo(Abs(ry)))) /
        Math::Sqrt2;
  }
}

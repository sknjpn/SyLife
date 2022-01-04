#include "Asset.h"
#include "World.h"

Asset::Asset() {
  // 一意な名前とパスの作成
  for (int i = 0;; ++i) {
    const String name = Format(U"asset_", i);
    const FilePath filepath = Format(U"asset_", i, U".json");

    if (!World::HasAsset(name) &&
        !FileSystem::Exists(World::GetDirectory() + U"assets/" + filepath)) {
      m_name = name;
      m_filepath = filepath;

      break;
    }
  }
}

void Asset::load(const JSON &json) {
  Model::load(json);

  m_name = json[U"name"].getString();
  m_nameJP = json[U"nameJP"].getString();
}

void Asset::save(JSON &json) const {
  Model::save(json);

  // name
  json.key(U"name").write(m_name);
  json.key(U"nameJP").write(m_nameJP);
}

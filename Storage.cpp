#include "Storage.h"
#include "ProteinAsset.h"
#include "World.h"

bool Storage::operator>=(const Storage& s) const {
  if (m_element < s.m_element)
    return false;

  for (const auto& m : s)
    if (numProtein(m.first) < m.second)
      return false;

  return true;
}

bool Storage::operator<=(const Storage& s) const {
  if (s.m_element > m_element)
    return false;

  for (const auto& m : *this)
    if (m.second > s.numProtein(m.first))
      return false;

  return true;
}

Storage& Storage::operator+=(const Storage& s) noexcept {
  m_element += s.m_element;

  for (const auto& m : s)
    addProtein(m.first, m.second);

  return *this;
}

Storage& Storage::operator-=(const Storage& s) noexcept {
  m_element -= s.m_element;

  for (const auto& m : s)
    pullProtein(m.first, m.second);

  return *this;
}

bool Storage::contain(const Storage& s) const {
  return containProtein(s) && m_element >= s.m_element;
}

bool Storage::containProtein(const Storage& s) const {
  for (const auto& m : s)
    if (numProtein(m.first) < m.second)
      return false;

  return true;
}

double Storage::getElementRecursive() const {
  double sum = m_element;

  for (const auto& m : *this)
    sum += m.first->getMaterial().getElementRecursive() * m.second;

  return sum;
}

void Storage::addProtein(const std::shared_ptr<ProteinAsset>& asset, int size) {
  auto it = find_if(begin(), end(), [&asset](const auto& m) { return m.first == asset; });

  if (it == end())
    emplace_back(asset, size);
  else
    (*it).second += size;
}

void Storage::pullProtein(const std::shared_ptr<ProteinAsset>& asset,
    int                                                        size) {
  auto it = find_if(begin(), end(), [&asset](const auto& m) { return m.first == asset; });

  if (it == end())
    throw Error(U"全く存在しないProteinの削除を試みました");
  else {
    if (((*it).second -= size) < 0)
      throw Error(U"存在しない量のProteinの削除を試みました");
    else if ((*it).second == 0)
      erase(it);
  }
}

int Storage::numProtein(const std::shared_ptr<ProteinAsset>& asset) const {
  auto it = find_if(begin(), end(), [&asset](const auto& m) { return m.first == asset; });

  if (it == end())
    return 0;
  else
    return (*it).second;
}

void Storage::load(const JSON& json) {
  // element
  m_element = json[U"element"].get<double>();

  // proteins
  for (auto protein : json[U"proteins"].arrayView()) {
    const auto& asset = World::GetAsset<ProteinAsset>(protein[U"name"].getString());
    const int   size = protein[U"size"].get<int>();

    emplace_back(asset, size);
  }
}

void Storage::save(JSON& json) const {
  // element
  json[U"element"] = m_element;

  // proteins
  {
    Array<JSON> jsonArray;

    for (const auto& protein : *this) {
      JSON& j = jsonArray.emplace_back();
      j[U"name"] = protein.first->getName();
      j[U"size"] = protein.second;
    }

    json[U"proteins"] = jsonArray;
  }
}

void Storage::load(Deserializer<BinaryReader>& reader) {
  // element
  reader >> m_element;

  // proteins
  {
    int storageSize;
    reader >> storageSize;

    for (int i = 0; i < storageSize; ++i) {
      String proteinAssetName;
      int    proteinSize;

      reader >> proteinAssetName;
      reader >> proteinSize;

      emplace_back(World::GetAsset<ProteinAsset>(proteinAssetName), proteinSize);
    }
  }
}

void Storage::save(Serializer<MemoryWriter>& writer) const {
  // element
  writer << m_element;

  // proteins
  {
    writer << int(size());

    for (const auto& protein : *this) {
      writer << protein.first->getName();
      writer << protein.second;
    }
  }
}

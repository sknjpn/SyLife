#include "Storage.h"

#include "ElementAsset.h"

#include "AssetManager.h"

bool Storage::operator>=(const Storage& s) const
{
	if (m_nutrition < s.m_nutrition) return false;

	for (const auto& m : s)
		if (numElement(m.first) < m.second) return false;

	return true;
}

bool Storage::operator<=(const Storage& s) const
{
	if (s.m_nutrition > m_nutrition) return false;

	for (const auto& m : *this)
		if (m.second > s.numElement(m.first)) return false;

	return true;
}

Storage& Storage::operator+=(const Storage& s) noexcept
{
	m_nutrition += s.m_nutrition;

	for (const auto& m : s)
		addElement(m.first, m.second);

	return *this;
}

Storage& Storage::operator-=(const Storage& s) noexcept
{
	m_nutrition -= s.m_nutrition;

	for (const auto& m : s)
		pullElement(m.first, m.second);

	return *this;
}

void Storage::addElement(const shared_ptr<ElementAsset>& asset, int size)
{
	auto it = find_if(begin(), end(), [&asset](const auto& m) { return m.first == asset; });

	if (it == end()) emplace_back(asset, size);
	else (*it).second += size;
}

void Storage::pullElement(const shared_ptr<ElementAsset>& asset, int size)
{
	auto it = find_if(begin(), end(), [&asset](const auto& m) { return m.first == asset; });

	if (it == end()) throw Error(U"全く存在しないElementの削除を試みました");
	else
	{
		if (((*it).second -= size) < 0) throw Error(U"存在しない量のElementの削除を試みました");
		else if ((*it).second == 0) erase(it);
	}
}

int Storage::numElement(const shared_ptr<ElementAsset>& asset) const
{
	auto it = find_if(begin(), end(), [&asset](const auto& m) { return m.first == asset; });

	if (it == end()) return 0;
	else return (*it).second;
}

void Storage::load(const JSONValue& json)
{
	Model::load(json);

	// nutrition
	m_nutrition = json[U"nutrition"].get<double>();

	// elements
	for (auto element : json[U"elements"].arrayView())
	{
		const auto& asset = g_assetManagerPtr->getAsset<ElementAsset>(element[U"name"].getString());
		const int size = element[U"size"].get<int>();

		emplace_back(asset, size);
	}
}

void Storage::save(JSONWriter& json) const
{
	Model::save(json);

	// nutrition
	json.key(U"nutrition").write(m_nutrition);

	// elements
	{
		json.key(U"elements").startArray();

		for (const auto& element : *this)
		{
			json.startObject();
		
			json.key(U"name").write(element.first->getName());
			json.key(U"size").write(element.second);

			json.endObject();
		}

		json.endArray();
	}
}

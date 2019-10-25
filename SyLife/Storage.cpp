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

void Storage::load_this(const ptree& pt)
{
	// nutrition
	m_nutrition = pt.get<double>("nutrition");

	// elements
	for (auto m : pt.get_child("elements"))
	{
		auto name = m.second.get<string>("name");

		const auto& asset = g_assetManagerPtr->getAsset<ElementAsset>(name);

		emplace_back(asset, m.second.get<int>("size"));
	}
}

void Storage::save_this(ptree& pt) const
{
	// nutrition
	pt.put<double>("nutrition", m_nutrition);

	// elements
	{
		ptree elements;

		for (const auto& m : *this)
		{
			ptree pt2;
			pt2.put<string>("name", m.first->getName());
			pt2.put<int>("size", m.second);

			elements.push_back(std::make_pair("", pt2));
		}

		pt.add_child("elements", elements);
	}
}

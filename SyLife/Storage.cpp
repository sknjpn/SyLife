#include "Storage.h"

#include "Element.h"

#include "AssetManager.h"

bool Storage::operator>=(const Storage& s) const
{
	for (const auto& m : s)
		if (numElement(m.first) < m.second) return false;

	return true;
}

bool Storage::operator<=(const Storage& s) const
{
	for (const auto& m : s)
		if (s.numElement(m.first) < m.second) return false;

	return true;
}

Storage& Storage::operator+=(const Storage& s) noexcept
{
	for (const auto& m : s)
		addElement(m.first, m.second);

	return *this;
}

Storage& Storage::operator-=(const Storage& s) noexcept
{
	for (const auto& m : s)
		pullElement(m.first, m.second);

	return *this;
}

void Storage::addElement(const shared_ptr<ElementAsset>& model, int size)
{
	auto it = find_if(begin(), end(), [&model](const auto& m) { return m.first == model; });

	if (it == end()) emplace_back(model, size);
	else (*it).second += size;
}

void Storage::pullElement(const shared_ptr<ElementAsset>& model, int size)
{
	auto it = find_if(begin(), end(), [&model](const auto& m) { return m.first == model; });

	if (it == end()) throw Error(U"全く存在しないElementの削除を試みました");
	else
	{
		if (((*it).second -= size) < 0) throw Error(U"存在しない量のElementの削除を試みました");
		else if ((*it).second == 0) erase(it);
	}
}

int Storage::numElement(const shared_ptr<ElementAsset>& model) const
{
	auto it = find_if(begin(), end(), [&model](const auto& m) { return m.first == model; });

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

		const auto& model = g_assetManagerPtr->getModel<ElementAsset>(name);

		emplace_back(model, m.second.get<int>("size"));
	}

	Model::load_this(pt);
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
			ptree part;
			part.put<string>("name", m.first->getName());
			part.put<int>("size", m.second);

			elements.push_back(std::make_pair("", part));
		}

		pt.add_child("elements", elements);
	}

	Model::save_this(pt);

	// type
	pt.put("type", "Storage");
}

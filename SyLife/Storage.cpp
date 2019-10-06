#include "Storage.h"

#include "AssetManager.h"

bool Storage::operator>=(const Storage& s) const
{
	for (const auto& m : s)
		if (numMolecule(m.first) < m.second) return false;

	return true;
}

bool Storage::operator<=(const Storage& s) const
{
	for (const auto& m : s)
		if (s.numMolecule(m.first) < m.second) return false;

	return true;
}

Storage& Storage::operator+=(const Storage& s) noexcept
{
	for (const auto& m : s)
		addMolecule(m.first, m.second);

	return *this;
}

Storage& Storage::operator-=(const Storage& s) noexcept
{
	for (const auto& m : s)
		pullMolecule(m.first, m.second);

	return *this;
}

void Storage::addMolecule(const shared_ptr<MoleculeModel>& model, int size)
{
	auto it = find_if(begin(), end(), [&model](const auto& m) { return m.first == model; });

	if (it == end()) emplace_back(model, size);
	else (*it).second += size;
}

void Storage::pullMolecule(const shared_ptr<MoleculeModel>& model, int size)
{
	auto it = find_if(begin(), end(), [&model](const auto& m) { return m.first == model; });

	if (it == end()) throw Error(U"全く存在しないMoleculeの削除を試みました");
	else
	{
		if (((*it).second -= size) < 0) throw Error(U"存在しない量のMoleculeの削除を試みました");
		else if ((*it).second == 0) erase(it);
	}
}

int Storage::numMolecule(const shared_ptr<MoleculeModel>& model) const
{
	auto it = find_if(begin(), end(), [&model](const auto& m) { return m.first == model; });

	if (it == end()) return 0;
	else return (*it).second;
}

void Storage::load_this(const ptree& pt)
{
	// molecules
	for (auto m : pt.get_child("molecules"))
	{
		auto name = m.second.get<string>("name");

		const auto& model = g_assetManagerPtr->GetModel<MoleculeModel>(name);

		emplace_back(model, m.second.get<int>("size"));
	}

	Model::load_this(pt);
}

void Storage::save_this(ptree& pt) const
{
	// molecules
	{
		ptree molecules;

		for (const auto& m : *this)
		{
			ptree part;
			part.put<string>("name", m.first->GetName());
			part.put<int>("size", m.second);

			molecules.push_back(std::make_pair("", part));
		}

		pt.add_child("molecules", molecules);
	}

	Model::save_this(pt);

	// type
	pt.put("type", "Storage");
}

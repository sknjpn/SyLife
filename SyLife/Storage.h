#pragma once

#include "Model.h"
#include "Molecule.h"

class MoleculeModel;

class Storage
	: public Model
{
	Array<pair<shared_ptr<MoleculeModel>, double>>	m_molecules;

public:
	// operator
	Storage	operator +(const Storage& s) const { return Storage(*this) += s; }
	Storage	operator -(const Storage& s) const { return Storage(*this) -= s; }
	bool operator >=(const Storage& s) const
	{
		for (const auto& m : s.m_molecules)
			if (Num(m.first) < m.second) return false;

		return true;
	}	
	bool operator <=(const Storage& s) const
	{
		for (const auto& m : m_molecules)
			if (s.Num(m.first) < m.second) return false;

		return true;
	}
	Storage& operator +=(const Storage& s) noexcept
	{
		for (const auto& m : s.m_molecules)
			Add(m.first, m.second);

		return *this;
	}
	Storage& operator -=(const Storage& s) noexcept
	{
		for (const auto& m : s.m_molecules)
			Pull(m.first, m.second);

		return *this;
	}

	// molecule
	void	Add(const shared_ptr<MoleculeModel>& model, double size)
	{
		auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

		if (it == m_molecules.end()) m_molecules.emplace_back(model, size);
		else (*it).second += size;
	}

	void	Pull(const shared_ptr<MoleculeModel>& model, double size)
	{
		auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

		if (it == m_molecules.end()) throw new exception;
		else
		{
			if (((*it).second -= size) < 0.0) throw new exception;
			else if ((*it).second == 0.0) m_molecules.erase(it);
		}
	}

	double	Num(const shared_ptr<MoleculeModel>& model) const
	{
		auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

		if (it == m_molecules.end()) return 0.0;
		else return (*it).second;
	}

	double	Num() const
	{
		double sum = 0.0;
		for (const auto& m : m_molecules)
			sum += m.second;

		return sum;
	}

	const Array<pair<shared_ptr<MoleculeModel>, double>>&	GetMolecules() const { return m_molecules; }

	bool	isEmpty() const { return m_molecules.empty(); }

	void	clear() { m_molecules.clear(); }

	// JSON
	void	load_this(const ptree& pt)
	{
		// molecules
		for (auto m : pt.get_child("molecules"))
		{
			auto name = m.second.get<string>("molecule");

			const auto& model = g_assetManagerPtr->GetModel<MoleculeModel>(name);

			m_molecules.emplace_back(model, m.second.get<double>("size"));
		}

		Model::load_this(pt);
	}
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const 
	{
		// molecules
		{
			ptree molecules;

			for (const auto& m : m_molecules)
			{
				ptree part;
				part.put<string>("molecule", m.first->GetName());
				part.put<int>("size", m.second);

				molecules.push_back(std::make_pair("", part));
			}

			pt.add_child("molecules", molecules);
		}

		Model::save_this(pt);

		// type
		pt.put("type", "Storage");
	}
	void	save(ptree& pt) const override { save_this(pt); }
};

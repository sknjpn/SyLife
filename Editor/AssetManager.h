#pragma once

#include <boost/property_tree/ptree.hpp>

using namespace boost::property_tree;

class Model;

class AssetManager
{
public:
	vector<shared_ptr<Model>>	m_models;

public:
	AssetManager()
	{
		m_models.reserve(1024);
	}

	void	Init();

	void	AddModels(const string& directory);

	void	AddModel(const string& filepath);

	template <typename T>
	void	AddModel(const string& filepath) { ptree pt; read_json(filepath, pt); AddModel<T>(pt); }

	template <typename T>
	void	AddModel(ptree pt) { return m_models.emplace_back(make_shared<T>())->Load(pt); }

	template <typename T>
	const shared_ptr<T>&	GetModel(const string& name) const;

	template <typename T>
	vector<shared_ptr<T>>	GetModels() const;
};

extern unique_ptr<AssetManager>	g_assetManagerPtr;

template <typename T>
inline const shared_ptr<T>& AssetManager::GetModel(const string& name) const
{
	for (auto it = m_models.begin(); it != m_models.end(); ++it)
		if ((*it)->m_name == name && dynamic_pointer_cast<T>(*it) != nullptr) return dynamic_pointer_cast<T>(*it);

	return nullptr;
}

template <typename T>
inline vector<shared_ptr<T>> AssetManager::GetModels() const;
{
	vector<shared_ptr<T>>	tModels;

	for (auto it = m_models.begin(); it != m_models.end(); ++it)
		if (dynamic_pointer_cast<T>(*it) != nullptr) tModels.emplace_back(dynamic_pointer_cast<T>(*it));

	return tModels;
}

#pragma once

class Model;

// すべてのAssetの統合管理を行う
class AssetManager
{
	Array<shared_ptr<Model>>	m_models;

public:
	AssetManager() { m_models.reserve(1024); }

	template <typename T>
	shared_ptr<T>		makeModel(const string& name) { return dynamic_pointer_cast<T>(m_models.emplace_back(make_shared<T>(name))); }

	template <typename T>
	shared_ptr<T>		makeModel() { return makeModel<T>(U"Model " + ToString(m_models.size())); }

	shared_ptr<Model>	makeModel(const string& name, const string& type);

	void	Init();

	bool	HasModel(const string& name) const { return getModel(name) != nullptr; }

	template <typename T>
	shared_ptr<T>		getModel(const string& name) const
	{
		for (auto it = m_models.begin(); it != m_models.end(); ++it)
			if ((*it)->getName() == name && dynamic_pointer_cast<T>(*it) != nullptr) return dynamic_pointer_cast<T>(*it);

		throw Error(U"存在しない名前のモデルを参照しました");
	}

	shared_ptr<Model>	getModel(const string& name) const;

	template <typename T>
	Array<shared_ptr<T>>	GetModels() const
	{
		Array<shared_ptr<T>> tModels;

		for (auto it = m_models.begin(); it != m_models.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tModels.emplace_back(dynamic_pointer_cast<T>(*it));

		return tModels;
	}
};

extern unique_ptr<AssetManager>	g_assetManagerPtr;

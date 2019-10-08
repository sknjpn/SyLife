#pragma once

class Asset;

// すべてのAssetの統合管理を行う
class AssetManager
{
	Array<shared_ptr<Asset>>	m_models;

public:
	AssetManager() { m_models.reserve(1024); }

	template <typename T>
	shared_ptr<T>		makeAsset(const string& name) { return dynamic_pointer_cast<T>(m_models.emplace_back(make_shared<T>(name))); }

	template <typename T>
	shared_ptr<T>		makeAsset() { return makeAsset<T>(U"Asset " + ToString(m_models.size())); }

	shared_ptr<Asset>	makeAsset(const string& name, const string& type);

	void	init();

	bool	HasAsset(const string& name) const { return getAsset(name) != nullptr; }

	template <typename T>
	shared_ptr<T>		getAsset(const string& name) const
	{
		for (auto it = m_models.begin(); it != m_models.end(); ++it)
			if ((*it)->getName() == name && dynamic_pointer_cast<T>(*it) != nullptr) return dynamic_pointer_cast<T>(*it);

		throw Error(U"存在しない名前のモデルを参照しました");
	}

	shared_ptr<Asset>	getAsset(const string& name) const;

	template <typename T>
	Array<shared_ptr<T>>	GetAssets() const
	{
		Array<shared_ptr<T>> tAssets;

		for (auto it = m_models.begin(); it != m_models.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tAssets.emplace_back(dynamic_pointer_cast<T>(*it));

		return tAssets;
	}
};

extern unique_ptr<AssetManager>	g_assetManagerPtr;

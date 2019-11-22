#pragma once

class Asset;

// すべてのAssetの統合管理を行う
class AssetManager
{
	Array<shared_ptr<Asset>>	m_assets;

public:
	AssetManager() { m_assets.reserve(1024); }

	template <typename T>
	shared_ptr<T>		makeAsset() { return dynamic_pointer_cast<T>(m_assets.emplace_back(make_shared<T>())); }

	shared_ptr<Asset>	makeAsset(const String& type);

	void	init();

	bool	hasAsset(const String& name) const { return getAsset(name) != nullptr; }

	template <typename T>
	shared_ptr<T>		getAsset(const String& name) const
	{
		for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
			if ((*it)->getName() == name && dynamic_pointer_cast<T>(*it) != nullptr) return dynamic_pointer_cast<T>(*it);

		throw Error(U"存在しない名前のモデルを参照しました");
	}

	shared_ptr<Asset>	getAsset(const String& name) const;

	template <typename T>
	Array<shared_ptr<T>>	getAssets() const
	{
		Array<shared_ptr<T>> tAssets;

		for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tAssets.emplace_back(dynamic_pointer_cast<T>(*it));

		return tAssets;
	}


	const Array<shared_ptr<Asset>>& getAssets() const { return m_assets; }
};

extern unique_ptr<AssetManager>	g_assetManagerPtr;

#pragma once

class Asset;

class Assets
	: Array<shared_ptr<Asset>>
{
public:
	Assets() { reserve(1024); }

	// Make
	template <typename T>
	shared_ptr<T>		makeAsset() { return dynamic_pointer_cast<T>(emplace_back(make_shared<T>())); }

	shared_ptr<Asset>	makeAsset(const String& type);
	
	// Get
	template <typename T>
	shared_ptr<T>		getAsset(const String& name) const
	{
		for (auto it = begin(); it != end(); ++it)
			if ((*it)->getName() == name && dynamic_pointer_cast<T>(*it) != nullptr) return dynamic_pointer_cast<T>(*it);

		throw Error(U"存在しない名前のモデルを参照しました name:" + name);
	}

	shared_ptr<Asset>	getAsset(const String& name) const;

	const Array<shared_ptr<Asset>>& getAssets() const { return *this; }

	template <typename T>
	Array<shared_ptr<T>>	getAssets() const
	{
		Array<shared_ptr<T>> tAssets;

		for (auto it = begin(); it != end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tAssets.emplace_back(dynamic_pointer_cast<T>(*it));

		return tAssets;
	}
	
	// Has
	bool	hasAsset(const String& name) const;

	// Load
	void	save(const FilePath& directory);
	void	load(const FilePath& directory);

	// Remove
	void	remove(const shared_ptr<Asset>& asset) { remove(asset); }

	void	clear() { Array<shared_ptr<Asset>>::clear(); }
};

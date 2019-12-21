#pragma once

#include "Asset.h"

class Assets
	: Array<shared_ptr<Asset>>
{
	static unique_ptr<Assets>	g_instance;

	void	save(const FilePath& directory) const;
	void	load(const FilePath& directory);

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

public:
	Assets() { reserve(1024); }

	static void	Save(const FilePath& directory);
	static void	Load(const FilePath& directory);

	// Make
	template <typename T>
	static shared_ptr<T>		MakeAsset() { return dynamic_pointer_cast<T>(g_instance->emplace_back(make_shared<T>())); }

	static shared_ptr<Asset>	MakeAsset(const String& type) { return g_instance->makeAsset(type); }

	// Get
	template <typename T>
	static shared_ptr<T>		GetAsset(const String& name) { return g_instance->getAsset<T>(name); }

	static shared_ptr<Asset>	GetAsset(const String& name) { return g_instance->getAsset(name); }

	static const Array<shared_ptr<Asset>>& GetAssets() { return *g_instance; }

	template <typename T>
	static Array<shared_ptr<T>>	GetAssets() { return g_instance->getAssets<T>(); }

	// Has
	static bool	HasAsset(const String& name) { return g_instance->hasAsset(name); }

	// Remove
	static void	Remove(const shared_ptr<Asset>& asset) { g_instance->remove(asset); }

	static void	Clear() { g_instance->clear(); }
};

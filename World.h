#pragma once

#include "Field.h"

class Asset;

class World
{
	String		m_name;
	FilePath	m_filePath;
	Field		m_field;

	Array<shared_ptr<Asset>>	m_assets;

	static unique_ptr<World>	g_instance;

	void	loadAssets();

	void	load();
	void	make();

	// Make
	template <typename T>
	shared_ptr<T>		makeAsset() { return dynamic_pointer_cast<T>(m_assets.emplace_back(make_shared<T>())); }

	shared_ptr<Asset>	makeAsset(const String& type);

	// Get
	template <typename T>
	shared_ptr<T>		getAsset(const String& name) const
	{
		auto asset = dynamic_pointer_cast<T>(getAsset(name));

		if (asset) return asset;
		throw Error(U"存在しない名前のモデルを参照しました name:" + name);
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

	// Has
	bool	hasAsset(const String& name) const;

public:
	// Assets
	template <typename T>
	static shared_ptr<T>		MakeAsset() { return dynamic_pointer_cast<T>(g_instance->m_assets.emplace_back(make_shared<T>())); }
	static shared_ptr<Asset>	MakeAsset(const String& type) { return g_instance->makeAsset(type); }
	template <typename T>
	static shared_ptr<T>		GetAsset(const String& name) { return g_instance->getAsset<T>(name); }
	static shared_ptr<Asset>	GetAsset(const String& name) { return g_instance->getAsset(name); }
	static const Array<shared_ptr<Asset>>& GetAssets() { return g_instance->m_assets; }
	template <typename T>
	static Array<shared_ptr<T>>	GetAssets() { return g_instance->getAssets<T>(); }
	static bool	HasAsset(const String& name) { return g_instance->hasAsset(name); }
	static void	Remove(const shared_ptr<Asset>& asset) { g_instance->m_assets.remove(asset); }
	static void	Clear() { g_instance->m_assets.clear(); }

	static void Make();
	static void Load(const FilePath& filepath);
	static const unique_ptr<World>& GetInstance() { return g_instance; }

	const Field& getField() const { return m_field; }
	Field& getField() { return m_field; }

	void	setName(const String& name) { m_name = name; }
	void	setFilePath(const FilePath& filePath) { m_filePath = filePath; }

	static FilePath	GetDirectory() { return g_instance->m_filePath; }

	void	update();

	void	save();
};

#pragma once

#include "Model.h"

class EasyViewer;

// AssetManagerに登録可能なModel
// JSONファイルに対応している
class Asset
	: public Model
{
	String	m_name;
	String	m_nameJP;
	String	m_filepath;
	bool	m_isUserAsset = true;
	Texture	m_assetIcon;

public:
	Asset();

	// Set
	void	setName(const String& name) { m_name = name; }
	void	setNameJP(const String& nameJP) { m_nameJP = nameJP; }
	void	setFilePath(const String& filepath) { m_filepath = filepath; }
	void	setIsUserAsset(bool isUserAsset) { m_isUserAsset = isUserAsset; }
	void	setAssetIcon(const Texture& assetIcon) { m_assetIcon = assetIcon; }

	// Get
	const Texture& getAssetIcon() const { return m_assetIcon; }
	const String& getName() const { return m_name; }
	const String& getNameJP() const { return m_nameJP; }
	const String& getFilePath() const { return m_filepath; }
	bool	getIsUserAsset() const { return m_isUserAsset; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;

	// TypeName
	virtual String	getTypeName() { return U"Asset"; }

	// parentにEditorを追加する
	virtual void	makeEditor(const std::shared_ptr<EasyViewer>& parent) const {}
};

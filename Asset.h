#pragma once

#include "Model.h"

// AssetManagerに登録可能なModel
// JSONファイルに対応している
class Asset
	: public Model
{
	String	m_name;
	String	m_nameJP;
	String	m_filepath;
	bool	m_isUserAsset =true;

public:
	Asset();

	// Set
	void	setName(const String& name) { m_name = name; }
	void	setNameJP(const String& nameJP) { m_nameJP = nameJP; }
	void	setFilePath(const String& filepath) { m_filepath = filepath; }
	void	setIsUserAsset(bool isUserAsset) { m_isUserAsset = isUserAsset; }

	// Get
	const String& getName() const { return m_name; }
	const String& getNameJP() const { return m_nameJP; }
	const String& getFilePath() const { return m_filepath; }
	bool	getIsUserAsset() const { return m_isUserAsset; }

	// Icon
	virtual void	drawIcon() const {}

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	virtual String	getTypeName() { return U"Asset"; }
};

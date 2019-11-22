#pragma once

#include "Model.h"
#include "AssetEditor.h"

// AssetManagerに登録可能なModel
// JSONファイルに対応している
class Asset
	: public Model
{
	String	m_name;
	String	m_filepath;

public:
	// Set
	void	setName(const String& name) { m_name = name; }
	void	setFilePath(const String& filepath);

	// Get
	const String& getName() const { return m_name; }
	const String& getFilePath() const { return m_filepath; }

	virtual void	makeEditor(const shared_ptr<Viewer>& parent) = 0;

	// Icon
	virtual void	drawIcon() const {}

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};

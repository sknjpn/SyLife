#pragma once

#include "Asset.h"

#include "Storage.h"

class ElementAsset
	: public Asset
{
	double	m_mass;
	double	m_radius;
	Color	m_color;
	Storage	m_material;

	class Editor
		: public AssetEditor
	{
		shared_ptr<ElementAsset>	m_elementAsset;

	public:
		Editor(const shared_ptr<ElementAsset>& elementAsset)
			: m_elementAsset(elementAsset)
		{}

		void	init() override;
		void	update() override;
	};

public:
	// Editor
	void	makeEditor(const shared_ptr<Viewer>& parent) { parent->addChildViewer<Editor>(dynamic_pointer_cast<ElementAsset>(shared_from_this())); }

	// Get
	double	getMass() const { return m_mass; }
	double	getRadius() const { return m_radius; }
	const Color& getColor() const { return m_color; }
	const Storage& getMaterial() const { return m_material; }

	// Icon
	void	drawIcon() const override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};
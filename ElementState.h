#pragma once

#include "Rigidbody.h"

class ElementAsset;

class ElementState
	: public Rigidbody
{
	shared_ptr<ElementAsset>	m_elementAsset;

public:
	ElementState(const shared_ptr<ElementAsset>& asset);
	ElementState(Deserializer<ByteArray>& reader) { load(reader); }

	// Get
	const shared_ptr<ElementAsset>& getPartAsset() const { return m_elementAsset; }

	void	updateElement();
	void	draw();

	void	load(Deserializer<ByteArray>& reader);
	void	save(Serializer<MemoryWriter>& writer) const;
};
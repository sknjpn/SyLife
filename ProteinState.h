#pragma once

#include "Rigidbody.h"

class ProteinAsset;

class ProteinState
	: public Rigidbody
{
	shared_ptr<ProteinAsset>	m_proteinAsset;

public:
	ProteinState(const shared_ptr<ProteinAsset>& asset);
	ProteinState(Deserializer<ByteArray>& reader) { load(reader); }

	// Get
	const shared_ptr<ProteinAsset>& getPartAsset() const { return m_proteinAsset; }

	void	updateProtein();
	void	draw();

	void	load(Deserializer<ByteArray>& reader);
	void	save(Serializer<MemoryWriter>& writer) const;
};
#pragma once

#include "Storage.h"

class Chip
{
	friend class ChipManager;

	Storage		m_storage;

	shared_ptr<Chip>	m_l;
	shared_ptr<Chip>	m_u;
	shared_ptr<Chip>	m_r;
	shared_ptr<Chip>	m_d;

public:
	Storage& getStorage() { return m_storage; }
	const Storage& getStorage() const { return m_storage; }

	Color getColor() const;
	void sendTo(const shared_ptr<Chip> chip, double rate);
};


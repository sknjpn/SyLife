#pragma once

class Chip
{
	friend class FieldManager;

	shared_ptr<Chip>	m_l;
	shared_ptr<Chip>	m_u;
	shared_ptr<Chip>	m_r;
	shared_ptr<Chip>	m_d;

public:

};


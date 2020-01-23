#pragma once

#include "Storage.h"

class World;

class TileState
{
	friend class World;

	double	m_element = 0.0;
	Vec2	m_waveVelocity;
	double	m_sendRate[3][3];	// 周囲のマスに送る比率

public:
	const Vec2& getWaveVelocity() const { return m_waveVelocity; }

	double	getElement() const { return m_element; }

	void	setElement(double element) { m_element = element; }
	void	addElement(double element) { m_element += element; }
	void	pullElement(double element) { m_element -= element; }

	Color	getColor() const { return Math::Lerp(Color(0, 0), Palette::Palegreen, Min(m_element / 100.0, 1.0)); }

	void	load(Deserializer<ByteArray>& reader)
	{
		reader >> m_waveVelocity;
		reader >> m_element;
	}

	void	save(Serializer<MemoryWriter>& writer) const
	{
		writer << m_waveVelocity;
		writer << m_element;
	}
};


#pragma once

class Chip;

class ChipManager
{
	Grid<shared_ptr<Chip>> m_chips;
	Size	m_chipSize;
	double	m_chipLength;

	bool	m_drawWaveEnabled = false;

	// 求まるもの
	Vec2	m_fieldSize;

public:
	ChipManager();

	Point	getPoint(const Vec2& position) const { return Point(int(position.x / m_chipLength), int(position.y / m_chipLength)); }

	shared_ptr<Chip> getChip(const Point& point) const;
	shared_ptr<Chip> getChip(const Vec2& position) const { return getChip(getPoint(position)); }

	void	generateWave();

	void	init();
	void	drawChips();
	void	updateChips();

	double	getChipLength() const { return m_chipLength; }
	const Size& getChipSize() const { return m_chipSize; }
	const Vec2& getFieldSize() const { return m_fieldSize; }

	double	getNutrition(const Vec2& position) const;
	void	addNutrition(const Vec2& position, double nutrition);
	void	pullNutrition(const Vec2& position, double nutrition);
};

extern unique_ptr<ChipManager>	g_chipManagerPtr;

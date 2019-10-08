#pragma once

class Chip;

class ChipManager
{
	Grid<shared_ptr<Chip>> m_chips;
	Rect	m_rect;
	double	m_length;

public:
	ChipManager();

	Point	getPoint(const Vec2& position) const;

	shared_ptr<Chip>& getChip(const Vec2& position) { return m_chips[getPoint(position)]; }
	const shared_ptr<Chip>& getChip(const Vec2& position) const { return m_chips[getPoint(position)]; }

	void	init();
	void	drawChips();
	void	updateChips();

	const Rect& getRect() const { return m_rect; }
	const Point& getSize() const { return m_rect.size; }

	double	getNutrition(const Vec2& position) const;
	void	pullNutrition(const Vec2& position, double nutrition);
};

extern unique_ptr<ChipManager>	g_chipManagerPtr;

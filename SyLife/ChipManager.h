#pragma once

class Chip;

class ChipManager
{
	Grid<shared_ptr<Chip>> m_chips;
	Rect	m_rect;
	double	m_length;

public:
	ChipManager();

	Point	getPoint(const Vec2& position) const { return Point(int(position.x / m_length), int(position.y / m_length)); }

	shared_ptr<Chip> getChip(const Point& point) const { return m_rect.contains(point) ? m_chips[point] : nullptr; }
	shared_ptr<Chip> getChip(const Vec2& position) const { return getChip(getPoint(position)); }

	void	init();
	void	drawChips();
	void	updateChips();

	const Rect& getRect() const { return m_rect; }
	const Point& getSize() const { return m_rect.size; }

	double	getNutrition(const Vec2& position) const;
	void	addNutrition(const Vec2& position, double nutrition);
	void	pullNutrition(const Vec2& position, double nutrition);
};

extern unique_ptr<ChipManager>	g_chipManagerPtr;

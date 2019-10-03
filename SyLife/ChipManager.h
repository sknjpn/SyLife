#pragma once

class Chip;

class ChipManager
{
	Grid<shared_ptr<Chip>> m_chips;
	Size	m_size;
	double	m_length;

public:
	ChipManager();

	Point	getPoint(const Vec2& position) const;

	shared_ptr<Chip>& getChip(const Vec2& position) { return m_chips[getPoint(position)]; }
	const shared_ptr<Chip>& getChip(const Vec2& position) const { return m_chips[getPoint(position)]; }

	void	drawChips();
	void	updateChips();
};

extern unique_ptr<ChipManager>	g_chipManagerPtr;

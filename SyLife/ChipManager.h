#pragma once

class Chip;

class ChipManager
{
	Grid<shared_ptr<Chip>> m_chips;
	Size	m_size;

public:
	shared_ptr<Chip>& GetChip(const Vec2& position);
	const shared_ptr<Chip>& GetChip(const Vec2& position) const;
};

extern unique_ptr<ChipManager>	g_chipManagerPtr;

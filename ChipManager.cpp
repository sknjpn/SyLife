#include "ChipManager.h"
#include "Chip.h"
#include "FieldViewer.h"

unique_ptr<ChipManager> g_chipManagerPtr;

ChipManager::ChipManager()
	: m_chipSize(80, 45)
	, m_chipLength(100)
	, m_chips(m_chipSize)
{
	m_fieldSize = m_chipSize * m_chipLength;
}

shared_ptr<Chip> ChipManager::getChip(const Point& point) const
{
	Point ap = point;

	if (ap.x < 0) ap.x = 0;
	if (ap.y < 0) ap.y = 0;
	if (ap.x >= m_chipSize.x) ap.x = m_chipSize.x - 1;
	if (ap.y >= m_chipSize.y) ap.y = m_chipSize.y - 1;

	return m_chips[ap];
}

void ChipManager::generateWave()
{
	const PerlinNoise perlinNoiseX(Random(0xFFFFFFFF));
	const PerlinNoise perlinNoiseY(Random(0xFFFFFFFF));
	const auto interval = 500.0;
	//const auto octave = 10;

	for (auto& chip : m_chips)
	{
		//外的圧力比
		const double rx = (chip->m_point.x - m_chipSize.x / 2.0) / (m_chipSize.x / 2.0);
		const double ry = (chip->m_point.y - m_chipSize.y / 2.0) / (m_chipSize.y / 2.0);

		const auto wx = perlinNoiseX.noise(chip->getCentroid().x / interval, chip->getCentroid().y / interval);
		const auto wy = perlinNoiseY.noise(chip->getCentroid().x / interval, chip->getCentroid().y / interval);

		// 最大の長さを1とする
		chip->m_waveVelocity = Vec2(Math::Lerp(wx, rx > 0 ? -1.0 : 1.0, EaseInExpo(Abs(rx))), Math::Lerp(wy, ry > 0 ? -1.0 : 1.0, EaseInExpo(Abs(ry)))) / Math::Sqrt2;
	}
}

void ChipManager::init()
{
	// Chips
	{
		const Size size = m_chipSize;

		m_chipSize = size;
		m_chips.resize(size);

		for (auto point : step(size))
		{
			m_chips[point] = MakeShared<Chip>(point);
			m_chips[point]->m_nutrition = 10.0;
		}
	}

	generateWave();

	for (auto& chip : m_chips)
	{
		chip->init();
	}
}

void ChipManager::drawChips()
{
	static bool showWave = false;
	if (KeyP.down()) showWave = !showWave;

	// 領域の取得
	Rect rect;
	{
		auto cameraRect = Viewer::GetRootViewer()->getChildViewer<FieldViewer>()->getCamera().getCameraRect();

		rect.pos = getPoint(cameraRect.pos);
		rect.size = getPoint(cameraRect.br()) - rect.pos;
	}

	rect = rect.stretched(1);

	for (const auto p : step(rect.pos, rect.size))
	{
		if (!Rect(m_chipSize).contains(p)) continue;

		m_chips[p]->draw();
	}
}

void ChipManager::updateChips()
{
	for (const auto& chip : m_chips)
		chip->update();
}

double ChipManager::getNutrition(const Vec2& position) const
{
	return getChip(position)->getNutrition();
}

void ChipManager::addNutrition(const Vec2& position, double nutrition)
{
	getChip(position)->addNutrition(nutrition);
}

void ChipManager::pullNutrition(const Vec2& position, double nutrition)
{
	getChip(position)->pullNutrition(nutrition);
}

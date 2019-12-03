#include "Field.h"
#include "ChipState.h"

Field::Field()
	: m_cellStateKDTree(m_cellStates)
	, m_eggStateKDTree(m_eggStates)
	, m_elementStateKDTree(m_elementStates)
	, m_chipSize(80, 45)
	, m_chipLength(100)
	, m_chips(m_chipSize)
{
	m_fieldSize = m_chipSize * m_chipLength;
	m_cellStates.reserve(0xFFFF);
	m_elementStates.reserve(0xFFFF);
	m_eggStates.reserve(0xFFFF);
}

void Field::update()
{
	// Cell
	{
		for (const auto& cellState : m_cellStates)
		{
			if (cellState->isDestroyed()) continue;

			cellState->updateParticle();
			cellState->updateRigidbody();
			cellState->updateCell();
		}

		m_cellStates.remove_if([](const auto& cellState) { return cellState->isDestroyed(); });
		m_cellStateKDTree.rebuildIndex();
	}

	// Egg
	{
		for (const auto& eggState : getEggStates())
		{
			if (eggState->isDestroyed()) continue;

			eggState->updateParticle();
			eggState->updateEgg();
		}

		m_eggStates.remove_if([](const auto& eggState) { return eggState->isDestroyed(); });
		m_eggStateKDTree.rebuildIndex();
	}

	// Element
	{
		for (const auto& elementState : getElementStates())
		{
			if (elementState->isDestroyed()) continue;

			elementState->updateParticle();
			elementState->updateElement();
		}

		// 存在しないElementの削除
		m_elementStates.remove_if([](const auto& elementState) { return elementState->isDestroyed(); });

		m_elementStateKDTree.rebuildIndex();
	}

	// Chip
	{
		for (const auto& chip : m_chips)
		{
			chip->update();
		}
	}
}

void Field::draw()
{
	static bool showWave = false;
	if (KeyP.down()) showWave = !showWave;

	for (const auto& chip : m_chips)
		chip->draw();
	for (const auto& e : getElementStates())
		if (!e->isDestroyed()) e->draw();
	for (const auto& e : getEggStates())
		if (!e->isDestroyed()) e->draw();
	for (const auto& c : m_cellStates)
		if (!c->isDestroyed()) c->draw();
}

shared_ptr<ChipState> Field::getChip(const Point& point) const
{
	Point ap = point;

	if (ap.x < 0) ap.x = 0;
	if (ap.y < 0) ap.y = 0;
	if (ap.x >= m_chipSize.x) ap.x = m_chipSize.x - 1;
	if (ap.y >= m_chipSize.y) ap.y = m_chipSize.y - 1;

	return m_chips[ap];
}

void Field::save(const FilePath& directory)
{
	JSONWriter json;

	json.startObject();

	// Cells
	{
		json.key(U"cells").startArray();
		for (const auto& cellState : m_cellStates)
		{
			json.startObject();

			cellState->save(json);

			json.endObject();
		}
		json.endArray();
	}

	// Eggs
	{
		json.key(U"eggs").startArray();
		for (const auto& eggState : m_eggStates)
		{
			json.startObject();

			eggState->save(json);

			json.endObject();
		}
		json.endArray();
	}

	// Elements
	{
		json.key(U"elements").startArray();
		for (const auto& elementState : m_elementStates)
		{
			json.startObject();

			elementState->save(json);

			json.endObject();
		}
		json.endArray();
	}

	// Chips
	{
		json.key(U"chipSize").write(m_chipSize);

		json.key(U"chips").startArray();
		for (int y = 0; y < m_chipSize.y; ++y)
		{
			json.startArray();
			for (int x = 0; x < m_chipSize.x; ++x)
			{
				json.startObject();

				const auto& chip = m_chips.at(y, x);

				chip->save(json);

				json.endObject();
			}
			json.endArray();
		}
		json.endArray();
	}

	json.endObject();
	json.save(directory + U"field.json");
}

void Field::load(const FilePath& directory)
{
	JSONReader json(directory + U"field.json");

	// Cells
	m_cellStates.clear();
	for (auto cell : json[U"cells"].arrayView())
		m_cellStates.emplace_back(MakeShared<CellState>(cell));

	// Eggs
	m_eggStates.clear();
	for (auto egg : json[U"eggs"].arrayView())
		m_eggStates.emplace_back(MakeShared<EggState>(egg));

	// Elements
	m_elementStates.clear();
	for (auto element : json[U"elements"].arrayView())
		m_elementStates.emplace_back(MakeShared<ElementState>(element));

	// Chips
	{
		m_chipSize = json[U"chipSize"].get<Size>();

		// サイズ設定と初期化
		{
			m_chips.resize(m_chipSize);

			for (auto point : step(m_chipSize))
				m_chips[point] = MakeShared<ChipState>(point);
		}

		// 読み込み
		int y = 0;
		for (auto row : json[U"chips"].arrayView())
		{
			int x = 0;

			for (auto chip : row.arrayView())
			{
				m_chips.at(y, x)->load(chip);

				++x;
			}

			++y;
		}
	}
}

void Field::generateWave(const Size& chipSize)
{
	m_chipSize = chipSize;
	m_chips.resize(chipSize);

	const PerlinNoise perlinNoiseX(Random(0xFFFFFFFF));
	const PerlinNoise perlinNoiseY(Random(0xFFFFFFFF));
	const auto interval = 500.0;

	for (auto& chip : m_chips)
	{
		//外的圧力比
		const double rx = (chip->getPoint().x - m_chipSize.x / 2.0) / (m_chipSize.x / 2.0);
		const double ry = (chip->getPoint().y - m_chipSize.y / 2.0) / (m_chipSize.y / 2.0);

		const auto wx = perlinNoiseX.noise(chip->getCentroid().x / interval, chip->getCentroid().y / interval);
		const auto wy = perlinNoiseY.noise(chip->getCentroid().x / interval, chip->getCentroid().y / interval);

		// 最大の長さを1とする
		chip->setWaveVelocity(Vec2(Math::Lerp(wx, rx > 0 ? -1.0 : 1.0, EaseInExpo(Abs(rx))), Math::Lerp(wy, ry > 0 ? -1.0 : 1.0, EaseInExpo(Abs(ry)))) / Math::Sqrt2);
	}
}

void Field::init()
{
	// Chips
	{
		const Size size = m_chipSize;

		m_chipSize = size;
		m_chips.resize(size);

		for (auto point : step(size))
		{
			m_chips[point] = MakeShared<ChipState>(point);
			m_chips[point]->setNutrition(10.0);
		}
	}

	generateWave(Size(80, 45));
}

#include "Field.h"
#include "TileState.h"

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

shared_ptr<TileState> Field::getChip(const Point& point) const
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
	Serializer<MemoryWriter> writer;

	save(writer);

	BinaryWriter binWriter(directory + U"field");
	binWriter.write(writer.getWriter().data(), writer.getWriter().size());
}

void Field::load(const FilePath& directory)
{
	Deserializer<ByteArray> reader(directory + U"field");

	load(reader);
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
			m_chips[point] = MakeShared<TileState>(point);
			m_chips[point]->setNutrition(50.0);
		}
	}

	generateWave(Size(80, 45));
}

void Field::load(Deserializer<ByteArray>& reader)
{
	{
		int cellStateSize;
		reader >> cellStateSize;
		m_cellStates.resize(cellStateSize);

		for (auto& cellState : m_cellStates) cellState = MakeShared<CellState>(reader);
	}

	{
		int eggStateSize;
		reader >> eggStateSize;
		m_eggStates.resize(eggStateSize);

		for (auto& eggState : m_eggStates) eggState = MakeShared<EggState>(reader);
	}

	{
		int elementStateSize;
		reader >> elementStateSize;
		m_elementStates.resize(elementStateSize);

		for (auto& elementState : m_elementStates) elementState = MakeShared<ElementState>(reader);
	}

	{
		Size tileStateSize;
		reader >> tileStateSize;
		m_chips.resize(tileStateSize);

		for (auto p : step(m_chips.size()))
			m_chips.at(p) = MakeShared<TileState>(p);

		for (const auto& tileState : m_chips) 
			tileState->load(reader);
	}
}

void Field::save(Serializer<MemoryWriter>& writer) const
{

	// Cells
	writer << int(m_cellStates.size());
	for (const auto& cellState : m_cellStates)
		cellState->save(writer);

	// Eggs
	writer << int(m_eggStates.size());
	for (const auto& eggState : m_eggStates)
		eggState->save(writer);

	// Elements
	writer << int(m_elementStates.size());
	for (const auto& elementState : m_elementStates)
		elementState->save(writer);

	// Chips
	writer << m_chipSize;
	for (const auto& tileState : m_chips)
		tileState->save(writer);
}

#include "PartState_Wing.h"

#include "PartAsset_Wing.h"
#include "PartConfig.h"

#include "CellState.h"

PartState_Wing::PartState_Wing(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_partAsset_Wing(std::dynamic_pointer_cast<PartAsset_Wing>(partConfig->getPartAsset()))
{}

void PartState_Wing::draw(const CellState& cellState) const
{
	auto t1 = Transformer2D(getPartConfig()->getMat3x2());
	auto t2 = Transformer2D(Mat3x2::Rotate(m_partAsset_Wing->getIsRight() ? m_p : -m_p));

	const auto& shape = getPartConfig()->getPartAsset()->getShape();

	shape.getPreRenderTexture()
		.scaled(1.0 / GeneralSetting::GetInstance().m_textureScale).draw(shape.getBoundingRect().pos, ColorF(1.0, 0.5));
}

void PartState_Wing::update(CellState& cellState)
{
	if ((m_timer++) == 60)
	{
		m_timer = 0;

		if (m_counter > 0) --m_counter;
		else
		{
			flap(cellState);
			m_v = 0.1;
		}
	}

	if ((rand() % 1000) == 0) m_counter = 3;

	m_p += m_v;
	m_p *= 0.95;
	m_v *= 0.95;
}

void PartState_Wing::flap(CellState& cellState)
{
	const double strength = m_partAsset_Wing->getStrength();
	auto centroid = getPartConfig()->getPartAsset()->getShape().getCentroid().rotated(getPartConfig()->getRotation());
	cellState.addImpulseInLocal(Vec2::Up().rotated(getPartConfig()->getRotation()) * strength, getPartConfig()->getPosition() + centroid);
}

void PartState_Wing::load(Deserializer<ByteArray>& reader)
{
	reader >> m_timer;
	reader >> m_v;
	reader >> m_p;
	reader >> m_counter;
}

void PartState_Wing::save(Serializer<MemoryWriter>& writer) const
{
	writer << m_timer;
	writer << m_v;
	writer << m_p;
	writer << m_counter;
}

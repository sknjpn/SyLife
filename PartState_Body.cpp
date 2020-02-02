#include "PartState_Body.h"
#include "PartAsset_Body.h"
#include "PartConfig.h"

PartState_Body::PartState_Body(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_partAsset_Body(std::dynamic_pointer_cast<PartAsset_Body>(partConfig->getPartAsset()))
{}

void PartState_Body::draw(const CellState& cellState) const
{
	//getPartConfig()->getPartAsset()->draw();

	m_partAsset_Body->m_texture
		.resized(m_partAsset_Body->m_texture.size() / m_partAsset_Body->m_scale)
		.drawAt(Vec2::Zero(), ColorF(1.0, 0.5));
}

void PartState_Body::update(CellState& cellState)
{
}

﻿#include "PartState_Synthesizer.h"
#include "PartAsset_Synthesizer.h"
#include "PartConfig.h"
#include "CellState.h"
#include "CellAsset.h"
#include "ProteinAsset.h"


PartState_Synthesizer::PartState_Synthesizer(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_partAsset_Synthesizer(std::dynamic_pointer_cast<PartAsset_Synthesizer>(partConfig->getPartAsset()))
{}

void PartState_Synthesizer::draw(const CellState& cellState) const
{
	auto t = Transformer2D(getPartConfig()->getMat3x2());
	
	const auto& shape = getPartConfig()->getPartAsset()->getShape();
	const double alpha = Math::Lerp(0.1, 0.5, Min(1.0, m_timer / m_partAsset_Synthesizer->getProductTime()));

	shape.getPreRenderTexture()
		.scaled(1.0 / GeneralSetting::GetInstance().m_textureScale).draw(shape.getBoundingRect().pos, ColorF(1.0, alpha));
}

void PartState_Synthesizer::update(CellState& cellState)
{
	m_timer += DeltaTime;

	if (m_timer > m_partAsset_Synthesizer->getProductTime() &&
		cellState.m_storage >= m_partAsset_Synthesizer->getExport()->getMaterial() &&
		cellState.m_cellAsset->getMaxStorage().numProtein(m_partAsset_Synthesizer->getExport()) > cellState.m_storage.numProtein(m_partAsset_Synthesizer->getExport()))
	{
		m_timer = 0.0;

		cellState.m_storage -= m_partAsset_Synthesizer->getExport()->getMaterial();
		cellState.m_storage.addProtein(m_partAsset_Synthesizer->getExport());
	}
}

void PartState_Synthesizer::load(Deserializer<ByteArray>& reader)
{
	reader >> m_timer;
}

void PartState_Synthesizer::save(Serializer<MemoryWriter>& writer) const
{
	writer << m_timer;
}

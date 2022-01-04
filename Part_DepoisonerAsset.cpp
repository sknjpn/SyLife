﻿#include "Part_DepoisonerAsset.h"
#include "Part_DepoisonerState.h"

std::shared_ptr<PartState> Part_DepoisonerAsset::makePartState(const std::shared_ptr<PartConfig>& partConfig) const
{
	return MakeShared<Part_DepoisonerState>(partConfig);
}

void Part_DepoisonerAsset::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_amount = json[U"amount"].get<double>();
}

void Part_DepoisonerAsset::save(JSONWriter& json) const
{
	PartAsset::save(json);

	json.key(U"amount").writeDouble(m_amount);
}
﻿#include "Part_NucleusAsset.h"
#include "Part_NucleusState.h"

std::shared_ptr<PartState> Part_NucleusAsset::makePartState(const std::shared_ptr<PartConfig>& partConfig) const
{
	return MakeShared<Part_NucleusState>(partConfig);
}

void Part_NucleusAsset::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_lifespanTime = json[U"lifespanTime"].get<double>();
	m_bornTime = json[U"bornTime"].get<double>();
	m_yieldTime = json[U"yieldTime"].get<double>();

	m_armor = json[U"armor"].get<int>();
}

void Part_NucleusAsset::save(JSONWriter& json) const
{
	PartAsset::save(json);

	// time
	json.key(U"lifespanTime").write(m_lifespanTime);
	json.key(U"bornTime").write(m_bornTime);
	json.key(U"yieldTime").write(m_yieldTime);

	json.key(U"armor").write(m_armor);
}
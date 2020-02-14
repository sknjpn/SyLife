#pragma once

#include "EasyViewer.h"

class ProteinAsset;

class ProteinAsset_Editor : public EasyViewer
{
	std::shared_ptr<ProteinAsset> m_proteinAsset;

public:
	ProteinAsset_Editor(const std::shared_ptr<ProteinAsset>& proteinAsset)
		: m_proteinAsset(proteinAsset)
	{}

	void	init() override;
	void	update() override;
};


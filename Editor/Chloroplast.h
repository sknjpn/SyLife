#pragma once

#include "Module.h"
#include "Viewer.h"

class ChloroplastViewer;
class ChloroplastConfig;

class ChloroplastModel
	: public ModuleModel
{
public:
	void	MakeViewers() override;
	shared_ptr<PartConfig>	MakePartConfig() const override;
};

class ChloroplastConfig
	: public ModuleConfig
{
public:
};

class ChloroplastViewer
	: public ModuleViewer
{
public:
	ChloroplastViewer(const shared_ptr<PartModel>& model)
		: ModuleViewer(model)
	{}
};
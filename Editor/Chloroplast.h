#pragma once

#include "Module.h"
#include "Viewer.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class ChloroplastViewer;
class ChloroplastConfig;

class ChloroplastModel
	: public ModuleModel
{
public:
	void	MakeViewers() override;
	shared_ptr<PartConfig>	MakeConfig() const override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	AddToJSON(ptree pt) const;
	ptree	Save() const override { return AddToJSON(ptree()); }
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

inline shared_ptr<PartConfig> ChloroplastModel::MakeConfig() const { return make_shared<ChloroplastConfig>(); }

inline void ChloroplastModel::SetFromJSON(const ptree & pt)
{
}

inline ptree ChloroplastModel::AddToJSON(ptree pt) const
{
	return ptree();
}

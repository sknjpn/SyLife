#pragma once

#include "Model.h"
#include "ModelViewer.h"
#include "Rigidbody.h"

#include "Storage.h"

class PartConfig;
class PartState;

class CellModel
	: public Model
{
	Storage	m_material;
	Array<shared_ptr<PartConfig>>	m_partConfigs;

	// Rigidbody
	double	m_mass;
	double	m_radius;
	double	m_inertia;

public:
	void	MakeViewer() override;

	// Get
	const Storage& GetMaterial() const { return m_material; }
	Storage& GetMaterial() { return m_material; }
	double	GetMass() const { return m_mass; }
	double	GetRadius() const { return m_radius; }
	double	GetInertia() const { return m_inertia; }
	const Array<shared_ptr<PartConfig>>& GetPartConfigs() const { return m_partConfigs; }
	Array<shared_ptr<PartConfig>>& GetPartConfigs() { return m_partConfigs; }

	// Add
	shared_ptr<PartConfig>& AddPartConfig();

	template <typename T>
	Array<shared_ptr<T>>	GetPartConfigs() const
	{
		Array<shared_ptr<T>> tModels;

		for (auto it = m_partConfigs.begin(); it != m_partConfigs.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tModels.emplace_back(dynamic_pointer_cast<T>(*it));

		return tModels;
	}

	void	UpdateProperties();

	// Draw
	void	Draw(double a = 0.5);

	// JSON
	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const;
	void	Save(ptree& pt) const override { Save_this(pt); }
	string	GetFilepath() const override { return "assets/models/cells/" + GetFilename(); }
};

class CellState
	: public Rigidbody
{
public:
	double	m_startTimer;
	double	m_deathTimer;
	Storage	m_storage;

	shared_ptr<CellModel>	m_model;

	Array<shared_ptr<PartState>>		m_partStates;

public:
	CellState(const shared_ptr<CellModel>& model);

	void	UpdateCell();
	void	Draw();

	void	TakeMolecule(const shared_ptr<MoleculeState>& molecule);
	void	ExpireMolecule(const shared_ptr<MoleculeModel>& model, unsigned int size = 1);
};

class CellViewer
	: public ModelViewer
{
public:
	// Reload
	void ReloadProperties_this()
	{
		ModelViewer::ReloadProperties_this();
	}
	void ReloadProperties() override { ReloadProperties_this(); }

	// Update
	void Update_this()
	{
		auto model = GetModel<CellModel>();
		static Font font(24, Typeface::Bold);

		ModelViewer::Update_this();

		MoveDrawPos(4, 0);
		font(U"Mass:").draw();
		MoveDrawPos(96, 0);
		font(ToString(model->GetMass())).draw();
		MoveDrawPos(-100, 28);

		MoveDrawPos(4, 0);
		font(U"Radius:").draw();
		MoveDrawPos(96, 0);
		font(ToString(model->GetRadius())).draw();
		MoveDrawPos(-100, 28);

		MoveDrawPos(4, 0);
		font(U"Inertia:").draw();
		MoveDrawPos(96, 0);
		font(ToString(model->GetInertia())).draw();
		MoveDrawPos(-100, 28);
	}
	void Update() override { Update_this(); }
};

inline void	CellModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<CellViewer>()->SetModel(shared_from_this());
}

#pragma once

#include "AssetModel.h"
#include "ModelEditor.h"
#include "Rigidbody.h"

#include "Storage.h"

class PartConfig;
class PartState;

class CellModel
	: public AssetModel
{
	Storage	m_material;
	Array<shared_ptr<PartConfig>>	m_partConfigs;

	// Rigidbody
	double	m_mass;
	double	m_radius;
	double	m_inertia;

public:
	void	makeViewer() override;

	// Get
	const Storage& GetMaterial() const { return m_material; }
	Storage& GetMaterial() { return m_material; }
	double	getMass() const { return m_mass; }
	double	getRadius() const { return m_radius; }
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
	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
	string	getFilepath() const override { return "assets/models/cells/" + getFilename(); }
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

	// void	TakeMolecule(const shared_ptr<MoleculeState>& molecule);
	// void	ExpireMolecule(const shared_ptr<MoleculeModel>& model, unsigned int size = 1);
};

class CellViewer
	: public ModelEditor
{
public:
	// Reload
	void reloadProperties_this()
	{
		ModelEditor::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		auto model = getModel<CellModel>();
		static Font font(24, Typeface::Bold);

		ModelEditor::Update_this();

		MoveDrawPos(4, 0);
		font(U"Mass:").draw();
		MoveDrawPos(96, 0);
		font(ToString(model->getMass())).draw();
		MoveDrawPos(-100, 28);

		MoveDrawPos(4, 0);
		font(U"Radius:").draw();
		MoveDrawPos(96, 0);
		font(ToString(model->getRadius())).draw();
		MoveDrawPos(-100, 28);

		MoveDrawPos(4, 0);
		font(U"Inertia:").draw();
		MoveDrawPos(96, 0);
		font(ToString(model->GetInertia())).draw();
		MoveDrawPos(-100, 28);
	}
	void Update() override { Update_this(); }
};

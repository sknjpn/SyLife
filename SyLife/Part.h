#pragma once

#include "ModelViewer.h"
#include "Storage.h"
#include "ShapeModel.h"
#include "PartShapeViewer.h"

class CellState;
class PartConfig;
class PartState;
class MoleculeModel;

class PartModel
	: public Model
{
public:
	friend class PartViewer;

	double	m_mass;
	Storage	m_material;
	Array<ShapeModel>	m_shapes;

public:
	// Get
	double	GetMass() const { return m_mass; }
	const Storage& GetMaterial() const { return m_material; }
	const Array<ShapeModel>& GetShapes() const { return m_shapes; }

	void MakeViewer() override;
	virtual shared_ptr<PartState>	MakeState();

	void	Draw(double a = 0.5) { for (const auto& s : m_shapes) s.Draw(a); }

	RectF	GetApproximateRect() const;
	Vec2	GetApproximateRectTopLeft() const;
	Vec2	GetApproximateRectBottomDown() const;
	double	GetRectInertia() const;
	Vec2	GetCenter() const { return (GetApproximateRectTopLeft() + GetApproximateRectBottomDown()) / 2.0; }

	// JSON
	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
	string	GetFilepath() const override { return "assets/models/parts/" + GetFilename(); }
};

class PartConfig
	: public Model
{
	shared_ptr<PartModel>	m_partModel;
	Vec2	m_position;
	double	m_rotation;

public:
	// Get
	const shared_ptr<PartModel>& GetModel() const { return m_partModel; }
	const Vec2& GetPosition() const { return m_position; }
	double	GetRotation() const { return m_rotation; }

	// Set
	void	SetModel(const shared_ptr<PartModel>& partModel) { m_partModel = partModel; }
	void	SetPosition(const Vec2& position) { m_position = position; }
	void	SetRotation(double rotation) { m_rotation = rotation; }

	double	GetInertia() const { return m_partModel->GetRectInertia() + (m_position + m_partModel->GetCenter().rotated(m_rotation)).lengthSq() * m_partModel->GetMass(); }

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};

class PartState
{
	shared_ptr<PartConfig>	m_partConfig;

public:
	virtual ~PartState() {}

	// Get
	const shared_ptr<PartConfig>& GetPartConfig() const { return m_partConfig; }

	// Set
	void	SetPartConfig(const shared_ptr<PartConfig>& partConfig) { m_partConfig = partConfig; }

	virtual void	Draw(const CellState& cell) const {}
	virtual void	Update(CellState& cell) {}
};

class PartViewer
	: public ModelViewer
{
	TextEditState		m_textEditState_mass;

public:
	// Reload
	void reloadProperties_this()
	{
		ModelViewer::reloadProperties_this();

		m_textEditState_mass.text = ToString(GetModel<PartModel>()->m_mass);
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		ModelViewer::Update_this();

		// mass
		SimpleGUI::TextBox(m_textEditState_mass, Vec2(8, 8));
		GetModel<PartModel>()->m_mass = Parse<double>(m_textEditState_mass.text);
		MoveDrawPos(0, 48);
	}
	void Update() override { Update_this(); }
};

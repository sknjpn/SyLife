#pragma once

#include "AssetModel.h"

#include "ModelEditor.h"
#include "Storage.h"
#include "ShapeModel.h"
#include "PartShapeViewer.h"

class CellState;
class PartConfig;
class PartState;

class PartModel
	: public AssetModel
{
public:
	friend class PartEditor;

	double	m_mass;
	Storage	m_material;
	Array<ShapeModel>	m_shapes;

public:
	// Get
	double	getMass() const { return m_mass; }
	const Storage& GetMaterial() const { return m_material; }
	const Array<ShapeModel>& GetShapes() const { return m_shapes; }

	void makeViewer() override;
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
	string	getFilepath() const override { return "assets/models/parts/" + getFilename(); }
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

#pragma once

class MoleculeAsset
	: public Asset
{
	friend class MoleculeViewer;

	double	m_mass;
	double	m_radius;
	Color	m_color;
	Storage	m_material;

public:
	void	makeViewer() override;

	// Get
	double	getMass() const { return m_mass; }
	double	getRadius() const { return m_radius; }
	const Color& getColor() const { return m_color; }

	// JSON
	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
	string	getFilepath() const override { return "assets/models/molecules/" + getFilename(); }
};
#pragma once

class MoleculeState;
class MoleculeModel;

struct MoleculeStateAdapter
{
	using dataset_type = Array<shared_ptr<MoleculeState>>;
	using point_type = Vec2;
	using element_type = double;
	static constexpr int32 Dimensions = 2;

	static const element_type* GetPointer(const point_type& point) { return &point.x; }
	static element_type GetElement(const dataset_type& dataset, size_t index, size_t dim);
	static element_type DistanceSq(const dataset_type& dataset, size_t index, const element_type* other);
};

// MoleculeStateÇÃï€ë∂Ç®ÇÊÇ—çXêVÇçsÇ§
class MoleculeManager
{
	Array<shared_ptr<MoleculeState>>	m_moleculeStates;
	KDTree<MoleculeStateAdapter>		m_moleculeStateKDTree;

public:
	MoleculeManager()
		: m_moleculeStateKDTree(m_moleculeStates)
	{
		m_moleculeStates.reserve(0xFFFF);
	}

	int NumMolecule(const shared_ptr<MoleculeModel>& model);

	const shared_ptr<MoleculeState>&	AddMoleculeState(const shared_ptr<MoleculeModel>& model);
	const shared_ptr<MoleculeState>&	AddMoleculeState(const shared_ptr<MoleculeModel>& model, const Vec2& position);

	void	AddMoleculesRandom(const shared_ptr<MoleculeModel>& model, size_t size);

	// Get
	Array<shared_ptr<MoleculeState>>&		GetMoleculeStates() { return m_moleculeStates; }
	const Array<shared_ptr<MoleculeState>>&	GetMoleculeStates() const { return m_moleculeStates; }
	const KDTree<MoleculeStateAdapter>&		GetMoleculeStateKDTree() const { return m_moleculeStateKDTree; }

	void	UpdateMoleculeStates();
	void	DrawMoleculeStates();
};

extern unique_ptr<MoleculeManager> g_moleculeManagerPtr;
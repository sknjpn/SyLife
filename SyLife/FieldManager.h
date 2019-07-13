#pragma once

class Rigidbody;

struct RigidbodyAdapter
{
	using dataset_type = Array<shared_ptr<Rigidbody>>;
	using point_type = Vec2;
	using element_type = double;
	static constexpr int32 Dimensions = 2;

	static const element_type* GetPointer(const point_type& point) { return &point.x; }
	static element_type GetElement(const dataset_type& dataset, size_t index, size_t dim);
	static element_type DistanceSq(const dataset_type& dataset, size_t index, const element_type* other);
};

class FieldManager
{
	Array<shared_ptr<Rigidbody>>	m_rigidbodies;
	KDTree<RigidbodyAdapter>		m_rigidbodyKDTree;

public:
	FieldManager()
		: m_rigidbodyKDTree(m_rigidbodies)
	{
		m_rigidbodies.reserve(0xFFFF);
	}

	// Get
	Array<shared_ptr<Rigidbody>>&			GetRigidbody() { return m_rigidbodies; }
	const Array<shared_ptr<Rigidbody>>&		GetRigidbody() const { return m_rigidbodies; }
	const KDTree<RigidbodyAdapter>&			GetRigidbodyKDTree() const { return m_rigidbodyKDTree; }

	constexpr double	GetDeltaTime() const { return 1.0 / 60.0; }

	void	Init();
	void	Update();
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;

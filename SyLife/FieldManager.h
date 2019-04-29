#pragma once

class Rigidbody;

class FieldManager
{
public:
	vector<shared_ptr<Rigidbody>>	m_rigidbodies;
	Vector2D	m_size;

public:
	FieldManager();
	~FieldManager();

	void	Init();
	void	Update();

	double	GetDeltaTime() const { return 1.0 / 60.0; }
};

extern unique_ptr<FieldManager>	g_fieldManager;
#pragma once

class Rigidbody;

class FieldManager
{
public:
	vector<shared_ptr<Rigidbody>>	m_rigidbodies;

public:
	FieldManager();
	~FieldManager();

	void	Init();
	void	Update();
};

extern unique_ptr<FieldManager>	g_fieldManager;
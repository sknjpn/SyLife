#pragma once

class FieldManager
{
	Camera2D	m_camera;

public:
	// Get
	Camera2D&		GetCamera2D() { return m_camera; }
	const Camera2D&	GetCamera2D() const { return m_camera; }

	void	Init();
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;

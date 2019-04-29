#pragma once

class Molecule;
class Cell;

class FieldManager
{
public:
	FieldManager();
	~FieldManager();

	void	Init();
	void	Update();
};

extern unique_ptr<FieldManager>	g_fieldManager;
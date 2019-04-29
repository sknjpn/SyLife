#pragma once

class Field
{
public:
	Field();
	~Field();

	void	Init();
	void	Update();
};

extern unique_ptr<Field>	g_field;
#pragma once

class Field
{
public:
	Field();
	~Field();

	void	Start();
};

extern unique_ptr<Field>	g_field;
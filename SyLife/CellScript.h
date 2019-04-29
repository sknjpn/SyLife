#pragma once

class CellScript
{
	sol::state	state;

public:
	CellScript();
	~CellScript();

	void	Run();
};


#pragma once

// Engineの共通している基盤の共有
class SystemManager
{
public:
	SystemManager();

	constexpr double	GetDeltaTime() const { return 1.0 / 60.0; }

	void	update();
};

extern unique_ptr<SystemManager>	g_systemManagerPtr;

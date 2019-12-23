#pragma once

#include "stdafx.h"

// シミュレーションの状態を決めるもの
class State
{
public:
	// JSON
	virtual void	load(const JSONValue& json) {}
	virtual void	save(JSONWriter& json) const {}
};

#pragma once

#include "stdafx.h"

class Model
	: public enable_shared_from_this<Model>
{
public:
	virtual ~Model() = default;

	// JSON
	virtual void	load(const JSONValue& json) {}
	virtual void	save(JSONWriter& json) const {}
};

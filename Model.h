#pragma once

#include "Viewer.h"

class Model
	: public enable_shared_from_this<Model>
{
public:
	virtual ~Model() = default;

	virtual void	makeViewer() {}

	// JSON
	virtual void	load(const JSONValue& json) {}
	virtual void	save(const JSONWriter& json) const {}
};

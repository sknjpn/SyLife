#pragma once

class Model
{
public:
	virtual ~Model() = default;

	virtual string	ToJson() const { return "{}"; }
};


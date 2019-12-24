#pragma once

#include "Viewer.h"

class MagnifyingViewer
	: public Viewer
{
public:
	void init() override;
	void update() override;
};

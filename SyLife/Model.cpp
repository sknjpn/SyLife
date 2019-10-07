#include "stdafx.h"
#include "Model.h"

void Model::load_this(const ptree& pt)
{

}

void Model::save_this(ptree& pt) const
{
	// type
	pt.put("type", "Model");
}

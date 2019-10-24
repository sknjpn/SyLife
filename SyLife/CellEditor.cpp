#include "CellEditor.h"
#include "CellAsset.h"

void CellEditor::reloadProperties_this()
{
	ModelEditor::reloadProperties_this();
}

void CellEditor::update_this()
{
	auto asset = getModel<CellAsset>();
	static Font font(24, Typeface::Bold);

	ModelEditor::update_this();

	moveDrawPos(4, 0);
	font(U"Mass:").draw();
	moveDrawPos(96, 0);
	font(ToString(asset->getMass())).draw();
	moveDrawPos(-100, 28);

	moveDrawPos(4, 0);
	font(U"Radius:").draw();
	moveDrawPos(96, 0);
	font(ToString(asset->getRadius())).draw();
	moveDrawPos(-100, 28);

	moveDrawPos(4, 0);
	font(U"Inertia:").draw();
	moveDrawPos(96, 0);
	font(ToString(asset->getInertia())).draw();
	moveDrawPos(-100, 28);
}

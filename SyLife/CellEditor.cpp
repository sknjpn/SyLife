#include "CellEditor.h"
#include "CellAsset.h"

void CellEditor::reloadProperties_this()
{
	ModelEditor::reloadProperties_this();
}

void CellEditor::update_this()
{
	auto model = getModel<CellAsset>();
	static Font font(24, Typeface::Bold);

	ModelEditor::update_this();

	MoveDrawPos(4, 0);
	font(U"Mass:").draw();
	MoveDrawPos(96, 0);
	font(ToString(model->getMass())).draw();
	MoveDrawPos(-100, 28);

	MoveDrawPos(4, 0);
	font(U"Radius:").draw();
	MoveDrawPos(96, 0);
	font(ToString(model->getRadius())).draw();
	MoveDrawPos(-100, 28);

	MoveDrawPos(4, 0);
	font(U"Inertia:").draw();
	MoveDrawPos(96, 0);
	font(ToString(model->getInertia())).draw();
	MoveDrawPos(-100, 28);
}

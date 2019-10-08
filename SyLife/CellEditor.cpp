#include "CellEditor.h"
#include "CellAsset.h"

void CellEditor::reloadProperties_this()
{
	ModelEditor::reloadProperties_this();
}

void CellEditor::Update_this()
{
	auto model = getModel<CellAsset>();
	static Font font(24, Typeface::Bold);

	ModelEditor::Update_this();

	MovedrawPos(4, 0);
	font(U"Mass:").draw();
	MovedrawPos(96, 0);
	font(ToString(model->getMass())).draw();
	MovedrawPos(-100, 28);

	MovedrawPos(4, 0);
	font(U"Radius:").draw();
	MovedrawPos(96, 0);
	font(ToString(model->getRadius())).draw();
	MovedrawPos(-100, 28);

	MovedrawPos(4, 0);
	font(U"Inertia:").draw();
	MovedrawPos(96, 0);
	font(ToString(model->getInertia())).draw();
	MovedrawPos(-100, 28);
}

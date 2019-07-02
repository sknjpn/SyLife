#pragma once

#include "Viewer.h"
#include "ShapeModel.h"
#include "TinyCamera.h"
#include "Part.h"

class PartShapeViewer
	: public Viewer
{
public:
	shared_ptr<PartModel>	m_model;

	Array<Vec2>	m_verticles;

	Vec2*		m_selectedVerticle = nullptr;

	CursorCamera2D	m_camera;

public:
	PartShapeViewer(const shared_ptr<PartModel>& model)
		: m_model(model)
	{
		m_drawRect = RectF(0, 0, 600, 450);

		m_camera.setTargetCenter(Window::BaseSize() / 2 - m_drawRect.size / 2);
		m_camera.setCenter(Window::BaseSize() / 2 - m_drawRect.size / 2);
	}

	void	Init() override;
	void	Update(bool isMouseOver) override;
};


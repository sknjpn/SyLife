#pragma once

#include "EasyViewer.h"

class HiddenViewer : public EasyViewer
{
	Vec2	m_firstPosInLocal;
	Vec2	m_secondPosInLocal;

	enum class Mode
	{
		None,
		MoveToFirstPos,
		MoveToSecondPos,
	} m_mode = Mode::None;

public:
	std::shared_ptr<HiddenViewer> setFirstPosInLocal(double x, double y) { return setFirstPosInLocal(Vec2(x, y)); }
	std::shared_ptr<HiddenViewer> setFirstPosInLocal(const Vec2& pos)
	{
		if (m_mode == Mode::None)
		{
			m_mode = Mode::MoveToFirstPos;
			setViewerPosInLocal(pos);
		}

		m_firstPosInLocal = pos; return std::dynamic_pointer_cast<HiddenViewer>(shared_from_this());
	}

	std::shared_ptr<HiddenViewer> setSecondPosInLocal(double x, double y) { return setSecondPosInLocal(Vec2(x, y)); }
	std::shared_ptr<HiddenViewer> setSecondPosInLocal(const Vec2& pos) { m_secondPosInLocal = pos; return std::dynamic_pointer_cast<HiddenViewer>(shared_from_this()); }

	void moveToFirstPos() { m_mode = Mode::MoveToFirstPos; }
	void moveToSecondPos() { m_mode = Mode::MoveToSecondPos; }

	void update() override
	{
		const Vec2 posInLocal = getViewerPosInLocal();
		const double rate = 0.1;

		switch (m_mode)
		{
		case HiddenViewer::Mode::None:
			break;
		case HiddenViewer::Mode::MoveToFirstPos:
		{
			setViewerPosInLocal(posInLocal.lerp(m_firstPosInLocal, rate));
		}
		break;
		case HiddenViewer::Mode::MoveToSecondPos:
		{
			setViewerPosInLocal(posInLocal.lerp(m_firstPosInLocal, rate));
		}
		break;
		default:
			break;
		}
	}
};
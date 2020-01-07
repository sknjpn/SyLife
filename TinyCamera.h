# pragma once

/* プリコンパイルでSiv3D.hppを読み込む
# include <Siv3D.hpp> // OpenSiv3D v0.2.6
*/

// 表示領域からTransformerを作成できる
class BasicCamera
{
protected:
	RectF	m_screen = Scene::Rect();	// 投影先の領域
	Vec2	m_center = Scene::Center();	// 切り取り領域の中心
	double	m_scale = 1.0;				// 切り取り領域の拡大率

public:
	BasicCamera() = default;

	BasicCamera(const Vec2& center, double scale)
		: m_center(center)
		, m_scale(scale)
	{}

	// 描画対象の切り取り領域を返す
	[[nodiscard]] RectF getCameraRect() const { return RectF(m_screen.size / m_scale).setCenter(m_center); }

	[[nodiscard]] Mat3x2 getCursorMat3x2() const { return Mat3x2::Translate(-m_center).scaled(m_scale).translated(m_screen.center()); }
	[[nodiscard]] Mat3x2 getGraphics2DMat3x2() const { return Mat3x2::Translate(-m_center).scaled(m_scale).translated(m_screen.size / 2.0); }
	[[nodiscard]] ScopedViewport2D createScopedViewport() const { return ScopedViewport2D(Rect(m_screen)); }
	[[nodiscard]] Transformer2D createTransformer() const { return Transformer2D(getGraphics2DMat3x2(), getCursorMat3x2()); }

	// 描画対象の切り取り領域を設定する
	void		setCameraRect(const RectF& rect)
	{
		setCenter(rect.center());
		setScale(Max(Scene::Width() / rect.w, Scene::Height() / rect.h));
	}

	// 描画対象の切り取り領域の中心を設定する
	void		setCenter(const Vec2& center) { m_center = center; }

	// 投影する領域を設定する
	void		setScreen(const RectF& screen) { m_screen = screen; }

	void		setScale(double scale) { m_scale = scale; }

	// 描画対象の切り取り領域の中心を返す
	const Vec2& getCentroid() const noexcept { return m_center; }
	const RectF& getScreen() const noexcept { return m_screen; }

	double		getScale() const noexcept { return m_scale; }
};

class TinyCamera
	: public BasicCamera
{
	// 追従目標の値
	Vec2		m_targetCenter = Scene::Size() * 0.5;
	double		m_targetScale = 1.0;

	// 追従速度 (0.0～1.0)
	double		m_followingSpeed = 0.25;

	// 拡大の感度 (0.0～)
	double		m_scalingSensitivity = 0.1;

	// 移動の速度 (dot per frame)
	double		m_movingSensitivity = 0.01;

	// 画面外でもコントロールを有効にする
	bool		m_controlOutOfScreenEnabled = false;

	std::function<double()> m_wheelControl = []() { return Mouse::Wheel(); };

	// 移動に対するコントロール条件
	std::array<std::function<bool()>, 4> m_controls =
	{
		[]() { return KeyW.pressed(); },
		[]() { return KeyA.pressed(); },
		[]() { return KeyS.pressed(); },
		[]() { return KeyD.pressed(); },
	};

	// 最小最大の拡大率
	Optional<double>	m_minScale = 1.0;
	Optional<double>	m_maxScale = 8.0;

	// 切り取り領域の制限範囲
	Optional<RectF>		m_restrictedRect = RectF(Scene::Rect());

	// 拡大縮小を行う
	void magnify()
	{
		const auto delta = 1.0 + m_scalingSensitivity * m_wheelControl();
		const auto cursorPos = (Cursor::PosF() - m_screen.center()) / m_targetScale + m_targetCenter;

		m_targetScale /= delta;
		m_targetCenter = (m_targetCenter - cursorPos) * delta + cursorPos;
	}

	// 中心の移動を行う
	void move()
	{
		if (m_controls[0]()) { m_targetCenter.y -= m_movingSensitivity * m_screen.h / m_targetScale; }
		if (m_controls[1]()) { m_targetCenter.x -= m_movingSensitivity * m_screen.w / m_targetScale; }
		if (m_controls[2]()) { m_targetCenter.y += m_movingSensitivity * m_screen.h / m_targetScale; }
		if (m_controls[3]()) { m_targetCenter.x += m_movingSensitivity * m_screen.w / m_targetScale; }
	}

	void follow()
	{
		m_center = Math::Lerp(m_center, m_targetCenter, m_followingSpeed);
		m_scale = 1.0 / Math::Lerp(1.0 / m_scale, 1.0 / m_targetScale, m_followingSpeed);
	}

	void	restrictScale()
	{
		if (m_minScale) m_scale = Max(m_scale, m_minScale.value());
		if (m_maxScale) m_scale = Min(m_scale, m_maxScale.value());

		if (m_restrictedRect) m_scale = Max({ m_scale, m_screen.h / m_restrictedRect.value().h, m_screen.w / m_restrictedRect.value().w });
	}

	void	restrictRect()
	{
		if (!m_restrictedRect || m_restrictedRect.value().contains(getCameraRect())) return;

		const auto tl = m_restrictedRect.value().tl() - getCameraRect().tl();
		const auto br = m_restrictedRect.value().br() - getCameraRect().br();

		if (tl.x > 0) { m_center.moveBy(tl.x, 0); }
		if (tl.y > 0) { m_center.moveBy(0, tl.y); }
		if (br.x < 0) { m_center.moveBy(br.x, 0); }
		if (br.y < 0) { m_center.moveBy(0, br.y); }
	}

	void	restrictTargetScale()
	{
		if (m_minScale) m_targetScale = Max(m_targetScale, m_minScale.value());
		if (m_maxScale) m_targetScale = Min(m_targetScale, m_maxScale.value());

		if (m_restrictedRect) m_targetScale = Max({ m_targetScale, m_screen.h / m_restrictedRect.value().h, m_screen.w / m_restrictedRect.value().w });
	}

	void	restrictTargetRect()
	{
		if (!m_restrictedRect || m_restrictedRect.value().contains(getTargetCameraRect())) return;

		const auto tl = m_restrictedRect.value().tl() - getTargetCameraRect().tl();
		const auto br = m_restrictedRect.value().br() - getTargetCameraRect().br();

		if (tl.x > 0) { m_targetCenter.moveBy(tl.x, 0); }
		if (tl.y > 0) { m_targetCenter.moveBy(0, tl.y); }
		if (br.x < 0) { m_targetCenter.moveBy(br.x, 0); }
		if (br.y < 0) { m_targetCenter.moveBy(0, br.y); }
	}

public:
	TinyCamera() = default;

	void update(bool controlEnabled = true)
	{
		if (controlEnabled && (m_controlOutOfScreenEnabled || m_screen.mouseOver()))
		{
			magnify();

			restrictTargetScale();

			move();

			restrictTargetRect();
		}

		follow();
	}

	// 切り取り領域の制限領域の設定
	void	setRestrictedRect(Optional<RectF> restrictedRect)
	{
		m_restrictedRect = restrictedRect;

		restrictScale();
		restrictRect();
		restrictTargetScale();
		restrictTargetRect();
	}

	// 最大拡大率の設定
	void	setMaxScale(Optional<double> maxScale)
	{
		m_maxScale = maxScale;

		restrictScale();
		restrictRect();
		restrictTargetScale();
		restrictTargetRect();
	}

	// 最小拡大率の設定
	void	setMinScale(Optional<double> minScale)
	{
		m_minScale = minScale;

		restrictScale();
		restrictRect();
		restrictTargetScale();
		restrictTargetRect();
	}

	[[nodiscard]] Optional<RectF>	getRestrictedRect() const noexcept { return m_restrictedRect; }
	[[nodiscard]] Optional<double>	getMinScale() const noexcept { return m_minScale; }
	[[nodiscard]] Optional<double>	getMaxScale() const noexcept { return m_maxScale; }

	void	setFollowingSpeed(double followingSpeed) noexcept { m_followingSpeed = followingSpeed; }
	void	setMagnifyingSensitivity(double scalingSensitivity) noexcept { m_scalingSensitivity = scalingSensitivity; }
	void	setMovingSensitivity(double movingSensitivity) noexcept { m_movingSensitivity = movingSensitivity; }
	void	setControls(const std::array<std::function<bool()>, 4>& controls) noexcept { m_controls = controls; }

	void	setTargetCenter(const Vec2& targetCenter) noexcept { m_targetCenter = targetCenter; }
	void	setTargetScale(double targetScale) noexcept { m_targetScale = targetScale; }
	void	setControlOutOfScreenEnabled(bool enabled) noexcept { m_controlOutOfScreenEnabled = enabled; }

	RectF	getTargetCameraRect() const { return RectF(m_screen.size / m_targetScale).setCenter(m_targetCenter); }

	void moveU() { m_targetCenter.y -= m_movingSensitivity * m_screen.h / m_targetScale; restrictTargetRect(); }
	void moveL() { m_targetCenter.x -= m_movingSensitivity * m_screen.w / m_targetScale; restrictTargetRect(); }
	void moveD() { m_targetCenter.y += m_movingSensitivity * m_screen.h / m_targetScale; restrictTargetRect(); }
	void moveR() { m_targetCenter.x += m_movingSensitivity * m_screen.w / m_targetScale; restrictTargetRect(); }

	void zoomIn(double d = 0.2)
	{
		const auto delta = 1.0 + m_scalingSensitivity * (-d);
		const auto cursorPos = Vec2::Zero() / m_targetScale + m_targetCenter;

		m_targetScale /= delta;
		m_targetCenter = (m_targetCenter - cursorPos) * delta + cursorPos;

		restrictTargetScale();
	}
	void zoomOut(double d = 0.2)
	{
		const auto delta = 1.0 + m_scalingSensitivity * d;
		const auto cursorPos = Vec2::Zero() / m_targetScale + m_targetCenter;

		m_targetScale /= delta;
		m_targetCenter = (m_targetCenter - cursorPos) * delta + cursorPos;

		restrictTargetScale();
	}
};

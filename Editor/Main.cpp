struct Shape
{
	s3d::HSV		m_color;
	s3d::Array<s3d::Vec2>	m_verticles;

	Shape(s3d::Array<s3d::Vec2> verticle)
		: m_verticles(verticle)
		, m_color(s3d::RandomColor())
	{}
};

void Main()
{
	// 最終的な図形
	s3d::Array<Shape> shapes;

	// 新規作成時の図形
	s3d::Array<s3d::Vec2> verticles;

	s3d::Vec2* selectedVerticle = nullptr;

	while (s3d::System::Update())
	{
		s3d::ClearPrint();

		int scale = 10;

		const double thickness = 3.0 / scale;
		const s3d::Vec2 boxSize = s3d::Vec2(5.0, 5.0) / scale;
		auto t = s3d::Transformer2D(s3d::Mat3x2::Scale(scale).translated(s3d::Window::Size() / 2.0), true);

		s3d::Line(-s3d::Window::Size().x, 0, s3d::Window::Size().x, 0).draw(2.0 / scale, s3d::Palette::Red);
		s3d::Line(0, -s3d::Window::Size().y, 0, s3d::Window::Size().y).draw(2.0 / scale, s3d::Palette::Red);

		for (int x = -s3d::Window::Size().x / scale; x < s3d::Window::Size().x / scale; x += 1) s3d::Line(x, -s3d::Window::Size().y, x, s3d::Window::Size().y).draw(1.0 / scale, s3d::ColorF(1.0, 0.25));
		for (int y = -s3d::Window::Size().y / scale; y < s3d::Window::Size().y / scale; y += 1) s3d::Line(-s3d::Window::Size().x, y, s3d::Window::Size().x, y).draw(1.0 / scale, s3d::ColorF(1.0, 0.25));

		s3d::Point cursor = s3d::Cursor::Pos();
		s3d::RectF pointer(s3d::Arg::center(cursor), boxSize);

		s3d::Print << cursor;

		// Draw
		{
			for (const auto& s : shapes)
			{
				// Face
				{
					s3d::Polygon p(s.m_verticles);

					p.draw(s3d::ColorF(s.m_color, 0.5));
				}

				// Line
				for (auto it = s.m_verticles.begin(); it != s.m_verticles.end(); ++it)
				{
					s3d::Line l = (it == s.m_verticles.end() - 1) ? s3d::Line(s.m_verticles.front(), s.m_verticles.back()) : s3d::Line(*it, *(it + 1));

					l.draw(thickness, s3d::ColorF(s.m_color, 0.5));
				}

				// Verticle
				for (auto it = verticles.begin(); it != verticles.end(); ++it)
				{
					auto r = s3d::RectF(s3d::Arg::center(*it), boxSize);

					r.draw(r.mouseOver() ? s3d::Palette::Blue : s.m_color);
				}
			}

			if (verticles.empty())
			{
				// Pointer
				pointer.draw(s3d::Palette::Red);
			}
			else
			{
				// Line
				for (auto it = verticles.begin(); it < verticles.end() - 1; ++it)
					s3d::Line(*it, *(it + 1)).draw(thickness, s3d::ColorF(s3d::Palette::White, 0.5));

				// Last Line
				s3d::Line(verticles.back(), cursor).draw(thickness, s3d::ColorF(s3d::Palette::Gray, 0.5));

				// Verticle
				for (auto it = verticles.begin(); it != verticles.end(); ++it)
				{
					auto r = s3d::RectF(s3d::Arg::center(*it), boxSize);

					r.draw(r.mouseOver() ? s3d::Palette::Blue : s3d::Palette::White);
				}

				// Pointer
				//pointer.draw(s3d::Palette::Red);
			}
		}

		// Update
		{
			if (verticles.empty())
			{
				for(auto it = shapes.rbegin(); it != shapes.rend(); ++it)
				{
					if (s3d::Polygon((*it).m_verticles).mouseOver())
					{
						(*it).m_color.h += s3d::Mouse::Wheel();

						break;
					}
				}

				// 新規配置
				if (s3d::MouseL.down()) verticles.emplace_back(cursor);
			}
			else
			{
				//Verticle
				{
					bool f = false;

					for (auto it = verticles.begin(); it != verticles.end(); ++it)
					{
						auto r = s3d::RectF(s3d::Arg::center(*it), boxSize);

						if (r.leftClicked())
						{
							// Connect
							if (it != verticles.begin()) verticles.erase(verticles.begin(), it);

							shapes.emplace_back(verticles);

							verticles.clear();

							f = true;
							break;
						}
					}

					if (f) continue;
				}

				// 連続配置
				if (s3d::MouseL.down()) verticles.emplace_back(cursor);
				if (s3d::MouseR.down()) verticles.pop_back();
			}
		}

		// Save
		{
			s3d::String s;

			s += U"[";
			s += U"\r  ";
			for (auto it = shapes.begin(); it != shapes.end(); ++it)
			{
				s += U"{\r    ";
				{
					s += s3d::Format(U"\"color\": [", (*it).m_color.toColor().r, U", ", (*it).m_color.toColor().g, U", ", (*it).m_color.toColor().b, U"]");
				}
				s += U",";
				s += U"\r    ";

				{
					s += U"\"verticles\": [";
					for (auto v = (*it).m_verticles.begin(); v != (*it).m_verticles.end(); ++v)
					{
						s += s3d::Format(U"[", (*v).x, U", ", (*v).y, U"]");

						if (v != (*it).m_verticles.end() - 1) s += U", ";
					}
					s += U"]";

					s += U"\r  ";
				}
				s += U"}";

				if (it != shapes.end() - 1) s += U",\r  ";
				else s += U"\r";
			}
			s += U"]";

			s3d::TextWriter tw(U"a.json");

			tw.write(s);
			tw.close();
		}
	}
}

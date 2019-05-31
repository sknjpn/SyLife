struct Shape
{
	HSV		m_color;
	Array<Vec2>	m_verticles;

	Shape(Array<Vec2> verticle)
		: m_verticles(verticle)
		, m_color(RandomColor())
	{}
};

void Main()
{
	// 最終的な図形
	Array<Shape> shapes;

	// 新規作成時の図形
	Array<Vec2> verticles;

	Vec2* selectedVerticle = nullptr;

	while (System::Update())
	{
		ClearPrint();

		int scale = 10;

		const double thickness = 3.0 / scale;
		const Vec2 boxSize = Vec2(5.0, 5.0) / scale;
		auto t = Transformer2D(Mat3x2::Scale(scale).translated(Window::Size() / 2.0), true);

		Line(-Window::Size().x, 0, Window::Size().x, 0).draw(2.0 / scale, Palette::Red);
		Line(0, -Window::Size().y, 0, Window::Size().y).draw(2.0 / scale, Palette::Red);

		for (int x = -Window::Size().x / scale; x < Window::Size().x / scale; x += 1) Line(x, -Window::Size().y, x, Window::Size().y).draw(1.0 / scale, ColorF(1.0, 0.25));
		for (int y = -Window::Size().y / scale; y < Window::Size().y / scale; y += 1) Line(-Window::Size().x, y, Window::Size().x, y).draw(1.0 / scale, ColorF(1.0, 0.25));

		Point cursor = Cursor::Pos();
		RectF pointer(Arg::center(cursor), boxSize);

		Print << cursor;

		// Draw
		{
			for (const auto& s : shapes)
			{
				// Face
				{
					Polygon p(s.m_verticles);

					p.draw(ColorF(s.m_color, 0.5));
				}

				// Line
				for (auto it = s.m_verticles.begin(); it != s.m_verticles.end(); ++it)
				{
					Line l = (it == s.m_verticles.end() - 1) ? Line(s.m_verticles.front(), s.m_verticles.back()) : Line(*it, *(it + 1));

					l.draw(thickness, ColorF(s.m_color, 0.5));
				}

				// Verticle
				for (auto it = verticles.begin(); it != verticles.end(); ++it)
				{
					auto r = RectF(Arg::center(*it), boxSize);

					r.draw(r.mouseOver() ? Palette::Blue : s.m_color);
				}
			}

			if (verticles.empty())
			{
				// Pointer
				pointer.draw(Palette::Red);
			}
			else
			{
				// Line
				for (auto it = verticles.begin(); it < verticles.end() - 1; ++it)
					Line(*it, *(it + 1)).draw(thickness, ColorF(Palette::White, 0.5));

				// Last Line
				Line(verticles.back(), cursor).draw(thickness, ColorF(Palette::Gray, 0.5));

				// Verticle
				for (auto it = verticles.begin(); it != verticles.end(); ++it)
				{
					auto r = RectF(Arg::center(*it), boxSize);

					r.draw(r.mouseOver() ? Palette::Blue : Palette::White);
				}

				// Pointer
				//pointer.draw(Palette::Red);
			}
		}

		// Update
		{
			if (verticles.empty())
			{
				for(auto it = shapes.rbegin(); it != shapes.rend(); ++it)
				{
					if (Polygon((*it).m_verticles).mouseOver())
					{
						(*it).m_color.h += Mouse::Wheel();

						break;
					}
				}

				// 新規配置
				if (MouseL.down()) verticles.emplace_back(cursor);
			}
			else
			{
				//Verticle
				{
					bool f = false;

					for (auto it = verticles.begin(); it != verticles.end(); ++it)
					{
						auto r = RectF(Arg::center(*it), boxSize);

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
				if (MouseL.down()) verticles.emplace_back(cursor);
				if (MouseR.down()) verticles.pop_back();
			}
		}

		// Save
		{
			String s;

			s += U"[";
			s += U"\r  ";
			for (auto it = shapes.begin(); it != shapes.end(); ++it)
			{
				s += U"{\r    ";
				{
					s += Format(U"\"color\": [", (*it).m_color.toColor().r, U", ", (*it).m_color.toColor().g, U", ", (*it).m_color.toColor().b, U"]");
				}
				s += U",";
				s += U"\r    ";

				{
					s += U"\"verticles\": [";
					for (auto v = (*it).m_verticles.begin(); v != (*it).m_verticles.end(); ++v)
					{
						s += Format(U"[", (*v).x, U", ", (*v).y, U"]");

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

			TextWriter tw(U"a.json");

			tw.write(s);
			tw.close();
		}
	}
}

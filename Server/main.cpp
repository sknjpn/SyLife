#include <cstdio>
#include <unistd.h>

#include "Vector2D.h"
#include <vector>
#include <random>

#include <string>
using namespace std;

class Particle
{
public:
	Vector2D	m_position;
	Vector2D	m_velocity;

public:
	Particle()
		: m_position((rand() % 100) / 100.0, (rand() % 100) / 100.0)
		, m_velocity((rand() % 100) / 100.0, (rand() % 100) / 100.0)
	{}

	string	GetString() const { return to_string(m_position.m_x) + ", " + to_string(m_position.m_y); }
};


int main()
{
	vector<Particle> particles;
	for (int i = 0; i < 5; i++)
		particles.emplace_back();

	printf("hello from Server!\n");

	for (;;)
	{
		for (auto& p : particles)
		{
			p.m_position = p.m_position + p.m_velocity;
		}


		string t;
		for (const auto& p : particles)
		{
			t += "{";
			t += p.GetString();
			t += "}";
		}
		t += "\n";

		printf(t.c_str());

		sleep(1.0f);
	}

	return 0;
}
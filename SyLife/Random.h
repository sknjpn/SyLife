#pragma once

double	Random(double min, double max);
double	Random(double max) { Random(0.0, max); }
bool	RandomBool(double a = 0.5);
int		Random(int min, int max);
int		Random(int max) { Random(0, max); }
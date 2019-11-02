#pragma once
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include "vector.h"

class Sphere
{
public:
	double r;
	double mass;
	vec3 position;
	vec3 velocity;

public:
	Sphere(vec3 pos, double init_r)
	{
		position = pos;
		r = init_r;
		mass = 10;
		velocity.setZeroVector();
	}
};
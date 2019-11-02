

#pragma once

#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "sphere.h"
#include "cloth.h"
#include "vector.h"
#include <GL/glm/vec4.hpp>

using namespace std;


class Simulator
{
public:
	Simulator(void);
	~Simulator(void);

public:
	void					Initialize(void);
 	void					Update();
	void					Render();
	void					Lighting(void);
	void					DrawGround(void);
	void					DrawSphere(void);
	bool					LoadMeshFromFile(char* filename, char* texFile);

public:
	mass_cloth			*cloth;
	vec3				ground;
	Sphere				*sphere;
	float timeStep;
	GLuint mTexture;
};

